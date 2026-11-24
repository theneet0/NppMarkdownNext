#include "../include/NppMarkdownNext.h"
#include "../include/BiDiEngine.h"
#include "../include/HtmlExporter.h"
#include "../include/Resource.h"
#include "../include/NppLog.h"
#include "../include/ThemeCatalog.h"
#include <commctrl.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <utility>
#include <cstdint>
#include <iomanip>

extern "C" int GetPluginCmdId(int index);

namespace {
const wchar_t* s_panelClassName = L"NppMarkdownNextWindowClass";

bool IsRequestIdValid(const std::wstring& id) {
    return id.size() <= 64 && std::all_of(id.begin(), id.end(), [](wchar_t ch) {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') ||
               (ch >= L'0' && ch <= L'9') || ch == L':' || ch == L'-';
    });
}

bool IsTaskMarker(const std::string& line, size_t column, char expectedState) {
    size_t offset = 0;
    bool hasListMarker = false;
    while (offset < line.size()) {
        while (offset < line.size() && (line[offset] == ' ' || line[offset] == '\t')) ++offset;
        if (offset == line.size()) break;
        if (line[offset] == '>') { ++offset; continue; }
        size_t markerEnd = offset;
        if (line[offset] == '-' || line[offset] == '+' || line[offset] == '*') {
            ++markerEnd;
        } else {
            while (markerEnd < line.size() && line[markerEnd] >= '0' && line[markerEnd] <= '9') ++markerEnd;
            if (markerEnd == offset || markerEnd - offset > 9 || markerEnd == line.size() ||
                (line[markerEnd] != '.' && line[markerEnd] != ')')) break;
            ++markerEnd;
        }
        if (markerEnd == line.size() || (line[markerEnd] != ' ' && line[markerEnd] != '\t')) break;
        hasListMarker = true;
        offset = markerEnd;
    }
    return hasListMarker && offset == column && offset + 3 <= line.size() && line[offset] == '[' &&
        line[offset + 1] == expectedState && line[offset + 2] == ']' &&
        (offset + 3 == line.size() || line[offset + 3] == ' ' || line[offset + 3] == '\t');
}

int ReadingMessageValue(const std::wstring& message, const wchar_t* prefix, std::initializer_list<int> choices) {
    for (int value : choices) if (message == std::wstring(prefix) + std::to_wstring(value)) return value;
    return 0;
}

struct ExportScope {
    std::function<void()> finish;
    ~ExportScope() { finish(); }
};
}

NppMarkdownNext& NppMarkdownNext::Instance() {
    static NppMarkdownNext s_inst;
    return s_inst;
}

void NppMarkdownNext::Init(HINSTANCE hInst, NppData nppData) {
    m_hInst = hInst;
    m_nppData = nppData;
    m_shuttingDown = false;

    wchar_t configDir[MAX_PATH] = { 0 };
    if (m_nppData._nppHandle) {
        SendMessage(m_nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configDir);
    }
    if (configDir[0] == L'\0') {
        GetTempPathW(MAX_PATH, configDir);
    }
    m_configPath = std::wstring(configDir) + L"\\NppMarkdownNext.ini";
    m_config.Load(m_configPath);
    m_config.isPanelVisible = false;
    m_isPanelVisible = false;
}

void NppMarkdownNext::OnNppReady() {
    m_isNppReady = true;

    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));

    SyncThemeMenuItems();
    OnBufferActivated();
}

void NppMarkdownNext::SyncThemeMenuItems() {
    int cmdFollow = GetPluginCmdId(CMD_THEME_FOLLOW_NPP);
    if (cmdFollow > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdFollow, (LPARAM)(m_config.theme == ReaderTheme::FollowNpp ? TRUE : FALSE));
    int cmdLight = GetPluginCmdId(CMD_THEME_LIGHT);
    if (cmdLight > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdLight, (LPARAM)(m_config.theme == ReaderTheme::Light ? TRUE : FALSE));
    int cmdDark = GetPluginCmdId(CMD_THEME_DARK);
    if (cmdDark > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdDark, (LPARAM)(m_config.theme == ReaderTheme::Dark ? TRUE : FALSE));
}

void NppMarkdownNext::EnsurePanelRegistered() {
    if (m_isPanelRegistered) return;
    CreatePanelWindow();
}

void NppMarkdownNext::Cleanup() {
    if (m_shuttingDown) return;
    m_shuttingDown = true;
    m_isNppReady = false;
    m_recoveryQueued = false;
    m_forceRenderPending = false;
    m_exportQueued = false;
    m_queuedExportOperation.clear();
    m_queuedExportRequestId.clear();
    m_pendingExportWarnings.clear();
    ++m_viewerGeneration;
    if (m_hPanel) KillTimer(m_hPanel, IDT_RENDER_DEBOUNCE);
    SaveConfig(false);
    m_webViewViewer.Close();
    if (m_hPanel && IsWindow(m_hPanel)) {
        DestroyWindow(m_hPanel);
        m_hPanel = nullptr;
    }
    m_hViewerStatus = nullptr;
    m_hRetryViewer = nullptr;
    m_isPanelRegistered = false;
    m_isPanelVisible = false;
}

