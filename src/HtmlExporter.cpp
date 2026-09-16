#include "../include/HtmlExporter.h"
#include "../include/BiDiEngine.h"
#include "../include/SyntaxHighlighter.h"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace {

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

const char* s_modernPreviewCss = R"CSS(
:root {
    --bg-page: #ffffff;
    --text-primary: #1f2328;
    --text-secondary: #656d76;
    --border-color: #d0d7de;
    --code-bg: #f6f8fa;
    --code-header-bg: #eaeff4;
    --code-border: #d0d7de;
    --quote-bg: #f6f8fa;
    --quote-bar: #0969da;
    --table-alt: #f6f8fa;
    --table-header: #f0f3f6;
    --link-color: #0969da;
    --toolbar-bg: rgba(255, 255, 255, 0.85);
    --toolbar-border: rgba(208, 215, 222, 0.8);
    --toolbar-shadow: 0 8px 32px rgba(0, 0, 0, 0.12);
    --search-match: #fff3a8;
    --search-active: #ff9632;
    --alert-note-bg: #edf6fd;
    --alert-note-bar: #0969da;
    --alert-tip-bg: #edf9ef;
    --alert-tip-bar: #1a7f37;
    --alert-important-bg: #f7f2fa;
    --alert-important-bar: #8250df;
    --alert-warning-bg: #fffbea;
    --alert-warning-bar: #d29922;
    --alert-caution-bg: #fff0ed;
    --alert-caution-bar: #cf222e;
    --drawer-bg: #ffffff;
}

body.dark {
    --bg-page: #0d1117;
    --text-primary: #e6edf3;
    --text-secondary: #8b949e;
    --border-color: #30363d;
    --code-bg: #161b22;
    --code-header-bg: #21262d;
    --code-border: #30363d;
    --quote-bg: #161b22;
    --quote-bar: #2f81f7;
    --table-alt: #161b22;
    --table-header: #1c2128;
    --link-color: #58a6ff;
    --toolbar-bg: rgba(22, 27, 34, 0.85);
    --toolbar-border: rgba(48, 54, 61, 0.8);
    --toolbar-shadow: 0 8px 32px rgba(0, 0, 0, 0.4);
    --search-match: #53450e;
    --search-active: #9e6a03;
    --alert-note-bg: #1c2836;
    --alert-note-bar: #2f81f7;
    --alert-tip-bg: #162d20;
    --alert-tip-bar: #2ea043;
    --alert-important-bg: #261e33;
    --alert-important-bar: #a371f7;
    --alert-warning-bg: #2e2214;
    --alert-warning-bar: #d29922;
    --alert-caution-bg: #31191b;
    --alert-caution-bar: #f85149;
    --drawer-bg: #161b22;
}

* { box-sizing: border-box; }

body {
    margin: 0;
    padding: 0;
    background-color: var(--bg-page);
    color: var(--text-primary);
    font-family: -apple-system, BlinkMacSystemFont, "Vazirmatn", "Segoe UI", Tahoma, "Noto Sans Arabic", system-ui, sans-serif;
    font-size: 15px;
    line-height: 1.68;
    transition: background-color 0.25s ease, color 0.25s ease;
    overflow-x: hidden;
}

/* Main Layout & Content Container */
#app-layout {
    display: flex;
    flex-direction: row;
    width: 100%;
    min-height: 100vh;
    position: relative;
    align-items: stretch;
}

#content-container {
    flex: 1 1 0;
    min-width: 0;
    max-width: 900px;
    margin: 0 auto;
    padding: 24px 36px 120px 36px;
    word-wrap: break-word;
    overflow-wrap: break-word;
    transition: max-width 0.2s ease, padding 0.2s ease;
}

/* Sleek Floating Search Bar Overlay */
#search-overlay {
    position: fixed;
    top: 14px;
    right: 18px;
    z-index: 9990;
    display: none;
    align-items: center;
    gap: 6px;
    padding: 6px 12px;
    background: var(--toolbar-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color);
    border-radius: 24px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.25);
    transition: all 0.2s ease;
}

body.toc-open #search-overlay {
    right: 290px;
}

@media (max-width: 650px) {
    body.toc-open #search-overlay {
        right: 18px;
    }
}

#search-input {
    background: rgba(128, 128, 128, 0.12);
    border: 1px solid var(--border-color);
    border-radius: 14px;
    padding: 4px 10px;
    font-size: 12px;
    color: var(--text-primary);
    width: 140px;
    outline: none;
    transition: width 0.2s ease, border-color 0.2s ease;
}

#search-input:focus {
    width: 190px;
    border-color: var(--link-color);
}

#search-count {
    font-size: 11px;
    color: var(--text-secondary);
    min-width: 36px;
    text-align: center;
    user-select: none;
}

.search-btn {
    background: transparent;
    border: none;
    color: var(--text-primary);
    width: 26px;
    height: 26px;
    border-radius: 50%;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 11px;
    transition: background-color 0.15s ease, transform 0.1s ease;
}

.search-btn:hover {
    background: rgba(128, 128, 128, 0.2);
    transform: scale(1.06);
}

.search-btn:active {
    transform: scale(0.95);
}

.search-close {
    font-size: 13px;
    color: var(--text-secondary);
    margin-left: 2px;
}

mark.search-match {
    background-color: var(--search-match);
    color: inherit;
    border-radius: 2px;
    padding: 1px 2px;
}

mark.search-match.active {
    background-color: var(--search-active);
    color: #ffffff;
    font-weight: bold;
}

/* Modern Glassmorphic Context Menu */
.context-menu {
    position: fixed;
    z-index: 10000;
    min-width: 240px;
    direction: ltr;
    text-align: left;
    background: var(--toolbar-bg);
    backdrop-filter: blur(24px);
    -webkit-backdrop-filter: blur(24px);
    border: 1px solid var(--border-color);
    border-radius: 14px;
    box-shadow: 0 12px 36px rgba(0, 0, 0, 0.3);
    padding: 6px;
    user-select: none;
    font-family: inherit;
    font-size: 13px;
    display: none;
}

.menu-item {
    display: flex;
    align-items: center;
    gap: 10px;
    padding: 7px 10px;
    border-radius: 8px;
    cursor: pointer;
    color: var(--text-primary);
    transition: background-color 0.12s ease, color 0.12s ease;
}

.menu-item:hover {
    background: rgba(128, 128, 128, 0.16);
    color: var(--link-color);
}

.menu-icon {
    font-size: 14px;
    width: 18px;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-shrink: 0;
}

.menu-label {
    flex-grow: 1;
    white-space: nowrap;
}

.menu-shortcut {
    font-size: 11px;
    color: var(--text-secondary);
    margin-left: 8px;
    font-family: monospace;
    opacity: 0.85;
}

.menu-separator {
    height: 1px;
    background: var(--border-color);
    margin: 5px 6px;
}

/* Toast Feedback Notification */
.toast-notification {
    position: fixed;
    bottom: 24px;
    left: 50%;
    transform: translateX(-50%) translateY(20px);
    background: var(--toolbar-bg);
    backdrop-filter: blur(16px);
    -webkit-backdrop-filter: blur(16px);
    border: 1px solid var(--border-color);
    box-shadow: 0 8px 24px rgba(0, 0, 0, 0.25);
    color: var(--text-primary);
    padding: 8px 18px;
    border-radius: 20px;
    font-size: 12px;
    z-index: 10002;
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.25s ease, transform 0.25s ease;
}

.toast-notification.show {
    opacity: 1;
    transform: translateX(-50%) translateY(0);
}

/* Table of Contents Sticky Sidebar & Responsive Drawer */
#toc-backdrop {
    display: none;
    position: fixed;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background: rgba(0, 0, 0, 0.35);
    backdrop-filter: blur(2px);
    -webkit-backdrop-filter: blur(2px);
    z-index: 9998;
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.2s ease;
}

#toc-drawer {
    display: none;
    width: 270px;
    flex-shrink: 0;
    height: 100vh;
    position: sticky;
    top: 0;
    background: var(--drawer-bg);
    border-left: 1px solid var(--border-color);
    box-shadow: -4px 0 18px rgba(0, 0, 0, 0.06);
    z-index: 990;
    padding: 18px 16px;
    overflow-y: auto;
    box-sizing: border-box;
    user-select: none;
}

