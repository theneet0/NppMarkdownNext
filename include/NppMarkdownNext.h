#pragma once
#include <windows.h>
#include <string>
#include "PluginInterface.h"
#include "Docking.h"
#include "Config.h"
#include "MarkdownRenderer.h"
#include "WebView2Viewer.h"

class NppMarkdownNext {
public:
    static NppMarkdownNext& Instance();

    void Init(HINSTANCE hInst, NppData nppData);
    void Cleanup();

    // Menu Command Handlers
    void TogglePanel();
    void ToggleSyncWithCaret();
    void ToggleOutline();
    void CopyRenderedHtml();
    void SaveAsHtml();
    void ZoomIn();
    void ZoomOut();
    void ZoomReset();
    void ToggleBiDi();
    void SelectTheme(PreviewTheme theme, const std::wstring& beardedVariant = L"");
    void ShowAbout();
    void OpenSettings();

    // Notepad++ Notifications
    void OnNppReady();
    void OnNotification(SCNotification* notifyCode);
    void OnDarkModeChanged();
    void OnBufferActivated();
    void OnDocModified();
    void SyncThemeMenuItems();

    void EnsurePanelRegistered();
    bool IsNppReady() const noexcept { return m_isNppReady; }

    HWND GetPanelHwnd() const noexcept { return m_hPanel; }
    tTbData* GetTbData() noexcept { return &m_tbData; }

    HWND GetCurrentScintilla() const;
    std::string GetScintillaText(HWND hSci) const;
    int GetScintillaCaretLine(HWND hSci) const;
    int GetScintillaFirstVisibleLine(HWND hSci) const;

private:
    NppMarkdownNext() = default;
    ~NppMarkdownNext() = default;

    bool CreatePanelWindow();
    void RequestRenderDebounced();
    // Returns false when rendering could not run yet (no Scintilla handle)
    bool ExecuteRender();
    PreviewTheme EffectiveTheme() const;
    std::wstring ThemeClassName(PreviewTheme theme) const;
    // Canonical theme id as used by the in-preview picker (window._themeId)
    std::wstring CurrentThemeId() const;
    std::wstring GetCurrentBufferPath() const;
    bool IsCurrentBufferMarkdown() const;

    static LRESULT CALLBACK PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInst = nullptr;
    NppData m_nppData = {};
    HWND m_hPanel = nullptr;
    tTbData m_tbData = {};

    PluginConfig m_config;
    std::wstring m_configPath;
    MarkdownRenderer m_renderer;
    WebView2Viewer m_webViewViewer;
    MarkdownDocument m_currentDoc;

    bool m_useWebView2 = true;
    bool m_isNppReady = false;
    bool m_isPanelRegistered = false;
    bool m_isPanelVisible = false;
    bool m_isRenderPending = false;
    bool m_openSettingsPending = false;
    std::wstring m_lastRawContent;
    int m_lastScrolledCaretLine = -1;
    int m_renderRetryCount = 0;
};

extern "C" int GetPluginCmdId(int index);