bool NppMarkdownNext::CreatePanelWindow() {
    if (m_isPanelRegistered && m_hPanel && IsWindow(m_hPanel)) {
        return true;
    }

    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpfnWndProc = PanelWndProc;
    wc.hInstance = m_hInst;
    wc.lpszClassName = s_panelClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassEx(&wc);

    m_hPanel = CreateWindowEx(
        WS_EX_CONTROLPARENT,
        s_panelClassName,
        L"Markdown Reader (Kelk)",
        WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        0, 0, 450, 700,
        m_nppData._nppHandle,
        nullptr,
        m_hInst,
        this
    );

    if (!m_hPanel) return false;

    m_tbData.hClient = m_hPanel;
    m_tbData.pszName = L"Markdown Reader (Kelk)";
    m_tbData.dlgID = CMD_TOGGLE_PANEL;
    m_tbData.uMask = DWS_DF_CONT_RIGHT | DWS_USEOWNDARKMODE;
    m_tbData.hIconTab = nullptr;
    m_tbData.pszAddInfo = nullptr;
    m_tbData.rcFloat = { 0, 0, 0, 0 };
    m_tbData.iPrevCont = -1;
    m_tbData.pszModuleName = L"NppMarkdownNext.dll";

    SendMessage(m_nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&m_tbData);
    m_isPanelRegistered = true;

    m_hViewerStatus = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_CENTER, 16, 24, 300, 64,
        m_hPanel, nullptr, m_hInst, nullptr);
    m_hRetryViewer = CreateWindowExW(0, L"BUTTON", L"Retry preview", WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
        16, 96, 120, 30, m_hPanel, reinterpret_cast<HMENU>(IDC_RETRY_VIEWER), m_hInst, nullptr);
    SendMessage(m_hViewerStatus, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), TRUE);
    SendMessage(m_hRetryViewer, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    m_webViewViewer.SetPageReadyCallback([this]() {
        if (m_shuttingDown) return;
        NppLog("WebView2 page ready, executing initial render");
        m_viewerStatusVisible = false;
        ShowWindow(m_hViewerStatus, SW_HIDE);
        ShowWindow(m_hRetryViewer, SW_HIDE);
        m_webViewViewer.SetVisible(m_isPanelVisible);
        m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
        SyncUiState();
        if (m_isPanelVisible) {
            RequestRenderDebounced(true);
        }
        if (m_openSettingsPending) {
            m_openSettingsPending = false;
            m_webViewViewer.ExecuteScript(L"window.openSettingsPopover();");
        }
    });

    m_webViewViewer.SetCheckboxCallback([this](const CheckboxRequest& request) { HandleCheckboxToggle(request); });

    m_webViewViewer.SetJumpToLineCallback([this](int lineIdx) {
        HWND hSci = GetCurrentScintilla();
        if (!hSci || lineIdx < 0) return;
        SendMessage(hSci, SCI_ENSUREVISIBLE, lineIdx, 0);
        SendMessage(hSci, SCI_GOTOLINE, lineIdx, 0);
        SetFocus(hSci);
    });

    m_webViewViewer.SetSyncCallback([this]() {
        ToggleSyncWithCaret();
    });

    m_webViewViewer.SetThemeCallback([this]() {
        SetTheme(IsDarkThemeEffective() ? ReaderTheme::Light : ReaderTheme::Dark);
    });
    m_webViewViewer.SetSelectThemeCallback([this](const std::wstring& id) {
        if (!m_config.SetThemeId(id)) return;
        SaveConfig();
        SyncThemeMenuItems();
        m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
        SyncUiState();
    });
    m_webViewViewer.SetSettingsCallback([this](const std::wstring& message) { OnSettingsMessage(message); });

    m_webViewViewer.SetBiDiCallback([this]() {
        ToggleBiDi();
    });

    m_webViewViewer.SetZoomChangeCallback([this](float factor) {
        m_config.zoomLevel = factor;
        if (m_config.zoomLevel < 0.3f) m_config.zoomLevel = 0.3f;
        if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
        SaveConfig();
        SyncUiState();
    });

    InitializeViewer();

    return true;
}

void NppMarkdownNext::LayoutViewerStatus() {
    if (!m_hPanel) return;
    RECT rect = {};
    GetClientRect(m_hPanel, &rect);
    int width = (std::max)(1L, rect.right - rect.left);
    int margin = (std::min)(16, width / 8);
    int buttonWidth = (std::min)(140, (std::max)(1, width - 2 * margin));
    SetWindowPos(m_hViewerStatus, HWND_TOP, margin, 24, (std::max)(1, width - 2 * margin), 72, SWP_NOACTIVATE);
    SetWindowPos(m_hRetryViewer, HWND_TOP, (width - buttonWidth) / 2, 108, buttonWidth, 30, SWP_NOACTIVATE);
}

void NppMarkdownNext::SetViewerStatus(const std::wstring& text, bool retry) {
    m_viewerStatusVisible = true;
    SetWindowTextW(m_hViewerStatus, text.c_str());
    EnableWindow(m_hRetryViewer, retry ? TRUE : FALSE);
    LayoutViewerStatus();
    ShowWindow(m_hViewerStatus, m_isPanelVisible ? SW_SHOW : SW_HIDE);
    ShowWindow(m_hRetryViewer, m_isPanelVisible && retry ? SW_SHOW : SW_HIDE);
}

