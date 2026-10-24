#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "MarkdownParser.h"
#include "Config.h"

struct PreviewComponents {
    std::string fullHtml;
    std::string bodyHtml;
    std::string tocHtml;
};

class HtmlExporter {
public:
    // Generate full interactive HTML for WebView2 preview with modern floating UI, TOC, and Search
    static std::string GeneratePreviewHtml(
        const MarkdownDocument& doc,
        const std::wstring& title,
        PreviewTheme theme,
        float zoomLevel = 1.0f,
        bool isSyncEnabled = true,
        int darkModeOverride = 0,
        const std::wstring& beardedVariant = L""
    );

    // Generate preview components for instant in-place DOM updates (zero-latency preview)
    static PreviewComponents GeneratePreviewComponents(
        const MarkdownDocument& doc,
        const std::wstring& title,
        PreviewTheme theme,
        float zoomLevel = 1.0f,
        bool isSyncEnabled = true,
        int darkModeOverride = 0,
        const std::wstring& beardedVariant = L""
    );

    // Standalone export to HTML file
    static std::wstring ExportToHtml(const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    static bool SaveToFile(const std::wstring& filePath, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    static bool CopyToClipboard(HWND hwndOwner, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");

    static std::wstring EscapeHtml(const std::wstring& str);
    static std::string EscapeHtmlUtf8(const std::string& str);
    static std::wstring InlinesToHtml(const std::vector<MarkdownSpan>& inlines);
    static std::wstring InlinesToPlainText(const std::vector<MarkdownSpan>& inlines);

private:
    static std::string GenerateCfHtml(const std::string& htmlFragment);
};