body.toc-open #toc-drawer {
    display: flex;
    flex-direction: column;
}

@media (max-width: 650px) {
    #toc-drawer {
        position: fixed;
        right: 0;
        top: 0;
        z-index: 9999;
        box-shadow: -8px 0 32px rgba(0, 0, 0, 0.35);
        width: min(290px, 85vw);
    }
    body.toc-open #toc-backdrop {
        display: block;
        opacity: 1;
        pointer-events: auto;
    }
}

.toc-header {
    font-size: 13px;
    font-weight: 700;
    margin-bottom: 14px;
    padding-bottom: 8px;
    border-bottom: 1px solid var(--border-color);
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.toc-title {
    display: flex;
    align-items: center;
    gap: 6px;
    color: var(--text-primary);
}

.toc-close {
    cursor: pointer;
    background: none;
    border: none;
    font-size: 16px;
    color: var(--text-secondary);
    padding: 4px;
    border-radius: 4px;
    display: flex;
    align-items: center;
    justify-content: center;
    transition: color 0.15s ease, background-color 0.15s ease;
}

.toc-close:hover {
    color: var(--text-primary);
    background: rgba(128, 128, 128, 0.15);
}

.toc-item {
    display: block;
    color: var(--text-secondary);
    text-decoration: none;
    font-size: 12px;
    padding: 5px 8px;
    border-radius: 6px;
    margin-bottom: 2px;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    transition: all 0.15s ease;
    unicode-bidi: plaintext;
    text-align: start;
}

.toc-item:hover {
    color: var(--link-color);
    background: rgba(128, 128, 128, 0.1);
}

.toc-item.active {
    color: var(--link-color);
    font-weight: 600;
    background: rgba(9, 105, 218, 0.1);
}

.toc-l1 { padding-inline-start: 8px; font-weight: 600; }
.toc-l2 { padding-inline-start: 18px; }
.toc-l3 { padding-inline-start: 28px; }
.toc-l4 { padding-inline-start: 38px; }

/* Markdown Typography */
h1, h2, h3, h4, h5, h6 {
    margin-top: 28px;
    margin-bottom: 16px;
    font-weight: 700;
    line-height: 1.3;
    scroll-margin-top: 60px;
}

h1 { font-size: 2em; border-bottom: 1px solid var(--border-color); padding-bottom: 0.3em; }
h2 { font-size: 1.5em; border-bottom: 1px solid var(--border-color); padding-bottom: 0.3em; }
h3 { font-size: 1.25em; }
h4 { font-size: 1.05em; }

p { margin-top: 0; margin-bottom: 16px; }

a { color: var(--link-color); text-decoration: none; }
a:hover { text-decoration: underline; }

hr {
    height: 2px;
    background-color: var(--border-color);
    border: none;
    margin: 28px 0;
}

blockquote {
    margin: 16px 0;
    padding: 10px 18px;
    background: var(--quote-bg);
    border-left: 4px solid var(--quote-bar);
    border-radius: 6px;
    color: var(--text-primary);
}

blockquote[dir="rtl"] {
    border-left: none;
    border-right: 4px solid var(--quote-bar);
}

/* Modern Code Blocks with macOS styling */
.code-block-card {
    margin: 20px 0;
    background: var(--code-bg);
    border: 1px solid var(--code-border);
    border-radius: 10px;
    overflow: hidden;
    box-shadow: 0 4px 16px rgba(0, 0, 0, 0.04);
    direction: ltr !important;
    text-align: left !important;
}

.code-block-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 8px 14px;
    background: var(--code-header-bg);
    border-bottom: 1px solid var(--code-border);
    user-select: none;
    direction: ltr !important;
}

.mac-controls {
    display: flex;
    gap: 7px;
    align-items: center;
}

.mac-dot {
    width: 11px;
    height: 11px;
    border-radius: 50%;
}