std::wstring NppMarkdownNext::GetViewerDataDirectory() const {
    DWORD required = GetTempPathW(0, nullptr);
    if (!required) return {};
    std::wstring temp(required, L'\0');
    DWORD copied = GetTempPathW(required, temp.data());
    if (!copied || copied >= required) return {};
    temp.resize(copied);

    size_t separator = m_configPath.find_last_of(L"\\/");
    std::wstring configRoot = separator == std::wstring::npos ? L"." : m_configPath.substr(0, separator);
    DWORD pathSize = GetFullPathNameW(configRoot.c_str(), 0, nullptr, nullptr);
    if (pathSize) {
        std::wstring absolute(pathSize, L'\0');
        DWORD length = GetFullPathNameW(configRoot.c_str(), pathSize, absolute.data(), nullptr);
        if (length && length < pathSize) {
            absolute.resize(length);
            configRoot = std::move(absolute);
        }
    }
    while (configRoot.size() > 3 && (configRoot.back() == L'\\' || configRoot.back() == L'/')) configRoot.pop_back();
    std::uint64_t identity = 14695981039346656037ULL;
    for (wchar_t unit : configRoot) {
        if (unit == L'/') unit = L'\\';
        if (unit >= L'A' && unit <= L'Z') unit += L'a' - L'A';
        identity ^= static_cast<unsigned char>(unit & 0xff);
        identity *= 1099511628211ULL;
        identity ^= static_cast<unsigned char>((unit >> 8) & 0xff);
        identity *= 1099511628211ULL;
    }
    // Cache writes must remain available when the configuration root is read-only.
    std::wostringstream path;
    path << temp << L"NppMarkdownNext-WebView2-" << std::hex << std::setfill(L'0') << std::setw(16) << identity;
    return path.str();
}

bool NppMarkdownNext::InitializeViewer() {
    if (m_shuttingDown) return false;
    const UINT_PTR generation = ++m_viewerGeneration;
    m_recoveryQueued = false;
    m_settingsWarningReported = false;
    m_webViewViewer.SetInitFailedCallback([this, generation]() {
        if (m_shuttingDown || generation != m_viewerGeneration || m_recoveryQueued) return;
        m_recoveryQueued = true;
        if (!PostMessage(m_hPanel, WM_APP_VIEWER_RECOVERY, generation, 0)) m_recoveryQueued = false;
    });
#ifdef NPP_READER_TESTING
    if (m_testInitializeViewer) return m_testInitializeViewer();
#endif
    SetViewerStatus(L"Loading Markdown preview...", false);
    m_webViewViewer.SetVisible(false);
    const std::wstring userDataDir = GetViewerDataDirectory();
    if (userDataDir.empty()) {
        SetViewerStatus(L"The preview cache directory is unavailable. Check TEMP and retry.", true);
        return false;
    }
    bool started = m_webViewViewer.Initialize(m_hPanel, userDataDir);
    m_webViewViewer.SetZoom(m_config.zoomLevel);
    return started;
}

void NppMarkdownNext::HandleViewerRecovery(bool manual) {
    if (m_shuttingDown || !m_hPanel || !IsWindow(m_hPanel)) return;
    if (m_exportInProgress) { m_recoverAfterExport = true; return; }
    ULONGLONG now = GetTickCount64();
#ifdef NPP_READER_TESTING
    if (m_testClock) now = m_testClock();
#endif
    if (!manual && m_hasRecoveryAttempt && now - m_lastRecoveryTick < 30000) {
        SetViewerStatus(L"Preview could not be loaded. Check WebView2 Runtime and retry.", true);
        return;
    }
    m_hasRecoveryAttempt = true;
    m_lastRecoveryTick = now;
    m_webViewViewer.Close();
    InitializeViewer();
}

HWND NppMarkdownNext::GetCurrentScintilla() const {
    int which = -1;
    SendMessage(m_nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    return (which == 0) ? m_nppData._scintillaMainHandle : m_nppData._scintillaSecondHandle;
}

UINT_PTR NppMarkdownNext::GetCurrentBufferId() const {
    return static_cast<UINT_PTR>(SendMessage(m_nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0));
}

std::string NppMarkdownNext::GetScintillaText(HWND hSci) const {
    if (!hSci) return "";
    int length = static_cast<int>(SendMessage(hSci, SCI_GETLENGTH, 0, 0));
    if (length <= 0) return "";

    std::string buffer(length + 1, '\0');
    SendMessage(hSci, SCI_GETTEXT, length + 1, (LPARAM)&buffer[0]);
    buffer.resize(length);
    return buffer;
}

int NppMarkdownNext::GetScintillaCaretLine(HWND hSci) const {
    if (!hSci) return 0;
    int pos = static_cast<int>(SendMessage(hSci, SCI_GETCURRENTPOS, 0, 0));
    return static_cast<int>(SendMessage(hSci, SCI_LINEFROMPOSITION, pos, 0));
}

std::wstring NppMarkdownNext::GetCurrentBufferPath() const {
    wchar_t path[MAX_PATH] = { 0 };
    SendMessage(m_nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)path);
    return path;
}

bool NppMarkdownNext::IsCurrentBufferMarkdown() const {
    std::wstring path = GetCurrentBufferPath();
    if (path.empty() || path.find(L'\\') == std::wstring::npos) return true; // Unsaved tab like "new 1"

    size_t dotPos = path.find_last_of(L'.');
    if (dotPos == std::wstring::npos) return false;
    std::wstring ext = path.substr(dotPos);
    return m_config.IsExtensionSupported(ext);
}

