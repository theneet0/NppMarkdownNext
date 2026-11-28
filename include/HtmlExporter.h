#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "MarkdownParser.h"
#include "Config.h"
#include "OperationResult.h"

using PreviewTheme = ReaderTheme;

struct PreviewComponents {
    std::string fullHtml;
    std::string bodyHtml;
    std::string tocHtml;
};

class HtmlExporter {
public:
    static std::string GeneratePreviewHtml(
        const MarkdownDocument& doc,
        const std::wstring& title,
        PreviewTheme theme,
        float zoomLevel = 1.0f,
        bool isSyncEnabled = true,
        int darkModeOverride = 0,
        const std::wstring& beardedVariant = L""
    );

    static PreviewComponents GeneratePreviewComponents(
        const MarkdownDocument& doc,
        const std::wstring& title,
        PreviewTheme theme,
        float zoomLevel = 1.0f,
        bool isSyncEnabled = true,
        int darkModeOverride = 0,
        const std::wstring& beardedVariant = L""
    );

    static std::wstring ExportToHtml(const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    static OperationResult SaveToFile(const std::wstring& filePath, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    static OperationResult CopyToClipboard(HWND hwndOwner, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    // Export of what the reader shows (its sanitized, rendered body); the parser-based calls above are the fallback.
    static OperationResult SaveRenderedToFile(const std::wstring& filePath, const std::string& bodyHtmlUtf8, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant = L"");
    static OperationResult CopyRenderedToClipboard(HWND hwndOwner, const std::string& fragmentUtf8, const std::wstring& plainText);
    static std::string EmbedDocumentImages(const std::string& htmlUtf8, const std::wstring& folder);
    static std::string WrapDocument(const std::string& bodyHtmlUtf8, const std::string& tocHtmlUtf8, const std::wstring& title,
                                    PreviewTheme theme, int darkModeOverride, const std::wstring& beardedVariant, bool includeScripts);

    static std::wstring EscapeHtml(const std::wstring& str);
    static std::string EscapeHtmlUtf8(const std::string& str);
    static std::wstring InlinesToHtml(const std::vector<MarkdownSpan>& inlines);
    static std::wstring InlinesToPlainText(const std::vector<MarkdownSpan>& inlines);

private:
    static std::string GenerateCfHtml(const std::string& htmlFragment);
    static OperationResult CopyFragmentToClipboard(HWND hwndOwner, const std::string& fragmentUtf8, const std::wstring& plain);
    static std::wstring DocumentToPlainText(const MarkdownDocument& doc);
};

#ifdef NPP_READER_TESTING
namespace ExportIoTest {
struct Hooks {
    decltype(&::CreateFileW) createFile = ::CreateFileW;
    decltype(&::WriteFile) writeFile = ::WriteFile;
    decltype(&::FlushFileBuffers) flushFile = ::FlushFileBuffers;
    decltype(&::CloseHandle) closeHandle = ::CloseHandle;
    decltype(&::ReplaceFileW) replaceFile = ::ReplaceFileW;
    decltype(&::MoveFileExW) moveFile = ::MoveFileExW;
    decltype(&::OpenClipboard) openClipboard = ::OpenClipboard;
    decltype(&::EmptyClipboard) emptyClipboard = ::EmptyClipboard;
    decltype(&::CloseClipboard) closeClipboard = ::CloseClipboard;
    decltype(&::RegisterClipboardFormatA) registerFormat = ::RegisterClipboardFormatA;
    decltype(&::GlobalAlloc) globalAlloc = ::GlobalAlloc;
    decltype(&::GlobalLock) globalLock = ::GlobalLock;
    decltype(&::GlobalFree) globalFree = ::GlobalFree;
    decltype(&::SetClipboardData) setClipboardData = ::SetClipboardData;
};
void SetHooks(const Hooks& hooks);
}
#endif
