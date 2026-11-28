#include "../include/WebView2Viewer.h"
#include "../include/BiDiEngine.h"
#include "../include/NppLog.h"
#include "../include/LinkPolicy.h"
#include "../include/ReaderText.h"
#include "../include/Config.h"
#include "KelkBundle.inc"
#include "ReaderThemes.inc"
#include <initguid.h>
#include <WebView2.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <sstream>
#include <utility>
#include <limits>
#include <array>
#include <climits>

namespace {

void MatchSystemDpi(HWND parent, ICoreWebView2* view, ICoreWebView2Controller* controller) {
    const HMODULE user32 = GetModuleHandleW(L"user32.dll");
    static const auto getContext = reinterpret_cast<HANDLE (WINAPI*)(HWND)>(GetProcAddress(user32, "GetWindowDpiAwarenessContext"));
    static const auto sameContext = reinterpret_cast<BOOL (WINAPI*)(HANDLE, HANDLE)>(GetProcAddress(user32, "AreDpiAwarenessContextsEqual"));
    static const auto windowDpi = reinterpret_cast<UINT (WINAPI*)(HWND)>(GetProcAddress(user32, "GetDpiForWindow"));
    static const auto processDpi = reinterpret_cast<UINT (WINAPI*)(HANDLE)>(GetProcAddress(user32, "GetSystemDpiForProcess"));
    if (!getContext || !sameContext || !windowDpi || !processDpi ||
        !sameContext(getContext(parent), DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) return;

    UINT32 pid = 0;
    if (FAILED(view->get_BrowserProcessId(&pid)) || !pid) return;
    HANDLE browser = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!browser) return;
    const UINT browserDpi = processDpi(browser);
    CloseHandle(browser);
    const UINT hostDpi = windowDpi(parent);
    if (!hostDpi || !browserDpi || browserDpi == hostDpi) return;

    // A browser started after a scale change can have a different system DPI from the still-running host.
    ICoreWebView2Controller3* dpiController = nullptr;
    if (FAILED(controller->QueryInterface(IID_ICoreWebView2Controller3, reinterpret_cast<void**>(&dpiController)))) return;
    if (SUCCEEDED(dpiController->put_ShouldDetectMonitorScaleChanges(FALSE))) {
        if (FAILED(dpiController->put_RasterizationScale(hostDpi / 96.0)))
            dpiController->put_ShouldDetectMonitorScaleChanges(TRUE);
        else
            NppLog("WebView2 rasterization matched to panel DPI %u (browser DPI %u)", hostDpi, browserDpi);
    }
    dpiController->Release();
}

bool ParseCheckboxRequest(const std::wstring& value, CheckboxRequest& request) {
    std::array<ULONGLONG, 5> fields = {};
    size_t offset = 0;
    for (size_t index = 0; index < fields.size(); ++index) {
        size_t end = value.find(L':', offset);
        if ((index + 1 == fields.size()) != (end == std::wstring::npos)) return false;
        if (end == std::wstring::npos) end = value.size();
        if (end == offset) return false;
        for (size_t i = offset; i < end; ++i) {
            if (value[i] < L'0' || value[i] > L'9') return false;
            unsigned digit = value[i] - L'0';
            if (fields[index] > ((std::numeric_limits<ULONGLONG>::max)() - digit) / 10) return false;
            fields[index] = fields[index] * 10 + digit;
        }
        if (index == 0) request.bufferId = value.substr(offset, end - offset);
        offset = end + 1;
    }
    if (!fields[0] || !fields[1] || fields[2] > INT_MAX || fields[3] > INT_MAX || fields[4] > 2) return false;
    request.renderRevision = fields[1];
    request.line = static_cast<int>(fields[2]);
    request.markerColumn = static_cast<int>(fields[3]);
    request.expectedState = fields[4] == 0 ? ' ' : fields[4] == 1 ? 'x' : 'X';
    return true;
}

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

class AcceleratorKeyPressedHandler : public ICoreWebView2AcceleratorKeyPressedEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2Controller*, ICoreWebView2AcceleratorKeyPressedEventArgs*)> m_func;
public:
    explicit AcceleratorKeyPressedHandler(std::function<HRESULT(ICoreWebView2Controller*, ICoreWebView2AcceleratorKeyPressedEventArgs*)> func) : m_func(std::move(func)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) return E_POINTER;
        *object = nullptr;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2AcceleratorKeyPressedEventHandler) return E_NOINTERFACE;
        *object = static_cast<ICoreWebView2AcceleratorKeyPressedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG ref = InterlockedDecrement(&m_ref); if (!ref) delete this; return ref; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2Controller* sender, ICoreWebView2AcceleratorKeyPressedEventArgs* args) override {
        return m_func ? m_func(sender, args) : S_OK;
    }
};

