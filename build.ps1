# Build script for NppMarkdownNext Native C++26 Edition
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# 1. Locate toolchain (clang++ and windres)
$clang64 = "clang++"
$clang32 = "i686-w64-mingw32-clang++"
$clangArm64 = "aarch64-w64-mingw32-clang++"
$windres64 = "windres"
$windres32 = "i686-w64-mingw32-windres"
$windresArm64 = "aarch64-w64-mingw32-windres"

if (-not (Get-Command $clang64 -ErrorAction SilentlyContinue)) {
    $wingetBin = Get-ChildItem -Path "$env:LOCALAPPDATA\Microsoft\WinGet\Packages" -Recurse -Filter "clang++.exe" -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty DirectoryName
    if ($wingetBin) {
        $env:PATH = "$wingetBin;$env:PATH"
    } else {
        Write-Error "clang++ compiler could not be found!"
        exit 1
    }
}

Write-Host ">>> Using Compiler: $(Get-Command $clang64 | Select-Object -ExpandProperty Source)" -ForegroundColor Cyan

# Ensure bin directories
$binDir = Join-Path $scriptDir "bin"
$bin32Dir = Join-Path $binDir "x86"
$binArm64Dir = Join-Path $binDir "arm64"
if (-not (Test-Path $binDir)) { New-Item -ItemType Directory -Path $binDir | Out-Null }
if (-not (Test-Path $bin32Dir)) { New-Item -ItemType Directory -Path $bin32Dir | Out-Null }
if (-not (Test-Path $binArm64Dir)) { New-Item -ItemType Directory -Path $binArm64Dir | Out-Null }

# Ensure WebView2 SDK package is present
$wv2Version = "1.0.4191.47"
$wv2PkgDir = Join-Path $scriptDir "packages\Microsoft.Web.WebView2.$wv2Version"
$wv2Header = Join-Path $wv2PkgDir "build\native\include\WebView2.h"
$wv2Loader64 = Join-Path $wv2PkgDir "build\native\x64\WebView2Loader.dll"
$wv2Loader32 = Join-Path $wv2PkgDir "build\native\x86\WebView2Loader.dll"
$wv2LoaderArm64 = Join-Path $wv2PkgDir "build\native\arm64\WebView2Loader.dll"

if (-not (Test-Path $wv2Header) -or -not (Test-Path $wv2Loader64) -or -not (Test-Path $wv2Loader32) -or -not (Test-Path $wv2LoaderArm64)) {
    Write-Host ">>> Restoring Microsoft.Web.WebView2 v$wv2Version..." -ForegroundColor Cyan
    $nugetCache = Join-Path $env:USERPROFILE ".nuget\packages\microsoft.web.webview2\$wv2Version"
    if (Test-Path "$nugetCache\build\native\include\WebView2.h") {
        New-Item -ItemType Directory -Path (Split-Path -Parent $wv2PkgDir) -Force | Out-Null
        Copy-Item -Path $nugetCache -Destination $wv2PkgDir -Recurse -Force
    } else {
        $tempZip = Join-Path $env:TEMP "webview2.$wv2Version.zip"
        $downloadUrl = "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/$wv2Version"
        try {
            [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12 -bor [Net.SecurityProtocolType]::Tls13
            Invoke-WebRequest -Uri $downloadUrl -OutFile $tempZip -UseBasicParsing
        } catch {
            Write-Host "Invoke-WebRequest failed, trying curl.exe..." -ForegroundColor Yellow
            & curl.exe -s -L -o $tempZip $downloadUrl
        }
        if (-not (Test-Path $tempZip) -or (Get-Item $tempZip).Length -lt 1000) {
            Write-Error "Failed to download WebView2 NuGet package from $downloadUrl"
            exit 1
        }
        New-Item -ItemType Directory -Path $wv2PkgDir -Force | Out-Null
        Expand-Archive -Path $tempZip -DestinationPath $wv2PkgDir -Force
        Remove-Item $tempZip -Force -ErrorAction SilentlyContinue
    }
}
$wv2Include = Join-Path $wv2PkgDir "build\native\include"

# 2. Compile and run unit tests
Write-Host "`n>>> [1/4] Building and running unit tests..." -ForegroundColor Yellow
$testExe = Join-Path $binDir "test_suite.exe"
$testSources = @(
    (Join-Path $scriptDir "tests\test_suite.cpp"),
    (Join-Path $scriptDir "src\MarkdownParser.cpp"),
    (Join-Path $scriptDir "src\BiDiEngine.cpp"),
    (Join-Path $scriptDir "src\SyntaxHighlighter.cpp"),
    (Join-Path $scriptDir "src\HtmlExporter.cpp")
)

& $clang64 -std=c++23 -O2 -I"$scriptDir\include" $testSources -luser32 -o "$testExe"
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to compile unit tests!"
    exit $LASTEXITCODE
}

