#include "../include/NppMarkdownNext.h"
#include "../include/BiDiEngine.h"
#include "../include/HtmlExporter.h"
#include "../include/Resource.h"
#include "../include/NppLog.h"
#include <commctrl.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <algorithm>

namespace {
const wchar_t* s_panelClassName = L"NppMarkdownNextWindowClass";
const wchar_t* s_renderCanvasClassName = L"NppMarkdownCanvasWindowClass";
}

NppMarkdownNext& NppMarkdownNext::Instance() {
    static NppMarkdownNext s_inst;
    return s_inst;
}

void NppMarkdownNext::Init(HINSTANCE hInst, NppData nppData) {
    m_hInst = hInst;
    m_nppData = nppData;

    // Load config path from Notepad++ plugin config directory
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
    m_config.showOutline = false;

    // CRITICAL: We do NOT call CreatePanelWindow() or send NPPM_DMMREGASDCKDLG here!
    // Docking registration must only occur on NPPN_READY or on-demand, after Notepad++
    // has completed its main window creation, pluginsManager initialization, and getFuncsArray.
}

void NppMarkdownNext::OnNppReady() {
    m_isNppReady = true;

    // Synchronize initial menu check states using allocated command IDs
    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
    int cmdOutline = GetPluginCmdId(CMD_TOGGLE_OUTLINE);
    if (cmdOutline > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdOutline, (LPARAM)(m_config.showOutline ? TRUE : FALSE));
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));

    SyncThemeMenuItems();
    ApplyAutoShowPanel();
}

void NppMarkdownNext::SyncThemeMenuItems() {
    struct ThemeCmd { PreviewTheme theme; int cmdId; };
    ThemeCmd themeCmds[] = {
        { PreviewTheme::FollowNpp, CMD_THEME_FOLLOW_NPP },
        { PreviewTheme::Light, CMD_THEME_LIGHT },
        { PreviewTheme::Dark, CMD_THEME_DARK },
        { PreviewTheme::Sepia, CMD_THEME_SEPIA },
        { PreviewTheme::Monokai, CMD_THEME_MONOKAI },
        { PreviewTheme::Dracula, CMD_THEME_DRACULA },
        { PreviewTheme::SolarizedLight, CMD_THEME_SOLARIZED_LIGHT },
        { PreviewTheme::SolarizedDark, CMD_THEME_SOLARIZED_DARK },
        { PreviewTheme::Nord, CMD_THEME_NORD },
        { PreviewTheme::GitHubLight, CMD_THEME_GITHUB_LIGHT },
        { PreviewTheme::GitHubDark, CMD_THEME_GITHUB_DARK },
        { PreviewTheme::VSCodeDark, CMD_THEME_VSCODE_DARK },
    };
    for (auto& tc : themeCmds) {
        int cmdId = GetPluginCmdId(tc.cmdId);
        if (cmdId > 0) {
            SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId,
                        (LPARAM)(m_config.theme == tc.theme ? TRUE : FALSE));
        }
    }

    // Bearded: the plain entry means the classic variation, the Gold entry its own variation
    int cmdBearded = GetPluginCmdId(CMD_THEME_BEARDED);
    if (cmdBearded > 0) {
        bool checked = (m_config.theme == PreviewTheme::Bearded) && m_config.beardedVariant.empty();
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBearded, (LPARAM)(checked ? TRUE : FALSE));
    }
    int cmdBeardedGold = GetPluginCmdId(CMD_THEME_BEARDED_GOLD);
    if (cmdBeardedGold > 0) {
        bool checked = (m_config.theme == PreviewTheme::Bearded) && (m_config.beardedVariant == L"gold-d-raynh");
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBeardedGold, (LPARAM)(checked ? TRUE : FALSE));
    }
}

void NppMarkdownNext::EnsurePanelRegistered() {
    NppLog("EnsurePanelRegistered() called, registered=%d", (int)m_isPanelRegistered);
    if (m_isPanelRegistered) return;
    CreatePanelWindow();
    NppLog("EnsurePanelRegistered() finished, registered=%d", (int)m_isPanelRegistered);
}

void NppMarkdownNext::Cleanup() {
    m_config.Save(m_configPath);
    if (m_hPanel && IsWindow(m_hPanel)) {
        DestroyWindow(m_hPanel);
        m_hPanel = nullptr;
    }
}