.mac-dot.close { background: #ff5f56; }
.mac-dot.min { background: #ffbd2e; }
.mac-dot.max { background: #27c93f; }

.lang-badge {
    font-size: 11px;
    font-family: "Cascadia Code", "Cascadia Mono", Consolas, "Courier New", monospace;
    font-weight: 600;
    color: var(--text-secondary);
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

.copy-btn {
    background: rgba(128, 128, 128, 0.15);
    border: 1px solid var(--border-color);
    border-radius: 6px;
    color: var(--text-primary);
    font-size: 11px;
    padding: 3px 10px;
    cursor: pointer;
    display: flex;
    align-items: center;
    gap: 5px;
    transition: all 0.15s ease;
}

.copy-btn:hover {
    background: rgba(128, 128, 128, 0.28);
}

.copy-btn.copied {
    background: #2ea043;
    color: #ffffff;
    border-color: #2ea043;
}

pre {
    margin: 0;
    padding: 14px 18px;
    overflow-x: auto;
    font-family: "Cascadia Code", "Cascadia Mono", Consolas, "Courier New", monospace;
    font-size: 13.5px;
    line-height: 1.55;
    tab-size: 4;
    direction: ltr !important;
    text-align: left !important;
    unicode-bidi: isolate;
}

pre code {
    background: transparent;
    border: none;
    padding: 0;
    font-size: 100%;
    display: block;
    direction: ltr !important;
    text-align: left !important;
    unicode-bidi: isolate;
}

code.inline-code, code {
    font-family: "Cascadia Code", "Cascadia Mono", Consolas, "Courier New", monospace;
    font-size: 88%;
    padding: 0.15em 0.45em;
    background-color: var(--code-bg);
    border: 1px solid var(--border-color);
    border-radius: 5px;
    direction: ltr;
    unicode-bidi: isolate;
    display: inline;
    white-space: break-spaces;
    word-break: break-word;
}

code[dir="rtl"], code.inline-code[dir="rtl"] {
    direction: rtl;
    unicode-bidi: isolate;
    text-align: right;
}

code[dir="ltr"], code.inline-code[dir="ltr"] {
    direction: ltr;
    unicode-bidi: isolate;
    text-align: left;
}

bdi {
    unicode-bidi: isolate;
}

bdi[dir="ltr"] {
    direction: ltr;
    text-align: left;
}

bdi[dir="rtl"] {
    direction: rtl;
    text-align: right;
}

/* Syntax Highlighting Tokens */
.hl-keyword { color: #cf222e; font-weight: 600; }
.hl-type { color: #953800; }
.hl-string { color: #0a3069; }
.hl-comment { color: #6e7781; font-style: italic; }
.hl-number { color: #0550ae; }
.hl-preprocessor { color: #8250df; }
.hl-operator { color: #24292f; }

body.dark .hl-keyword { color: #ff7b72; font-weight: 600; }
body.dark .hl-type { color: #ffa657; }
body.dark .hl-string { color: #a5d6ff; }
body.dark .hl-comment { color: #8b949e; font-style: italic; }
body.dark .hl-number { color: #79c0ff; }
body.dark .hl-preprocessor { color: #d2a8ff; }
body.dark .hl-operator { color: #e6edf3; }

/* Alert Callouts */
.alert-callout {
    border-radius: 8px;
    padding: 14px 18px;
    margin: 18px 0;
    border-left: 4px solid var(--quote-bar);
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.03);
}

.alert-callout[dir="rtl"] {
    border-left: none;
    border-right: 4px solid var(--quote-bar);
}

.alert-callout.note { background: var(--alert-note-bg); border-color: var(--alert-note-bar); }
.alert-callout.tip { background: var(--alert-tip-bg); border-color: var(--alert-tip-bar); }
.alert-callout.important { background: var(--alert-important-bg); border-color: var(--alert-important-bar); }
.alert-callout.warning { background: var(--alert-warning-bg); border-color: var(--alert-warning-bar); }
.alert-callout.caution { background: var(--alert-caution-bg); border-color: var(--alert-caution-bar); }

.alert-header {
    display: flex;
    align-items: center;
    gap: 8px;
    font-weight: 700;
    margin-bottom: 6px;
}

.alert-callout.note .alert-header { color: var(--alert-note-bar); }
.alert-callout.tip .alert-header { color: var(--alert-tip-bar); }
.alert-callout.important .alert-header { color: var(--alert-important-bar); }
.alert-callout.warning .alert-header { color: var(--alert-warning-bar); }
.alert-callout.caution .alert-header { color: var(--alert-caution-bar); }

/* Tables */
table {
    border-collapse: collapse;
    width: 100%;
    margin: 20px 0;
    border-radius: 8px;
    overflow: hidden;
    border: 1px solid var(--border-color);
}

th, td {
    border: 1px solid var(--border-color);
    padding: 9px 15px;
}

th {
    background-color: var(--table-header);
    font-weight: 600;
}

tr:nth-child(2n) {
    background-color: var(--table-alt);
}

/* Task Lists */
.task-list {
    list-style-type: none;
    padding-left: 0;
}

.task-list[dir="rtl"] {
    padding-right: 0;
}

.task-item {
    display: flex;
    align-items: flex-start;
    gap: 8px;
    margin: 6px 0;
}

.task-checkbox {
    width: 16px;
    height: 16px;
    margin-top: 4px;
    cursor: pointer;
    accent-color: var(--link-color);
}

/* Highlight and Math */
mark {
    background-color: #fff8c5;
    color: #1f2328;
    padding: 0.15em 0.4em;
    border-radius: 4px;
}

body.dark mark {
    background-color: #3e3816;
    color: #e6edf3;
}

/* Math and Diagrams (100% Offline) */
.katex-math, .katex, .katex-display {
    font-family: "Cambria Math", "Latin Modern Math", "STIX Two Math", "Times New Roman", serif;
    font-size: 1.15em;
    direction: ltr !important;
    text-align: center;
    margin: 14px 0;
    overflow-x: auto;
    unicode-bidi: isolate;
}

.katex-inline {
    display: inline;
    font-family: "Cambria Math", "Latin Modern Math", "STIX Two Math", "Times New Roman", serif;
    font-size: 1.05em;
    direction: ltr !important;
    unicode-bidi: isolate;
}

math {
    direction: ltr !important;
    unicode-bidi: isolate;
}

.mermaid {
    margin: 20px 0;
    display: flex;
    justify-content: center;
    align-items: center;
    background: var(--code-bg);
    border: 1px solid var(--border-color);
    border-radius: 10px;
    padding: 16px;
    overflow-x: auto;
    direction: ltr !important;
}

.mermaid-svg {
    max-width: 100%;
    height: auto;
    display: block;
}

@media print {
    #custom-context-menu, #search-overlay, #toc-drawer, #toc-backdrop, #toast-msg { display: none !important; }
    #content-container { padding: 0 !important; max-width: 100% !important; }
    body { background: #ffffff !important; color: #000000 !important; }
}
)CSS";

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

namespace {

std::wstring TokenizeCodeHtml(const std::wstring& line, const std::wstring& language) {
    if (line.empty()) return L"";
    auto tokens = SyntaxHighlighter::Tokenize(line, language);
    if (tokens.empty()) {
        return HtmlExporter::EscapeHtml(line);
    }

    std::wstringstream ss;
    size_t cursor = 0;
    for (const auto& tok : tokens) {
        if (tok.start > cursor) {
            std::wstring plain = line.substr(cursor, tok.start - cursor);
            ss << HtmlExporter::EscapeHtml(plain);
        }

        std::wstring tokenText = line.substr(tok.start, tok.length);
        const wchar_t* clsName = L"hl-default";
        switch (tok.type) {
            case HighlightTokenType::Keyword: clsName = L"hl-keyword"; break;
            case HighlightTokenType::Type: clsName = L"hl-type"; break;
            case HighlightTokenType::String: clsName = L"hl-string"; break;
            case HighlightTokenType::Comment: clsName = L"hl-comment"; break;
            case HighlightTokenType::Number: clsName = L"hl-number"; break;
            case HighlightTokenType::Preprocessor: clsName = L"hl-preprocessor"; break;
            case HighlightTokenType::Operator: clsName = L"hl-operator"; break;
            default: break;
        }

        ss << L"<span class=\"" << clsName << L"\">" << HtmlExporter::EscapeHtml(tokenText) << L"</span>";
        cursor = tok.start + tok.length;
    }

    if (cursor < line.size()) {
        std::wstring plain = line.substr(cursor);
        ss << HtmlExporter::EscapeHtml(plain);
    }

    return ss.str();
}

} // namespace

PreviewComponents HtmlExporter::GeneratePreviewComponents(
    const MarkdownDocument& doc,
    const std::wstring& title,
    bool isDarkMode,
    float zoomLevel,
    bool isSyncEnabled
) {
    std::wstringstream bodyStream;
    std::wstringstream tocStream;
    int headingIndex = 0;

    bool inUnorderedList = false;
    bool inOrderedList = false;
    bool inTaskList = false;

    auto closeOpenLists = [&]() {
        if (inUnorderedList) { bodyStream << L"</ul>\n"; inUnorderedList = false; }
        if (inOrderedList) { bodyStream << L"</ol>\n"; inOrderedList = false; }
        if (inTaskList) { bodyStream << L"</div>\n"; inTaskList = false; }
    };

    for (const auto& block : doc.blocks) {
        if (block.type != MarkdownBlockType::UnorderedListItem) {
            if (inUnorderedList) { bodyStream << L"</ul>\n"; inUnorderedList = false; }
        }
        if (block.type != MarkdownBlockType::OrderedListItem) {
            if (inOrderedList) { bodyStream << L"</ol>\n"; inOrderedList = false; }
        }
        if (block.type != MarkdownBlockType::TaskListItem) {
            if (inTaskList) { bodyStream << L"</div>\n"; inTaskList = false; }
        }

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

                headingIndex++;
                std::wstring hId = L"h-" + std::to_wstring(headingIndex);
                std::wstring hText = InlinesToHtml(block.inlines);

                bodyStream << L"<h" << hNum << L" id=\"" << hId << L"\" data-source-line=\""
                           << block.sourceLineStart << L"\"" << dirAttr << L">"
                           << hText << L"</h" << hNum << L">\n";

                // Add to TOC
                tocStream << L"<a href=\"#" << hId << L"\" class=\"toc-item toc-l" << hNum << L"\""
                          << dirAttr << L" onclick=\"onTocClick('" << hId << L"'); return false;\">"
                          << hText << L"</a>\n";
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
                const wchar_t* alertCls = L"note";
                const wchar_t* alertIcon = L"ℹ️";
                switch (block.alertType) {
                    case AlertType::Tip: alertCls = L"tip"; alertIcon = L"💡"; break;
                    case AlertType::Important: alertCls = L"important"; alertIcon = L"🟣"; break;
                    case AlertType::Warning: alertCls = L"warning"; alertIcon = L"⚠️"; break;
                    case AlertType::Caution: alertCls = L"caution"; alertIcon = L"🛑"; break;
                    default: break;
                }

                bodyStream << L"<div class=\"alert-callout " << alertCls << L"\" data-source-line=\""
                           << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                bodyStream << L"  <div class=\"alert-header\">" << alertIcon << L" " << EscapeHtml(block.alertTitle) << L"</div>\n";
                bodyStream << L"  <div class=\"alert-body\">" << InlinesToHtml(block.inlines) << L"</div>\n";
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
                } else {
                    std::wstring langDisplay = block.codeLanguage.empty() ? L"TEXT" : block.codeLanguage;
                    bodyStream << L"<div class=\"code-block-card\" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                    bodyStream << L"  <div class=\"code-block-header\">\n";
                    bodyStream << L"    <div class=\"mac-controls\"><div class=\"mac-dot close\"></div><div class=\"mac-dot min\"></div><div class=\"mac-dot max\"></div></div>\n";
                    bodyStream << L"    <div class=\"lang-badge\">" << EscapeHtml(langDisplay) << L"</div>\n";
                    bodyStream << L"    <button class=\"copy-btn\" onclick=\"copyCodeBlock(this)\">📋 Copy</button>\n";
                    bodyStream << L"  </div>\n";
                    bodyStream << L"  <pre><code>";
                    for (size_t i = 0; i < block.codeLines.size(); ++i) {
                        bodyStream << TokenizeCodeHtml(block.codeLines[i], block.codeLanguage);
                        if (i + 1 < block.codeLines.size()) bodyStream << L"\n";
                    }
                    bodyStream << L"</code></pre>\n";
                    bodyStream << L"</div>\n";
                }
                break;
            }

            case MarkdownBlockType::Table: {
                bodyStream << L"<table data-source-line=\"" << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                for (size_t r = 0; r < block.table.rows.size(); ++r) {
                    const auto& row = block.table.rows[r];
                    bodyStream << L"  <tr>\n";
                    for (size_t c = 0; c < row.cells.size(); ++c) {
                        const wchar_t* tag = row.isHeader ? L"th" : L"td";
                        std::wstring alignStyle;
                        if (c < block.table.alignments.size()) {
                            if (block.table.alignments[c] == TableColumnAlign::Center) alignStyle = L" style=\"text-align:center;\"";
                            else if (block.table.alignments[c] == TableColumnAlign::Right) alignStyle = L" style=\"text-align:right;\"";
                            else if (block.table.alignments[c] == TableColumnAlign::Left) alignStyle = L" style=\"text-align:left;\"";
                        }
                        bodyStream << L"    <" << tag << alignStyle << L">" << InlinesToHtml(row.cells[c].spans) << L"</" << tag << L">\n";
                    }
                    bodyStream << L"  </tr>\n";
                }
                bodyStream << L"</table>\n";
                break;
            }

            case MarkdownBlockType::UnorderedListItem: {
                if (!inUnorderedList) {
                    bodyStream << L"<ul" << dirAttr << L" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                    inUnorderedList = true;
                }
                bodyStream << L"  <li data-source-line=\"" << block.sourceLineStart << L"\">" << InlinesToHtml(block.inlines) << L"</li>\n";
                break;
            }

            case MarkdownBlockType::OrderedListItem: {
                if (!inOrderedList) {
                    bodyStream << L"<ol" << dirAttr << L" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                    inOrderedList = true;
                }
                bodyStream << L"  <li data-source-line=\"" << block.sourceLineStart << L"\">" << InlinesToHtml(block.inlines) << L"</li>\n";
                break;
            }

            case MarkdownBlockType::TaskListItem: {
                if (!inTaskList) {
                    bodyStream << L"<div class=\"task-list\"" << dirAttr << L" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                    inTaskList = true;
                }
                bodyStream << L"  <div class=\"task-item\" data-source-line=\"" << block.sourceLineStart << L"\">\n";
                bodyStream << L"    <input type=\"checkbox\" class=\"task-checkbox\" data-line=\""
                           << block.sourceLineStart << L"\" " << (block.isTaskChecked ? L"checked" : L"")
                           << L" onchange=\"handleTaskCheck(this)\">\n";
                bodyStream << L"    <span>" << InlinesToHtml(block.inlines) << L"</span>\n";
                bodyStream << L"  </div>\n";
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

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "<title>" << WideToUtf8(title) << "</title>\n";

    // 100% Local Offline Styles (zero external CDN or web font latency)
    html << "<style>\n" << s_modernPreviewCss << "\n</style>\n";
    html << "</head>\n<body class=\"" << (isDarkMode ? "dark" : "") << "\">\n";

    // In-page overlays & context menu
    html << R"HTML(
<div id="search-overlay" style="display: none;">
  <span style="font-size:13px; opacity:0.8;">🔍</span>
  <input type="text" id="search-input" placeholder="Find in document..." oninput="doSearch()" onkeydown="onSearchKey(event)" />
  <span id="search-count">0/0</span>
  <button class="search-btn" title="Previous match (Shift+Enter)" onclick="prevMatch()">▲</button>
  <button class="search-btn" title="Next match (Enter)" onclick="nextMatch()">▼</button>
  <button class="search-btn search-close" title="Close (Escape)" onclick="closeSearch()">✕</button>
</div>

<div id="custom-context-menu" class="context-menu" style="display: none;" onmousedown="event.preventDefault()">
  <div class="menu-item" id="menu-copy-selection" style="display: none;" onclick="copySelectionFromMenu()">
    <span class="menu-icon">✂️</span>
    <span class="menu-label">Copy Selection</span>
    <span class="menu-shortcut">Ctrl+C</span>
  </div>
  <div class="menu-item" onclick="openSearchFromMenu()">
    <span class="menu-icon">🔍</span>
    <span class="menu-label">Find in Document</span>
    <span class="menu-shortcut">Ctrl+F</span>
  </div>
  <div class="menu-item" onclick="toggleTocFromMenu()">
    <span class="menu-icon">📑</span>
    <span class="menu-label" id="menu-toc-label">Outline / Table of Contents</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="zoomInFromMenu()">
    <span class="menu-icon">🔍</span>
    <span class="menu-label">Zoom In</span>
    <span class="menu-shortcut">Ctrl++</span>
  </div>
  <div class="menu-item" onclick="zoomOutFromMenu()">
    <span class="menu-icon">🔍</span>
    <span class="menu-label">Zoom Out</span>
    <span class="menu-shortcut">Ctrl+-</span>
  </div>
  <div class="menu-item" onclick="zoomResetFromMenu()">
    <span class="menu-icon">🔍</span>
    <span class="menu-label">Reset Zoom (100%)</span>
    <span class="menu-shortcut">Ctrl+0</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="toggleThemeFromMenu()">
    <span class="menu-icon">🌓</span>
    <span class="menu-label" id="menu-theme-label">Toggle Dark / Light Theme</span>
  </div>
  <div class="menu-item" onclick="toggleSyncFromMenu()">
    <span class="menu-icon">🔄</span>
    <span class="menu-label" id="menu-sync-label">Toggle Caret Sync Scroll</span>
  </div>
  <div class="menu-item" onclick="toggleBiDiFromMenu()">
    <span class="menu-icon">🌐</span>
    <span class="menu-label" id="menu-bidi-label">Smart BiDi (Persian/Arabic RTL)</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="copyFullHtmlFromMenu()">
    <span class="menu-icon">📋</span>
    <span class="menu-label">Copy Full Rendered HTML</span>
  </div>
  <div class="menu-item" onclick="saveAsHtmlFromMenu()">
    <span class="menu-icon">💾</span>
    <span class="menu-label">Save As HTML...</span>
  </div>
  <div class="menu-item" onclick="printFromMenu()">
    <span class="menu-icon">📄</span>
    <span class="menu-label">Print / Save as PDF</span>
  </div>
</div>

<div id="toc-backdrop" onclick="closeToc()"></div>
<div id="toast-msg" class="toast-notification"></div>

<div id="app-layout">
  <main id="content-container">)HTML";
    html << bodyHtmlUtf8;
    html << R"HTML(  </main>
  <aside id="toc-drawer">
    <div class="toc-header">
      <div class="toc-title">
        <span>📑</span>
        <span>Outline / Table of Contents</span>
      </div>
      <button class="toc-close" title="Close (Escape)" onclick="closeToc()">✕</button>
    </div>
    <div class="toc-content">)HTML";
    html << tocHtmlUtf8;
    html << R"HTML(    </div>
  </aside>
</div>

<script>
window._syncEnabled = )HTML";
    html << (isSyncEnabled ? "true" : "false");
    html << R"HTML(;
// Native 100% offline MathML engine (KaTeX compatible)
function renderLocalMath() {
    var container = document.getElementById("content-container");
    if (!container) return;

    var mathSymbols = {
        "alpha": "α", "beta": "β", "gamma": "γ", "delta": "δ", "epsilon": "ε",
        "zeta": "ζ", "eta": "η", "theta": "θ", "iota": "ι", "kappa": "κ",
        "lambda": "λ", "mu": "μ", "nu": "ν", "xi": "ξ", "pi": "π", "rho": "ρ",
        "sigma": "σ", "tau": "τ", "upsilon": "υ", "phi": "φ", "chi": "χ",
        "psi": "ψ", "omega": "ω", "Gamma": "Γ", "Delta": "Δ", "Theta": "Θ",
        "Lambda": "Λ", "Xi": "Ξ", "Pi": "Π", "Sigma": "Σ", "Phi": "Φ", "Psi": "Ψ",
        "Omega": "Ω", "infty": "∞", "pm": "±", "times": "×", "div": "÷",
        "cdot": "·", "approx": "≈", "neq": "≠", "ne": "≠", "le": "≤", "leq": "≤",
        "ge": "≥", "geq": "≥", "in": "∈", "notin": "∉", "partial": "∂",
        "sum": "∑", "prod": "∏", "int": "∫", "oint": "∮", "to": "→", "rightarrow": "→",
        "leftarrow": "←", "Rightarrow": "⇒", "Leftarrow": "⇐", "forall": "∀", "exists": "∃"
    };

    function texToMathML(tex, isBlock) {
        var str = tex.trim();
        var fracRegex = /\\frac\s*\{([^{}]+)\}\s*\{([^{}]+)\}/g;
        while (fracRegex.test(str)) {
            str = str.replace(fracRegex, function(_, a, b) {
                return '<mfrac><mrow>' + convertTokens(a) + '</mrow><mrow>' + convertTokens(b) + '</mrow></mfrac>';
            });
        }
        var sqrtRegex = /\\sqrt\s*\{([^{}]+)\}/g;
        while (sqrtRegex.test(str)) {
            str = str.replace(sqrtRegex, function(_, a) {
                return '<msqrt><mrow>' + convertTokens(a) + '</mrow></msqrt>';
            });
        }
        return '<math display="' + (isBlock ? 'block' : 'inline') + '" class="katex-math katex">' + convertTokens(str) + '</math>';
    }

    function convertTokens(expr) {
        expr = expr.replace(/([a-zA-Z0-9\u0370-\u03FF]+|\))\s*\^\s*\{([^{}]+)\}/g, function(_, base, sup) {
            return '<msup><mrow>' + tokenChunk(base) + '</mrow><mrow>' + convertTokens(sup) + '</mrow></msup>';
        });
        expr = expr.replace(/([a-zA-Z0-9\u0370-\u03FF]+|\))\s*\^\s*([a-zA-Z0-9])/g, function(_, base, sup) {
            return '<msup><mrow>' + tokenChunk(base) + '</mrow><mrow>' + tokenChunk(sup) + '</mrow></msup>';
        });
        expr = expr.replace(/([a-zA-Z0-9\u0370-\u03FF]+|\))\s*_\s*\{([^{}]+)\}/g, function(_, base, sub) {
            return '<msub><mrow>' + tokenChunk(base) + '</mrow><mrow>' + convertTokens(sub) + '</mrow></msub>';
        });
        expr = expr.replace(/([a-zA-Z0-9\u0370-\u03FF]+|\))\s*_\s*([a-zA-Z0-9])/g, function(_, base, sub) {
            return '<msub><mrow>' + tokenChunk(base) + '</mrow><mrow>' + tokenChunk(sub) + '</mrow></msub>';
        });
        return tokenChunk(expr);
    }

    function tokenChunk(s) {
        return s.replace(/\\([a-zA-Z]+)/g, function(match, name) {
            if (mathSymbols[name]) {
                var sym = mathSymbols[name];
                if ("∑∏∫∮".indexOf(sym) !== -1) {
                    return '<mo largeop="true">' + sym + '</mo>';
                } else if ("±×÷·≈≠≤≥∈∉→←⇒⇐∀∃".indexOf(sym) !== -1) {
                    return '<mo>' + sym + '</mo>';
                }
                return '<mi>' + sym + '</mi>';
            }
            return '<mi>' + match + '</mi>';
        }).replace(/([0-9]+(?:\.[0-9]+)?)/g, '<mn>$1</mn>')
          .replace(/([=+\-*\/<>(),!])/g, '<mo>$1</mo>')
          .replace(/(?![^<]*>)([a-zA-Z])/g, '<mi>$1</mi>');
    }

    // 1. Process explicit .katex-inline elements
    var inlines = container.querySelectorAll(".katex-inline");
    inlines.forEach(function(el) {
        if (el.querySelector("math")) return;
        var tex = el.getAttribute("data-tex") || el.textContent.trim().replace(/^\$|\$$/g, '');
        if (tex) {
            el.innerHTML = texToMathML(tex, false);
            el.classList.add("katex");
        }
    });

    // 2. Process display math blocks ($$...$$) and remaining inline math in text
    var walker = document.createTreeWalker(container, NodeFilter.SHOW_TEXT, null, false);
    var textNodes = [];
    while (walker.nextNode()) {
        var node = walker.currentNode;
        if (node.parentElement && (node.parentElement.tagName === "PRE" || node.parentElement.tagName === "CODE" || node.parentElement.tagName === "SCRIPT" || node.parentElement.tagName === "STYLE" || node.parentElement.closest("pre, code, math, .katex-inline"))) continue;
        if (node.nodeValue.indexOf("$") !== -1) {
            textNodes.push(node);
        }
    }

    textNodes.forEach(function(node) {
        var text = node.nodeValue;
        if (text.indexOf("$$") !== -1) {
            var parts = text.split("$$");
            if (parts.length >= 3) {
                var frag = document.createDocumentFragment();
                for (var i = 0; i < parts.length; i++) {
                    if (i % 2 === 1) {
                        var span = document.createElement("div");
                        span.className = "katex-display katex";
                        span.innerHTML = texToMathML(parts[i], true);
                        frag.appendChild(span);
                    } else if (parts[i].length > 0) {
                        frag.appendChild(document.createTextNode(parts[i]));
                    }
                }
                node.parentNode.replaceChild(frag, node);
                return;
            }
        }
        if (text.indexOf("$") !== -1) {
            var parts = text.split("$");
            if (parts.length >= 3) {
                var frag = document.createDocumentFragment();
                for (var i = 0; i < parts.length; i++) {
                    if (i % 2 === 1) {
                        var span = document.createElement("span");
                        span.className = "katex-inline katex";
                        span.innerHTML = texToMathML(parts[i], false);
                        frag.appendChild(span);
                    } else if (parts[i].length > 0) {
                        frag.appendChild(document.createTextNode(parts[i]));
                    }
                }
                node.parentNode.replaceChild(frag, node);
            }
        }
    });
}

// Native 100% offline Mermaid procedural SVG diagram engine
function renderLocalMermaid() {
    var blocks = document.querySelectorAll(".mermaid");
    if (!blocks || blocks.length === 0) return;

    var isDark = document.body.classList.contains("dark");
    var strokeColor = isDark ? "#58a6ff" : "#0969da";
    var nodeBg = isDark ? "#161b22" : "#ffffff";
    var nodeBorder = isDark ? "#30363d" : "#d0d7de";
    var textColor = isDark ? "#e6edf3" : "#1f2328";
    var edgeColor = isDark ? "#8b949e" : "#57606a";
    var labelBg = isDark ? "#21262d" : "#f6f8fa";

    blocks.forEach(function(block) {
        var code = block.getAttribute("data-code");
        if (!code) {
            code = block.textContent.trim();
            block.setAttribute("data-code", code);
        }
        var lines = code.split("\n").map(function(l) { return l.trim(); }).filter(function(l) { return l.length > 0; });
        if (lines.length === 0) return;

        var firstLine = lines[0].toLowerCase();
        var isLR = firstLine.indexOf("lr") !== -1;
        var nodes = {};
        var edges = [];

        function addNode(id, label) {
            if (!nodes[id]) {
                nodes[id] = { id: id, label: label || id, inEdges: 0, outEdges: [] };
            } else if (label && nodes[id].label === id) {
                nodes[id].label = label;
            }
            return nodes[id];
        }

        function cleanLabel(raw) {
            if (!raw) return "";
            return raw.replace(/^[\[\(\{]+|[\]\)\}]+$/g, '').trim();
        }

        for (var i = 0; i < lines.length; i++) {
            var line = lines[i];
            if (line.indexOf("graph") === 0 || line.indexOf("flowchart") === 0) continue;

            var edgeMatch = line.match(/^([a-zA-Z0-9_\-]+)(?:([\[\(\{].*?[\]\)\}]))?\s*(?:(-->|==>|-\.->|---)\s*(?:\|(.*?)\|)?|---\s*\|(.*?)\|\s*-->|-->\|(.*?)\|)\s*([a-zA-Z0-9_\-]+)(?:([\[\(\{].*?[\]\)\}]))?$/);
            if (edgeMatch) {
                var uId = edgeMatch[1];
                var uText = cleanLabel(edgeMatch[2]) || uId;
                var edgeLabel = edgeMatch[4] || edgeMatch[5] || edgeMatch[6] || "";
                var vId = edgeMatch[7];
                var vText = cleanLabel(edgeMatch[8]) || vId;

                addNode(uId, uText);
                addNode(vId, vText);
                nodes[uId].outEdges.push({ to: vId, label: edgeLabel });
                nodes[vId].inEdges++;
                edges.push({ from: uId, to: vId, label: edgeLabel });
            } else {
                var single = line.match(/^([a-zA-Z0-9_\-]+)(?:([\[\(\{].*?[\]\)\}]))$/);
                if (single) {
                    addNode(single[1], cleanLabel(single[2]) || single[1]);
                }
            }
        }

        var nodeKeys = Object.keys(nodes);
        if (nodeKeys.length === 0) return;

        var layers = {};
        var maxLayer = 0;
        nodeKeys.forEach(function(k) {
            if (nodes[k].inEdges === 0) {
                layers[k] = 0;
            }
        });
        if (Object.keys(layers).length === 0) {
            layers[nodeKeys[0]] = 0;
        }

        var queue = Object.keys(layers);
        while (queue.length > 0) {
            var cur = queue.shift();
            var curL = layers[cur];
            nodes[cur].outEdges.forEach(function(e) {
                var nxt = e.to;
                if (layers[nxt] === undefined || layers[nxt] < curL + 1) {
                    layers[nxt] = curL + 1;
                    if (layers[nxt] > maxLayer) maxLayer = layers[nxt];
                    queue.push(nxt);
                }
            });
        }
        nodeKeys.forEach(function(k) {
            if (layers[k] === undefined) layers[k] = 0;
        });

        var layerGroups = [];
        for (var l = 0; l <= maxLayer; l++) layerGroups.push([]);
        nodeKeys.forEach(function(k) {
            layerGroups[layers[k]].push(k);
        });

        var nodeWidth = 130;
        var nodeHeight = 44;
        var nodePos = {};
        var svgW = 0, svgH = 0;

        if (isLR) {
            var colGap = 100;
            var rowGap = 36;
            var maxRows = 1;
            layerGroups.forEach(function(g) { if (g.length > maxRows) maxRows = g.length; });
            svgW = (maxLayer + 1) * (nodeWidth + colGap) + 40;
            svgH = maxRows * (nodeHeight + rowGap) + 40;

            layerGroups.forEach(function(g, colIdx) {
                var totalColH = g.length * nodeHeight + (g.length - 1) * rowGap;
                var startY = (svgH - totalColH) / 2;
                var x = 30 + colIdx * (nodeWidth + colGap);
                g.forEach(function(k, rowIdx) {
                    var y = startY + rowIdx * (nodeHeight + rowGap);
                    nodePos[k] = { x: x, y: y };
                });
            });
        } else {
            var colGap = 40;
            var rowGap = 70;
            var maxCols = 1;
            layerGroups.forEach(function(g) { if (g.length > maxCols) maxCols = g.length; });
            svgW = maxCols * (nodeWidth + colGap) + 40;
            svgH = (maxLayer + 1) * (nodeHeight + rowGap) + 40;

            layerGroups.forEach(function(g, rowIdx) {
                var totalRowW = g.length * nodeWidth + (g.length - 1) * colGap;
                var startX = (svgW - totalRowW) / 2;
                var y = 30 + rowIdx * (nodeHeight + rowGap);
                g.forEach(function(k, colIdx) {
                    var x = startX + colIdx * (nodeWidth + colGap);
                    nodePos[k] = { x: x, y: y };
                });
            });
        }

        var svgParts = [];
        svgParts.push('<svg class="mermaid-svg" viewBox="0 0 ' + svgW + ' ' + svgH + '" width="' + svgW + '" height="' + svgH + '">');
        svgParts.push('<defs><marker id="arrow" viewBox="0 0 10 10" refX="8" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse"><path d="M 0 1 L 9 5 L 0 9 z" fill="' + strokeColor + '" /></marker></defs>');

        edges.forEach(function(e) {
            var p1 = nodePos[e.from];
            var p2 = nodePos[e.to];
            if (!p1 || !p2) return;

            var x1, y1, x2, y2;
            if (isLR) {
                x1 = p1.x + nodeWidth;
                y1 = p1.y + nodeHeight / 2;
                x2 = p2.x;
                y2 = p2.y + nodeHeight / 2;
            } else {
                x1 = p1.x + nodeWidth / 2;
                y1 = p1.y + nodeHeight;
                x2 = p2.x + nodeWidth / 2;
                y2 = p2.y;
            }

            var midX = (x1 + x2) / 2;
            var midY = (y1 + y2) / 2;
            var d = isLR
                ? 'M ' + x1 + ' ' + y1 + ' C ' + (x1 + 40) + ' ' + y1 + ', ' + (x2 - 40) + ' ' + y2 + ', ' + x2 + ' ' + y2
                : 'M ' + x1 + ' ' + y1 + ' C ' + x1 + ' ' + (y1 + 35) + ', ' + x2 + ' ' + (y2 - 35) + ', ' + x2 + ' ' + y2;

            svgParts.push('<path d="' + d + '" stroke="' + edgeColor + '" stroke-width="2" fill="none" marker-end="url(#arrow)" />');
            if (e.label) {
                var lblW = e.label.length * 8 + 12;
                svgParts.push('<rect x="' + (midX - lblW/2) + '" y="' + (midY - 10) + '" width="' + lblW + '" height="18" rx="4" fill="' + labelBg + '" stroke="' + nodeBorder + '" stroke-width="1" />');
                svgParts.push('<text x="' + midX + '" y="' + (midY + 3) + '" text-anchor="middle" font-size="11" fill="' + textColor + '" font-family="system-ui, sans-serif">' + e.label + '</text>');
            }
        });

        nodeKeys.forEach(function(k) {
            var pos = nodePos[k];
            var node = nodes[k];
            svgParts.push('<rect x="' + pos.x + '" y="' + pos.y + '" width="' + nodeWidth + '" height="' + nodeHeight + '" rx="8" fill="' + nodeBg + '" stroke="' + strokeColor + '" stroke-width="2" />');
            svgParts.push('<text x="' + (pos.x + nodeWidth / 2) + '" y="' + (pos.y + nodeHeight / 2 + 5) + '" text-anchor="middle" font-size="13" font-weight="600" fill="' + textColor + '" font-family="system-ui, sans-serif">' + node.label + '</text>');
        });

        svgParts.push('</svg>');
        block.innerHTML = svgParts.join('');
    });
}
window.mermaid = { initialize: function() { renderLocalMermaid(); } };

// Checkbox two-way sync to Notepad++ Scintilla
function handleTaskCheck(cb) {
    var line = cb.getAttribute("data-line");
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleCheckbox:" + line);
    }
}

function fallbackCopy(text, onSuccess) {
    var ta = document.createElement("textarea");
    ta.value = text;
    ta.style.position = "fixed";
    ta.style.left = "-9999px";
    ta.style.top = "-9999px";
    ta.style.opacity = "0";
    document.body.appendChild(ta);
    ta.focus();
    ta.select();
    try {
        document.execCommand("copy");
        if (onSuccess) onSuccess();
    } catch(e) {}
    document.body.removeChild(ta);
}

// Copy single code block
function copyCodeBlock(btn) {
    var card = btn.closest(".code-block-card");
    if (!card) return;
    var code = card.querySelector("pre code");
    if (!code) code = card.querySelector("pre");
    if (!code) return;
    var text = code.innerText;
    var onSuccess = function() {
        var oldText = btn.innerHTML;
        btn.innerHTML = "✓ Copied!";
        btn.classList.add("copied");
        setTimeout(function() {
            btn.innerHTML = oldText;
            btn.classList.remove("copied");
        }, 1800);
    };
    if (navigator.clipboard && navigator.clipboard.writeText) {
        navigator.clipboard.writeText(text).then(onSuccess).catch(function() {
            fallbackCopy(text, onSuccess);
        });
    } else {
        fallbackCopy(text, onSuccess);
    }
}

// Toast notification
function showToast(text) {
    var toast = document.getElementById("toast-msg");
    if (!toast) return;
    toast.textContent = text;
    toast.classList.add("show");
    if (window._toastTimer) clearTimeout(window._toastTimer);
    window._toastTimer = setTimeout(function() {
        toast.classList.remove("show");
    }, 2000);
}

// Copy full HTML
function copyFullHtml() {
    var c = document.getElementById("content-container").innerHTML;
    var onSuccess = function() {
        showToast("📋 Rendered HTML copied to clipboard");
    };
    if (navigator.clipboard && navigator.clipboard.writeText) {
        navigator.clipboard.writeText(c).then(onSuccess).catch(function() {
            fallbackCopy(c, onSuccess);
        });
    } else {
        fallbackCopy(c, onSuccess);
    }
}

function copyFullHtmlFromMenu() {
    closeContextMenu();
    copyFullHtml();
}

function copySelectionFromMenu() {
    closeContextMenu();
    var sel = window._selectedText || (window.getSelection() ? window.getSelection().toString() : "");
    if (!sel) return;
    var onSuccess = function() {
        showToast("✂️ Selection copied");
    };
    if (navigator.clipboard && navigator.clipboard.writeText) {
        navigator.clipboard.writeText(sel).then(onSuccess).catch(function() {
            fallbackCopy(sel, onSuccess);
        });
    } else {
        fallbackCopy(sel, onSuccess);
    }
}

// Theme toggle
function toggleTheme() {
    document.body.classList.toggle("dark");
    renderLocalMermaid();
}

function toggleThemeFromMenu() {
    closeContextMenu();
    toggleTheme();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleTheme");
    }
}

window.onThemeChanged = function() {
    renderLocalMermaid();
};

function toggleSyncFromMenu() {
    closeContextMenu();
    window._syncEnabled = !window._syncEnabled;
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleSync");
    }
    showToast(window._syncEnabled ? "🔄 Caret sync enabled" : "🔄 Caret sync disabled");
}

var _clientZoom = 1.0;
function applyClientZoom(factor) {
    _clientZoom = Math.min(3.0, Math.max(0.3, factor));
    var container = document.getElementById("content-container");
    if (container) {
        container.style.zoom = _clientZoom;
    }
}

function zoomInFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomIn");
    } else {
        applyClientZoom(_clientZoom + 0.1);
    }
}

function zoomOutFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomOut");
    } else {
        applyClientZoom(_clientZoom - 0.1);
    }
}

function zoomResetFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomReset");
    } else {
        applyClientZoom(1.0);
    }
}

function toggleBiDiFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleBiDi");
    } else {
        var container = document.getElementById("content-container");
        if (container) {
            var isRtl = container.getAttribute("dir") === "rtl";
            container.setAttribute("dir", isRtl ? "ltr" : "rtl");
            showToast(isRtl ? "🌐 LTR direction set" : "🌐 RTL direction set");
        }
    }
}

function saveAsHtmlFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("saveAsHtml");
    } else {
        var html = "<!DOCTYPE html>\n" + document.documentElement.outerHTML;
        var blob = new Blob([html], { type: "text/html;charset=utf-8" });
        var a = document.createElement("a");
        a.href = URL.createObjectURL(blob);
        a.download = (document.title ? document.title.replace(/[\/\\?%*:|"<>]/g, '_') : "document") + ".html";
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        showToast("💾 HTML exported");
    }
}

function printFromMenu() {
    closeContextMenu();
    window.print();
}

// TOC Sticky Sidebar & Drawer
function closeToc() {
    document.body.classList.remove("toc-open");
    var drawer = document.getElementById("toc-drawer");
    var backdrop = document.getElementById("toc-backdrop");
    if (drawer) drawer.classList.remove("open");
    if (backdrop) backdrop.classList.remove("open");
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("tocStateChanged:false");
    }
}

function openToc() {
    closeContextMenu();
    document.body.classList.add("toc-open");
    var drawer = document.getElementById("toc-drawer");
    var backdrop = document.getElementById("toc-backdrop");
    if (drawer) drawer.classList.add("open");
    if (backdrop) backdrop.classList.add("open");
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("tocStateChanged:true");
    }
}

function toggleToc() {
    if (document.body.classList.contains("toc-open")) {
        closeToc();
    } else {
        openToc();
    }
}