Push-Location $scriptDir
try {
    & $testExe
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Unit tests failed!"
        exit $LASTEXITCODE
    }
} finally {
    Pop-Location
}

# 3. Build x64 DLL
Write-Host "`n>>> [2/4] Compiling NppMarkdownNext.dll (x64 Native)..." -ForegroundColor Yellow
$resFile64 = Join-Path $binDir "resource.res"
& $windres64 (Join-Path $scriptDir "res\NppMarkdownNext.rc") -O coff -o "$resFile64"

$dllPath64 = Join-Path $binDir "NppMarkdownNext.dll"
$pluginSources = @(
    (Join-Path $scriptDir "src\Main.cpp"),
    (Join-Path $scriptDir "src\NppMarkdownNext.cpp"),
    (Join-Path $scriptDir "src\WebView2Viewer.cpp"),
    (Join-Path $scriptDir "src\MarkdownRenderer.cpp"),
    (Join-Path $scriptDir "src\MarkdownParser.cpp"),
    (Join-Path $scriptDir "src\BiDiEngine.cpp"),
    (Join-Path $scriptDir "src\SyntaxHighlighter.cpp"),
    (Join-Path $scriptDir "src\HtmlExporter.cpp"),
    (Join-Path $scriptDir "src\Config.cpp"),
    $resFile64
)

$libs = @("-ld2d1", "-ldwrite", "-luser32", "-lgdi32", "-lcomctl32", "-lshlwapi", "-lole32", "-luuid", "-lcomdlg32")

& $clang64 -shared -std=c++23 -O3 -static -municode -I"$scriptDir\include" -I"$wv2Include" $pluginSources $libs -o "$dllPath64"
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to compile x64 NppMarkdownNext.dll!"
    exit $LASTEXITCODE
}
strip --strip-all "$dllPath64"

# Copy x64 WebView2Loader.dll to bin
if (Test-Path $wv2Loader64) {
    Copy-Item $wv2Loader64 -Destination (Join-Path $binDir "WebView2Loader.dll") -Force
}

Write-Host "Built x64 DLL: $dllPath64 ($( (Get-Item $dllPath64).Length / 1KB ) KB)" -ForegroundColor Green

