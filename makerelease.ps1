# Release Packaging Script for NppMarkdownNext
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

$releaseDir = Join-Path $scriptDir "Release"
if (Test-Path $releaseDir) { Remove-Item -Recurse -Force $releaseDir }
New-Item -ItemType Directory -Path $releaseDir | Out-Null

$version = "1.0.0"

# Package x64
$x64Dll = Join-Path $scriptDir "bin\NppMarkdownNext.dll"
$wv2Loader64 = Join-Path $scriptDir "bin\WebView2Loader.dll"
if (Test-Path $x64Dll) {
    $zipX64 = Join-Path $releaseDir "NppMarkdownNext-$version-x64.zip"
    $stageDir64 = Join-Path $releaseDir "stage_x64\NppMarkdownNext"
    New-Item -ItemType Directory -Path $stageDir64 -Force | Out-Null
    Copy-Item $x64Dll -Destination (Join-Path $stageDir64 "NppMarkdownNext.dll")
    if (Test-Path $wv2Loader64) {
        Copy-Item $wv2Loader64 -Destination (Join-Path $stageDir64 "WebView2Loader.dll")
    }
    Copy-Item (Join-Path $scriptDir "README.md") -Destination $stageDir64
    Copy-Item (Join-Path $scriptDir "License.txt") -Destination $stageDir64

    Compress-Archive -Path "$stageDir64" -DestinationPath $zipX64 -Force
    Remove-Item -Recurse -Force (Join-Path $releaseDir "stage_x64")
    Write-Host "Created Release Archive: $zipX64 ($( (Get-Item $zipX64).Length / 1KB ) KB)" -ForegroundColor Green
}

# Package x86
$x86Dll = Join-Path $scriptDir "bin\x86\NppMarkdownNext.dll"
$wv2Loader86 = Join-Path $scriptDir "bin\x86\WebView2Loader.dll"
if (Test-Path $x86Dll) {
    $zipX86 = Join-Path $releaseDir "NppMarkdownNext-$version-x86.zip"
    $stageDir86 = Join-Path $releaseDir "stage_x86\NppMarkdownNext"
    New-Item -ItemType Directory -Path $stageDir86 -Force | Out-Null
    Copy-Item $x86Dll -Destination (Join-Path $stageDir86 "NppMarkdownNext.dll")
    if (Test-Path $wv2Loader86) {
        Copy-Item $wv2Loader86 -Destination (Join-Path $stageDir86 "WebView2Loader.dll")
    }
    Copy-Item (Join-Path $scriptDir "README.md") -Destination $stageDir86
    Copy-Item (Join-Path $scriptDir "License.txt") -Destination $stageDir86

    Compress-Archive -Path "$stageDir86" -DestinationPath $zipX86 -Force
    Remove-Item -Recurse -Force (Join-Path $releaseDir "stage_x86")
    Write-Host "Created Release Archive: $zipX86 ($( (Get-Item $zipX86).Length / 1KB ) KB)" -ForegroundColor Green
}

Write-Host "[SUCCESS] Release packages created in $releaseDir" -ForegroundColor Cyan