#include "../include/HtmlExporter.h"
#include "../include/BiDiEngine.h"
#include "KelkBundle.inc"
#include "ReaderThemes.inc"
#include "../include/HeadingIds.h"
#include "../include/LinkPolicy.h"
#include "../include/ReaderText.h"
#include <fstream>
#include <cctype>
#include <cwctype>
#include <sstream>
#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <string_view>

#ifdef NPP_READER_TESTING
namespace ExportIoTest {
static Hooks current;
void SetHooks(const Hooks& hooks) { current = hooks; }
}
#define EXPORT_IO(name, ...) ExportIoTest::current.name(__VA_ARGS__)
#else
#define EXPORT_IO(name, ...) ExportIoNative::name(__VA_ARGS__)
namespace ExportIoNative {
const auto createFile = ::CreateFileW;
const auto writeFile = ::WriteFile;
const auto flushFile = ::FlushFileBuffers;
const auto closeHandle = ::CloseHandle;
const auto replaceFile = ::ReplaceFileW;
const auto moveFile = ::MoveFileExW;
const auto openClipboard = ::OpenClipboard;
const auto emptyClipboard = ::EmptyClipboard;
const auto closeClipboard = ::CloseClipboard;
const auto registerFormat = ::RegisterClipboardFormatA;
const auto globalAlloc = ::GlobalAlloc;
const auto globalLock = ::GlobalLock;
const auto globalFree = ::GlobalFree;
const auto setClipboardData = ::SetClipboardData;
}
#endif