bool NppMarkdownNext::CreatePanelWindow() {
    NppLog("CreatePanelWindow() start");
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
    NppLog("RegisterClassEx completed");

    m_hPanel = CreateWindowEx(
        WS_EX_CONTROLPARENT,
        s_panelClassName,
        L"Markdown Panel",
        WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        0, 0, 450, 700,
        m_nppData._nppHandle,
        nullptr,
        m_hInst,
        this
    );

    NppLog("CreateWindowEx completed, m_hPanel=0x%p", m_hPanel);
    if (!m_hPanel) return false;

    // Setup docking data and register with Notepad++ FIRST
    // Notepad++ reparents m_hPanel to IDC_CLIENT_TAB and sets WS_CHILD.
    // This must occur before Direct2D render targets or child controls are created.
    m_tbData.hClient = m_hPanel;
    m_tbData.pszName = L"Markdown Panel";
    m_tbData.dlgID = CMD_TOGGLE_PANEL;
    m_tbData.uMask = DWS_DF_CONT_RIGHT | DWS_USEOWNDARKMODE;
    m_tbData.hIconTab = nullptr;
    m_tbData.pszAddInfo = nullptr;
    m_tbData.rcFloat = { 0, 0, 0, 0 };
    m_tbData.iPrevCont = -1;
    m_tbData.pszModuleName = L"NppMarkdownNext.dll";

    NppLog("Sending NPPM_DMMREGASDCKDLG, nppHandle=0x%p, tbData.hClient=0x%p", m_nppData._nppHandle, m_tbData.hClient);
    SendMessage(m_nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&m_tbData);
    NppLog("NPPM_DMMREGASDCKDLG returned successfully");
    m_isPanelRegistered = true;

    // Dark mode check from Notepad++
    bool isDark = (EffectiveTheme() == PreviewTheme::Dark);

    // Try initializing modern WebView2 (Chromium Evergreen) first
    wchar_t tempPath[MAX_PATH] = { 0 };
    GetTempPathW(MAX_PATH, tempPath);
    std::wstring userDataDir = std::wstring(tempPath) + L"NppMarkdownNext_WebView2";

    // Fallbacks for the two silent startup failures:
    //  - page becomes ready after an early render attempt -> re-render once loaded
    //  - async WebView2 init fails -> switch to Direct2D instead of a blank panel
    m_webViewViewer.SetPageReadyCallback([this]() {
        // Push settings state (BiDi / dark mode / zoom %) into the freshly
        // loaded page so the settings popover shows correct values.
        m_webViewViewer.SyncUiState(m_config.isSmartBiDiEnabled, m_config.darkModeOverride,
                                    static_cast<int>(m_config.zoomLevel * 100.0f + 0.5f),
                                    m_config.autoShowPanel);
        if (m_openSettingsPending) {
            m_openSettingsPending = false;
            m_webViewViewer.ExecuteScript(L"if (typeof openSettingsPopover === 'function') openSettingsPopover();");
        }
        m_lastRawContent.clear();
        if (!m_isPanelVisible) return;
        NppLog("WebView2 page ready, re-rendering");
        m_renderRetryCount = 0;
        if (!ExecuteRender() && m_hPanel) {
            SetTimer(m_hPanel, IDT_RENDER_RETRY, IDT_RENDER_RETRY_DELAY_MS, nullptr);
        }
    });
    m_webViewViewer.SetInitFailedCallback([this]() {
        if (!m_useWebView2) return;
        NppLog("WebView2 async initialization failed, falling back to Direct2D");
        m_useWebView2 = false;
        m_renderer.Initialize(m_hPanel);
        m_renderer.SetZoom(m_config.zoomLevel);
        m_renderer.SetDarkMode(EffectiveTheme() == PreviewTheme::Dark);
        m_lastRawContent.clear();
        m_renderRetryCount = 0;
        if (!ExecuteRender() && m_hPanel) {
            SetTimer(m_hPanel, IDT_RENDER_RETRY, IDT_RENDER_RETRY_DELAY_MS, nullptr);
        }
    });
    // Startup race guard: whatever order the panel-show / WebView2-init / buffer
    // restore events arrive in, once the controller is usable we force a render.
    // Without this a render that ran too early (empty buffer, viewer not ready)
    // leaves a blank white panel until some later buffer event.
    m_webViewViewer.SetInitCompletedCallback([this]() {
        NppLog("WebView2 init completed, forceRender visible=%d", (int)m_isPanelVisible);
        if (!m_isPanelVisible) return;
        m_lastRawContent.clear();
        m_renderRetryCount = 0;
        if (!ExecuteRender() && m_hPanel) {
            SetTimer(m_hPanel, IDT_RENDER_RETRY, IDT_RENDER_RETRY_DELAY_MS, nullptr);
        }
    });

    m_useWebView2 = m_webViewViewer.Initialize(m_hPanel, userDataDir);
    NppLog("CreatePanelWindow: Initialize returned %d (useWebView2=%d)", (int)m_useWebView2, (int)m_useWebView2);
    if (m_useWebView2) {
        m_webViewViewer.SetTheme(ThemeClassName(EffectiveTheme()), CurrentThemeId());
        m_webViewViewer.SetZoom(m_config.zoomLevel);
        m_webViewViewer.SetCheckboxCallback([this](int lineIdx) {
            HWND hSci = GetCurrentScintilla();
            if (!hSci || lineIdx < 0) return;
            int lineStart = static_cast<int>(SendMessage(hSci, SCI_POSITIONFROMLINE, lineIdx, 0));
            int lineLen = static_cast<int>(SendMessage(hSci, SCI_LINELENGTH, lineIdx, 0));
            if (lineLen > 0) {
                std::string lineText(lineLen + 1, '\0');
                SendMessage(hSci, SCI_GETLINE, lineIdx, (LPARAM)&lineText[0]);
                lineText.resize(lineLen);

                size_t uncheckedPos = lineText.find("[ ]");
                if (uncheckedPos != std::string::npos) {
                    SendMessage(hSci, SCI_SETSEL, lineStart + static_cast<int>(uncheckedPos), lineStart + static_cast<int>(uncheckedPos) + 3);
                    SendMessage(hSci, SCI_REPLACESEL, 0, (LPARAM)"[x]");
                } else {
                    size_t checkedPos = lineText.find("[x]");
                    if (checkedPos == std::string::npos) checkedPos = lineText.find("[X]");
                    if (checkedPos != std::string::npos) {
                        SendMessage(hSci, SCI_SETSEL, lineStart + static_cast<int>(checkedPos), lineStart + static_cast<int>(checkedPos) + 3);
                        SendMessage(hSci, SCI_REPLACESEL, 0, (LPARAM)"[ ]");
                    }
                }
            }
        });
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
            SelectTheme(EffectiveTheme() == PreviewTheme::Dark ? PreviewTheme::Light : PreviewTheme::Dark);
        });
        m_webViewViewer.SetSelectThemeCallback([this](const std::wstring& themeName) {
            PreviewTheme theme = PreviewTheme::FollowNpp;
            std::wstring beardedVariant;
            const std::wstring beardedPrefix = L"bearded-v-";
            if (themeName == L"light") theme = PreviewTheme::Light;
            else if (themeName == L"dark") theme = PreviewTheme::Dark;
            else if (themeName == L"sepia") theme = PreviewTheme::Sepia;
            else if (themeName == L"monokai") theme = PreviewTheme::Monokai;
            else if (themeName == L"dracula") theme = PreviewTheme::Dracula;
            else if (themeName == L"solarized-light") theme = PreviewTheme::SolarizedLight;
            else if (themeName == L"solarized-dark") theme = PreviewTheme::SolarizedDark;
            else if (themeName == L"nord") theme = PreviewTheme::Nord;
            else if (themeName == L"github-light") theme = PreviewTheme::GitHubLight;
            else if (themeName == L"github-dark") theme = PreviewTheme::GitHubDark;
            else if (themeName == L"vscode-dark") theme = PreviewTheme::VSCodeDark;
            else if (themeName.rfind(beardedPrefix, 0) == 0) {
                theme = PreviewTheme::Bearded;
                beardedVariant = themeName.substr(beardedPrefix.length());
            }
            else if (themeName == L"bearded") theme = PreviewTheme::Bearded;
            SelectTheme(theme, beardedVariant);
        });
        m_webViewViewer.SetTocCallback([this](bool isOpen) {
            m_config.showOutline = isOpen;
            m_config.Save(m_configPath);
            int cmdOutline = GetPluginCmdId(CMD_TOGGLE_OUTLINE);
            if (cmdOutline > 0) {
                SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdOutline, (LPARAM)(isOpen ? TRUE : FALSE));
            }
        });
        m_webViewViewer.SetSaveAsHtmlCallback([this]() {
            SaveAsHtml();
        });
        m_webViewViewer.SetBiDiCallback([this]() {
            ToggleBiDi();
        });
        m_webViewViewer.SetAutoShowCallback([this]() {
            ToggleAutoShowPanel();
        });
        m_webViewViewer.SetCopyHtmlCallback([this]() {
            CopyRenderedHtml();
        });
        m_webViewViewer.SetDarkModeCallback([this](int mode) {
            if (mode < -1 || mode > 1) mode = -1;
            if (mode == m_config.darkModeOverride) return;
            m_config.darkModeOverride = mode;
            m_config.Save(m_configPath);
            SelectTheme(m_config.theme, m_config.beardedVariant);
        });
        m_webViewViewer.SetZoomInCallback([this]() {
            ZoomIn();
        });
        m_webViewViewer.SetZoomOutCallback([this]() {
            ZoomOut();
        });
        m_webViewViewer.SetZoomResetCallback([this]() {
            ZoomReset();
        });
        m_webViewViewer.SetZoomChangeCallback([this](float factor) {
            m_config.zoomLevel = factor;
            if (m_config.zoomLevel < 0.3f) m_config.zoomLevel = 0.3f;
            if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
            m_config.Save(m_configPath);
            m_renderer.SetZoom(m_config.zoomLevel);
            // Native Ctrl+wheel zoom never goes through SetZoom(), so echo the
            // state here or the popover's zoom label stays stale.
            m_webViewViewer.SyncUiState(m_config.isSmartBiDiEnabled, m_config.darkModeOverride,
                                        static_cast<int>(m_config.zoomLevel * 100.0f + 0.5f),
                                        m_config.autoShowPanel);
        });
        NppLog("Modern WebView2 engine initialized successfully");
    } else {
        // Fallback to Direct2D Renderer
        NppLog("Initializing Direct2D Fallback Renderer");
        m_renderer.Initialize(m_hPanel);
        m_renderer.SetZoom(m_config.zoomLevel);
        m_renderer.SetDarkMode(isDark);
        NppLog("Direct2D Fallback Renderer initialized");
    }

    return true;
}