function toggleTocFromMenu() {
    closeContextMenu();
    toggleToc();
}

function onTocClick(id) {
    var el = document.getElementById(id);
    if (el) {
        el.scrollIntoView({ behavior: 'smooth', block: 'start' });
    }
    if (window.innerWidth <= 650) {
        closeToc();
    }
}

// Custom Context Menu
var contextMenu = null;

function showContextMenu(x, y) {
    if (!contextMenu) contextMenu = document.getElementById("custom-context-menu");
    if (!contextMenu) return;

    var sel = window.getSelection() ? window.getSelection().toString() : "";
    window._selectedText = sel;
    var copySelItem = document.getElementById("menu-copy-selection");
    if (copySelItem) {
        copySelItem.style.display = sel.length > 0 ? "flex" : "none";
    }

    // Dynamic state feedback in menu items
    var isDark = document.body.classList.contains("dark");
    var themeLabel = document.getElementById("menu-theme-label");
    if (themeLabel) {
        themeLabel.innerText = isDark ? "Switch to Light Theme" : "Switch to Dark Theme";
    }

    var syncLabel = document.getElementById("menu-sync-label");
    if (syncLabel) {
        syncLabel.innerText = window._syncEnabled ? "Caret Sync: Enabled" : "Caret Sync: Disabled";
    }

    var tocLabel = document.getElementById("menu-toc-label");
    if (tocLabel) {
        var isTocOpen = document.body.classList.contains("toc-open");
        tocLabel.innerText = isTocOpen ? "Hide Outline / TOC" : "Show Outline / TOC";
    }

    var bidiLabel = document.getElementById("menu-bidi-label");
    if (bidiLabel) {
        var cont = document.getElementById("content-container");
        var isRtl = cont && cont.getAttribute("dir") === "rtl";
        bidiLabel.innerText = isRtl ? "Smart BiDi (Switch to LTR)" : "Smart BiDi (Persian/Arabic RTL)";
    }

    contextMenu.style.display = "block";

    var menuWidth = contextMenu.offsetWidth || 240;
    var menuHeight = contextMenu.offsetHeight || 260;

    var posX = x;
    var posY = y;

    if (posX + menuWidth > window.innerWidth) {
        posX = window.innerWidth - menuWidth - 8;
    }
    if (posY + menuHeight > window.innerHeight) {
        posY = window.innerHeight - menuHeight - 8;
    }
    if (posX < 8) posX = 8;
    if (posY < 8) posY = 8;

    contextMenu.style.left = posX + "px";
    contextMenu.style.top = posY + "px";
}

