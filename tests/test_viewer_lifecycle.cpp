#include "../include/NppMarkdownNext.h"
#include "../include/Resource.h"
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
        bool readOnly = false;
        size_t selectionStart = 0;
        size_t selectionEnd = 0;
        unsigned writes = 0;

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
            case NPPM_GETCURRENTSCINTILLA: *reinterpret_cast<int*>(lParam) = 0; return TRUE;
            case NPPM_ISDARKMODEENABLED: return FALSE;
            case NPPM_DMMSHOW: ShowWindow(reinterpret_cast<HWND>(lParam), SW_SHOW); return TRUE;
            case NPPM_DMMHIDE: ShowWindow(reinterpret_cast<HWND>(lParam), SW_HIDE); return TRUE;
            case SCI_GETLENGTH: return static_cast<LRESULT>(editor->text.size());
            case SCI_GETTEXT:
                strncpy_s(reinterpret_cast<char*>(lParam), static_cast<size_t>(wParam), editor->text.c_str(), _TRUNCATE);
                return static_cast<LRESULT>(editor->text.size());
            case SCI_GETCURRENTPOS:
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
        fixture.host.RequestRenderDebounced();
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
        {"TypedOperationResultsAndRequests", Peer::TypedOperationResultsAndRequests},
        {"ExportWarningsReachNativeUi", Peer::ExportWarningsReachNativeUi},
        {"CheckboxProtocolIsStrict", Peer::CheckboxProtocolIsStrict},
        {"CheckboxChangesOnlyVerifiedMarker", Peer::CheckboxChangesOnlyVerifiedMarker},
        {"CheckboxRejectsStaleBufferRevisionAndContent", Peer::CheckboxRejectsStaleBufferRevisionAndContent},
        {"ReadingSettingsAndPalettePriority", Peer::ReadingSettingsAndPalettePriority}
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