HWND NppMarkdownNext::GetCurrentScintilla() const {
    int which = -1;
    SendMessage(m_nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    return (which == 0) ? m_nppData._scintillaMainHandle : m_nppData._scintillaSecondHandle;
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

int NppMarkdownNext::GetScintillaFirstVisibleLine(HWND hSci) const {
    if (!hSci) return 0;
    return static_cast<int>(SendMessage(hSci, SCI_GETFIRSTVISIBLELINE, 0, 0));
}

std::wstring NppMarkdownNext::GetCurrentBufferPath() const {
    wchar_t path[MAX_PATH] = { 0 };
    // NPPM_GETFULLPATHFROMBUFFERID needs a real buffer id; -1 returned "" and
    // made every tab look like an unsaved (Markdown) one
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

void NppMarkdownNext::OpenSettings() {
    EnsurePanelRegistered();
    if (!m_isPanelVisible) {
        m_userClosedPanel = false;
        SetPanelVisible(true);
    }
    if (!m_useWebView2) return;
    if (m_webViewViewer.IsPageReady()) {
        m_webViewViewer.ExecuteScript(L"if (typeof openSettingsPopover === 'function') openSettingsPopover();");
    } else {
        m_openSettingsPending = true;
    }
}

void NppMarkdownNext::TogglePanel() {
    NppLog("TogglePanel() enter, current isVisible=%d", (int)m_isPanelVisible);
    bool show = !m_isPanelRegistered || !m_isPanelVisible;
    m_userClosedPanel = !show;
    SetPanelVisible(show);
    NppLog("TogglePanel() exit");
}

void NppMarkdownNext::SetPanelVisible(bool visible) {
    // Registering a docking dialog shows it, so never register just to hide
    if (!visible && !m_isPanelRegistered) return;
    EnsurePanelRegistered();
    m_isPanelVisible = visible;
    SendMessage(m_nppData._nppHandle, visible ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, (LPARAM)m_hPanel);

    int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
    if (cmdId > 0) {
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(visible ? TRUE : FALSE));
    }

    if (visible) {
        m_lastRawContent.clear();
        ExecuteRender();
    }
}

void NppMarkdownNext::ApplyAutoShowPanel() {
    if (!m_config.autoShowPanel) return;
    // Unsaved tabs ("new 1") have no extension: keep the current panel state
    if (GetCurrentBufferPath().find(L'\\') == std::wstring::npos) return;
    bool isMarkdown = IsCurrentBufferMarkdown();
    if (isMarkdown == m_isPanelVisible) return;
    if (isMarkdown && m_userClosedPanel) return;
    NppLog("ApplyAutoShowPanel: %s", isMarkdown ? "show" : "hide");
    SetPanelVisible(isMarkdown);
}

void NppMarkdownNext::ToggleAutoShowPanel() {
    m_config.autoShowPanel = !m_config.autoShowPanel;
    m_config.Save(m_configPath);
}

void NppMarkdownNext::ToggleSyncWithCaret() {
    m_config.syncWithCaret = !m_config.syncWithCaret;
    m_config.Save(m_configPath);
    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
}

void NppMarkdownNext::ToggleOutline() {
    m_config.showOutline = !m_config.showOutline;
    m_config.Save(m_configPath);
    if (m_useWebView2) {
        if (m_config.showOutline) {
            m_webViewViewer.ExecuteScript(L"if (typeof openToc === 'function') openToc();");
        } else {
            m_webViewViewer.ExecuteScript(L"if (typeof closeToc === 'function') closeToc();");
        }
    }
    int cmdOutline = GetPluginCmdId(CMD_TOGGLE_OUTLINE);
    if (cmdOutline > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdOutline, (LPARAM)(m_config.showOutline ? TRUE : FALSE));

    // Trigger window resize to arrange canvas and outline
    if (m_hPanel) {
        RECT rc;
        GetClientRect(m_hPanel, &rc);
        SendMessage(m_hPanel, WM_SIZE, 0, MAKELPARAM(rc.right - rc.left, rc.bottom - rc.top));
    }
}

void NppMarkdownNext::CopyRenderedHtml() {
    std::wstring title = L"Markdown Preview";
    HtmlExporter::CopyToClipboard(m_hPanel, m_currentDoc, title, m_config.theme, m_config.beardedVariant);
}

void NppMarkdownNext::SaveAsHtml() {
    wchar_t fileName[MAX_PATH] = L"export.html";
    OPENFILENAMEW ofn = { sizeof(OPENFILENAMEW) };
    ofn.hwndOwner = m_hPanel;
    ofn.lpstrFilter = L"HTML Files (*.html;*.htm)\0*.html;*.htm\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = L"html";

    if (GetSaveFileNameW(&ofn)) {
        HtmlExporter::SaveToFile(fileName, m_currentDoc, L"Markdown Export", m_config.theme, m_config.beardedVariant);
    }
}

void NppMarkdownNext::ZoomIn() {
    m_config.zoomLevel += 0.1f;
    if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
    m_config.Save(m_configPath);
    if (m_useWebView2) {
        m_webViewViewer.SetZoom(m_config.zoomLevel);
    } else {
        m_renderer.SetZoom(m_config.zoomLevel);
        if (m_hPanel) InvalidateRect(m_hPanel, nullptr, FALSE);
    }
}

void NppMarkdownNext::ZoomOut() {
    m_config.zoomLevel -= 0.1f;
    if (m_config.zoomLevel < 0.4f) m_config.zoomLevel = 0.4f;
    m_config.Save(m_configPath);
    if (m_useWebView2) {
        m_webViewViewer.SetZoom(m_config.zoomLevel);
    } else {
        m_renderer.SetZoom(m_config.zoomLevel);
        if (m_hPanel) InvalidateRect(m_hPanel, nullptr, FALSE);
    }
}

void NppMarkdownNext::ZoomReset() {
    m_config.zoomLevel = 1.0f;
    m_config.Save(m_configPath);
    if (m_useWebView2) {
        m_webViewViewer.SetZoom(m_config.zoomLevel);
    } else {
        m_renderer.SetZoom(m_config.zoomLevel);
        if (m_hPanel) InvalidateRect(m_hPanel, nullptr, FALSE);
    }
}

void NppMarkdownNext::ToggleBiDi() {
    m_config.isSmartBiDiEnabled = !m_config.isSmartBiDiEnabled;
    m_config.Save(m_configPath);
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));
    ExecuteRender();
}

