#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "PluginInterface.h"
#include "Docking.h"
#include "Config.h"
#include "WebView2Viewer.h"
#include "MarkdownParser.h"

// Rendered body + plain text the reader sent with an export request (absent: the native parser renders).
struct HtmlExportContent { bool present = false; std::string htmlUtf8; std::wstring text; };

// Reader state remembered per document (column widths, reading place), see NppMarkdownNext.documents.ini.
enum class DocumentState { TableWidths = 0, ReadingPosition = 1 };

class NppMarkdownNext {
public:
    static NppMarkdownNext& Instance();

    void Init(HINSTANCE hInst, NppData nppData);
    void Cleanup();

    // Menu Command Handlers
    void TogglePanel();
    void ToggleHideEditor();
    void SetHideEditor(bool hide, bool saveConfig = true);
    void ToggleSyncWithCaret();
    void ToggleBiDi();
    void SetTheme(ReaderTheme theme);
    void ZoomIn();
    void ZoomOut();
    void ZoomReset();
    using ExportContent = HtmlExportContent;
    void RequestHtmlExport(const std::wstring& operation);
    void CopyRenderedHtml(const std::wstring& requestId = L"", const ExportContent& content = {});
    void SaveAsHtml(const std::wstring& requestId = L"", const ExportContent& content = {});
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
    void SyncHideEditorMenuItem();
    void UpdateHostLayout();
    void UpdateOverlayBounds();
    void ScheduleOverlaySync();
    void InstallHostSubclasses();
    void RemoveHostSubclasses();
    void ApplyAutoShowPanel();
    void RequestRenderDebounced(bool force = false);
    bool ExecuteRender(bool force = false);
    bool IsDarkThemeEffective() const;
    void SyncUiState();
    bool SaveConfig(bool notifyUser = true);
    void QueueExport(const std::wstring& operation, const std::wstring& requestId, ExportContent content = {});
    void FinishExport();
    void ReportExportResult(const std::wstring& operation, const OperationResult& result, const std::wstring& requestId);
    void ShowExportWarning(const std::wstring& message);
    void HandleCheckboxToggle(const CheckboxRequest& request);
    void OnSettingsMessage(const std::wstring& message);
    std::wstring LoadDocumentState(DocumentState kind, const std::wstring& path) const;
    void StoreDocumentState(DocumentState kind, const std::wstring& path, const std::wstring& value);
    void SaveDocumentStateMessage(DocumentState kind, const std::wstring& message);
    void CopyDocumentState(const std::wstring& from, const std::wstring& to);
    void OpenLink(const std::wstring& href);
    void UpdateStatusFont();
    COLORREF StatusColor(bool background) const;
    std::wstring GetCurrentBufferPath() const;
    std::wstring GetCurrentDocumentFolder() const;
    void ApplyReaderLanguage();
    bool IsCurrentBufferMarkdown() const;

    static LRESULT CALLBACK PanelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK ViewerHostWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HostSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassId, DWORD_PTR refData);

    HINSTANCE m_hInst = nullptr;
    NppData m_nppData = {};
    HWND m_hPanel = nullptr;
    HWND m_hOverlay = nullptr;
    HWND m_hViewerHost = nullptr;
    HWND m_hViewerStatus = nullptr;
    HWND m_hRetryViewer = nullptr;
    tTbData m_tbData = {};

    PluginConfig m_config;
    std::wstring m_configPath;
    std::wstring m_pendingAnchorPath, m_pendingAnchor; // a section link's target, scrolled to after its first render
    std::wstring m_documentStatePath; // NppMarkdownNext.documents.ini: per-document reader state
    std::unordered_map<std::wstring, std::wstring> m_documentStateCache; // "section/key" -> value; survives a failed write
    WebView2Viewer m_webViewViewer;
    MarkdownDocument m_currentDoc;

    bool m_isNppReady = false;
    bool m_isPanelRegistered = false;
    bool m_isPanelVisible = false;
    bool m_userClosedPanel = false;
    bool m_overlayActive = false;
    bool m_suppressDockNotification = false;
    bool m_overlaySyncQueued = false;
    bool m_subclassesInstalled = false;
    bool m_documentStateWarningReported = false; // a failing documents.ini is reported once per session
    bool m_nativeLanguageRussian = false; // Notepad++ itself is shown in Russian
    bool m_restorePanelOnStartup = false; // the panel was open when Notepad++ closed (used when auto-show is off)
    HFONT m_statusFont = nullptr;
    UINT m_statusFontDpi = 0;
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
    ExportContent m_queuedExportContent;
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