class ProcessFailedHandler : public ICoreWebView2ProcessFailedEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2ProcessFailedEventArgs*)> m_func;
public:
    explicit ProcessFailedHandler(std::function<HRESULT(ICoreWebView2ProcessFailedEventArgs*)> func) : m_func(std::move(func)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) return E_POINTER;
        *object = nullptr;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2ProcessFailedEventHandler) return E_NOINTERFACE;
        *object = static_cast<ICoreWebView2ProcessFailedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG ref = InterlockedDecrement(&m_ref); if (!ref) delete this; return ref; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2*, ICoreWebView2ProcessFailedEventArgs* args) override { return m_func(args); }
};

class NavigationCompletedHandler : public ICoreWebView2NavigationCompletedEventHandler {
    LONG m_ref = 1;
    std::function<HRESULT(ICoreWebView2NavigationCompletedEventArgs*)> m_func;
public:
    explicit NavigationCompletedHandler(std::function<HRESULT(ICoreWebView2NavigationCompletedEventArgs*)> func) : m_func(std::move(func)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) return E_POINTER;
        *object = nullptr;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2NavigationCompletedEventHandler) return E_NOINTERFACE;
        *object = static_cast<ICoreWebView2NavigationCompletedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG ref = InterlockedDecrement(&m_ref); if (!ref) delete this; return ref; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs* args) override { return m_func(args); }
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
    if (m_lifetime) m_lifetime->active = false;
    if (!g_isDllDetaching) Close();
}

bool WebView2Viewer::IsAvailable() {
    std::wstring modDir = GetModuleDir();
    std::wstring loaderPath = modDir + L"\\WebView2Loader.dll";
    if (PathFileExistsW(loaderPath.c_str())) return true;
    if (PathFileExistsW(L"WebView2Loader.dll")) return true;
    return false;
}

void WebView2Viewer::Close() {
    if (m_lifetime) m_lifetime->active = false;
    m_lifetime.reset();
    m_pendingRender = {};
    m_renderedBufferId.clear();
    m_documentFolder.clear(); // the mapping belongs to the closed webview
    ++m_renderSequence;
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
    m_lifetime = std::make_shared<ViewerLifetime>();

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

    auto* envHandler = new EnvironmentCompletedHandler(MakeEnvironmentCallback());

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
    m_isInitialized = false;
    m_isInitializing = false;
    m_pageReady = false;
    if (m_initFailedCallback) {
        m_initFailedCallback();
    }
}

std::function<HRESULT(HRESULT, ICoreWebView2Environment*)> WebView2Viewer::MakeEnvironmentCallback() {
    return [this, lifetime = m_lifetime](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
        if (!lifetime || !lifetime->active) return S_OK;
        OnEnvironmentCreated(result, env);
        return S_OK;
    };
}

