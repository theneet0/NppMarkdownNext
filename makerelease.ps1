# Release packaging and the build provenance shared with build.ps1.
param([switch]$FunctionsOnly)
$ErrorActionPreference = 'Stop'
$script:PluginRoot = $PSScriptRoot
$script:PluginVersion = '1.6.0'
Add-Type -AssemblyName System.IO.Compression.FileSystem
Add-Type -AssemblyName System.IO.Compression

function Get-PluginSourceState {
    $scope = @('src','include','res','tools','themes','tests','build.ps1','makerelease.ps1','License.txt','README.md','THIRD_PARTY_NOTICES.md')
    $paths = @(& git -c core.quotepath=false -C $script:PluginRoot ls-files -co --exclude-standard -- @scope)
    if ($LASTEXITCODE -ne 0) { throw 'Cannot enumerate the build inputs.' }
    $unique = [Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
    foreach ($path in $paths) { [void]$unique.Add($path.Replace('\','/')) }
    $sorted = [string[]]@($unique)
    [Array]::Sort($sorted, [StringComparer]::Ordinal)
    $record = [Text.StringBuilder]::new()
    foreach ($path in $sorted) {
        $file = Get-Item -LiteralPath (Join-Path $script:PluginRoot $path)
        $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash.ToLowerInvariant()
        [void]$record.Append($path).Append([char]0).Append($file.Length).Append([char]0).Append($hash).Append([char]10)
    }
    $sha = [Security.Cryptography.SHA256]::Create()
    try { $digest = [BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($record.ToString()))).Replace('-','').ToLowerInvariant() }
    finally { $sha.Dispose() }
    $commit = & git -C $script:PluginRoot rev-parse HEAD
    if ($LASTEXITCODE -ne 0) { throw 'Cannot identify the source commit.' }
    $dirty = @(& git -C $script:PluginRoot status --porcelain --untracked-files=normal -- @scope).Count -gt 0
    [pscustomobject]@{ commit=$commit; treeDigest=$digest; dirty=$dirty }
}

function Get-PluginPeArchitecture([string]$Path) {
    $file = [IO.File]::OpenRead($Path)
    $reader = [IO.BinaryReader]::new($file)
    try {
        if ($file.Length -lt 64 -or $reader.ReadUInt16() -ne 0x5a4d) { throw "Invalid PE file: $Path" }
        $file.Position = 0x3c
        $offset = $reader.ReadInt32()
        if ($offset -lt 64 -or $offset + 6 -gt $file.Length) { throw "Invalid PE header: $Path" }
        $file.Position = $offset
        if ($reader.ReadUInt32() -ne 0x4550) { throw "Invalid PE signature: $Path" }
        switch ($reader.ReadUInt16()) { 0x8664 { 'x64' } 0x14c { 'x86' } 0xaa64 { 'arm64' } default { throw "Unsupported PE architecture: $Path" } }
    } finally { $reader.Dispose(); $file.Dispose() }
}

function Write-PluginBuildInfo([string]$Architecture, [string]$BinaryDirectory) {
    $infoPath = Join-Path $BinaryDirectory 'build-info.json'
    $after = Get-PluginSourceState
    if (!$script:PluginSourceBefore -or $after.commit -ne $script:PluginSourceBefore.commit -or
        $after.treeDigest -ne $script:PluginSourceBefore.treeDigest) { throw 'Sources changed during compilation; no build-info was written.' }
    $files = @()
    foreach ($name in @('NppMarkdownNext.dll','WebView2Loader.dll','README.md','License.txt','THIRD_PARTY_NOTICES.md','themes/BEARDED_LICENSE')) {
        $path = if ($name.EndsWith('.dll')) { Join-Path $BinaryDirectory $name } else { Join-Path $script:PluginRoot $name }
        $file = Get-Item -LiteralPath $path
        if ($name.EndsWith('.dll') -and (Get-PluginPeArchitecture $path) -ne $Architecture) { throw "Wrong $Architecture binary: $name" }
        $files += [pscustomobject]@{path=$name; size=$file.Length; sha256=(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()}
    }
    $info = [pscustomobject]@{schemaVersion=1; product='NppMarkdownNext'; version=$script:PluginVersion;
        architecture=$Architecture; source=$after; builtAtUtc=[DateTime]::UtcNow.ToString('o'); files=$files}
    [IO.File]::WriteAllText($infoPath, ($info | ConvertTo-Json -Depth 6), [Text.UTF8Encoding]::new($false))
    $info
}

function Start-PluginBuild([string]$BinaryDirectory) {
    $info = Join-Path $BinaryDirectory 'build-info.json'
    if (Test-Path -LiteralPath $info) { Remove-Item -LiteralPath $info -Force }
    $script:PluginSourceBefore = Get-PluginSourceState
}

function Assert-PluginArchive([string]$ArchivePath) {
    $zip = [IO.Compression.ZipFile]::OpenRead($ArchivePath)
    try {
        $entries = @{}
        foreach ($entry in $zip.Entries) {
            $name = $entry.FullName
            if ($name -ceq 'themes/') { continue }
            if ($name -cnotin @('NppMarkdownNext.dll','WebView2Loader.dll','README.md','License.txt','THIRD_PARTY_NOTICES.md','themes/BEARDED_LICENSE','provenance.json','build-info.json','manifest.json') -or $entries.ContainsKey($name)) { throw "Unsafe or duplicate ZIP entry: $name" }
            $entries[$name] = $entry
        }
        foreach ($name in @('NppMarkdownNext.dll','WebView2Loader.dll','manifest.json')) {
            if (!$entries.ContainsKey($name) -or $entries[$name].FullName -cne $name) { throw "Required root entry is missing: $name" }
        }
        $reader = [IO.StreamReader]::new($entries['manifest.json'].Open())
        try { $manifest = $reader.ReadToEnd() | ConvertFrom-Json } finally { $reader.Dispose() }
        if ($manifest.schemaVersion -ne 1 -or $manifest.product -ne 'NppMarkdownNext' -or
            $manifest.architecture -notin @('x64','x86','arm64') -or !$manifest.version -or
            $manifest.source.commit -notmatch '^[0-9a-f]{40}$' -or $manifest.source.treeDigest -notmatch '^[0-9a-f]{64}$' -or
            $manifest.source.dirty -isnot [bool]) { throw 'Invalid package manifest.' }
        $seen = @{}
        foreach ($file in $manifest.files) {
            $name = [string]$file.path
            if ($name -eq 'manifest.json' -or !$entries.ContainsKey($name) -or $seen.ContainsKey($name) -or
                $entries[$name].FullName -cne $name -or $file.sha256 -notmatch '^[0-9a-f]{64}$') { throw "Invalid manifest file: $name" }
            $seen[$name] = $true
            $stream = $entries[$name].Open(); $sha = [Security.Cryptography.SHA256]::Create()
            try { $hash = [BitConverter]::ToString($sha.ComputeHash($stream)).Replace('-','').ToLowerInvariant() }
            finally { $sha.Dispose(); $stream.Dispose() }
            if ($hash -cne $file.sha256 -or $entries[$name].Length -ne $file.size) { throw "Package hash/size mismatch: $name" }
        }
        if (!$seen.ContainsKey('NppMarkdownNext.dll') -or !$seen.ContainsKey('WebView2Loader.dll') -or
            $seen.Count -ne $entries.Count - 1) { throw 'The manifest does not cover all package files.' }
        $manifest
    } finally { $zip.Dispose() }
}

function New-PluginArchive([string]$Architecture, [string]$BinaryDirectory) {
    $infoPath = Join-Path $BinaryDirectory 'build-info.json'
    $info = Get-Content -LiteralPath $infoPath -Raw | ConvertFrom-Json
    $state = Get-PluginSourceState
    if ($info.architecture -ne $Architecture -or $info.source.commit -ne $state.commit -or
        $info.source.treeDigest -ne $state.treeDigest) { throw 'Stale build-info; rebuild before packaging.' }
    $release = Join-Path $script:PluginRoot 'Release'
    New-Item -ItemType Directory -Path $release -Force | Out-Null
    $stage = Join-Path $release ('stage-' + $Architecture + '-' + [Guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $stage | Out-Null
    $zipPath = Join-Path $release "NppMarkdownNext-$($info.version)-$Architecture.zip"
    $temporaryZip = Join-Path $release ([Guid]::NewGuid().ToString('N') + '.zip')
    try {
        foreach ($file in $info.files) {
            $name = [string]$file.path
            if ($name -cnotin @('NppMarkdownNext.dll','WebView2Loader.dll','README.md','License.txt','THIRD_PARTY_NOTICES.md','themes/BEARDED_LICENSE','provenance.json','build-info.json')) { throw 'Invalid build-info filename.' }
            $source = if ($name.EndsWith('.dll')) { Join-Path $BinaryDirectory $name } else { Join-Path $script:PluginRoot $name }
            if ((Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash.ToLowerInvariant() -cne $file.sha256 -or
                (Get-Item -LiteralPath $source).Length -ne $file.size) { throw "Build output changed: $name" }
            if ($name.EndsWith('.dll') -and (Get-PluginPeArchitecture $source) -ne $Architecture) { throw 'Wrong package architecture.' }
            $stagedPath = Join-Path $stage $name
            New-Item -ItemType Directory -Path (Split-Path $stagedPath -Parent) -Force | Out-Null
            Copy-Item -LiteralPath $source -Destination $stagedPath
        }
        Copy-Item -LiteralPath $infoPath -Destination (Join-Path $stage 'manifest.json')
        $zip = [IO.Compression.ZipFile]::Open($temporaryZip,[IO.Compression.ZipArchiveMode]::Create)
        try {
            if (@($info.files | Where-Object { $_.path.StartsWith('themes/') }).Count) { $null = $zip.CreateEntry('themes/') }
            foreach ($name in (@($info.files.path) + @('manifest.json'))) {
                $null = [IO.Compression.ZipFileExtensions]::CreateEntryFromFile($zip,(Join-Path $stage $name),$name,[IO.Compression.CompressionLevel]::Optimal)
            }
        } finally { $zip.Dispose() }
        $null = Assert-PluginArchive $temporaryZip
        Move-Item -LiteralPath $temporaryZip -Destination $zipPath -Force
        $zipPath
    } finally {
        $resolved = [IO.Path]::GetFullPath($stage)
        if (!$resolved.StartsWith(([IO.Path]::GetFullPath($release) + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase)) { throw 'Unsafe staging cleanup path.' }
        Remove-Item -LiteralPath $resolved -Recurse -Force
        if (Test-Path -LiteralPath $temporaryZip) { Remove-Item -LiteralPath $temporaryZip -Force }
    }
}

if ($FunctionsOnly -or $MyInvocation.InvocationName -eq '.') { return }
$packageCount = 0
foreach ($entry in @(@('x64','bin'),@('x86','bin\x86'),@('arm64','bin\arm64'))) {
    $binaryDirectory = Join-Path $script:PluginRoot $entry[1]
    if (Test-Path -LiteralPath (Join-Path $binaryDirectory 'NppMarkdownNext.dll')) {
        $archive = New-PluginArchive $entry[0] $binaryDirectory
        $packageCount++
        Write-Host "Created verified package: $archive" -ForegroundColor Green
    }
}
if (!$packageCount) { throw 'No plugin binaries were found; no release packages were produced. Run build.ps1 first.' }
