#include "../include/NppMarkdownNext.h"
#include "../include/BiDiEngine.h"
#include "../include/HtmlExporter.h"
#include "../include/Resource.h"
#include "../include/NppLog.h"
#include <commctrl.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <algorithm>
#include <fstream>
#include <sstream>

extern "C" int GetPluginCmdId(int index);

namespace {
const wchar_t* s_panelClassName = L"NppMarkdownNextWindowClass";
}

NppMarkdownNext& NppMarkdownNext::Instance() {
    static NppMarkdownNext s_inst;
    return s_inst;
}

void NppMarkdownNext::Init(HINSTANCE hInst, NppData nppData) {
    m_hInst = hInst;
    m_nppData = nppData;

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
    ApplyAutoShowPanel();
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
    m_config.Save(m_configPath);
    if (m_hPanel && IsWindow(m_hPanel)) {
        DestroyWindow(m_hPanel);
        m_hPanel = nullptr;
    }
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

    wchar_t tempPath[MAX_PATH] = { 0 };
    GetTempPathW(MAX_PATH, tempPath);
    std::wstring userDataDir = std::wstring(tempPath) + L"NppMarkdownNext_WebView2";

    m_webViewViewer.SetPageReadyCallback([this]() {
        NppLog("WebView2 page ready, executing initial render");
        m_lastRawContentUtf8.clear();
        if (m_isPanelVisible) {
            ExecuteRender();
        }
    });

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
        SetTheme(IsDarkThemeEffective() ? ReaderTheme::Light : ReaderTheme::Dark);
    });

    m_webViewViewer.SetBiDiCallback([this]() {
        ToggleBiDi();
    });

    m_webViewViewer.SetZoomChangeCallback([this](float factor) {
        m_config.zoomLevel = factor;
        if (m_config.zoomLevel < 0.3f) m_config.zoomLevel = 0.3f;
        if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
        m_config.Save(m_configPath);
    });

    m_webViewViewer.Initialize(m_hPanel, userDataDir);
    m_webViewViewer.SetZoom(m_config.zoomLevel);

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
        m_webViewViewer.SetVisible(true);
        m_lastRawContentUtf8.clear();
        ExecuteRender();
    }
}

void NppMarkdownNext::ApplyAutoShowPanel() {
    if (!m_config.autoShowPanel) return;
    if (GetCurrentBufferPath().find(L'\\') == std::wstring::npos) return;
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
    m_config.Save(m_configPath);
    int cmdCaret = GetPluginCmdId(CMD_SYNC_CARET);
    if (cmdCaret > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdCaret, (LPARAM)(m_config.syncWithCaret ? TRUE : FALSE));
}

void NppMarkdownNext::ToggleBiDi() {
    m_config.isSmartBiDiEnabled = !m_config.isSmartBiDiEnabled;
    m_config.Save(m_configPath);
    int cmdBiDi = GetPluginCmdId(CMD_TOGGLE_BIDI);
    if (cmdBiDi > 0) SendMessage(m_nppData._nppHandle, NPPM_SETMENUITEMCHECK, (WPARAM)cmdBiDi, (LPARAM)(m_config.isSmartBiDiEnabled ? TRUE : FALSE));
    m_webViewViewer.SetBiDi(m_config.isSmartBiDiEnabled);
}

bool NppMarkdownNext::IsDarkThemeEffective() const {
    if (m_config.theme == ReaderTheme::Dark) return true;
    if (m_config.theme == ReaderTheme::Light) return false;
    // Follow Notepad++
    return SendMessage(m_nppData._nppHandle, NPPM_ISDARKMODEENABLED, 0, 0) != 0;
}

void NppMarkdownNext::SetTheme(ReaderTheme theme) {
    m_config.theme = theme;
    m_config.Save(m_configPath);
    SyncThemeMenuItems();
    m_webViewViewer.SetTheme(IsDarkThemeEffective());
}