std::function<HRESULT(HRESULT, ICoreWebView2Controller*)> WebView2Viewer::MakeControllerCallback() {
    return [this, lifetime = m_lifetime](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
        if (!lifetime || !lifetime->active) return S_OK;
        OnControllerCreated(result, controller);
        return S_OK;
    };
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

    auto* controllerHandler = new ControllerCompletedHandler(MakeControllerCallback());
    HRESULT hr = m_pEnvironment->CreateCoreWebView2Controller(m_hParent, controllerHandler);
    controllerHandler->Release();
    if (FAILED(hr)) {
        NppLog("CreateCoreWebView2Controller failed hr=0x%08X", (unsigned)hr);
        NotifyInitFailed();
    }
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
    m_pController->put_IsVisible(m_visible ? TRUE : FALSE);

    HRESULT coreHr = m_pController->get_CoreWebView2(&m_pWebView);
    NppLog("OnControllerCreated: get_CoreWebView2 hr=0x%08X web=%p", (unsigned)coreHr, (void*)m_pWebView);
    if (SUCCEEDED(coreHr) && m_pWebView) {
        MatchSystemDpi(m_hParent, m_pWebView, m_pController);
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
            [this, lifetime = m_lifetime](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                if (!lifetime->active) return S_OK;
                LPWSTR msgRaw = nullptr;
                if (SUCCEEDED(args->TryGetWebMessageAsString(&msgRaw)) && msgRaw) {
                    OnWebMessageReceived(msgRaw);
                    CoTaskMemFree(msgRaw);
                }
                return S_OK;
            }
        );
        HRESULT messageHr = m_pWebView->add_WebMessageReceived(msgHandler, &msgToken);
        msgHandler->Release();
        if (FAILED(messageHr)) {
            NppLog("add_WebMessageReceived failed hr=0x%08X", (unsigned)messageHr);
            NotifyInitFailed();
            return;
        }

        // Register NavigationStarting handler to open external links in system browser
        EventRegistrationToken navToken;
        auto* navHandler = new NavigationStartingHandler(
            [this, lifetime = m_lifetime](ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
                if (!lifetime->active) return S_OK;
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
        HRESULT startingHr = m_pWebView->add_NavigationStarting(navHandler, &navToken);
        navHandler->Release();
        if (FAILED(startingHr)) {
            NppLog("add_NavigationStarting failed hr=0x%08X", (unsigned)startingHr);
            NotifyInitFailed();
            return;
        }

        // Register ZoomFactorChanged handler
        EventRegistrationToken zoomToken;
        auto* zoomHandler = new ZoomFactorChangedHandler(
            [this, lifetime = m_lifetime](ICoreWebView2Controller* sender, IUnknown*) -> HRESULT {
                if (!lifetime->active) return S_OK;
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

        // Forward host-level shortcuts (Ctrl+W, Ctrl+F4, Ctrl+S, Ctrl+Shift+M) back to Notepad++
        EventRegistrationToken accelToken;
        auto* accelHandler = new AcceleratorKeyPressedHandler(
            [this, lifetime = m_lifetime](ICoreWebView2Controller*, ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT {
                if (!lifetime->active || !args) return S_OK;
                COREWEBVIEW2_KEY_EVENT_KIND kind = COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN;
                if (FAILED(args->get_KeyEventKind(&kind)) ||
                    (kind != COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN && kind != COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)) {
                    return S_OK;
                }
                COREWEBVIEW2_PHYSICAL_KEY_STATUS status = {};
                const bool isRepeat = SUCCEEDED(args->get_PhysicalKeyStatus(&status)) && status.WasKeyDown;
                UINT vk = 0;
                if (FAILED(args->get_VirtualKey(&vk))) return S_OK;
                const bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
                const bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                const bool alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
                if (HandleAcceleratorKey(vk, ctrl, shift, alt, isRepeat)) {
                    args->put_Handled(TRUE);
                }
                return S_OK;
            }
        );
        m_pController->add_AcceleratorKeyPressed(accelHandler, &accelToken);
        accelHandler->Release();

        EventRegistrationToken processToken;
        auto* processHandler = new ProcessFailedHandler([this, lifetime = m_lifetime](ICoreWebView2ProcessFailedEventArgs* args) {
            if (!lifetime->active) return S_OK;
            COREWEBVIEW2_PROCESS_FAILED_KIND kind;
            if (SUCCEEDED(args->get_ProcessFailedKind(&kind)) &&
                (kind == COREWEBVIEW2_PROCESS_FAILED_KIND_BROWSER_PROCESS_EXITED ||
                 kind == COREWEBVIEW2_PROCESS_FAILED_KIND_RENDER_PROCESS_EXITED ||
                 kind == COREWEBVIEW2_PROCESS_FAILED_KIND_RENDER_PROCESS_UNRESPONSIVE)) {
                NppLog("WebView2 process failure kind=%u", (unsigned)kind);
                NotifyInitFailed();
            }
            return S_OK;
        });
        HRESULT processHr = m_pWebView->add_ProcessFailed(processHandler, &processToken);
        processHandler->Release();
        if (FAILED(processHr)) {
            NppLog("add_ProcessFailed failed hr=0x%08X", (unsigned)processHr);
            NotifyInitFailed();
            return;
        }

        EventRegistrationToken completedToken;
        auto* completedHandler = new NavigationCompletedHandler([this, lifetime = m_lifetime](ICoreWebView2NavigationCompletedEventArgs* args) {
            if (!lifetime->active) return S_OK;
            BOOL success = FALSE;
            COREWEBVIEW2_WEB_ERROR_STATUS error = COREWEBVIEW2_WEB_ERROR_STATUS_UNKNOWN;
            args->get_WebErrorStatus(&error);
            if ((FAILED(args->get_IsSuccess(&success)) || !success) &&
                error != COREWEBVIEW2_WEB_ERROR_STATUS_OPERATION_CANCELED) NotifyInitFailed();
            return S_OK;
        });
        HRESULT completedHr = m_pWebView->add_NavigationCompleted(completedHandler, &completedToken);
        completedHandler->Release();
        if (FAILED(completedHr)) {
            NotifyInitFailed();
            return;
        }

        m_isInitialized = true;
        SetZoom(m_zoom);

        // Navigate to the lightweight Kelk reader shell HTML (~300 KB)
        std::wstring wideShell = BiDiEngine::Utf8ToWide(ReaderShellHtml());
        HRESULT navHr = m_pWebView->NavigateToString(wideShell.c_str());
        NppLog("OnControllerCreated: NavigateToString(%u bytes) hr=0x%08X",
               (unsigned)wideShell.size() * 2, (unsigned)navHr);
        if (FAILED(navHr)) NotifyInitFailed();
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
    m_visible = visible;
    if (m_pController) {
        m_pController->put_IsVisible(visible ? TRUE : FALSE);
    }
}

void WebView2Viewer::NotifyParentWindowPositionChanged() {
    if (m_pController) {
        m_pController->NotifyParentWindowPositionChanged();
    }
}

void WebView2Viewer::Focus() {
    if (m_pController) {
        m_pController->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
    }
}

void WebView2Viewer::SetZoom(float zoomFactor) {
    m_zoom = zoomFactor;
    if (m_pController) {
        m_pController->put_ZoomFactor(static_cast<double>(zoomFactor));
    }
}

void WebView2Viewer::SetTheme(bool isDark, const std::wstring& themeId) {
    m_currentDark = isDark;
    m_currentThemeId = themeId;
    PostJson(L"{\"type\":\"setTheme\",\"isDark\":" + std::wstring(isDark ? L"true" : L"false") +
             L",\"themeId\":\"" + EscapeJsonString(themeId) + L"\"}");
}

void WebView2Viewer::SetBiDi(bool isBiDi) {
    m_currentBiDi = isBiDi;
    PostJson(L"{\"type\":\"setBiDi\",\"isBiDi\":" + std::wstring(isBiDi ? L"true" : L"false") + L"}");
}

namespace {
class FindStartedHandler : public ICoreWebView2FindStartCompletedHandler {
    LONG m_ref = 1;
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) return E_POINTER;
        *object = nullptr;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2FindStartCompletedHandler) return E_NOINTERFACE;
        *object = static_cast<ICoreWebView2FindStartCompletedHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_ref); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG ref = InterlockedDecrement(&m_ref); if (!ref) delete this; return ref; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT errorCode) override {
        if (FAILED(errorCode)) NppLog("Find start failed hr=0x%08X", (unsigned)errorCode);
        return S_OK;
    }
};

template <class T> void ReleaseCom(T*& pointer) { if (pointer) { pointer->Release(); pointer = nullptr; } }

}

void WebView2Viewer::ScrollToAnchor(const std::wstring& fragment) {
    PostJson(L"{\"type\":\"scrollToAnchor\",\"id\":\"" + EscapeJsonString(fragment) + L"\"}");
}

void WebView2Viewer::PostNotice(const std::wstring& text) {
    PostJson(L"{\"type\":\"notice\",\"text\":\"" + EscapeJsonString(text) + L"\"}");
}

// Edge's own find bar (ICoreWebView2Find, runtime 1.0.2849+), seeded with the reader's selection.
bool WebView2Viewer::StartFind(const std::wstring& term) {
    ICoreWebView2_28* webView = nullptr;
    ICoreWebView2Environment15* environment = nullptr;
    ICoreWebView2Find* find = nullptr;
    ICoreWebView2FindOptions* options = nullptr;
    bool started = false;
    if (m_pWebView && m_pEnvironment &&
        SUCCEEDED(m_pWebView->QueryInterface(IID_ICoreWebView2_28, reinterpret_cast<void**>(&webView))) &&
        SUCCEEDED(webView->get_Find(&find)) && find &&
        SUCCEEDED(m_pEnvironment->QueryInterface(IID_ICoreWebView2Environment15, reinterpret_cast<void**>(&environment))) &&
        SUCCEEDED(environment->CreateFindOptions(&options)) && options) {
        options->put_FindTerm(term.c_str());
        options->put_ShouldHighlightAllMatches(TRUE);
        auto* handler = new FindStartedHandler();
        started = SUCCEEDED(find->Start(options, handler));
        handler->Release();
    }
    ReleaseCom(options); ReleaseCom(environment); ReleaseCom(find); ReleaseCom(webView);
    if (!started) PostNotice(Tr("runtime.find"));
    return started;
}

bool WebView2Viewer::ShowPrintDialog() {
    ICoreWebView2_16* webView = nullptr;
    bool shown = m_pWebView && SUCCEEDED(m_pWebView->QueryInterface(IID_ICoreWebView2_16, reinterpret_cast<void**>(&webView))) &&
                 SUCCEEDED(webView->ShowPrintUI(COREWEBVIEW2_PRINT_DIALOG_KIND_BROWSER));
    ReleaseCom(webView);
    if (!shown) PostNotice(Tr("runtime.print"));
    return shown;
}

// Relative images resolve against the document's folder through a virtual host (NavigateToString pages
// have no base URL). Only that folder tree is reachable, read-only, without CORS.
void WebView2Viewer::SetDocumentFolder(const std::wstring& folder) {
    if (folder == m_documentFolder) return;
    ICoreWebView2_3* webView = nullptr;
    if (!m_pWebView || FAILED(m_pWebView->QueryInterface(IID_ICoreWebView2_3, reinterpret_cast<void**>(&webView)))) return;
    if (!m_documentFolder.empty()) webView->ClearVirtualHostNameToFolderMapping(kDocumentHost);
    m_documentFolder.clear();
    if (!folder.empty() && SUCCEEDED(webView->SetVirtualHostNameToFolderMapping(kDocumentHost, folder.c_str(), COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_DENY_CORS)))
        m_documentFolder = folder;
    ReleaseCom(webView);
}

void WebView2Viewer::SetDocumentState(const std::wstring& kind, const std::wstring& bufferId, const std::wstring& value) {
    PostJson(L"{\"type\":\"documentState\",\"kind\":\"" + EscapeJsonString(kind) + L"\",\"bufferId\":\"" + EscapeJsonString(bufferId) + L"\",\"value\":\"" + EscapeJsonString(value) + L"\"}");
}

void WebView2Viewer::SyncUiState(const PluginConfig& config) {
    std::wstringstream json;
    json << L"{\"type\":\"settings\",\"sync\":" << (config.syncWithCaret ? L"true" : L"false")
         << L",\"bidi\":" << (config.isSmartBiDiEnabled ? L"true" : L"false")
         << L",\"darkMode\":" << config.darkModeOverride
         << L",\"darkModeApplicable\":" << (config.theme == ReaderTheme::FollowNpp ? L"true" : L"false")
         << L",\"themeId\":\"" << EscapeJsonString(config.ThemeId()) << L"\""
         << L",\"zoomPercent\":" << static_cast<int>(config.zoomLevel * 100.0f + 0.5f)
         << L",\"autoShow\":" << (config.autoShowPanel ? L"true" : L"false")
         << L",\"hideEditor\":" << (config.hideEditor ? L"true" : L"false")
         << L",\"outline\":" << (config.showOutline ? L"true" : L"false")
         << L",\"fontSizePx\":" << config.contentFontSizePx
         << L",\"lineHeightPercent\":" << config.contentLineHeightPercent
         << L",\"maxWidthPx\":" << config.contentMaxWidthPx
         << L",\"outlineWidthPx\":" << config.outlineWidthPx
         << L",\"language\":\"" << (ReaderTextRussian() ? L"ru" : L"en") << L"\""
         << L",\"languageSetting\":\"" << EscapeJsonString(config.readerLanguage) << L"\"}";
    PostJson(json.str());
}

bool WebView2Viewer::ReportOperationResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId) {
    const wchar_t* status = L"failed";
    switch (result.status) {
        case OperationStatus::Succeeded: status = L"succeeded"; break;
        case OperationStatus::Cancelled: status = L"cancelled"; break;
        case OperationStatus::Partial: status = L"partial"; break;
        case OperationStatus::Failed: break;
    }
    return PostJson(L"{\"type\":\"operationResult\",\"operation\":\"" + EscapeJsonString(operation) +
        L"\",\"status\":\"" + status + L"\",\"error\":" + std::to_wstring(result.error) +
        L",\"detail\":\"" + EscapeJsonString(result.detail) + L"\",\"recoveryPath\":\"" +
        EscapeJsonString(result.recoveryPath) + L"\",\"requestId\":\"" + EscapeJsonString(requestId) + L"\"}");
}