# 4. Build x86 DLL (if 32-bit compiler is available)
if (Get-Command $clang32 -ErrorAction SilentlyContinue) {
    Write-Host "`n>>> [3/4] Compiling NppMarkdownNext.dll (x86 Native)..." -ForegroundColor Yellow
    $resFile32 = Join-Path $bin32Dir "resource.res"
    & $windres32 (Join-Path $scriptDir "res\NppMarkdownNext.rc") -O coff -o "$resFile32"

    $dllPath32 = Join-Path $bin32Dir "NppMarkdownNext.dll"
    $pluginSources32 = @(
        (Join-Path $scriptDir "src\Main.cpp"),
        (Join-Path $scriptDir "src\NppMarkdownNext.cpp"),
        (Join-Path $scriptDir "src\WebView2Viewer.cpp"),
        (Join-Path $scriptDir "src\MarkdownRenderer.cpp"),
        (Join-Path $scriptDir "src\MarkdownParser.cpp"),
        (Join-Path $scriptDir "src\BiDiEngine.cpp"),
        (Join-Path $scriptDir "src\SyntaxHighlighter.cpp"),
        (Join-Path $scriptDir "src\HtmlExporter.cpp"),
        (Join-Path $scriptDir "src\Config.cpp"),
        $resFile32
    )

    & $clang32 -shared -std=c++23 -O3 -static -municode -I"$scriptDir\include" -I"$wv2Include" $pluginSources32 $libs -o "$dllPath32"
    if ($LASTEXITCODE -eq 0) {
        if (Get-Command strip -ErrorAction SilentlyContinue) { strip --strip-all "$dllPath32" }
        elseif (Get-Command llvm-strip -ErrorAction SilentlyContinue) { llvm-strip --strip-all "$dllPath32" }

        # Copy x86 WebView2Loader.dll to bin\x86
        if (Test-Path $wv2Loader32) {
            Copy-Item $wv2Loader32 -Destination (Join-Path $bin32Dir "WebView2Loader.dll") -Force
        }

        Write-Host "Built x86 DLL: $dllPath32 ($( (Get-Item $dllPath32).Length / 1KB ) KB)" -ForegroundColor Green
    }
}

# 5. Build ARM64 DLL (if ARM64 compiler is available)
if (Get-Command $clangArm64 -ErrorAction SilentlyContinue) {
    Write-Host "`n>>> [4/4] Compiling NppMarkdownNext.dll (ARM64 Native)..." -ForegroundColor Yellow
    $resFileArm64 = Join-Path $binArm64Dir "resource.res"
    & $windresArm64 (Join-Path $scriptDir "res\NppMarkdownNext.rc") -O coff -o "$resFileArm64"

    $dllPathArm64 = Join-Path $binArm64Dir "NppMarkdownNext.dll"
    $pluginSourcesArm64 = @(
        (Join-Path $scriptDir "src\Main.cpp"),
        (Join-Path $scriptDir "src\NppMarkdownNext.cpp"),
        (Join-Path $scriptDir "src\WebView2Viewer.cpp"),
        (Join-Path $scriptDir "src\MarkdownRenderer.cpp"),
        (Join-Path $scriptDir "src\MarkdownParser.cpp"),
        (Join-Path $scriptDir "src\BiDiEngine.cpp"),
        (Join-Path $scriptDir "src\SyntaxHighlighter.cpp"),
        (Join-Path $scriptDir "src\HtmlExporter.cpp"),
        (Join-Path $scriptDir "src\Config.cpp"),
        $resFileArm64
    )

    & $clangArm64 -shared -std=c++23 -O3 -static -municode -I"$scriptDir\include" -I"$wv2Include" $pluginSourcesArm64 $libs -o "$dllPathArm64"
    if ($LASTEXITCODE -eq 0) {
        if (Get-Command llvm-strip -ErrorAction SilentlyContinue) { llvm-strip --strip-all "$dllPathArm64" }
        elseif (Get-Command strip -ErrorAction SilentlyContinue) { strip --strip-all "$dllPathArm64" }

        # Copy arm64 WebView2Loader.dll to bin\arm64
        if (Test-Path $wv2LoaderArm64) {
            Copy-Item $wv2LoaderArm64 -Destination (Join-Path $binArm64Dir "WebView2Loader.dll") -Force
        }

        Write-Host "Built ARM64 DLL: $dllPathArm64 ($( (Get-Item $dllPathArm64).Length / 1KB ) KB)" -ForegroundColor Green
    }
}

Write-Host "`n[SUCCESS] Build completed successfully!" -ForegroundColor Green