namespace {

DWORD IoError() { DWORD error = GetLastError(); return error ? error : ERROR_GEN_FAILURE; }

struct ReadHandle {
    HANDLE value = INVALID_HANDLE_VALUE;
    ~ReadHandle() { if (value != INVALID_HANDLE_VALUE) ::CloseHandle(value); }
};

bool MatchesOriginal(HANDLE original, const std::wstring& path) {
    ReadHandle candidate{::CreateFileW(path.c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr)};
    if (candidate.value == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER first{}, second{}, zero{};
    if (!GetFileSizeEx(original, &first) || !GetFileSizeEx(candidate.value, &second) ||
        first.QuadPart != second.QuadPart || !SetFilePointerEx(original, zero, nullptr, FILE_BEGIN)) return false;
    std::array<char, 65536> a{}, b{};
    for (;;) {
        DWORD readA = 0, readB = 0;
        if (!ReadFile(original, a.data(), static_cast<DWORD>(a.size()), &readA, nullptr) ||
            !ReadFile(candidate.value, b.data(), static_cast<DWORD>(b.size()), &readB, nullptr) ||
            readA != readB || std::memcmp(a.data(), b.data(), readA)) return false;
        if (!readA) return true;
    }
}

OperationResult WriteHtmlAtomically(const std::wstring& path, std::string_view htmlUtf8) {
    if (path.empty() || path.find(L'\0') != std::wstring::npos)
        return {OperationStatus::Failed, ERROR_INVALID_PARAMETER, Tr("export.badPath")};
    DWORD attributes = GetFileAttributesW(path.c_str());
    bool existed = attributes != INVALID_FILE_ATTRIBUTES;
    if (!existed && GetLastError() != ERROR_FILE_NOT_FOUND && GetLastError() != ERROR_PATH_NOT_FOUND)
        return {OperationStatus::Failed, IoError(), Tr("export.inspectFailed")};
    if (existed && (attributes & FILE_ATTRIBUTE_DIRECTORY))
        return {OperationStatus::Failed, ERROR_DIRECTORY, Tr("export.isDirectory")};
    ReadHandle original;
    if (existed) {
        original.value = ::CreateFileW(path.c_str(), GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (original.value == INVALID_HANDLE_VALUE)
            return {OperationStatus::Failed, IoError(), Tr("export.preserveFailed")};
    }
    static std::atomic<unsigned long> counter{0};
    std::wstring temporary;
    HANDLE output = INVALID_HANDLE_VALUE;
    for (int attempt = 0; attempt < 32; ++attempt) {
        temporary = path + L".nppmd-" + std::to_wstring(GetCurrentProcessId()) + L"-" +
                    std::to_wstring(++counter) + L".tmp";
        output = EXPORT_IO(createFile, temporary.c_str(), GENERIC_WRITE, 0, nullptr,
                           CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (output != INVALID_HANDLE_VALUE || GetLastError() != ERROR_FILE_EXISTS) break;
    }
    if (output == INVALID_HANDLE_VALUE)
        return {OperationStatus::Failed, IoError(), Tr("export.tempFailed")};
    DWORD error = ERROR_SUCCESS;
    size_t position = 0;
    while (position < htmlUtf8.size()) {
        DWORD written = 0;
        DWORD count = static_cast<DWORD>(std::min<size_t>(htmlUtf8.size() - position, 1024 * 1024));
        if (!EXPORT_IO(writeFile, output, htmlUtf8.data() + position, count, &written, nullptr)) {
            error = IoError(); break;
        }
        if (!written || written > count) { error = ERROR_WRITE_FAULT; break; }
        position += written;
    }
    if (!error && !EXPORT_IO(flushFile, output)) error = IoError();
    if (!EXPORT_IO(closeHandle, output) && !error) error = IoError();
    if (error) {
        DeleteFileW(temporary.c_str());
        return {OperationStatus::Failed, error, Tr("export.writeFailed")};
    }
    if (!existed) {
        if (EXPORT_IO(moveFile, temporary.c_str(), path.c_str(), 0))
            return {OperationStatus::Succeeded};
        error = IoError();
        DeleteFileW(temporary.c_str());
        return {OperationStatus::Failed, error, Tr("export.commitFailed")};
    }
    std::wstring backup = temporary + L".bak";
    if (GetFileAttributesW(backup.c_str()) != INVALID_FILE_ATTRIBUTES) {
        DeleteFileW(temporary.c_str());
        return {OperationStatus::Failed, ERROR_FILE_EXISTS, Tr("export.backupInUse")};
    }
    if (EXPORT_IO(replaceFile, path.c_str(), temporary.c_str(), backup.c_str(), 0, nullptr, nullptr)) {
        DeleteFileW(backup.c_str());
        return {OperationStatus::Succeeded};
    }
    error = IoError();
    std::wstring recovery;
    if (MatchesOriginal(original.value, path)) {
        recovery = path;
    } else if (MatchesOriginal(original.value, backup)) {
        // Restore only into an absent destination; a concurrent file must survive.
        if (GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES &&
            EXPORT_IO(moveFile, backup.c_str(), path.c_str(), 0) && MatchesOriginal(original.value, path))
            recovery = path;
        else if (MatchesOriginal(original.value, backup)) recovery = backup;
    }
    if (!recovery.empty()) DeleteFileW(temporary.c_str());
    return {OperationStatus::Failed, error,
        recovery.empty() ? Tr("export.replaceRetained") :
                           Tr("export.replaceRecovery"), recovery};
}

struct ClipboardMemory {
    HGLOBAL value = nullptr;
    ~ClipboardMemory() { if (value) EXPORT_IO(globalFree, value); }
};

bool PrepareClipboardMemory(ClipboardMemory& memory, const void* bytes, size_t count) {
    memory.value = EXPORT_IO(globalAlloc, GMEM_MOVEABLE, count);
    if (!memory.value) return false;
    void* destination = EXPORT_IO(globalLock, memory.value);
    if (!destination) return false;
    std::memcpy(destination, bytes, count);
    SetLastError(ERROR_SUCCESS);
    if (!GlobalUnlock(memory.value) && GetLastError()) return false;
    return true;
}

std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return "";
    std::string str(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, nullptr, nullptr);
    return str;
}

std::wstring Utf8ToWide(const std::string& str) {
    if (str.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (len <= 0) return L"";
    std::wstring wstr(len - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
    return wstr;
}

} // namespace

std::wstring HtmlExporter::EscapeHtml(const std::wstring& str) {
    std::wstringstream ss;
    for (wchar_t ch : str) {
        switch (ch) {
            case L'&': ss << L"&amp;"; break;
            case L'<': ss << L"&lt;"; break;
            case L'>': ss << L"&gt;"; break;
            case L'\"': ss << L"&quot;"; break;
            case L'\'': ss << L"&#39;"; break;
            default: ss << ch; break;
        }
    }
    return ss.str();
}

std::string HtmlExporter::EscapeHtmlUtf8(const std::string& str) {
    std::stringstream ss;
    for (char ch : str) {
        switch (ch) {
            case '&': ss << "&amp;"; break;
            case '<': ss << "&lt;"; break;
            case '>': ss << "&gt;"; break;
            case '\"': ss << "&quot;"; break;
            case '\'': ss << "&#39;"; break;
            default: ss << ch; break;
        }
    }
    return ss.str();
}

std::wstring HtmlExporter::InlinesToPlainText(const std::vector<MarkdownSpan>& inlines) {
    std::wstring result;
    for (const auto& span : inlines) {
        result += span.text;
    }
    return result;
}

std::wstring HtmlExporter::InlinesToHtml(const std::vector<MarkdownSpan>& inlines) {
    std::wstringstream ss;
    for (const auto& span : inlines) {
        switch (span.type) {
            case InlineStyleType::Bold:
                ss << L"<strong>" << EscapeHtml(span.text) << L"</strong>";
                break;
            case InlineStyleType::Italic:
                ss << L"<em>" << EscapeHtml(span.text) << L"</em>";
                break;
            case InlineStyleType::BoldItalic:
                ss << L"<strong><em>" << EscapeHtml(span.text) << L"</em></strong>";
                break;
            case InlineStyleType::Strikethrough:
                ss << L"<del>" << EscapeHtml(span.text) << L"</del>";
                break;
            case InlineStyleType::Highlight:
                ss << L"<mark>" << EscapeHtml(span.text) << L"</mark>";
                break;
            case InlineStyleType::InlineCode: {
                bool isCodeRtl = BiDiEngine::IsParagraphRTL(span.text);
                const wchar_t* cDir = isCodeRtl ? L"rtl" : L"ltr";
                ss << L"<code class=\"inline-code\" dir=\"" << cDir << L"\"><bdi dir=\"" << cDir << L"\">"
                   << EscapeHtml(span.text) << L"</bdi></code>";
                break;
            }
            case InlineStyleType::InlineMath:
                ss << L"<span class=\"kelk-math katex-inline\" data-display=\"inline\" dir=\"ltr\" data-tex=\""
                   << EscapeHtml(span.text) << L"\"><bdi dir=\"ltr\">$"
                   << EscapeHtml(span.text) << L"$</bdi></span>";
                break;
            case InlineStyleType::Link:
                // A shared file must not carry script links (javascript:, vbscript:, data:...): keep the text only.
                if (IsSafeLinkScheme(LinkScheme(span.extra)))
                    ss << L"<a href=\"" << EscapeHtml(span.extra) << L"\" target=\"_blank\" rel=\"noopener\">"
                       << EscapeHtml(span.text) << L"</a>";
                else
                    ss << EscapeHtml(span.text);
                break;
            case InlineStyleType::Image:
                if (IsSafeImageScheme(LinkScheme(span.extra)))
                    ss << L"<img src=\"" << EscapeHtml(span.extra) << L"\" alt=\"" << EscapeHtml(span.text) << L"\" />";
                else
                    ss << EscapeHtml(span.text);
                break;
            case InlineStyleType::Normal:
            default:
                ss << EscapeHtml(span.text);
                break;
        }
    }
    return ss.str();
}

PreviewComponents HtmlExporter::GeneratePreviewComponents(
    const MarkdownDocument& doc,
    const std::wstring& title,
    PreviewTheme theme,
    float zoomLevel,
    bool isSyncEnabled,
    int darkModeOverride,
    const std::wstring& beardedVariant
) {
    std::wstringstream bodyStream;
    std::wstringstream tocStream;
    size_t tocEntries = 0;
    std::set<std::wstring> headingIds = ReservedReaderHeadingIds();

    // Open lists, innermost last; each one has an open <li> that a deeper list nests into.
    struct OpenList { bool ordered; int level; };
    std::vector<OpenList> openLists;
    auto closeListsDeeperThan = [&](int level) {
        while (!openLists.empty() && openLists.back().level > level) {
            bodyStream << L"</li>\n" << (openLists.back().ordered ? L"</ol>\n" : L"</ul>\n");
            openLists.pop_back();
        }
    };
    auto closeOpenLists = [&]() { closeListsDeeperThan(-1); };
    // Starts the item's <li> (left open): closes deeper lists and the previous sibling, opens a list if needed.
    auto beginListItem = [&](const MarkdownBlock& block, bool ordered) {
        int level = (std::max)(0, block.level);
        closeListsDeeperThan(level);
        if (!openLists.empty() && openLists.back().level == level) {
            if (openLists.back().ordered == ordered) bodyStream << L"</li>\n";
            else closeListsDeeperThan(level - 1);
        }
        if (openLists.empty() || openLists.back().level < level) {
            if (ordered) bodyStream << L"<ol" << (block.listIndex != 1 ? L" start=\"" + std::to_wstring(block.listIndex) + L"\"" : L"") << L">\n";
            else bodyStream << L"<ul>\n";
            openLists.push_back({ordered, level});
        }
    };

    for (const auto& block : doc.blocks) {
        if (block.type != MarkdownBlockType::UnorderedListItem && block.type != MarkdownBlockType::TaskListItem &&
            block.type != MarkdownBlockType::OrderedListItem) closeOpenLists();

        const wchar_t* dirAttr = block.isRTL ? L" dir=\"rtl\"" : L" dir=\"ltr\"";

        switch (block.type) {
            case MarkdownBlockType::Header1:
            case MarkdownBlockType::Header2:
            case MarkdownBlockType::Header3:
            case MarkdownBlockType::Header4:
            case MarkdownBlockType::Header5:
            case MarkdownBlockType::Header6: {
                int hNum = 1;
                if (block.type == MarkdownBlockType::Header2) hNum = 2;
                else if (block.type == MarkdownBlockType::Header3) hNum = 3;
                else if (block.type == MarkdownBlockType::Header4) hNum = 4;
                else if (block.type == MarkdownBlockType::Header5) hNum = 5;
                else if (block.type == MarkdownBlockType::Header6) hNum = 6;

                std::wstring hId = MakeHeadingId(InlinesToPlainText(block.inlines), headingIds);
                std::wstring hText = InlinesToHtml(block.inlines);

                bodyStream << L"<h" << hNum << L" id=\"" << hId << L"\" data-source-line=\""
                           << block.sourceLineStart << L"\"" << dirAttr << L">"
                           << hText << L"</h" << hNum << L">\n";

                std::wstring tocText = InlinesToPlainText(block.inlines);
                tocStream << L"<a href=\"#" << hId << L"\" class=\"toc-item toc-l" << hNum << L"\""
                          << dirAttr << L">" << EscapeHtml(tocText) << L"</a>\n";
                ++tocEntries;
                break;
            }

            case MarkdownBlockType::HorizontalRule: {
                bodyStream << L"<hr data-source-line=\"" << block.sourceLineStart << L"\" />\n";
                break;
            }

            case MarkdownBlockType::Blockquote: {
                bodyStream << L"<blockquote data-source-line=\"" << block.sourceLineStart << L"\""
                           << dirAttr << L"><p>" << InlinesToHtml(block.inlines) << L"</p></blockquote>\n";
                break;
            }

            case MarkdownBlockType::AlertCallout: {
                const wchar_t* alertTypeStr = L"note";
                switch (block.alertType) {
                    case AlertType::Tip: alertTypeStr = L"tip"; break;
                    case AlertType::Important: alertTypeStr = L"important"; break;
                    case AlertType::Warning: alertTypeStr = L"warning"; break;
                    case AlertType::Caution: alertTypeStr = L"caution"; break;
                    case AlertType::Note:
                    default: alertTypeStr = L"note"; break;
                }
                bodyStream << L"<div class=\"alert-callout alert-" << alertTypeStr << L"\" data-source-line=\""
                           << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                bodyStream << L"  <div class=\"alert-content\">" << InlinesToHtml(block.inlines) << L"</div>\n";
                bodyStream << L"</div>\n";
                break;
            }

            case MarkdownBlockType::CodeBlock: {
                if (block.codeLanguage == L"mermaid") {
                    bodyStream << L"<div class=\"mermaid\" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                    for (const auto& cl : block.codeLines) {
                        bodyStream << EscapeHtml(cl) << L"\n";
                    }
                    bodyStream << L"</div>\n";
                } else if (block.codeLanguage == L"math" || block.codeLanguage == L"latex" || block.codeLanguage == L"katex") {
                    std::wstring mathCode;
                    for (size_t k = 0; k < block.codeLines.size(); ++k) {
                        if (k > 0) mathCode += L"\n";
                        mathCode += block.codeLines[k];
                    }
                    bodyStream << L"<div class=\"kelk-math math-block\" data-display=\"block\" dir=\"ltr\" data-source-line=\""
                               << block.sourceLineStart << L"\" data-tex=\"" << EscapeHtml(mathCode) << L"\">"
                               << L"$$\n" << EscapeHtml(mathCode) << L"\n$$</div>\n";
                } else {
                    bodyStream << L"<pre><code>";
                    for (size_t i = 0; i < block.codeLines.size(); ++i) {
                        const auto& cl = block.codeLines[i];
                        bool isLineRtl = BiDiEngine::IsParagraphRTL(cl);
                        const wchar_t* lineDir = isLineRtl ? L"dir=\"rtl\"" : L"dir=\"ltr\"";
                        bodyStream << L"<span class=\"code-line\" " << lineDir << L">"
                                   << EscapeHtml(cl)
                                   << L"</span>";
                        if (i + 1 < block.codeLines.size()) bodyStream << L"\n";
                    }
                    bodyStream << L"</code></pre>\n";
                }
                break;
            }

            case MarkdownBlockType::Table: {
                bodyStream << L"<table data-source-line=\"" << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                if (!block.table.rows.empty()) {
                    bodyStream << L"<thead>\n<tr>\n";
                    for (size_t c = 0; c < block.table.rows[0].cells.size(); ++c) {
                        bodyStream << L"<th";
                        if (c < block.table.alignments.size()) {
                            switch (block.table.alignments[c]) {
                                case TableColumnAlign::Left: bodyStream << L" align=\"left\""; break;
                                case TableColumnAlign::Center: bodyStream << L" align=\"center\""; break;
                                case TableColumnAlign::Right: bodyStream << L" align=\"right\""; break;
                            }
                        }
                        bodyStream << L">" << InlinesToHtml(block.table.rows[0].cells[c].spans) << L"</th>\n";
                    }
                    bodyStream << L"</tr>\n</thead>\n";

                    if (block.table.rows.size() > 1) {
                        bodyStream << L"<tbody>\n";
                        for (size_t r = 1; r < block.table.rows.size(); ++r) {
                            bodyStream << L"<tr>\n";
                            for (size_t c = 0; c < block.table.rows[r].cells.size(); ++c) {
                                bodyStream << L"<td";
                                if (c < block.table.alignments.size()) {
                                    switch (block.table.alignments[c]) {
                                        case TableColumnAlign::Left: bodyStream << L" align=\"left\""; break;
                                        case TableColumnAlign::Center: bodyStream << L" align=\"center\""; break;
                                        case TableColumnAlign::Right: bodyStream << L" align=\"right\""; break;
                                    }
                                }
                                bodyStream << L">" << InlinesToHtml(block.table.rows[r].cells[c].spans) << L"</td>\n";
                            }
                            bodyStream << L"</tr>\n";
                        }
                        bodyStream << L"</tbody>\n";
                    }
                }
                bodyStream << L"</table>\n";
                break;
            }

            case MarkdownBlockType::UnorderedListItem:
            case MarkdownBlockType::OrderedListItem: {
                beginListItem(block, block.type == MarkdownBlockType::OrderedListItem);
                bodyStream << L"  <li data-source-line=\"" << block.sourceLineStart << L"\"" << dirAttr << L">"
                           << InlinesToHtml(block.inlines);
                break;
            }

            case MarkdownBlockType::TaskListItem: {
                beginListItem(block, false);
                bodyStream << L"  <li class=\"task-list-item\" data-source-line=\"" << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                bodyStream << L"    <input type=\"checkbox\" class=\"task-checkbox\" data-line=\""
                           << block.sourceLineStart << L"\" " << (block.isTaskChecked ? L"checked" : L"") << L"> ";
                bodyStream << L"<span>" << InlinesToHtml(block.inlines) << L"</span>";
                break;
            }

            case MarkdownBlockType::Paragraph:
            default: {
                bodyStream << L"<p data-source-line=\"" << block.sourceLineStart << L"\"" << dirAttr << L">"
                           << InlinesToHtml(block.inlines) << L"</p>\n";
                break;
            }
        }
    }
    closeOpenLists();

    std::string bodyHtmlUtf8 = WideToUtf8(bodyStream.str());
    std::string tocHtmlUtf8 = WideToUtf8(tocStream.str());

    // A shared file has no reader outline drawer: longer documents get their contents list on top.
    const std::string fullHtml = WrapDocument(bodyHtmlUtf8, tocEntries >= 3 ? tocHtmlUtf8 : std::string(), title, theme, darkModeOverride, beardedVariant, true);
    return PreviewComponents{ fullHtml, bodyHtmlUtf8, tocHtmlUtf8 };
}

// The document around a rendered body: theme styles, the page structure, optionally the contents list and the
// reader's scripts (highlighting and BiDi run on open). A body taken from the live preview is already highlighted
// and laid out, so it is shared without scripts.
std::string HtmlExporter::WrapDocument(const std::string& bodyHtmlUtf8, const std::string& tocHtmlUtf8, const std::wstring& title,
                                       PreviewTheme theme, int darkModeOverride, const std::wstring& beardedVariant, bool includeScripts) {
    bool isDark = (theme == PreviewTheme::Dark) || (darkModeOverride == 1);
    std::string bodyClass = isDark ? "dark" : "";
    PluginConfig themeConfig;
    themeConfig.theme = theme;
    themeConfig.beardedVariant = beardedVariant;
    if (theme != PreviewTheme::FollowNpp && theme != PreviewTheme::Light && theme != PreviewTheme::Dark) {
        bodyClass += " " + EscapeHtmlUtf8(WideToUtf8(themeConfig.ThemeId()));
    }

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html lang=\"" << BiDiEngine::DocumentLanguage(bodyHtmlUtf8) << "\" data-theme=\"" << (isDark ? "dark" : "light") << "\">\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "<title>" << WideToUtf8(title) << "</title>\n";
    html << "<style>\n" << s_kelkCss << "\n" << s_readerPaletteCss << "\n" << s_readerThemeCss
         << "\n" << s_readerSemanticCss << "\n</style>\n";
    if (includeScripts) html << "<script>\n" << s_kelkScripts << "\n</script>\n";
    html << "</head>\n<body class=\"" << bodyClass << "\">\n";
    html << "<div id=\"reader-wrap\">\n";
    html << "  <main id=\"content\" class=\"doc\" dir=\"auto\">\n";
    if (!tocHtmlUtf8.empty()) html << "<nav class=\"export-toc\" aria-label=\"" << WideToUtf8(Tr("ui.contents")) << "\">\n" << tocHtmlUtf8 << "</nav>\n";
    html << bodyHtmlUtf8;
    html << "\n  </main>\n</div>\n";
    html << "</body>\n</html>\n";
    return html.str();
}

namespace {
// "%D0%BF%20x&amp;y" -> UTF-8 bytes "п x&y" (an attribute value as innerHTML serializes it, percent-encoded by the reader)
std::string DecodeAttributeUrl(const std::string& value) {
    std::string unescaped;
    for (size_t i = 0; i < value.size(); ++i) {
        if (value.compare(i, 5, "&amp;") == 0) { unescaped += '&'; i += 4; }
        else if (value[i] == '%' && i + 2 < value.size() && isxdigit(static_cast<unsigned char>(value[i + 1])) && isxdigit(static_cast<unsigned char>(value[i + 2]))) {
            unescaped += static_cast<char>(std::stoi(value.substr(i + 1, 2), nullptr, 16));
            i += 2;
        } else unescaped += value[i];
    }
    return unescaped;
}

const char* ImageMimeType(std::wstring extension) {
    for (auto& c : extension) c = static_cast<wchar_t>(towlower(c));
    if (extension == L".png") return "image/png";
    if (extension == L".jpg" || extension == L".jpeg") return "image/jpeg";
    if (extension == L".gif") return "image/gif";
    if (extension == L".webp") return "image/webp";
    if (extension == L".bmp") return "image/bmp";
    if (extension == L".svg") return "image/svg+xml"; // shown through <img>: its scripts never run
    if (extension == L".ico") return "image/x-icon";
    return nullptr;
}
}

// Pictures the reader loaded from the document's folder (its virtual host) become data: URIs, so a shared file
// keeps them; one that cannot be read (missing, too large, unknown type) keeps its path relative to the document.
// The path comes from the reader's message, so it is checked again here: no "..", no drive, no stream, no device.
namespace {
// The real location of an open file or folder: symbolic links and junctions resolved.
std::wstring FinalPath(HANDLE handle) {
    wchar_t buffer[4 * MAX_PATH];
    const DWORD length = GetFinalPathNameByHandleW(handle, buffer, static_cast<DWORD>(std::size(buffer)), FILE_NAME_NORMALIZED);
    return length && length < std::size(buffer) ? std::wstring(buffer, length) : std::wstring();
}
}

std::string HtmlExporter::EmbedDocumentImages(const std::string& htmlUtf8, const std::wstring& folder) {
    std::wstring root; // the document folder's real path + '\\', resolved on the first picture
    const std::string marker = std::string("src=\"") + kDocumentHostUrlUtf8;
    std::string out;
    size_t position = 0;
    for (size_t at; (at = htmlUtf8.find(marker, position)) != std::string::npos;) {
        const size_t start = at + marker.size(), end = htmlUtf8.find('"', start);
        if (end == std::string::npos) break;
        out.append(htmlUtf8, position, at - position);
        const std::string relative = DecodeAttributeUrl(htmlUtf8.substr(start, end - start));
        std::wstring path = BiDiEngine::Utf8ToWide(relative);
        std::replace(path.begin(), path.end(), L'/', L'\\');
        std::string replacement = EscapeHtmlUtf8(relative);
        const bool safe = !folder.empty() && !path.empty() && path.find(L"..") == std::wstring::npos &&
                          path.find_first_of(L":*?\"<>|") == std::wstring::npos && path.front() != L'\\';
        const size_t dot = path.find_last_of(L'.'), slash = path.find_last_of(L'\\');
        const std::wstring extension = dot == std::wstring::npos || (slash != std::wstring::npos && dot < slash) ? L"" : path.substr(dot);
        const char* mime = safe ? ImageMimeType(extension) : nullptr;
        if (mime) {
            const std::wstring file = folder + L"\\" + path;
            HANDLE handle = CreateFileW(file.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            LARGE_INTEGER size = {};
            if (handle != INVALID_HANDLE_VALUE && root.empty()) {
                HANDLE directory = CreateFileW(folder.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                               nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
                if (directory != INVALID_HANDLE_VALUE) { root = FinalPath(directory) + L"\\"; CloseHandle(directory); }
            }
            // a link inside the folder may point anywhere: its target must still be inside before it is shared
            const std::wstring real = handle != INVALID_HANDLE_VALUE ? FinalPath(handle) : std::wstring();
            const bool inside = root.size() > 1 && real.size() > root.size() &&
                                CompareStringOrdinal(real.c_str(), static_cast<int>(root.size()), root.c_str(), static_cast<int>(root.size()), TRUE) == CSTR_EQUAL;
            if (inside && GetFileSizeEx(handle, &size) && size.QuadPart > 0 && size.QuadPart <= 8 * 1024 * 1024) {
                std::string bytes(static_cast<size_t>(size.QuadPart), '\0');
                DWORD read = 0;
                if (ReadFile(handle, bytes.data(), static_cast<DWORD>(bytes.size()), &read, nullptr) && read == bytes.size())
                    replacement = std::string("data:") + mime + ";base64," + BiDiEngine::Base64Encode(reinterpret_cast<const uint8_t*>(bytes.data()), bytes.size());
            }
            if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
        }
        out += "src=\"" + replacement + "\"";
        position = end + 1;
    }
    out.append(htmlUtf8, position, std::string::npos);
    return out;
}

OperationResult HtmlExporter::SaveRenderedToFile(const std::wstring& filePath, const std::string& bodyHtmlUtf8, const std::wstring& title,
                                                 PreviewTheme theme, const std::wstring& beardedVariant) {
    return WriteHtmlAtomically(filePath, WrapDocument(bodyHtmlUtf8, std::string(), title, theme, 0, beardedVariant, false));
}

OperationResult HtmlExporter::CopyRenderedToClipboard(HWND hwndOwner, const std::string& fragmentUtf8, const std::wstring& plainText) {
    return CopyFragmentToClipboard(hwndOwner, fragmentUtf8, plainText);
}

std::string HtmlExporter::GeneratePreviewHtml(
    const MarkdownDocument& doc,
    const std::wstring& title,
    PreviewTheme theme,
    float zoomLevel,
    bool isSyncEnabled,
    int darkModeOverride,
    const std::wstring& beardedVariant
) {
    return GeneratePreviewComponents(doc, title, theme, zoomLevel, isSyncEnabled, darkModeOverride, beardedVariant).fullHtml;
}

std::wstring HtmlExporter::ExportToHtml(const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant) {
    std::string previewHtml = GeneratePreviewHtml(doc, title, theme, 1.0f, false, 0, beardedVariant);
    return Utf8ToWide(previewHtml);
}

OperationResult HtmlExporter::SaveToFile(const std::wstring& filePath, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant) {
    std::string html = GeneratePreviewHtml(doc, title, theme, 1.0f, false, 0, beardedVariant);
    return WriteHtmlAtomically(filePath, html);
}

std::wstring HtmlExporter::DocumentToPlainText(const MarkdownDocument& doc) {
    std::wstring text;
    for (const auto& block : doc.blocks) {
        if (!text.empty()) text += L"\r\n";
        if (block.type == MarkdownBlockType::CodeBlock) {
            for (size_t i = 0; i < block.codeLines.size(); ++i) {
                if (i) text += L"\r\n";
                text += block.codeLines[i];
            }
        } else if (block.type == MarkdownBlockType::Table) {
            for (size_t row = 0; row < block.table.rows.size(); ++row) {
                if (row) text += L"\r\n";
                const auto& cells = block.table.rows[row].cells;
                for (size_t column = 0; column < cells.size(); ++column) {
                    if (column) text += L'\t';
                    text += InlinesToPlainText(cells[column].spans);
                }
            }
        } else {
            if (block.type == MarkdownBlockType::TaskListItem) text += block.isTaskChecked ? L"[x] " : L"[ ] ";
            text += InlinesToPlainText(block.inlines);
        }
    }
    return text;
}

OperationResult HtmlExporter::CopyToClipboard(HWND hwndOwner, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant) {
    auto components = GeneratePreviewComponents(doc, title, theme, 1.0f, false, 0, beardedVariant);
    return CopyFragmentToClipboard(hwndOwner, components.bodyHtml, DocumentToPlainText(doc));
}

OperationResult HtmlExporter::CopyFragmentToClipboard(HWND hwndOwner, const std::string& fragmentUtf8, const std::wstring& plain) {
    std::string cfHtml = GenerateCfHtml(fragmentUtf8);
    UINT format = EXPORT_IO(registerFormat, "HTML Format");
    if (!format) return {OperationStatus::Failed, IoError(), Tr("clip.registerFailed")};
    ClipboardMemory htmlMemory, textMemory;
    if (!PrepareClipboardMemory(htmlMemory, cfHtml.c_str(), cfHtml.size() + 1) ||
        !PrepareClipboardMemory(textMemory, plain.c_str(), (plain.size() + 1) * sizeof(wchar_t)))
        return {OperationStatus::Failed, IoError(), Tr("clip.prepareFailed")};
    if (!EXPORT_IO(openClipboard, hwndOwner))
        return {OperationStatus::Failed, IoError(), Tr("clip.busy")};
    if (!EXPORT_IO(emptyClipboard)) {
        DWORD error = IoError();
        EXPORT_IO(closeClipboard);
        return {OperationStatus::Failed, error, Tr("clip.clearFailed")};
    }
    DWORD error = ERROR_SUCCESS;
    bool htmlCopied = EXPORT_IO(setClipboardData, format, htmlMemory.value) != nullptr;
    if (htmlCopied) htmlMemory.value = nullptr;
    else error = IoError();
    bool textCopied = EXPORT_IO(setClipboardData, CF_UNICODETEXT, textMemory.value) != nullptr;
    if (textCopied) textMemory.value = nullptr;
    else if (!error) error = IoError();
    if (!EXPORT_IO(closeClipboard)) {
        DWORD closeError = IoError();
        return {OperationStatus::Failed, closeError,
            htmlCopied || textCopied ? Tr("clip.closeAfterCopy") :
                                       Tr("clip.closeAfterFail")};
    }
    if (htmlCopied && textCopied) return {OperationStatus::Succeeded};
    if (htmlCopied || textCopied) return {OperationStatus::Partial, error,
        htmlCopied ? Tr("clip.textMissing") :
                     Tr("clip.htmlMissing")};
    return {OperationStatus::Failed, error, Tr("clip.nothing")};
}

std::string HtmlExporter::GenerateCfHtml(const std::string& htmlFragment) {
    std::string header =
        "Version:0.9\r\n"
        "StartHTML:00000000\r\n"
        "EndHTML:00000000\r\n"
        "StartFragment:00000000\r\n"
        "EndFragment:00000000\r\n";

    std::string startHtml = "<html><body>\r\n<!--StartFragment-->";
    std::string endHtml = "<!--EndFragment-->\r\n</body></html>";

    size_t startHtmlPos = header.length();
    size_t startFragPos = startHtmlPos + startHtml.length();
    size_t endFragPos = startFragPos + htmlFragment.length();
    size_t endHtmlPos = endFragPos + endHtml.length();

    char buf[128];
    snprintf(buf, sizeof(buf),
        "Version:0.9\r\n"
        "StartHTML:%08zu\r\n"
        "EndHTML:%08zu\r\n"
        "StartFragment:%08zu\r\n"
        "EndFragment:%08zu\r\n",
        startHtmlPos, endHtmlPos, startFragPos, endFragPos);

    return std::string(buf) + startHtml + htmlFragment + endHtml;
}
