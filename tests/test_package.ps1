param([switch]$HelpersOnly)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression
$script:PackageTestProject = Split-Path $PSScriptRoot -Parent
. (Join-Path $script:PackageTestProject 'makerelease.ps1') -FunctionsOnly

function Assert-PackageTest([bool]$Condition, [string]$Name) { if (!$Condition) { throw "FAIL: $Name" } }
function Assert-PackageThrows([scriptblock]$Action, [string]$Name) {
    $failed = $false
    try { $null = & $Action } catch { $failed = $true }
    Assert-PackageTest $failed $Name
}
function New-TestPe([string]$Architecture='x64') {
    $bytes = [byte[]]::new(128)
    [Array]::Copy([BitConverter]::GetBytes([uint16]0x5a4d),0,$bytes,0,2)
    [Array]::Copy([BitConverter]::GetBytes([int32]64),0,$bytes,0x3c,4)
    [Array]::Copy([BitConverter]::GetBytes([uint32]0x4550),0,$bytes,64,4)
    $machine = switch ($Architecture) { x64 { 0x8664 } x86 { 0x14c } arm64 { 0xaa64 } }
    [Array]::Copy([BitConverter]::GetBytes([uint16]$machine),0,$bytes,68,2)
    ,$bytes
}
function New-TestPackage([string]$Directory, [string]$Architecture='x64') {
    New-Item -ItemType Directory -Path $Directory -Force | Out-Null
    $manifest = Get-Content -LiteralPath (Join-Path $script:PackageTestProject 'tests\fixtures\package-manifest.json') -Raw | ConvertFrom-Json
    $manifest.architecture = $Architecture
    $contents = [ordered]@{'NppMarkdownNext.dll'=(New-TestPe $Architecture);'WebView2Loader.dll'=(New-TestPe $Architecture);
        'README.md'=[Text.Encoding]::UTF8.GetBytes('readme');'License.txt'=[Text.Encoding]::UTF8.GetBytes('license');
        'THIRD_PARTY_NOTICES.md'=[Text.Encoding]::UTF8.GetBytes('notices');'themes/BEARDED_LICENSE'=[Text.Encoding]::UTF8.GetBytes('theme license');
        'provenance.json'=[Text.Encoding]::UTF8.GetBytes('{}')}
    $manifest.files = @()
    foreach ($entry in $contents.GetEnumerator()) {
        $sha = [Security.Cryptography.SHA256]::Create()
        try { $hash = [BitConverter]::ToString($sha.ComputeHash([byte[]]$entry.Value)).Replace('-','').ToLowerInvariant() }
        finally { $sha.Dispose() }
        $manifest.files += [pscustomobject]@{path=$entry.Key;size=$entry.Value.Length;sha256=$hash}
    }
    $contents['manifest.json'] = [Text.Encoding]::UTF8.GetBytes(($manifest | ConvertTo-Json -Depth 6))
    $path = Join-Path $Directory ([Guid]::NewGuid().ToString('N') + '.zip')
    $stream = [IO.File]::Create($path)
    $zip = $null
    try {
        $zip = [IO.Compression.ZipArchive]::new($stream,[IO.Compression.ZipArchiveMode]::Create)
        foreach ($entry in $contents.GetEnumerator()) {
            $output = $zip.CreateEntry($entry.Key).Open()
            try { $output.Write($entry.Value,0,$entry.Value.Length) } finally { $output.Dispose() }
        }
    } finally { if ($zip) { $zip.Dispose() }; $stream.Dispose() }
    $path
}
function Add-TestZipEntry([string]$Archive, [string]$Name, [byte[]]$Bytes) {
    $zip = [IO.Compression.ZipFile]::Open($Archive,[IO.Compression.ZipArchiveMode]::Update)
    try { $stream = $zip.CreateEntry($Name).Open(); try { $stream.Write($Bytes,0,$Bytes.Length) } finally { $stream.Dispose() } }
    finally { $zip.Dispose() }
}
if ($HelpersOnly -or $MyInvocation.InvocationName -eq '.') { return }
$scratch = Join-Path $script:PackageTestProject ('bin\package-test-' + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $scratch | Out-Null
try {
    $shell = (Get-Process -Id $PID).Path
    $emptyReleaseRoot = Join-Path $scratch 'empty-release'
    New-Item -ItemType Directory -Path $emptyReleaseRoot | Out-Null
    $emptyReleaseScript = Join-Path $emptyReleaseRoot 'makerelease.ps1'
    Copy-Item -LiteralPath (Join-Path $script:PackageTestProject 'makerelease.ps1') -Destination $emptyReleaseScript
    $releaseProcess = Start-Process -FilePath $shell -WindowStyle Hidden -Wait -PassThru -ArgumentList @('-NoProfile','-File',$emptyReleaseScript) -RedirectStandardOutput (Join-Path $scratch 'empty-release.out') -RedirectStandardError (Join-Path $scratch 'empty-release.err')
    $releaseError = Get-Content -LiteralPath (Join-Path $scratch 'empty-release.err') -Raw
    Assert-PackageTest ($releaseProcess.ExitCode -ne 0 -and $releaseError -match 'no release packages were produced') 'Package_EmptyReleaseCommandFails'
    Assert-PackageTest (!(Test-Path -LiteralPath (Join-Path $emptyReleaseRoot 'Release'))) 'Package_EmptyReleaseCommandCreatesNoPackage'

    foreach ($architecture in @('x64','x86','arm64')) {
        $package = New-TestPackage $scratch $architecture
        $manifest = Assert-PluginArchive $package
        Assert-PackageTest ($manifest.architecture -eq $architecture) 'PackageArchiveContract architecture'
        Assert-PackageTest (@($manifest.files | Where-Object path -eq 'themes/BEARDED_LICENSE').Count -eq 1) 'PackageAdditionalFilesAllowed'
    }
    foreach ($bad in @('NppMarkdownNext/NppMarkdownNext.dll','nppmarkdownnext.dll','../outside.dll','C:/outside.dll')) {
        $package = New-TestPackage $scratch
        Add-TestZipEntry $package $bad ([byte[]]@(1))
        Assert-PackageThrows { Assert-PluginArchive $package } "PackageArchiveContract $bad"
    }
    $package = New-TestPackage $scratch
    $zip = [IO.Compression.ZipFile]::Open($package,[IO.Compression.ZipArchiveMode]::Update)
    try { $zip.GetEntry('WebView2Loader.dll').Delete() } finally { $zip.Dispose() }
    Assert-PackageThrows { Assert-PluginArchive $package } 'PackageArchiveContract missing loader'

    $fixtureRoot = Join-Path $scratch 'source'
    & git clone --quiet --shared --no-checkout $script:PackageTestProject $fixtureRoot
    if ($LASTEXITCODE -ne 0) { throw 'Cannot prepare the isolated source fixture.' }
    $fixtureReleaseScript = Join-Path $fixtureRoot 'makerelease.ps1'
    Copy-Item -LiteralPath (Join-Path $script:PackageTestProject 'makerelease.ps1') -Destination $fixtureReleaseScript
    foreach ($path in @('README.md','License.txt','THIRD_PARTY_NOTICES.md','themes/BEARDED_LICENSE','src/probe.cpp')) {
        $file = Join-Path $fixtureRoot $path
        New-Item -ItemType Directory -Path (Split-Path $file -Parent) -Force | Out-Null
        [IO.File]::WriteAllText($file,'fixture')
    }
    $script:PluginRoot = $fixtureRoot
    $binary = Join-Path $fixtureRoot 'bin'
    New-Item -ItemType Directory -Path $binary | Out-Null
    foreach ($name in @('NppMarkdownNext.dll','WebView2Loader.dll')) { [IO.File]::WriteAllBytes((Join-Path $binary $name),(New-TestPe)) }
    Start-PluginBuild $binary
    $null = Write-PluginBuildInfo 'x64' $binary
    $archive = New-PluginArchive 'x64' $binary
    $null = Assert-PluginArchive $archive
    Remove-Item -LiteralPath $archive
    $releaseProcess = Start-Process -FilePath $shell -WindowStyle Hidden -Wait -PassThru -ArgumentList @('-NoProfile','-File',$fixtureReleaseScript) -RedirectStandardOutput (Join-Path $scratch 'single-release.out') -RedirectStandardError (Join-Path $scratch 'single-release.err')
    Assert-PackageTest ($releaseProcess.ExitCode -eq 0 -and (Test-Path -LiteralPath $archive)) 'Package_SingleArchitectureReleaseCommandSucceeds'
    $null = Assert-PluginArchive $archive
    Start-PluginBuild $binary
    Assert-PackageTest (!(Test-Path -LiteralPath (Join-Path $binary 'build-info.json'))) 'Package_FailedRebuildCannotReusePreviousInfo'
    [IO.File]::AppendAllText((Join-Path $fixtureRoot 'src/probe.cpp'),'changed')
    Assert-PackageThrows { Write-PluginBuildInfo 'x64' $binary } 'Package_SourceChangedDuringCompileRejected'
    Start-PluginBuild $binary; $null = Write-PluginBuildInfo 'x64' $binary
    [IO.File]::AppendAllText((Join-Path $binary 'NppMarkdownNext.dll'),'changed')
    Assert-PackageThrows { New-PluginArchive 'x64' $binary } 'PackageArchiveContract changed binary'

    $buildErrors = $null
    $buildAst = [Management.Automation.Language.Parser]::ParseFile((Join-Path $script:PackageTestProject 'build.ps1'),[ref]$null,[ref]$buildErrors)
    Assert-PackageTest (!$buildErrors) 'Package_BuildScriptParses'
    $failedCompiler = Join-Path $scratch 'failed-compiler.cmd'
    [IO.File]::WriteAllText($failedCompiler,"@exit /b 41`r`n")
    foreach ($architecture in @('x86','ARM64')) {
        $compilerVariable = if ($architecture -eq 'x86') { '$clang32' } else { '$clangArm64' }
        $branch = @($buildAst.EndBlock.Statements | Where-Object { $_.Extent.Text.StartsWith('if (Get-Command ' + $compilerVariable + ' ') })
        Assert-PackageTest ($branch.Count -eq 1) "Package_CompilerBranchFound $architecture"
        $failedOutput = Join-Path $binary $architecture
        New-Item -ItemType Directory -Path $failedOutput | Out-Null
        $staleInfo = Join-Path $failedOutput 'build-info.json'
        [IO.File]::WriteAllText($staleInfo,'stale build receipt')
        $message = & {
            $scriptDir = $fixtureRoot; $bin32Dir = $failedOutput; $binArm64Dir = $failedOutput
            $clang32 = $failedCompiler; $clangArm64 = $failedCompiler; $libs = @(); $wv2Include = $fixtureRoot
            function Invoke-ResourceCompile { }
            try { & ([scriptblock]::Create($branch[0].Extent.Text)) } catch { $_.Exception.Message }
        }
        Assert-PackageTest ($message -match "Failed to compile $architecture NppMarkdownNext.dll \(exit 41\)") "Package_AttemptedCompilerFailureFailsBuild $architecture"
        Assert-PackageTest (!(Test-Path -LiteralPath $staleInfo)) "Package_FailedCompilerInvalidatesStaleBuildReceipt $architecture"
    }
    Write-Output 'PASS: PackageArchiveContract, PackageAdditionalFilesAllowed, build provenance failures, empty-release failure, single-architecture release, optional compiler failures'
} finally {
    $script:PluginRoot = $script:PackageTestProject
    $resolved = [IO.Path]::GetFullPath($scratch)
    if (!$resolved.StartsWith((Join-Path $script:PackageTestProject 'bin') + [IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)) { throw 'Unsafe test cleanup.' }
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
