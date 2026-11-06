[CmdletBinding()]
param(
    [string]$NppExe = 'C:\Program Files\Notepad++\notepad++.exe',
    [string]$BinaryDirectory = '',
    [switch]$KeepOpen,
    [switch]$Show
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
if (!$BinaryDirectory) { $BinaryDirectory = Join-Path $repo 'bin' }
$probe = Join-Path $PSScriptRoot 'native_reader_probe.cjs'
$node = (Get-Command node -ErrorAction Stop).Source
$NppExe = (Resolve-Path -LiteralPath $NppExe).Path
$binary = (Resolve-Path -LiteralPath $BinaryDirectory).Path
$base = Join-Path $repo 'bin\native-smoke'
$runRoot = Join-Path $base ('run-' + [Guid]::NewGuid().ToString('N'))
if (![IO.Path]::GetFullPath($runRoot).StartsWith([IO.Path]::GetFullPath($base) + '\', [StringComparison]::OrdinalIgnoreCase)) {
    throw 'Unsafe smoke workspace.'
}
New-Item -ItemType Directory -Path $runRoot -Force | Out-Null

if (-not ('NppSmokeApi' -as [type])) {
    Add-Type -TypeDefinition @'
using System;
using System.Text;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
public static class NppSmokeApi {
    private delegate bool EnumProc(IntPtr window, IntPtr data);
    [DllImport("user32.dll")] private static extern bool EnumWindows(EnumProc callback, IntPtr data);
    [DllImport("user32.dll")] private static extern bool EnumChildWindows(IntPtr parent, EnumProc callback, IntPtr data);
    [DllImport("user32.dll")] private static extern uint GetWindowThreadProcessId(IntPtr window, out uint process);
    [DllImport("user32.dll", CharSet=CharSet.Unicode)] private static extern int GetClassNameW(IntPtr window, StringBuilder text, int count);
    [DllImport("user32.dll", SetLastError=true)] private static extern IntPtr SendMessageTimeoutW(IntPtr window, uint message, UIntPtr wParam, IntPtr lParam, uint flags, uint timeout, out UIntPtr result);
    [DllImport("user32.dll", CharSet=CharSet.Unicode)] private static extern int GetMenuStringW(IntPtr menu, uint item, StringBuilder text, int count, uint flags);
    [DllImport("user32.dll")] private static extern int GetMenuItemCount(IntPtr menu);
    [DllImport("user32.dll")] private static extern IntPtr GetSubMenu(IntPtr menu, int position);
    [DllImport("user32.dll")] private static extern uint GetMenuItemID(IntPtr menu, int position);
    [DllImport("user32.dll")] private static extern int GetWindowLongW(IntPtr window, int index);
    [DllImport("user32.dll")] public static extern IntPtr GetDlgItem(IntPtr window, int id);
    [DllImport("user32.dll")] public static extern bool IsWindowEnabled(IntPtr window);
    [DllImport("kernel32.dll", SetLastError=true)] private static extern IntPtr OpenProcess(uint access, bool inherit, uint process);
    [DllImport("kernel32.dll", SetLastError=true)] private static extern IntPtr VirtualAllocEx(IntPtr process, IntPtr address, UIntPtr size, uint allocation, uint protection);
    [DllImport("kernel32.dll", SetLastError=true)] private static extern bool VirtualFreeEx(IntPtr process, IntPtr address, UIntPtr size, uint freeType);
    [DllImport("kernel32.dll", SetLastError=true)] private static extern bool WriteProcessMemory(IntPtr process, IntPtr address, byte[] data, UIntPtr size, out UIntPtr written);
    [DllImport("kernel32.dll", SetLastError=true)] private static extern bool ReadProcessMemory(IntPtr process, IntPtr address, byte[] data, UIntPtr size, out UIntPtr read);
    [DllImport("kernel32.dll")] private static extern bool CloseHandle(IntPtr handle);
    public static string ClassName(IntPtr window) { var text = new StringBuilder(256); GetClassNameW(window,text,text.Capacity); return text.ToString(); }
    public static IntPtr FindMain(uint process) {
        IntPtr found=IntPtr.Zero;
        EnumWindows((window,data) => { uint owner; GetWindowThreadProcessId(window,out owner); if(owner==process && ClassName(window)=="Notepad++") found=window; return true; },IntPtr.Zero);
        return found;
    }
    public static IntPtr[] Children(IntPtr parent,string className) {
        var windows=new List<IntPtr>();
        EnumChildWindows(parent,(window,data)=>{if(ClassName(window)==className)windows.Add(window);return true;},IntPtr.Zero);
        return windows.ToArray();
    }
    public static bool OwnVisible(IntPtr window) { return window!=IntPtr.Zero && (GetWindowLongW(window,-16)&0x10000000)!=0; }
    public static long Scalar(uint process,IntPtr window,uint message,ulong wParam,long lParam) {
        uint owner; GetWindowThreadProcessId(window,out owner);
        if(owner!=process)throw new InvalidOperationException("SDK target is not owned by the isolated process");
        UIntPtr result;
        if(SendMessageTimeoutW(window,message,new UIntPtr(wParam),new IntPtr(lParam),2,5000,out result)==IntPtr.Zero)
            throw new Win32Exception(Marshal.GetLastWin32Error(),"Isolated SDK command failed or timed out");
        return unchecked((long)result.ToUInt64());
    }
    private static byte[] Remote(uint process,IntPtr window,uint message,ulong wParam,byte[] initial,bool readBack) {
        const uint access=0x0008|0x0010|0x0020|0x1000;
        IntPtr handle=OpenProcess(access,false,process);
        if(handle==IntPtr.Zero)throw new Win32Exception(Marshal.GetLastWin32Error());
        IntPtr address=IntPtr.Zero;
        try {
            address=VirtualAllocEx(handle,IntPtr.Zero,new UIntPtr((uint)initial.Length),0x1000|0x2000,4);
            if(address==IntPtr.Zero)throw new Win32Exception(Marshal.GetLastWin32Error());
            UIntPtr transferred;
            if(!WriteProcessMemory(handle,address,initial,new UIntPtr((uint)initial.Length),out transferred)||transferred.ToUInt64()!=(ulong)initial.Length)
                throw new Win32Exception(Marshal.GetLastWin32Error(),"Remote SDK argument write failed");
            long result=Scalar(process,window,message,wParam,address.ToInt64());
            if(!readBack)return BitConverter.GetBytes(result);
            var bytes=new byte[initial.Length];
            if(!ReadProcessMemory(handle,address,bytes,new UIntPtr((uint)bytes.Length),out transferred)||transferred.ToUInt64()!=(ulong)bytes.Length)
                throw new Win32Exception(Marshal.GetLastWin32Error(),"Remote SDK result read failed");
            return bytes;
        } finally { if(address!=IntPtr.Zero)VirtualFreeEx(handle,address,UIntPtr.Zero,0x8000);CloseHandle(handle); }
    }
    public static long PathCommand(uint process,IntPtr window,uint message,ulong wParam,string path) {
        return BitConverter.ToInt64(Remote(process,window,message,wParam,Encoding.Unicode.GetBytes(path+"\0"),false),0);
    }
    public static string ReadPath(uint process,IntPtr window,uint message) {
        return Encoding.Unicode.GetString(Remote(process,window,message,4096,new byte[8192],true)).Split('\0')[0];
    }
    public static string ReadText(uint process,IntPtr window,int length) {
        if(length<0||length>1048576)throw new InvalidOperationException("Unexpected isolated document length");
        return Encoding.UTF8.GetString(Remote(process,window,2182,(ulong)(length+1),new byte[length+1],true)).TrimEnd('\0');
    }
    public static void AppendText(uint process,IntPtr window,string text) {
        byte[] bytes=Encoding.UTF8.GetBytes(text+"\0");
        Remote(process,window,2282,(ulong)(bytes.Length-1),bytes,false);
    }
    public static uint FindCommand(IntPtr menu,string prefix) {
        int count=GetMenuItemCount(menu);
        for(int i=0;i<count;i++) {
            var text=new StringBuilder(512);GetMenuStringW(menu,(uint)i,text,text.Capacity,0x400);
            IntPtr child=GetSubMenu(menu,i);
            if(child!=IntPtr.Zero){uint nested=FindCommand(child,prefix);if(nested!=0)return nested;}
            else if(text.ToString().Replace("&","").StartsWith(prefix,StringComparison.Ordinal))return GetMenuItemID(menu,i);
        }
        return 0;
    }
}
'@
}

$portable = Join-Path $runRoot 'portable'
$settingsRoot = Join-Path $runRoot 'profile'
# Portable plugin configuration stays beside the executable even with -settingsDir.
$config = Join-Path $portable 'plugins\Config'
$plugin = Join-Path $portable 'plugins\NppMarkdownNext'
New-Item -ItemType Directory -Path $portable,$settingsRoot,$config,$plugin -Force | Out-Null
$portableExe = Join-Path $portable 'notepad++.exe'
Copy-Item -LiteralPath $NppExe -Destination $portableExe
foreach ($dll in Get-ChildItem -LiteralPath (Split-Path -Parent $NppExe) -File -Filter '*.dll') {
    Copy-Item -LiteralPath $dll.FullName -Destination (Join-Path $portable $dll.Name)
}
foreach ($name in @('langs','stylers')) {
    $model = Join-Path (Split-Path -Parent $NppExe) ($name + '.model.xml')
    if (!(Test-Path -LiteralPath $model)) { throw "The official $name.model.xml bootstrap file is required." }
    Copy-Item -LiteralPath $model -Destination (Join-Path $portable ($name + '.model.xml'))
    Copy-Item -LiteralPath $model -Destination (Join-Path $settingsRoot ($name + '.xml'))
    [xml]$modelXml = [IO.File]::ReadAllText((Join-Path $settingsRoot ($name + '.xml')))
    if (!$modelXml.NotepadPlus) { throw "Invalid official $name model XML." }
}
[IO.File]::WriteAllBytes((Join-Path $portable 'doLocalConf.xml'), [byte[]]@())
# Fresh factory settings only; never copy the owner's config, history, shortcuts, or session.
$freshConfig = @'
<?xml version="1.0" encoding="UTF-8" ?>
<NotepadPlus>
  <GUIConfigs>
    <GUIConfig name="AppPosition" x="100" y="100" width="1400" height="900" isMaximized="no" />
    <GUIConfig name="RememberLastSession">no</GUIConfig>
    <GUIConfig name="CheckHistoryFiles">no</GUIConfig>
    <GUIConfig name="noUpdate">yes</GUIConfig>
  </GUIConfigs>
</NotepadPlus>
'@
[IO.File]::WriteAllText((Join-Path $portable 'config.model.xml'),$freshConfig,[Text.UTF8Encoding]::new($false))
[IO.File]::WriteAllText((Join-Path $settingsRoot 'config.xml'),$freshConfig,[Text.UTF8Encoding]::new($false))
foreach ($name in @('NppMarkdownNext.dll','WebView2Loader.dll')) {
    $source = Join-Path $binary $name
    if (!(Test-Path -LiteralPath $source)) { throw "Candidate binary missing: $source" }
    Copy-Item -LiteralPath $source -Destination (Join-Path $plugin $name)
    if ((Get-FileHash -LiteralPath $source).Hash -ne (Get-FileHash -LiteralPath (Join-Path $plugin $name)).Hash) {
        throw 'Candidate binary transfer hash mismatch.'
    }
}
$ini = Join-Path $config 'NppMarkdownNext.ini'
$udf = Join-Path $runRoot 'webview-cache'
[IO.File]::WriteAllText($ini, @'
[Settings]
IsPanelVisible=0
SyncWithCaret=0
ShowOutline=0
IsSmartBiDiEnabled=1
DarkModeOverride=-1
ZoomLevelPercent=115
SupportedExtensions=.md,.markdown,.mdown,.mkd,.rst
Theme=12
BeardedVariant=gold-d-raynh
AutoShowPanel=1
ContentFontSizePx=15
ContentLineHeightPercent=170
ContentMaxWidthPx=860
'@)
$marker = 'NPP_NATIVE_' + [Guid]::NewGuid().ToString('N')
$initial = Join-Path $runRoot 'initial.md'
$plain = Join-Path $runRoot 'before-save.txt'
$converted = Join-Path $runRoot 'after-save.md'
$inlineCode = [string][char]96 + 'inline' + [char]96
$nl = [Environment]::NewLine
$longBody = (@(1..140 | ForEach-Object { "Paragraph $_ for scrolling and source synchronization.$nl" }) -join $nl)
$document = [string]::Join($nl,@("# $marker",'',"**Bold** and *italic* with $inlineCode and [a link](https://example.invalid/smoke).",'','> A quote.','','- [x] literal [ ]','','## Reading section','',$longBody))
[IO.File]::WriteAllText($initial,$document,[Text.UTF8Encoding]::new($false))
[IO.File]::WriteAllText($plain,$document.Replace($marker,$marker + '_SAVEAS'),[Text.UTF8Encoding]::new($false))
$listener = [Net.Sockets.TcpListener]::new([Net.IPAddress]::Loopback,0)
$listener.Start()
$port = $listener.LocalEndpoint.Port
$listener.Stop()
$checks = [Collections.Generic.List[object]]::new()
$report = [ordered]@{passed=$false;runRoot=$runRoot;portableExe=$portableExe;profile=$settingsRoot;pluginConfig=$config;binaryDirectory=$binary;udf=$udf;port=$port;pluginSha256=(Get-FileHash -LiteralPath (Join-Path $plugin 'NppMarkdownNext.dll')).Hash;checks=$checks;states=[ordered]@{};notChecked=@('User clipboard contents','Save HTML dialog automation')}
$reportPath = Join-Path $runRoot 'result.json'
$app = $null
$failure = $null
$nppPid = 0
$main = [IntPtr]::Zero

function Assert-Check([bool]$Condition,[string]$Name,[string]$Detail='') {
    if (!$Condition) { throw "$Name failed: $Detail" }
    $checks.Add([pscustomobject]@{name=$Name;passed=$true;detail=$Detail})
    Write-Host "PASS $Name"
}
function Assert-OwnedNpp {
    $record = Get-CimInstance Win32_Process -Filter "ProcessId=$nppPid"
    if (!$record -or ![string]::Equals($record.ExecutablePath,$portableExe,[StringComparison]::OrdinalIgnoreCase)) {
        throw 'The isolated Notepad++ process identity could not be verified.'
    }
    return $record
}
function Assert-OwnedBrowser([int]$ProcessId,[switch]$AllowExited) {
    $record = Get-CimInstance Win32_Process -Filter "ProcessId=$ProcessId"
    if (!$record -and $AllowExited) { return }
    if (!$record -or $record.Name -ne 'msedgewebview2.exe' -or !$record.CommandLine -or
        $record.CommandLine.IndexOf($udf,[StringComparison]::OrdinalIgnoreCase) -lt 0) {
        throw "Browser process $ProcessId does not prove ownership of the isolated UDF."
    }
    return $record
}
function Stop-OwnedBrowser([int]$ProcessId) {
    $ownedProcess = Get-Process -Id $ProcessId -ErrorAction SilentlyContinue
    if (!$ownedProcess) { return }
    try {
        # Pin the process object before checking ownership so its PID cannot be reused.
        $null = $ownedProcess.Handle
        if ($ownedProcess.HasExited -or !(Assert-OwnedBrowser $ProcessId -AllowExited)) { return }
        $ownedProcess.Kill()
        if (!$ownedProcess.WaitForExit(5000)) { throw 'The owned browser did not exit.' }
    } catch {
        if (!$ownedProcess.HasExited) { throw }
    } finally {
        $ownedProcess.Dispose()
    }
}
function Get-OwnedPortProcess {
    Assert-OwnedNpp | Out-Null
    $connections = @(Get-NetTCPConnection -LocalPort $port -State Listen -ErrorAction SilentlyContinue)
    if (!$connections.Count) { throw 'The isolated CDP listener is not ready.' }
    $owners = @($connections | Select-Object -ExpandProperty OwningProcess -Unique)
    if ($owners.Count -ne 1 -or @($connections | Where-Object { $_.LocalAddress -notin @('127.0.0.1','::1') }).Count) {
        throw 'The CDP endpoint is not a unique loopback listener.'
    }
    Assert-OwnedBrowser $owners[0] | Out-Null
    return [int]$owners[0]
}
function Invoke-Probe([string]$Command='state',[string]$Expression='',[string]$Screenshot='') {
    Get-OwnedPortProcess | Out-Null
    $info = [Diagnostics.ProcessStartInfo]::new()
    $info.FileName = $node
    $info.UseShellExecute = $false
    $info.CreateNoWindow = $true
    $info.WindowStyle = [Diagnostics.ProcessWindowStyle]::Hidden
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    foreach ($argument in @($probe,'--port',[string]$port,'--command',$Command)) { $info.ArgumentList.Add($argument) }
    if ($Expression) {
        $info.ArgumentList.Add('--expression-base64')
        $info.ArgumentList.Add([Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($Expression)))
    }
    if ($Screenshot) { $info.ArgumentList.Add('--screenshot'); $info.ArgumentList.Add($Screenshot) }
    $process = [Diagnostics.Process]::Start($info)
    $output = $process.StandardOutput.ReadToEndAsync()
    $errors = $process.StandardError.ReadToEndAsync()
    if (!$process.WaitForExit(15000)) { $process.Kill(); throw 'The owned CDP helper timed out.' }
    $text = $output.GetAwaiter().GetResult()
    $errorText = $errors.GetAwaiter().GetResult()
    if ($process.ExitCode -ne 0) { throw "CDP probe failed: $errorText" }
    return $text | ConvertFrom-Json
}
function Wait-State([scriptblock]$Condition,[string]$Name,[int]$Seconds=35) {
    $watch = [Diagnostics.Stopwatch]::StartNew()
    $last = ''
    while ($watch.Elapsed.TotalSeconds -lt $Seconds) {
        try {
            $state = Invoke-Probe
            if (& $Condition $state) { return $state }
        } catch { $last = $_.Exception.Message }
        Start-Sleep -Milliseconds 150
    }
    throw "$Name timed out. $last"
}
function Get-Panel {
    $panels = @([NppSmokeApi]::Children($main,'NppMarkdownNextWindowClass'))
    if ($panels.Count -ne 1) { throw 'Expected exactly one owned Markdown panel.' }
    return $panels[0]
}
function Invoke-Menu([int]$Command) {
    Assert-OwnedNpp | Out-Null
    [NppSmokeApi]::Scalar($nppPid,$main,2072,0,$Command) | Out-Null
}
function Assert-Restored($State,[string]$Name) {
    Assert-Check ($State.state -eq 'ready' -and $State.text.Contains($marker)) $Name 'The current synthetic document was rendered.'
    Assert-Check ($State.themeId -eq 'bearded-v-gold-d-raynh' -and $State.zoomPercent -eq 115 -and
        !$State.sync -and $State.bidi -and $State.autoShow -and !$State.outline) ($Name + 'Settings') 'Gold, 115%, sync off, BiDi/auto-show on, outline off.'
}

try {
    $start = [Diagnostics.ProcessStartInfo]::new()
    $start.FileName = $portableExe
    $start.WorkingDirectory = $portable
    $start.UseShellExecute = $false
    $start.WindowStyle = if ($Show) { [Diagnostics.ProcessWindowStyle]::Normal } else { [Diagnostics.ProcessWindowStyle]::Hidden }
    foreach ($argument in @('-multiInst','-nosession',('-settingsDir=' + $settingsRoot),$initial)) { $start.ArgumentList.Add($argument) }
    $start.EnvironmentVariables['WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS'] = "--remote-debugging-port=$port --remote-debugging-address=127.0.0.1"
    $start.EnvironmentVariables['WEBVIEW2_USER_DATA_FOLDER'] = $udf
    $app = [Diagnostics.Process]::Start($start)
    $nppPid = $app.Id
    $report.processId = $nppPid
    Assert-OwnedNpp | Out-Null
    $watch = [Diagnostics.Stopwatch]::StartNew()
    while ($main -eq [IntPtr]::Zero -and $watch.Elapsed.TotalSeconds -lt 20) {
        $main = [NppSmokeApi]::FindMain($nppPid)
        if ($main -eq [IntPtr]::Zero) { Start-Sleep -Milliseconds 100 }
    }
    Assert-Check ($main -ne [IntPtr]::Zero) 'OwnedMainWindow'
    $state = Wait-State {param($s) $s.state -eq 'ready' -and $s.text.Contains($marker)} 'InitialRender'
    $actualConfig = [NppSmokeApi]::ReadPath($nppPid,$main,2070)
    Assert-Check ([string]::Equals($actualConfig.TrimEnd('\'),$config,[StringComparison]::OrdinalIgnoreCase)) 'IsolatedConfigurationRoot' $actualConfig
    Assert-Restored $state 'InitialRender'
    $state = Invoke-Probe 'inspect' '' (Join-Path $runRoot 'initial-gold.png')
    Assert-Check ($state.settings.Count -eq 11 -and @($state.settings | Where-Object { $_ -eq 'Typography' }).Count -eq 1) 'FullSettingsMenu' 'All eleven root rows including Typography are present.'
    Assert-Check ($state.colors.background -eq 'rgb(15, 22, 40)' -and $state.colors.heading -eq 'rgb(255, 208, 0)' -and
        $state.colors.strong -eq 'rgb(237, 242, 247)' -and $state.colors.emphasis -eq 'rgb(255, 130, 63)' -and
        $state.colors.code -eq 'rgb(161, 103, 255)' -and $state.colors.quote -eq 'rgb(228, 172, 115)' -and
        $state.colors.link -eq 'rgb(62, 178, 255)') 'NativeGoldMarkdownRoles'
    $report.states.initial = $state

    Assert-Check ([NppSmokeApi]::PathCommand($nppPid,$main,2101,0,$plain) -ne 0) 'OpenPlainTextSdk'
    $watch.Restart()
    while ([NppSmokeApi]::OwnVisible((Get-Panel)) -and $watch.Elapsed.TotalSeconds -lt 10) { Start-Sleep -Milliseconds 100 }
    Assert-Check (![NppSmokeApi]::OwnVisible((Get-Panel))) 'PlainTextHidesPreview'
    Assert-Check ([NppSmokeApi]::PathCommand($nppPid,$main,2102,0,$converted) -ne 0) 'SaveAsMarkdownSdk'
    Assert-Check ((Test-Path -LiteralPath $converted) -and (Get-FileHash -LiteralPath $converted).Hash -eq (Get-FileHash -LiteralPath $plain).Hash) 'SaveAsBytesVerified'
    $state = Wait-State {param($s) $s.state -eq 'ready' -and $s.text.Contains($marker + '_SAVEAS') -and $s.title -eq 'after-save.md'} 'SaveAsRefresh'
    Assert-Check ([NppSmokeApi]::OwnVisible((Get-Panel))) 'SaveAsShowsPreview'
    $report.states.saveAs = $state

    $editors = @([NppSmokeApi]::Children($main,'Scintilla'))
    $matching = @($editors | Where-Object {
        $length = [NppSmokeApi]::Scalar($nppPid,$_ ,2006,0,0)
        $length -gt 0 -and $length -lt 1048576 -and [NppSmokeApi]::ReadText($nppPid,$_,$length).Contains($marker + '_SAVEAS')
    })
    Assert-Check ($matching.Count -eq 1) 'OwnedEditorBySyntheticContent'
    $savedMarker = $marker + '_PERSISTED'
    [NppSmokeApi]::AppendText($nppPid,$matching[0],($nl + $savedMarker + $nl))
    $state = Wait-State {param($s) $s.text.Contains($savedMarker)} 'EditorChangeRenders'
    $beforeTask = $state.sequence
    Invoke-Probe 'evaluate' 'document.querySelector("#content input.task-checkbox").click()' | Out-Null
    $state = Wait-State {param($s) $s.sequence -gt $beforeTask -and $s.tasks.Count -eq 1 -and !$s.tasks[0].checked -and !$s.tasks[0].disabled} 'TaskToggleRenders'
    $editorLength = [NppSmokeApi]::Scalar($nppPid,$matching[0],2006,0,0)
    $edited = [NppSmokeApi]::ReadText($nppPid,$matching[0],$editorLength)
    Assert-Check ($edited.Contains('- [ ] literal [ ]') -and !$edited.Contains('- [x] literal [x]')) 'TaskToggleChangesOnlyActualMarker'
    [NppSmokeApi]::Scalar($nppPid,$matching[0],2171,1,0) | Out-Null
    try {
        $beforeTask = $state.sequence
        Invoke-Probe 'evaluate' 'document.querySelector("#content input.task-checkbox").click()' | Out-Null
        $state = Wait-State {param($s) $s.sequence -gt $beforeTask -and !$s.tasks[0].checked -and !$s.tasks[0].disabled} 'ReadonlyTaskRejected'
        $editorLength = [NppSmokeApi]::Scalar($nppPid,$matching[0],2006,0,0)
        Assert-Check ([NppSmokeApi]::ReadText($nppPid,$matching[0],$editorLength) -ceq $edited) 'ReadonlyTaskPreservesBytes'
    } finally { [NppSmokeApi]::Scalar($nppPid,$matching[0],2171,0,0) | Out-Null }
    Invoke-Probe 'evaluate' '(()=>{const root=window.getReaderScrollRoot();root.style.scrollBehavior="auto";root.scrollTop=500;return root.scrollTop;})()' | Out-Null
    $beforeSave = Invoke-Probe
    Assert-Check ($beforeSave.scrollY -gt 100) 'ReadingPositionWasMoved'
    Assert-Check ([NppSmokeApi]::Scalar($nppPid,$main,2062,0,0) -ne 0) 'OrdinarySaveSdk'
    Assert-Check ([IO.File]::ReadAllText($converted).Contains($savedMarker)) 'OrdinarySaveBytesVerified'
    Start-Sleep -Milliseconds 250
    $afterSave = Invoke-Probe
    Assert-Check ($afterSave.sequence -eq $beforeSave.sequence -and [Math]::Abs($afterSave.scrollY - $beforeSave.scrollY) -lt 3) 'OrdinarySaveKeepsReadingPosition'
    $report.states.ordinarySave = $afterSave

    Invoke-Menu 41004
    $watch.Restart()
    while ([NppSmokeApi]::OwnVisible((Get-Panel)) -and $watch.Elapsed.TotalSeconds -lt 10) { Start-Sleep -Milliseconds 100 }
    Assert-Check (![NppSmokeApi]::OwnVisible((Get-Panel))) 'LastCloseHidesPreview'
    $pluginMenu = [IntPtr][NppSmokeApi]::Scalar($nppPid,$main,2049,0,0)
    $toggle = [NppSmokeApi]::FindCommand($pluginMenu,'Toggle Markdown Reader')
    Assert-Check ($toggle -gt 0 -and $toggle -ne [uint32]::MaxValue) 'OwnedPreviewToggleFound'
    [NppSmokeApi]::Scalar($nppPid,$main,273,$toggle,0) | Out-Null
    $state = Wait-State {param($s) $s.state -eq 'empty' -and !$s.text.Contains($marker)} 'ManualEmptyRefresh'
    Assert-Check ([NppSmokeApi]::OwnVisible((Get-Panel))) 'ManualEmptyPanelVisible'
    $report.states.empty = $state

    Assert-Check ([NppSmokeApi]::PathCommand($nppPid,$main,2101,0,$initial) -ne 0) 'ReopenBeforeCrash'
    $state = Wait-State {param($s) $s.state -eq 'ready' -and $s.text.Contains($marker)} 'BeforeRendererCrash'
    Invoke-Probe 'evaluate' 'selectThemeFromMenu("light")' | Out-Null
    $state = Wait-State {
        param($s)
        $s.themeId -eq 'light' -and $s.colors.background -ne 'rgb(15, 22, 40)' -and
        [IO.File]::ReadAllText($ini) -match '(?m)^Theme=1\r?$'
    } 'NativeLightPalette'
    Assert-Check ($state.colors.background -and $state.colors.background -ne 'rgb(15, 22, 40)') 'NativeLightPaletteChangesAndPersists'
    $report.states.light = Invoke-Probe 'inspect' '' (Join-Path $runRoot 'light.png')
    Invoke-Probe 'evaluate' 'selectThemeFromMenu("bearded-v-gold-d-raynh")' | Out-Null
    $state = Wait-State {
        param($s)
        $s.themeId -eq 'bearded-v-gold-d-raynh' -and $s.colors.background -eq 'rgb(15, 22, 40)' -and
        [IO.File]::ReadAllText($ini) -match '(?m)^Theme=12\r?$'
    } 'NativeGoldPaletteRestored'
    Assert-Restored $state 'GoldRestoredBeforeCrashes'
    $oldTarget = $state.targetId
    Invoke-Probe 'crash-renderer' | Out-Null
    $state = Wait-State {param($s) $s.targetId -ne $oldTarget -and $s.state -eq 'ready' -and $s.text.Contains($marker)} 'RendererRecovery'
    Assert-Restored $state 'RendererRecovery'
    $report.states.rendererRecovery = $state

    Invoke-Probe 'crash-renderer' | Out-Null
    $watch.Restart()
    $retry = [IntPtr]::Zero
    while ($watch.Elapsed.TotalSeconds -lt 15) {
        $retry = [NppSmokeApi]::GetDlgItem((Get-Panel),5001)
        if ($retry -ne [IntPtr]::Zero -and [NppSmokeApi]::OwnVisible($retry) -and [NppSmokeApi]::IsWindowEnabled($retry)) { break }
        Start-Sleep -Milliseconds 100
    }
    Assert-Check ($retry -ne [IntPtr]::Zero -and [NppSmokeApi]::OwnVisible($retry) -and [NppSmokeApi]::IsWindowEnabled($retry)) 'AutomaticRetryIsBounded'
    [NppSmokeApi]::Scalar($nppPid,(Get-Panel),273,5001,$retry.ToInt64()) | Out-Null
    $state = Wait-State {param($s) $s.state -eq 'ready' -and $s.text.Contains($marker)} 'NativeRetry'
    Assert-Restored $state 'NativeRetry'
    $report.states.nativeRetry = $state

    Write-Host 'Waiting 31 seconds for the documented automatic retry window.'
    $watch.Restart()
    while ($watch.Elapsed.TotalSeconds -lt 31) { Start-Sleep -Milliseconds 200 }
    $oldTarget = $state.targetId
    $browser = Get-OwnedPortProcess
    $crash = Invoke-Probe 'crash-browser'
    if ($crash.unsupported) {
        Stop-OwnedBrowser $browser
        $report.browserFaultMode = 'verified-owned-browser-process'
    } else { $report.browserFaultMode = 'CDP Browser.crash' }
    $state = Wait-State {param($s) $s.targetId -ne $oldTarget -and $s.state -eq 'ready' -and $s.text.Contains($marker)} 'BrowserRecovery'
    Assert-Restored $state 'BrowserRecovery'
    $report.states.final = Invoke-Probe 'inspect' '' (Join-Path $runRoot 'final-gold.png')
    $report.passed = $true
} catch {
    $failure = $_
    $report.error = $_.Exception.Message
} finally {
    if ($app -and !$KeepOpen) {
        try {
            $record = Get-CimInstance Win32_Process -Filter "ProcessId=$nppPid" -ErrorAction SilentlyContinue
            if ($record) {
                Assert-OwnedNpp | Out-Null
                if ($main -ne [IntPtr]::Zero) { [NppSmokeApi]::Scalar($nppPid,$main,16,0,0) | Out-Null }
                if (!$app.WaitForExit(5000)) {
                    Assert-OwnedNpp | Out-Null
                    Stop-Process -Id $nppPid -Force
                    if (!$app.WaitForExit(5000)) { throw 'The owned Notepad++ process did not exit.' }
                }
            }
            foreach ($browser in @(Get-CimInstance Win32_Process -Filter "Name='msedgewebview2.exe'" | Where-Object {
                $_.CommandLine -and $_.CommandLine.IndexOf($udf,[StringComparison]::OrdinalIgnoreCase) -ge 0
            })) {
                Stop-OwnedBrowser $browser.ProcessId
            }
            $report.processClosed = $true
        } catch {
            $report.cleanupError = $_.Exception.Message
            $report.passed = $false
            if (!$failure) { $failure = $_ }
        }
    } elseif ($app) { $report.keptOpen = $true }
    [IO.File]::WriteAllText($reportPath,($report | ConvertTo-Json -Depth 12),[Text.UTF8Encoding]::new($false))
}
[pscustomobject]@{passed=$report.passed;report=$reportPath;checks=$checks.Count;processId=$nppPid;keptOpen=[bool]$KeepOpen} | ConvertTo-Json -Compress
if ($failure) { throw $failure }
