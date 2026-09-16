#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <vector>
#include <memory>
#include "MarkdownParser.h"
#include "Config.h"

struct RenderItem {
    MarkdownBlockType type;
    IDWriteTextLayout* pLayout = nullptr;
    D2D1_RECT_F rect = { 0, 0, 0, 0 };
    int sourceLineStart = 0;
    int sourceLineEnd = 0;
    bool isRTL = false;
    int level = 0;
    int listIndex = 1;
    bool isTaskChecked = false;
    D2D1_RECT_F checkboxRect = { 0, 0, 0, 0 };
    std::wstring codeLanguage;
    std::vector<std::wstring> codeLines;
    std::vector<IDWriteTextLayout*> codeLineLayouts;
    D2D1_RECT_F copyBtnRect = { 0, 0, 0, 0 };
    bool isCopyBtnHovered = false;
    bool isCopiedAnim = false;
    MarkdownTable table;
    std::vector<std::vector<IDWriteTextLayout*>> tableCellLayouts;

    // Alert Callouts
    AlertType alertType = AlertType::None;
    std::wstring alertTitle;
    IDWriteTextLayout* pAlertTitleLayout = nullptr;
};

struct ClickableLink {
    D2D1_RECT_F rect;
    std::wstring url;
};

class MarkdownRenderer {
public:
    MarkdownRenderer();
    ~MarkdownRenderer();

    bool Initialize(HWND hwnd);
    void SetDarkMode(bool isDark);
    bool IsDarkMode() const noexcept { return m_isDarkMode; }

    void SetDocument(const MarkdownDocument& doc);
    void Layout(float clientWidth);
    void Render();
    void Resize(UINT width, UINT height);

    // Scrolling
    void ScrollBy(int deltaY);
    void ScrollTo(int newScrollY);
    void ScrollToSourceLine(int sourceLine);
    int GetScrollY() const noexcept { return m_scrollY; }
    int GetTotalHeight() const noexcept { return static_cast<int>(m_totalHeight); }

    // Interactivity
    bool OnMouseMove(int x, int y);
    bool OnLButtonDown(int x, int y, HWND hSciEditor);
    void SetZoom(float zoom);
    float GetZoom() const noexcept { return m_zoom; }

    void DiscardDeviceResources();

private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void ClearLayout();

    HWND m_hwnd = nullptr;
    bool m_isDarkMode = true;
    float m_zoom = 1.0f;
    float m_baseFontSize = 15.0f;
    float m_dpiScale = 1.0f;

    int m_scrollY = 0;
    float m_totalHeight = 0.0f;
    float m_lastClientWidth = 0.0f;

    MarkdownDocument m_doc;
    std::vector<RenderItem> m_renderItems;
    std::vector<ClickableLink> m_links;

    // Direct2D & DirectWrite Interfaces
    ID2D1Factory* m_pD2DFactory = nullptr;
    IDWriteFactory* m_pDWriteFactory = nullptr;
    ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

    // DirectWrite Text Formats
    IDWriteTextFormat* m_pFormatH1 = nullptr;
    IDWriteTextFormat* m_pFormatH2 = nullptr;
    IDWriteTextFormat* m_pFormatH3 = nullptr;
    IDWriteTextFormat* m_pFormatH4 = nullptr;
    IDWriteTextFormat* m_pFormatH5 = nullptr;
    IDWriteTextFormat* m_pFormatH6 = nullptr;
    IDWriteTextFormat* m_pFormatBody = nullptr;
    IDWriteTextFormat* m_pFormatBodyRTL = nullptr;
    IDWriteTextFormat* m_pFormatCode = nullptr;
    IDWriteTextFormat* m_pFormatTable = nullptr;
    IDWriteTextFormat* m_pFormatTableHeader = nullptr;
    IDWriteTextFormat* m_pFormatBadge = nullptr;
    IDWriteTextFormat* m_pFormatAlertTitle = nullptr;

    // Color Brushes
    ID2D1SolidColorBrush* m_pBrushBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushText = nullptr;
    ID2D1SolidColorBrush* m_pBrushH1 = nullptr;
    ID2D1SolidColorBrush* m_pBrushH2 = nullptr;
    ID2D1SolidColorBrush* m_pBrushH3 = nullptr;
    ID2D1SolidColorBrush* m_pBrushCodeBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushCodeText = nullptr;
    ID2D1SolidColorBrush* m_pBrushCodeBorder = nullptr;
    ID2D1SolidColorBrush* m_pBrushKeyword = nullptr;
    ID2D1SolidColorBrush* m_pBrushType = nullptr;
    ID2D1SolidColorBrush* m_pBrushString = nullptr;
    ID2D1SolidColorBrush* m_pBrushComment = nullptr;
    ID2D1SolidColorBrush* m_pBrushNumber = nullptr;
    ID2D1SolidColorBrush* m_pBrushQuoteBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushQuoteBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushBorder = nullptr;
    ID2D1SolidColorBrush* m_pBrushTableHdrBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushTableAltBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushLink = nullptr;
    ID2D1SolidColorBrush* m_pBrushLinkHover = nullptr;
    ID2D1SolidColorBrush* m_pBrushCheckboxBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushCheckboxBorder = nullptr;
    ID2D1SolidColorBrush* m_pBrushCheckboxCheck = nullptr;
    ID2D1SolidColorBrush* m_pBrushBtnBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushBtnHover = nullptr;

    // Alert Callout Brushes
    ID2D1SolidColorBrush* m_pBrushAlertNoteBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertNoteBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertTipBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertTipBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertImportantBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertImportantBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertWarningBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertWarningBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertCautionBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushAlertCautionBar = nullptr;
    ID2D1SolidColorBrush* m_pBrushHighlightBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushInlineCodeBg = nullptr;
    ID2D1SolidColorBrush* m_pBrushMacClose = nullptr;
    ID2D1SolidColorBrush* m_pBrushMacMin = nullptr;
    ID2D1SolidColorBrush* m_pBrushMacMax = nullptr;
};
