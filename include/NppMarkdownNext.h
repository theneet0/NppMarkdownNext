#pragma once
#include <windows.h>
#include <string>
#include <vector>
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
    void CopyRenderedHtml(const std::wstring& requestId = L"");
    void SaveAsHtml(const std::wstring& requestId = L"");
    void ShowAbout();
    void OpenSettings();

    // Notepad++ Notifications
    void OnNppReady();
    void OnNotification(SCNotification* notifyCode);
    void OnDarkModeChanged();
    void OnBufferActivated();
    void OnBufferClosed(UINT_PTR bufferId);
    void OnDocumentPathChanged(UINT_PTR bufferId);
    void OnDocModified();
    void SyncThemeMenuItems();

    void EnsurePanelRegistered();
    bool IsNppReady() const noexcept { return m_isNppReady; }

    HWND GetPanelHwnd() const noexcept { return m_hPanel; }
    tTbData* GetTbData() noexcept { return &m_tbData; }

    HWND GetCurrentScintilla() const;
    std::string GetScintillaText(HWND hSci) const;
    int GetScintillaCaretLine(HWND hSci) const;
    UINT_PTR GetCurrentBufferId() const;

private:
    NppMarkdownNext() = default;
    ~NppMarkdownNext() = default;

    bool CreatePanelWindow();
    bool InitializeViewer();
    std::wstring GetViewerDataDirectory() const;
    void HandleViewerRecovery(bool manual = false);
    void SetViewerStatus(const std::wstring& text, bool retry);
    void LayoutViewerStatus();
    void SetPanelVisible(bool visible);
    void ApplyAutoShowPanel();
    void RequestRenderDebounced(bool force = false);
    bool ExecuteRender(bool force = false);
    bool IsDarkThemeEffective() const;
    void SyncUiState();
    bool SaveConfig(bool notifyUser = true);
    void QueueExport(const std::wstring& operation, const std::wstring& requestId);
    void FinishExport();
    void ReportExportResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId);
    void ShowExportWarning(const std::wstring& message);
    void HandleCheckboxToggle(const CheckboxRequest& request);
    void OnSettingsMessage(const std::wstring& message);
    std::wstring GetCurrentBufferPath() const;
    bool IsCurrentBufferMarkdown() const;

    static LRESULT CALLBACK PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInst = nullptr;
    NppData m_nppData = {};
    HWND m_hPanel = nullptr;
    HWND m_hViewerStatus = nullptr;
    HWND m_hRetryViewer = nullptr;
    tTbData m_tbData = {};

    PluginConfig m_config;
    std::wstring m_configPath;
    WebView2Viewer m_webViewViewer;
    MarkdownDocument m_currentDoc;

    bool m_isNppReady = false;
    bool m_isPanelRegistered = false;
    bool m_isPanelVisible = false;
    bool m_userClosedPanel = false;
    bool m_openSettingsPending = false;
    bool m_shuttingDown = false;
    bool m_forceRenderPending = false;
    bool m_recoveryQueued = false;
    bool m_hasRecoveryAttempt = false;
    ULONGLONG m_lastRecoveryTick = 0;
    UINT_PTR m_viewerGeneration = 0;
    bool m_viewerStatusVisible = false;
    bool m_settingsSaveWarningActive = false;
    bool m_settingsWarningReported = false;
    DWORD m_settingsSaveError = ERROR_SUCCESS;
    bool m_exportInProgress = false;
    bool m_recoverAfterExport = false;
    bool m_exportQueued = false;
    std::wstring m_queuedExportOperation;
    std::wstring m_queuedExportRequestId;
    std::vector<std::wstring> m_pendingExportWarnings;
    UINT_PTR m_activeBufferId = 0;
    std::wstring m_activeBufferPath;
    std::string m_lastRawContentUtf8;
    ULONGLONG m_sourceRevision = 0;
    ULONGLONG m_lastRenderedSourceRevision = 0;
    int m_lastScrolledCaretLine = -1;
#ifdef NPP_READER_TESTING
    friend struct ReaderLifecycleTestPeer;
    std::function<ULONGLONG()> m_testClock;
    std::function<bool()> m_testInitializeViewer;
    std::function<void(const std::wstring&)> m_testExportWarning;
#endif
};
