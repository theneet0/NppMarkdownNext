#pragma once

#include <windows.h>
#include <string>
#include <functional>

struct ICoreWebView2Environment;
struct ICoreWebView2Controller;
struct ICoreWebView2;

class WebView2Viewer {
public:
    using CheckboxCallback = std::function<void(int lineNo)>;
    using JumpToLineCallback = std::function<void(int lineNo)>;
    using SyncCallback = std::function<void()>;
    using ThemeCallback = std::function<void()>;
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
    bool UpdateMarkdown(const std::string& markdownUtf8, const std::wstring& title, bool isDark, bool isBiDi, int caretLine = -1);
    void ScrollToLine(int line);
    void SetTheme(bool isDark);
    void SetBiDi(bool isBiDi);
    void SetZoom(float zoomFactor);
    void ExecuteScript(const std::wstring& script);

    // Callbacks
    void SetCheckboxCallback(CheckboxCallback cb) { m_checkboxCallback = cb; }
    void SetJumpToLineCallback(JumpToLineCallback cb) { m_jumpToLineCallback = cb; }
    void SetSyncCallback(SyncCallback cb) { m_syncCallback = cb; }
    void SetThemeCallback(ThemeCallback cb) { m_themeCallback = cb; }
    void SetBiDiCallback(BiDiCallback cb) { m_bidiCallback = cb; }
    void SetZoomChangeCallback(ZoomChangeCallback cb) { m_zoomChangeCallback = cb; }
    void SetPageReadyCallback(PageReadyCallback cb) { m_pageReadyCallback = cb; }
    void SetInitFailedCallback(InitFailedCallback cb) { m_initFailedCallback = cb; }

private:
    void OnEnvironmentCreated(HRESULT result, ICoreWebView2Environment* env);
    void OnControllerCreated(HRESULT result, ICoreWebView2Controller* controller);
    void OnWebMessageReceived(const std::wstring& message);
    void NotifyInitFailed();

    HWND m_hParent = nullptr;
    bool m_isInitialized = false;
    bool m_isInitializing = false;
    bool m_pageReady = false;

    ICoreWebView2Environment* m_pEnvironment = nullptr;
    ICoreWebView2Controller* m_pController = nullptr;
    ICoreWebView2* m_pWebView = nullptr;
    HMODULE m_hLoaderModule = nullptr;

    float m_zoom = 1.0f;
    bool m_currentDark = false;
    bool m_currentBiDi = true;

    struct PendingRender {
        bool valid = false;
        std::string markdown;
        std::wstring title;
        bool isDark = false;
        bool isBiDi = true;
        int caretLine = -1;
    } m_pendingRender;

    CheckboxCallback m_checkboxCallback;
    JumpToLineCallback m_jumpToLineCallback;
    SyncCallback m_syncCallback;
    ThemeCallback m_themeCallback;
    BiDiCallback m_bidiCallback;
    ZoomChangeCallback m_zoomChangeCallback;
    PageReadyCallback m_pageReadyCallback;
    InitFailedCallback m_initFailedCallback;
};
