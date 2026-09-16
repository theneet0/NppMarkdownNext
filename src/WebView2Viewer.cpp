#include "../include/WebView2Viewer.h"
#include "../include/BiDiEngine.h"
#include <initguid.h>
#include <WebView2.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <sstream>

typedef HRESULT (STDAPICALLTYPE *CreateCoreWebView2EnvironmentWithOptionsFn)(
    PCWSTR browserExecutableFolder,
    PCWSTR userDataFolder,
    ICoreWebView2EnvironmentOptions* environmentOptions,
    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* environment_created_handler
);

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
    // Check if WebView2Loader.dll can be found
    std::wstring modDir = GetModuleDir();
    std::wstring loaderPath = modDir + L"\\WebView2Loader.dll";
    if (PathFileExistsW(loaderPath.c_str())) return true;
    if (PathFileExistsW(L"WebView2Loader.dll")) return true;
    return false;
}

bool WebView2Viewer::Initialize(HWND hParentWnd, const std::wstring& userDataDir) {
    if (m_isInitialized || m_isInitializing) return true;
    m_hParent = hParentWnd;
    m_isInitializing = true;

    std::wstring modDir = GetModuleDir();
    std::wstring loaderPath = modDir + L"\\WebView2Loader.dll";
    m_hLoaderModule = LoadLibraryW(loaderPath.c_str());
    if (!m_hLoaderModule) {
        m_hLoaderModule = LoadLibraryW(L"WebView2Loader.dll");
    }
    if (!m_hLoaderModule) {
        m_isInitializing = false;
        return false;
    }

    auto pfnCreate = (CreateCoreWebView2EnvironmentWithOptionsFn)GetProcAddress(
        m_hLoaderModule, "CreateCoreWebView2EnvironmentWithOptions"
    );
    if (!pfnCreate) {
        m_isInitializing = false;
        return false;
    }

    // Prepare user data directory in %TEMP%\NppMarkdownPanel_WebView2
    std::wstring targetUserData = userDataDir;
    if (targetUserData.empty()) {
        wchar_t tempPath[MAX_PATH] = { 0 };
        GetTempPathW(MAX_PATH, tempPath);
        targetUserData = std::wstring(tempPath) + L"NppMarkdownPanel_WebView2";
    }
    CreateDirectoryW(targetUserData.c_str(), nullptr);

    auto* envHandler = new EnvironmentCompletedHandler(
        [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
            OnEnvironmentCreated(result, env);
            return S_OK;
        }
    );

    HRESULT hr = pfnCreate(nullptr, targetUserData.c_str(), nullptr, envHandler);
    if (FAILED(hr)) {
        envHandler->Release();
        m_isInitializing = false;
        return false;
    }

    return true;
}

