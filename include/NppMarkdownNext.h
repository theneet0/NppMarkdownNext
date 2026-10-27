#pragma once
#include <windows.h>
#include <string>
#include "PluginInterface.h"
#include "Docking.h"
#include "Config.h"
#include "WebView2Viewer.h"
#include "MarkdownParser.h"

class NppMarkdownNext {
public:
    static NppMarkdownNext& Instance();

    void Init(HINSTANCE hInst, NppData nppData);
    void Cleanup();

    // Menu Command Handlers
    void TogglePanel();
    void ToggleSyncWithCaret();
    void ToggleBiDi();
    void SetTheme(ReaderTheme theme);
    void ZoomIn();
    void ZoomOut();
    void ZoomReset();
    void CopyRenderedHtml();
    void SaveAsHtml();
    void ShowAbout();

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

private:
    NppMarkdownNext() = default;
    ~NppMarkdownNext() = default;

    bool CreatePanelWindow();
    void SetPanelVisible(bool visible);
    void ApplyAutoShowPanel();
    void RequestRenderDebounced();
    bool ExecuteRender();
    bool IsDarkThemeEffective() const;
    std::wstring GetCurrentBufferPath() const;
    bool IsCurrentBufferMarkdown() const;

    static LRESULT CALLBACK PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInst = nullptr;
    NppData m_nppData = {};
    HWND m_hPanel = nullptr;
    tTbData m_tbData = {};

    PluginConfig m_config;
    std::wstring m_configPath;
    WebView2Viewer m_webViewViewer;
    MarkdownDocument m_currentDoc;

    bool m_isNppReady = false;
    bool m_isPanelRegistered = false;
    bool m_isPanelVisible = false;
    bool m_userClosedPanel = false;
    std::string m_lastRawContentUtf8;
    int m_lastScrolledCaretLine = -1;
};
