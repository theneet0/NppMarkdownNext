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
    /* Material Design 3 Baseline Light Tokens */
    --md-sys-color-surface: #fdfcff;
    --md-sys-color-surface-dim: #ded8e1;
    --md-sys-color-surface-bright: #fdfcff;
    --md-sys-color-surface-container-lowest: #ffffff;
    --md-sys-color-surface-container-low: #f7f2fa;
    --md-sys-color-surface-container: #f1ecf4;
    --md-sys-color-surface-container-high: #ece6ee;
    --md-sys-color-surface-container-highest: #e6e0e9;
    --md-sys-color-on-surface: #1d1b20;
    --md-sys-color-on-surface-variant: #49454f;
    --md-sys-color-outline: #79747e;
    --md-sys-color-outline-variant: #cac4d0;
    --md-sys-color-primary: #005ac1;
    --md-sys-color-on-primary: #ffffff;
    --md-sys-color-primary-container: #d8e2ff;
    --md-sys-color-on-primary-container: #001a41;
    --md-sys-color-secondary: #575e71;
    --md-sys-color-secondary-container: #dbe2f9;
    --md-sys-color-on-secondary-container: #141b2c;
    --md-sys-color-tertiary: #715573;
    --md-sys-color-tertiary-container: #fbd7fc;
    --md-sys-color-on-tertiary-container: #29132d;
    --md-sys-color-error: #ba1a1a;
    --md-sys-color-error-container: #ffdad6;

    --bg-page: var(--md-sys-color-surface);
    --text-primary: var(--md-sys-color-on-surface);
    --text-secondary: var(--md-sys-color-on-surface-variant);
    --border-color: var(--md-sys-color-outline-variant);
    --code-bg: var(--md-sys-color-surface-container);
    --code-header-bg: var(--md-sys-color-surface-container-high);
    --code-border: var(--md-sys-color-outline-variant);
    --quote-bg: var(--md-sys-color-surface-container-low);
    --quote-bar: var(--md-sys-color-primary);
    --table-alt: var(--md-sys-color-surface-container-lowest);
    --table-header: var(--md-sys-color-surface-container-high);
    --link-color: var(--md-sys-color-primary);
    --toolbar-bg: rgba(247, 242, 250, 0.92);
    --toolbar-border: var(--md-sys-color-outline-variant);
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
    --search-match: #ffe082;
    --search-active: #ffb300;

    /* M3 Alert Callouts */
    --alert-note-bg: #edf3fd;
    --alert-note-bar: #005ac1;
    --alert-tip-bg: #eaf7ed;
    --alert-tip-bar: #198754;
    --alert-important-bg: #f8effb;
    --alert-important-bar: #8338ec;
    --alert-warning-bg: #fff8e6;
    --alert-warning-bar: #d97706;
    --alert-caution-bg: #fdf0f0;
    --alert-caution-bar: #dc2626;
    --drawer-bg: var(--md-sys-color-surface-container-low);
}