PreviewTheme NppMarkdownNext::EffectiveTheme() const {
    if (m_config.theme != PreviewTheme::FollowNpp) return m_config.theme;
    bool isNppDark = SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
    bool isDark = (m_config.darkModeOverride == 1) || (m_config.darkModeOverride == -1 && isNppDark);
    return isDark ? PreviewTheme::Dark : PreviewTheme::Light;
}

std::wstring NppMarkdownNext::ThemeClassName(PreviewTheme theme) const {
    switch (theme) {
        case PreviewTheme::Dark: return L"dark";
        case PreviewTheme::Sepia: return L"sepia";
        case PreviewTheme::Monokai: return L"monokai";
        case PreviewTheme::Dracula: return L"dracula";
        case PreviewTheme::SolarizedLight: return L"solarized-light";
        case PreviewTheme::SolarizedDark: return L"solarized-dark";
        case PreviewTheme::Nord: return L"nord";
        case PreviewTheme::GitHubLight: return L"github-light";
        case PreviewTheme::GitHubDark: return L"github-dark";
        case PreviewTheme::VSCodeDark: return L"vscode-dark";
        case PreviewTheme::Bearded:
            if (m_config.beardedVariant.empty()) return L"bearded";
            return L"bearded-v-" + m_config.beardedVariant;
        default: return L"";
    }
}

