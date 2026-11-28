#include "../include/NppMarkdownNext.h"
#include "../include/Resource.h"
#include "../include/BiDiEngine.h"
#include "../include/Scintilla.h"
#include "../include/ReaderText.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

bool g_isDllDetaching = false;
extern "C" int GetPluginCmdId(int index) { return index + 2000; }

struct ReaderLifecycleTestPeer {
    struct Editor {
        std::string text = "# Document\n";
        std::wstring path = L"C:\\reader-test\\one.md";
        std::wstring configRoot;
        UINT_PTR bufferId = 42;
        UINT codePage = SC_CP_UTF8;
        std::wstring lastOpened;
        bool readOnly = false;
        size_t selectionStart = 0;
        size_t selectionEnd = 0;
        unsigned writes = 0;
        std::vector<LPARAM> menuCommands;

        size_t LineStart(WPARAM line) const {
            size_t start = 0;
            while (line--) {
                size_t end = text.find('\n', start);
                if (end == std::string::npos) return std::string::npos;
                start = end + 1;
            }
            return start;
        }

        size_t LineLength(WPARAM line) const {
            size_t start = LineStart(line);
            if (start == std::string::npos) return 0;
            size_t end = text.find('\n', start);
            return (end == std::string::npos ? text.size() : end + 1) - start;
        }
    };