body.dark {
    /* Material Design 3 Baseline Dark Tokens */
    --md-sys-color-surface: #141218;
    --md-sys-color-surface-dim: #141218;
    --md-sys-color-surface-bright: #3b383e;
    --md-sys-color-surface-container-lowest: #0f0d13;
    --md-sys-color-surface-container-low: #1d1b20;
    --md-sys-color-surface-container: #211f26;
    --md-sys-color-surface-container-high: #2b2930;
    --md-sys-color-surface-container-highest: #36343b;
    --md-sys-color-on-surface: #e6e0e9;
    --md-sys-color-on-surface-variant: #cac4d0;
    --md-sys-color-outline: #938f99;
    --md-sys-color-outline-variant: #49454f;
    --md-sys-color-primary: #adc6ff;
    --md-sys-color-on-primary: #002e69;
    --md-sys-color-primary-container: #004494;
    --md-sys-color-on-primary-container: #d8e2ff;
    --md-sys-color-secondary: #bfc6dc;
    --md-sys-color-secondary-container: #3f4759;
    --md-sys-color-on-secondary-container: #dbe2f9;
    --md-sys-color-tertiary: #debcdf;
    --md-sys-color-tertiary-container: #583e5a;
    --md-sys-color-on-tertiary-container: #fbd7fc;
    --md-sys-color-error: #ffb4ab;
    --md-sys-color-error-container: #93000a;

    --bg-page: var(--md-sys-color-surface);
    --text-primary: var(--md-sys-color-on-surface);
    --text-secondary: var(--md-sys-color-on-surface-variant);
    --border-color: var(--md-sys-color-outline-variant);
    --code-bg: var(--md-sys-color-surface-container);
    --code-header-bg: var(--md-sys-color-surface-container-high);
    --code-border: var(--md-sys-color-outline-variant);
    --quote-bg: var(--md-sys-color-surface-container-low);
    --quote-bar: var(--md-sys-color-primary);
    --table-alt: var(--md-sys-color-surface-container);
    --table-header: var(--md-sys-color-surface-container-high);
    --link-color: var(--md-sys-color-primary);
    --toolbar-bg: rgba(29, 27, 32, 0.92);
    --toolbar-border: var(--md-sys-color-outline-variant);
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.35);
    --search-match: #5d4037;
    --search-active: #ffb300;

    --alert-note-bg: #132438;
    --alert-note-bar: #adc6ff;
    --alert-tip-bg: #142a1d;
    --alert-tip-bar: #4ade80;
    --alert-important-bg: #291a38;
    --alert-important-bar: #c084fc;
    --alert-warning-bg: #2e200c;
    --alert-warning-bar: #fbbf24;
    --alert-caution-bg: #301314;
    --alert-caution-bar: #f87171;
    --drawer-bg: var(--md-sys-color-surface-container-low);
}

* { box-sizing: border-box; }

body {
    margin: 0;
    padding: 0;
    background-color: var(--bg-page);
    color: var(--text-primary);
    font-family: "Vazirmatn", "Roboto", -apple-system, BlinkMacSystemFont, "Segoe UI", Tahoma, system-ui, sans-serif;
    font-size: 15px;
    line-height: 1.7;
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
    max-width: 920px;
    margin: 0 auto;
    padding: 24px 36px 120px 36px;
    word-wrap: break-word;
    overflow-wrap: break-word;
    transition: max-width 0.2s ease, padding 0.2s ease;
}

/* Material 3 Floating Search Bar */
#search-overlay {
    position: fixed;
    top: 14px;
    right: 18px;
    z-index: 9990;
    display: none;
    align-items: center;
    gap: 6px;
    padding: 6px 14px;
    background: var(--toolbar-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color);
    border-radius: 28px;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.12);
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
    border-radius: 18px;
    padding: 4px 12px;
    font-size: 12.5px;
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
    background: none;
    border: none;
    color: var(--text-secondary);
    cursor: pointer;
    font-size: 11px;
    padding: 4px 6px;
    border-radius: 50%;
    transition: background 0.15s ease, color 0.15s ease;
}

.search-btn:hover {
    color: var(--text-primary);
    background: rgba(128, 128, 128, 0.15);
}

.search-close {
    font-size: 13px;
    margin-left: 2px;
}

.search-highlight {
    background-color: var(--search-match);
    color: inherit;
    border-radius: 3px;
    padding: 0 2px;
}

.search-highlight.active {
    background-color: var(--search-active);
    color: #ffffff;
    font-weight: 600;
}

/* Material 3 Context Menu */
.context-menu {
    position: fixed;
    z-index: 10000;
    background: var(--toolbar-bg);
    backdrop-filter: blur(24px);
    -webkit-backdrop-filter: blur(24px);
    border: 1px solid var(--border-color);
    border-radius: 14px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.16);
    padding: 6px;
    min-width: 190px;
    user-select: none;
    animation: menuFadeIn 0.12s cubic-bezier(0, 0, 0.2, 1);
    display: none;
}

@keyframes menuFadeIn {
    from { opacity: 0; transform: scale(0.96); }
    to { opacity: 1; transform: scale(1); }
}