function closeContextMenu() {
    if (!contextMenu) contextMenu = document.getElementById("custom-context-menu");
    if (contextMenu) {
        contextMenu.style.display = "none";
    }
}

window.addEventListener("contextmenu", function(e) {
    e.preventDefault();
    showContextMenu(e.clientX, e.clientY);
});

document.addEventListener("click", function(e) {
    if (contextMenu && !contextMenu.contains(e.target)) {
        closeContextMenu();
    }
});

window.addEventListener("scroll", function() {
    closeContextMenu();
}, true);

// Scroll to Scintilla source line
window.scrollToSourceLine = function(targetLine) {
    var el = document.querySelector('[data-source-line="' + targetLine + '"]');
    if (!el) {
        var all = document.querySelectorAll('[data-source-line]');
        var minDiff = 999999;
        for (var i = 0; i < all.length; i++) {
            var l = parseInt(all[i].getAttribute('data-source-line'));
            var diff = Math.abs(l - targetLine);
            if (diff < minDiff) {
                minDiff = diff;
                el = all[i];
            }
        }
    }
    if (el) {
        el.scrollIntoView({ behavior: 'smooth', block: 'center' });
    }
};

// In-Page Realtime Search Overlay
var searchMatches = [];
var currentMatchIndex = -1;

function openSearch() {
    closeContextMenu();
    var overlay = document.getElementById("search-overlay");
    if (overlay) {
        overlay.style.display = "flex";
        var input = document.getElementById("search-input");
        if (input) {
            input.focus();
            input.select();
            if (input.value) doSearch();
        }
    }
}