    static LRESULT CALLBACK EditorProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_NCCREATE) {
            auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        }
        auto* editor = reinterpret_cast<Editor*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (!editor) return DefWindowProc(hwnd, message, wParam, lParam);
        switch (message) {
            case NPPM_GETPLUGINSCONFIGDIR:
            case NPPM_GETFULLCURRENTPATH: {
                const auto& path = message == NPPM_GETPLUGINSCONFIGDIR ? editor->configRoot : editor->path;
                wcsncpy_s(reinterpret_cast<wchar_t*>(lParam), static_cast<size_t>(wParam), path.c_str(), _TRUNCATE);
                return TRUE;
            }
            case NPPM_GETCURRENTBUFFERID: return editor->bufferId;
            case NPPM_DOOPEN: editor->lastOpened = reinterpret_cast<const wchar_t*>(lParam); return TRUE;
            case NPPM_GETFULLPATHFROMBUFFERID:
                if (wParam != editor->bufferId) return -1;
                if (lParam) wcscpy_s(reinterpret_cast<wchar_t*>(lParam), editor->path.size() + 1, editor->path.c_str());
                return static_cast<LRESULT>(editor->path.size());
            case NPPM_GETCURRENTSCINTILLA: *reinterpret_cast<int*>(lParam) = 0; return TRUE;
            case NPPM_ISDARKMODEENABLED: return FALSE;
            case NPPM_DMMSHOW: ShowWindow(reinterpret_cast<HWND>(lParam), SW_SHOW); return TRUE;
            case NPPM_DMMHIDE: ShowWindow(reinterpret_cast<HWND>(lParam), SW_HIDE); return TRUE;
            case NPPM_MENUCOMMAND: editor->menuCommands.push_back(lParam); return TRUE;
            case SCI_GETLENGTH: return static_cast<LRESULT>(editor->text.size());
            case SCI_GETTEXT:
                strncpy_s(reinterpret_cast<char*>(lParam), static_cast<size_t>(wParam), editor->text.c_str(), _TRUNCATE);
                return static_cast<LRESULT>(editor->text.size());
            case SCI_GETCURRENTPOS: return static_cast<LRESULT>(editor->selectionEnd);
            case SCI_GETANCHOR: return static_cast<LRESULT>(editor->selectionStart);
            case SCI_GETCODEPAGE: return static_cast<LRESULT>(editor->codePage);
            case SCI_LINEFROMPOSITION: return 0;
            case SCI_GETREADONLY: return editor->readOnly;
            case SCI_POSITIONFROMLINE: {
                size_t start = editor->LineStart(wParam);
                return start == std::string::npos ? -1 : static_cast<LRESULT>(start);
            }
            case SCI_LINELENGTH: return static_cast<LRESULT>(editor->LineLength(wParam));
            case SCI_GETLINE: {
                size_t start = editor->LineStart(wParam), length = editor->LineLength(wParam);
                if (length) std::memcpy(reinterpret_cast<char*>(lParam), editor->text.data() + start, length);
                return static_cast<LRESULT>(length);
            }
            case SCI_SETSEL:
                editor->selectionStart = static_cast<size_t>(wParam);
                editor->selectionEnd = static_cast<size_t>(lParam);
                return 0;
            case SCI_REPLACESEL:
                assert(!editor->readOnly && editor->selectionEnd <= editor->text.size());
                editor->text.replace(editor->selectionStart, editor->selectionEnd - editor->selectionStart, reinterpret_cast<const char*>(lParam));
                ++editor->writes;
                return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    struct Fixture {
        Editor editor;
        NppMarkdownNext host;
        std::vector<std::wstring> messages;
        std::vector<std::wstring> nativeWarnings;
        HWND owner = nullptr;
        std::wstring iniPath;

        Fixture() {
            wchar_t temp[MAX_PATH] = {};
            wchar_t unique[MAX_PATH] = {};
            assert(GetTempPathW(MAX_PATH, temp));
            assert(GetTempFileNameW(temp, L"Npr", 0, unique));
            assert(DeleteFileW(unique));
            assert(CreateDirectoryW(unique, nullptr));
            editor.configRoot = unique;
            iniPath = editor.configRoot + L"\\NppMarkdownNext.ini";
            WNDCLASSW wc = {};
            wc.lpfnWndProc = EditorProc;
            wc.hInstance = GetModuleHandleW(nullptr);
            wc.lpszClassName = L"ReaderLifecycleMockEditor";
            RegisterClassW(&wc);
            owner = CreateWindowExW(0, wc.lpszClassName, L"", WS_POPUP, 0, 0, 800, 600,
                                    nullptr, nullptr, wc.hInstance, &editor);
            assert(owner);
            NppData npp = {};
            npp._nppHandle = owner;
            npp._scintillaMainHandle = owner;
            host.Init(wc.hInstance, npp);
            host.m_testInitializeViewer = [] { return true; };
            host.m_testExportWarning = [this](const std::wstring& message) { nativeWarnings.push_back(message); };
            assert(host.CreatePanelWindow());
            host.m_isNppReady = true;
            host.m_webViewViewer.m_isInitialized = true;
            host.m_webViewViewer.m_testPostJson = [this](const std::wstring& json) {
                messages.push_back(json);
                return S_OK;
            };
        }

        ~Fixture() {
            host.Cleanup();
            if (owner) DestroyWindow(owner);
            DeleteFileW(iniPath.c_str());
            DeleteFileW((editor.configRoot + L"\\NppMarkdownNext.documents.ini").c_str());
            assert(RemoveDirectoryW(editor.configRoot.c_str()));
        }

        void Ready() { host.m_webViewViewer.OnWebMessageReceived(L"pageLoaded"); }
        void Tick() { SendMessage(host.m_hPanel, WM_TIMER, IDT_RENDER_DEBOUNCE, 0); }
        size_t Count(const wchar_t* part) const {
            size_t count = 0;
            for (const auto& message : messages) if (message.find(part) != std::wstring::npos) ++count;
            return count;
        }
        void DrainRecovery() {
            MSG message;
            unsigned count = 0;
            while (PeekMessageW(&message, host.m_hPanel, WM_APP_VIEWER_RECOVERY, WM_APP_VIEWER_RECOVERY, PM_REMOVE)) {
                assert(++count < 16);
                DispatchMessageW(&message);
            }
        }
        void DrainWarnings() {
            MSG message;
            while (PeekMessageW(&message, host.m_hPanel, WM_APP_EXPORT_RESULT, WM_APP_EXPORT_RESULT, PM_REMOVE)) DispatchMessageW(&message);
        }
        void DrainHostCommands() {
            MSG message;
            while (PeekMessageW(&message, host.m_hPanel, WM_APP_HOST_COMMAND, WM_APP_HOST_COMMAND, PM_REMOVE)) DispatchMessageW(&message);
        }
        std::wstring Checkbox(int line, int column, unsigned state) const {
            return L"toggleCheckbox:" + std::to_wstring(editor.bufferId) + L":" +
                std::to_wstring(host.m_webViewViewer.m_renderSequence) + L":" + std::to_wstring(line) + L":" +
                std::to_wstring(column) + L":" + std::to_wstring(state);
        }
        void SendCheckbox(const std::wstring& message) { host.m_webViewViewer.OnWebMessageReceived(message); }
    };

    static void ViewerCacheUsesWritableTempRoot() {
        Fixture fixture;
        auto& host = fixture.host;
        host.m_configPath = L"C:\\Profiles\\ReaderA\\NppMarkdownNext.ini";
        std::wstring first = host.GetViewerDataDirectory();
        assert(first.ends_with(L"NppMarkdownNext-WebView2-dbd84032223cd0d0"));
        assert(first == host.GetViewerDataDirectory());
        host.m_configPath = L"C:/profiles/readera/another-settings.ini";
        assert(first == host.GetViewerDataDirectory());
        host.m_configPath = L"C:\\Profiles\\ReaderB\\NppMarkdownNext.ini";
        assert(first != host.GetViewerDataDirectory());

        std::wstring blockedRoot = fixture.editor.configRoot + L"\\blocked-config";
        HANDLE blocked = CreateFileW(blockedRoot.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_READONLY, nullptr);
        assert(blocked != INVALID_HANDLE_VALUE && CloseHandle(blocked));
        host.m_configPath = blockedRoot + L"\\settings\\NppMarkdownNext.ini";
        host.m_config.contentFontSizePx = 19;
        assert(!host.SaveConfig(false));
        assert(host.m_config.contentFontSizePx == 19);
        std::wstring cache = host.GetViewerDataDirectory();
        wchar_t temp[MAX_PATH] = {};
        assert(GetTempPathW(MAX_PATH, temp));
        assert(cache.starts_with(temp));
        assert(!cache.starts_with(blockedRoot));
        assert(CreateDirectoryW(cache.c_str(), nullptr));
        std::wstring probe = cache + L"\\write-probe.tmp";
        HANDLE file = CreateFileW(probe.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY, nullptr);
        assert(file != INVALID_HANDLE_VALUE);
        DWORD written = 0;
        assert(WriteFile(file, "test", 4, &written, nullptr) && written == 4);
        assert(CloseHandle(file));
        assert(DeleteFileW(probe.c_str()) && RemoveDirectoryW(cache.c_str()));
        assert(SetFileAttributesW(blockedRoot.c_str(), FILE_ATTRIBUTE_NORMAL));
        assert(DeleteFileW(blockedRoot.c_str()));
        host.m_configPath = fixture.iniPath;
        std::cout << "  -> stable profile key; distinct roots; live settings retained; TEMP cache write verified" << std::endl;
    }

    static void PendingClearedAfterReady() {
        WebView2Viewer viewer;
        viewer.m_isInitialized = true;
        viewer.m_testPostJson = [](const std::wstring&) { return S_OK; };
        assert(!viewer.UpdateMarkdown(std::string(1024 * 1024, 'x'), L"Old", false, true, 0, L"42"));
        viewer.SetPageReadyCallback([&] {
            assert(!viewer.m_pendingRender.valid && viewer.m_pendingRender.markdown.empty());
            viewer.m_pageReady = false;
            assert(!viewer.UpdateMarkdown("New", L"New title", false, true, -1, L"84"));
        });
        viewer.OnWebMessageReceived(L"pageLoaded");
        assert(viewer.m_pendingRender.valid);
        assert(viewer.m_pendingRender.markdown == "New");
        assert(viewer.m_pendingRender.bufferId == L"84");
        viewer.Close();
        assert(!viewer.m_pendingRender.valid && viewer.m_pendingRender.markdown.empty());
        assert(viewer.m_pendingRender.title.empty() && viewer.m_pendingRender.bufferId.empty());
    }

    static void PendingFallbackAndClose() {
        WebView2Viewer viewer;
        viewer.m_isInitialized = true;
        std::wstring sent;
        viewer.m_testPostJson = [&](const std::wstring& json) { sent = json; return S_OK; };
        assert(!viewer.UpdateMarkdown("First", L"Title", false, true, 7, L"9007199254740993"));
        viewer.OnWebMessageReceived(L"pageLoaded");
        assert(sent.find(L"\"bufferId\":\"9007199254740993\"") != std::wstring::npos);
        assert(sent.find(L"\"caretLine\":7") != std::wstring::npos);
        assert(!viewer.m_pendingRender.valid);
        viewer.m_pageReady = false;
        assert(!viewer.UpdateMarkdown("Retry", L"Retry", true, false, -1, L"43"));
        viewer.m_testPostJson = [](const std::wstring&) { return E_FAIL; };
        viewer.OnWebMessageReceived(L"pageLoaded");
        assert(viewer.m_pendingRender.valid && viewer.m_pendingRender.bufferId == L"43");
        viewer.m_testPostJson = [&](const std::wstring&) {
            viewer.m_pageReady = false;
            viewer.UpdateMarkdown("Newer", L"Newer", false, true, -1, L"44");
            return E_FAIL;
        };
        viewer.OnWebMessageReceived(L"pageLoaded");
        assert(viewer.m_pendingRender.markdown == "Newer" && viewer.m_pendingRender.bufferId == L"44");
        viewer.Close();
        assert(!viewer.m_pendingRender.valid && viewer.m_pendingRender.bufferId.empty());
    }

    static void CallbackAfterCloseIgnored() {
        int failed = 0;
        std::function<HRESULT(HRESULT, ICoreWebView2Environment*)> environment;
        std::function<HRESULT(HRESULT, ICoreWebView2Controller*)> controller;
        {
            auto viewer = std::make_unique<WebView2Viewer>();
            viewer->m_lifetime = std::make_shared<ViewerLifetime>();
            viewer->SetInitFailedCallback([&] { ++failed; });
            environment = viewer->MakeEnvironmentCallback();
            controller = viewer->MakeControllerCallback();
            viewer->Close();
            assert(environment(E_FAIL, nullptr) == S_OK);
        }
        assert(environment(E_FAIL, nullptr) == S_OK);
        assert(controller(E_FAIL, nullptr) == S_OK);
        assert(failed == 0);
    }

    static void RecoveryBudgetUsesMonotonicClock() {
        Fixture fixture;
        auto& host = fixture.host;
        ULONGLONG tick = 0;
        int starts = 0;
        host.m_testClock = [&] { return tick; };
        host.m_testInitializeViewer = [&] {
            ++starts;
            host.m_webViewViewer.m_initFailedCallback();
            host.m_webViewViewer.m_initFailedCallback();
            return false;
        };
        host.InitializeViewer();
        fixture.DrainRecovery();
        assert(starts == 2 && !host.m_recoveryQueued);
        tick = 29999;
        host.m_webViewViewer.m_initFailedCallback();
        fixture.DrainRecovery();
        assert(starts == 2);
        tick = 30000;
        host.m_webViewViewer.m_initFailedCallback();
        fixture.DrainRecovery();
        assert(starts == 3);
        host.HandleViewerRecovery(true);
        fixture.DrainRecovery();
        assert(starts == 4);
        tick = 59999;
        host.m_webViewViewer.m_initFailedCallback();
        fixture.DrainRecovery();
        assert(starts == 4);
        tick = 60000;
        host.m_webViewViewer.m_initFailedCallback();
        fixture.DrainRecovery();
        assert(starts == 5);
        const auto generation = host.m_viewerGeneration;
        host.Cleanup();
        SendMessage(host.m_hPanel, WM_APP_VIEWER_RECOVERY, generation, 0);
        host.HandleViewerRecovery(true);
        assert(starts == 5);
    }

    static void RenderCoalescesLifecycle() {
        Fixture fixture;
        fixture.host.SetPanelVisible(true);
        fixture.host.OnBufferActivated();
        fixture.Ready();
        assert(fixture.Count(L"\"type\":\"render\"") == 0);
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 1);
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 1);
        fixture.messages.clear();
        fixture.editor.text = "# Edited\n";
        ULONGLONG started = GetTickCount64();
        fixture.host.OnDocModified();
        MSG message;
        while (fixture.Count(L"\"type\":\"render\"") == 0 && GetTickCount64() - started < 2000) {
            if (PeekMessageW(&message, fixture.host.m_hPanel, WM_TIMER, WM_TIMER, PM_REMOVE)) DispatchMessageW(&message);
            else Sleep(1);
        }
        assert(fixture.Count(L"\"type\":\"render\"") == 1);
        assert(GetTickCount64() - started >= 35);
        assert(RENDER_DEBOUNCE_DELAY_MS == 50);
    }

    static void ForceEmptyAndManualText() {
        Fixture fixture;
        fixture.host.m_config.autoShowPanel = false;
        fixture.editor.path = L"C:\\reader-test\\manual.txt";
        fixture.host.SetPanelVisible(true);
        fixture.Ready();
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 1);
        fixture.editor.text.clear();
        fixture.host.OnBufferActivated();
        fixture.Tick();
        assert(fixture.Count(L"\"markdown\":\"\"") == 1);
        fixture.host.m_config.autoShowPanel = true;
        fixture.editor.path = L"new 1";
        fixture.host.OnBufferActivated();
        assert(!fixture.host.m_isPanelVisible);
        auto count = fixture.messages.size();
        fixture.Tick();
        assert(count == fixture.messages.size());
        fixture.host.TogglePanel();
        fixture.Tick();
        assert(fixture.host.m_isPanelVisible);
        assert(fixture.Count(L"\"markdown\":\"\"") == 2);
    }

    static void SaveAsAndRenamePolicy() {
        Fixture fixture;
        fixture.Ready();
        fixture.editor.path = L"new 1";
        fixture.host.OnBufferActivated();
        assert(!fixture.host.m_isPanelVisible);
        fixture.editor.path = L"C:\\reader-test\\saved.md";
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId);
        assert(fixture.host.m_isPanelVisible);
        fixture.Tick();
        assert(fixture.Count(L"\"title\":\"saved.md\"") == 1);
        fixture.messages.clear();
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId);
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId + 1);
        assert(!fixture.host.m_forceRenderPending);
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 0);
        fixture.editor.path = L"C:\\reader-test\\saved.txt";
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId);
        assert(!fixture.host.m_isPanelVisible);
        fixture.host.m_userClosedPanel = true;
        fixture.editor.path = L"C:\\reader-test\\saved-again.md";
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId);
        assert(!fixture.host.m_isPanelVisible);
    }

    static void EditorIntegrationDetails() {
        Fixture fixture;
        auto& host = fixture.host;
        // An "ANSI" buffer (code page 0) reaches the reader as UTF-8 in the system code page's meaning
        fixture.editor.codePage = 0;
        fixture.editor.text = "caf\xE9";
        wchar_t expected[8] = {};
        assert(MultiByteToWideChar(CP_ACP, 0, "caf\xE9", 4, expected, 8) == 4);
        assert(host.GetScintillaText(fixture.owner) == BiDiEngine::WideToUtf8(expected));
        fixture.editor.codePage = SC_CP_UTF8;

        // Toggling a task keeps the user's selection; a read-only file explains why nothing changed
        fixture.editor.text = "- [ ] Task\nsome text\n";
        host.SetPanelVisible(true);
        fixture.Ready();
        fixture.Tick();
        fixture.editor.selectionStart = 13;
        fixture.editor.selectionEnd = 17;
        fixture.SendCheckbox(fixture.Checkbox(0, 2, 0));
        assert(fixture.editor.text.starts_with("- [x] Task") && fixture.editor.selectionStart == 13 && fixture.editor.selectionEnd == 17);
        fixture.Tick();
        fixture.messages.clear();
        fixture.editor.readOnly = true;
        fixture.SendCheckbox(fixture.Checkbox(0, 2, 1));
        assert(fixture.Count(L"\"type\":\"notice\"") == 1 && fixture.Count(L"read-only") == 1);
        fixture.editor.readOnly = false;

        // A render request without any edit copies nothing and posts nothing
        fixture.Tick();
        fixture.messages.clear();
        host.RequestRenderDebounced();
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 0);

        // A manually previewed non-Markdown file stays live while it is edited (auto-show would hide the panel there)
        host.m_config.autoShowPanel = false;
        fixture.editor.path = L"C:\\reader-test\\notes.txt";
        host.OnBufferActivated();
        fixture.Tick();
        fixture.messages.clear();
        fixture.editor.text = "# Edited notes\n";
        host.OnDocModified();
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 1 && fixture.Count(L"Edited notes") == 1);
        assert(fixture.Count(L"\"docBase\":\"https://npp-reader-document.local/\"") == 0); // no webview in the test: no mapping

        // Reading width presets come from one list: 0 (the whole pane) is a valid choice, other values are not
        host.OnSettingsMessage(L"readingWidth:0");
        assert(host.m_config.contentMaxWidthPx == 0);
        host.OnSettingsMessage(L"readingWidth:999");
        assert(host.m_config.contentMaxWidthPx == 0);
        host.OnSettingsMessage(L"readingWidth:860");
        assert(host.m_config.contentMaxWidthPx == 860);
    }

    static void LinksOpenOnlySafeTargets() {
        Fixture fixture;
        fixture.Ready();
        const std::wstring folder = fixture.editor.configRoot, plan = folder + L"\\plan.md";
        HANDLE file = CreateFileW(plan.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        assert(file != INVALID_HANDLE_VALUE);
        CloseHandle(file);
        fixture.editor.path = folder + L"\\doc.md";
        fixture.host.SetPanelVisible(true);
        fixture.host.OnSettingsMessage(L"openLink:plan.md#%D1%80%D0%B8%D1%81%D0%BA%D0%B8");
        assert(fixture.editor.lastOpened == plan);
        // the opened document scrolls to the section once it is rendered (Notepad++ activates it)
        fixture.messages.clear();
        fixture.editor.path = plan;
        fixture.host.OnBufferActivated();
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"scrollToAnchor\"") == 1 && fixture.Count(L"\"id\":\"%D1%80%D0%B8%D1%81%D0%BA%D0%B8\"") == 1);
        // a section link into the document already on screen scrolls at once
        fixture.messages.clear();
        fixture.host.OnSettingsMessage(L"openLink:plan.md#next");
        assert(fixture.Count(L"\"id\":\"next\"") == 1);
        fixture.editor.path = folder + L"\\doc.md";
        fixture.editor.lastOpened.clear();
        fixture.messages.clear();
        fixture.host.OnSettingsMessage(L"openLink:\\\\attacker.example\\share\\x.md");
        fixture.host.OnSettingsMessage(L"openLink:file://attacker.example/share/x.md");
        assert(fixture.editor.lastOpened.empty() && fixture.Count(L"network locations") == 2);
        fixture.host.OnSettingsMessage(L"openLink:ms-settings:privacy");
        fixture.host.OnSettingsMessage(L"openLink:missing.md");
        assert(fixture.editor.lastOpened.empty() && fixture.Count(L"not opened from the preview: ms-settings") == 1 && fixture.Count(L"was not found") == 1);
        // .svg opens as text; shell links are refused before anything is touched
        const std::wstring svg = folder + L"\\pic.svg", shortcut = folder + L"\\run.lnk";
        for (const auto& name : {svg, shortcut}) { HANDLE created = CreateFileW(name.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, 0, nullptr); assert(created != INVALID_HANDLE_VALUE); CloseHandle(created); }
        fixture.host.OnSettingsMessage(L"openLink:pic.svg");
        assert(fixture.editor.lastOpened == svg);
        fixture.editor.lastOpened.clear();
        fixture.host.OnSettingsMessage(L"openLink:run.lnk");
        assert(fixture.editor.lastOpened.empty() && fixture.Count(L"Shortcut files are not opened") == 1);
        // a document on a share does not make every \\?\UNC\ or //host link "the same share"
        fixture.messages.clear();
        fixture.editor.path = L"\\\\server\\share\\doc.md";
        fixture.host.OnSettingsMessage(L"openLink:\\\\?\\UNC\\evil\\share\\x.md");
        fixture.host.OnSettingsMessage(L"openLink://evil/share/x.md");
        fixture.host.OnSettingsMessage(L"openLink:\\\\.\\pipe\\x");
        assert(fixture.editor.lastOpened.empty() && fixture.Count(L"network locations") == 3);
        DeleteFileW(svg.c_str());
        DeleteFileW(shortcut.c_str());
        DeleteFileW(plan.c_str());
    }

    static void ReadingPlaceAndRenames() {
        Fixture fixture;
        fixture.host.SetPanelVisible(true);
        fixture.Ready();
        const std::wstring id = std::to_wstring(fixture.editor.bufferId);
        fixture.host.OnSettingsMessage(L"readingPosition:" + id + L":120:-40");
        fixture.host.OnSettingsMessage(L"readingPosition:" + id + L":12;x");
        assert(fixture.host.LoadDocumentState(DocumentState::ReadingPosition, fixture.editor.path) == L"120:-40");
        fixture.host.OnSettingsMessage(L"tableWidths:" + id + L":0a1b2c3d:150,250");
        fixture.messages.clear();
        fixture.host.OnBufferActivated();
        fixture.Tick();
        assert(fixture.Count(L"\"kind\":\"readingPosition\"") == 1 && fixture.Count(L"\"value\":\"120:-40\"") == 1);
        // Save As / rename: the state follows the document to its new path (and the old path keeps it)
        const std::wstring oldPath = fixture.editor.path;
        fixture.editor.path = L"C:\\reader-test\\renamed.md";
        fixture.host.OnDocumentPathChanged(fixture.editor.bufferId);
        assert(fixture.host.LoadDocumentState(DocumentState::ReadingPosition, fixture.editor.path) == L"120:-40");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, fixture.editor.path) == L"0a1b2c3d:150,250");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, oldPath) == L"0a1b2c3d:150,250");
        // it reached the file, not just the session cache
        fixture.host.m_documentStateCache.clear();
        assert(fixture.host.LoadDocumentState(DocumentState::ReadingPosition, fixture.editor.path) == L"120:-40");

        // the first version's NppMarkdownNext.tables.ini moves to the new name once, keeping its widths
        const std::wstring documents = fixture.editor.configRoot + L"\\NppMarkdownNext.documents.ini";
        const std::wstring legacy = fixture.editor.configRoot + L"\\NppMarkdownNext.tables.ini";
        assert(MoveFileExW(documents.c_str(), legacy.c_str(), 0));
        fixture.host.Init(GetModuleHandleW(nullptr), fixture.host.m_nppData);
        assert(GetFileAttributesW(legacy.c_str()) == INVALID_FILE_ATTRIBUTES && GetFileAttributesW(documents.c_str()) != INVALID_FILE_ATTRIBUTES);
        fixture.host.m_documentStateCache.clear();
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, oldPath) == L"0a1b2c3d:150,250");
    }

    static void TableWidthsPersistPerDocument() {
        Fixture fixture;
        fixture.host.SetPanelVisible(true);
        fixture.Ready();
        const std::wstring id = std::to_wstring(fixture.editor.bufferId);
        fixture.host.OnSettingsMessage(L"tableWidths:" + id + L":0a1b2c3d:120,200");
        fixture.host.OnSettingsMessage(L"tableWidths:" + id + L":bad=value");
        fixture.host.OnSettingsMessage(L"tableWidths:x1:0a1b2c3d:1");
        fixture.host.OnSettingsMessage(L"tableWidths:999:0a1b2c3d:1");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, fixture.editor.path) == L"0a1b2c3d:120,200");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, L"C:\\READER-TEST\\ONE.MD") == L"0a1b2c3d:120,200");
        fixture.host.m_documentStateCache.clear(); // a restarted Notepad++ has only the file
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, fixture.editor.path) == L"0a1b2c3d:120,200");
        fixture.messages.clear();
        fixture.host.OnBufferActivated();
        fixture.Tick();
        assert(fixture.Count(L"\"kind\":\"tableWidths\"") == 1 && fixture.Count(L"\"value\":\"0a1b2c3d:120,200\"") == 1);
        fixture.messages.clear();
        fixture.editor.text = "# Typed\n";
        fixture.host.OnDocModified();
        fixture.Tick();
        assert(fixture.Count(L"\"type\":\"render\"") == 1 && fixture.Count(L"\"kind\":\"tableWidths\"") == 0);
        fixture.host.OnSettingsMessage(L"tableWidths:" + id + L":");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, fixture.editor.path).empty());
        fixture.editor.path = L"new 1";
        fixture.host.OnSettingsMessage(L"tableWidths:" + id + L":0a1b2c3d:1");
        assert(fixture.host.LoadDocumentState(DocumentState::TableWidths, L"new 1").empty());
        // an unwritable state file: reported once, not after every scroll; an unchanged value is not rewritten
        fixture.editor.path = L"C:\\Reader-Test\\one.md";
        fixture.host.m_documentStatePath = L"Z:\\no-such-folder-reader-test\\documents.ini";
        fixture.messages.clear();
        for (const wchar_t* place : {L"10", L"20", L"20", L"30"}) fixture.host.OnSettingsMessage(L"readingPosition:" + id + L":" + place);
        assert(fixture.Count(L"saveSettings") == 1);
    }

    static void SettingsWarningLifecycle() {
        Fixture fixture;
        fixture.Ready();
        fixture.messages.clear();
        fixture.host.m_configPath.clear();
        fixture.host.m_config.contentFontSizePx = 19;
        assert(!fixture.host.SaveConfig());
        assert(!fixture.host.SaveConfig());
        fixture.host.SyncUiState();
        assert(fixture.Count(L"\"operation\":\"saveSettings\"") == 1);
        assert(fixture.host.m_config.contentFontSizePx == 19);
        fixture.host.m_configPath = fixture.iniPath;
        assert(fixture.host.SaveConfig());
        assert(fixture.Count(L"\"status\":\"succeeded\"") == 1);
        assert(!fixture.host.m_settingsSaveWarningActive);
    }

    static void TypedOperationResultsAndRequests() {
        Fixture fixture;
        fixture.Ready();
        fixture.messages.clear();
        fixture.host.m_webViewViewer.ReportOperationResult(L"copyHtml", {OperationStatus::Partial, ERROR_ACCESS_DENIED,
            L"Only \"HTML\"", L"C:\\test\\backup.html"}, L"page:7");
        assert(fixture.Count(L"\"status\":\"partial\"") == 1);
        assert(fixture.Count(L"\"requestId\":\"page:7\"") == 1);
        assert(fixture.Count(LR"(Only \"HTML\")") == 1);
        fixture.host.OnSettingsMessage(L"copyHtml:page:8");
        assert(fixture.host.m_exportQueued && fixture.host.m_queuedExportRequestId == L"page:8");
        fixture.host.OnSettingsMessage(L"saveAsHtml:page:9");
        assert(fixture.Count(L"\"requestId\":\"page:9\"") == 1);
        assert(fixture.host.m_queuedExportRequestId == L"page:8");
        fixture.host.m_exportQueued = false;
        fixture.host.m_exportInProgress = true;
        fixture.host.CopyRenderedHtml(L"page:10");
        assert(fixture.Count(L"\"requestId\":\"page:10\"") == 1);
        fixture.host.m_exportInProgress = false;
        fixture.host.OnSettingsMessage(L"copyHtml:bad\"id");
        assert(!fixture.host.m_exportQueued);
    }

    static void ExportWarningsReachNativeUi() {
        Fixture fixture;
        fixture.Ready();
        auto& host = fixture.host;
        OperationResult partial{OperationStatus::Partial, ERROR_ACCESS_DENIED, L"Clipboard text could not be copied.", L"C:\\test\\recovered.html"};
        host.ReportExportResult(L"copyHtml", partial, L"page:1");
        assert(fixture.nativeWarnings.empty());
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 1);
        assert(fixture.nativeWarnings.back().find(partial.recoveryPath) != std::wstring::npos);
        assert(fixture.Count(L"\"status\":\"partial\"") == 1);

        host.SetPanelVisible(true);
        host.ReportExportResult(L"copyHtml", partial, L"page:2");
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 1);
        host.ReportExportResult(L"copyHtml", {OperationStatus::Succeeded}, L"");
        host.ReportExportResult(L"saveAsHtml", {OperationStatus::Cancelled}, L"");
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 1);

        host.m_webViewViewer.m_testPostJson = [](const std::wstring&) { return E_FAIL; };
        host.ReportExportResult(L"copyHtml", partial, L"page:3");
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 2);
        host.m_webViewViewer.m_pageReady = false;
        host.ReportExportResult(L"saveAsHtml", partial, L"page:4");
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 3);

        // An unavailable editor exercises both menu failures without touching the clipboard or a file dialog.
        host.m_nppData._scintillaMainHandle = nullptr;
        host.CopyRenderedHtml();
        host.SaveAsHtml();
        assert(!host.m_exportInProgress);
        fixture.DrainWarnings();
        assert(fixture.nativeWarnings.size() == 5);
        assert(fixture.nativeWarnings[3].find(L"no active document to copy") != std::wstring::npos);
        assert(fixture.nativeWarnings[4].find(L"no active document to export") != std::wstring::npos);
    }

    static void CheckboxProtocolIsStrict() {
        WebView2Viewer viewer;
        unsigned callbacks = 0;
        viewer.SetCheckboxCallback([&](const CheckboxRequest& request) {
            ++callbacks;
            assert(request.bufferId == L"9007199254740993");
            assert(request.renderRevision == 7 && request.line == 3 && request.markerColumn == 9 && request.expectedState == 'X');
        });
        for (const auto* payload : {L"toggleCheckbox:3", L"toggleCheckbox:42:7:-1:2:0", L"toggleCheckbox:42:7:0:2:3",
                 L"toggleCheckbox:42:7:2147483648:2:0", L"toggleCheckbox:42:18446744073709551616:0:2:0",
                 L"toggleCheckbox:0:7:0:2:0", L"toggleCheckbox:42:0:0:2:0", L"toggleCheckbox:42:7:0:2:0:1",
                 L"toggleCheckbox:42:7:0:2:0junk", L"toggleCheckbox:42::0:2:0", L"toggleCheckbox:42:7:0:NaN:0"}) {
            viewer.OnWebMessageReceived(payload);
        }
        assert(callbacks == 0);
        viewer.OnWebMessageReceived(L"toggleCheckbox:9007199254740993:7:3:9:2");
        assert(callbacks == 1);
    }

    static void CheckboxChangesOnlyVerifiedMarker() {
        Fixture fixture;
        auto& host = fixture.host;
        fixture.editor.text = "- [x] Keep literal [ ]\r\n> - [X] Quoted\r\n  1. [ ] Ordered\n[ ] Plain text\n<input value='[ ]'>\n";
        host.SetPanelVisible(true);
        fixture.Ready();
        fixture.Tick();
        auto checked = fixture.Checkbox(0, 2, 1);
        fixture.SendCheckbox(checked);
        assert(fixture.editor.text.starts_with("- [ ] Keep literal [ ]\r\n"));
        assert(fixture.editor.writes == 1);
        fixture.SendCheckbox(checked);
        assert(fixture.editor.writes == 1 && host.m_forceRenderPending);
        fixture.Tick();
        assert(fixture.Count(L"\"renderRevision\":\"") >= 2);
        fixture.SendCheckbox(fixture.Checkbox(1, 4, 2));
        assert(fixture.editor.text.find("> - [ ] Quoted\r\n") != std::string::npos && fixture.editor.writes == 2);
        fixture.Tick();
        fixture.SendCheckbox(fixture.Checkbox(2, 5, 0));
        assert(fixture.editor.text.find("  1. [x] Ordered\n") != std::string::npos && fixture.editor.writes == 3);
        fixture.Tick();

        fixture.SendCheckbox(fixture.Checkbox(0, 19, 0));
        fixture.SendCheckbox(fixture.Checkbox(3, 0, 0));
        fixture.SendCheckbox(fixture.Checkbox(4, 14, 0));
        fixture.SendCheckbox(fixture.Checkbox(0, 2, 1));
        fixture.SendCheckbox(fixture.Checkbox(99, 2, 0));
        fixture.editor.readOnly = true;
        fixture.SendCheckbox(fixture.Checkbox(0, 2, 0));
        assert(fixture.editor.writes == 3);
    }

    static void CheckboxRejectsStaleBufferRevisionAndContent() {
        Fixture fixture;
        auto& host = fixture.host;
        fixture.editor.text = "- [ ] Task\n";
        host.SetPanelVisible(true);
        fixture.Ready();
        fixture.Tick();
        auto old = fixture.Checkbox(0, 2, 0);

        fixture.editor.bufferId = 43;
        fixture.SendCheckbox(old);
        assert(fixture.editor.writes == 0 && host.m_forceRenderPending);
        fixture.editor.bufferId = 42;
        host.OnBufferActivated();
        fixture.SendCheckbox(old);
        assert(fixture.editor.writes == 0);
        fixture.Tick();
        fixture.SendCheckbox(old);
        assert(fixture.editor.writes == 0);

        auto current = fixture.Checkbox(0, 2, 0);
        fixture.editor.text += "Extra content\n";
        fixture.SendCheckbox(current);
        assert(fixture.editor.writes == 0);
        fixture.editor.text = "- [ ] Task\n";
        SCNotification modified = {};
        modified.nmhdr.code = SCN_MODIFIED;
        modified.modificationType = SC_MOD_INSERTTEXT | SC_PERFORMED_USER;
        host.OnNotification(&modified);
        modified.modificationType = SC_MOD_DELETETEXT | SC_PERFORMED_UNDO;
        host.OnNotification(&modified);
        fixture.SendCheckbox(current);
        assert(fixture.editor.writes == 0 && host.m_forceRenderPending);
        fixture.Tick();
        fixture.SendCheckbox(fixture.Checkbox(0, 2, 0));
        assert(fixture.editor.writes == 1 && fixture.editor.text == "- [x] Task\n");
    }

    static void ReaderLanguageFollowsSetting() {
        Fixture fixture;
        fixture.Ready();
        fixture.host.OnSettingsMessage(L"readerLanguage:ru");
        assert(fixture.host.m_config.readerLanguage == L"ru" && ReaderTextRussian());
        assert(fixture.Count(L"\"language\":\"ru\",\"languageSetting\":\"ru\"") >= 1);
        assert(Tr("status.retry") != L"Retry preview");
        assert(Tr("link.notFound", L"x.md").find(L"x.md") != std::wstring::npos);
        assert(Tr("no.such.key") == L"no.such.key");
        for (const char* key : kPluginMenuKeys) assert(Tr(key) != std::wstring(key, key + std::strlen(key)));
        fixture.host.OnSettingsMessage(L"readerLanguage:de");
        assert(fixture.host.m_config.readerLanguage == L"ru");
        fixture.host.OnSettingsMessage(L"readerLanguage:auto");
        // the test host has no Notepad++ window to report a language, so Auto means English here
        assert(fixture.host.m_config.readerLanguage == L"auto" && !ReaderTextRussian());
        assert(fixture.Count(L"\"language\":\"en\",\"languageSetting\":\"auto\"") >= 1);
        assert(Tr("status.retry") == L"Retry preview");
    }

    static void ReadingSettingsAndPalettePriority() {
        Fixture fixture;
        fixture.Ready();
        assert(fixture.host.m_config.SetThemeId(L"bearded-v-gold-d-raynh"));
        fixture.host.OnSettingsMessage(L"darkMode:0");
        assert(fixture.host.m_config.darkModeOverride == -1);
        fixture.host.OnSettingsMessage(L"readingFontSize:17");
        fixture.host.OnSettingsMessage(L"readingLineHeight:190");
        fixture.host.OnSettingsMessage(L"readingWidth:1100");
        fixture.host.OnSettingsMessage(L"readingFontSize:18");
        fixture.host.OnSettingsMessage(L"readingLineHeight:NaN");
        assert(fixture.host.m_config.contentFontSizePx == 17);
        assert(fixture.host.m_config.contentLineHeightPercent == 190);
        assert(fixture.host.m_config.contentMaxWidthPx == 1100);
        assert(fixture.host.m_config.SetThemeId(L"follow"));
        fixture.host.OnSettingsMessage(L"darkMode:0");
        assert(fixture.host.m_config.darkModeOverride == 0);
        fixture.host.OnBufferClosed(9007199254740993ULL);
        assert(fixture.Count(L"\"type\":\"bufferClosed\"") == 1);
    }

    static void HostShortcutsForwardCloseSaveAndToggle() {
        Fixture fixture;
        auto& viewer = fixture.host.m_webViewViewer;
        fixture.host.SetPanelVisible(true);

        // Ctrl+W and Ctrl+F4 forward IDM_FILE_CLOSE; auto-repeat is swallowed without closing a second tab.
        assert(viewer.HandleAcceleratorKey('W', true, false, false, false));
        assert(viewer.HandleAcceleratorKey('W', true, false, false, true));
        assert(viewer.HandleAcceleratorKey(VK_F4, true, false, false, false));
        assert(viewer.HandleAcceleratorKey('S', true, false, false, false));
        fixture.DrainHostCommands();
        assert(fixture.editor.menuCommands.size() == 3);
        assert(fixture.editor.menuCommands[0] == IDM_FILE_CLOSE);
        assert(fixture.editor.menuCommands[1] == IDM_FILE_CLOSE);
        assert(fixture.editor.menuCommands[2] == IDM_FILE_SAVE);

        // Ctrl+Shift+M toggles the panel; browser/editor-local or AltGr combinations pass through untouched.
        assert(viewer.HandleAcceleratorKey('M', true, true, false, false));
        fixture.DrainHostCommands();
        assert(!fixture.host.m_isPanelVisible);

        assert(!viewer.HandleAcceleratorKey('C', true, false, false, false));
        assert(!viewer.HandleAcceleratorKey('V', true, false, false, false));
        assert(!viewer.HandleAcceleratorKey('W', true, true, false, false));
        assert(!viewer.HandleAcceleratorKey('W', true, false, true, false));
        assert(!viewer.HandleAcceleratorKey('W', false, false, true, false));
        fixture.DrainHostCommands();
        assert(fixture.editor.menuCommands.size() == 3);
    }

    static void HideEditorOverlayLifecycle() {
        auto isShown = [](HWND hwnd) { return hwnd && (GetWindowLongPtrW(hwnd, GWL_STYLE) & WS_VISIBLE) != 0; };
        Fixture fixture;
        auto& host = fixture.host;
        fixture.Ready();
        host.SetPanelVisible(true);
        assert(!host.m_config.hideEditor && !host.m_overlayActive);
        assert(GetParent(host.m_hViewerHost) == host.m_hPanel);

        // Enabling Hide Editor moves the live viewer host into the full-editor overlay without losing visibility.
        host.ToggleHideEditor();
        assert(host.m_config.hideEditor && host.m_overlayActive && host.m_isPanelVisible);
        assert(GetParent(host.m_hViewerHost) == host.m_hOverlay);
        assert(isShown(host.m_hOverlay) && !isShown(host.m_hPanel));
        RECT overlayRc = {}, ownerRc = {};
        assert(GetClientRect(host.m_hOverlay, &overlayRc) && GetClientRect(fixture.owner, &ownerRc));
        assert(overlayRc.right == ownerRc.right && overlayRc.bottom == ownerRc.bottom);
        assert(fixture.Count(L"\"hideEditor\":true") >= 1);

        // Switching to a non-Markdown tab suspends the overlay so the user sees the code editor; returning to .md restores it.
        fixture.editor.path = L"C:\\reader-test\\code.cpp";
        fixture.editor.bufferId = 43;
        host.OnBufferActivated();
        assert(!host.m_isPanelVisible && !host.m_overlayActive && !isShown(host.m_hOverlay));
        assert(host.m_config.hideEditor);

        fixture.editor.path = L"C:\\reader-test\\one.md";
        fixture.editor.bufferId = 42;
        host.OnBufferActivated();
        assert(host.m_isPanelVisible && host.m_overlayActive && isShown(host.m_hOverlay));
        assert(GetParent(host.m_hViewerHost) == host.m_hOverlay);

        // Double-clicking text in preview must not unhide the editor (preserves double-click word selection).
        host.m_webViewViewer.OnWebMessageReceived(L"jumpToLine:0");
        assert(host.m_config.hideEditor && host.m_overlayActive && host.m_isPanelVisible);
        assert(GetParent(host.m_hViewerHost) == host.m_hOverlay);
        assert(isShown(host.m_hOverlay) && !isShown(host.m_hPanel));

        // Ctrl+Shift+E from WebView2 restores the editor, toggles Hide Editor back on, and Ctrl+Shift+M closes cleanly.
        assert(host.m_webViewViewer.HandleAcceleratorKey('E', true, true, false, false));
        fixture.DrainHostCommands();
        assert(!host.m_config.hideEditor && !host.m_overlayActive && host.m_isPanelVisible);
        assert(GetParent(host.m_hViewerHost) == host.m_hPanel);
        assert(!isShown(host.m_hOverlay) && isShown(host.m_hPanel));

        assert(host.m_webViewViewer.HandleAcceleratorKey('E', true, true, false, false));
        fixture.DrainHostCommands();
        assert(host.m_config.hideEditor && host.m_overlayActive);
        assert(host.m_webViewViewer.HandleAcceleratorKey('M', true, true, false, false));
        fixture.DrainHostCommands();
        assert(!host.m_isPanelVisible && !host.m_overlayActive && !isShown(host.m_hOverlay));
    }
};

