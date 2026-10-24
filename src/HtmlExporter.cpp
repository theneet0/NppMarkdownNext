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
    cursor: pointer;
    transition: opacity 0.15s ease;
}

h1:hover, h2:hover, h3:hover, h4:hover, h5:hover, h6:hover {
    opacity: 0.82;
}

ol[dir="rtl"] {
    list-style-type: persian;
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

/* Floating theme picker button */
#theme-fab {
    position: fixed;
    right: 16px;
    bottom: 16px;
    width: 34px;
    height: 34px;
    display: flex;
    align-items: center;
    justify-content: center;
    z-index: 9985;
    background: var(--toolbar-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color);
    border-radius: 50%;
    box-shadow: 0 4px 16px rgba(0, 0, 0, 0.18);
    color: var(--text-secondary);
    cursor: pointer;
    transition: color 0.15s ease, border-color 0.15s ease, transform 0.15s ease;
}

#theme-fab:hover {
    color: var(--link-color);
    border-color: var(--link-color);
    transform: rotate(30deg);
}

body.toc-open #theme-fab {
    right: 290px;
}

#theme-popover {
    position: fixed;
    right: 16px;
    bottom: 58px;
    width: 200px;
    max-height: 65vh;
    overflow-y: auto;
    z-index: 9990;
    display: none;
    background: var(--toolbar-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color);
    border-radius: 14px;
    box-shadow: var(--toolbar-shadow);
    padding: 6px;
}

body.toc-open #theme-popover {
    right: 290px;
}

.theme-pop-title {
    font-size: 11px;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.06em;
    color: var(--text-secondary);
    padding: 6px 10px 4px;
    user-select: none;
}

.theme-pop-item {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 7px 10px;
    border-radius: 8px;
    font-size: 13px;
    color: var(--text-primary);
    cursor: pointer;
    user-select: none;
}

.theme-pop-item:hover {
    background: rgba(128, 128, 128, 0.16);
}

.theme-pop-item.checked {
    color: var(--link-color);
    font-weight: 600;
}

.theme-pop-check {
    width: 14px;
    height: 14px;
    display: flex;
    align-items: center;
    justify-content: center;
    flex: 0 0 14px;
}

.theme-pop-label {
    flex: 1;
}

.theme-pop-arrow {
    flex: 0 0 auto;
    display: flex;
    align-items: center;
    margin-left: auto;
    padding-left: 8px;
    opacity: 0.55;
}

@media print {
    #custom-context-menu, #search-overlay, #toc-drawer, #toc-backdrop, #toast-msg, #theme-fab, #theme-popover { display: none !important; }
    #content-container { padding: 0 !important; max-width: 100% !important; }
    body { background: #ffffff !important; color: #000000 !important; }
}
)CSS";