function closeSearch() {
    var overlay = document.getElementById("search-overlay");
    if (overlay) overlay.style.display = "none";
    clearSearch();
}

function toggleSearch() {
    var overlay = document.getElementById("search-overlay");
    if (overlay && overlay.style.display === "flex") {
        closeSearch();
    } else {
        openSearch();
    }
}

function openSearchFromMenu() {
    closeContextMenu();
    openSearch();
}

function clearSearch() {
    var container = document.getElementById("content-container");
    if (!container) return;
    var marks = container.querySelectorAll("mark.search-match");
    marks.forEach(function(m) {
        var parent = m.parentNode;
        if (parent) {
            parent.replaceChild(document.createTextNode(m.textContent), m);
        }
    });
    container.normalize();
    searchMatches = [];
    currentMatchIndex = -1;
    var countEl = document.getElementById("search-count");
    if (countEl) countEl.innerText = "0/0";
}

function doSearch() {
    clearSearch();
    var input = document.getElementById("search-input");
    if (!input) return;
    var query = input.value.trim();
    if (!query) return;

    var container = document.getElementById("content-container");
    if (!container) return;
    var walker = document.createTreeWalker(container, NodeFilter.SHOW_TEXT, null, false);
    var textNodes = [];
    while (walker.nextNode()) {
        if (walker.currentNode.parentElement.closest("pre, code, #custom-context-menu, #search-overlay, #toc-drawer, #toast-msg")) continue;
        textNodes.push(walker.currentNode);
    }

    var qLower = query.toLowerCase();
    textNodes.forEach(function(node) {
        var text = node.nodeValue;
        var tLower = text.toLowerCase();
        var idx = tLower.indexOf(qLower);
        if (idx !== -1) {
            var frag = document.createDocumentFragment();
            var lastIdx = 0;
            while (idx !== -1) {
                if (idx > lastIdx) {
                    frag.appendChild(document.createTextNode(text.substring(lastIdx, idx)));
                }
                var mark = document.createElement("mark");
                mark.className = "search-match";
                mark.textContent = text.substr(idx, query.length);
                frag.appendChild(mark);
                searchMatches.push(mark);
                lastIdx = idx + query.length;
                idx = tLower.indexOf(qLower, lastIdx);
            }
            if (lastIdx < text.length) {
                frag.appendChild(document.createTextNode(text.substring(lastIdx)));
            }
            if (node.parentNode) {
                node.parentNode.replaceChild(frag, node);
            }
        }
    });

    var countEl = document.getElementById("search-count");
    if (searchMatches.length > 0) {
        currentMatchIndex = 0;
        updateActiveMatch();
    } else {
        if (countEl) countEl.innerText = "0/0";
    }
}

