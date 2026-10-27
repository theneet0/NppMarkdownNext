#include "../include/WebView2Viewer.h"
#include "../include/BiDiEngine.h"
#include "../include/NppLog.h"
#include "KelkBundle.inc"
#include <initguid.h>
#include <WebView2.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <sstream>

namespace {

class EnvironmentCompletedHandler : public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler {
    LONG m_ref = 1;
    std::function<HRESULT(HRESULT, ICoreWebView2Environment*)> m_func;
public:
    EnvironmentCompletedHandler(std::function<HRESULT(HRESULT, ICoreWebView2Environment*)> func) : m_func(func) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler) {
            *ppvObject = static_cast<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_ref);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT result, ICoreWebView2Environment* env) override {
        return m_func ? m_func(result, env) : S_OK;
    }
};

class ControllerCompletedHandler : public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler {
    LONG m_ref = 1;
    std::function<HRESULT(HRESULT, ICoreWebView2Controller*)> m_func;
public:
    ControllerCompletedHandler(std::function<HRESULT(HRESULT, ICoreWebView2Controller*)> func) : m_func(func) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler) {
            *ppvObject = static_cast<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_ref);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT result, ICoreWebView2Controller* controller) override {
        return m_func ? m_func(result, controller) : S_OK;
    }
};

class WebMessageReceivedHandler : public ICoreWebView2WebMessageReceivedEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*)> m_func;
public:
    WebMessageReceivedHandler(std::function<HRESULT(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*)> func) : m_func(func) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_ICoreWebView2WebMessageReceivedEventHandler) {
            *ppvObject = static_cast<ICoreWebView2WebMessageReceivedEventHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_ref);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) override {
        return m_func ? m_func(sender, args) : S_OK;
    }
};

class NavigationStartingHandler : public ICoreWebView2NavigationStartingEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs*)> m_func;
public:
    NavigationStartingHandler(std::function<HRESULT(ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs*)> func) : m_func(func) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_ICoreWebView2NavigationStartingEventHandler) {
            *ppvObject = static_cast<ICoreWebView2NavigationStartingEventHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_ref);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) override {
        return m_func ? m_func(sender, args) : S_OK;
    }
};

class ZoomFactorChangedHandler : public ICoreWebView2ZoomFactorChangedEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2Controller*, IUnknown*)> m_func;
public:
    ZoomFactorChangedHandler(std::function<HRESULT(ICoreWebView2Controller*, IUnknown*)> func) : m_func(func) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_ICoreWebView2ZoomFactorChangedEventHandler) {
            *ppvObject = static_cast<ICoreWebView2ZoomFactorChangedEventHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_ref);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2Controller* sender, IUnknown* args) override {
        return m_func ? m_func(sender, args) : S_OK;
    }
};

std::wstring EscapeJsonString(const std::wstring& wide) {
    if (wide.empty()) return L"";
    std::wstringstream ss;
    for (wchar_t ch : wide) {
        switch (ch) {
            case L'\\': ss << L"\\\\"; break;
            case L'\"': ss << L"\\\""; break;
            case L'\b': ss << L"\\b"; break;
            case L'\f': ss << L"\\f"; break;
            case L'\n': ss << L"\\n"; break;
            case L'\r': ss << L"\\r"; break;
            case L'\t': ss << L"\\t"; break;
            default:
                if (ch < 0x20) {
                    wchar_t buf[7];
                    swprintf_s(buf, L"\\u%04x", (unsigned int)ch);
                    ss << buf;
                } else {
                    ss << ch;
                }
                break;
        }
    }
    return ss.str();
}

std::wstring EscapeJsonString(const std::string& utf8Str) {
    if (utf8Str.empty()) return L"";
    return EscapeJsonString(BiDiEngine::Utf8ToWide(utf8Str));
}

typedef HRESULT (STDAPICALLTYPE *CreateCoreWebView2EnvironmentWithOptionsFn)(
    PCWSTR browserExecutableFolder,
    PCWSTR userDataFolder,
    ICoreWebView2EnvironmentOptions* environmentOptions,
    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* environment_created_handler
);

std::wstring GetModuleDir() {
    wchar_t path[MAX_PATH] = { 0 };
    HMODULE hMod = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCWSTR)&GetModuleDir, &hMod);
    GetModuleFileNameW(hMod, path, MAX_PATH);
    PathRemoveFileSpecW(path);
    return path;
}

} // namespace

WebView2Viewer::WebView2Viewer() = default;

WebView2Viewer::~WebView2Viewer() {
    Close();
}