int main(int argc, char** argv) {
    using Peer = ReaderLifecycleTestPeer;
    const std::pair<const char*, void (*)()> cases[] = {
        {"ViewerCacheUsesWritableTempRoot", Peer::ViewerCacheUsesWritableTempRoot},
        {"PendingClearedAfterReady", Peer::PendingClearedAfterReady},
        {"PendingFallbackAndClose", Peer::PendingFallbackAndClose},
        {"CallbackAfterCloseIgnored", Peer::CallbackAfterCloseIgnored},
        {"RecoveryBudgetUsesMonotonicClock", Peer::RecoveryBudgetUsesMonotonicClock},
        {"RenderCoalescesLifecycle", Peer::RenderCoalescesLifecycle},
        {"ForceEmptyAndManualText", Peer::ForceEmptyAndManualText},
        {"SaveAsAndRenamePolicy", Peer::SaveAsAndRenamePolicy},
        {"SettingsWarningLifecycle", Peer::SettingsWarningLifecycle},
        {"TableWidthsPersistPerDocument", Peer::TableWidthsPersistPerDocument},
        {"ReadingPlaceAndRenames", Peer::ReadingPlaceAndRenames},
        {"LinksOpenOnlySafeTargets", Peer::LinksOpenOnlySafeTargets},
        {"EditorIntegrationDetails", Peer::EditorIntegrationDetails},
        {"TypedOperationResultsAndRequests", Peer::TypedOperationResultsAndRequests},
        {"ExportWarningsReachNativeUi", Peer::ExportWarningsReachNativeUi},
        {"CheckboxProtocolIsStrict", Peer::CheckboxProtocolIsStrict},
        {"CheckboxChangesOnlyVerifiedMarker", Peer::CheckboxChangesOnlyVerifiedMarker},
        {"CheckboxRejectsStaleBufferRevisionAndContent", Peer::CheckboxRejectsStaleBufferRevisionAndContent},
        {"ReaderLanguageFollowsSetting", Peer::ReaderLanguageFollowsSetting},
        {"ReadingSettingsAndPalettePriority", Peer::ReadingSettingsAndPalettePriority},
        {"HostShortcutsForwardCloseSaveAndToggle", Peer::HostShortcutsForwardCloseSaveAndToggle},
        {"HideEditorOverlayLifecycle", Peer::HideEditorOverlayLifecycle}
    };
    const char* selected = argc == 3 && std::strcmp(argv[1], "--case") == 0 ? argv[2] : nullptr;
    if (argc != 1 && !selected) { std::cerr << "Usage: test_viewer_lifecycle [--case name]\n"; return 2; }
    bool found = false;
    for (const auto& [name, run] : cases) {
        if (selected && std::strcmp(selected, name) != 0) continue;
        found = true;
        std::cout << "[TEST] " << name << std::endl;
        run();
        std::cout << "  -> PASS" << std::endl;
    }
    if (!found) { std::cerr << "Unknown test case\n"; return 2; }
    return 0;
}
