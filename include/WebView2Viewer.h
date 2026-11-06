#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <memory>
#include "OperationResult.h"

struct ICoreWebView2Environment;
struct ICoreWebView2Controller;
struct ICoreWebView2;
struct PluginConfig;
struct ViewerLifetime { bool active = true; };
extern bool g_isDllDetaching;

struct CheckboxRequest {
    std::wstring bufferId;
    ULONGLONG renderRevision = 0;
    int line = 0;
    int markerColumn = 0;
    char expectedState = ' ';
};

class WebView2Viewer {
public:
    using CheckboxCallback = std::function<void(const CheckboxRequest&)>;
    using JumpToLineCallback = std::function<void(int lineNo)>;
    using SyncCallback = std::function<void()>;
    using ThemeCallback = std::function<void()>;
    using SelectThemeCallback = std::function<void(const std::wstring&)>;
    using SettingsCallback = std::function<void(const std::wstring&)>;
    using BiDiCallback = std::function<void()>;
    using ZoomChangeCallback = std::function<void(float factor)>;
    using PageReadyCallback = std::function<void()>;
    using InitFailedCallback = std::function<void()>;

    WebView2Viewer();
    ~WebView2Viewer();

    static bool IsAvailable();
    bool Initialize(HWND hParentWnd, const std::wstring& userDataDir);
    void Close();

    // Window management
    void Resize(int x, int y, int width, int height);
    void SetVisible(bool visible);
    bool IsInitialized() const { return m_isInitialized; }
    bool IsPageReady() const { return m_isInitialized && m_pageReady; }

    // Content updates
    bool UpdateMarkdown(const std::string& markdownUtf8, const std::wstring& title, bool isDark, bool isBiDi, int caretLine = -1, const std::wstring& bufferId = L"");
    void NotifyBufferClosed(const std::wstring& bufferId);
    void ScrollToLine(int line);
    void SetTheme(bool isDark, const std::wstring& themeId = L"follow");
    void SetBiDi(bool isBiDi);
    void SyncUiState(const PluginConfig& config);
    bool ReportOperationResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId = L"");
    bool IsCurrentRender(const CheckboxRequest& request) const;
    void SetZoom(float zoomFactor);
    void ExecuteScript(const std::wstring& script);

    // Callbacks
    void SetCheckboxCallback(CheckboxCallback cb) { m_checkboxCallback = cb; }
    void SetJumpToLineCallback(JumpToLineCallback cb) { m_jumpToLineCallback = cb; }
    void SetSyncCallback(SyncCallback cb) { m_syncCallback = cb; }
    void SetThemeCallback(ThemeCallback cb) { m_themeCallback = cb; }
    void SetSelectThemeCallback(SelectThemeCallback cb) { m_selectThemeCallback = cb; }
    void SetSettingsCallback(SettingsCallback cb) { m_settingsCallback = cb; }
    void SetBiDiCallback(BiDiCallback cb) { m_bidiCallback = cb; }
    void SetZoomChangeCallback(ZoomChangeCallback cb) { m_zoomChangeCallback = cb; }
    void SetPageReadyCallback(PageReadyCallback cb) { m_pageReadyCallback = cb; }
    void SetInitFailedCallback(InitFailedCallback cb) { m_initFailedCallback = cb; }

private:
    bool PostJson(const std::wstring& json);
    std::function<HRESULT(HRESULT, ICoreWebView2Environment*)> MakeEnvironmentCallback();
    std::function<HRESULT(HRESULT, ICoreWebView2Controller*)> MakeControllerCallback();
    void OnEnvironmentCreated(HRESULT result, ICoreWebView2Environment* env);
    void OnControllerCreated(HRESULT result, ICoreWebView2Controller* controller);
    void OnWebMessageReceived(const std::wstring& message);
    void NotifyInitFailed();

    HWND m_hParent = nullptr;
    bool m_isInitialized = false;
    bool m_isInitializing = false;
    bool m_pageReady = false;
    bool m_visible = false;
    std::shared_ptr<ViewerLifetime> m_lifetime;

    ICoreWebView2Environment* m_pEnvironment = nullptr;
    ICoreWebView2Controller* m_pController = nullptr;
    ICoreWebView2* m_pWebView = nullptr;
    HMODULE m_hLoaderModule = nullptr;

    float m_zoom = 1.0f;
    bool m_currentDark = false;
    std::wstring m_currentThemeId = L"follow";
    bool m_currentBiDi = true;

    struct PendingRender {
        bool valid = false;
        std::string markdown;
        std::wstring title;
        bool isDark = false;
        bool isBiDi = true;
        int caretLine = -1;
        std::wstring bufferId;
    } m_pendingRender;
    ULONGLONG m_renderSequence = 0;
    std::wstring m_renderedBufferId;

    CheckboxCallback m_checkboxCallback;
    JumpToLineCallback m_jumpToLineCallback;
    SyncCallback m_syncCallback;
    ThemeCallback m_themeCallback;
    SelectThemeCallback m_selectThemeCallback;
    SettingsCallback m_settingsCallback;
    BiDiCallback m_bidiCallback;
    ZoomChangeCallback m_zoomChangeCallback;
    PageReadyCallback m_pageReadyCallback;
    InitFailedCallback m_initFailedCallback;
#ifdef NPP_READER_TESTING
    friend struct ReaderLifecycleTestPeer;
    std::function<HRESULT(const std::wstring&)> m_testPostJson;
#endif
};
