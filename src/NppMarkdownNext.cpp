#include "../include/NppMarkdownNext.h"
#include "../include/BiDiEngine.h"
#include "../include/HtmlExporter.h"
#include "../include/Resource.h"
#include "../include/NppLog.h"
#include "../include/ThemeCatalog.h"
#include "../include/LinkPolicy.h"
#include "../include/ReaderText.h"
#include "../include/Version.h"
#include "../include/ReaderPresets.inc"
#include <optional>
#include <commctrl.h>
#include <shlwapi.h>
#ifndef URL_UNESCAPE_AS_UTF8
#define URL_UNESCAPE_AS_UTF8 0x00040000 // Windows 8+ (shlwapi.h); older MinGW headers lack the name
#endif
#include <commdlg.h>
#include <shellapi.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <utility>
#include <cstdint>
#include <iomanip>
#include <climits>

extern "C" int GetPluginCmdId(int index);

namespace {
const wchar_t* s_panelClassName = L"NppMarkdownNextWindowClass";
const wchar_t* s_overlayClassName = L"NppMarkdownNextOverlayClass";
const wchar_t* s_viewerHostClassName = L"NppMarkdownNextViewerHostClass";
constexpr UINT_PTR kSubclassIdNpp = 0x4D4401;
constexpr UINT_PTR kSubclassIdSciMain = 0x4D4402;
constexpr UINT_PTR kSubclassIdSciSub = 0x4D4403;

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

// A preset value from "<prefix><value>", only if it is one of the offered choices (0 is a valid width: the whole pane).
template <size_t N> std::optional<int> ReadingMessageValue(const std::wstring& message, const wchar_t* prefix, const int (&choices)[N]) {
    for (int value : choices) if (message == std::wstring(prefix) + std::to_wstring(value)) return value;
    return std::nullopt;
}

struct ExportScope {
    std::function<void()> finish;
    ~ExportScope() { finish(); }
};

// Per-document state is stored under a hash of the document's case-folded path:
// INI keys cannot carry every path character ('=', ';', '[').
std::wstring DocumentKey(std::wstring path) {
    CharLowerBuffW(path.data(), static_cast<DWORD>(path.size()));
    uint64_t hash = 1469598103934665603ULL;
    for (wchar_t c : path) { hash ^= static_cast<uint16_t>(c); hash *= 1099511628211ULL; }
    wchar_t key[17] = {};
    swprintf(key, 17, L"%016llx", static_cast<unsigned long long>(hash));
    return key;
}

// \\server\share prefix of a plain UNC path, lower-cased; empty for anything else, including the \\?\ and \\.\
// device forms (never treated as "the same share": \\?\UNC\<any host> would otherwise all compare equal).
std::wstring UncShare(const std::wstring& path) {
    if (path.starts_with(L"\\\\?\\") || path.starts_with(L"\\\\.\\") || !PathIsUNCW(path.c_str())) return L"";
    const size_t server = path.find(L'\\', 2);
    if (server == std::wstring::npos) return L"";
    std::wstring share = path.substr(0, path.find(L'\\', server + 1));
    CharLowerBuffW(share.data(), static_cast<DWORD>(share.size()));
    return share;
}
bool IsOnSameShare(const std::wstring& target, const std::wstring& document) {
    const std::wstring share = UncShare(target);
    return !share.empty() && share == UncShare(document);
}

// Each kind of per-document state: its INI section, the reader's message prefix and the characters its wire
// format may use (validated both ways; see persistTableWidths / persistReadingPosition in the reader).
struct DocumentStateSpec { const wchar_t* section; const wchar_t* message; const wchar_t* charset; size_t maxLength; };
constexpr DocumentStateSpec kDocumentStates[] = {
    {L"Widths", L"tableWidths", L"0123456789abcdef:,;", 4096},  // "fp:w,w;fp:w"
    {L"Reading", L"readingPosition", L"0123456789:-", 24},       // "line:offset"
};
const DocumentStateSpec& Spec(DocumentState kind) { return kDocumentStates[static_cast<size_t>(kind)]; }
bool IsDocumentStateValue(DocumentState kind, const std::wstring& value) {
    return value.size() <= Spec(kind).maxLength && value.find_first_not_of(Spec(kind).charset) == std::wstring::npos;
}
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
    m_documentStatePath = std::wstring(configDir) + L"\\NppMarkdownNext.documents.ini";
    // one-time move of the column widths kept by the first version under the older name
    const std::wstring legacyTables = std::wstring(configDir) + L"\\NppMarkdownNext.tables.ini";
    if (GetFileAttributesW(m_documentStatePath.c_str()) == INVALID_FILE_ATTRIBUTES)
        MoveFileExW(legacyTables.c_str(), m_documentStatePath.c_str(), 0);
    m_config.Load(m_configPath);
    m_restorePanelOnStartup = m_config.isPanelVisible;
    ApplyReaderLanguage(); // before getFuncsArray names the menu items
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
    SyncHideEditorMenuItem();
    // With auto-show on, the tab decides; otherwise reopen the panel that was open at the last exit.
    if (!m_config.autoShowPanel && m_restorePanelOnStartup) SetPanelVisible(true);
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
    m_queuedExportContent = {};
    m_pendingExportWarnings.clear();
    ++m_viewerGeneration;
    if (m_hPanel) { KillTimer(m_hPanel, IDT_RENDER_DEBOUNCE); KillTimer(m_hPanel, IDT_VIEWER_LOADING); }
    RemoveHostSubclasses();
    m_overlayActive = false;
    m_suppressDockNotification = false;
    m_overlaySyncQueued = false;
    SaveConfig(false);
    m_webViewViewer.Close();
    if (m_hViewerHost && IsWindow(m_hViewerHost)) {
        DestroyWindow(m_hViewerHost);
        m_hViewerHost = nullptr;
    }
    if (m_hOverlay && IsWindow(m_hOverlay)) {
        DestroyWindow(m_hOverlay);
        m_hOverlay = nullptr;
    }
    if (m_hPanel && IsWindow(m_hPanel)) {
        DestroyWindow(m_hPanel);
        m_hPanel = nullptr;
    }
    m_hViewerStatus = nullptr;
    m_hRetryViewer = nullptr;
    if (m_statusFont) { DeleteObject(m_statusFont); m_statusFont = nullptr; m_statusFontDpi = 0; }
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

    WNDCLASSEX overlayWc = wc;
    overlayWc.lpfnWndProc = OverlayWndProc;
    overlayWc.lpszClassName = s_overlayClassName;
    RegisterClassEx(&overlayWc);

    WNDCLASSEX hostWc = wc;
    hostWc.lpfnWndProc = ViewerHostWndProc;
    hostWc.lpszClassName = s_viewerHostClassName;
    RegisterClassEx(&hostWc);

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

    m_hOverlay = CreateWindowEx(
        WS_EX_CONTROLPARENT,
        s_overlayClassName,
        L"Markdown Reader Overlay",
        WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        0, 0, 450, 700,
        m_nppData._nppHandle,
        nullptr,
        m_hInst,
        this
    );