.menu-item {
    display: flex;
    align-items: center;
    gap: 10px;
    padding: 8px 12px;
    border-radius: 8px;
    cursor: pointer;
    color: var(--text-primary);
    font-size: 13px;
    transition: background-color 0.15s ease, color 0.15s ease;
}

.menu-item:hover {
    background: rgba(0, 90, 193, 0.08);
    color: var(--link-color);
}

body.dark .menu-item:hover {
    background: rgba(173, 198, 255, 0.12);
}

.menu-icon {
    width: 18px;
    height: 18px;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    flex-shrink: 0;
    color: var(--md-sys-color-on-surface-variant);
}

.menu-svg {
    width: 15px;
    height: 15px;
    stroke: currentColor;
    stroke-width: 2;
    stroke-linecap: round;
    stroke-linejoin: round;
    fill: none;
    transition: transform 0.15s cubic-bezier(0, 0, 0.2, 1), color 0.15s ease;
}

.menu-item:hover .menu-svg {
    transform: scale(1.12);
    color: var(--link-color);
}

.search-icon-wrap {
    display: inline-flex;
    align-items: center;
    justify-content: center;
    color: var(--md-sys-color-on-surface-variant);
}

.toc-title-icon {
    display: inline-flex;
    align-items: center;
    justify-content: center;
    color: var(--link-color);
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

/* Material 3 Toast Notification (Snackbar style) */
.toast-notification {
    position: fixed;
    bottom: 24px;
    left: 50%;
    transform: translateX(-50%) translateY(20px);
    background: var(--toolbar-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color);
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.18);
    color: var(--text-primary);
    padding: 8px 20px;
    border-radius: 24px;
    font-size: 12.5px;
    z-index: 10002;
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.25s ease, transform 0.25s ease;
}

.toast-notification.show {
    opacity: 1;
    transform: translateX(-50%) translateY(0);
}

/* Material 3 Table of Contents Navigation Drawer */
#toc-backdrop {
    display: none;
    position: fixed;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background: rgba(0, 0, 0, 0.25);
    backdrop-filter: blur(2px);
    z-index: 9980;
}

#toc-drawer {
    width: 280px;
    background: var(--drawer-bg);
    border-left: 1px solid var(--border-color);
    box-shadow: -4px 0 20px rgba(0, 0, 0, 0.06);
    display: flex;
    flex-direction: column;
    position: sticky;
    top: 0;
    height: 100vh;
    overflow: hidden;
    z-index: 9985;
    flex-shrink: 0;
    transition: transform 0.25s cubic-bezier(0, 0, 0.2, 1);
}

body:not(.toc-open) #toc-drawer {
    display: none;
}

@media (max-width: 650px) {
    #toc-drawer {
        position: fixed;
        right: 0;
        top: 0;
        height: 100vh;
        transform: translateX(100%);
    }
    #toc-drawer.open {
        transform: translateX(0);
    }
    #toc-backdrop.open {
        display: block;
    }
}

.toc-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 14px 18px;
    border-bottom: 1px solid var(--border-color);
    font-weight: 600;
    font-size: 13.5px;
}

.toc-title {
    display: flex;
    align-items: center;
    gap: 8px;
    color: var(--text-primary);
}

.toc-close {
    cursor: pointer;
    background: none;
    border: none;
    font-size: 16px;
    color: var(--text-secondary);
    padding: 4px;
    border-radius: 50%;
    display: flex;
    align-items: center;
    justify-content: center;
    transition: color 0.15s ease, background-color 0.15s ease;
}

.toc-close:hover {
    color: var(--text-primary);
    background: rgba(128, 128, 128, 0.15);
}

.toc-content {
    flex: 1 1 0;
    overflow-y: auto;
    padding: 12px 14px;
}

.toc-item {
    display: block;
    color: var(--text-secondary);
    text-decoration: none;
    font-size: 12.5px;
    padding: 6px 12px;
    border-radius: 20px;
    margin-bottom: 3px;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    transition: all 0.15s ease;
    unicode-bidi: plaintext;
    text-align: start;
}