void NppMarkdownNext::OnDarkModeChanged() {
    if (m_config.theme == ReaderTheme::FollowNpp) {
        m_webViewViewer.SetTheme(IsDarkThemeEffective());
    }
}

void NppMarkdownNext::ZoomIn() {
    m_config.zoomLevel += 0.1f;
    if (m_config.zoomLevel > 3.0f) m_config.zoomLevel = 3.0f;
    m_config.Save(m_configPath);
    m_webViewViewer.SetZoom(m_config.zoomLevel);
}

void NppMarkdownNext::ZoomOut() {
    m_config.zoomLevel -= 0.1f;
    if (m_config.zoomLevel < 0.3f) m_config.zoomLevel = 0.3f;
    m_config.Save(m_configPath);
    m_webViewViewer.SetZoom(m_config.zoomLevel);
}

void NppMarkdownNext::ZoomReset() {
    m_config.zoomLevel = 1.0f;
    m_config.Save(m_configPath);
    m_webViewViewer.SetZoom(m_config.zoomLevel);
}

void NppMarkdownNext::CopyRenderedHtml() {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return;
    std::string textUtf8 = GetScintillaText(hSci);
    std::wstring wideText = BiDiEngine::Utf8ToWide(textUtf8);
    MarkdownDocument doc = MarkdownParser::Parse(wideText);
    std::wstring docTitle = PathFindFileNameW(GetCurrentBufferPath().c_str());
    if (docTitle.empty()) docTitle = L"Markdown Preview";

    HtmlExporter::CopyToClipboard(m_hPanel ? m_hPanel : m_nppData._nppHandle, doc, docTitle,
                                  IsDarkThemeEffective() ? ReaderTheme::Dark : ReaderTheme::Light);
}

void NppMarkdownNext::SaveAsHtml() {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return;
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
        HtmlExporter::SaveToFile(fileName, doc, L"Markdown Export",
                                 IsDarkThemeEffective() ? ReaderTheme::Dark : ReaderTheme::Light);
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

void NppMarkdownNext::RequestRenderDebounced() {
    if (!m_isPanelVisible) return;
    if (!IsCurrentBufferMarkdown()) return;
    SetTimer(m_hPanel, IDT_RENDER_DEBOUNCE, RENDER_DEBOUNCE_DELAY_MS, nullptr);
}

bool NppMarkdownNext::ExecuteRender() {
    HWND hSci = GetCurrentScintilla();
    if (!hSci) return false;

    std::string utf8Content = GetScintillaText(hSci);
    int caretLine = m_config.syncWithCaret ? GetScintillaCaretLine(hSci) : -1;

    if (utf8Content == m_lastRawContentUtf8) {
        if (m_config.syncWithCaret && caretLine >= 0) {
            m_webViewViewer.ScrollToLine(caretLine);
        }
        return true;
    }

    m_lastRawContentUtf8 = utf8Content;

    std::wstring currentPath = GetCurrentBufferPath();
    std::wstring docTitle = PathFindFileNameW(currentPath.c_str());
    if (docTitle.empty()) docTitle = L"Markdown Reader";

    bool isDark = IsDarkThemeEffective();

    m_webViewViewer.UpdateMarkdown(utf8Content, docTitle, isDark, m_config.isSmartBiDiEnabled, caretLine);
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
                    m_webViewViewer.ScrollToLine(line);
                }
            }
        }
    }
}

void NppMarkdownNext::OnBufferActivated() {
    if (!m_isNppReady) return;
    ApplyAutoShowPanel();
    if (m_isPanelVisible) {
        m_lastRawContentUtf8.clear();
        ExecuteRender();
    }
}

void NppMarkdownNext::OnDocModified() {
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
                self->m_webViewViewer.SetVisible(true);
                self->m_lastRawContentUtf8.clear();
                self->ExecuteRender();
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
            break;
        }

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            self->m_webViewViewer.Resize(0, 0, width, height);
            return 0;
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