function updateActiveMatch() {
    searchMatches.forEach(function(m) { m.classList.remove("active"); });
    if (currentMatchIndex >= 0 && currentMatchIndex < searchMatches.length) {
        var active = searchMatches[currentMatchIndex];
        active.classList.add("active");
        active.scrollIntoView({ behavior: 'smooth', block: 'center' });
        var countEl = document.getElementById("search-count");
        if (countEl) countEl.innerText = (currentMatchIndex + 1) + "/" + searchMatches.length;
    }
}

function nextMatch() {
    if (searchMatches.length === 0) return;
    currentMatchIndex = (currentMatchIndex + 1) % searchMatches.length;
    updateActiveMatch();
}

function prevMatch() {
    if (searchMatches.length === 0) return;
    currentMatchIndex = (currentMatchIndex - 1 + searchMatches.length) % searchMatches.length;
    updateActiveMatch();
}

function onSearchKey(e) {
    if (e.key === "Enter") {
        if (e.shiftKey) prevMatch();
        else nextMatch();
    } else if (e.key === "Escape") {
        closeSearch();
    }
}

document.addEventListener("keydown", function(e) {
    if (e.key === "Escape") {
        if (contextMenu && contextMenu.style.display === "block") {
            closeContextMenu();
            return;
        }
        var searchOverlay = document.getElementById("search-overlay");
        if (searchOverlay && searchOverlay.style.display === "flex") {
            closeSearch();
            return;
        }
        var tocDrawer = document.getElementById("toc-drawer");
        if (tocDrawer && (tocDrawer.classList.contains("open") || document.body.classList.contains("toc-open"))) {
            closeToc();
            return;
        }
    }
    if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === "f") {
        e.preventDefault();
        openSearch();
    }
    if (e.ctrlKey || e.metaKey) {
        if (e.key === "+" || e.key === "=") {
            e.preventDefault();
            zoomInFromMenu();
        } else if (e.key === "-") {
            e.preventDefault();
            zoomOutFromMenu();
        } else if (e.key === "0") {
            e.preventDefault();
            zoomResetFromMenu();
        }
    }
});

// Initialization & WebView2 Host Communication
document.addEventListener("DOMContentLoaded", function() {
    closeToc();
    renderLocalMath();
    renderLocalMermaid();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("pageLoaded");
    }
});

if (window.chrome && window.chrome.webview) {
    window.chrome.webview.addEventListener("message", function(event) {
        var data = event.data;
        if (typeof data === "string") {
            try { data = JSON.parse(data); } catch(e) {}
        }
        if (data && data.type === "updateContent") {
            if (typeof data.title === "string" && data.title) {
                document.title = data.title;
            }
            if (typeof data.isDark === "boolean") {
                if (data.isDark) document.body.classList.add("dark");
                else document.body.classList.remove("dark");
            }
            var content = document.getElementById("content-container");
            if (content && typeof data.body === "string") {
                content.innerHTML = data.body;
            }
            var toc = document.querySelector("#toc-drawer .toc-content");
            if (toc && typeof data.toc === "string") {
                toc.innerHTML = data.toc;
            }
            renderLocalMath();
            renderLocalMermaid();
            if (typeof doSearch === "function") {
                var searchInput = document.getElementById("search-input");
                if (searchInput && searchInput.value) doSearch();
            }
        }
    });
}
</script>
</body>
</html>
)HTML";

    return PreviewComponents{ html.str(), bodyHtmlUtf8, tocHtmlUtf8 };
}

std::string HtmlExporter::GeneratePreviewHtml(
    const MarkdownDocument& doc,
    const std::wstring& title,
    bool isDarkMode,
    float zoomLevel,
    bool isSyncEnabled
) {
    return GeneratePreviewComponents(doc, title, isDarkMode, zoomLevel, isSyncEnabled).fullHtml;
}

std::wstring HtmlExporter::ExportToHtml(const MarkdownDocument& doc, const std::wstring& title, bool isDarkMode) {
    std::string previewHtml = GeneratePreviewHtml(doc, title, isDarkMode, 1.0f, false);
    return Utf8ToWide(previewHtml);
}

bool HtmlExporter::SaveToFile(const std::wstring& filePath, const MarkdownDocument& doc, const std::wstring& title, bool isDarkMode) {
    std::string html = GeneratePreviewHtml(doc, title, isDarkMode, 1.0f, false);
    std::ofstream out(filePath.c_str(), std::ios::binary);
    if (!out.is_open()) return false;
    out.write(html.data(), html.size());
    return true;
}

bool HtmlExporter::CopyToClipboard(HWND hwndOwner, const MarkdownDocument& doc, const std::wstring& title, bool isDarkMode) {
    std::string html = GeneratePreviewHtml(doc, title, isDarkMode, 1.0f, false);
    std::string cfHtml = GenerateCfHtml(html);

    if (!OpenClipboard(hwndOwner)) return false;
    EmptyClipboard();

    UINT cfHtmlFormat = RegisterClipboardFormatA("HTML Format");
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, cfHtml.size() + 1);
    if (hMem) {
        void* pData = GlobalLock(hMem);
        if (pData) {
            memcpy(pData, cfHtml.c_str(), cfHtml.size() + 1);
            GlobalUnlock(hMem);
            SetClipboardData(cfHtmlFormat, hMem);
        }
    }

    CloseClipboard();
    return true;
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
                ss << L"<span class=\"katex-inline\" dir=\"ltr\" data-tex=\""
                   << EscapeHtml(span.text) << L"\"><bdi dir=\"ltr\">$"
                   << EscapeHtml(span.text) << L"$</bdi></span>";
                break;
            case InlineStyleType::Link:
                ss << L"<a href=\"" << EscapeHtml(span.extra) << L"\">" << EscapeHtml(span.text) << L"</a>";
                break;
            case InlineStyleType::Image:
                ss << L"<img src=\"" << EscapeHtml(span.extra) << L"\" alt=\"" << EscapeHtml(span.text) << L"\" style=\"max-width:100%; border-radius:6px;\" />";
                break;
            case InlineStyleType::Normal:
            default:
                ss << EscapeHtml(span.text);
                break;
        }
    }
    return ss.str();
}