const char* s_themeOverrideCss = R"CSS(
/* === Theme: Sepia === */
body.sepia {
    --bg-page: #f4ecd8;
    --text-primary: #5b4636;
    --text-secondary: #8b7355;
    --border-color: #d4c5a9;
    --code-bg: #efe6d0;
    --code-header-bg: #e8dcc0;
    --code-border: #d4c5a9;
    --quote-bg: #efe6d0;
    --quote-bar: #b8860b;
    --table-alt: #f0e8d5;
    --table-header: #e8dcc0;
    --link-color: #8b4513;
    --toolbar-bg: rgba(244, 236, 216, 0.92);
    --toolbar-border: #d4c5a9;
    --toolbar-shadow: 0 4px 20px rgba(91, 70, 54, 0.15);
    --search-match: #d4a574;
    --search-active: #b8860b;
    --alert-note-bg: #e8f0e0;
    --alert-note-bar: #6b8e23;
    --alert-tip-bg: #e0f0e0;
    --alert-tip-bar: #2e8b57;
    --alert-important-bg: #f0e0f0;
    --alert-important-bar: #8b008b;
    --alert-warning-bg: #f5e6d0;
    --alert-warning-bar: #d2691e;
    --alert-caution-bg: #f0d0d0;
    --alert-caution-bar: #b22222;
    --drawer-bg: #efe6d0;
}
body.sepia .hl-keyword { color: #8b0000; }
body.sepia .hl-type { color: #8b4513; }
body.sepia .hl-string { color: #2e8b57; }
body.sepia .hl-comment { color: #8b7355; font-style: italic; }
body.sepia .hl-number { color: #006400; }
body.sepia .hl-preprocessor { color: #4b0082; }
body.sepia .hl-operator { color: #5b4636; }

/* === Theme: Monokai === */
body.monokai {
    --bg-page: #272822;
    --text-primary: #f8f8f2;
    --text-secondary: #a6a69c;
    --border-color: #3e3d32;
    --code-bg: #1e1f1c;
    --code-header-bg: #3e3d32;
    --code-border: #49483e;
    --quote-bg: #1e1f1c;
    --quote-bar: #a6e22e;
    --table-alt: #1e1f1c;
    --table-header: #3e3d32;
    --link-color: #66d9ef;
    --toolbar-bg: rgba(39, 40, 34, 0.92);
    --toolbar-border: #49483e;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
    --search-match: #49483e;
    --search-active: #a6e22e;
    --alert-note-bg: #1a2a1a;
    --alert-note-bar: #a6e22e;
    --alert-tip-bg: #1a2a1a;
    --alert-tip-bar: #a6e22e;
    --alert-important-bg: #2a1a2a;
    --alert-important-bar: #ae81ff;
    --alert-warning-bg: #2a2a1a;
    --alert-warning-bar: #e6db74;
    --alert-caution-bg: #2a1a1a;
    --alert-caution-bar: #f92672;
    --drawer-bg: #1e1f1c;
}
body.monokai .hl-keyword { color: #f92672; }
body.monokai .hl-type { color: #66d9ef; }
body.monokai .hl-string { color: #e6db74; }
body.monokai .hl-comment { color: #75715e; font-style: italic; }
body.monokai .hl-number { color: #ae81ff; }
body.monokai .hl-preprocessor { color: #f92672; }
body.monokai .hl-operator { color: #f8f8f2; }

/* === Theme: Dracula === */
body.dracula {
    --bg-page: #282a36;
    --text-primary: #f8f8f2;
    --text-secondary: #6272a4;
    --border-color: #44475a;
    --code-bg: #21222c;
    --code-header-bg: #44475a;
    --code-border: #6272a4;
    --quote-bg: #21222c;
    --quote-bar: #bd93f9;
    --table-alt: #21222c;
    --table-header: #44475a;
    --link-color: #8be9fd;
    --toolbar-bg: rgba(40, 42, 54, 0.92);
    --toolbar-border: #6272a4;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
    --search-match: #44475a;
    --search-active: #bd93f9;
    --alert-note-bg: #1a2a3a;
    --alert-note-bar: #8be9fd;
    --alert-tip-bg: #1a2a1a;
    --alert-tip-bar: #50fa7b;
    --alert-important-bg: #2a1a3a;
    --alert-important-bar: #bd93f9;
    --alert-warning-bg: #2a2a1a;
    --alert-warning-bar: #f1fa8c;
    --alert-caution-bg: #2a1a1a;
    --alert-caution-bar: #ff5555;
    --drawer-bg: #21222c;
}
body.dracula .hl-keyword { color: #ff79c6; }
body.dracula .hl-type { color: #8be9fd; }
body.dracula .hl-string { color: #f1fa8c; }
body.dracula .hl-comment { color: #6272a4; font-style: italic; }
body.dracula .hl-number { color: #bd93f9; }
body.dracula .hl-preprocessor { color: #ff79c6; }
body.dracula .hl-operator { color: #f8f8f2; }

/* === Theme: Solarized Light === */
body.solarized-light {
    --bg-page: #fdf6e3;
    --text-primary: #657b83;
    --text-secondary: #93a1a1;
    --border-color: #eee8d5;
    --code-bg: #eee8d5;
    --code-header-bg: #e0d5c0;
    --code-border: #d4c5a9;
    --quote-bg: #eee8d5;
    --quote-bar: #268bd2;
    --table-alt: #f5efdc;
    --table-header: #e0d5c0;
    --link-color: #268bd2;
    --toolbar-bg: rgba(253, 246, 227, 0.92);
    --toolbar-border: #d4c5a9;
    --toolbar-shadow: 0 4px 20px rgba(101, 123, 131, 0.15);
    --search-match: #b58900;
    --search-active: #cb4b16;
    --alert-note-bg: #e8f0f5;
    --alert-note-bar: #268bd2;
    --alert-tip-bg: #e8f5e8;
    --alert-tip-bar: #2aa198;
    --alert-important-bg: #f5e8f5;
    --alert-important-bar: #6c71c4;
    --alert-warning-bg: #f5f0e0;
    --alert-warning-bar: #cb4b16;
    --alert-caution-bg: #f5e8e8;
    --alert-caution-bar: #dc322f;
    --drawer-bg: #eee8d5;
}
body.solarized-light .hl-keyword { color: #dc322f; }
body.solarized-light .hl-type { color: #268bd2; }
body.solarized-light .hl-string { color: #2aa198; }
body.solarized-light .hl-comment { color: #93a1a1; font-style: italic; }
body.solarized-light .hl-number { color: #d33682; }
body.solarized-light .hl-preprocessor { color: #6c71c4; }
body.solarized-light .hl-operator { color: #657b83; }

/* === Theme: Solarized Dark === */
body.solarized-dark {
    --bg-page: #002b36;
    --text-primary: #839496;
    --text-secondary: #586e75;
    --border-color: #073642;
    --code-bg: #073642;
    --code-header-bg: #0a4050;
    --code-border: #0e4a5a;
    --quote-bg: #073642;
    --quote-bar: #268bd2;
    --table-alt: #073642;
    --table-header: #0a4050;
    --link-color: #268bd2;
    --toolbar-bg: rgba(0, 43, 54, 0.92);
    --toolbar-border: #0e4a5a;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
    --search-match: #0a4050;
    --search-active: #b58900;
    --alert-note-bg: #0a2a3a;
    --alert-note-bar: #268bd2;
    --alert-tip-bg: #0a2a1a;
    --alert-tip-bar: #2aa198;
    --alert-important-bg: #1a0a2a;
    --alert-important-bar: #6c71c4;
    --alert-warning-bg: #1a1a0a;
    --alert-warning-bar: #cb4b16;
    --alert-caution-bg: #1a0a0a;
    --alert-caution-bar: #dc322f;
    --drawer-bg: #073642;
}
body.solarized-dark .hl-keyword { color: #dc322f; }
body.solarized-dark .hl-type { color: #268bd2; }
body.solarized-dark .hl-string { color: #2aa198; }
body.solarized-dark .hl-comment { color: #586e75; font-style: italic; }
body.solarized-dark .hl-number { color: #d33682; }
body.solarized-dark .hl-preprocessor { color: #6c71c4; }
body.solarized-dark .hl-operator { color: #839496; }

/* === Theme: Nord === */
body.nord {
    --bg-page: #2e3440;
    --text-primary: #d8dee9;
    --text-secondary: #81a1c1;
    --border-color: #3b4252;
    --code-bg: #272c36;
    --code-header-bg: #3b4252;
    --code-border: #4c566a;
    --quote-bg: #272c36;
    --quote-bar: #88c0d0;
    --table-alt: #272c36;
    --table-header: #3b4252;
    --link-color: #88c0d0;
    --toolbar-bg: rgba(46, 52, 64, 0.92);
    --toolbar-border: #4c566a;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
    --search-match: #3b4252;
    --search-active: #88c0d0;
    --alert-note-bg: #1a2a3a;
    --alert-note-bar: #88c0d0;
    --alert-tip-bg: #1a2a1a;
    --alert-tip-bar: #a3be8c;
    --alert-important-bg: #2a1a3a;
    --alert-important-bar: #b48ead;
    --alert-warning-bg: #2a2a1a;
    --alert-warning-bar: #ebcb8b;
    --alert-caution-bg: #2a1a1a;
    --alert-caution-bar: #bf616a;
    --drawer-bg: #272c36;
}
body.nord .hl-keyword { color: #81a1c1; }
body.nord .hl-type { color: #8fbcbb; }
body.nord .hl-string { color: #a3be8c; }
body.nord .hl-comment { color: #4c566a; font-style: italic; }
body.nord .hl-number { color: #b48ead; }
body.nord .hl-preprocessor { color: #81a1c1; }
body.nord .hl-operator { color: #d8dee9; }

/* === Theme: GitHub Light === */
body.github-light {
    --bg-page: #ffffff;
    --text-primary: #24292f;
    --text-secondary: #57606a;
    --border-color: #d0d7de;
    --code-bg: #f6f8fa;
    --code-header-bg: #eaeef2;
    --code-border: #d0d7de;
    --quote-bg: #f6f8fa;
    --quote-bar: #0969da;
    --table-alt: #f6f8fa;
    --table-header: #eaeef2;
    --link-color: #0969da;
    --toolbar-bg: rgba(255, 255, 255, 0.92);
    --toolbar-border: #d0d7de;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
    --search-match: #fff8c5;
    --search-active: #fdac54;
    --alert-note-bg: #ddf4ff;
    --alert-note-bar: #0969da;
    --alert-tip-bg: #dafbe1;
    --alert-tip-bar: #1a7f37;
    --alert-important-bg: #fbefff;
    --alert-important-bar: #8250df;
    --alert-warning-bg: #fff8c5;
    --alert-warning-bar: #9a6700;
    --alert-caution-bg: #ffebe9;
    --alert-caution-bar: #cf222e;
    --drawer-bg: #f6f8fa;
}
body.github-light .hl-keyword { color: #cf222e; }
body.github-light .hl-type { color: #953800; }
body.github-light .hl-string { color: #0a3069; }
body.github-light .hl-comment { color: #6e7781; font-style: italic; }
body.github-light .hl-number { color: #0550ae; }
body.github-light .hl-preprocessor { color: #8250df; }
body.github-light .hl-operator { color: #24292f; }

/* === Theme: GitHub Dark === */
body.github-dark {
    --bg-page: #0d1117;
    --text-primary: #c9d1d9;
    --text-secondary: #8b949e;
    --border-color: #30363d;
    --code-bg: #161b22;
    --code-header-bg: #21262d;
    --code-border: #30363d;
    --quote-bg: #161b22;
    --quote-bar: #58a6ff;
    --table-alt: #161b22;
    --table-header: #21262d;
    --link-color: #58a6ff;
    --toolbar-bg: rgba(13, 17, 23, 0.92);
    --toolbar-border: #30363d;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
    --search-match: #30363d;
    --search-active: #fdac54;
    --alert-note-bg: #0c2d6b;
    --alert-note-bar: #58a6ff;
    --alert-tip-bg: #0a3d1f;
    --alert-tip-bar: #3fb950;
    --alert-important-bg: #2d1b4e;
    --alert-important-bar: #a371f7;
    --alert-warning-bg: #3d2d00;
    --alert-warning-bar: #d29922;
    --alert-caution-bg: #3d0c0c;
    --alert-caution-bar: #f85149;
    --drawer-bg: #161b22;
}
body.github-dark .hl-keyword { color: #ff7b72; }
body.github-dark .hl-type { color: #79c0ff; }
body.github-dark .hl-string { color: #a5d6ff; }
body.github-dark .hl-comment { color: #8b949e; font-style: italic; }
body.github-dark .hl-number { color: #79c0ff; }
body.github-dark .hl-preprocessor { color: #ff7b72; }
body.github-dark .hl-operator { color: #c9d1d9; }

/* === Theme: VSCode Dark === */
body.vscode-dark {
    --bg-page: #1e1e1e;
    --text-primary: #d4d4d4;
    --text-secondary: #9d9d9d;
    --border-color: #3c3c3c;
    --code-bg: #1e1e1e;
    --code-header-bg: #252526;
    --code-border: #3c3c3c;
    --quote-bg: #252526;
    --quote-bar: #4ec9b0;
    --table-alt: #252526;
    --table-header: #2d2d30;
    --link-color: #3794ff;
    --toolbar-bg: rgba(30, 30, 30, 0.94);
    --toolbar-border: #3c3c3c;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.45);
    --search-match: #264f78;
    --search-active: #0e639c;
    --alert-note-bg: #0c2d4a;
    --alert-note-bar: #3794ff;
    --alert-tip-bg: #0b3d2e;
    --alert-tip-bar: #4ec9b0;
    --alert-important-bg: #341b4a;
    --alert-important-bar: #c586c0;
    --alert-warning-bg: #3b2f00;
    --alert-warning-bar: #dcdcaa;
    --alert-caution-bg: #4a1414;
    --alert-caution-bar: #f14c4c;
    --drawer-bg: #252526;
}
body.vscode-dark .hl-keyword { color: #569cd6; }
body.vscode-dark .hl-type { color: #4ec9b0; }
body.vscode-dark .hl-string { color: #ce9178; }
body.vscode-dark .hl-comment { color: #6a9955; font-style: italic; }
body.vscode-dark .hl-number { color: #b5cea8; }
body.vscode-dark .hl-preprocessor { color: #c586c0; }
body.vscode-dark .hl-operator { color: #d4d4d4; }

/* === Theme: Bearded === */
body.bearded {
    --bg-page: #1c1f26;
    --text-primary: #c8ccd4;
    --text-secondary: #8b93a1;
    --border-color: #313642;
    --code-bg: #23272f;
    --code-header-bg: #2a2f39;
    --code-border: #313642;
    --quote-bg: #23272f;
    --quote-bar: #ff9f43;
    --table-alt: #23272f;
    --table-header: #2a2f39;
    --link-color: #ff9f43;
    --toolbar-bg: rgba(28, 31, 38, 0.94);
    --toolbar-border: #313642;
    --toolbar-shadow: 0 4px 20px rgba(0, 0, 0, 0.45);
    --search-match: #3a3f4b;
    --search-active: #ff9f43;
    --alert-note-bg: #14304a;
    --alert-note-bar: #4ecdc4;
    --alert-tip-bg: #143a2f;
    --alert-tip-bar: #4ecdc4;
    --alert-important-bg: #33203f;
    --alert-important-bar: #c084fc;
    --alert-warning-bg: #3d2b0f;
    --alert-warning-bar: #ff9f43;
    --alert-caution-bg: #451c1c;
    --alert-caution-bar: #ef4444;
    --drawer-bg: #23272f;
}
body.bearded .hl-keyword { color: #ff9f43; }
body.bearded .hl-type { color: #4ecdc4; }
body.bearded .hl-string { color: #ffd166; }
body.bearded .hl-comment { color: #6b7280; font-style: italic; }
body.bearded .hl-number { color: #f78fb3; }
body.bearded .hl-preprocessor { color: #c084fc; }
body.bearded .hl-operator { color: #e5e7eb; }
)CSS";

// Full palettes for all 64 official Bearded variations (+ classic mapped to body.bearded),
// generated by tools/gen-bearded.mjs from BeardedBear/bearded-theme.
#include "BeardedVariants.inc"

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
    PreviewTheme theme,
    float zoomLevel,
    bool isSyncEnabled,
    int darkModeOverride,
    const std::wstring& beardedVariant
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
                           << block.sourceLineStart << L"\"" << dirAttr
                           << L" onclick=\"jumpToSource(this)\" title=\"Click to jump to editor line\">"
                           << hText << L"</h" << hNum << L">\n";

                // Add to TOC (use InlinesToPlainText to prevent nested <a> tags)
                std::wstring tocText = InlinesToPlainText(block.inlines);
                tocStream << L"<a href=\"#" << hId << L"\" class=\"toc-item toc-l" << hNum << L"\""
                          << dirAttr << L" onclick=\"onTocClick('" << hId << L"'); return false;\">"
                          << EscapeHtml(tocText) << L"</a>\n";
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
                } else if (block.codeLanguage == L"math" || block.codeLanguage == L"latex" || block.codeLanguage == L"katex") {
                    bodyStream << L"<div class=\"katex-display katex\" data-source-line=\"" << block.sourceLineStart << L"\" dir=\"ltr\">\n";
                    std::wstring mathCode;
                    for (size_t k = 0; k < block.codeLines.size(); ++k) {
                        if (k > 0) mathCode += L"\n";
                        mathCode += block.codeLines[k];
                    }
                    bodyStream << L"<span class=\"katex-math\" data-tex=\"" << EscapeHtml(mathCode) << L"\">$$" << EscapeHtml(mathCode) << L"$$</span>\n";
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
    html << "<style>\n" << s_modernPreviewCss << "\n" << s_themeOverrideCss << "\n"
         << s_beardedVariantsCss << "\n</style>\n";

    std::string bodyClass;
    switch (theme) {
        case PreviewTheme::Dark: bodyClass = "dark"; break;
        case PreviewTheme::Sepia: bodyClass = "sepia"; break;
        case PreviewTheme::Monokai: bodyClass = "monokai"; break;
        case PreviewTheme::Dracula: bodyClass = "dracula"; break;
        case PreviewTheme::SolarizedLight: bodyClass = "solarized-light"; break;
        case PreviewTheme::SolarizedDark: bodyClass = "solarized-dark"; break;
        case PreviewTheme::Nord: bodyClass = "nord"; break;
        case PreviewTheme::GitHubLight: bodyClass = "github-light"; break;
        case PreviewTheme::GitHubDark: bodyClass = "github-dark"; break;
        case PreviewTheme::VSCodeDark: bodyClass = "vscode-dark"; break;
        case PreviewTheme::Bearded:
            bodyClass = beardedVariant.empty() ? "bearded" : ("bearded-v-" + WideToUtf8(beardedVariant));
            break;
        default: bodyClass = ""; break;
    }
    html << "</head>\n<body class=\"" << bodyClass << "\">\n";

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

<button id="theme-fab" title="Choose theme" onclick="toggleThemePopover(event)">
  <svg viewBox="0 0 24 24" width="16" height="16" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 1 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 1 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 1 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 1 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 1 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 1 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 1 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 1 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
</button>
<div id="theme-popover" style="display: none;">
  <div class="theme-pop-title">Theme</div>
  <div id="theme-pop-list"></div>
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
  <div class="menu-item" onclick="selectThemeFromMenu('follow')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><rect x="3" y="3" width="18" height="18" rx="2"/><circle cx="12" cy="12" r="4"/></svg></span>
    <span class="menu-label">Theme: Follow Notepad++</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('light')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="12" cy="12" r="4"/><path d="M12 2v2"/><path d="M12 20v2"/><path d="m4.93 4.93 1.41 1.41"/><path d="m17.66 17.66 1.41 1.41"/><path d="M2 12h2"/><path d="M20 12h2"/><path d="m6.34 17.66-1.41 1.41"/><path d="m19.07 4.93-1.41 1.41"/></svg></span>
    <span class="menu-label">Theme: Light</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('dark')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/></svg></span>
    <span class="menu-label">Theme: Dark</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('sepia')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"/></svg></span>
    <span class="menu-label">Theme: Sepia</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('monokai')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><polygon points="12 2 2 22 22 22"/></svg></span>
    <span class="menu-label">Theme: Monokai</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('dracula')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2L2 7l10 5 10-5-10-5zM2 17l10 5 10-5M2 12l10 5 10-5"/></svg></span>
    <span class="menu-label">Theme: Dracula</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('solarized-light')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><circle cx="12" cy="12" r="4"/><path d="M12 2v2"/><path d="M12 20v2"/><path d="m4.93 4.93 1.41 1.41"/><path d="m17.66 17.66 1.41 1.41"/><path d="M2 12h2"/><path d="M20 12h2"/><path d="m6.34 17.66-1.41 1.41"/><path d="m19.07 4.93-1.41 1.41"/></svg></span>
    <span class="menu-label">Theme: Solarized Light</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('solarized-dark')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/></svg></span>
    <span class="menu-label">Theme: Solarized Dark</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('nord')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2L2 22h20L12 2z"/></path d="M12 6l6 12H6l6-12z"/></svg></span>
    <span class="menu-label">Theme: Nord</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('github-light')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2z"/></svg></span>
    <span class="menu-label">Theme: GitHub Light</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('github-dark')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2z"/></svg></span>
    <span class="menu-label">Theme: GitHub Dark</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('vscode-dark')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2z"/><circle cx="12" cy="12" r="3"/></svg></span>
    <span class="menu-label">Theme: VSCode Dark</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('bearded')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M4 4h16v8a8 8 0 0 1-16 0V4z"/></svg></span>
    <span class="menu-label">Theme: Bearded</span>
  </div>
  <div class="menu-item" onclick="selectThemeFromMenu('bearded-v-gold-d-raynh')">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M4 4h16v8a8 8 0 0 1-16 0V4z"/><circle cx="12" cy="8" r="2"/></svg></span>
    <span class="menu-label">Theme: Bearded Gold D Raynh</span>
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
  <div class="menu-item" onclick="openSettingsFromMenu()">
    <span class="menu-icon"><svg class="menu-svg" viewBox="0 0 24 24"><path d="M20 7h-9"/><path d="M14 17H5"/><circle cx="17" cy="17" r="3"/><circle cx="7" cy="7" r="3"/></svg></span>
    <span class="menu-label">Settings...</span>
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
        "leftarrow": "←", "Rightarrow": "⇒", "Leftarrow": "⇐", "forall": "∀", "exists": "∃",
        "sin": "sin", "cos": "cos", "tan": "tan", "cot": "cot", "sec": "sec", "csc": "csc",
        "arcsin": "arcsin", "arccos": "arccos", "arctan": "arctan",
        "sinh": "sinh", "cosh": "cosh", "tanh": "tanh",
        "log": "log", "ln": "ln", "exp": "exp", "lim": "lim", "min": "min", "max": "max",
        "det": "det", "gcd": "gcd", "deg": "deg",
        "quad": "  ", "qquad": "    ", "dots": "…", "cdots": "⋯", "ldots": "…",
        "uparrow": "↑", "downarrow": "↓", "equiv": "≡", "sim": "∼", "simeq": "≃", "cong": "≅"
    };

    function texToMathML(tex, isBlock) {
        var str = tex.trim();
        str = str.replace(/\\text\s*\{([^{}]+)\}/g, function(_, txt) {
            return '<mtext>' + txt + '</mtext>';
        });
        str = str.replace(/\\mathbf\s*\{([^{}]+)\}/g, function(_, txt) {
            return '<mstyle mathvariant="bold">' + convertTokens(txt) + '</mstyle>';
        });
        str = str.replace(/\\mathrm\s*\{([^{}]+)\}/g, function(_, txt) {
            return '<mstyle mathvariant="normal">' + convertTokens(txt) + '</mstyle>';
        });
        var fracRegex = /\\frac\s*\{([^{}]+)\}/g;
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
        var funcNames = "sincostancotseccscarcsinarccosarctansinhcoshtanhloglnexplimminmaxdetgcddeg";
        return s.replace(/\\([a-zA-Z]+)/g, function(match, name) {
            if (mathSymbols[name]) {
                var sym = mathSymbols[name];
                if ("∑∏∫∮".indexOf(sym) !== -1) {
                    return '<mo largeop="true">' + sym + '</mo>';
                } else if ("±×÷·≈≠≤≥∈∉→←⇒⇐∀∃".indexOf(sym) !== -1) {
                    return '<mo>' + sym + '</mo>';
                } else if (funcNames.indexOf(name) !== -1) {
                    return '<mo>' + sym + '</mo>';
                }
                return '<mi>' + sym + '</mi>';
            }
            return '<mi>' + match + '</mi>';
        }).replace(/([0-9]+(?:\.[0-9]+)?)/g, '<mn>$1</mn>')
          .replace(/([=+\-*\/<>(),!])/g, '<mo>$1</mo>')
          .replace(/(?![^<]*>)([a-zA-Z])/g, '<mi>$1</mi>');
    }

    // 1. Process explicit .katex-inline and .katex-display elements
    var inlines = container.querySelectorAll(".katex-inline");
    inlines.forEach(function(el) {
        if (el.querySelector("math")) return;
        var tex = el.getAttribute("data-tex") || el.textContent.trim().replace(/^\$|\$$/g, '');
        if (tex) {
            el.innerHTML = texToMathML(tex, false);
            el.classList.add("katex");
        }
    });

    var displays = container.querySelectorAll(".katex-display .katex-math");
    displays.forEach(function(el) {
        if (el.querySelector("math")) return;
        var tex = el.getAttribute("data-tex") || el.textContent.trim().replace(/^\$\$|\$\$$/g, '');
        if (tex) {
            el.innerHTML = texToMathML(tex, true);
            el.classList.add("katex");
        }
    });

    // 2. Process display math blocks ($$...$$) and remaining inline math in text
    var walker = document.createTreeWalker(container, NodeFilter.SHOW_TEXT, null, false);
    var textNodes = [];
    while (walker.nextNode()) {
        var node = walker.currentNode;
        if (node.parentElement && (node.parentElement.tagName === "PRE" || node.parentElement.tagName === "CODE" || node.parentElement.tagName === "SCRIPT" || node.parentElement.tagName === "STYLE" || node.parentElement.closest("pre, code, math, .katex-inline, .katex-display"))) continue;
        if (node.nodeValue.indexOf("$") !== -1) {
            textNodes.push(node);
        }
    }

    textNodes.forEach(function(node) {
        var text = node.nodeValue;
        if (!text || text.indexOf("$") === -1) return;

        // Process $$...$$ first if present
        if (text.indexOf("$$") !== -1) {
            var displayRegex = /\$\$([^\$]+?)\$\$/g;
            if (displayRegex.test(text)) {
                displayRegex.lastIndex = 0;
                var frag = document.createDocumentFragment();
                var lastIdx = 0;
                var m;
                while ((m = displayRegex.exec(text)) !== null) {
                    if (m.index > lastIdx) {
                        frag.appendChild(document.createTextNode(text.substring(lastIdx, m.index)));
                    }
                    var span = document.createElement("div");
                    span.className = "katex-display katex";
                    span.innerHTML = texToMathML(m[1], true);
                    frag.appendChild(span);
                    lastIdx = m.index + m[0].length;
                }
                if (lastIdx < text.length) {
                    frag.appendChild(document.createTextNode(text.substring(lastIdx)));
                }
                node.parentNode.replaceChild(frag, node);
                return;
            }
        }

        // Process $...$ inline math (ignoring price patterns like $50 and $10)
        var inlineRegex = /(?:^|[^\\])\$([^\$\s](?:[^$]*?[^\$\s])?)\$/g;
        if (inlineRegex.test(text)) {
            inlineRegex.lastIndex = 0;
            var frag = document.createDocumentFragment();
            var lastIdx = 0;
            var m;
            while ((m = inlineRegex.exec(text)) !== null) {
                var matchStart = m.index;
                if (m[0][0] !== '$') matchStart++;
                if (matchStart > lastIdx) {
                    frag.appendChild(document.createTextNode(text.substring(lastIdx, matchStart)));
                }
                var span = document.createElement("span");
                span.className = "katex-inline katex";
                span.innerHTML = texToMathML(m[1], false);
                frag.appendChild(span);
                lastIdx = matchStart + (m[0][0] !== '$' ? m[0].length - 1 : m[0].length);
            }
            if (lastIdx < text.length) {
                frag.appendChild(document.createTextNode(text.substring(lastIdx)));
            }
            node.parentNode.replaceChild(frag, node);
        }
    });
}

// Native 100% offline Mermaid procedural SVG diagram engine
function renderLocalMermaid() {
    var blocks = document.querySelectorAll(".mermaid");
    if (!blocks || blocks.length === 0) return;

    var isDark = isDarkTheme();
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
            if (line.indexOf("graph") === 0 || line.indexOf("flowchart") === 0 ||
                line.indexOf("subgraph") === 0 || line === "end" || line.indexOf("%%") === 0 ||
                line.indexOf("style ") === 0 || line.indexOf("classDef ") === 0 || line.indexOf("class ") === 0) continue;

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

// Bi-directional source jump: Click on heading or double-click anywhere to jump to editor line
function jumpToSource(el) {
    if (!el) return;
    var target = (el.closest && el.closest("[data-source-line]")) ? el.closest("[data-source-line]") : el;
    var line = target.getAttribute ? target.getAttribute("data-source-line") : null;
    if (line !== null && line !== undefined && line !== "" && window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("jumpToLine:" + line);
    }
}

document.addEventListener("dblclick", function(e) {
    if (e.target.closest("button, input, textarea, a")) return;
    var target = e.target.closest("[data-source-line]");
    if (target) {
        jumpToSource(target);
    }
});

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
var THEME_CLASSES = ['dark','sepia','monokai','dracula','solarized-light','solarized-dark','nord','github-light','github-dark','vscode-dark','bearded'];
var THEME_LIST = [
    { id: 'follow', label: 'Follow Notepad++' },
    { id: 'light', label: 'Light' },
    { id: 'dark', label: 'Dark' },
    { id: 'sepia', label: 'Sepia' },
    { id: 'monokai', label: 'Monokai' },
    { id: 'dracula', label: 'Dracula' },
    { id: 'solarized-light', label: 'Solarized Light' },
    { id: 'solarized-dark', label: 'Solarized Dark' },
    { id: 'nord', label: 'Nord' },
    { id: 'github-light', label: 'GitHub Light' },
    { id: 'github-dark', label: 'GitHub Dark' },
    { id: 'vscode-dark', label: 'VSCode Dark' },
    { id: 'bearded', label: 'Bearded' }
];
window._themeId = window._themeId || null;

function isDarkTheme() {
    // Luminance of the body background: keeps light Bearded variations light too
    try {
        var bg = getComputedStyle(document.body).backgroundColor || "";
        var m = bg.match(/rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)/);
        if (m) return (0.2126 * (+m[1]) + 0.7152 * (+m[2]) + 0.0722 * (+m[3])) < 140;
    } catch (e) {}
    for (var i = 0; i < THEME_CLASSES.length; i++) {
        if (document.body.classList.contains(THEME_CLASSES[i])) return true;
    }
    return false;
}

function removeThemeClasses() {
    var toks = document.body.className.split(/\s+/);
    for (var i = 0; i < toks.length; i++) {
        var c = toks[i];
        if (!c) continue;
        if (c === 'light' || THEME_CLASSES.indexOf(c) !== -1 || c.indexOf('bearded-v-') === 0) {
            document.body.classList.remove(c);
        }
    }
}

function applyThemeClass(themeName) {
    removeThemeClasses();
    if (themeName && themeName !== 'follow' && themeName !== 'light') {
        document.body.classList.add(themeName);
    }
}

function detectedThemeId() {
    if (window._themeId) return window._themeId;
    var toks = document.body.className.split(/\s+/);
    for (var i = 0; i < toks.length; i++) {
        if (toks[i] && toks[i].indexOf('bearded-v-') === 0) return toks[i];
    }
    for (var j = 0; j < THEME_CLASSES.length; j++) {
        if (document.body.classList.contains(THEME_CLASSES[j])) return THEME_CLASSES[j];
    }
    return 'follow';
}

function toggleTheme() {
    if (isDarkTheme()) {
        window._themeId = 'light';
        applyThemeClass('light');
    } else {
        window._themeId = 'dark';
        applyThemeClass('dark');
    }
    renderLocalMermaid();
    refreshThemePopover();
}

function toggleThemeFromMenu() {
    closeContextMenu();
    toggleTheme();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleTheme");
    }
}

// Select specific theme
function selectThemeFromMenu(themeName) {
    closeContextMenu();
    closeThemePopover();
    window._themeId = themeName;
    applyThemeClass(themeName);
    renderLocalMermaid();
    refreshThemePopover();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("selectTheme:" + themeName);
    }
}

/* Floating theme picker (gear button): settings menu root + theme list,
   Bearded variants, zoom and dark mode subviews */
var _themePopView = 'settings';

function toggleThemePopover(ev) {
    if (ev) ev.stopPropagation();
    var pop = document.getElementById('theme-popover');
    if (!pop) return;
    if (pop.style.display === 'block') { closeThemePopover(); return; }
    _themePopView = 'settings';
    refreshThemePopover();
    pop.style.display = 'block';
}

/* Hover preview (VSCode-style): show a theme while the pointer is over an item,
   commit only on click; leaving the popover / closing it restores the real theme. */
var _themePreview = { savedClass: null, id: null };

function themePreviewIdFor(id) {
    if (!id || id === '__back' || id === 'follow') return null;
    if (id === 'bearded') {
        // Hovering the Bearded row previews what its label shows (current variant if any).
        var cur = detectedThemeId();
        if (cur.indexOf('bearded-v-') === 0) return cur;
    }
    return id;
}

function startThemePreview(id) {
    var pid = themePreviewIdFor(id);
    if (!pid || pid === _themePreview.id) return;
    if (_themePreview.savedClass === null) _themePreview.savedClass = document.body.className;
    _themePreview.id = pid;
    applyThemeClass(pid);
}

function endThemePreview() {
    if (_themePreview.savedClass === null) { _themePreview.id = null; return; }
    document.body.className = _themePreview.savedClass;
    _themePreview.savedClass = null;
    _themePreview.id = null;
}

function closeThemePopover() {
    endThemePreview();
    var pop = document.getElementById('theme-popover');
    if (pop) pop.style.display = 'none';
}

function openSettingsPopover() {
    var pop = document.getElementById('theme-popover');
    if (!pop) return;
    _themePopView = 'settings';
    refreshThemePopover();
    pop.style.display = 'block';
    pop.style.top = '58px';
    pop.style.bottom = 'auto';
}

function openSettingsFromMenu() {
    closeContextMenu();
    openSettingsPopover();
}

function openThemeView(view) {
    _themePopView = view;
    refreshThemePopover();
}

function openBeardedThemeList() {
    openThemeView('bearded');
}

function backToThemeList() {
    if (_themePopView === 'bearded') openThemeView('themes');
    else openThemeView('settings');
}

function escHtml(s) {
    return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}

function beardedVariantLabel(id) {
    var list = window.BEARDED_VARIANTS || [];
    for (var i = 0; i < list.length; i++) {
        if (list[i].id === id) return list[i].label;
    }
    return id.replace('bearded-v-', '');
}

var _POP_CHECK_SVG = '<svg viewBox="0 0 24 24" width="12" height="12" stroke="currentColor" stroke-width="3" fill="none"><polyline points="20 6 9 17 4 12"/></svg>';
var _POP_CHEV_SVG = '<svg viewBox="0 0 24 24" width="12" height="12" stroke="currentColor" stroke-width="2.5" fill="none"><polyline points="9 18 15 12 9 6"/></svg>';

function themePopRow(label, onclick, checked, chevron, themeId) {
    return '<div class="theme-pop-item' + (checked ? ' checked' : '') + '"'
         + (themeId ? ' data-theme="' + themeId + '"' : '')
         + ' onclick="' + onclick + '">'
         + '<span class="theme-pop-check">' + (checked ? _POP_CHECK_SVG : '') + '</span>'
         + '<span class="theme-pop-label">' + escHtml(label) + '</span>'
         + (chevron ? '<span class="theme-pop-arrow">' + _POP_CHEV_SVG + '</span>' : '')
         + '</div>';
}

function themePopItem(id, label, checked, onclick) {
    return themePopRow(label, onclick, checked, false, id);
}

function themeItemLabelById(id) {
    if (id.indexOf('bearded-v-') === 0) return 'Bearded: ' + beardedVariantLabel(id);
    for (var i = 0; i < THEME_LIST.length; i++) {
        if (THEME_LIST[i].id === id) return THEME_LIST[i].label;
    }
    return id === 'follow' ? 'Follow Notepad++' : id;
}

function zoomPercentLabel() {
    return (window._zoomPercent || 100) + '%';
}

function darkModeLabel() {
    if (window._darkMode === 0) return 'Light';
    if (window._darkMode === 1) return 'Dark';
    return 'Auto';
}

function refreshThemePopover() {
    var list = document.getElementById('theme-pop-list');
    if (!list) return;
    var title = document.querySelector('#theme-popover .theme-pop-title');
    var current = detectedThemeId();
    var html = '';

    if (_themePopView === 'settings') {
        if (title) title.textContent = 'Settings';
        html += themePopRow('Theme: ' + themeItemLabelById(current), "openThemeView('themes')", false, true);
        html += themePopRow('Sync with caret', 'toggleSyncFromMenu(); refreshThemePopover();', !!window._syncEnabled, false);
        html += themePopRow('Outline / TOC', 'toggleTocFromMenu(); refreshThemePopover();', document.body.classList.contains('toc-open'), false);
        html += themePopRow('Smart BiDi', 'toggleBiDiFromMenu(); refreshThemePopover();', !!window._bidiEnabled, false);
        html += themePopRow('Auto-show for Markdown', 'toggleAutoShowFromMenu(); refreshThemePopover();', !!window._autoShowPanel, false);
        html += themePopRow('Zoom: ' + zoomPercentLabel(), "openThemeView('zoom')", false, true);
        html += themePopRow('Copy HTML', 'copyHtmlFromMenu(); closeThemePopover();', false, false);
        html += themePopRow('Save HTML...', 'closeThemePopover(); saveAsHtmlFromMenu();', false, false);
        html += themePopRow('Dark mode: ' + darkModeLabel(), "openThemeView('darkmode')", false, true);
        html += themePopRow('About NppMarkdownNext', "openThemeView('about')", false, false);
    } else if (_themePopView === 'zoom') {
        if (title) title.textContent = 'Zoom (' + zoomPercentLabel() + ')';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        html += themePopRow('Zoom in', 'zoomInFromMenu(); refreshThemePopover();', false, false);
        html += themePopRow('Zoom out', 'zoomOutFromMenu(); refreshThemePopover();', false, false);
        html += themePopRow('Reset (100%)', 'zoomResetFromMenu(); refreshThemePopover();', false, false);
        html += themePopRow('80%', "setZoomPercentFromMenu(80); setTimeout(refreshThemePopover, 80);", window._zoomPercent === 80, false);
        html += themePopRow('100%', "setZoomPercentFromMenu(100); setTimeout(refreshThemePopover, 80);", window._zoomPercent === 100, false);
        html += themePopRow('125%', "setZoomPercentFromMenu(125); setTimeout(refreshThemePopover, 80);", window._zoomPercent === 125, false);
        html += themePopRow('150%', "setZoomPercentFromMenu(150); setTimeout(refreshThemePopover, 80);", window._zoomPercent === 150, false);
    } else if (_themePopView === 'darkmode') {
        if (title) title.textContent = 'Dark mode';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        html += themePopRow('Auto (follow Notepad++)', 'setDarkModeFromMenu(-1);', window._darkMode === undefined || window._darkMode === -1, false);
        html += themePopRow('Light', 'setDarkModeFromMenu(0);', window._darkMode === 0, false);
        html += themePopRow('Dark', 'setDarkModeFromMenu(1);', window._darkMode === 1, false);
    } else if (_themePopView === 'about') {
        if (title) title.textContent = 'About';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        html += themePopRow('NppMarkdownNext v1.0.4.0', '', false, false);
        html += themePopRow('Open project page', "location.href='https://github.com/theneet0/NppMarkdownNext';", false, false);
    } else if (_themePopView === 'bearded') {
        if (title) title.textContent = 'Bearded themes';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        html += themePopItem('bearded', 'Classic (Arc)', current === 'bearded', "selectThemeFromMenu('bearded')");
        var variants = window.BEARDED_VARIANTS || [];
        for (var v = 0; v < variants.length; v++) {
            html += themePopItem(variants[v].id, variants[v].label, current === variants[v].id,
                                 "selectThemeFromMenu('" + variants[v].id + "')");
        }
    } else {
        if (title) title.textContent = 'Theme';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        for (var i = 0; i < THEME_LIST.length; i++) {
            var t = THEME_LIST[i];
            if (t.id === 'bearded') {
                var isVariant = current.indexOf('bearded-v-') === 0;
                var label = isVariant ? ('Bearded: ' + beardedVariantLabel(current)) : t.label;
                var checked = (current === 'bearded' || isVariant);
                html += themePopItem('bearded', label, checked, 'openBeardedThemeList()');
            } else {
                html += themePopItem(t.id, t.label, t.id === current, "selectThemeFromMenu('" + t.id + "')");
            }
        }
    }
    list.innerHTML = html;
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
    window._zoomPercent = Math.round(_clientZoom * 100);
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

function setZoomPercentFromMenu(percent) {
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomChange:" + (percent / 100));
    } else {
        applyClientZoom(percent / 100);
    }
}

function toggleAutoShowFromMenu() {
    window._autoShowPanel = !window._autoShowPanel;
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleAutoShow");
    }
    showToast(window._autoShowPanel ? "Auto-show for Markdown enabled" : "Auto-show for Markdown disabled");
}

function toggleBiDiFromMenu() {
    closeContextMenu();
    window._bidiEnabled = !window._bidiEnabled;
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

function copyHtmlFromMenu() {
    closeContextMenu();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('copyHtml');
        showToast('📋 Rendered HTML copied');
    } else {
        showToast('⚠ Copy is available in the preview panel');
    }
}

function setDarkModeFromMenu(mode) {
    closeThemePopover();
    window._darkMode = mode;
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('darkMode:' + mode);
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
    var isDark = isDarkTheme();
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

// Keep clicks inside the popover from ever reaching the outside-click handler:
// rows rebuild #theme-pop-list (innerHTML swap), which detaches e.target and would
// make pop.contains(e.target) false, closing the popover right after a view switch
// (main list -> Bearded submenu, submenu -> Back).
var themePopoverRoot = document.getElementById("theme-popover");
if (themePopoverRoot) {
    themePopoverRoot.addEventListener("click", function(e) { e.stopPropagation(); });
    themePopoverRoot.addEventListener("mouseover", function(e) {
        var t = e.target;
        var it = t && t.closest ? t.closest(".theme-pop-item") : null;
        if (it) startThemePreview(it.getAttribute("data-theme"));
    });
    themePopoverRoot.addEventListener("mouseleave", function() { endThemePreview(); });
}

document.addEventListener("click", function(e) {
    if (contextMenu && !contextMenu.contains(e.target)) {
        closeContextMenu();
    }
    // Close the theme picker when clicking outside it (gear button stops propagation itself)
    var pop = document.getElementById("theme-popover");
    if (pop && pop.style.display === "block") {
        var inPop = pop.contains(e.target);
        var inFab = !!(e.target && e.target.closest && e.target.closest("#theme-fab"));
        if (!inPop && !inFab) closeThemePopover();
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
        var themePop = document.getElementById("theme-popover");
        if (themePop && themePop.style.display === "block") {
            closeThemePopover();
            return;
        }
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
            if (typeof data.themeClass === "string") {
                applyThemeClass(data.themeClass === "" ? 'light' : data.themeClass);
                renderLocalMermaid();
                refreshThemePopover();
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
)HTML";
    // Bearded variation list for the theme picker submenu (generated data)
    html << "<script>window.BEARDED_VARIANTS=" << s_beardedVariantsJs << ";</script>\n";
    html << "<script>window._zoomPercent=" << static_cast<int>(zoomLevel*100+0.5) << ";window._darkMode=" << darkModeOverride << ";</script>\n";
    html << "</body>\n</html>\n";

    return PreviewComponents{ html.str(), bodyHtmlUtf8, tocHtmlUtf8 };
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

bool HtmlExporter::SaveToFile(const std::wstring& filePath, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant) {
    std::string html = GeneratePreviewHtml(doc, title, theme, 1.0f, false, 0, beardedVariant);
    std::ofstream out(filePath.c_str(), std::ios::binary);
    if (!out.is_open()) return false;
    out.write(html.data(), html.size());
    return true;
}

bool HtmlExporter::CopyToClipboard(HWND hwndOwner, const MarkdownDocument& doc, const std::wstring& title, PreviewTheme theme, const std::wstring& beardedVariant) {
    auto components = GeneratePreviewComponents(doc, title, theme, 1.0f, false, 0, beardedVariant);
    std::string cfHtml = GenerateCfHtml(components.bodyHtml);

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