std::wstring NppMarkdownNext::CurrentThemeId() const {
    switch (m_config.theme) {
        case PreviewTheme::FollowNpp: return L"follow";
        case PreviewTheme::Light: return L"light";
        case PreviewTheme::Dark: return L"dark";
        case PreviewTheme::Sepia: return L"sepia";
        case PreviewTheme::Monokai: return L"monokai";
        case PreviewTheme::Dracula: return L"dracula";
        case PreviewTheme::SolarizedLight: return L"solarized-light";
        case PreviewTheme::SolarizedDark: return L"solarized-dark";
        case PreviewTheme::Nord: return L"nord";
        case PreviewTheme::GitHubLight: return L"github-light";
        case PreviewTheme::GitHubDark: return L"github-dark";
        case PreviewTheme::VSCodeDark: return L"vscode-dark";
        case PreviewTheme::Bearded:
            if (m_config.beardedVariant.empty()) return L"bearded";
            return L"bearded-v-" + m_config.beardedVariant;
        default: return L"";
    }
}

void NppMarkdownNext::SelectTheme(PreviewTheme theme, const std::wstring& beardedVariant) {
    m_config.theme = theme;
    // Variant ids are generated as [a-z0-9-]; drop anything else from config/JS input
    std::wstring variant;
    if (theme == PreviewTheme::Bearded) {
        for (wchar_t ch : beardedVariant) {
            if ((ch >= L'a' && ch <= L'z') || (ch >= L'0' && ch <= L'9') || ch == L'-') variant.push_back(ch);
        }
    }
    m_config.beardedVariant = variant;
    NppLog("SelectTheme: theme=%d variant=%s", static_cast<int>(theme),
           BiDiEngine::WideToUtf8(variant).c_str());
    m_config.Save(m_configPath);
    SyncThemeMenuItems();

    PreviewTheme effective = EffectiveTheme();
    bool isDark = (effective == PreviewTheme::Dark);

    if (m_useWebView2) {
        m_webViewViewer.SetTheme(ThemeClassName(effective), CurrentThemeId());
    }

    m_renderer.SetDarkMode(isDark);
    if (m_hPanel) {
        InvalidateRect(m_hPanel, nullptr, TRUE);
    }

    ExecuteRender();
}

