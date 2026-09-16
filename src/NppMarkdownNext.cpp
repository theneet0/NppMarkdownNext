#include "../include/NppMarkdownNext.h"
#include "../include/BiDiEngine.h"
#include "../include/HtmlExporter.h"
#include "../include/Resource.h"
#include <commctrl.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <algorithm>

namespace {
const wchar_t* s_panelClassName = L"NppMarkdownNextWindowClass";
const wchar_t* s_renderCanvasClassName = L"NppMarkdownCanvasWindowClass";

#ifdef _DEBUG
void NppLog(const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
}
#else
inline void NppLog(const char*, ...) {}
#endif
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
    bool isNppDark = SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
    bool isDark = (m_config.darkModeOverride == 1) || (m_config.darkModeOverride == -1 && isNppDark);

    // Try initializing modern WebView2 (Chromium Evergreen) first
    wchar_t tempPath[MAX_PATH] = { 0 };
    GetTempPathW(MAX_PATH, tempPath);
    std::wstring userDataDir = std::wstring(tempPath) + L"NppMarkdownNext_WebView2";

    m_useWebView2 = m_webViewViewer.Initialize(m_hPanel, userDataDir);
    if (m_useWebView2) {
        m_webViewViewer.SetDarkMode(isDark);
        m_webViewViewer.SetZoom(m_config.zoomLevel);
        m_webViewViewer.SetCheckboxCallback([this](int lineNo) {
            HWND hSci = GetCurrentScintilla();
            if (!hSci || lineNo < 1) return;
            int lineStart = static_cast<int>(SendMessage(hSci, SCI_POSITIONFROMLINE, lineNo - 1, 0));
            int lineLen = static_cast<int>(SendMessage(hSci, SCI_LINELENGTH, lineNo - 1, 0));
            if (lineLen > 0) {
                std::string lineText(lineLen + 1, '\0');
                SendMessage(hSci, SCI_GETLINE, lineNo - 1, (LPARAM)&lineText[0]);
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
        m_webViewViewer.SetSyncCallback([this]() {
            ToggleSyncWithCaret();
        });
        m_webViewViewer.SetThemeCallback([this]() {
            bool isNppDark = SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
            bool curDark = (m_config.darkModeOverride == 1) || (m_config.darkModeOverride == -1 && isNppDark);
            m_config.darkModeOverride = curDark ? 0 : 1;
            OnDarkModeChanged();
        });
        m_webViewViewer.SetTocCallback([this](bool isOpen) {
            m_config.showOutline = isOpen;
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
            m_renderer.SetZoom(m_config.zoomLevel);
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
    SendMessage(m_nppData._nppHandle, NPPM_GETFULLPATHFROMBUFFERID, (WPARAM)-1, (LPARAM)path);
    return path;
}

bool NppMarkdownNext::IsCurrentBufferMarkdown() const {
    std::wstring path = GetCurrentBufferPath();
    if (path.empty()) return true; // New empty file

    size_t dotPos = path.find_last_of(L'.');
    if (dotPos == std::wstring::npos) return false;
    std::wstring ext = path.substr(dotPos);
    return m_config.IsExtensionSupported(ext);
}

void NppMarkdownNext::TogglePanel() {
    NppLog("TogglePanel() enter, current isVisible=%d", (int)m_isPanelVisible);
    bool wasRegistered = m_isPanelRegistered;
    EnsurePanelRegistered();
    NppLog("TogglePanel() after EnsurePanelRegistered, m_hPanel=0x%p", m_hPanel);

    if (!wasRegistered) {
        m_isPanelVisible = true;
    } else {
        m_isPanelVisible = !m_isPanelVisible;
    }

    NppLog("TogglePanel() sending NPPM_DMMSHOW/HIDE: %s", m_isPanelVisible ? "NPPM_DMMSHOW" : "NPPM_DMMHIDE");
    SendMessage(m_nppData._nppHandle, m_isPanelVisible ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, (LPARAM)m_hPanel);
    NppLog("TogglePanel() NPPM_DMMSHOW/HIDE returned");

    int cmdId = GetPluginCmdId(CMD_TOGGLE_PANEL);
    if (cmdId > 0) {
        SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdId, (LPARAM)(m_isPanelVisible ? TRUE : FALSE));
    }

    if (m_isPanelVisible) {
        NppLog("TogglePanel() calling ExecuteRender");
        m_lastRawContent.clear();
        ExecuteRender();
        NppLog("TogglePanel() ExecuteRender returned");
    }
    NppLog("TogglePanel() exit");
}

void NppMarkdownNext::ToggleSyncWithCaret() {
    m_config.syncWithCaret = !m_config.syncWithCaret;
    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
}

void NppMarkdownNext::ToggleOutline() {
    m_config.showOutline = !m_config.showOutline;
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
    HtmlExporter::CopyToClipboard(m_hPanel, m_currentDoc, title, m_renderer.IsDarkMode());
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
        HtmlExporter::SaveToFile(fileName, m_currentDoc, L"Markdown Export", m_renderer.IsDarkMode());
    }
}

void NppMarkdownNext::ZoomIn() {
    m_config.zoomLevel += 0.1f;
    if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
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
    if (m_useWebView2) {
        m_webViewViewer.SetZoom(m_config.zoomLevel);
    } else {
        m_renderer.SetZoom(m_config.zoomLevel);
        if (m_hPanel) InvalidateRect(m_hPanel, nullptr, FALSE);
    }
}

void NppMarkdownNext::ZoomReset() {
    m_config.zoomLevel = 1.0f;
    if (m_useWebView2) {
        m_webViewViewer.SetZoom(m_config.zoomLevel);
    } else {
        m_renderer.SetZoom(m_config.zoomLevel);
        if (m_hPanel) InvalidateRect(m_hPanel, nullptr, FALSE);
    }
}

void NppMarkdownNext::ToggleBiDi() {
    m_config.isSmartBiDiEnabled = !m_config.isSmartBiDiEnabled;
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));
    ExecuteRender();
}

void NppMarkdownNext::ShowAbout() {
    const wchar_t* aboutMsg =
        L"NppMarkdownNext - Modern Native Edition (2026)\n"
        L"Version 1.0.1 (x64 / x86 Pure C++26)\n\n"
        L"Ultra-lightweight Notepad++ Markdown preview panel.\n"
        L"Engine: Direct2D 1.1 + DirectWrite GPU Hardware Accelerated\n"
        L"Features:\n"
        L"  • Zero .NET / Zero Chromium Child Processes (RAM < 8 MB)\n"
        L"  • First-class Persian / RTL BiDi & Inline Code Isolation\n"
        L"  • Persian Numerals in Ordered Lists & Vazirmatn Typography\n"
        L"  • GitHub & Persian Alert Callouts ([!NOTE], [!نکته], etc.)\n"
        L"  • GFM Tables, Task Lists, Syntax Highlighting & Copy Button\n"
        L"  • Live Caret & Scrollbar Synchronization\n"
        L"  • Document Map / Outline (TOC) Navigation\n"
        L"  • Self-Contained HTML Export & Clipboard Copy\n"
        L"  • Native Dark / Light Mode Synchronization\n\n"
        L"Original concept by theneet0 & mohzy83\n"
        L"Modernized into C++26 by Google DeepMind Antigravity.";

    MessageBox(m_nppData._nppHandle, aboutMsg, L"About Markdown Panel", MB_OK | MB_ICONINFORMATION);
}

void NppMarkdownNext::RequestRenderDebounced() {
    if (!m_isPanelVisible) return;
    SetTimer(m_hPanel, IDT_RENDER_DEBOUNCE, RENDER_DEBOUNCE_DELAY_MS, nullptr);
}

void NppMarkdownNext::ExecuteRender() {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return;

    std::string utf8Content = GetScintillaText(hSci);
    std::wstring wideContent = BiDiEngine::Utf8ToWide(utf8Content);

    if (wideContent == m_lastRawContent) {
        // Only sync caret position if text didn't change
        if (m_config.syncWithCaret) {
            int line = GetScintillaCaretLine(hSci);
            if (m_useWebView2) {
                m_webViewViewer.ScrollToLine(line + 1);
            } else {
                m_renderer.ScrollToSourceLine(line);
            }
        }
        return;
    }

    m_lastRawContent = wideContent;

    // Parse Document
    m_currentDoc = MarkdownParser::Parse(wideContent);

    std::wstring currentPath = GetCurrentBufferPath();
    std::wstring docTitle = PathFindFileNameW(currentPath.c_str());
    if (docTitle.empty()) docTitle = L"Markdown Preview";

    bool isDark = (m_config.darkModeOverride == 1) ||
                  (m_config.darkModeOverride == -1 && SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0);

    if (m_useWebView2) {
        auto components = HtmlExporter::GeneratePreviewComponents(
            m_currentDoc,
            docTitle,
            isDark,
            m_config.zoomLevel,
            m_config.syncWithCaret
        );

        if (m_webViewViewer.IsPageReady()) {
            std::string titleUtf8 = BiDiEngine::WideToUtf8(docTitle);
            if (!m_webViewViewer.UpdateContent(components.bodyHtml, components.tocHtml, titleUtf8, isDark)) {
                m_webViewViewer.SetHtmlContent(components.fullHtml);
            }
        } else {
            m_webViewViewer.SetHtmlContent(components.fullHtml);
        }

        if (m_config.syncWithCaret) {
            int line = GetScintillaCaretLine(hSci);
            m_webViewViewer.ScrollToLine(line + 1);
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
                if (m_useWebView2) {
                    m_webViewViewer.ScrollToLine(line + 1);
                } else {
                    m_renderer.ScrollToSourceLine(line);
                }
            }
        }
    }
}

void NppMarkdownNext::OnDarkModeChanged() {
    if (!m_isNppReady) return;
    bool isDark = SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
    if (m_config.darkModeOverride != -1) {
        isDark = (m_config.darkModeOverride == 1);
    }
    if (m_useWebView2) {
        m_webViewViewer.SetDarkMode(isDark);
    }
    m_renderer.SetDarkMode(isDark);
    if (m_hPanel) {
        InvalidateRect(m_hPanel, nullptr, TRUE);
    }
}

void NppMarkdownNext::OnBufferActivated() {
    if (!m_isNppReady) return;

    if (m_isPanelVisible) {
        m_lastRawContent.clear();
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

    if (msg != WM_SETCURSOR && msg != WM_NCHITTEST && msg != WM_MOUSEMOVE) {
        NppLog("PanelWndProc msg=0x%04X, wp=0x%p, lp=0x%p", msg, (void*)wParam, (void*)lParam);
    }

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
                self->ExecuteRender();
            }
            return 0;
        }

        case WM_NOTIFY: {
            NMHDR* pnm = reinterpret_cast<NMHDR*>(lParam);
            if (pnm && pnm->code == DMN_CLOSE) {
                self->m_isPanelVisible = false;
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