void WebView2Viewer::NotifyBufferClosed(const std::wstring& bufferId) {
    if (m_renderedBufferId == bufferId) m_renderedBufferId.clear();
    if (m_pendingRender.bufferId == bufferId) {
        m_pendingRender = {};
        ++m_renderSequence;
    }
    PostJson(L"{\"type\":\"bufferClosed\",\"bufferId\":\"" + EscapeJsonString(bufferId) + L"\"}");
}

bool WebView2Viewer::IsCurrentRender(const CheckboxRequest& request) const {
    return IsPageReady() && request.bufferId == m_renderedBufferId && request.renderRevision == m_renderSequence;
}

bool WebView2Viewer::PostJson(const std::wstring& json) {
    if (!m_isInitialized || !m_pageReady) return false;
#ifdef NPP_READER_TESTING
    if (m_testPostJson) return SUCCEEDED(m_testPostJson(json));
#endif
    return m_pWebView && SUCCEEDED(m_pWebView->PostWebMessageAsJson(json.c_str()));
}

void WebView2Viewer::ScrollToLine(int line) {
    if (line >= 0) PostJson(L"{\"type\":\"scrollToLine\",\"line\":" + std::to_wstring(line) + L"}");
}

void WebView2Viewer::ExecuteScript(const std::wstring& script) {
    if (m_isInitialized && m_pWebView) {
        m_pWebView->ExecuteScript(script.c_str(), nullptr);
    }
}