void NppMarkdownNext::TogglePanel() {
    bool show = !m_isPanelRegistered || !m_isPanelVisible;
    m_userClosedPanel = !show;
    SetPanelVisible(show);
}

void NppMarkdownNext::SetPanelVisible(bool visible) {
    if (m_shuttingDown) return;
    if (!visible && !m_isPanelRegistered) return;
    EnsurePanelRegistered();
    m_isPanelVisible = visible;
    SendMessage(m_nppData._nppHandle, visible ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, (LPARAM)m_hPanel);

    int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
    if (cmdId > 0) {
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(visible ? TRUE : FALSE));
    }

    if (visible) {
        RECT rc;
        GetClientRect(m_hPanel, &rc);
        m_webViewViewer.Resize(0, 0, rc.right - rc.left, rc.bottom - rc.top);
        m_webViewViewer.SetVisible(m_webViewViewer.IsPageReady());
        RequestRenderDebounced(true);
    } else {
        KillTimer(m_hPanel, IDT_RENDER_DEBOUNCE);
        m_forceRenderPending = false;
        m_webViewViewer.SetVisible(false);
    }
}

void NppMarkdownNext::ApplyAutoShowPanel() {
    if (!m_config.autoShowPanel) return;
    if (GetCurrentBufferPath().find(L'\\') == std::wstring::npos) {
        HWND hSci = GetCurrentScintilla();
        if (hSci && SendMessage(hSci, SCI_GETLENGTH, 0, 0) == 0) SetPanelVisible(false);
        return;
    }
    bool isMarkdown = IsCurrentBufferMarkdown();
    if (isMarkdown && !m_isPanelVisible && !m_userClosedPanel) {
        SetPanelVisible(true);
    } else if (!isMarkdown && m_isPanelVisible) {
        SetPanelVisible(false);
        m_userClosedPanel = false;
    }
}

void NppMarkdownNext::ToggleSyncWithCaret() {
    m_config.syncWithCaret = !m_config.syncWithCaret;
    SaveConfig();
    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
    SyncUiState();
}

void NppMarkdownNext::ToggleBiDi() {
    m_config.isSmartBiDiEnabled = !m_config.isSmartBiDiEnabled;
    SaveConfig();
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));
    m_webViewViewer.SetBiDi(m_config.isSmartBiDiEnabled);
    SyncUiState();
}

bool NppMarkdownNext::IsDarkThemeEffective() const {
    if (m_config.theme != ReaderTheme::FollowNpp) {
        const auto* entry = FindTheme(m_config.ThemeId());
        return entry ? entry->isDark : false;
    }
    if (m_config.darkModeOverride >= 0) return m_config.darkModeOverride == 1;
    // Follow Notepad++
    return SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
}

void NppMarkdownNext::SetTheme(ReaderTheme theme) {
    if (!FindTheme(theme)) return;
    m_config.theme = theme;
    m_config.beardedVariant.clear();
    SaveConfig();
    SyncThemeMenuItems();
    m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
    SyncUiState();
}

void NppMarkdownNext::OnDarkModeChanged() {
    if (m_config.theme == ReaderTheme::FollowNpp) {
        m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
    }
}

void NppMarkdownNext::ZoomIn() {
    m_config.zoomLevel += 0.1f;
    if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
    SaveConfig();
    m_webViewViewer.SetZoom(m_config.zoomLevel);
    SyncUiState();
}

void NppMarkdownNext::ZoomOut() {
    m_config.zoomLevel -= 0.1f;
    if (m_config.zoomLevel < 0.3f) m_config.zoomLevel = 0.3f;
    SaveConfig();
    m_webViewViewer.SetZoom(m_config.zoomLevel);
    SyncUiState();
}

void NppMarkdownNext::ZoomReset() {
    m_config.zoomLevel = 1.0f;
    SaveConfig();
    m_webViewViewer.SetZoom(m_config.zoomLevel);
    SyncUiState();
}

void NppMarkdownNext::SyncUiState() {
    m_webViewViewer.SyncUiState(m_config);
    if (m_settingsSaveWarningActive && !m_settingsWarningReported && m_webViewViewer.IsPageReady()) {
        m_webViewViewer.ReportOperationResult(L"saveSettings", {OperationStatus::Failed, m_settingsSaveError,
            L"Settings apply to this session, but could not be saved. Check the configuration file permissions."});
        m_settingsWarningReported = true;
    }
}

bool NppMarkdownNext::SaveConfig(bool notifyUser) {
    bool saved = m_config.Save(m_configPath);
    DWORD error = saved ? ERROR_SUCCESS : GetLastError();
    if (!saved) {
        NppLog("Configuration save failed error=%u", (unsigned)error);
        bool firstFailure = !m_settingsSaveWarningActive;
        m_settingsSaveWarningActive = true;
        m_settingsSaveError = error;
        if (notifyUser && firstFailure && m_webViewViewer.IsPageReady()) {
            m_webViewViewer.ReportOperationResult(L"saveSettings", {OperationStatus::Failed, error,
                L"Settings apply to this session, but could not be saved. Check the configuration file permissions."});
            m_settingsWarningReported = true;
        }
    } else if (m_settingsSaveWarningActive) {
        m_settingsSaveWarningActive = false;
        m_settingsWarningReported = false;
        m_settingsSaveError = ERROR_SUCCESS;
        if (notifyUser) m_webViewViewer.ReportOperationResult(L"saveSettings", {OperationStatus::Succeeded, ERROR_SUCCESS, L"Settings saved."});
    }
    return saved;
}

