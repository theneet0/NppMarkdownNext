# Install a verified local package, or restore one of its backups.
[CmdletBinding(DefaultParameterSetName='Install')]
param(
    [Parameter(Mandatory,ParameterSetName='Install')][string]$PackagePath,
    [Parameter(Mandatory,ParameterSetName='Rollback')][string]$RollbackPath,
    [string]$NppDirectory = 'C:\Program Files\Notepad++',
    [switch]$DryRun,
    [switch]$Force
)
$ErrorActionPreference = 'Stop'
$script:InstallerEntryPath = $PSCommandPath
$installerLoadedSource = $MyInvocation.MyCommand.ScriptBlock.Ast.Extent.Text
Add-Type -AssemblyName System.IO.Compression.FileSystem
Add-Type -AssemblyName System.IO.Compression

function Get-InstallerHash([byte[]]$Bytes) {
    $sha = [Security.Cryptography.SHA256]::Create()
    try { [BitConverter]::ToString($sha.ComputeHash($Bytes)).Replace('-','').ToLowerInvariant() }
    finally { $sha.Dispose() }
}
$script:InstallerSourceSha256 = Get-InstallerHash ([Text.Encoding]::UTF8.GetBytes($installerLoadedSource.TrimStart([char]0xfeff).Replace("`r`n","`n").Replace("`r","`n")))
function Resolve-InstallerFileSystemPath([string]$Path) {
    $resolved = Resolve-Path -LiteralPath $Path
    if ($resolved.Provider.Name -ne 'FileSystem') { throw 'Installer paths must use the FileSystem provider.' }
    $resolved.ProviderPath
}
function Read-InstallerLockedBytes([string]$Path) {
    $stream = [IO.File]::OpenRead($Path)
    $memory = [IO.MemoryStream]::new()
    try { $stream.CopyTo($memory); ,$memory.ToArray() }
    finally { $memory.Dispose(); $stream.Dispose() }
}
function Enter-InstallerTargetLock([string]$Destination) {
    $name = 'Global\NppMarkdownNextInstaller-' + (Get-InstallerHash ([Text.Encoding]::UTF8.GetBytes([IO.Path]::GetFullPath($Destination).ToUpperInvariant())))
    $mutex = $null
    try {
        $mutex = [Threading.Mutex]::new($false,$name)
        try {
            if (!$mutex.WaitOne(0)) { throw 'Another installer or rollback is using this target.' }
        } catch [Threading.AbandonedMutexException] {
            $mutex.ReleaseMutex()
            throw 'A previous installer stopped unexpectedly; inspect its backup before retrying.'
        }
        $mutex
    } catch {
        if ($mutex) { $mutex.Dispose() }
        throw "Cannot acquire the installer target lock. $($_.Exception.Message)"
    }
}
function Get-InstallerArchitecture([byte[]]$Bytes) {
    if ($Bytes.Length -lt 70 -or [BitConverter]::ToUInt16($Bytes,0) -ne 0x5a4d) { throw 'Invalid PE image.' }
    $offset = [BitConverter]::ToInt32($Bytes,0x3c)
    if ($offset -lt 64 -or $offset + 6 -gt $Bytes.Length -or [BitConverter]::ToUInt32($Bytes,$offset) -ne 0x4550) { throw 'Invalid PE header.' }
    switch ([BitConverter]::ToUInt16($Bytes,$offset+4)) {
        0x8664 { 'x64' } 0x14c { 'x86' } 0xaa64 { 'arm64' } default { throw 'Unsupported PE architecture.' }
    }
}
function Test-InstallerName([string]$Name) {
    $Name -cin @('NppMarkdownNext.dll','WebView2Loader.dll','README.md','License.txt',
        'THIRD_PARTY_NOTICES.md','themes/BEARDED_LICENSE','provenance.json','build-info.json','manifest.json')
}
function Test-InstallerAccessDenied([Exception]$Exception) {
    while ($Exception) {
        if ($Exception -is [UnauthorizedAccessException]) { return $true }
        $Exception = $Exception.InnerException
    }
    $false
}
function Assert-InstallerNoReparse([string]$Path) {
    $cursor = [IO.Path]::GetFullPath($Path)
    while ($cursor) {
        try {
            if ([IO.File]::GetAttributes($cursor) -band [IO.FileAttributes]::ReparsePoint) {
                throw "Reparse points are not allowed in an installation path: $cursor"
            }
        } catch [IO.FileNotFoundException] { } catch [IO.DirectoryNotFoundException] { }
        $parent = [IO.Path]::GetDirectoryName($cursor.TrimEnd('\'))
        if ($parent -eq $cursor) { break }
        $cursor = $parent
    }
}
function Assert-InstallerContainedPath([string]$Path, [string]$Root) {
    $full = [IO.Path]::GetFullPath($Path).TrimEnd('\')
    $base = [IO.Path]::GetFullPath($Root).TrimEnd('\')
    if (!$full.Equals($base,[StringComparison]::OrdinalIgnoreCase) -and
        !$full.StartsWith($base + '\',[StringComparison]::OrdinalIgnoreCase)) { throw 'Installation path escaped its intended root.' }
    Assert-InstallerNoReparse $Path
}
function Assert-PackageDestinationPaths($Package, $Target) {
    Assert-InstallerContainedPath $Target.destination $Target.root
    foreach ($file in $Package.files) { Assert-InstallerContainedPath (Join-Path $Target.destination $file.path) $Target.root }
}
function Read-InstallPackage([string]$Path) {
    $archive = [IO.File]::OpenRead([IO.Path]::GetFullPath($Path))
    $zip = $null; $sha = [Security.Cryptography.SHA256]::Create()
    try {
        $archiveHash = [BitConverter]::ToString($sha.ComputeHash($archive)).Replace('-','').ToLowerInvariant()
        $archive.Position = 0
        $zip = [IO.Compression.ZipArchive]::new($archive,[IO.Compression.ZipArchiveMode]::Read,$true)
        $contents = @{}; $total = 0L
        foreach ($entry in $zip.Entries) {
            $name = $entry.FullName
            if ($name -ceq 'themes/') { continue }
            if (!(Test-InstallerName $name) -or $contents.ContainsKey($name) -or $entry.Length -gt 64MB) { throw "Unsafe, duplicate, or oversized package entry: $name" }
            $total += $entry.Length
            if ($total -gt 128MB) { throw 'Package is too large.' }
            $stream = $entry.Open(); $memory = [IO.MemoryStream]::new()
            try {
                $buffer = [byte[]]::new(65536)
                while (($count = $stream.Read($buffer,0,$buffer.Length)) -gt 0) {
                    if ($memory.Length + $count -gt $entry.Length) { throw 'Invalid ZIP entry length.' }
                    $memory.Write($buffer,0,$count)
                }
                $contents[$name] = $memory.ToArray()
            } finally { $stream.Dispose(); $memory.Dispose() }
        }
    } finally { if ($zip) { $zip.Dispose() }; $archive.Dispose(); $sha.Dispose() }
    if (!$contents.ContainsKey('manifest.json')) { throw 'Package manifest is missing.' }
    $manifest = [Text.Encoding]::UTF8.GetString($contents['manifest.json']) | ConvertFrom-Json
    if ($manifest.schemaVersion -ne 1 -or $manifest.product -ne 'NppMarkdownNext' -or !$manifest.version -or
        $manifest.architecture -notin @('x64','x86','arm64') -or $manifest.source.commit -notmatch '^[0-9a-f]{40}$' -or
        $manifest.source.treeDigest -notmatch '^[0-9a-f]{64}$' -or $manifest.source.dirty -isnot [bool]) { throw 'Invalid package manifest.' }
    $seen = @{}; $files = @()
    foreach ($file in $manifest.files) {
        $name = [string]$file.path
        if (!(Test-InstallerName $name) -or $name -eq 'manifest.json' -or $seen.ContainsKey($name) -or
            !$contents.ContainsKey($name) -or $file.sha256 -notmatch '^[0-9a-f]{64}$' -or
            (Get-InstallerHash $contents[$name]) -cne $file.sha256 -or $contents[$name].Length -ne $file.size) { throw "Package hash or manifest mismatch: $name" }
        if ($name.EndsWith('.dll') -and (Get-InstallerArchitecture $contents[$name]) -ne $manifest.architecture) { throw "Wrong DLL architecture: $name" }
        $seen[$name] = $true
        $files += [pscustomobject]@{path=$name; bytes=$contents[$name]; sha256=$file.sha256}
    }
    if (!$seen.ContainsKey('NppMarkdownNext.dll') -or !$seen.ContainsKey('WebView2Loader.dll') -or $seen.Count -ne $contents.Count-1) { throw 'Manifest does not cover the package.' }
    $files += [pscustomobject]@{path='manifest.json'; bytes=$contents['manifest.json']; sha256=(Get-InstallerHash $contents['manifest.json'])}
    [pscustomobject]@{manifest=$manifest; files=$files; sha256=$archiveHash}
}
function Get-InstallTarget([string]$Directory) {
    Assert-InstallerNoReparse $Directory
    $root = (Resolve-Path -LiteralPath $Directory).Path.TrimEnd('\')
    $executable = Join-Path $root 'notepad++.exe'
    Assert-InstallerContainedPath $executable $root
    Assert-InstallerContainedPath (Join-Path $root 'plugins\NppMarkdownNext') $root
    $architecture = Get-InstallerArchitecture ([IO.File]::ReadAllBytes($executable))
    [pscustomobject]@{root=$root; executable=$executable; architecture=$architecture; destination=(Join-Path $root 'plugins\NppMarkdownNext')}
}
function Get-NotepadProcesses { @(Get-CimInstance Win32_Process -Filter "Name='notepad++.exe'") }
function Get-TargetEditors([string]$ExecutablePath) {
    foreach ($process in (Get-NotepadProcesses)) {
        if (!$process.ExecutablePath) { throw "Cannot identify Notepad++ process $($process.ProcessId); installation was stopped." }
        if ([IO.Path]::GetFullPath($process.ExecutablePath).Equals($ExecutablePath,[StringComparison]::OrdinalIgnoreCase)) { $process }
    }
}
function Close-TargetEditors([string]$ExecutablePath, [switch]$ForceClose) {
    $editors = @(Get-TargetEditors $ExecutablePath)
    if ($editors.Count -and !$ForceClose) {
        if ((Read-Host 'Close the selected Notepad++? Unsaved documents will prompt to save. [y/N]') -notmatch '^(y|yes)$') { throw 'Installation cancelled.' }
    }
    foreach ($editor in $editors) {
        $process = Get-Process -Id $editor.ProcessId -ErrorAction SilentlyContinue
        if ($process) { [void]$process.CloseMainWindow(); $process | Wait-Process -Timeout 60 -ErrorAction SilentlyContinue }
    }
    if (@(Get-TargetEditors $ExecutablePath).Count) { throw 'Notepad++ did not close. Resolve its save dialog and retry.' }
}
function Test-InstalledPackage($Package, [string]$Destination) {
    foreach ($file in $Package.files) {
        $path = Join-Path $Destination $file.path
        Assert-InstallerContainedPath $path $Destination
        if (!(Test-Path -LiteralPath $path -PathType Leaf) -or
            (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant() -cne $file.sha256) { return $false }
    }
    $true
}
function Write-InstallerBytesAtomically([string]$Path, [byte[]]$Bytes, [Nullable[bool]]$ReplaceExisting=$null, [string]$ExpectedExistingSha256='') {
    Assert-InstallerNoReparse $Path
    $directory = Split-Path $Path -Parent
    New-Item -ItemType Directory -Path $directory -Force | Out-Null
    Assert-InstallerNoReparse $Path
    $temporary = $Path + '.' + [Guid]::NewGuid().ToString('N') + '.tmp'
    $stagedSha256 = Get-InstallerHash $Bytes
    try {
        $stream = [IO.FileStream]::new($temporary,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
        try { $stream.Write($Bytes,0,$Bytes.Length); $stream.Flush($true) } finally { $stream.Dispose() }
        Assert-InstallerNoReparse $Path
        $replace = if ($null -eq $ReplaceExisting) { [IO.File]::Exists($Path) } else { [bool]$ReplaceExisting }
        if ($replace) {
            for ($attempt = 0; ; $attempt++) {
                try {
                    Assert-InstallerContainedPath $Path $directory
                    Assert-InstallerContainedPath $temporary $directory
                    if ((Get-FileHash -LiteralPath $temporary -Algorithm SHA256).Hash.ToLowerInvariant() -cne $stagedSha256) { throw 'The staged installer bytes changed before commit; the target was preserved.' }
                    if ($ExpectedExistingSha256 -and (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant() -cne $ExpectedExistingSha256) { throw 'The original target changed before replacement; unrelated bytes were preserved.' }
                    [IO.File]::Replace($temporary,$Path,[NullString]::Value)
                    break
                } catch [IO.IOException] {
                    $failure = $_.Exception
                    while ($failure.InnerException) { $failure = $failure.InnerException }
                    if ($attempt -ge 9 -or ($failure.HResult -band 0xffff) -notin @(32,33,1175)) { throw }
                    Start-Sleep -Milliseconds 50
                }
            }
        }
        else {
            Assert-InstallerContainedPath $Path $directory
            Assert-InstallerContainedPath $temporary $directory
            if ((Get-FileHash -LiteralPath $temporary -Algorithm SHA256).Hash.ToLowerInvariant() -cne $stagedSha256) { throw 'The staged installer bytes changed before commit; the target was preserved.' }
            [IO.File]::Move($temporary,$Path)
        }
    } finally {
        if (Test-Path -LiteralPath $temporary) { Assert-InstallerNoReparse $temporary; Remove-Item -LiteralPath $temporary -Force }
    }
}
function Write-InstallerFile([string]$Path, [byte[]]$Bytes, [Nullable[bool]]$ReplaceExisting=$null, [string]$ExpectedExistingSha256='') { Write-InstallerBytesAtomically $Path $Bytes $ReplaceExisting $ExpectedExistingSha256 }
function Read-PluginBackup([string]$Path, $Target, [string]$ExpectedSha256='') {
    Assert-InstallerContainedPath $Target.destination $Target.root
    Assert-InstallerNoReparse $Path
    $root = (Resolve-Path -LiteralPath $Path).Path
    Assert-InstallerContainedPath (Join-Path $root 'backup.json') $root
    $manifestBytes = Read-InstallerLockedBytes (Join-Path $root 'backup.json')
    $manifestHash = Get-InstallerHash $manifestBytes
    if ($ExpectedSha256 -and $manifestHash -cne $ExpectedSha256) { throw 'The selected backup manifest changed before rollback.' }
    $manifest = [Text.Encoding]::UTF8.GetString($manifestBytes) | ConvertFrom-Json
    if ($manifest.schemaVersion -ne 1 -or $manifest.product -ne 'NppMarkdownNext' -or
        $manifest.target -cne $Target.destination) { throw 'Backup belongs to a different target.' }
    $seen = @{}; $originals = @{}
    foreach ($file in $manifest.files) {
        if (!(Test-InstallerName $file.path) -or $seen.ContainsKey($file.path) -or $file.existed -isnot [bool] -or
            $file.installedSha256 -notmatch '^[0-9a-f]{64}$') { throw 'Invalid backup entry.' }
        $seen[$file.path] = $true
        Assert-InstallerContainedPath (Join-Path $root $file.path) $root
        Assert-InstallerContainedPath (Join-Path $Target.destination $file.path) $Target.root
        if ($file.existed) {
            $bytes = Read-InstallerLockedBytes (Join-Path $root $file.path)
            if ((Get-InstallerHash $bytes) -cne $file.sha256) { throw 'Backup hash mismatch.' }
            $originals[$file.path] = $bytes
        }
    }
    if (!$seen.ContainsKey('NppMarkdownNext.dll') -or !$seen.ContainsKey('WebView2Loader.dll')) { throw 'Backup does not describe both plugin binaries.' }
    [pscustomobject]@{root=$root; manifest=$manifest; manifestBytes=$manifestBytes; sha256=$manifestHash; originals=$originals}
}
function Restore-PluginBackup([string]$Path, $Target, [string[]]$OwnedPaths=@(), [string]$ExpectedSha256='') {
    $targetLock = Enter-InstallerTargetLock $Target.destination
    try {
    $backup = Read-PluginBackup $Path $Target -ExpectedSha256 $ExpectedSha256
    if (!$PSBoundParameters.ContainsKey('OwnedPaths')) {
        $receiptPath = Join-Path $backup.root 'committed.json'
        Assert-InstallerContainedPath $receiptPath $backup.root
        if (Test-Path -LiteralPath $receiptPath) {
            $receipt = Get-Content -LiteralPath $receiptPath -Encoding UTF8 -Raw | ConvertFrom-Json
            $OwnedPaths = @($receipt.paths)
            foreach ($name in $OwnedPaths) {
                if (!(Test-InstallerName $name) -or $name -notin @($backup.manifest.files.path)) { throw 'Invalid committed-file receipt.' }
            }
        }
    }
    $problems = @(); $permissionOnly = $true
    foreach ($file in $backup.manifest.files) {
        $destination = Join-Path $Target.destination $file.path
        try {
            Assert-InstallerContainedPath $destination $Target.root
            if ($file.existed) {
                $present = Test-Path -LiteralPath $destination -PathType Leaf
                if ($present) {
                    $currentHash = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
                    if ($currentHash -ceq $file.sha256) { continue }
                    if ($file.path -notin $OwnedPaths -or $currentHash -cne $file.installedSha256) { throw 'Unrelated changes occupy the original rollback target.' }
                }
                Assert-InstallerContainedPath (Join-Path $backup.root $file.path) $backup.root
                Write-InstallerFile $destination $backup.originals[$file.path] ([bool]$present) $currentHash
                if ((Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant() -cne $file.sha256) { throw 'Restored hash mismatch.' }
            } elseif (Test-Path -LiteralPath $destination) {
                $hash = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
                if ($file.path -notin $OwnedPaths -or $hash -cne $file.installedSha256) { throw 'A new unrelated file occupies the rollback target.' }
                Assert-InstallerContainedPath $destination $Target.root
                Remove-Item -LiteralPath $destination -Force
                if (Test-Path -LiteralPath $destination) { throw 'The newly installed file could not be removed.' }
            }
        } catch {
            $problems += "$($file.path): $($_.Exception.Message)"
            if (!(Test-InstallerAccessDenied $_.Exception)) { $permissionOnly = $false }
        }
    }
    if ($problems.Count) {
        $message = "Recovery required. Backup: $($backup.root). $($problems -join '; ')"
        if ($permissionOnly) { throw [UnauthorizedAccessException]::new($message) }
        throw $message
    }
    [pscustomobject]@{status='RolledBack'; backupPath=$backup.root; files=$backup.manifest.files}
    } finally { $targetLock.ReleaseMutex(); $targetLock.Dispose() }
}
function Install-VerifiedPackage([string]$PackagePath, [string]$DestinationDirectory, [switch]$DryRun, [string]$ExpectedSha256='', [string]$BackupPath='') {
    $package = Read-InstallPackage $PackagePath
    if ($ExpectedSha256 -and $package.sha256 -cne $ExpectedSha256) { throw 'The selected package changed before installation.' }
    $target = Get-InstallTarget $DestinationDirectory
    if ($package.manifest.architecture -ne $target.architecture) { throw 'Package and Notepad++ architectures differ.' }
    Assert-PackageDestinationPaths $package $target
    $targetLock = Enter-InstallerTargetLock $target.destination
    try {
    if (Test-InstalledPackage $package $target.destination) { return [pscustomobject]@{status='NoChange'; backupPath=''; files=$package.manifest.files} }
    if ($DryRun) { return [pscustomobject]@{status='DryRun'; backupPath=''; files=$package.manifest.files; source=$package.manifest.source; target=$target.destination; version=$package.manifest.version} }
    if (@(Get-TargetEditors $target.executable).Count) { throw 'The selected editor is running; no files were changed.' }
    if (!$BackupPath) { $BackupPath = Join-Path $target.destination ('.backups\' + [Guid]::NewGuid().ToString('N')) }
    $backupRoot = Join-Path $target.destination '.backups'
    Assert-InstallerContainedPath $BackupPath $backupRoot
    if ([IO.Path]::GetDirectoryName($BackupPath) -cne $backupRoot -or [IO.Path]::GetFileName($BackupPath) -cnotmatch '^[0-9a-f]{32}$') { throw 'Invalid installer backup path.' }
    Assert-InstallerContainedPath $backupPath $target.root
    New-Item -ItemType Directory -Path $backupPath | Out-Null
    $originals = @()
    foreach ($file in $package.files) {
        $path = Join-Path $target.destination $file.path
        Assert-InstallerContainedPath $path $target.root
        $existed = Test-Path -LiteralPath $path -PathType Leaf
        $entry = [ordered]@{path=$file.path; existed=[bool]$existed; installedSha256=$file.sha256}
        if ($existed) {
            $bytes = [IO.File]::ReadAllBytes($path)
            $entry.sha256 = Get-InstallerHash $bytes
            $backupFile = Join-Path $backupPath $file.path
            Assert-InstallerContainedPath $backupFile $backupPath
            New-Item -ItemType Directory -Path (Split-Path $backupFile -Parent) -Force | Out-Null
            [IO.File]::WriteAllBytes($backupFile,$bytes)
            if ((Get-FileHash -LiteralPath $backupFile -Algorithm SHA256).Hash.ToLowerInvariant() -cne $entry.sha256) { throw 'Backup verification failed; no installed files were changed.' }
        }
        $originals += [pscustomobject]$entry
    }
    $backupManifest = [pscustomobject]@{schemaVersion=1; product='NppMarkdownNext'; target=$target.destination; files=$originals}
    $backupJsonPath = Join-Path $backupPath 'backup.json'
    Assert-InstallerContainedPath $backupJsonPath $backupPath
    $backupManifestBytes = [Text.Encoding]::UTF8.GetBytes(($backupManifest | ConvertTo-Json -Depth 5))
    $backupManifestHash = Get-InstallerHash $backupManifestBytes
    Write-InstallerBytesAtomically $backupJsonPath $backupManifestBytes
    $committed = @()
    try {
        foreach ($file in $package.files) {
            $path = Join-Path $target.destination $file.path
            Assert-InstallerContainedPath $path $target.root
            $original = $originals | Where-Object { $_.path -ceq $file.path }
            Write-InstallerFile $path $file.bytes ([bool]$original.existed) $original.sha256
            if ((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant() -cne $file.sha256) { throw "Installed hash mismatch: $($file.path)" }
            $committed += $file.path
            $receiptPath = Join-Path $backupPath 'committed.json'
            Assert-InstallerContainedPath $receiptPath $backupPath
            Write-InstallerBytesAtomically $receiptPath ([Text.Encoding]::UTF8.GetBytes(([pscustomobject]@{paths=@($committed)} | ConvertTo-Json)))
        }
    } catch {
        $installError = $_.Exception
        try { $null = Restore-PluginBackup $backupPath $target -OwnedPaths $committed -ExpectedSha256 $backupManifestHash }
        catch { throw "$($installError.Message). $($_.Exception.Message)" }
        $message = "$($installError.Message). Original files restored. Backup: $backupPath"
        if (Test-InstallerAccessDenied $installError) { throw [UnauthorizedAccessException]::new($message,$installError) }
        throw $message
    }
    [pscustomobject]@{status='Installed'; backupPath=$backupPath; files=$package.manifest.files}
    } finally { $targetLock.ReleaseMutex(); $targetLock.Dispose() }
}
function Test-InstallerAdministrator {
    ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}
function New-InstallerElevationCommand($Data) {
    $Data.scriptSha256 = $script:InstallerSourceSha256
    $encodedData = [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes(($Data | ConvertTo-Json -Compress)))
    $command = '$data=[Text.Encoding]::UTF8.GetString([Convert]::FromBase64String("' + $encodedData + '"))|ConvertFrom-Json; ' +
        '$stream=[IO.File]::OpenRead($data.script); $reader=[IO.StreamReader]::new($stream,[Text.Encoding]::UTF8,$true); ' +
        'try{$source=$reader.ReadToEnd()}finally{$reader.Dispose();$stream.Dispose()}; ' +
        '$source=$source.TrimStart([char]0xfeff).Replace("`r`n","`n").Replace("`r","`n"); ' +
        '$sha=[Security.Cryptography.SHA256]::Create(); try{$scriptHash=[BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($source))).Replace("-","").ToLowerInvariant()}finally{$sha.Dispose()}; ' +
        'if($scriptHash -cne $data.scriptSha256){throw "Installer script changed before elevation"}; $installer=[ScriptBlock]::Create($source); ' +
        'if((Get-FileHash -LiteralPath $data.verifyPath -Algorithm SHA256).Hash.ToLowerInvariant() -cne $data.sha256){throw "Input changed before elevation"}; ' +
        '$arguments=@{}; if($data.mode -eq "Install"){$arguments.PackagePath=$data.path}else{$arguments.RollbackPath=$data.path}; ' +
        '. $installer @arguments; ' +
        'Invoke-PluginInstaller $data.mode $data.path $data.target -ForceClose:([bool]$data.force) -ExpectedSha256 $data.sha256 -BackupPath $data.backupPath | Format-List; exit 0'
    [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes($command))
}
function Invoke-PluginInstaller([string]$Mode, [string]$Path, [string]$Directory, [switch]$DryRun, [switch]$ForceClose, [string]$ExpectedSha256='', [string]$BackupPath='') {
    $Path = Resolve-InstallerFileSystemPath $Path
    $Directory = Resolve-InstallerFileSystemPath $Directory
    $target = Get-InstallTarget $Directory
    if ($Mode -eq 'Install') {
        $package = Read-InstallPackage $Path
        if ($ExpectedSha256 -and $package.sha256 -cne $ExpectedSha256) { throw 'The selected package changed before installation.' }
        if ($package.manifest.architecture -ne $target.architecture) { throw 'Package and Notepad++ architectures differ.' }
        Assert-PackageDestinationPaths $package $target
        Write-Host ("Package {0}, commit {1}, source {2}, target {3}" -f $package.manifest.version,$package.manifest.source.commit,$package.manifest.source.treeDigest,$target.destination)
        if (Test-InstalledPackage $package $target.destination) { return [pscustomobject]@{status='NoChange'; backupPath=''; files=$package.manifest.files} }
        $verifyPath = $Path; $hash = $package.sha256
    } else {
        $backup = Read-PluginBackup $Path $target -ExpectedSha256 $ExpectedSha256
        $verifyPath = Join-Path $backup.root 'backup.json'
        $hash = $backup.sha256
    }
    if ($DryRun) {
        if ($Mode -eq 'Install') { return Install-VerifiedPackage $Path $Directory -DryRun -ExpectedSha256 $hash }
        return [pscustomobject]@{status='DryRun'; backupPath=$Path; files=@()}
    }
    $null = @(Get-TargetEditors $target.executable)
    Close-TargetEditors $target.executable -ForceClose:$ForceClose
    if ((Get-FileHash -LiteralPath $verifyPath -Algorithm SHA256).Hash.ToLowerInvariant() -cne $hash) { throw 'Input changed before installation.' }
    try {
        if ($Mode -eq 'Install') { return Install-VerifiedPackage $Path $Directory -ExpectedSha256 $hash -BackupPath $BackupPath }
        $target = Get-InstallTarget $Directory
        if (@(Get-TargetEditors $target.executable).Count) { throw 'The selected editor restarted; no files were changed.' }
        return Restore-PluginBackup $Path $target -ExpectedSha256 $hash
    } catch {
        if (!(Test-InstallerAccessDenied $_.Exception) -or (Test-InstallerAdministrator)) { throw }
    }
    $data = @{script=$script:InstallerEntryPath; mode=$Mode; path=$Path; target=$target.root;
        force=[bool]$ForceClose; verifyPath=$verifyPath; sha256=$hash;
        backupPath=(Join-Path $target.destination ('.backups\' + [Guid]::NewGuid().ToString('N')))}
    $encoded = New-InstallerElevationCommand $data
    $process = Start-Process -FilePath (Get-Process -Id $PID).Path -Verb RunAs -Wait -PassThru -ArgumentList @('-NoProfile','-ExecutionPolicy','Bypass','-EncodedCommand',$encoded)
    if ($process.ExitCode -ne 0) { throw 'Elevated installation failed.' }
    $target = Get-InstallTarget $Directory
    if ($Mode -eq 'Install') {
        $verified = Read-InstallPackage $Path
        if ($verified.sha256 -cne $hash -or !(Test-InstalledPackage $verified $target.destination)) { throw 'Elevated process returned success, but the installed files do not match the selected package.' }
        $backup = Read-PluginBackup $data.backupPath $target
        $receiptPath = Join-Path $backup.root 'committed.json'
        Assert-InstallerContainedPath $receiptPath $backup.root
        $committed = Get-Content -LiteralPath $receiptPath -Encoding UTF8 -Raw | ConvertFrom-Json
        if (@($backup.manifest.files).Count -ne $verified.files.Count -or @($committed.paths).Count -ne $verified.files.Count) { throw 'Elevated installation did not leave a complete rollback receipt.' }
        foreach ($file in $verified.files) {
            $original = @($backup.manifest.files | Where-Object { $_.path -ceq $file.path })
            if ($original.Count -ne 1 -or $original[0].installedSha256 -cne $file.sha256 -or $file.path -cnotin @($committed.paths)) { throw 'Elevated rollback receipt does not match the selected package.' }
        }
        $BackupPath = $backup.root
    } else {
        $verified = Read-PluginBackup $Path $target -ExpectedSha256 $hash
        foreach ($file in $verified.manifest.files) {
            $installedPath = Join-Path $target.destination $file.path
            if ($file.existed) {
                if (!(Test-Path -LiteralPath $installedPath -PathType Leaf) -or (Get-FileHash -LiteralPath $installedPath -Algorithm SHA256).Hash.ToLowerInvariant() -cne $file.sha256) { throw 'Elevated rollback did not restore the original file.' }
            } elseif (Test-Path -LiteralPath $installedPath) { throw 'Elevated rollback did not restore the original file absence.' }
        }
        $BackupPath = $verified.root
    }
    return [pscustomobject]@{status='CompletedByElevatedProcess'; backupPath=$BackupPath; files=$verified.manifest.files}
}

if ($MyInvocation.InvocationName -eq '.') { return }
try {
    [Console]::OutputEncoding = [Text.Encoding]::UTF8
    $mode = if ($PSCmdlet.ParameterSetName -eq 'Rollback') { 'Rollback' } else { 'Install' }
    $path = if ($mode -eq 'Rollback') { $RollbackPath } else { $PackagePath }
    Invoke-PluginInstaller $mode $path $NppDirectory -DryRun:$DryRun -ForceClose:$Force | Format-List
} catch {
    Write-Host ("ERROR: " + $_.Exception.Message) -ForegroundColor Red
    exit 1
}