.toc-item:hover {
    color: var(--link-color);
    background: rgba(0, 90, 193, 0.06);
}

.toc-item.active {
    color: var(--md-sys-color-on-primary-container);
    font-weight: 600;
    background: var(--md-sys-color-primary-container);
}

.toc-l1 { padding-inline-start: 10px; font-weight: 600; }
.toc-l2 { padding-inline-start: 22px; }
.toc-l3 { padding-inline-start: 34px; }
.toc-l4 { padding-inline-start: 46px; }

/* Material 3 Typography */
h1, h2, h3, h4, h5, h6 {
    margin-top: 32px;
    margin-bottom: 14px;
    font-weight: 600;
    line-height: 1.35;
    scroll-margin-top: 60px;
    color: var(--text-primary);
}

h1 { font-size: 1.9em; letter-spacing: -0.4px; }
h2 { font-size: 1.45em; letter-spacing: -0.2px; }
h3 { font-size: 1.2em; }
h4 { font-size: 1.05em; }

p { margin-top: 0; margin-bottom: 16px; }

a { color: var(--link-color); text-decoration: none; transition: color 0.15s ease; }
a:hover { text-decoration: underline; }

hr {
    height: 1px;
    background-color: var(--border-color);
    border: none;
    margin: 28px 0;
}

/* Material 3 Blockquotes */
blockquote {
    margin: 18px 0;
    padding: 12px 18px;
    background: var(--quote-bg);
    border-inline-start: 4px solid var(--quote-bar);
    border-radius: 10px;
    color: var(--text-primary);
}

blockquote p {
    margin: 0;
}

/* Material 3 Minimal Code Blocks */
.code-block-card {
    margin: 18px 0;
    background: var(--code-bg);
    border: 1px solid var(--code-border);
    border-radius: 14px;
    overflow: hidden;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.04);
    transition: border-color 0.2s ease, box-shadow 0.2s ease;
}

.code-block-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 7px 14px;
    background: var(--code-header-bg);
    border-bottom: 1px solid var(--code-border);
    user-select: none;
    direction: ltr !important;
}

.lang-badge {
    display: inline-flex;
    align-items: center;
    padding: 3px 8px;
    border-radius: 6px;
    background: var(--md-sys-color-surface-container-highest, rgba(128, 128, 128, 0.15));
    font-size: 11px;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace;
    font-weight: 600;
    color: var(--text-secondary);
    text-transform: uppercase;
    letter-spacing: 0.6px;
}

.copy-btn {
    display: inline-flex;
    align-items: center;
    gap: 6px;
    background: transparent;
    border: 1px solid var(--border-color);
    border-radius: 8px;
    color: var(--text-primary);
    font-size: 11.5px;
    font-weight: 500;
    padding: 4px 10px;
    cursor: pointer;
    transition: all 0.2s ease;
}

.copy-btn:hover {
    background: rgba(0, 90, 193, 0.08);
    border-color: var(--link-color);
    color: var(--link-color);
}

body.dark .copy-btn:hover {
    background: rgba(173, 198, 255, 0.12);
}

.copy-btn.copied {
    background: #198754;
    color: #ffffff;
    border-color: #198754;
}

.copy-btn svg {
    flex-shrink: 0;
}

pre {
    margin: 0;
    padding: 14px 18px;
    overflow-x: auto;
    font-family: "Cascadia Code", "Cascadia Mono", Consolas, "Courier New", "Vazirmatn", monospace;
    font-size: 13.5px;
    line-height: 1.65;
    tab-size: 4;
    white-space: pre-wrap;
    word-break: break-word;
}

pre code {
    background: transparent;
    border: none;
    padding: 0;
    font-size: 100%;
    display: block;
}

.code-line {
    display: block;
    min-height: 1.45em;
    unicode-bidi: isolate;
}

.code-line[dir="rtl"] {
    direction: rtl;
    text-align: right;
}

.code-line[dir="ltr"] {
    direction: ltr;
    text-align: left;
}