void NppMarkdownNext::OpenSettings() {
    m_userClosedPanel = false;
    SetPanelVisible(true);
    if (m_webViewViewer.IsPageReady()) {
        SyncUiState();
        m_webViewViewer.ExecuteScript(L"window.openSettingsPopover();");
    } else {
        m_openSettingsPending = true;
    }
}

void NppMarkdownNext::OnSettingsMessage(const std::wstring& message) {
    if (m_shuttingDown) return;
    if (message == L"zoomIn") { ZoomIn(); return; }
    if (message == L"zoomOut") { ZoomOut(); return; }
    if (message == L"zoomReset") { ZoomReset(); return; }
    if (message == L"copyHtml" || message == L"saveAsHtml") { QueueExport(message, L""); return; }
    for (const auto* operation : {L"copyHtml", L"saveAsHtml"}) {
        const std::wstring prefix = std::wstring(operation) + L":";
        if (message.starts_with(prefix)) {
            std::wstring id = message.substr(prefix.size());
            if (!id.empty() && IsRequestIdValid(id)) QueueExport(operation, id);
            else ReportExportResult(operation, {OperationStatus::Failed, ERROR_INVALID_PARAMETER, L"Invalid export request identifier."}, id);
            return;
        }
    }
    if (message == L"toggleAutoShow") {
        m_config.autoShowPanel = !m_config.autoShowPanel;
        ApplyAutoShowPanel();
    } else if (message == L"tocStateChanged:true" || message == L"tocStateChanged:false") {
        m_config.showOutline = message == L"tocStateChanged:true";
    } else if (message == L"darkMode:-1" || message == L"darkMode:0" || message == L"darkMode:1") {
        if (m_config.theme != ReaderTheme::FollowNpp) { SyncUiState(); return; }
        m_config.darkModeOverride = message == L"darkMode:-1" ? -1 : (message == L"darkMode:1" ? 1 : 0);
        m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
    } else if (int value = ReadingMessageValue(message, L"readingFontSize:", {15, 17, 19})) {
        m_config.contentFontSizePx = value;
    } else if (int value = ReadingMessageValue(message, L"readingLineHeight:", {150, 170, 190})) {
        m_config.contentLineHeightPercent = value;
    } else if (int value = ReadingMessageValue(message, L"readingWidth:", {680, 860, 1100})) {
        m_config.contentMaxWidthPx = value;
    } else if (message.rfind(L"zoomChange:", 0) == 0) {
        try {
            size_t parsed = 0;
            const std::wstring value = message.substr(11);
            float zoom = std::stof(value, &parsed);
            if (parsed != value.size() || !std::isfinite(zoom)) return;
            m_config.zoomLevel = std::clamp(zoom, 0.3f, 3.0f);
            m_webViewViewer.SetZoom(m_config.zoomLevel);
        } catch (const std::exception&) { return; }
    } else {
        return;
    }
    SaveConfig();
    SyncUiState();
}

void NppMarkdownNext::QueueExport(const std::wstring& operation, const std::wstring& requestId) {
    if (m_shuttingDown) return;
    if (m_exportInProgress || m_exportQueued) {
        ReportExportResult(operation, {OperationStatus::Failed, ERROR_BUSY, L"Another HTML operation is already in progress."}, requestId);
        return;
    }
    m_exportQueued = true;
    m_queuedExportOperation = operation;
    m_queuedExportRequestId = requestId;
    if (!PostMessage(m_hPanel, WM_APP_EXPORT_REQUEST, 0, 0)) {
        DWORD error = GetLastError();
        m_exportQueued = false;
        m_queuedExportOperation.clear();
        m_queuedExportRequestId.clear();
        ReportExportResult(operation, {OperationStatus::Failed, error, L"The HTML operation could not be started."}, requestId);
    }
}

void NppMarkdownNext::FinishExport() {
    m_exportInProgress = false;
    if (m_recoverAfterExport && !m_shuttingDown) {
        m_recoverAfterExport = false;
        if (!m_recoveryQueued) {
            m_recoveryQueued = true;
            if (!PostMessage(m_hPanel, WM_APP_VIEWER_RECOVERY, m_viewerGeneration, 0)) m_recoveryQueued = false;
        }
    }
}

void NppMarkdownNext::ShowExportWarning(const std::wstring& message) {
#ifdef NPP_READER_TESTING
    if (m_testExportWarning) { m_testExportWarning(message); return; }
#endif
    MessageBoxW(m_nppData._nppHandle, message.c_str(), L"Markdown HTML export", MB_OK | MB_ICONWARNING);
}