void NppMarkdownNext::ShowAbout() {
    const wchar_t* aboutMsg =
        L"NppMarkdownNext (Native C++ Modern Edition)\n"
        L"Version 1.0.4 (x64 / x86 / ARM64)\n\n"
        L"Next-generation native Markdown live preview panel for Notepad++.\n\n"
        L"Features:\n"
        L"  • Dual Engine: Chromium WebView2 + Direct2D Fallback\n"
        L"  • Material Design 3 Minimal Typography & Theming\n"
        L"  • Minimal Vector SVG Context Menu & Overlays\n"
        L"  • Intelligent RTL / BiDi & Persian/Arabic Typography\n"
        L"  • GFM Tables, Task Lists, Syntax Highlighting & 1-Click Copy\n"
        L"  • LaTeX / KaTeX Math & Offline Mermaid Diagrams\n"
        L"  • Live Caret & Scrollbar Synchronization\n"
        L"  • Responsive Table of Contents & In-Page Search\n"
        L"  • Standalone HTML Export & PDF Printing\n\n"
        L"Author: theneet0\n"
        L"Licensed under the MIT License";

    MessageBox(m_nppData._nppHandle, aboutMsg, L"About Markdown Next", MB_OK | MB_ICONINFORMATION);
}

void NppMarkdownNext::RequestRenderDebounced() {
    if (!m_isPanelVisible) return;
    if (!IsCurrentBufferMarkdown()) return;
    SetTimer(m_hPanel, IDT_RENDER_DEBOUNCE, RENDER_DEBOUNCE_DELAY_MS, nullptr);
}