code.inline-code, code {
    font-family: "Cascadia Code", "Cascadia Mono", Consolas, "Courier New", "Vazirmatn", monospace;
    font-size: 88%;
    padding: 0.15em 0.45em;
    background-color: var(--code-bg);
    border: 1px solid var(--border-color);
    border-radius: 6px;
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
.hl-keyword { color: #b3261e; font-weight: 600; }
.hl-type { color: #7d5260; }
.hl-string { color: #198754; }
.hl-comment { color: #79747e; font-style: italic; }
.hl-number { color: #005ac1; }
.hl-preprocessor { color: #8338ec; }
.hl-operator { color: #1d1b20; }

body.dark .hl-keyword { color: #ffb4ab; font-weight: 600; }
body.dark .hl-type { color: #debcdf; }
body.dark .hl-string { color: #4ade80; }
body.dark .hl-comment { color: #938f99; font-style: italic; }
body.dark .hl-number { color: #adc6ff; }
body.dark .hl-preprocessor { color: #c084fc; }
body.dark .hl-operator { color: #e6e0e9; }

/* Material 3 Alert Callouts */
.alert-callout {
    border-radius: 14px;
    padding: 14px 18px;
    margin: 18px 0;
    border: 1px solid var(--border-color);
    box-shadow: 0 1px 2px rgba(0, 0, 0, 0.03);
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
    font-weight: 600;
    font-size: 13.5px;
    margin-bottom: 6px;
}

.alert-header svg { flex-shrink: 0; }
.alert-callout.note .alert-header { color: var(--alert-note-bar); }
.alert-callout.tip .alert-header { color: var(--alert-tip-bar); }
.alert-callout.important .alert-header { color: var(--alert-important-bar); }
.alert-callout.warning .alert-header { color: var(--alert-warning-bar); }
.alert-callout.caution .alert-header { color: var(--alert-caution-bar); }

/* Material 3 Tables */
table {
    border-collapse: separate;
    border-spacing: 0;
    width: 100%;
    margin: 20px 0;
    border-radius: 12px;
    overflow: hidden;
    border: 1px solid var(--border-color);
}

th, td {
    border-bottom: 1px solid var(--border-color);
    border-inline-end: 1px solid var(--border-color);
    padding: 10px 16px;
}

th:last-child, td:last-child {
    border-inline-end: none;
}

tr:last-child td {
    border-bottom: none;
}

th {
    background-color: var(--table-header);
    font-weight: 600;
}

tr:nth-child(2n) td {
    background-color: var(--table-alt);
}

/* Material 3 Task Lists */
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
    gap: 10px;
    margin: 7px 0;
}

.task-checkbox {
    width: 18px;
    height: 18px;
    margin-top: 3px;
    cursor: pointer;
    accent-color: var(--link-color);
    border-radius: 4px;
}

/* Highlight and Math */
mark {
    background-color: var(--md-sys-color-primary-container);
    color: var(--md-sys-color-on-primary-container);
    padding: 0.15em 0.4em;
    border-radius: 6px;
}

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
    border-radius: 14px;
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
                const wchar_t* alertIconSvg = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 24 24\" fill=\"currentColor\"><path d=\"M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 15h-2v-6h2v6zm0-8h-2V7h2v2z\"/></svg>";
                switch (block.alertType) {
                    case AlertType::Tip:
                        alertCls = L"tip";
                        alertIconSvg = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 24 24\" fill=\"currentColor\"><path d=\"M9 21c0 .55.45 1 1 1h4c.55 0 1-.45 1-1v-1H9v1zm3-19C8.14 2 5 5.14 5 9c0 2.38 1.19 4.47 3 5.74V17c0 .55.45 1 1 1h6c.55 0 1-.45 1-1v-2.26c1.81-1.27 3-3.36 3-5.74 0-3.86-3.14-7-7-7z\"/></svg>";
                        break;
                    case AlertType::Important:
                        alertCls = L"important";
                        alertIconSvg = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 24 24\" fill=\"currentColor\"><path d=\"M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm-1 14h2v2h-2v-2zm0-10h2v8h-2V6z\"/></svg>";
                        break;
                    case AlertType::Warning:
                        alertCls = L"warning";
                        alertIconSvg = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 24 24\" fill=\"currentColor\"><path d=\"M1 21h22L12 2 1 21zm12-3h-2v-2h2v2zm0-4h-2v-4h2v4z\"/></svg>";
                        break;
                    case AlertType::Caution:
                        alertCls = L"caution";
                        alertIconSvg = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 24 24\" fill=\"currentColor\"><path d=\"M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm5 11H7v-2h10v2z\"/></svg>";
                        break;
                    default: break;
                }

                bodyStream << L"<div class=\"alert-callout " << alertCls << L"\" data-source-line=\""
                           << block.sourceLineStart << L"\"" << dirAttr << L">\n";
                bodyStream << L"  <div class=\"alert-header\">" << alertIconSvg << L" <span>" << EscapeHtml(block.alertTitle) << L"</span></div>\n";
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
                    bodyStream << L"    <span class=\"lang-badge\">" << EscapeHtml(langDisplay) << L"</span>\n";
                    bodyStream << L"    <button class=\"copy-btn\" onclick=\"copyCodeBlock(this)\">\n";
                    bodyStream << L"      <svg width=\"13\" height=\"13\" viewBox=\"0 0 24 24\" fill=\"none\" stroke=\"currentColor\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"round\"><rect x=\"9\" y=\"9\" width=\"13\" height=\"13\" rx=\"2\" ry=\"2\"></rect><path d=\"M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1\"></path></svg>\n";
                    bodyStream << L"      <span class=\"copy-text\">Copy</span>\n";
                    bodyStream << L"    </button>\n";
                    bodyStream << L"  </div>\n";
                    bodyStream << L"  <pre><code>";
                    for (size_t i = 0; i < block.codeLines.size(); ++i) {
                        const auto& cl = block.codeLines[i];
                        bool isLineRtl = BiDiEngine::IsParagraphRTL(cl);
                        const wchar_t* lineDir = isLineRtl ? L"dir=\"rtl\"" : L"dir=\"ltr\"";
                        bodyStream << L"<div class=\"code-line\" " << lineDir << L">"
                                   << TokenizeCodeHtml(cl, block.codeLanguage)
                                   << L"</div>";
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
  <span class="search-icon-wrap"><svg viewBox="0 0 24 24" width="14" height="14" stroke="currentColor" stroke-width="2" fill="none"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/></svg></span>
  <input type="text" id="search-input" placeholder="Find in document..." oninput="doSearch()" onkeydown="onSearchKey(event)" />
  <span id="search-count">0/0</span>
  <button class="search-btn" title="Previous match (Shift+Enter)" onclick="prevMatch()"><svg viewBox="0 0 24 24" width="12" height="12" stroke="currentColor" stroke-width="2.5" fill="none"><polyline points="18 15 12 9 6 15"/></svg></button>
  <button class="search-btn" title="Next match (Enter)" onclick="nextMatch()"><svg viewBox="0 0 24 24" width="12" height="12" stroke="currentColor" stroke-width="2.5" fill="none"><polyline points="6 9 12 15 18 9"/></svg></button>
  <button class="search-btn search-close" title="Close (Escape)" onclick="closeSearch()"><svg viewBox="0 0 24 24" width="12" height="12" stroke="currentColor" stroke-width="2.5" fill="none"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg></button>
</div>

<div id="custom-context-menu" class="context-menu" style="display: none;" onmousedown="event.preventDefault()">
  <div class="menu-item" id="menu-copy-selection" style="display: none;" onclick="copySelectionFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><rect x="9" y="9" width="13" height="13" rx="2" ry="2"/><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"/></svg></span>
    <span class="menu-label">Copy Selection</span>
    <span class="menu-shortcut">Ctrl+C</span>
  </div>
  <div class="menu-item" onclick="openSearchFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/></svg></span>
    <span class="menu-label">Find in Document</span>
    <span class="menu-shortcut">Ctrl+F</span>
  </div>
  <div class="menu-item" onclick="toggleTocFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><line x1="8" y1="6" x2="21" y2="6"/><line x1="8" y1="12" x2="21" y2="12"/><line x1="8" y1="18" x2="21" y2="18"/><line x1="3" y1="6" x2="3.01" y2="6"/><line x1="3" y1="12" x2="3.01" y2="12"/><line x1="3" y1="18" x2="3.01" y2="18"/></svg></span>
    <span class="menu-label" id="menu-toc-label">Outline / Table of Contents</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="zoomInFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/><line x1="11" y1="8" x2="11" y2="14"/><line x1="8" y1="11" x2="14" y2="11"/></svg></span>
    <span class="menu-label">Zoom In</span>
    <span class="menu-shortcut">Ctrl++</span>
  </div>
  <div class="menu-item" onclick="zoomOutFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/><line x1="8" y1="11" x2="14" y2="11"/></svg></span>
    <span class="menu-label">Zoom Out</span>
    <span class="menu-shortcut">Ctrl+-</span>
  </div>
  <div class="menu-item" onclick="zoomResetFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M3 12a9 9 0 1 0 9-9 9.75 9.75 0 0 0-6.74 2.74L3 8"/><path d="M3 3v5h5"/></svg></span>
    <span class="menu-label">Reset Zoom (100%)</span>
    <span class="menu-shortcut">Ctrl+0</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="toggleThemeFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="12" cy="12" r="4"/><path d="M12 2v2"/><path d="M12 20v2"/><path d="m4.93 4.93 1.41 1.41"/><path d="m17.66 17.66 1.41 1.41"/><path d="M2 12h2"/><path d="M20 12h2"/><path d="m6.34 17.66-1.41 1.41"/><path d="m19.07 4.93-1.41 1.41"/></svg></span>
    <span class="menu-label" id="menu-theme-label">Toggle Dark / Light Theme</span>
  </div>
  <div class="menu-item" onclick="toggleSyncFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="m17 2 4 4-4 4"/><path d="M3 11v-1a4 4 0 0 1 4-4h14"/><path d="m7 22-4-4 4-4"/><path d="M21 13v1a4 4 0 0 1-4 4H3"/></svg></span>
    <span class="menu-label" id="menu-sync-label">Toggle Caret Sync Scroll</span>
  </div>
  <div class="menu-item" onclick="toggleBiDiFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="m5 8 6 6"/><path d="m4 14 6-6 2-3"/><path d="M2 5h12"/><path d="M7 2h1"/><path d="m22 22-5-10-5 10"/><path d="M14 18h6"/></svg></span>
    <span class="menu-label" id="menu-bidi-label">Smart BiDi (Persian/Arabic RTL)</span>
  </div>
  <div class="menu-separator"></div>
  <div class="menu-item" onclick="copyFullHtmlFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><polyline points="16 18 22 12 16 6"/><polyline points="8 6 2 12 8 18"/></svg></span>
    <span class="menu-label">Copy Full Rendered HTML</span>
  </div>
  <div class="menu-item" onclick="saveAsHtmlFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="7 10 12 15 17 10"/><line x1="12" y1="15" x2="12" y2="3"/></svg></span>
    <span class="menu-label">Save As HTML...</span>
  </div>
  <div class="menu-item" onclick="printFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><polyline points="6 9 6 2 18 2 18 9"/><path d="M6 18H4a2 2 0 0 1-2-2v-5a2 2 0 0 1 2-2h16a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2h-2"/><rect x="6" y="14" width="12" height="8"/></svg></span>
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
        <span class="toc-title-icon"><svg viewBox="0 0 24 24" width="16" height="16" stroke="currentColor" stroke-width="2" fill="none"><line x1="8" y1="6" x2="21" y2="6"/><line x1="8" y1="12" x2="21" y2="12"/><line x1="8" y1="18" x2="21" y2="18"/><line x1="3" y1="6" x2="3.01" y2="6"/><line x1="3" y1="12" x2="3.01" y2="12"/><line x1="3" y1="18" x2="3.01" y2="18"/></svg></span>
        <span>Outline / Table of Contents</span>
      </div>
      <button class="toc-close" title="Close (Escape)" onclick="closeToc()"><svg viewBox="0 0 24 24" width="14" height="14" stroke="currentColor" stroke-width="2" fill="none"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg></button>
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
            var s = raw.trim();
            var m = s.match(/^[\[\(\{]+(?:"([^"]*)"|'([^']*)'|(.*?))[\]\)\}]+$/);
            if (m) {
                return (m[1] !== undefined ? m[1] : (m[2] !== undefined ? m[2] : m[3])).trim();
            }
            return s.replace(/^[\[\(\{]+|[\]\)\}]+$/g, '').replace(/^["']|["']$/g, '').trim();
        }

        function parseNodeExpr(str) {
            str = str.trim();
            var m = str.match(/^([a-zA-Z0-9_\-]+)\s*(.*)$/);
            if (!m) return null;
            var id = m[1];
            var rest = m[2] ? m[2].trim() : "";
            var label = id;
            if (rest.length > 0) {
                label = cleanLabel(rest) || id;
            }
            return { id: id, label: label };
        }

        for (var i = 0; i < lines.length; i++) {
            var line = lines[i];
            if (line.indexOf("graph") === 0 || line.indexOf("flowchart") === 0) continue;

            var arrowMatch = line.match(/\s*(-->|==>|-\.->|---)\s*(?:\|(.*?)\|)?\s*/);
            if (arrowMatch) {
                var arrowIdx = arrowMatch.index;
                var leftStr = line.substring(0, arrowIdx).trim();
                var rightStr = line.substring(arrowIdx + arrowMatch[0].length).trim();
                var edgeLabel = arrowMatch[2] || "";

                var uNode = parseNodeExpr(leftStr);
                var vNode = parseNodeExpr(rightStr);
                if (uNode && vNode) {
                    addNode(uNode.id, uNode.label);
                    addNode(vNode.id, vNode.label);
                    nodes[uNode.id].outEdges.push({ to: vNode.id, label: edgeLabel });
                    nodes[vNode.id].inEdges++;
                    edges.push({ from: uNode.id, to: vNode.id, label: edgeLabel });
                }
            } else {
                var single = parseNodeExpr(line);
                if (single) {
                    addNode(single.id, single.label);
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
        var visitedCount = {};
        while (queue.length > 0) {
            var cur = queue.shift();
            visitedCount[cur] = (visitedCount[cur] || 0) + 1;
            if (visitedCount[cur] > nodeKeys.length + 2) continue; // Cycle breaker
            var curL = layers[cur];
            nodes[cur].outEdges.forEach(function(e) {
                var nxt = e.to;
                if (layers[nxt] === undefined || layers[nxt] < curL + 1) {
                    layers[nxt] = Math.min(curL + 1, nodeKeys.length);
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

        var maxLabelLen = 0;
        nodeKeys.forEach(function(k) {
            if (nodes[k].label.length > maxLabelLen) maxLabelLen = nodes[k].label.length;
        });
        var nodeWidth = 140;
        if (maxLabelLen > 14) {
            nodeWidth = Math.min(240, Math.max(140, maxLabelLen * 8 + 24));
        }
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

// Copy single code block (with per-line preservation)
function copyCodeBlock(btn) {
    var card = btn.closest(".code-block-card");
    if (!card) return;
    var code = card.querySelector("pre code");
    if (!code) code = card.querySelector("pre");
    if (!code) return;
    var lines = code.querySelectorAll(".code-line");
    var text = "";
    if (lines && lines.length > 0) {
        var arr = [];
        for (var i = 0; i < lines.length; i++) {
            arr.push(lines[i].innerText);
        }
        text = arr.join("\n");
    } else {
        text = code.innerText;
    }
    var copyTextSpan = btn.querySelector(".copy-text");
    var originalText = copyTextSpan ? copyTextSpan.textContent : "Copy";
    var onSuccess = function() {
        if (copyTextSpan) copyTextSpan.textContent = "Copied!";
        btn.classList.add("copied");
        setTimeout(function() {
            if (copyTextSpan) copyTextSpan.textContent = originalText;
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