void WebView2Viewer::OnEnvironmentCreated(HRESULT result, ICoreWebView2Environment* env) {
    if (FAILED(result) || !env) {
        m_isInitializing = false;
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
}

void WebView2Viewer::OnControllerCreated(HRESULT result, ICoreWebView2Controller* controller) {
    m_isInitializing = false;
    if (FAILED(result) || !controller) {
        return;
    }

    m_pController = controller;
    m_pController->AddRef();

    // Resize to fit parent client rect
    RECT rc;
    GetClientRect(m_hParent, &rc);
    m_pController->put_Bounds(rc);
    m_pController->put_IsVisible(TRUE);

    if (SUCCEEDED(m_pController->get_CoreWebView2(&m_pWebView)) && m_pWebView) {
        // Disable default browser context menu so our modern custom HTML context menu takes over
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

        // Register NavigationStarting handler to open external links in default browser
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

        // Register ZoomFactorChanged handler to sync native browser zoom (e.g. Ctrl+Wheel / pinch)
        EventRegistrationToken zoomToken;
        auto* zoomHandler = new ZoomFactorChangedHandler(
            [this](ICoreWebView2Controller* sender, IUnknown* /*args*/) -> HRESULT {
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

        m_isInitialized = true;

        // Apply pending settings
        SetZoom(m_zoom);

        // Navigate to pending HTML if present
        if (!m_pendingHtml.empty()) {
            SetHtmlContent(m_pendingHtml);
            m_pendingHtml.clear();
        }
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

namespace {
std::wstring EscapeJsonWide(const std::string& utf8Str) {
    if (utf8Str.empty()) return L"";
    std::wstring wide = BiDiEngine::Utf8ToWide(utf8Str);

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
} // namespace

void WebView2Viewer::SetHtmlContent(const std::string& htmlUtf8) {
    if (!m_isInitialized || !m_pWebView) {
        m_pendingHtml = htmlUtf8;
        return;
    }

    m_pageReady = false;
    std::wstring wideHtml = BiDiEngine::Utf8ToWide(htmlUtf8);
    if (!wideHtml.empty()) {
        m_pWebView->NavigateToString(wideHtml.c_str());
    }
}

bool WebView2Viewer::UpdateContent(const std::string& bodyHtml, const std::string& tocHtml, const std::string& title, bool isDark) {
    if (!m_isInitialized || !m_pWebView || !m_pageReady) {
        return false;
    }

    std::wstringstream json;
    json << L"{\"type\":\"updateContent\",\"body\":\"" << EscapeJsonWide(bodyHtml)
         << L"\",\"toc\":\"" << EscapeJsonWide(tocHtml)
         << L"\",\"title\":\"" << EscapeJsonWide(title)
         << L"\",\"isDark\":" << (isDark ? L"true" : L"false") << L"}";

    HRESULT hr = m_pWebView->PostWebMessageAsJson(json.str().c_str());
    return SUCCEEDED(hr);
}

void WebView2Viewer::ExecuteScript(const std::wstring& script) {
    if (m_isInitialized && m_pWebView) {
        m_pWebView->ExecuteScript(script.c_str(), nullptr);
    }
}

void WebView2Viewer::SetDarkMode(bool isDark) {
    m_darkMode = isDark;
    if (m_isInitialized) {
        std::wstring script = isDark ? L"document.body.classList.add('dark'); if (window.onThemeChanged) window.onThemeChanged();"
                                     : L"document.body.classList.remove('dark'); if (window.onThemeChanged) window.onThemeChanged();";
        ExecuteScript(script);
    }
}

void WebView2Viewer::SetZoom(float zoomFactor) {
    m_zoom = zoomFactor;
    if (m_pController) {
        m_pController->put_ZoomFactor(static_cast<double>(zoomFactor));
    }
}

void WebView2Viewer::ScrollToLine(int line) {
    if (m_isInitialized) {
        std::wstringstream ss;
        ss << L"if (window.scrollToSourceLine) { window.scrollToSourceLine(" << line << L"); }";
        ExecuteScript(ss.str());
    }
}

void WebView2Viewer::PrintToPdf(const std::wstring& pdfPath) {
    if (m_isInitialized) {
        ExecuteScript(L"window.print();");
    }
}

void WebView2Viewer::OnWebMessageReceived(const std::wstring& message) {
    if (message == L"pageLoaded") {
        m_pageReady = true;
        return;
    }

    const std::wstring prefixCheckbox = L"toggleCheckbox:";
    if (message.find(prefixCheckbox) == 0) {
        std::wstring lineStr = message.substr(prefixCheckbox.length());
        int line = _wtoi(lineStr.c_str());
        if (m_checkboxCallback) {
            m_checkboxCallback(line);
        }
        return;
    }

    if (message == L"toggleSync") {
        if (m_syncCallback) {
            m_syncCallback();
        }
        return;
    }

    if (message == L"toggleTheme") {
        if (m_themeCallback) {
            m_themeCallback();
        }
        return;
    }

    const std::wstring prefixToc = L"tocStateChanged:";
    if (message.find(prefixToc) == 0) {
        std::wstring stateStr = message.substr(prefixToc.length());
        bool isOpen = (stateStr == L"true");
        if (m_tocCallback) {
            m_tocCallback(isOpen);
        }
        return;
    }

    if (message == L"zoomIn") {
        if (m_zoomInCallback) {
            m_zoomInCallback();
        }
        return;
    }

    if (message == L"zoomOut") {
        if (m_zoomOutCallback) {
            m_zoomOutCallback();
        }
        return;
    }

    if (message == L"zoomReset") {
        if (m_zoomResetCallback) {
            m_zoomResetCallback();
        }
        return;
    }

    if (message == L"saveAsHtml") {
        if (m_saveAsHtmlCallback) {
            m_saveAsHtmlCallback();
        }
        return;
    }

    if (message == L"toggleBiDi") {
        if (m_bidiCallback) {
            m_bidiCallback();
        }
        return;
    }

    const std::wstring prefixZoomChange = L"zoomChange:";
    if (message.find(prefixZoomChange) == 0) {
        std::wstring factorStr = message.substr(prefixZoomChange.length());
        float factor = static_cast<float>(_wtof(factorStr.c_str()));
        if (factor >= 0.3f && factor <= 3.0f) {
            SetZoom(factor);
            if (m_zoomChangeCallback) {
                m_zoomChangeCallback(factor);
            }
        }
        return;
    }
}

void WebView2Viewer::Close() {
    m_isInitialized = false;
    m_isInitializing = false;
    m_pageReady = false;

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
}