bool NppMarkdownNext::ExecuteRender() {
    HWND hSci = GetCurrentScintilla();
    NppLog("ExecuteRender: hSci=%p visible=%d wv2=%d ready=%d",
           (void*)hSci, (int)m_isPanelVisible, (int)m_useWebView2,
           (int)m_webViewViewer.IsPageReady());
    if (!hSci) return false;

    std::string utf8Content = GetScintillaText(hSci);
    std::wstring wideContent = BiDiEngine::Utf8ToWide(utf8Content);
    NppLog("ExecuteRender: contentBytes=%u cacheWide=%u",
           (unsigned)utf8Content.size(), (unsigned)m_lastRawContent.size());

    if (wideContent == m_lastRawContent) {
        NppLog("ExecuteRender: cache hit, caret-sync only");
        // Only sync caret position if text didn't change
        if (m_config.syncWithCaret) {
            int line = GetScintillaCaretLine(hSci);
            if (m_useWebView2) {
                m_webViewViewer.ScrollToLine(line);
            } else {
                m_renderer.ScrollToSourceLine(line);
            }
        }
        return true;
    }

    m_lastRawContent = wideContent;

    // Parse Document
    m_currentDoc = MarkdownParser::Parse(wideContent);

    std::wstring currentPath = GetCurrentBufferPath();
    std::wstring docTitle = PathFindFileNameW(currentPath.c_str());
    if (docTitle.empty()) docTitle = L"Markdown Preview";

    PreviewTheme effective = EffectiveTheme();

    if (m_useWebView2) {
        auto components = HtmlExporter::GeneratePreviewComponents(
            m_currentDoc,
            docTitle,
            effective,
            m_config.zoomLevel,
            m_config.syncWithCaret,
            m_config.darkModeOverride,
            m_config.beardedVariant
        );

        if (m_webViewViewer.IsPageReady()) {
            std::string titleUtf8 = BiDiEngine::WideToUtf8(docTitle);
            if (!m_webViewViewer.UpdateContent(components.bodyHtml, components.tocHtml, titleUtf8,
                                               ThemeClassName(effective))) {
                NppLog("ExecuteRender: UpdateContent failed, full navigation");
                m_webViewViewer.SetHtmlContent(components.fullHtml);
            }
        } else {
            NppLog("ExecuteRender: page not ready, full SetHtmlContent len=%u",
                   (unsigned)components.fullHtml.size());
            m_webViewViewer.SetHtmlContent(components.fullHtml);
        }

        if (m_config.syncWithCaret) {
            int line = GetScintillaCaretLine(hSci);
            m_webViewViewer.ScrollToLine(line);
        }
    } else {
        // Layout in Renderer
        RECT rc;
        GetClientRect(m_hPanel, &rc);
        float clientW = static_cast<float>(rc.right - rc.left);
        m_renderer.SetDocument(m_currentDoc);
        m_renderer.Layout(clientW);

        // Sync scroll with current position
        if (m_config.syncWithCaret) {
            int line = GetScintillaCaretLine(hSci);
            m_renderer.ScrollToSourceLine(line);
        }

        InvalidateRect(m_hPanel, nullptr, FALSE);
    }

    return true;
}