void NppMarkdownNext::ReportExportResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId) {
    bool delivered = m_webViewViewer.ReportOperationResult(operation, result, requestId);
    if ((result.status != OperationStatus::Failed && result.status != OperationStatus::Partial) ||
        (!requestId.empty() && delivered && m_isPanelVisible && !m_viewerStatusVisible)) return;

    std::wstring message = operation == L"copyHtml" ? L"Copy HTML" : L"Save HTML";
    message += result.status == OperationStatus::Partial ? L" completed partially.\n\n" : L" failed.\n\n";
    message += result.detail;
    if (result.error) message += L"\n\nError code: " + std::to_wstring(result.error);
    if (!result.recoveryPath.empty()) message += L"\n\nRecovery file: " + result.recoveryPath;
    if (!m_hPanel || !IsWindow(m_hPanel)) {
        ShowExportWarning(message);
    } else {
        // WebView callbacks cannot open a modal dialog until their stack unwinds.
        m_pendingExportWarnings.push_back(std::move(message));
        if (!PostMessage(m_hPanel, WM_APP_EXPORT_RESULT, 0, 0)) {
            NppLog("Native export warning could not be scheduled error=%u", (unsigned)GetLastError());
        }
    }
}

void NppMarkdownNext::CopyRenderedHtml(const std::wstring& requestId) {
    if (m_shuttingDown) return;
    if (!IsRequestIdValid(requestId) || m_exportInProgress || m_exportQueued) {
        ReportExportResult(L"copyHtml", {OperationStatus::Failed, ERROR_BUSY, L"Another HTML operation is already in progress."}, requestId);
        return;
    }
    m_exportInProgress = true;
    ExportScope scope{[this]() { FinishExport(); }};
    HWND hSci = GetCurrentScintilla();
    if (!hSci) {
        ReportExportResult(L"copyHtml", {OperationStatus::Failed, ERROR_INVALID_HANDLE, L"There is no active document to copy."}, requestId);
        return;
    }
    std::string textUtf8 = GetScintillaText(hSci);
    std::wstring wideText = BiDiEngine::Utf8ToWide(textUtf8);
    MarkdownDocument doc = MarkdownParser::Parse(wideText);
    std::wstring docTitle = PathFindFileNameW(GetCurrentBufferPath().c_str());
    if (docTitle.empty()) docTitle = L"Markdown Preview";

    OperationResult result = HtmlExporter::CopyToClipboard(m_hPanel ? m_hPanel : m_nppData._nppHandle, doc, docTitle,
                                  m_config.theme == ReaderTheme::FollowNpp ?
                                      (IsDarkThemeEffective() ? ReaderTheme::Dark : ReaderTheme::Light) : m_config.theme,
                                  m_config.beardedVariant);
    ReportExportResult(L"copyHtml", result, requestId);
}

void NppMarkdownNext::SaveAsHtml(const std::wstring& requestId) {
    if (m_shuttingDown) return;
    if (!IsRequestIdValid(requestId) || m_exportInProgress || m_exportQueued) {
        ReportExportResult(L"saveAsHtml", {OperationStatus::Failed, ERROR_BUSY, L"Another HTML operation is already in progress."}, requestId);
        return;
    }
    m_exportInProgress = true;
    ExportScope scope{[this]() { FinishExport(); }};
    HWND hSci = GetCurrentScintilla();
    if (!hSci) {
        ReportExportResult(L"saveAsHtml", {OperationStatus::Failed, ERROR_INVALID_HANDLE, L"There is no active document to export."}, requestId);
        return;
    }
    std::string textUtf8 = GetScintillaText(hSci);
    std::wstring wideText = BiDiEngine::Utf8ToWide(textUtf8);
    MarkdownDocument doc = MarkdownParser::Parse(wideText);

    wchar_t fileName[MAX_PATH] = L"export.html";
    OPENFILENAMEW ofn = { sizeof(OPENFILENAMEW) };
    ofn.hwndOwner = m_hPanel ? m_hPanel : m_nppData._nppHandle;
    ofn.lpstrFilter = L"HTML Document (*.html;*.htm)\0*.html;*.htm\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = L"html";

    if (GetSaveFileNameW(&ofn)) {
        OperationResult result = HtmlExporter::SaveToFile(fileName, doc, L"Markdown Export",
                                 m_config.theme == ReaderTheme::FollowNpp ?
                                     (IsDarkThemeEffective() ? ReaderTheme::Dark : ReaderTheme::Light) : m_config.theme,
                                 m_config.beardedVariant);
        ReportExportResult(L"saveAsHtml", result, requestId);
    } else {
        DWORD error = CommDlgExtendedError();
        ReportExportResult(L"saveAsHtml", {
            error ? OperationStatus::Failed : OperationStatus::Cancelled, error,
            error ? L"The Save HTML dialog could not be opened." : L""}, requestId);
    }
}

void NppMarkdownNext::ShowAbout() {
    const wchar_t* aboutMsg =
        L"NppMarkdownNext (Kelk Reader Edition)\n"
        L"Version 1.6.0 (x64 / x86 / ARM64)\n\n"
        L"Ultra-lightweight native Markdown live reader panel for Notepad++ based on Kelk.\n\n"
        L"Features:\n"
        L"  • Pure Kelk Markdown Reader Core (GFM Tables, Task Lists, Code Blocks)\n"
        L"  • Kelk BiDiCore & BidiView: Flawless Right-to-Left (Persian, Arabic, Urdu, Hebrew)\n"
        L"  • System Font Rendering (Vazirmatn / Sahel / Segoe UI)\n"
        L"  • Native Chromium WebView2 with zero bloat (300 KB reader shell)\n"
        L"  • Light / Dark mode sync with Notepad++\n"
        L"  • Interactive Task Checkboxes & Caret Sync\n"
        L"  • Double-click to jump to editor line\n"
        L"  • 100% Offline with zero external dependencies\n\n"
        L"Based on Kelk (https://github.com/mohsen-1984/kelk)\n"
        L"Licensed under the MIT License";

    MessageBox(m_nppData._nppHandle, aboutMsg, L"About Markdown Reader", MB_OK | MB_ICONINFORMATION);
}