bool WebView2Viewer::UpdateMarkdown(const std::string& markdownUtf8, const std::wstring& title, bool isDark, bool isBiDi, int caretLine, const std::wstring& bufferId) {
    const ULONGLONG sequence = ++m_renderSequence;
    m_currentDark = isDark;
    m_currentBiDi = isBiDi;

    bool hasTransport = m_pWebView != nullptr;
#ifdef NPP_READER_TESTING
    hasTransport = hasTransport || static_cast<bool>(m_testPostJson);
#endif
    if (!m_isInitialized || !hasTransport || !m_pageReady) {
        NppLog("UpdateMarkdown: queued pending (init=%d web=%d ready=%d bytes=%u)",
               (int)m_isInitialized, (int)(m_pWebView != nullptr), (int)m_pageReady, (unsigned)markdownUtf8.size());
        m_pendingRender.valid = true;
        m_pendingRender.markdown = markdownUtf8;
        m_pendingRender.title = title;
        m_pendingRender.isDark = isDark;
        m_pendingRender.isBiDi = isBiDi;
        m_pendingRender.caretLine = caretLine;
        m_pendingRender.bufferId = bufferId;
        return false;
    }

    std::wstringstream json;
    json << L"{\"type\":\"render\""
         << L",\"markdown\":\"" << EscapeJsonString(markdownUtf8) << L"\""
         << L",\"title\":\"" << EscapeJsonString(title) << L"\""
         << L",\"isDark\":" << (isDark ? L"true" : L"false")
         << L",\"themeId\":\"" << EscapeJsonString(m_currentThemeId) << L"\""
         << L",\"isBiDi\":" << (isBiDi ? L"true" : L"false")
         << L",\"caretLine\":" << caretLine
         << L",\"bufferId\":\"" << EscapeJsonString(bufferId) << L"\""
         << L",\"lang\":\"" << BiDiEngine::Utf8ToWide(BiDiEngine::DocumentLanguage(markdownUtf8)) << L"\""
         << L",\"renderRevision\":\"" << sequence << L"\""
         << L",\"docBase\":\"" << (m_documentFolder.empty() ? L"" : std::wstring(L"https://") + kDocumentHost + L"/") << L"\""
         << L"}";

    bool posted = PostJson(json.str());
    if (posted && sequence == m_renderSequence) {
        m_pendingRender = {};
        m_renderedBufferId = bufferId;
    }
    NppLog("UpdateMarkdown: posted=%d (bytes=%u)", (int)posted, (unsigned)markdownUtf8.size());
    return posted;
}

