param([string]$InstallerPath)
$ErrorActionPreference = 'Stop'
$project = Split-Path $PSScriptRoot -Parent
if (!$InstallerPath) { $InstallerPath = Join-Path $project 'tools\Install-NppMarkdownNext.ps1' }
$parseErrors = $null
$installerAst = [Management.Automation.Language.Parser]::ParseFile($InstallerPath,[ref]$null,[ref]$parseErrors)
$parameterNames = @($installerAst.ParamBlock.Parameters | ForEach-Object { $_.Name.VariablePath.UserPath })
if ($parseErrors -or 'PackagePath' -notin $parameterNames -or 'RollbackPath' -notin $parameterNames) {
    throw 'The isolated test requires the verified-package installer; the legacy installer will not be executed.'
}
. (Join-Path $PSScriptRoot 'test_package.ps1') -HelpersOnly
. $InstallerPath -PackagePath 'unused'
$scratch = Join-Path $project ('bin\installer-test-' + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $scratch | Out-Null
$script:InstallTestJunctions = [Collections.Generic.List[string]]::new()
function New-InstallTestJunction([string]$Path,[string]$Target) {
    New-Item -ItemType Directory -Path (Split-Path $Path -Parent) -Force | Out-Null
    New-Item -ItemType Junction -Path $Path -Target $Target | Out-Null
    $script:InstallTestJunctions.Add($Path)
}
function Remove-InstallTestJunction([string]$Path) {
    $full = [IO.Path]::GetFullPath($Path)
    if (!$full.StartsWith($scratch + [IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase) -or
        !([IO.File]::GetAttributes($full) -band [IO.FileAttributes]::ReparsePoint)) { throw 'Unsafe test junction cleanup.' }
    [IO.Directory]::Delete($full)
}
$script:MockProcesses = @()
function Get-NotepadProcesses { $script:MockProcesses }
function Test-InstallerAdministrator { throw 'DryRun or invalid input attempted elevation.' }
$script:RealCloser = ${function:Close-TargetEditors}
function Close-TargetEditors { throw 'DryRun or invalid input attempted editor close.' }
function New-InstallFixture([string]$Name, [switch]$Existing) {
    $root = Join-Path $scratch $Name
    New-Item -ItemType Directory -Path $root | Out-Null
    [IO.File]::WriteAllBytes((Join-Path $root 'notepad++.exe'),(New-TestPe))
    $target = Get-InstallTarget $root
    if ($Existing) {
        New-Item -ItemType Directory -Path $target.destination -Force | Out-Null
        [IO.File]::WriteAllBytes((Join-Path $target.destination 'NppMarkdownNext.dll'),[byte[]]@(7,8,9))
        [IO.File]::WriteAllBytes((Join-Path $target.destination 'WebView2Loader.dll'),[byte[]]@(9,8,7))
    }
    $target
}
function Invoke-InstallTestChildCommand([string]$Command,[string]$Name) {
    $encodedCommand = [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes('function Get-CimInstance { @() }; ' + $Command))
    Microsoft.PowerShell.Management\Start-Process -FilePath (Get-Process -Id $PID).Path -WindowStyle Hidden -Wait -PassThru -ArgumentList @('-NoProfile','-EncodedCommand',$encodedCommand) -RedirectStandardOutput (Join-Path $scratch ($Name + '.out')) -RedirectStandardError (Join-Path $scratch ($Name + '.err'))
}
function Get-InstallTestBackupVariant([string]$Path) {
    $manifest = Get-Content -LiteralPath (Join-Path $Path 'backup.json') -Raw | ConvertFrom-Json
    $bytes = [Text.Encoding]::UTF8.GetBytes('different valid original backup bytes')
    ($manifest.files | Where-Object { $_.path -ceq 'NppMarkdownNext.dll' }).sha256 = Get-InstallerHash $bytes
    [pscustomobject]@{manifestBytes=[Text.Encoding]::UTF8.GetBytes(($manifest | ConvertTo-Json -Depth 6));bytes=$bytes}
}
try {
    $package = New-TestPackage $scratch
    $target = New-InstallFixture 'portable Юникод [safe]'
    $result = Invoke-PluginInstaller 'Install' $package $target.root -DryRun
    Assert-PackageTest ($result.status -eq 'DryRun' -and !(Test-Path -LiteralPath $target.destination)) 'InstallerSafePaths DryRun'
    $wrong = New-TestPackage $scratch 'arm64'
    Assert-PackageThrows { Invoke-PluginInstaller 'Install' $wrong $target.root } 'InstallerSafePaths wrong architecture'
    foreach ($name in @('../evil.dll','nppmarkdownnext.dll','NppMarkdownNext/NppMarkdownNext.dll')) {
        $bad = New-TestPackage $scratch; Add-TestZipEntry $bad $name ([byte[]]@(1))
        Assert-PackageThrows { Invoke-PluginInstaller 'Install' $bad $target.root } "InstallerSafePaths $name"
    }
    foreach ($kind in @('plugin-junction','plugins-junction','themes-junction','backup-junction')) {
        $linked = New-InstallFixture $kind
        $outside = Join-Path $scratch ('outside-' + $kind)
        New-Item -ItemType Directory -Path $outside | Out-Null
        $sentinel = Join-Path $outside 'sentinel.txt'
        [IO.File]::WriteAllText($sentinel,'outside sentinel')
        $link = switch ($kind) {
            'plugin-junction' { $linked.destination }
            'plugins-junction' { Join-Path $linked.root 'plugins' }
            'themes-junction' { Join-Path $linked.destination 'themes' }
            'backup-junction' { Join-Path $linked.destination '.backups' }
        }
        New-InstallTestJunction $link $outside
        Assert-PackageThrows { Install-VerifiedPackage $package $linked.root } "Installer_RejectsReparse $kind"
        Assert-PackageTest ([IO.File]::ReadAllText($sentinel) -eq 'outside sentinel') "Installer_OutsideSentinelPreserved $kind"
        Assert-PackageTest (@(Get-ChildItem -LiteralPath $outside -Force).Count -eq 1) "Installer_NoOutsideWrites $kind"
        Remove-InstallTestJunction $link
        [void]$script:InstallTestJunctions.Remove($link)
    }
    $script:MockProcesses = @([pscustomobject]@{ProcessId=1;ExecutablePath=$target.executable},
        [pscustomobject]@{ProcessId=2;ExecutablePath=(Join-Path $scratch 'another\notepad++.exe')})
    $matches = @(Get-TargetEditors $target.executable)
    Assert-PackageTest ($matches.Count -eq 1 -and $matches[0].ProcessId -eq 1) 'Installer_TargetScopingKeepsOtherInstancesOpen'
    function Read-Host { 'N' }
    Assert-PackageThrows { & $script:RealCloser $target.executable } 'InstallerSafePaths cancelled close'
    Assert-PackageTest (!(Test-Path -LiteralPath $target.destination)) 'Installer cancellation leaves destination unchanged'
    $script:MockProcesses = @([pscustomobject]@{ProcessId=3;ExecutablePath=$null})
    Assert-PackageThrows { Get-TargetEditors $target.executable } 'InstallerSafePaths unknown process path'
    $script:MockProcesses = @()

    $selected = Read-InstallPackage $package
    $data = @{script=$InstallerPath; mode='Install'; path=$package; target=$target.root;
        force=$true; verifyPath=$package; sha256=$selected.sha256}
    $command = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String((New-InstallerElevationCommand $data)))
    $encoded = [regex]::Match($command,'FromBase64String\("([A-Za-z0-9+/=]+)"\)').Groups[1].Value
    $decoded = [Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($encoded)) | ConvertFrom-Json
    Assert-PackageTest ($decoded.path -ceq $package -and $decoded.target -ceq $target.root -and $decoded.force -and $decoded.sha256 -ceq $selected.sha256) 'Installer_ElevationPreservesUnicodePathsAndParameters'
    Assert-PackageTest ($decoded.scriptSha256 -ceq $script:InstallerSourceSha256 -and $decoded.scriptSha256 -match '^[0-9a-f]{64}$') 'Installer_ElevationPinsLoadedScriptFingerprint'
    Assert-PackageTest ((New-InstallerElevationCommand $data).Length -lt 16000) 'Installer_ElevationCommandFitsWindowsCommandLine'
    $syntaxErrors = $null
    [void][Management.Automation.Language.Parser]::ParseInput($command,[ref]$null,[ref]$syntaxErrors)
    Assert-PackageTest (!$syntaxErrors) 'Installer elevation command parses'
    $installerSource = [IO.File]::ReadAllText($InstallerPath)
    foreach ($mutationPhase in @('before-command','after-command','after-verification','format-only')) {
        $scriptCopy = Join-Path $scratch ($mutationPhase + '-installer.ps1')
        $marker = Join-Path $scratch ($mutationPhase + '-substituted.txt')
        $markerPath = [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($marker))
        $replacement = '[IO.File]::WriteAllText([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String("' + $markerPath + '")),"substituted"); exit 0'
        $source = if ($mutationPhase -eq 'format-only') { $installerSource.Replace("`r`n","`n").Replace("`n","`r`n") } else { $installerSource }
        [IO.File]::WriteAllText($scriptCopy,$source,[Text.UTF8Encoding]::new($mutationPhase -eq 'format-only'))
        $mutationTarget = New-InstallFixture ('script-' + $mutationPhase)
        $mutationData = @{script=$scriptCopy;mode='Install';path=$package;target=$mutationTarget.root;force=$true;verifyPath=$package;sha256=$selected.sha256;
            backupPath=(Join-Path $mutationTarget.destination ('.backups\' + [Guid]::NewGuid().ToString('N')));replacementSource=[Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($replacement))}
        $builder = [PowerShell]::Create()
        try {
            [void]$builder.AddScript({param($copy,$phase,$data)
                . $copy -PackagePath 'unused'
                if ($phase -eq 'before-command') { [IO.File]::WriteAllText($copy,[Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($data.replacementSource))) }
                $encoded = New-InstallerElevationCommand $data
                if ($phase -eq 'after-command') { [IO.File]::WriteAllText($copy,[Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($data.replacementSource))) }
                if ($phase -eq 'format-only') { [IO.File]::WriteAllText($copy,[IO.File]::ReadAllText($copy).Replace("`r`n","`n"),[Text.UTF8Encoding]::new($false)) }
                [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String($encoded))
            }).AddArgument($scriptCopy).AddArgument($mutationPhase).AddArgument($mutationData)
            $mutationCommand = @($builder.Invoke())[-1]
            Assert-PackageTest (!$builder.HadErrors) "Installer_LoadedScriptFixture $mutationPhase"
        } finally { $builder.Dispose() }
        if ($mutationPhase -eq 'after-verification') {
            $mutationCommand = $mutationCommand.Replace('. $installer @arguments;','[IO.File]::WriteAllText($data.script,[Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($data.replacementSource))); . $installer @arguments;')
        }
        $mutationProcess = Invoke-InstallTestChildCommand $mutationCommand ('script-' + $mutationPhase)
        Assert-PackageTest (!(Test-Path -LiteralPath $marker)) "Installer_SubstitutedScriptNeverRuns $mutationPhase"
        if ($mutationPhase -in @('before-command','after-command')) {
            $mutationError = Get-Content -LiteralPath (Join-Path $scratch ('script-' + $mutationPhase + '.err')) -Raw
            Assert-PackageTest ($mutationProcess.ExitCode -ne 0 -and $mutationError -match 'Installer script changed before elevation' -and !(Test-Path -LiteralPath $mutationTarget.destination)) "Installer_RejectsChangedLoadedScript $mutationPhase"
        } else {
            Assert-PackageTest ($mutationProcess.ExitCode -eq 0 -and (Test-InstalledPackage $selected $mutationTarget.destination)) "Installer_ExecutesVerifiedInMemoryScript $mutationPhase"
        }
    }
    function Test-InstallerAdministrator { $false }
    ${function:Close-TargetEditors} = $script:RealCloser
    function Start-Process { throw 'A writable portable target must not request elevation.' }
    foreach ($hasOriginals in @($false,$true)) {
        $portable = New-InstallFixture ('public-portable-' + $hasOriginals + ' Юникод [safe]') -Existing:$hasOriginals
        $publicInstall = Invoke-PluginInstaller 'Install' $package $portable.root -ForceClose
        Assert-PackageTest ($publicInstall.status -eq 'Installed' -and (Test-InstalledPackage $selected $portable.destination)) 'Installer_PublicPortableInstallWithoutAdministrator'
        $receipt = Get-Content -LiteralPath (Join-Path $publicInstall.backupPath 'committed.json') -Raw | ConvertFrom-Json
        Assert-PackageTest (@($receipt.paths).Count -eq $selected.files.Count) 'Installer_PublicPortableCommittedReceipt'
        Assert-PackageTest ((Invoke-PluginInstaller 'Install' $package $portable.root).status -eq 'NoChange') 'Installer_PublicPortableNoChangeWithoutAdministrator'
        $publicRollback = Invoke-PluginInstaller 'Rollback' $publicInstall.backupPath $portable.root -ForceClose
        Assert-PackageTest ($publicRollback.status -eq 'RolledBack') 'Installer_PublicPortableRollbackWithoutAdministrator'
        if ($hasOriginals) {
            Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $portable.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'Installer_PublicPortableRollbackOriginalPlugin'
            Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $portable.destination 'WebView2Loader.dll'))) -eq 'CQgH') 'Installer_PublicPortableRollbackOriginalLoader'
        } else {
            Assert-PackageTest (!(Test-Path -LiteralPath (Join-Path $portable.destination 'NppMarkdownNext.dll')) -and !(Test-Path -LiteralPath (Join-Path $portable.destination 'WebView2Loader.dll'))) 'Installer_PublicPortableRollbackOriginalAbsence'
        }
        Assert-PackageTest (!(Test-Path -LiteralPath (Join-Path $portable.destination 'themes\BEARDED_LICENSE'))) 'Installer_PublicPortableRollbackRemovesAddedFiles'
    }

    $relativeTarget = New-InstallFixture 'relative Юникод [safe]' -Existing
    $script:RealWriter = ${function:Write-InstallerFile}
    $nativeDirectory = [Environment]::CurrentDirectory
    Push-Location -LiteralPath $scratch
    try {
        [Environment]::CurrentDirectory = $project
        $relativePackage = '.\' + [IO.Path]::GetFileName($package)
        $relativeDirectory = '.\' + [IO.Path]::GetFileName($relativeTarget.root)
        $relativeInstall = Invoke-PluginInstaller 'Install' $relativePackage $relativeDirectory
        Assert-PackageTest ($relativeInstall.status -eq 'Installed' -and (Test-InstalledPackage $selected $relativeTarget.destination)) 'Installer_PublicRelativeInstallUsesPowerShellLocation'
        $relativeBackup = '.\' + $relativeInstall.backupPath.Substring($scratch.Length + 1)
        Assert-PackageTest ((Invoke-PluginInstaller 'Rollback' $relativeBackup $relativeDirectory).status -eq 'RolledBack') 'Installer_PublicRelativeRollbackUsesPowerShellLocation'
        function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected parent-only relative target denial.') }
        function Start-Process {
            param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList)
            $childCommand = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String($ArgumentList[-1]))
            $encodedData = [regex]::Match($childCommand,'FromBase64String\("([A-Za-z0-9+/=]+)"\)').Groups[1].Value
            $script:RelativeElevationData = [Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($encodedData)) | ConvertFrom-Json
            Invoke-InstallTestChildCommand $childCommand ('relative-' + $script:RelativeElevationData.mode)
        }
        $relativeElevated = Invoke-PluginInstaller 'Install' $relativePackage $relativeDirectory
        Assert-PackageTest ($relativeElevated.status -eq 'CompletedByElevatedProcess' -and $script:RelativeElevationData.path -ceq $package -and $script:RelativeElevationData.target -ceq $relativeTarget.root -and $script:RelativeElevationData.verifyPath -ceq $package) 'Installer_RelativeElevationUsesResolvedAbsolutePaths'
        $relativeBackup = '.\' + $relativeElevated.backupPath.Substring($scratch.Length + 1)
        $relativeRolledBack = Invoke-PluginInstaller 'Rollback' $relativeBackup $relativeDirectory
        Assert-PackageTest ($relativeRolledBack.status -eq 'CompletedByElevatedProcess' -and $script:RelativeElevationData.path -ceq $relativeElevated.backupPath -and $script:RelativeElevationData.verifyPath -ceq (Join-Path $relativeElevated.backupPath 'backup.json')) 'Installer_RelativeRollbackElevationUsesResolvedAbsolutePaths'
    } finally {
        ${function:Write-InstallerFile} = $script:RealWriter
        function Start-Process { throw 'A writable portable target must not request elevation.' }
        Pop-Location
        [Environment]::CurrentDirectory = $nativeDirectory
    }

    $script:RealReader = ${function:Read-InstallPackage}
    $script:SwapArchive = New-TestPackage $scratch
    $script:AlternateArchive = New-TestPackage $scratch
    $alternateZip = [IO.Compression.ZipFile]::Open($script:AlternateArchive,[IO.Compression.ZipArchiveMode]::Update)
    try {
        $manifestReader = [IO.StreamReader]::new($alternateZip.GetEntry('manifest.json').Open())
        try { $alternateManifest = $manifestReader.ReadToEnd() | ConvertFrom-Json } finally { $manifestReader.Dispose() }
        $alternateManifest.version = '9.9.9'
        $alternateZip.GetEntry('manifest.json').Delete()
        $manifestBytes = [Text.Encoding]::UTF8.GetBytes(($alternateManifest | ConvertTo-Json -Depth 6))
        $manifestStream = $alternateZip.CreateEntry('manifest.json').Open()
        try { $manifestStream.Write($manifestBytes,0,$manifestBytes.Length) } finally { $manifestStream.Dispose() }
    } finally { $alternateZip.Dispose() }
    $swapSelected = Read-InstallPackage $script:SwapArchive
    Assert-PackageTest ((Read-InstallPackage $script:AlternateArchive).sha256 -cne $swapSelected.sha256) 'Installer_AlternateArchiveIsValidAndDifferent'
    $swappedTarget = New-InstallFixture 'public-package-swap'
    $script:PackageReads = 0
    function Read-InstallPackage([string]$Path) {
        $script:PackageReads++
        if ($script:PackageReads -eq 2) { Copy-Item -LiteralPath $script:AlternateArchive -Destination $script:SwapArchive -Force }
        & $script:RealReader $Path
    }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Install' $script:SwapArchive $swappedTarget.root } catch { $message = $_.Exception.Message }
    ${function:Read-InstallPackage} = $script:RealReader
    Assert-PackageTest ($message -match 'selected package changed' -and !(Test-Path -LiteralPath $swappedTarget.destination)) 'Installer_PublicArchiveSwapAfterPreflightWritesNothing'

    $childSwapArchive = New-TestPackage $scratch
    $childSwapTarget = New-InstallFixture 'elevated-package-swap'
    $childSwapPackage = Read-InstallPackage $childSwapArchive
    $childData = @{script=$InstallerPath;mode='Install';path=$childSwapArchive;target=$childSwapTarget.root;force=$true;
        verifyPath=$childSwapArchive;sha256=$childSwapPackage.sha256;backupPath=(Join-Path $childSwapTarget.destination ('.backups\' + [Guid]::NewGuid().ToString('N')))}
    $childCommand = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String((New-InstallerElevationCommand $childData)))
    $swapPath = [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($script:AlternateArchive))
    $childCommand = $childCommand.Replace('. $installer @arguments;',('. $installer @arguments; Copy-Item -LiteralPath ([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String("' + $swapPath + '"))) -Destination $data.path -Force;'))
    $childProcess = Invoke-InstallTestChildCommand $childCommand 'elevated-package-swap'
    $childError = Get-Content -LiteralPath (Join-Path $scratch 'elevated-package-swap.err') -Raw
    Assert-PackageTest ($childProcess.ExitCode -ne 0 -and $childError -match 'selected package changed' -and !(Test-Path -LiteralPath $childSwapTarget.destination)) 'Installer_ElevatedArchiveSwapAfterHashCheckWritesNothing'

    $script:ElevationAttempts = 0
    function Start-Process { param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList); $script:ElevationAttempts++; [pscustomobject]@{ExitCode=0} }
    $acl = Get-Acl -LiteralPath $target.root
    $deniedAcl = Get-Acl -LiteralPath $target.root
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent().User
    $rule = [Security.AccessControl.FileSystemAccessRule]::new($identity,[Security.AccessControl.FileSystemRights]::Write,
        [Security.AccessControl.InheritanceFlags]'ContainerInherit,ObjectInherit',[Security.AccessControl.PropagationFlags]::None,[Security.AccessControl.AccessControlType]::Deny)
    $deniedAcl.AddAccessRule($rule)
    $hostHash = (Get-FileHash -LiteralPath $target.executable -Algorithm SHA256).Hash
    try {
        Set-Acl -LiteralPath $target.root -AclObject $deniedAcl
        $message = ''
        try { $null = Invoke-PluginInstaller 'Install' $package $target.root } catch { $message = $_.Exception.Message }
        Assert-PackageTest ($script:ElevationAttempts -eq 1 -and $message -match 'installed files do not match') 'Installer_VerifiesEffectsAfterElevatedExitZero'
        $protectedFiles = @()
        if (Test-Path -LiteralPath $target.destination) { $protectedFiles = @(Get-ChildItem -LiteralPath $target.destination -Recurse -File -Force) }
        Assert-PackageTest ($protectedFiles.Count -eq 0 -and (Get-FileHash -LiteralPath $target.executable -Algorithm SHA256).Hash -ceq $hostHash) 'Installer_DeniedElevationPreservesHostAndCreatesNoFiles'
    } finally { Set-Acl -LiteralPath $target.root -AclObject $acl }

    $script:RealWriter = ${function:Write-InstallerFile}
    $ioFailure = New-InstallFixture 'public-io-failure' -Existing
    function Write-InstallerFile { throw [IO.IOException]::new('Injected non-permission IO failure.') }
    function Start-Process { throw 'An IO failure must not request elevation.' }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Install' $package $ioFailure.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($message -match '^Injected non-permission IO failure\.') 'Installer_PublicIoFailureDoesNotElevate'

    $installDenied = New-InstallFixture 'public-install-denied' -Existing
    function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected target write denial.') }
    $script:ElevationAttempts = 0
    function Start-Process { param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList); $script:ElevationAttempts++; [pscustomobject]@{ExitCode=0} }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Install' $package $installDenied.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($script:ElevationAttempts -eq 1 -and $message -match 'installed files do not match') 'Installer_PublicRecoveredWriteDenialElevates'
    Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $installDenied.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'Installer_PublicRecoveredWriteDenialPreservesOriginal'

    $recoveryDenied = New-InstallFixture 'public-recovery-denied' -Existing
    $script:Writes = 0
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes,[Nullable[bool]]$ReplaceExisting=$null,[string]$ExpectedExistingSha256='') {
        $script:Writes++
        if ($script:Writes -ge 2) { throw [UnauthorizedAccessException]::new('Injected install and recovery denial.') }
        & $script:RealWriter $Path $Bytes $ReplaceExisting $ExpectedExistingSha256
    }
    function Start-Process { throw 'A failed recovery must not retry the installation elevated.' }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Install' $package $recoveryDenied.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($message -match 'Recovery required\. Backup:' -and $message -notmatch 'must not retry') 'Installer_PublicFailedRecoveryDoesNotElevate'

    $rollbackDenied = New-InstallFixture 'public-rollback-denied' -Existing
    ${function:Write-InstallerFile} = $script:RealWriter
    $rollbackInstall = Invoke-PluginInstaller 'Install' $package $rollbackDenied.root
    function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected target write denial.') }
    $script:ElevationAttempts = 0
    function Start-Process { param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList); $script:ElevationAttempts++; [pscustomobject]@{ExitCode=0} }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Rollback' $rollbackInstall.backupPath $rollbackDenied.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($script:ElevationAttempts -eq 1 -and $message -match 'Elevated rollback did not restore') 'Installer_PublicRollbackDenialElevatesAndVerifiesEffects'
    ${function:Write-InstallerFile} = $script:RealWriter

    foreach ($tamperReceipt in @($false,$true)) {
        $elevatedTarget = New-InstallFixture ('elevated-receipt-' + $tamperReceipt + ' Юникод [safe]') -Existing
        $script:TamperElevatedReceipt = $tamperReceipt
        $script:ExpectedElevatedBackup = ''
        function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected parent-only target denial.') }
        function Start-Process {
            param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList)
            $childCommand = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String($ArgumentList[-1]))
            $encodedData = [regex]::Match($childCommand,'FromBase64String\("([A-Za-z0-9+/=]+)"\)').Groups[1].Value
            $childData = [Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($encodedData)) | ConvertFrom-Json
            $script:ExpectedElevatedBackup = $childData.backupPath
            $process = Invoke-InstallTestChildCommand $childCommand ('elevated-receipt-' + $script:TamperElevatedReceipt)
            if ($script:TamperElevatedReceipt -and $process.ExitCode -eq 0) {
                [IO.File]::WriteAllText((Join-Path $childData.backupPath 'committed.json'),'{"paths":[]}')
            }
            $process
        }
        $elevatedResult = $null; $message = ''
        try { $elevatedResult = Invoke-PluginInstaller 'Install' $package $elevatedTarget.root } catch { $message = $_.Exception.Message }
        ${function:Write-InstallerFile} = $script:RealWriter
        if ($tamperReceipt) {
            Assert-PackageTest ($message -match 'complete rollback receipt') 'Installer_ElevatedSuccessRejectsIncompleteReceipt'
        } else {
            Assert-PackageTest ($elevatedResult.status -eq 'CompletedByElevatedProcess' -and $elevatedResult.backupPath -ceq $script:ExpectedElevatedBackup) 'Installer_ElevatedSuccessReturnsExactValidatedBackup'
            $null = Read-PluginBackup $elevatedResult.backupPath $elevatedTarget
            function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected parent-only rollback denial.') }
            $elevatedRollback = Invoke-PluginInstaller 'Rollback' $elevatedResult.backupPath $elevatedTarget.root
            ${function:Write-InstallerFile} = $script:RealWriter
            Assert-PackageTest ($elevatedRollback.status -eq 'CompletedByElevatedProcess' -and $elevatedRollback.backupPath -ceq $elevatedResult.backupPath) 'Installer_ElevatedRollbackReturnsExactBackup'
            Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $elevatedTarget.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'Installer_ElevatedBackupRestoresOriginalBytes'
        }
    }

    $script:RealBackupReader = ${function:Read-PluginBackup}
    foreach ($swapKind in @('manifest','file-after-read')) {
        $backupSwapTarget = New-InstallFixture ('public-backup-swap-' + $swapKind) -Existing
        $backupSwapInstall = Install-VerifiedPackage $package $backupSwapTarget.root
        $script:SwappedBackup = $backupSwapInstall.backupPath
        $script:BackupSwapKind = $swapKind
        $script:BackupReads = 0
        function Read-PluginBackup([string]$Path,$Target,[string]$ExpectedSha256='') {
            $script:BackupReads++
            if ($script:BackupReads -eq 2 -and $script:BackupSwapKind -eq 'manifest') {
                $variant = Get-InstallTestBackupVariant $Path
                [IO.File]::WriteAllBytes((Join-Path $Path 'NppMarkdownNext.dll'),$variant.bytes)
                [IO.File]::WriteAllBytes((Join-Path $Path 'backup.json'),$variant.manifestBytes)
            }
            $snapshot = & $script:RealBackupReader $Path $Target -ExpectedSha256 $ExpectedSha256
            if ($script:BackupReads -eq 2 -and $script:BackupSwapKind -eq 'file-after-read') {
                [IO.File]::WriteAllText((Join-Path $Path 'NppMarkdownNext.dll'),'unverified bytes swapped after snapshot')
            }
            $snapshot
        }
        $rollbackResult = $null; $message = ''
        try { $rollbackResult = Invoke-PluginInstaller 'Rollback' $backupSwapInstall.backupPath $backupSwapTarget.root } catch { $message = $_.Exception.Message }
        ${function:Read-PluginBackup} = $script:RealBackupReader
        if ($swapKind -eq 'manifest') {
            Assert-PackageTest ($message -match 'selected backup manifest changed' -and (Test-InstalledPackage $selected $backupSwapTarget.destination)) 'Installer_RollbackRejectsDifferentValidManifestBeforeWrites'
        } else {
            Assert-PackageTest ($rollbackResult.status -eq 'RolledBack' -and [Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $backupSwapTarget.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'Installer_RollbackWritesVerifiedSnapshotAfterBackupFileSwap'
        }
    }

    $childBackupSwapTarget = New-InstallFixture 'elevated-backup-manifest-swap' -Existing
    $childBackupSwapInstall = Install-VerifiedPackage $package $childBackupSwapTarget.root
    $childBackup = Read-PluginBackup $childBackupSwapInstall.backupPath $childBackupSwapTarget
    $variant = Get-InstallTestBackupVariant $childBackup.root
    $childData = @{script=$InstallerPath;mode='Rollback';path=$childBackup.root;target=$childBackupSwapTarget.root;force=$true;
        verifyPath=(Join-Path $childBackup.root 'backup.json');sha256=$childBackup.sha256;backupPath='';
        replacementManifest=[Convert]::ToBase64String($variant.manifestBytes);replacementOriginal=[Convert]::ToBase64String($variant.bytes)}
    $childCommand = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String((New-InstallerElevationCommand $childData)))
    $childCommand = $childCommand.Replace('. $installer @arguments;','. $installer @arguments; [IO.File]::WriteAllBytes((Join-Path $data.path "NppMarkdownNext.dll"),[Convert]::FromBase64String($data.replacementOriginal)); [IO.File]::WriteAllBytes((Join-Path $data.path "backup.json"),[Convert]::FromBase64String($data.replacementManifest));')
    $childProcess = Invoke-InstallTestChildCommand $childCommand 'elevated-backup-manifest-swap'
    $childError = Get-Content -LiteralPath (Join-Path $scratch 'elevated-backup-manifest-swap.err') -Raw
    Assert-PackageTest ($childProcess.ExitCode -ne 0 -and $childError -match 'selected backup manifest changed' -and (Test-InstalledPackage $selected $childBackupSwapTarget.destination)) 'Installer_ElevatedRollbackRejectsManifestSwapAfterHashCheck'

    $script:PostVerifyTarget = New-InstallFixture 'elevated-backup-postverify-swap' -Existing
    $postVerifyInstall = Install-VerifiedPackage $package $script:PostVerifyTarget.root
    function Write-InstallerFile { throw [UnauthorizedAccessException]::new('Injected parent-only rollback denial.') }
    function Start-Process {
        param($FilePath,$Verb,[switch]$Wait,[switch]$PassThru,$ArgumentList)
        $childCommand = [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String($ArgumentList[-1]))
        $process = Invoke-InstallTestChildCommand $childCommand 'elevated-backup-postverify-swap'
        if ($process.ExitCode -eq 0) {
            $variant = Get-InstallTestBackupVariant $postVerifyInstall.backupPath
            [IO.File]::WriteAllBytes((Join-Path $postVerifyInstall.backupPath 'NppMarkdownNext.dll'),$variant.bytes)
            [IO.File]::WriteAllBytes((Join-Path $postVerifyInstall.backupPath 'backup.json'),$variant.manifestBytes)
            [IO.File]::WriteAllBytes((Join-Path $script:PostVerifyTarget.destination 'NppMarkdownNext.dll'),$variant.bytes)
        }
        $process
    }
    $message = ''
    try { $null = Invoke-PluginInstaller 'Rollback' $postVerifyInstall.backupPath $script:PostVerifyTarget.root } catch { $message = $_.Exception.Message }
    ${function:Write-InstallerFile} = $script:RealWriter
    Assert-PackageTest ($message -match 'selected backup manifest changed') 'Installer_ParentElevatedRollbackVerificationRejectsDifferentValidManifest'

    $existing = New-InstallFixture 'existing Юникод' -Existing
    $ini = Join-Path $existing.destination 'NppMarkdownNext.ini'
    [IO.File]::WriteAllText($ini,'untouched settings')
    $installed = Install-VerifiedPackage $package $existing.root
    Assert-PackageTest ($installed.status -eq 'Installed') 'Installer installs verified bytes'
    $backupState = Read-PluginBackup $installed.backupPath $existing
    Assert-PackageTest ($backupState.manifest.target -ceq $existing.destination -and $existing.destination.Contains('Юникод')) 'Installer_UnicodeBackupTargetRoundTrips'
    Assert-PackageTest ((Install-VerifiedPackage $package $existing.root).status -eq 'NoChange') 'InstallerRollback same package no-op'
    $null = Restore-PluginBackup $installed.backupPath $existing
    $backupLink = Join-Path $scratch 'linked-backup'
    New-InstallTestJunction $backupLink $installed.backupPath
    Assert-PackageThrows { Restore-PluginBackup $backupLink $existing } 'Installer_RejectsReparseBackupRoot'
    Remove-InstallTestJunction $backupLink
    [void]$script:InstallTestJunctions.Remove($backupLink)
    Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $existing.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'InstallerRollback original plugin bytes'
    Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $existing.destination 'WebView2Loader.dll'))) -eq 'CQgH') 'InstallerRollback original loader bytes'
    Assert-PackageTest ([IO.File]::ReadAllText($ini) -eq 'untouched settings') 'Installer never replaces INI'

    $serialized = New-InstallFixture 'serialized-target' -Existing
    $serializedInstall = Install-VerifiedPackage $package $serialized.root
    $targetLock = Enter-InstallerTargetLock $serialized.destination
    try {
        foreach ($operation in @('Install','Rollback')) {
            $runspace = [PowerShell]::Create()
            try {
                [void]$runspace.AddScript({param($installer,$archive,$root,$backup,$operation)
                    . $installer -PackagePath $archive
                    function Get-NotepadProcesses { @() }
                    if ($operation -eq 'Install') { Install-VerifiedPackage $archive $root }
                    else { Restore-PluginBackup $backup (Get-InstallTarget $root) }
                }).AddArgument($InstallerPath).AddArgument($package).AddArgument($serialized.root).AddArgument($serializedInstall.backupPath).AddArgument($operation)
                $lockError = ''
                try { $null = $runspace.Invoke() } catch { $lockError = $_.Exception.Message }
                Assert-PackageTest (($lockError + ($runspace.Streams.Error | Out-String)) -match 'Another installer or rollback') "Installer_TargetLockExcludesConcurrent$operation"
            } finally { $runspace.Dispose() }
        }
    } finally { $targetLock.ReleaseMutex(); $targetLock.Dispose() }
    Assert-PackageTest ((Restore-PluginBackup $serializedInstall.backupPath $serialized).status -eq 'RolledBack') 'Installer_TargetLockReleasedAfterConcurrentRejection'

    $concurrentOriginal = New-InstallFixture 'existing-concurrent-write' -Existing
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes,[Nullable[bool]]$ReplaceExisting=$null,[string]$ExpectedExistingSha256='') {
        if ([IO.Path]::GetFileName($Path) -ceq 'NppMarkdownNext.dll') { [IO.File]::WriteAllText($Path,'foreign update without a thrown error') }
        & $script:RealWriter $Path $Bytes $ReplaceExisting $ExpectedExistingSha256
    }
    $message = ''
    try { $null = Install-VerifiedPackage $package $concurrentOriginal.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($message -match 'original target changed before replacement' -and [IO.File]::ReadAllText((Join-Path $concurrentOriginal.destination 'NppMarkdownNext.dll')) -eq 'foreign update without a thrown error') 'Installer_OriginalChangedBeforeWritePreservesForeignBytes'
    ${function:Write-InstallerFile} = $script:RealWriter

    $script:RealWriter = ${function:Write-InstallerFile}
    $script:Writes = 0
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes,[Nullable[bool]]$ReplaceExisting=$null,[string]$ExpectedExistingSha256='') {
        $script:Writes++
        if ($script:Writes -eq 2) { throw 'Injected second write failure.' }
        & $script:RealWriter $Path $Bytes $ReplaceExisting $ExpectedExistingSha256
    }
    Assert-PackageThrows { Install-VerifiedPackage $package $existing.root } 'InstallerRollback second copy failure'
    Assert-PackageTest ([Convert]::ToBase64String([IO.File]::ReadAllBytes((Join-Path $existing.destination 'NppMarkdownNext.dll'))) -eq 'BwgJ') 'InstallerRollback restores first copy'
    $first = New-InstallFixture 'first-install'
    $script:Writes = 0
    Assert-PackageThrows { Install-VerifiedPackage $package $first.root } 'Installer_FirstInstallSecondCopyFailureRestoresAbsence'
    Assert-PackageTest (!(Test-Path -LiteralPath (Join-Path $first.destination 'NppMarkdownNext.dll')) -and !(Test-Path -LiteralPath (Join-Path $first.destination 'WebView2Loader.dll'))) 'Installer first-install original absence'

    function Write-InstallerFile([string]$Path,[byte[]]$Bytes,[Nullable[bool]]$ReplaceExisting=$null,[string]$ExpectedExistingSha256='') {
        $script:Writes++
        if ($script:Writes -ge 2) { throw 'Injected install and restore failure.' }
        & $script:RealWriter $Path $Bytes $ReplaceExisting $ExpectedExistingSha256
    }
    $script:Writes = 0; $message = ''
    try { $null = Install-VerifiedPackage $package $existing.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($message -match 'Recovery required\. Backup:') 'InstallerRollback failure recovery path'
    $foreign = New-InstallFixture 'foreign-before-write'
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes) {
        New-Item -ItemType Directory -Path (Split-Path $Path -Parent) -Force | Out-Null
        [IO.File]::WriteAllText($Path,'unrelated sentinel')
        throw 'Injected failure before the installer opened the destination.'
    }
    Assert-PackageThrows { Install-VerifiedPackage $package $foreign.root } 'Installer_FailureBeforeWritePreservesForeignFile'
    $foreignFile = Join-Path $foreign.destination 'NppMarkdownNext.dll'
    Assert-PackageTest ((Test-Path -LiteralPath $foreignFile) -and [IO.File]::ReadAllText($foreignFile) -eq 'unrelated sentinel') 'Installer_FailureBeforeWritePreservesForeignFile bytes'
    $identical = New-InstallFixture 'identical-before-write'
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes) {
        New-Item -ItemType Directory -Path (Split-Path $Path -Parent) -Force | Out-Null
        [IO.File]::WriteAllBytes($Path,$Bytes)
        throw 'A different writer created identical content before this installer opened it.'
    }
    $message = ''
    try { $null = Install-VerifiedPackage $package $identical.root } catch { $message = $_.Exception.Message }
    $identicalFile = Join-Path $identical.destination 'NppMarkdownNext.dll'
    Assert-PackageTest ((Test-Path -LiteralPath $identicalFile) -and $message -match 'Recovery required') 'Installer_FailedWriteDoesNotOwnMatchingForeignBytes'
    Assert-PackageThrows { & $script:RealWriter $foreignFile ([byte[]]@(4,5,6)) $false } 'Installer_AtomicCreateRejectsConcurrentDestination'
    Assert-PackageTest ([IO.File]::ReadAllText($foreignFile) -eq 'unrelated sentinel') 'Installer_AtomicCreatePreservesForeignBytes'
    $changedExisting = New-InstallFixture 'existing-foreign-before-write' -Existing
    function Write-InstallerFile([string]$Path,[byte[]]$Bytes) {
        [IO.File]::WriteAllText($Path,'foreign update to the original path')
        throw 'Injected failure before this installer committed anything.'
    }
    $message = ''
    try { $null = Install-VerifiedPackage $package $changedExisting.root } catch { $message = $_.Exception.Message }
    Assert-PackageTest ($message -match 'Recovery required' -and [IO.File]::ReadAllText((Join-Path $changedExisting.destination 'NppMarkdownNext.dll')) -eq 'foreign update to the original path') 'Installer_PreCommitFailureDoesNotOverwriteConcurrentUpdate'
    foreach ($retryCase in @('release','exhaust','foreign','staged')) {
        $script:RetryPath = Join-Path $scratch ('replace-retry-' + $retryCase + '.bin')
        [IO.File]::WriteAllBytes($script:RetryPath,[byte[]]@(7,8,9))
        $script:RetryLock = [IO.File]::Open($script:RetryPath,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
        $script:RetryCase = $retryCase; $script:RetrySleeps = 0; $script:RetryDelayTotal = 0
        function Start-Sleep([int]$Milliseconds) {
            $script:RetrySleeps++; $script:RetryDelayTotal += $Milliseconds
            if ($script:RetryCase -ne 'exhaust') {
                $script:RetryLock.Dispose(); $script:RetryLock = $null
                if ($script:RetryCase -eq 'foreign') { [IO.File]::WriteAllBytes($script:RetryPath,[byte[]]@(4,5,6)) }
                if ($script:RetryCase -eq 'staged') {
                    $staged = @(Get-ChildItem -LiteralPath $scratch -Filter ([IO.Path]::GetFileName($script:RetryPath) + '.*.tmp'))
                    Assert-PackageTest ($staged.Count -eq 1) 'Installer_AtomicReplaceStagedFixtureExistsDuringRetry'
                    [IO.File]::WriteAllBytes($staged[0].FullName,[byte[]]@(10,11,12))
                }
            }
        }
        try {
            $retryError = $null
            try { Write-InstallerBytesAtomically $script:RetryPath ([byte[]]@(1,2,3)) $true (Get-InstallerHash ([byte[]]@(7,8,9))) }
            catch { $retryError = $_.Exception }
            $actualBytes = [Convert]::ToBase64String([IO.File]::ReadAllBytes($script:RetryPath))
            if ($retryCase -eq 'release') {
                Assert-PackageTest (!$retryError -and $script:RetrySleeps -eq 1 -and $actualBytes -eq 'AQID') 'Installer_AtomicReplaceRetriesUntilReadLockReleases'
            } elseif ($retryCase -eq 'exhaust') {
                while ($retryError.InnerException) { $retryError = $retryError.InnerException }
                Assert-PackageTest ($retryError -and ($retryError.HResult -band 0xffff) -eq 32 -and $script:RetrySleeps -eq 9 -and $actualBytes -eq 'BwgJ') 'Installer_AtomicReplaceBoundedFailurePreservesOriginal'
            } elseif ($retryCase -eq 'foreign') {
                Assert-PackageTest ($retryError -and $retryError.Message -match 'original target changed before replacement' -and $script:RetrySleeps -eq 1 -and $actualBytes -eq 'BAUG') 'Installer_AtomicReplaceRechecksOriginalAfterRetryWait'
            } else {
                Assert-PackageTest ($retryError -and $retryError.Message -match 'staged installer bytes changed before commit' -and $script:RetrySleeps -eq 1 -and $actualBytes -eq 'BwgJ') 'Installer_AtomicReplaceRejectsStagedTamperingBeforeCommit'
            }
            Assert-PackageTest ($script:RetryDelayTotal -le 450 -and @(Get-ChildItem -LiteralPath $scratch -Filter ([IO.Path]::GetFileName($script:RetryPath) + '.*.tmp')).Count -eq 0) "Installer_AtomicReplaceRetryBoundAndCleanup $retryCase"
        } finally {
            if ($script:RetryLock) { $script:RetryLock.Dispose(); $script:RetryLock = $null }
            Remove-Item -LiteralPath Function:\Start-Sleep
        }
    }
    $script:HashRacePath = Join-Path $scratch 'staged-hash-source-race.bin'
    [IO.File]::WriteAllBytes($script:HashRacePath,[byte[]]@(7,8,9))
    function Get-FileHash([string]$LiteralPath,[string]$Algorithm) {
        if ($LiteralPath.StartsWith($script:HashRacePath + '.', [StringComparison]::OrdinalIgnoreCase)) {
            [IO.File]::WriteAllBytes($script:HashRacePath,[byte[]]@(4,5,6))
        }
        Microsoft.PowerShell.Utility\Get-FileHash -LiteralPath $LiteralPath -Algorithm $Algorithm
    }
    try {
        $message = ''
        try { Write-InstallerBytesAtomically $script:HashRacePath ([byte[]]@(1,2,3)) $true (Get-InstallerHash ([byte[]]@(7,8,9))) }
        catch { $message = $_.Exception.Message }
        Assert-PackageTest ($message -match 'original target changed before replacement' -and [Convert]::ToBase64String([IO.File]::ReadAllBytes($script:HashRacePath)) -eq 'BAUG') 'Installer_RechecksOriginalAfterHashingStagedBytes'
        Assert-PackageTest (@(Get-ChildItem -LiteralPath $scratch -Filter 'staged-hash-source-race.bin.*.tmp').Count -eq 0) 'Installer_StagedHashSourceRaceCleansTemporaryFile'
    } finally { Remove-Item -LiteralPath Function:\Get-FileHash }
    Write-Output 'PASS: InstallerSafePaths, InstallerRollback, bounded atomic-replace retries, loaded-script elevation integrity, public relative paths, ordinary-user public install/rollback, permission-only elevation, elevated-effect verification, elevated rollback receipts, immutable package selection, immutable rollback selection and bytes, target mutex, original-hash concurrency checks, UnicodeBackupTargetRoundTrips, target scoping, elevation data, first-install recovery, junction guards, foreign-file preservation'
} finally {
    foreach ($link in $script:InstallTestJunctions) { Remove-InstallTestJunction $link }
    $resolved = [IO.Path]::GetFullPath($scratch)
    if (!$resolved.StartsWith((Join-Path $project 'bin') + [IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)) { throw 'Unsafe installer test cleanup.' }
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