void NppMarkdownNext::RequestRenderDebounced(bool force) {
    if (m_shuttingDown || !m_isPanelVisible) return;
    if (!force && !IsCurrentBufferMarkdown()) return;
    m_forceRenderPending = m_forceRenderPending || force;
    if (!SetTimer(m_hPanel, IDT_RENDER_DEBOUNCE, m_forceRenderPending ? 1 : RENDER_DEBOUNCE_DELAY_MS, nullptr)) {
        NppLog("Render timer could not be scheduled error=%u", (unsigned)GetLastError());
    }
}

bool NppMarkdownNext::ExecuteRender(bool force) {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return false;

    const ULONGLONG sourceRevision = m_sourceRevision;
    std::string utf8Content = GetScintillaText(hSci);
    int caretLine = m_config.syncWithCaret ? GetScintillaCaretLine(hSci) : -1;

    if (!force && utf8Content == m_lastRawContentUtf8 && m_sourceRevision == m_lastRenderedSourceRevision) {
        if (m_config.syncWithCaret && caretLine >= 0) {
            m_webViewViewer.ScrollToLine(caretLine);
        }
        return true;
    }

    std::wstring currentPath = GetCurrentBufferPath();
    std::wstring docTitle = PathFindFileNameW(currentPath.c_str());
    if (docTitle.empty()) docTitle = L"Markdown Reader";

    bool isDark = IsDarkThemeEffective();

    if (m_webViewViewer.UpdateMarkdown(utf8Content, docTitle, isDark, m_config.isSmartBiDiEnabled, caretLine,
                                      std::to_wstring(GetCurrentBufferId()))) {
        m_lastRawContentUtf8 = utf8Content;
        m_lastRenderedSourceRevision = sourceRevision;
        return true;
    }
    return false;
}

void NppMarkdownNext::HandleCheckboxToggle(const CheckboxRequest& request) {
    if (m_shuttingDown || !m_isNppReady) return;
    auto reject = [this]() { RequestRenderDebounced(true); };
    HWND hSci = GetCurrentScintilla();
    if (!hSci || !m_webViewViewer.IsCurrentRender(request) || request.bufferId != std::to_wstring(GetCurrentBufferId()) ||
        m_sourceRevision != m_lastRenderedSourceRevision || SendMessage(hSci, SCI_GETREADONLY, 0, 0) ||
        GetScintillaText(hSci) != m_lastRawContentUtf8) {
        reject();
        return;
    }

    auto lineStart = SendMessage(hSci, SCI_POSITIONFROMLINE, request.line, 0);
    auto lineLen = SendMessage(hSci, SCI_LINELENGTH, request.line, 0);
    if (lineStart < 0 || lineLen < 3 || request.markerColumn < 0 || request.markerColumn > lineLen - 3) {
        reject();
        return;
    }
    std::string lineText(static_cast<size_t>(lineLen) + 1, '\0');
    SendMessage(hSci, SCI_GETLINE, request.line, reinterpret_cast<LPARAM>(lineText.data()));
    lineText.resize(static_cast<size_t>(lineLen));
    while (!lineText.empty() && (lineText.back() == '\r' || lineText.back() == '\n')) lineText.pop_back();

    if (!IsTaskMarker(lineText, static_cast<size_t>(request.markerColumn), request.expectedState)) {
        reject();
        return;
    }

    ++m_sourceRevision;
    SendMessage(hSci, SCI_SETSEL, lineStart + request.markerColumn, lineStart + request.markerColumn + 3);
    SendMessage(hSci, SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(request.expectedState == ' ' ? "[x]" : "[ ]"));
    RequestRenderDebounced(true);
}

void NppMarkdownNext::OnNotification(SCNotification* notifyCode) {
    if (!notifyCode || !m_isNppReady) return;

    if (notifyCode->nmhdr.code == SCN_MODIFIED) {
        if (notifyCode->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT)) {
            OnDocModified();
        }
    } else if (notifyCode->nmhdr.code == SCN_UPDATEUI) {
        if (m_isPanelVisible && m_config.syncWithCaret) {
            HWND hSci = GetCurrentScintilla();
            if (hSci) {
                int line = GetScintillaCaretLine(hSci);
                if (line != m_lastScrolledCaretLine) {
                    m_lastScrolledCaretLine = line;
                    m_webViewViewer.ScrollToLine(line);
                }
            }
        }
    }
}

void NppMarkdownNext::OnBufferActivated() {
    if (!m_isNppReady || m_shuttingDown) return;
    ++m_sourceRevision;
    m_activeBufferId = GetCurrentBufferId();
    m_activeBufferPath = GetCurrentBufferPath();
    ApplyAutoShowPanel();
    if (m_isPanelVisible) {
        RequestRenderDebounced(true);
    }
}