bool WebView2Viewer::HandleAcceleratorKey(UINT virtualKey, bool ctrl, bool shift, bool alt, bool isRepeat) {
    return m_hostShortcutCallback && m_hostShortcutCallback(virtualKey, ctrl, shift, alt, isRepeat);
}

void WebView2Viewer::OnWebMessageReceived(const std::wstring& message) {
    // Only the command name: payloads carry link targets, search text and document paths.
    const std::string command = BiDiEngine::WideToUtf8(message.substr(0, (std::min)(message.find(L':'), static_cast<size_t>(40))));
    NppLog("OnWebMessageReceived: %s (%u chars)", command.c_str(), static_cast<unsigned>(message.size()));

    const std::wstring prefixError = L"jsError:";
    if (message.find(prefixError) == 0) {
        std::string errUtf8 = BiDiEngine::WideToUtf8(message.substr(prefixError.length()));
        NppLog("JavaScript Exception: %s", errUtf8.c_str());
        return;
    }

    if (message == L"pageLoaded") {
        m_pageReady = true;
        NppLog("OnWebMessageReceived: pageLoaded");
        PendingRender pending = std::exchange(m_pendingRender, {});
        if (m_pageReadyCallback) {
            m_pageReadyCallback();
        } else if (pending.valid) {
            const ULONGLONG expectedSequence = m_renderSequence + 1;
            if (!UpdateMarkdown(pending.markdown, pending.title, pending.isDark, pending.isBiDi, pending.caretLine, pending.bufferId) &&
                m_renderSequence == expectedSequence && !m_pendingRender.valid) m_pendingRender = std::move(pending);
        }
        return;
    }

    const std::wstring prefixCheckbox = L"toggleCheckbox:";
    const std::wstring prefixTheme = L"selectTheme:";
    if (message.rfind(prefixTheme, 0) == 0) {
        if (m_selectThemeCallback) m_selectThemeCallback(message.substr(prefixTheme.size()));
        return;
    }
    if (message.find(prefixCheckbox) == 0) {
        CheckboxRequest request;
        if (ParseCheckboxRequest(message.substr(prefixCheckbox.length()), request) && m_checkboxCallback) m_checkboxCallback(request);
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
    if (message == L"print") { ShowPrintDialog(); return; }
    if (message == L"find" || message.starts_with(L"find:")) { StartFind(message.size() > 5 ? message.substr(5, 256) : L""); return; }

    if (message == L"toggleDark") {
        if (m_themeCallback) m_themeCallback();
        return;
    }

    if (message == L"toggleBiDi") {
        if (m_bidiCallback) m_bidiCallback();
        return;
    }
    if (m_settingsCallback) m_settingsCallback(message);
}