    m_hViewerHost = CreateWindowEx(
        WS_EX_CONTROLPARENT,
        s_viewerHostClassName,
        L"Markdown Reader Host",
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        0, 0, 450, 700,
        m_hPanel,
        nullptr,
        m_hInst,
        this
    );
    if (!m_hViewerHost) return false;

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
        m_hViewerHost, nullptr, m_hInst, nullptr);
    m_hRetryViewer = CreateWindowExW(0, L"BUTTON", Tr("status.retry").c_str(), WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
        16, 96, 120, 30, m_hViewerHost, reinterpret_cast<HMENU>(IDC_RETRY_VIEWER), m_hInst, nullptr);
    UpdateStatusFont();

    m_webViewViewer.SetPageReadyCallback([this]() {
        if (m_shuttingDown) return;
        NppLog("WebView2 page ready, executing initial render");
        KillTimer(m_hPanel, IDT_VIEWER_LOADING);
        m_viewerStatusVisible = false;
        ShowWindow(m_hViewerStatus, SW_HIDE);
        ShowWindow(m_hRetryViewer, SW_HIDE);
        m_webViewViewer.SetVisible(m_isPanelVisible);
        if (m_overlayActive) {
            UpdateOverlayBounds();
            m_webViewViewer.Focus();
        }
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
        // Keep double-click word selection working undisturbed while the editor is hidden.
        if (m_overlayActive) return;
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

    m_webViewViewer.SetHostShortcutCallback([this](UINT vk, bool ctrl, bool shift, bool alt, bool isRepeat) -> bool {
        if (m_shuttingDown || !m_hPanel) return false;
        // Post to the panel window so Notepad++ commands (including modal Save prompts) run outside WebView2's COM stack.
        if (ctrl && !shift && !alt && (vk == 'W' || vk == VK_F4)) {
            if (!isRepeat) PostMessage(m_hPanel, WM_APP_HOST_COMMAND, IDM_FILE_CLOSE, 0);
            return true;
        }
        if (ctrl && !shift && !alt && vk == 'S') {
            if (!isRepeat) PostMessage(m_hPanel, WM_APP_HOST_COMMAND, IDM_FILE_SAVE, 0);
            return true;
        }
        if (ctrl && shift && !alt && vk == 'M') {
            if (!isRepeat) PostMessage(m_hPanel, WM_APP_HOST_COMMAND, 0, 1);
            return true;
        }
        if (ctrl && shift && !alt && vk == 'E') {
            if (!isRepeat) PostMessage(m_hPanel, WM_APP_HOST_COMMAND, 0, 2);
            return true;
        }
        return false;
    });

    InitializeViewer();

    return true;
}

void NppMarkdownNext::LayoutViewerStatus() {
    HWND host = m_hViewerHost ? m_hViewerHost : m_hPanel;
    if (!host) return;
    RECT rect = {};
    GetClientRect(host, &rect);
    UpdateStatusFont();
    const int dpi = static_cast<int>(m_statusFontDpi ? m_statusFontDpi : 96);
    auto scale = [dpi](int value) { return MulDiv(value, dpi, 96); };
    int width = (std::max)(1L, rect.right - rect.left);
    int margin = (std::min)(scale(16), width / 8);
    int buttonWidth = (std::min)(scale(140), (std::max)(1, width - 2 * margin));
    SetWindowPos(m_hViewerStatus, HWND_TOP, margin, scale(24), (std::max)(1, width - 2 * margin), scale(72), SWP_NOACTIVATE);
    SetWindowPos(m_hRetryViewer, HWND_TOP, (width - buttonWidth) / 2, scale(108), buttonWidth, scale(30), SWP_NOACTIVATE);
}

void NppMarkdownNext::SetViewerStatus(const std::wstring& text, bool retry) {
    m_viewerStatusVisible = true;
    // A status with Retry is final (an error): the loading timeout must not replace its specific text.
    if (retry && m_hPanel) KillTimer(m_hPanel, IDT_VIEWER_LOADING);
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
    SetViewerStatus(Tr("status.loading"), false);
    // Retry stays disabled while loading; a start that never reports back must not leave it disabled forever.
    SetTimer(m_hPanel, IDT_VIEWER_LOADING, VIEWER_LOADING_TIMEOUT_MS, nullptr);
    m_webViewViewer.SetVisible(false);
    const std::wstring userDataDir = GetViewerDataDirectory();
    if (userDataDir.empty()) {
        const DWORD error = GetLastError();
        SetViewerStatus(Tr("status.noTemp", std::to_wstring(error)), true);
        return false;
    }
    bool started = m_webViewViewer.Initialize(m_hViewerHost ? m_hViewerHost : m_hPanel, userDataDir);
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
        SetViewerStatus(Tr("status.loadFailed"), true);
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
    const auto length = static_cast<size_t>(SendMessage(hSci, SCI_GETLENGTH, 0, 0));
    if (length == 0) return "";

    std::string buffer(length + 1, '\0');
    SendMessage(hSci, SCI_GETTEXT, length + 1, (LPARAM)&buffer[0]);
    buffer.resize(length);
    // Notepad++ keeps "ANSI" documents in the system code page (Scintilla code page 0) and DBCS ones in theirs;
    // the reader and the exporter expect UTF-8, so convert instead of showing a Windows-1251 file as mojibake.
    const auto codePage = static_cast<UINT>(SendMessage(hSci, SCI_GETCODEPAGE, 0, 0));
    if (codePage != SC_CP_UTF8 && length <= static_cast<size_t>(INT_MAX)) {
        const UINT source = codePage == 0 ? CP_ACP : codePage;
        const int wideLength = MultiByteToWideChar(source, 0, buffer.data(), static_cast<int>(length), nullptr, 0);
        if (wideLength > 0) {
            std::wstring wide(static_cast<size_t>(wideLength), L'\0');
            MultiByteToWideChar(source, 0, buffer.data(), static_cast<int>(length), wide.data(), wideLength);
            return BiDiEngine::WideToUtf8(wide);
        }
    }
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

// The plugin's text follows its setting, or with "auto" the language Notepad++ itself is shown in.
void NppMarkdownNext::ApplyReaderLanguage() {
    char file[64] = {};
    if (m_nppData._nppHandle) SendMessage(m_nppData._nppHandle, NPPM_GETNATIVELANGFILENAME, sizeof(file), reinterpret_cast<LPARAM>(file));
    m_nativeLanguageRussian = _strnicmp(file, "russian", 7) == 0;
    ReaderTextRussian() = m_config.readerLanguage == L"ru" || (m_config.readerLanguage == L"auto" && m_nativeLanguageRussian);
    // A later change also renames the Plugins menu items Notepad++ already shows, keeping their shortcut text.
    HMENU menu = m_nppData._nppHandle ? reinterpret_cast<HMENU>(SendMessage(m_nppData._nppHandle, NPPM_GETMENUHANDLE, NPPPLUGINMENU, 0)) : nullptr;
    for (int i = 0; menu && i < NB_PLUGIN_COMMANDS; ++i) {
        wchar_t current[256] = {};
        MENUITEMINFOW info{sizeof(info), MIIM_STRING};
        info.dwTypeData = current;
        info.cch = static_cast<UINT>(std::size(current));
        if (!GetMenuItemInfoW(menu, GetPluginCmdId(i), FALSE, &info)) continue;
        const wchar_t* tab = wcschr(current, L'\t');
        std::wstring text = Tr(kPluginMenuKeys[i]) + (tab ? tab : L"");
        info.dwTypeData = text.data();
        SetMenuItemInfoW(menu, GetPluginCmdId(i), FALSE, &info);
    }
    if (menu) DrawMenuBar(m_nppData._nppHandle);
}

// The folder of the document on screen, empty for an unsaved tab. A drive root keeps its separator ("C:\"):
// "C:" alone would mean that drive's current directory.
std::wstring NppMarkdownNext::GetCurrentDocumentFolder() const {
    const std::wstring path = GetCurrentBufferPath();
    const size_t end = path.find_last_of(L'\\');
    return end == std::wstring::npos ? L"" : path.substr(0, end == 2 ? 3 : end);
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

void NppMarkdownNext::ToggleHideEditor() {
    SetHideEditor(!m_config.hideEditor);
}

void NppMarkdownNext::SetHideEditor(bool hide, bool saveConfig) {
    if (m_shuttingDown) return;
    m_config.hideEditor = hide;
    if (hide && !m_isPanelVisible) {
        m_userClosedPanel = false;
        SetPanelVisible(true);
    } else if (m_isPanelRegistered) {
        UpdateHostLayout();
    }
    SyncHideEditorMenuItem();
    if (saveConfig) SaveConfig();
    SyncUiState();
}

void NppMarkdownNext::SyncHideEditorMenuItem() {
    int cmdHide = GetPluginCmdId(CMD_TOGGLE_HIDE_EDITOR);
    if (cmdHide > 0 && m_nppData._nppHandle) {
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdHide, (LPARAM)(m_config.hideEditor ? TRUE : FALSE));
    }
}

void NppMarkdownNext::InstallHostSubclasses() {
    if (m_subclassesInstalled) return;
    if (m_nppData._nppHandle && IsWindow(m_nppData._nppHandle)) {
        SetWindowSubclass(m_nppData._nppHandle, HostSubclassProc, kSubclassIdNpp, reinterpret_cast<DWORD_PTR>(this));
    }
    if (m_nppData._scintillaMainHandle && IsWindow(m_nppData._scintillaMainHandle) &&
        m_nppData._scintillaMainHandle != m_nppData._nppHandle) {
        SetWindowSubclass(m_nppData._scintillaMainHandle, HostSubclassProc, kSubclassIdSciMain, reinterpret_cast<DWORD_PTR>(this));
    }
    if (m_nppData._scintillaSecondHandle && IsWindow(m_nppData._scintillaSecondHandle) &&
        m_nppData._scintillaSecondHandle != m_nppData._nppHandle &&
        m_nppData._scintillaSecondHandle != m_nppData._scintillaMainHandle) {
        SetWindowSubclass(m_nppData._scintillaSecondHandle, HostSubclassProc, kSubclassIdSciSub, reinterpret_cast<DWORD_PTR>(this));
    }
    m_subclassesInstalled = true;
}

void NppMarkdownNext::RemoveHostSubclasses() {
    if (!m_subclassesInstalled) return;
    if (m_nppData._nppHandle && IsWindow(m_nppData._nppHandle)) {
        RemoveWindowSubclass(m_nppData._nppHandle, HostSubclassProc, kSubclassIdNpp);
    }
    if (m_nppData._scintillaMainHandle && IsWindow(m_nppData._scintillaMainHandle) &&
        m_nppData._scintillaMainHandle != m_nppData._nppHandle) {
        RemoveWindowSubclass(m_nppData._scintillaMainHandle, HostSubclassProc, kSubclassIdSciMain);
    }
    if (m_nppData._scintillaSecondHandle && IsWindow(m_nppData._scintillaSecondHandle) &&
        m_nppData._scintillaSecondHandle != m_nppData._nppHandle &&
        m_nppData._scintillaSecondHandle != m_nppData._scintillaMainHandle) {
        RemoveWindowSubclass(m_nppData._scintillaSecondHandle, HostSubclassProc, kSubclassIdSciSub);
    }
    m_subclassesInstalled = false;
}

void NppMarkdownNext::ScheduleOverlaySync() {
    if (m_shuttingDown || !m_overlayActive || !m_hOverlay || !IsWindow(m_hOverlay) || m_overlaySyncQueued) return;
    m_overlaySyncQueued = true;
    if (!PostMessage(m_hOverlay, WM_APP_SYNC_OVERLAY, 0, 0)) m_overlaySyncQueued = false;
}

void NppMarkdownNext::UpdateOverlayBounds() {
    if (m_shuttingDown || !m_overlayActive || !m_hOverlay || !IsWindow(m_hOverlay) || !m_nppData._nppHandle) return;
    HWND hSci = GetCurrentScintilla();
    if (!hSci || !IsWindow(hSci)) hSci = m_nppData._scintillaMainHandle;
    if (!hSci || !IsWindow(hSci)) return;

    RECT rc = {};
    if (hSci == m_nppData._nppHandle) {
        GetClientRect(m_nppData._nppHandle, &rc);
    } else {
        // Ensure Scintilla clips sibling child windows so GDI painting/caret does not bleed over the overlay.
        const LONG_PTR style = GetWindowLongPtrW(hSci, GWL_STYLE);
        if ((style & WS_CLIPSIBLINGS) == 0) SetWindowLongPtrW(hSci, GWL_STYLE, style | WS_CLIPSIBLINGS);
        if (!GetWindowRect(hSci, &rc)) return;
        MapWindowPoints(HWND_DESKTOP, m_nppData._nppHandle, reinterpret_cast<POINT*>(&rc), 2);
    }
    const int w = static_cast<int>((std::max)(0L, rc.right - rc.left));
    const int h = static_cast<int>((std::max)(0L, rc.bottom - rc.top));
    if (w <= 0 || h <= 0) return;

    SetWindowPos(m_hOverlay, HWND_TOP, rc.left, rc.top, w, h, SWP_SHOWWINDOW | SWP_NOACTIVATE);
    if (m_hViewerHost && GetParent(m_hViewerHost) == m_hOverlay) {
        SetWindowPos(m_hViewerHost, nullptr, 0, 0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    m_webViewViewer.Resize(0, 0, w, h);
    m_webViewViewer.NotifyParentWindowPositionChanged();
    LayoutViewerStatus();
}

void NppMarkdownNext::UpdateHostLayout() {
    if (m_shuttingDown || !m_isPanelRegistered || !m_hPanel || !m_hViewerHost) return;
    const bool wantOverlay = m_isPanelVisible && m_config.hideEditor && m_hOverlay && IsWindow(m_hOverlay);
    if (wantOverlay) {
        // Hide the docked panel first so Notepad++ expands the tab bar and Scintilla to full width.
        m_suppressDockNotification = true;
        SendMessage(m_nppData._nppHandle, NPPM_DMMHIDE, 0, reinterpret_cast<LPARAM>(m_hPanel));
        m_suppressDockNotification = false;

        const bool reparented = GetParent(m_hViewerHost) != m_hOverlay;
        if (reparented) SetParent(m_hViewerHost, m_hOverlay);
        ShowWindow(m_hViewerHost, SW_SHOW);
        m_overlayActive = true;
        InstallHostSubclasses();
        UpdateOverlayBounds();
        ScheduleOverlaySync();
        m_webViewViewer.SetVisible(m_webViewViewer.IsPageReady());
        if (m_viewerStatusVisible) {
            ShowWindow(m_hViewerStatus, SW_SHOW);
            ShowWindow(m_hRetryViewer, IsWindowEnabled(m_hRetryViewer) ? SW_SHOW : SW_HIDE);
        }
        if (reparented && m_webViewViewer.IsPageReady()) m_webViewViewer.Focus();
    } else {
        const bool wasOverlay = m_overlayActive;
        m_overlayActive = false;
        m_overlaySyncQueued = false;
        if (m_hOverlay && IsWindow(m_hOverlay)) ShowWindow(m_hOverlay, SW_HIDE);
        const bool reparented = GetParent(m_hViewerHost) != m_hPanel;
        if (reparented) SetParent(m_hViewerHost, m_hPanel);
        if (m_isPanelVisible) {
            ShowWindow(m_hViewerHost, SW_SHOW);
            m_suppressDockNotification = true;
            SendMessage(m_nppData._nppHandle, NPPM_DMMSHOW, 0, reinterpret_cast<LPARAM>(m_hPanel));
            m_suppressDockNotification = false;
            RECT rc = {};
            GetClientRect(m_hPanel, &rc);
            const int w = static_cast<int>((std::max)(1L, rc.right - rc.left));
            const int h = static_cast<int>((std::max)(1L, rc.bottom - rc.top));
            SetWindowPos(m_hViewerHost, nullptr, 0, 0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
            m_webViewViewer.Resize(0, 0, w, h);
            if (reparented || wasOverlay) m_webViewViewer.NotifyParentWindowPositionChanged();
            m_webViewViewer.SetVisible(m_webViewViewer.IsPageReady());
            if (m_viewerStatusVisible) {
                ShowWindow(m_hViewerStatus, SW_SHOW);
                ShowWindow(m_hRetryViewer, IsWindowEnabled(m_hRetryViewer) ? SW_SHOW : SW_HIDE);
            }
        } else {
            m_suppressDockNotification = true;
            SendMessage(m_nppData._nppHandle, NPPM_DMMHIDE, 0, reinterpret_cast<LPARAM>(m_hPanel));
            m_suppressDockNotification = false;
            m_webViewViewer.SetVisible(false);
            ShowWindow(m_hViewerStatus, SW_HIDE);
            ShowWindow(m_hRetryViewer, SW_HIDE);
        }
    }
}

void NppMarkdownNext::SetPanelVisible(bool visible) {
    if (m_shuttingDown) return;
    if (!visible && !m_isPanelRegistered) return;
    EnsurePanelRegistered();
    m_isPanelVisible = visible;
    m_config.isPanelVisible = visible;

    int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
    if (cmdId > 0) {
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(visible ? TRUE : FALSE));
    }

    UpdateHostLayout();

    if (visible) {
        RequestRenderDebounced(true);
    } else {
        KillTimer(m_hPanel, IDT_RENDER_DEBOUNCE);
        m_forceRenderPending = false;
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
            Tr("save.settingsFailed")});
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
                Tr("save.settingsFailed")});
            m_settingsWarningReported = true;
        }
    } else if (m_settingsSaveWarningActive) {
        m_settingsSaveWarningActive = false;
        m_settingsWarningReported = false;
        m_settingsSaveError = ERROR_SUCCESS;
        if (notifyUser) m_webViewViewer.ReportOperationResult(L"saveSettings", {OperationStatus::Succeeded, ERROR_SUCCESS, Tr("save.settingsSaved")});
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
            // "<op>:<id>" or, from the reader, "<op>:<id>\n<html length>\n<html><plain text>" (what it shows)
            const std::wstring rest = message.substr(prefix.size());
            const size_t idEnd = rest.find(L'\n');
            std::wstring id = rest.substr(0, idEnd);
            ExportContent content;
            if (idEnd != std::wstring::npos) {
                const size_t lengthEnd = rest.find(L'\n', idEnd + 1);
                const std::wstring digits = lengthEnd == std::wstring::npos ? L"" : rest.substr(idEnd + 1, lengthEnd - idEnd - 1);
                const size_t length = digits.empty() || digits.size() > 9 || digits.find_first_not_of(L"0123456789") != std::wstring::npos
                                          ? std::wstring::npos : std::stoul(digits);
                if (length == std::wstring::npos || length > rest.size() - lengthEnd - 1) {
                    ReportExportResult(operation, {OperationStatus::Failed, ERROR_INVALID_DATA, Tr("export.incomplete")}, id);
                    return;
                }
                content.present = true;
                content.htmlUtf8 = BiDiEngine::WideToUtf8(rest.substr(lengthEnd + 1, length));
                content.text = rest.substr(lengthEnd + 1 + length);
            }
            if (!id.empty() && IsRequestIdValid(id)) QueueExport(operation, id, std::move(content));
            else ReportExportResult(operation, {OperationStatus::Failed, ERROR_INVALID_PARAMETER, Tr("export.badId")}, id);
            return;
        }
    }
    if (message.starts_with(L"openLink:")) {
        OpenLink(message.substr(9));
        return;
    }
    for (const DocumentState kind : {DocumentState::TableWidths, DocumentState::ReadingPosition}) {
        const std::wstring prefix = std::wstring(Spec(kind).message) + L":";
        if (message.starts_with(prefix)) {
            SaveDocumentStateMessage(kind, message.substr(prefix.size()));
            return;
        }
    }
    if (message == L"toggleAutoShow") {
        m_config.autoShowPanel = !m_config.autoShowPanel;
        ApplyAutoShowPanel();
    } else if (message == L"toggleHideEditor") {
        ToggleHideEditor();
        return;
    } else if (message == L"tocStateChanged:true" || message == L"tocStateChanged:false") {
        m_config.showOutline = message == L"tocStateChanged:true";
    } else if (message == L"darkMode:-1" || message == L"darkMode:0" || message == L"darkMode:1") {
        if (m_config.theme != ReaderTheme::FollowNpp) { SyncUiState(); return; }
        m_config.darkModeOverride = message == L"darkMode:-1" ? -1 : (message == L"darkMode:1" ? 1 : 0);
        m_webViewViewer.SetTheme(IsDarkThemeEffective(), m_config.ThemeId());
    } else if (auto value = ReadingMessageValue(message, L"readingFontSize:", kFontSizePresets)) {
        m_config.contentFontSizePx = *value;
    } else if (auto value = ReadingMessageValue(message, L"readingLineHeight:", kLineHeightPresets)) {
        m_config.contentLineHeightPercent = *value;
    } else if (auto value = ReadingMessageValue(message, L"readingWidth:", kReadingWidthPresets)) {
        m_config.contentMaxWidthPx = *value;
    } else if (message == L"readerLanguage:auto" || message == L"readerLanguage:en" || message == L"readerLanguage:ru") {
        m_config.readerLanguage = message.substr(15);
        ApplyReaderLanguage();
        RequestRenderDebounced(true); // rebuilds the document's own controls (grips, buttons, image notes) in the new language
    } else if (message.starts_with(L"outlineWidth:")) {
        const std::wstring raw = message.substr(13);
        if (raw.empty() || raw.size() > 4 || raw.find_first_not_of(L"0123456789") != std::wstring::npos) return;
        m_config.outlineWidthPx = std::clamp(std::stoi(raw), kOutlineWidthMin, kOutlineWidthMax);
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

void NppMarkdownNext::QueueExport(const std::wstring& operation, const std::wstring& requestId, ExportContent content) {
    if (m_shuttingDown) return;
    if (m_exportInProgress || m_exportQueued) {
        ReportExportResult(operation, {OperationStatus::Failed, ERROR_BUSY, Tr("export.busy")}, requestId);
        return;
    }
    m_exportQueued = true;
    m_queuedExportOperation = operation;
    m_queuedExportRequestId = requestId;
    m_queuedExportContent = std::move(content);
    if (!PostMessage(m_hPanel, WM_APP_EXPORT_REQUEST, 0, 0)) {
        DWORD error = GetLastError();
        m_exportQueued = false;
        m_queuedExportOperation.clear();
        m_queuedExportRequestId.clear();
        m_queuedExportContent = {};
        ReportExportResult(operation, {OperationStatus::Failed, error, Tr("export.notStarted")}, requestId);
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
    MessageBoxW(m_nppData._nppHandle, message.c_str(), Tr("export.warningTitle").c_str(), MB_OK | MB_ICONWARNING);
}

void NppMarkdownNext::ReportExportResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId) {
    bool delivered = m_webViewViewer.ReportOperationResult(operation, result, requestId);
    if ((result.status != OperationStatus::Failed && result.status != OperationStatus::Partial) ||
        (!requestId.empty() && delivered && m_isPanelVisible && !m_viewerStatusVisible)) return;

    const bool partial = result.status == OperationStatus::Partial;
    std::wstring message = Tr(operation == L"copyHtml" ? (partial ? "export.copyPartial" : "export.copyFailed")
                                                       : (partial ? "export.savePartial" : "export.saveFailed")) + L"\n\n";
    message += result.detail;
    if (result.error) message += L"\n\n" + Tr("export.errorCode", std::to_wstring(result.error));
    if (!result.recoveryPath.empty()) message += L"\n\n" + Tr("export.recoveryFile", result.recoveryPath);
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

// Menu commands: the reader exports what it shows (same renderer); without a ready preview, the native parser.
void NppMarkdownNext::RequestHtmlExport(const std::wstring& operation) {
    if (m_isPanelVisible && m_webViewViewer.IsPageReady()) {
        m_webViewViewer.ExecuteScript(operation == L"copyHtml" ? L"copyHtmlFromMenu();" : L"saveAsHtmlFromMenu();");
        return;
    }
    if (operation == L"copyHtml") CopyRenderedHtml();
    else SaveAsHtml();
}

void NppMarkdownNext::CopyRenderedHtml(const std::wstring& requestId, const ExportContent& content) {
    if (m_shuttingDown) return;
    if (!IsRequestIdValid(requestId) || m_exportInProgress || m_exportQueued) {
        ReportExportResult(L"copyHtml", {OperationStatus::Failed, ERROR_BUSY, Tr("export.busy")}, requestId);
        return;
    }
    m_exportInProgress = true;
    ExportScope scope{[this]() { FinishExport(); }};
    HWND hSci = GetCurrentScintilla();
    if (!hSci) {
        ReportExportResult(L"copyHtml", {OperationStatus::Failed, ERROR_INVALID_HANDLE, Tr("export.noDocCopy")}, requestId);
        return;
    }
    const HWND owner = m_hPanel ? m_hPanel : m_nppData._nppHandle;
    if (content.present) {
        const std::string body = HtmlExporter::EmbedDocumentImages(content.htmlUtf8, GetCurrentDocumentFolder());
        ReportExportResult(L"copyHtml", HtmlExporter::CopyRenderedToClipboard(owner, body, content.text), requestId);
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

void NppMarkdownNext::SaveAsHtml(const std::wstring& requestId, const ExportContent& content) {
    if (m_shuttingDown) return;
    if (!IsRequestIdValid(requestId) || m_exportInProgress || m_exportQueued) {
        ReportExportResult(L"saveAsHtml", {OperationStatus::Failed, ERROR_BUSY, Tr("export.busy")}, requestId);
        return;
    }
    m_exportInProgress = true;
    ExportScope scope{[this]() { FinishExport(); }};
    HWND hSci = GetCurrentScintilla();
    if (!hSci) {
        ReportExportResult(L"saveAsHtml", {OperationStatus::Failed, ERROR_INVALID_HANDLE, Tr("export.noDocSave")}, requestId);
        return;
    }
    wchar_t fileName[MAX_PATH] = L"export.html";
    // Suggest the document's own name (report.md -> report.html) instead of a generic one.
    {
        std::wstring base = PathFindFileNameW(GetCurrentBufferPath().c_str());
        const size_t dot = base.find_last_of(L'.');
        if (dot != std::wstring::npos && dot > 0) base.resize(dot);
        std::erase_if(base, [](wchar_t c) { return c < 32 || wcschr(L"\\/:*?\"<>|", c) != nullptr; });
        if (!base.empty() && base.size() + 5 < MAX_PATH) wcscpy_s(fileName, (base + L".html").c_str());
    }
    OPENFILENAMEW ofn = { sizeof(OPENFILENAMEW) };
    ofn.hwndOwner = m_hPanel ? m_hPanel : m_nppData._nppHandle;
    ofn.lpstrFilter = L"HTML Document (*.html;*.htm)\0*.html;*.htm\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = L"html";

    if (GetSaveFileNameW(&ofn)) {
        const ReaderTheme exportTheme = m_config.theme == ReaderTheme::FollowNpp ?
                                            (IsDarkThemeEffective() ? ReaderTheme::Dark : ReaderTheme::Light) : m_config.theme;
        std::wstring title = PathFindFileNameW(GetCurrentBufferPath().c_str());
        if (title.empty()) title = L"Markdown Export";
        OperationResult result = content.present
            ? HtmlExporter::SaveRenderedToFile(fileName, HtmlExporter::EmbedDocumentImages(content.htmlUtf8, GetCurrentDocumentFolder()),
                                               title, exportTheme, m_config.beardedVariant)
            : HtmlExporter::SaveToFile(fileName, MarkdownParser::Parse(BiDiEngine::Utf8ToWide(GetScintillaText(hSci))), title, exportTheme, m_config.beardedVariant);
        ReportExportResult(L"saveAsHtml", result, requestId);
    } else {
        DWORD error = CommDlgExtendedError();
        ReportExportResult(L"saveAsHtml", {
            error ? OperationStatus::Failed : OperationStatus::Cancelled, error,
            error ? Tr("export.dialogFailed") : L""}, requestId);
    }
}

void NppMarkdownNext::ShowAbout() {
    MessageBoxW(m_nppData._nppHandle, Tr("about.text", NPPMD_VERSION_WSTRING).c_str(), Tr("about.title").c_str(), MB_OK | MB_ICONINFORMATION);
}

void NppMarkdownNext::RequestRenderDebounced(bool force) {
    if (m_shuttingDown || !m_isPanelVisible) return;
    // Edits keep the document on screen live, even one the user opened by hand (a Markdown-like .txt);
    // only an unrelated non-Markdown tab is left alone.
    if (!force && !IsCurrentBufferMarkdown() && m_webViewViewer.RenderedBufferId() != std::to_wstring(GetCurrentBufferId())) return;
    m_forceRenderPending = m_forceRenderPending || force;
    // Large documents wait a little longer between keystrokes: each render copies, converts and re-parses the
    // whole buffer, so typing in a 1 MB file renders after ~300 ms of quiet instead of every 50 ms.
    UINT delay = RENDER_DEBOUNCE_DELAY_MS;
    if (!m_forceRenderPending) {
        if (HWND hSci = GetCurrentScintilla()) {
            const auto length = static_cast<ULONGLONG>(SendMessage(hSci, SCI_GETLENGTH, 0, 0));
            delay += static_cast<UINT>((std::min)(350ULL, length / 4096));
        }
    }
    if (!SetTimer(m_hPanel, IDT_RENDER_DEBOUNCE, m_forceRenderPending ? 1 : delay, nullptr)) {
        NppLog("Render timer could not be scheduled error=%u", (unsigned)GetLastError());
    }
}

bool NppMarkdownNext::ExecuteRender(bool force) {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return false;

    const ULONGLONG sourceRevision = m_sourceRevision;
    int caretLine = m_config.syncWithCaret ? GetScintillaCaretLine(hSci) : -1;
    auto followCaret = [&]() {
        if (m_config.syncWithCaret && caretLine >= 0) m_webViewViewer.ScrollToLine(caretLine);
        return true;
    };
    // Every edit bumps the revision: an unchanged revision needs no copy of a possibly large buffer.
    if (!force && sourceRevision == m_lastRenderedSourceRevision) return followCaret();
    std::string utf8Content = GetScintillaText(hSci);
    if (!force && utf8Content == m_lastRawContentUtf8) {
        m_lastRenderedSourceRevision = sourceRevision; // e.g. an edit that was undone: the preview already matches
        return followCaret();
    }

    std::wstring currentPath = GetCurrentBufferPath();
    std::wstring docTitle = PathFindFileNameW(currentPath.c_str());
    if (docTitle.empty()) docTitle = L"Markdown Reader";

    bool isDark = IsDarkThemeEffective();
    const std::wstring bufferId = std::to_wstring(GetCurrentBufferId());
    const std::wstring folder = GetCurrentDocumentFolder();
    m_webViewViewer.SetDocumentFolder(folder);
    // Forced renders (tab switch, panel shown, page reload) refresh the saved widths; typing renders keep the reader's copy.
    if (force && !folder.empty()) {
        for (const DocumentState kind : {DocumentState::TableWidths, DocumentState::ReadingPosition})
            m_webViewViewer.SetDocumentState(Spec(kind).message, bufferId, LoadDocumentState(kind, currentPath));
    }

    if (m_webViewViewer.UpdateMarkdown(utf8Content, docTitle, isDark, m_config.isSmartBiDiEnabled, caretLine, bufferId)) {
        if (!m_pendingAnchor.empty() && CompareStringOrdinal(m_pendingAnchorPath.c_str(), -1, currentPath.c_str(), -1, TRUE) == CSTR_EQUAL) {
            m_webViewViewer.ScrollToAnchor(std::exchange(m_pendingAnchor, {}));
            m_pendingAnchorPath.clear();
        }
        m_lastRawContentUtf8 = utf8Content;
        m_lastRenderedSourceRevision = sourceRevision;
        return true;
    }
    return false;
}

std::wstring NppMarkdownNext::LoadDocumentState(DocumentState kind, const std::wstring& path) const {
    const std::wstring key = std::wstring(Spec(kind).section) + L"/" + DocumentKey(path);
    if (auto cached = m_documentStateCache.find(key); cached != m_documentStateCache.end()) return cached->second;
    if (m_documentStatePath.empty()) return L"";
    std::wstring value(Spec(kind).maxLength + 1, L'\0');
    value.resize(GetPrivateProfileStringW(Spec(kind).section, DocumentKey(path).c_str(), L"", value.data(),
                                          static_cast<DWORD>(value.size()), m_documentStatePath.c_str()));
    return IsDocumentStateValue(kind, value) ? value : L"";
}

// Keeps the value for the session even when the file cannot be written (and says so).
// shortcut: entries of deleted files are never pruned (~100 bytes each); add LRU pruning if the file ever grows large.
void NppMarkdownNext::StoreDocumentState(DocumentState kind, const std::wstring& path, const std::wstring& value) {
    std::wstring& cached = m_documentStateCache[std::wstring(Spec(kind).section) + L"/" + DocumentKey(path)];
    if (cached == value) return;
    cached = value;
    if (m_documentStatePath.empty()) return;
    if (!WritePrivateProfileStringW(Spec(kind).section, DocumentKey(path).c_str(), value.empty() ? nullptr : value.c_str(), m_documentStatePath.c_str())) {
        const DWORD error = GetLastError();
        NppLog("StoreDocumentState: write failed error=%lu", error);
        if (!m_documentStateWarningReported && m_webViewViewer.IsPageReady()) {
            m_webViewViewer.ReportOperationResult(L"saveSettings", {OperationStatus::Failed, error, Tr("save.stateFailed")});
            m_documentStateWarningReported = true;
        }
    }
}

// Save As / rename: the document keeps its column widths and reading place under the new name (copied, so a
// Save As leaves the original's state in place too).
void NppMarkdownNext::CopyDocumentState(const std::wstring& from, const std::wstring& to) {
    if (from.find(L'\\') == std::wstring::npos || to.find(L'\\') == std::wstring::npos || DocumentKey(from) == DocumentKey(to)) return;
    for (const DocumentState kind : {DocumentState::TableWidths, DocumentState::ReadingPosition}) {
        const std::wstring value = LoadDocumentState(kind, from);
        if (!value.empty() && LoadDocumentState(kind, to).empty()) StoreDocumentState(kind, to, value);
    }
}

// message = "<bufferId>:<value>"; an empty value removes the document's entry.
void NppMarkdownNext::SaveDocumentStateMessage(DocumentState kind, const std::wstring& message) {
    const size_t colon = message.find(L':');
    if (colon == 0 || colon == std::wstring::npos || colon > 20 ||
        message.find_first_not_of(L"0123456789") != colon) return;
    const std::wstring value = message.substr(colon + 1);
    if (!IsDocumentStateValue(kind, value)) {
        // the reader bounds what it sends, so only a malformed message lands here: nothing to tell the user
        NppLog("SaveDocumentStateMessage: rejected a malformed value (%u chars)", static_cast<unsigned>(value.size()));
        return;
    }
    const auto bufferId = static_cast<WPARAM>(std::wcstoull(message.substr(0, colon).c_str(), nullptr, 10));
    const LRESULT length = SendMessage(m_nppData._nppHandle, NPPM_GETFULLPATHFROMBUFFERID, bufferId, 0);
    if (length <= 0 || length > 32767) return;
    std::wstring path(static_cast<size_t>(length) + 1, L'\0');
    SendMessage(m_nppData._nppHandle, NPPM_GETFULLPATHFROMBUFFERID, bufferId, reinterpret_cast<LPARAM>(path.data()));
    path.resize(wcsnlen(path.c_str(), path.size()));
    if (path.find(L'\\') == std::wstring::npos) return; // unsaved tab: the reader keeps its state for the session only
    StoreDocumentState(kind, path, value);
}

// A link clicked in the preview (the reader routes every non-anchor link here).
// Web and mail links open in the default app. Local targets resolve against the document's folder: folders open
// in Explorer, documents/images in their default app, everything else in Notepad++ (as text, so a link can never
// run a program).
void NppMarkdownNext::OpenLink(const std::wstring& href) {
    if (href.empty() || href.size() > 4096 || href.find_first_of(L"\r\n") != std::wstring::npos) return;
    const std::wstring scheme = LinkScheme(href);
    if (scheme == L"http" || scheme == L"https" || scheme == L"mailto") {
        ShellExecuteW(nullptr, L"open", href.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return;
    }
    std::wstring target, fragment; // fragment: "plan.md#risks" scrolls the opened document to that section
    if (scheme == L"file") {
        std::wstring path(4096, L'\0');
        DWORD length = static_cast<DWORD>(path.size());
        if (FAILED(PathCreateFromUrlW(href.c_str(), path.data(), &length, 0))) return;
        path.resize(length);
        target = path;
    } else if (!scheme.empty()) {
        // another scheme (tel:, ftp:, app protocols) is not opened from a document
        m_webViewViewer.PostNotice(Tr("link.scheme", href));
        return;
    } else {
        target = href.substr(0, href.find_first_of(L"#?"));
        const size_t hash = href.find(L'#');
        if (hash != std::wstring::npos && href.size() - hash - 1 <= 256) fragment = href.substr(hash + 1);
        if (target.empty()) return;
        DWORD length = static_cast<DWORD>(target.size() + 1);
        UrlUnescapeW(target.data(), nullptr, &length, URL_UNESCAPE_INPLACE | URL_UNESCAPE_AS_UTF8);
        target.resize(wcslen(target.c_str()));
        std::replace(target.begin(), target.end(), L'/', L'\\');
    }
    if (PathIsRelativeW(target.c_str())) {
        const std::wstring folder = GetCurrentDocumentFolder();
        if (folder.empty()) {
            m_webViewViewer.PostNotice(Tr("link.saveFirst"));
            return;
        }
        target = folder + (folder.back() == L'\\' ? L"" : L"\\") + target;
    }
    std::wstring full(32768, L'\0');
    const DWORD fullLength = GetFullPathNameW(target.c_str(), static_cast<DWORD>(full.size()), full.data(), nullptr);
    if (!fullLength || fullLength >= full.size()) return;
    full.resize(fullLength);
    // Touching \\server\... makes Windows authenticate to that server (NTLM): a link in someone else's Markdown
    // must not reach an arbitrary host. Allowed only within the share the document itself was opened from.
    // Any "\\" prefix counts: \\server\share, \\?\UNC\..., \\.\device.
    // shortcut: the check is textual, so a local junction/symlink that points at a share is still followed;
    // that needs a link planted in the user's own folder; resolve reparse points first if that ever matters.
    if (full.starts_with(L"\\\\") && !IsOnSameShare(full, GetCurrentBufferPath())) {
        m_webViewViewer.PostNotice(Tr("link.network", full));
        return;
    }
    std::wstring extension = PathFindExtensionW(full.c_str());
    CharLowerBuffW(extension.data(), static_cast<DWORD>(extension.size()));
    // Shell links redirect to anything (a program, a share): never followed from a document.
    static constexpr const wchar_t* kShellLinks[] = {L".lnk", L".url", L".pif", L".scf", L".library-ms", L".searchconnector-ms"};
    if (std::any_of(std::begin(kShellLinks), std::end(kShellLinks), [&](const wchar_t* known) { return extension == known; })) {
        m_webViewViewer.PostNotice(Tr("link.shortcut", full));
        return;
    }
    const DWORD attributes = GetFileAttributesW(full.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        m_webViewViewer.PostNotice(Tr("link.notFound", full));
        return;
    }
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        ShellExecuteW(nullptr, L"explore", full.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return;
    }
    // No .svg (a browser would run its script) and no macro-capable legacy Office formats: those open as text.
    // The rest is the usual trade-off of a document link (as in a browser): PDF/OOXML open in their own viewers,
    // which apply their own protections (they can still fetch remote content they reference).
    static constexpr const wchar_t* kDefaultApp[] = {L".pdf", L".png", L".jpg", L".jpeg", L".gif", L".bmp", L".webp",
        L".docx", L".xlsx", L".pptx", L".odt", L".ods", L".odp"};
    if (std::any_of(std::begin(kDefaultApp), std::end(kDefaultApp), [&](const wchar_t* known) { return extension == known; }))
        ShellExecuteW(nullptr, L"open", full.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    else {
        const bool alreadyShown = CompareStringOrdinal(full.c_str(), -1, GetCurrentBufferPath().c_str(), -1, TRUE) == CSTR_EQUAL;
        SendMessage(m_nppData._nppHandle, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(full.c_str()));
        if (fragment.empty()) return;
        // the document on screen scrolls now; another one once its first render is posted (ExecuteRender)
        if (alreadyShown) m_webViewViewer.ScrollToAnchor(fragment);
        else if (m_config.IsExtensionSupported(extension)) { m_pendingAnchorPath = full; m_pendingAnchor = fragment; }
    }
}

// The loading/error surface follows the editor's own colors and the monitor's DPI.
COLORREF NppMarkdownNext::StatusColor(bool background) const {
    if (m_nppData._nppHandle) {
        const auto back = static_cast<COLORREF>(SendMessage(m_nppData._nppHandle, NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, 0, 0));
        const auto fore = static_cast<COLORREF>(SendMessage(m_nppData._nppHandle, NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, 0, 0));
        // An older Notepad++ answers 0 to both: equal colors mean "unknown", not black on black.
        if (back != fore) return background ? back : fore;
    }
    const bool dark = IsDarkThemeEffective();
    return background ? (dark ? RGB(30, 30, 30) : RGB(255, 255, 255)) : (dark ? RGB(235, 235, 235) : RGB(25, 25, 25));
}

void NppMarkdownNext::UpdateStatusFont() {
    if (!m_hPanel || !m_hViewerStatus || !m_hRetryViewer) return;
    using GetDpiForWindowFn = UINT (WINAPI*)(HWND);
    using SystemParametersInfoForDpiFn = BOOL (WINAPI*)(UINT, UINT, PVOID, UINT, UINT);
    // Resolved at run time: both exist from Windows 10 1607, Notepad++ still loads plugins on older systems.
    static const auto getDpi = reinterpret_cast<GetDpiForWindowFn>(GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForWindow"));
    static const auto metricsForDpi = reinterpret_cast<SystemParametersInfoForDpiFn>(GetProcAddress(GetModuleHandleW(L"user32.dll"), "SystemParametersInfoForDpi"));
    UINT dpi = getDpi ? getDpi(m_hPanel) : 96;
    if (!dpi) dpi = 96;
    if (m_statusFont && dpi == m_statusFontDpi) return;
    NONCLIENTMETRICSW metrics = {};
    metrics.cbSize = sizeof(metrics);
    bool haveMetrics = metricsForDpi ? metricsForDpi(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0, dpi) != FALSE
                                     : SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0) != FALSE;
    HFONT font = haveMetrics ? CreateFontIndirectW(&metrics.lfMessageFont) : nullptr;
    if (!font) return;
    SendMessage(m_hViewerStatus, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
    SendMessage(m_hRetryViewer, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
    if (m_statusFont) DeleteObject(m_statusFont);
    m_statusFont = font;
    m_statusFontDpi = dpi;
}

void NppMarkdownNext::HandleCheckboxToggle(const CheckboxRequest& request) {
    if (m_shuttingDown || !m_isNppReady) return;
    auto reject = [this]() { RequestRenderDebounced(true); };
    HWND hSci = GetCurrentScintilla();
    if (hSci && SendMessage(hSci, SCI_GETREADONLY, 0, 0)) {
        m_webViewViewer.PostNotice(Tr("task.readOnly"));
        reject();
        return;
    }
    // A stale preview simply re-renders: the user sees the current state and can click again.
    if (!hSci || !m_webViewViewer.IsCurrentRender(request) || request.bufferId != std::to_wstring(GetCurrentBufferId()) ||
        m_sourceRevision != m_lastRenderedSourceRevision || GetScintillaText(hSci) != m_lastRawContentUtf8) {
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
    // The marker is replaced by text of the same length, so the user's selection keeps its offsets.
    const auto anchor = SendMessage(hSci, SCI_GETANCHOR, 0, 0), caret = SendMessage(hSci, SCI_GETCURRENTPOS, 0, 0);
    SendMessage(hSci, SCI_SETSEL, lineStart + request.markerColumn, lineStart + request.markerColumn + 3);
    SendMessage(hSci, SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(request.expectedState == ' ' ? "[x]" : "[ ]"));
    SendMessage(hSci, SCI_SETSEL, anchor, caret);
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
    if (m_overlayActive) ScheduleOverlaySync();
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
    if (bufferId == m_activeBufferId) CopyDocumentState(m_activeBufferPath, path); // renamed or saved under a new name
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
            if (self->m_suppressDockNotification) return 0;
            self->m_isPanelVisible = (wParam != FALSE);
            int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
            if (cmdId > 0) {
                SendMessage(self->m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(self->m_isPanelVisible ? TRUE : FALSE));
            }
            if (self->m_isPanelVisible) {
                if (!self->m_overlayActive && self->m_hViewerHost) {
                    RECT rc;
                    GetClientRect(hwnd, &rc);
                    SetWindowPos(self->m_hViewerHost, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
                    self->m_webViewViewer.Resize(0, 0, rc.right - rc.left, rc.bottom - rc.top);
                }
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
                if (self->m_suppressDockNotification) return 0;
                self->m_isPanelVisible = false;
                self->m_config.isPanelVisible = false;
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
            if (wParam == IDT_VIEWER_LOADING) {
                KillTimer(hwnd, IDT_VIEWER_LOADING);
                // still the loading status (Retry disabled): an error shown meanwhile keeps its own text
                if (!self->m_shuttingDown && self->m_viewerStatusVisible && !self->m_webViewViewer.IsPageReady() &&
                    !IsWindowEnabled(self->m_hRetryViewer))
                    self->SetViewerStatus(Tr("status.slow"), true);
                return 0;
            }
            break;
        }

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            if (!self->m_overlayActive && self->m_hViewerHost && GetParent(self->m_hViewerHost) == hwnd) {
                SetWindowPos(self->m_hViewerHost, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
            }
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
                ExportContent content = std::exchange(self->m_queuedExportContent, {});
                self->m_exportQueued = false;
                if (operation == L"copyHtml") self->CopyRenderedHtml(requestId, content);
                else if (operation == L"saveAsHtml") self->SaveAsHtml(requestId, content);
            }
            return 0;

        case WM_APP_EXPORT_RESULT: {
            auto warnings = std::exchange(self->m_pendingExportWarnings, {});
            if (!self->m_shuttingDown) for (const auto& warning : warnings) self->ShowExportWarning(warning);
            return 0;
        }

        case WM_APP_HOST_COMMAND:
            if (!self->m_shuttingDown) {
                if (lParam == 1) self->TogglePanel();
                else if (lParam == 2) self->ToggleHideEditor();
                else if (wParam) SendMessage(self->m_nppData._nppHandle, NPPM_MENUCOMMAND, 0, static_cast<LPARAM>(wParam));
            }
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_RETRY_VIEWER && HIWORD(wParam) == BN_CLICKED) {
                self->HandleViewerRecovery(true);
                return 0;
            }
            break;

        case WM_CTLCOLORSTATIC: {
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetTextColor(dc, self->StatusColor(false));
            SetBkColor(dc, self->StatusColor(true));
            SetDCBrushColor(dc, self->StatusColor(true));
            return reinterpret_cast<LRESULT>(GetStockObject(DC_BRUSH));
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetDCBrushColor(dc, self->StatusColor(true));
            FillRect(dc, &rect, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
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

LRESULT CALLBACK NppMarkdownNext::OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            if (self->m_overlayActive && self->m_hViewerHost && GetParent(self->m_hViewerHost) == hwnd) {
                SetWindowPos(self->m_hViewerHost, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return 0;
        }
        case WM_APP_SYNC_OVERLAY:
            self->m_overlaySyncQueued = false;
            if (self->m_overlayActive && !self->m_shuttingDown) self->UpdateOverlayBounds();
            return 0;
        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetDCBrushColor(dc, self->StatusColor(true));
            FillRect(dc, &rect, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
            return TRUE;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK NppMarkdownNext::ViewerHostWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            self->m_webViewViewer.Resize(0, 0, width, height);
            self->LayoutViewerStatus();
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
            SetTextColor(dc, self->StatusColor(false));
            SetBkColor(dc, self->StatusColor(true));
            SetDCBrushColor(dc, self->StatusColor(true));
            return reinterpret_cast<LRESULT>(GetStockObject(DC_BRUSH));
        }
        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetDCBrushColor(dc, self->StatusColor(true));
            FillRect(dc, &rect, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
            return TRUE;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK NppMarkdownNext::HostSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR refData) {
    auto* self = reinterpret_cast<NppMarkdownNext*>(refData);
    if (msg == WM_NCDESTROY) {
        RemoveWindowSubclass(hwnd, HostSubclassProc, subclassId);
        return DefSubclassProc(hwnd, msg, wParam, lParam);
    }
    const LRESULT result = DefSubclassProc(hwnd, msg, wParam, lParam);
    if (self && !self->m_shuttingDown && self->m_overlayActive &&
        (msg == WM_SIZE || msg == WM_MOVE || msg == WM_WINDOWPOSCHANGED)) {
        self->ScheduleOverlaySync();
    }
    return result;
}