bool WebView2Viewer::IsAvailable() {
    std::wstring modDir = GetModuleDir();
    std::wstring loaderPath = modDir + L"\\WebView2Loader.dll";
    if (PathFileExistsW(loaderPath.c_str())) return true;
    if (PathFileExistsW(L"WebView2Loader.dll")) return true;
    return false;
}

void WebView2Viewer::Close() {
    if (m_pWebView) {
        m_pWebView->Release();
        m_pWebView = nullptr;
    }
    if (m_pController) {
        m_pController->Close();
        m_pController->Release();
        m_pController = nullptr;
    }
    if (m_pEnvironment) {
        m_pEnvironment->Release();
        m_pEnvironment = nullptr;
    }
    if (m_hLoaderModule) {
        FreeLibrary(m_hLoaderModule);
        m_hLoaderModule = nullptr;
    }
    m_isInitialized = false;
    m_isInitializing = false;
    m_pageReady = false;
}

bool WebView2Viewer::Initialize(HWND hParentWnd, const std::wstring& userDataDir) {
    if (m_isInitialized || m_isInitializing) return true;
    m_hParent = hParentWnd;
    m_isInitializing = true;
    m_pageReady = false;

    std::wstring modDir = GetModuleDir();
    std::wstring loaderPath = modDir + L"\\WebView2Loader.dll";
    m_hLoaderModule = LoadLibraryW(loaderPath.c_str());
    if (!m_hLoaderModule) {
        m_hLoaderModule = LoadLibraryW(L"WebView2Loader.dll");
    }
    if (!m_hLoaderModule) {
        NppLog("WebView2 Initialize: WebView2Loader.dll not found (tried %ls)", loaderPath.c_str());
        m_isInitializing = false;
        NotifyInitFailed();
        return false;
    }

    auto pfnCreate = (CreateCoreWebView2EnvironmentWithOptionsFn)GetProcAddress(
        m_hLoaderModule, "CreateCoreWebView2EnvironmentWithOptions"
    );
    if (!pfnCreate) {
        NppLog("WebView2 Initialize: CreateCoreWebView2EnvironmentWithOptions export missing");
        m_isInitializing = false;
        NotifyInitFailed();
        return false;
    }

    // Prepare user data directory in %TEMP%\NppMarkdownNext_WebView2
    std::wstring targetUserData = userDataDir;
    if (targetUserData.empty()) {
        wchar_t tempPath[MAX_PATH] = { 0 };
        GetTempPathW(MAX_PATH, tempPath);
        targetUserData = std::wstring(tempPath) + L"NppMarkdownNext_WebView2";
    }
    CreateDirectoryW(targetUserData.c_str(), nullptr);

    auto* envHandler = new EnvironmentCompletedHandler(
        [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
            OnEnvironmentCreated(result, env);
            return S_OK;
        }
    );

    HRESULT hr = pfnCreate(nullptr, targetUserData.c_str(), nullptr, envHandler);
    envHandler->Release();
    if (FAILED(hr)) {
        NppLog("WebView2 Initialize: CreateEnvironment hr=0x%08X", (unsigned)hr);
        m_isInitializing = false;
        NotifyInitFailed();
        return false;
    }

    NppLog("WebView2 Initialize: started, userData=%ls", targetUserData.c_str());
    return true;
}

void WebView2Viewer::NotifyInitFailed() {
    if (m_initFailedCallback) {
        m_initFailedCallback();
    }
}

void WebView2Viewer::OnEnvironmentCreated(HRESULT result, ICoreWebView2Environment* env) {
    NppLog("OnEnvironmentCreated: hr=0x%08X env=%p", (unsigned)result, (void*)env);
    if (FAILED(result) || !env) {
        m_isInitializing = false;
        NotifyInitFailed();
        return;
    }

    m_pEnvironment = env;
    m_pEnvironment->AddRef();

    auto* controllerHandler = new ControllerCompletedHandler(
        [this](HRESULT res, ICoreWebView2Controller* controller) -> HRESULT {
            OnControllerCreated(res, controller);
            return S_OK;
        }
    );

    m_pEnvironment->CreateCoreWebView2Controller(m_hParent, controllerHandler);
    controllerHandler->Release();
}