void NppMarkdownNext::OnNotification(SCNotification* notifyCode) {
    if (!notifyCode || !m_isNppReady) return;

    if (notifyCode->nmhdr.code == SCN_MODIFIED) {
        if (notifyCode->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT)) {
            RequestRenderDebounced();
        }
    } else if (notifyCode->nmhdr.code == SCN_UPDATEUI) {
        if (m_isPanelVisible && m_config.syncWithCaret) {
            HWND hSci = GetCurrentScintilla();
            if (hSci) {
                int line = GetScintillaCaretLine(hSci);
                if (line != m_lastScrolledCaretLine) {
                    m_lastScrolledCaretLine = line;
                    if (m_useWebView2) {
                        m_webViewViewer.ScrollToLine(line);
                    } else {
                        m_renderer.ScrollToSourceLine(line);
                    }
                }
            }
        }
    }
}

void NppMarkdownNext::OnDarkModeChanged() {
    if (!m_isNppReady) return;
    PreviewTheme effective = EffectiveTheme();
    bool isDark = (effective == PreviewTheme::Dark);
    if (m_useWebView2) {
        m_webViewViewer.SetTheme(ThemeClassName(effective), CurrentThemeId());
    }
    m_renderer.SetDarkMode(isDark);
    if (m_hPanel) {
        InvalidateRect(m_hPanel, nullptr, TRUE);
    }
}

void NppMarkdownNext::OnBufferActivated() {
    if (!m_isNppReady) return;
    NppLog("OnBufferActivated: visible=%d", (int)m_isPanelVisible);

    // A panel that auto-show just opened has already rendered
    bool wasVisible = m_isPanelVisible;
    ApplyAutoShowPanel();
    if (wasVisible && m_isPanelVisible) {
        m_lastRawContent.clear();
        m_lastScrolledCaretLine = -1;
        ExecuteRender();
    }
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
            NppLog("PanelWndProc WM_SHOWWINDOW wp=%d", (int)wParam);
            self->m_isPanelVisible = (wParam != FALSE);
            int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
            if (cmdId > 0) {
                SendMessage(self->m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(self->m_isPanelVisible ? TRUE : FALSE));
            }
            if (self->m_isPanelVisible) {
                self->m_lastRawContent.clear();
                self->m_renderRetryCount = 0;
                // Startup race: Scintilla may not be reachable yet - retry on a timer
                bool ok = self->ExecuteRender();
                NppLog("WM_SHOWWINDOW(TRUE) render=%d", (int)ok);
                if (!ok) {
                    SetTimer(hwnd, IDT_RENDER_RETRY, IDT_RENDER_RETRY_DELAY_MS, nullptr);
                }
            }
            return 0;
        }

        case WM_NOTIFY: {
            NMHDR* pnm = reinterpret_cast<NMHDR*>(lParam);
            if (pnm && pnm->code == DMN_CLOSE) {
                self->m_isPanelVisible = false;
                self->m_userClosedPanel = true;
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
                self->ExecuteRender();
                return 0;
            }
            if (wParam == IDT_RENDER_RETRY) {
                KillTimer(hwnd, IDT_RENDER_RETRY);
                bool ok = self->ExecuteRender();
                NppLog("IDT_RENDER_RETRY try=%d ok=%d", (int)self->m_renderRetryCount, (int)ok);
                if (!ok && ++self->m_renderRetryCount < 100) {
                    SetTimer(hwnd, IDT_RENDER_RETRY, IDT_RENDER_RETRY_DELAY_MS, nullptr);
                } else {
                    self->m_renderRetryCount = 0;
                }
                return 0;
            }
            break;
        }

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            if (self->m_useWebView2) {
                self->m_webViewViewer.Resize(0, 0, width, height);
            } else {
                self->m_renderer.Resize(width, height);
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (!self->m_useWebView2) {
                self->m_renderer.Render();
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_MOUSEWHEEL: {
            if (!self->m_useWebView2) {
                int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                bool isCtrl = (LOWORD(wParam) & MK_CONTROL) != 0;
                if (isCtrl) {
                    if (delta > 0) self->ZoomIn();
                    else self->ZoomOut();
                } else {
                    self->m_renderer.ScrollBy(-delta / 2);
                }
            }
            return 0;
        }

        case WM_MOUSEMOVE: {
            if (!self->m_useWebView2) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                if (y >= 0 && self->m_renderer.OnMouseMove(x, y)) {
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            if (!self->m_useWebView2) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                if (y >= 0) {
                    HWND hSci = self->GetCurrentScintilla();
                    if (self->m_renderer.OnLButtonDown(x, y, hSci)) {
                        InvalidateRect(hwnd, nullptr, FALSE);
                    }
                }
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1; // Prevent flicker
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