void NppMarkdownNext::OnBufferClosed(UINT_PTR bufferId) {
    if (!m_isNppReady || m_shuttingDown) return;
    m_webViewViewer.NotifyBufferClosed(std::to_wstring(bufferId));
}

void NppMarkdownNext::OnDocumentPathChanged(UINT_PTR bufferId) {
    if (!m_isNppReady || m_shuttingDown || bufferId != GetCurrentBufferId()) return;
    std::wstring path = GetCurrentBufferPath();
    if (bufferId == m_activeBufferId && path == m_activeBufferPath) return;
    m_activeBufferId = bufferId;
    m_activeBufferPath = std::move(path);
    ApplyAutoShowPanel();
    if (m_isPanelVisible) RequestRenderDebounced(true);
}

void NppMarkdownNext::OnDocModified() {
    ++m_sourceRevision;
    RequestRenderDebounced();
}

LRESULT CALLBACK NppMarkdownNext::PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    NppMarkdownNext* self = nullptr;
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = reinterpret_cast<NppMarkdownNext*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<NppMarkdownNext*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!self) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {
        case WM_SHOWWINDOW: {
            self->m_isPanelVisible = (wParam != FALSE);
            int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
            if (cmdId > 0) {
                SendMessage(self->m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(self->m_isPanelVisible ? TRUE : FALSE));
            }
            if (self->m_isPanelVisible) {
                RECT rc;
                GetClientRect(hwnd, &rc);
                self->m_webViewViewer.Resize(0, 0, rc.right - rc.left, rc.bottom - rc.top);
                self->m_webViewViewer.SetVisible(self->m_webViewViewer.IsPageReady());
                self->RequestRenderDebounced(true);
                if (self->m_viewerStatusVisible) {
                    ShowWindow(self->m_hViewerStatus, SW_SHOW);
                    ShowWindow(self->m_hRetryViewer, IsWindowEnabled(self->m_hRetryViewer) ? SW_SHOW : SW_HIDE);
                }
            } else {
                KillTimer(hwnd, IDT_RENDER_DEBOUNCE);
                self->m_forceRenderPending = false;
                self->m_webViewViewer.SetVisible(false);
                ShowWindow(self->m_hViewerStatus, SW_HIDE);
                ShowWindow(self->m_hRetryViewer, SW_HIDE);
            }
            return 0;
        }

        case WM_NOTIFY: {
            NMHDR* pnm = reinterpret_cast<NMHDR*>(lParam);
            if (pnm && pnm->code == DMN_CLOSE) {
                self->m_isPanelVisible = false;
                self->m_userClosedPanel = true;
                KillTimer(hwnd, IDT_RENDER_DEBOUNCE);
                self->m_forceRenderPending = false;
                self->m_webViewViewer.SetVisible(false);
                int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
                if (cmdId > 0) {
                    SendMessage(self->m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, FALSE);
                }
                return 0;
            }
            break;
        }

        case WM_TIMER: {
            if (wParam == IDT_RENDER_DEBOUNCE) {
                KillTimer(hwnd, IDT_RENDER_DEBOUNCE);
                bool force = std::exchange(self->m_forceRenderPending, false);
                if (!self->m_shuttingDown && self->m_isPanelVisible) self->ExecuteRender(force);
                return 0;
            }
            break;
        }

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            self->m_webViewViewer.Resize(0, 0, width, height);
            self->LayoutViewerStatus();
            return 0;
        }

        case WM_APP_VIEWER_RECOVERY:
            if (!self->m_shuttingDown && wParam == self->m_viewerGeneration) {
                self->m_recoveryQueued = false;
                self->HandleViewerRecovery();
            }
            return 0;

        case WM_APP_EXPORT_REQUEST:
            if (!self->m_shuttingDown && self->m_exportQueued) {
                std::wstring operation = std::move(self->m_queuedExportOperation);
                std::wstring requestId = std::move(self->m_queuedExportRequestId);
                self->m_exportQueued = false;
                if (operation == L"copyHtml") self->CopyRenderedHtml(requestId);
                else if (operation == L"saveAsHtml") self->SaveAsHtml(requestId);
            }
            return 0;

        case WM_APP_EXPORT_RESULT: {
            auto warnings = std::exchange(self->m_pendingExportWarnings, {});
            if (!self->m_shuttingDown) for (const auto& warning : warnings) self->ShowExportWarning(warning);
            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_RETRY_VIEWER && HIWORD(wParam) == BN_CLICKED) {
                self->HandleViewerRecovery(true);
                return 0;
            }
            break;

        case WM_CTLCOLORSTATIC: {
            HDC dc = reinterpret_cast<HDC>(wParam);
            bool dark = self->IsDarkThemeEffective();
            SetTextColor(dc, dark ? RGB(235, 235, 235) : RGB(25, 25, 25));
            SetBkColor(dc, dark ? RGB(0, 0, 0) : RGB(255, 255, 255));
            return reinterpret_cast<LRESULT>(GetStockObject(dark ? BLACK_BRUSH : WHITE_BRUSH));
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(reinterpret_cast<HDC>(wParam), &rect, static_cast<HBRUSH>(GetStockObject(self->IsDarkThemeEffective() ? BLACK_BRUSH : WHITE_BRUSH)));
            return TRUE;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY: {
            self->m_webViewViewer.Close();
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