void WebView2Viewer::OnControllerCreated(HRESULT result, ICoreWebView2Controller* controller) {
    m_isInitializing = false;
    NppLog("OnControllerCreated: hr=0x%08X controller=%p", (unsigned)result, (void*)controller);
    if (FAILED(result) || !controller) {
        NotifyInitFailed();
        return;
    }

    m_pController = controller;
    m_pController->AddRef();

    // Immediately resize to fit parent window bounds
    RECT rc = { 0, 0, 450, 700 };
    if (m_hParent && IsWindow(m_hParent)) {
        GetClientRect(m_hParent, &rc);
    }
    m_pController->put_Bounds(rc);
    m_pController->put_IsVisible(TRUE);

    HRESULT coreHr = m_pController->get_CoreWebView2(&m_pWebView);
    NppLog("OnControllerCreated: get_CoreWebView2 hr=0x%08X web=%p", (unsigned)coreHr, (void*)m_pWebView);
    if (SUCCEEDED(coreHr) && m_pWebView) {
        // Disable default browser context menus so our clean reader experience takes over
        ICoreWebView2Settings* settings = nullptr;
        if (SUCCEEDED(m_pWebView->get_Settings(&settings)) && settings) {
            settings->put_AreDefaultContextMenusEnabled(FALSE);
            settings->put_IsScriptEnabled(TRUE);
            settings->put_IsStatusBarEnabled(FALSE);
            settings->put_AreDevToolsEnabled(FALSE);
            settings->Release();
        }

        // Register WebMessageReceived handler
        EventRegistrationToken msgToken;
        auto* msgHandler = new WebMessageReceivedHandler(
            [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                LPWSTR msgRaw = nullptr;
                if (SUCCEEDED(args->TryGetWebMessageAsString(&msgRaw)) && msgRaw) {
                    OnWebMessageReceived(msgRaw);
                    CoTaskMemFree(msgRaw);
                }
                return S_OK;
            }
        );
        m_pWebView->add_WebMessageReceived(msgHandler, &msgToken);
        msgHandler->Release();

        // Register NavigationStarting handler to open external links in system browser
        EventRegistrationToken navToken;
        auto* navHandler = new NavigationStartingHandler(
            [this](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
                LPWSTR uriRaw = nullptr;
                if (SUCCEEDED(args->get_Uri(&uriRaw)) && uriRaw) {
                    std::wstring uri(uriRaw);
                    CoTaskMemFree(uriRaw);

                    if (uri.find(L"http://") == 0 || uri.find(L"https://") == 0) {
                        args->put_Cancel(TRUE);
                        ShellExecuteW(nullptr, L"open", uri.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
                    }
                }
                return S_OK;
            }
        );
        m_pWebView->add_NavigationStarting(navHandler, &navToken);
        navHandler->Release();

        // Register ZoomFactorChanged handler
        EventRegistrationToken zoomToken;
        auto* zoomHandler = new ZoomFactorChangedHandler(
            [this](ICoreWebView2Controller* sender, IUnknown*) -> HRESULT {
                if (sender) {
                    double factor = 1.0;
                    if (SUCCEEDED(sender->get_ZoomFactor(&factor))) {
                        m_zoom = static_cast<float>(factor);
                        if (m_zoomChangeCallback) {
                            m_zoomChangeCallback(m_zoom);
                        }
                    }
                }
                return S_OK;
            }
        );
        m_pController->add_ZoomFactorChanged(zoomHandler, &zoomToken);
        zoomHandler->Release();

        m_isInitialized = true;
        SetZoom(m_zoom);

        // Navigate to the lightweight Kelk reader shell HTML (~300 KB)
        std::wstring wideShell = BiDiEngine::Utf8ToWide(s_kelkReaderShellHtml);
        HRESULT navHr = m_pWebView->NavigateToString(wideShell.c_str());
        NppLog("OnControllerCreated: NavigateToString(%u bytes) hr=0x%08X",
               (unsigned)wideShell.size() * 2, (unsigned)navHr);
    } else {
        NotifyInitFailed();
    }
}

void WebView2Viewer::Resize(int x, int y, int width, int height) {
    if (m_pController) {
        RECT bounds = { x, y, x + width, y + height };
        m_pController->put_Bounds(bounds);
    }
}

void WebView2Viewer::SetVisible(bool visible) {
    if (m_pController) {
        m_pController->put_IsVisible(visible ? TRUE : FALSE);
    }
}

void WebView2Viewer::SetZoom(float zoomFactor) {
    m_zoom = zoomFactor;
    if (m_pController) {
        m_pController->put_ZoomFactor(static_cast<double>(zoomFactor));
    }
}

void WebView2Viewer::SetTheme(bool isDark) {
    m_currentDark = isDark;
    if (m_pageReady && m_pWebView) {
        std::wstring msg = L"{\"type\":\"setTheme\",\"isDark\":" + std::wstring(isDark ? L"true" : L"false") + L"}";
        m_pWebView->PostWebMessageAsJson(msg.c_str());
    }
}

void WebView2Viewer::SetBiDi(bool isBiDi) {
    m_currentBiDi = isBiDi;
    if (m_pageReady && m_pWebView) {
        std::wstring msg = L"{\"type\":\"setBiDi\",\"isBiDi\":" + std::wstring(isBiDi ? L"true" : L"false") + L"}";
        m_pWebView->PostWebMessageAsJson(msg.c_str());
    }
}

void WebView2Viewer::ScrollToLine(int line) {
    if (m_pageReady && m_pWebView && line >= 0) {
        std::wstring msg = L"{\"type\":\"scrollToLine\",\"line\":" + std::to_wstring(line) + L"}";
        m_pWebView->PostWebMessageAsJson(msg.c_str());
    }
}

void WebView2Viewer::ExecuteScript(const std::wstring& script) {
    if (m_isInitialized && m_pWebView) {
        m_pWebView->ExecuteScript(script.c_str(), nullptr);
    }
}

bool WebView2Viewer::UpdateMarkdown(const std::string& markdownUtf8, const std::wstring& title, bool isDark, bool isBiDi, int caretLine) {
    m_currentDark = isDark;
    m_currentBiDi = isBiDi;

    if (!m_isInitialized || !m_pWebView || !m_pageReady) {
        NppLog("UpdateMarkdown: queued pending (init=%d web=%d ready=%d bytes=%u)",
               (int)m_isInitialized, (int)(m_pWebView != nullptr), (int)m_pageReady, (unsigned)markdownUtf8.size());
        m_pendingRender.valid = true;
        m_pendingRender.markdown = markdownUtf8;
        m_pendingRender.title = title;
        m_pendingRender.isDark = isDark;
        m_pendingRender.isBiDi = isBiDi;
        m_pendingRender.caretLine = caretLine;
        return false;
    }

    std::wstringstream json;
    json << L"{\"type\":\"render\""
         << L",\"markdown\":\"" << EscapeJsonString(markdownUtf8) << L"\""
         << L",\"title\":\"" << EscapeJsonString(title) << L"\""
         << L",\"isDark\":" << (isDark ? L"true" : L"false")
         << L",\"isBiDi\":" << (isBiDi ? L"true" : L"false")
         << L",\"caretLine\":" << caretLine
         << L"}";

    HRESULT hr = m_pWebView->PostWebMessageAsJson(json.str().c_str());
    NppLog("UpdateMarkdown: PostWebMessageAsJson hr=0x%08X (bytes=%u)", (unsigned)hr, (unsigned)markdownUtf8.size());
    return SUCCEEDED(hr);
}

void WebView2Viewer::OnWebMessageReceived(const std::wstring& message) {
    std::string msgUtf8 = BiDiEngine::WideToUtf8(message);
    NppLog("OnWebMessageReceived: %s", msgUtf8.c_str());

    const std::wstring prefixError = L"jsError:";
    if (message.find(prefixError) == 0) {
        std::string errUtf8 = BiDiEngine::WideToUtf8(message.substr(prefixError.length()));
        NppLog("JavaScript Exception: %s", errUtf8.c_str());
        return;
    }

    if (message == L"pageLoaded") {
        m_pageReady = true;
        NppLog("OnWebMessageReceived: pageLoaded");
        if (m_pageReadyCallback) {
            m_pageReadyCallback();
        } else if (m_pendingRender.valid) {
            UpdateMarkdown(m_pendingRender.markdown, m_pendingRender.title,
                           m_pendingRender.isDark, m_pendingRender.isBiDi, m_pendingRender.caretLine);
            m_pendingRender.valid = false;
        }
        return;
    }

    const std::wstring prefixCheckbox = L"toggleCheckbox:";
    if (message.find(prefixCheckbox) == 0) {
        int line = _wtoi(message.substr(prefixCheckbox.length()).c_str());
        if (m_checkboxCallback) {
            m_checkboxCallback(line);
        }
        return;
    }

    const std::wstring prefixJump = L"jumpToLine:";
    if (message.find(prefixJump) == 0) {
        int line = _wtoi(message.substr(prefixJump.length()).c_str());
        if (m_jumpToLineCallback) {
            m_jumpToLineCallback(line);
        }
        return;
    }

    if (message == L"toggleSync") {
        if (m_syncCallback) m_syncCallback();
        return;
    }

    if (message == L"toggleDark") {
        if (m_themeCallback) m_themeCallback();
        return;
    }

    if (message == L"toggleBiDi") {
        if (m_bidiCallback) m_bidiCallback();
        return;
    }
}
