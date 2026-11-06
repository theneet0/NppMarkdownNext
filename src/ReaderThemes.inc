#include "ThemePalette.inc"

inline constexpr const char* s_readerThemeCss = R"THEME_CSS(

body, body.dark {
  --bg: var(--bg-page);
  --ink: var(--text-primary);
  --muted: var(--text-secondary);
  --line: var(--border-color);
  --line-strong: var(--code-border);
  --panel: var(--code-header-bg);
  --panel-2: var(--code-bg);
  --accent: var(--link-color);
  --accent-ink: var(--link-color);
  --accent-soft: var(--quote-bg);
  --code-head: var(--code-header-bg);
  --table-head: var(--table-header);
}
body[class] .doc :is(h1, h2, h3, h4, h5, h6) { color: var(--ink); }
.doc .hljs { background: transparent; color: var(--ink); }
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
    max-width: calc(100vw - 32px);
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
.theme-pop-item[aria-disabled="true"] { opacity: 0.55; cursor: default; }

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
    min-width: 0;
    overflow-wrap: anywhere;
}

.theme-pop-arrow {
    flex: 0 0 auto;
    display: flex;
    align-items: center;
    margin-left: auto;
    padding-left: 8px;
    opacity: 0.55;
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
    max-width: calc(100vw - 32px);
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
.toc-item[aria-current="location"] { color: var(--link-color); background: var(--quote-bg); font-weight: 600; }
#content { font-size: var(--reader-font-size, 15px); line-height: var(--reader-line-height, 1.7); max-width: var(--reader-max-width, 860px); }
.doc .alert-callout { border-inline-start-color: var(--alert-color); background: var(--alert-background); }
.alert-title { color: var(--alert-color); font-weight: 600; margin-bottom: 0.4em; }
.alert-note { --alert-color: var(--alert-note-bar); --alert-background: var(--alert-note-bg); }
.alert-tip { --alert-color: var(--alert-tip-bar); --alert-background: var(--alert-tip-bg); }
.alert-important { --alert-color: var(--alert-important-bar); --alert-background: var(--alert-important-bg); }
.alert-warning { --alert-color: var(--alert-warning-bar); --alert-background: var(--alert-warning-bg); }
.alert-caution { --alert-color: var(--alert-caution-bar); --alert-background: var(--alert-caution-bg); }
#reader-warnings { position: fixed; top: 8px; left: 8px; right: 8px; z-index: 9986; max-height: 25vh; overflow: auto; pointer-events: none; }
#reader-warnings > div:not([hidden]) { background: var(--toolbar-bg); border: 1px solid var(--border-color); border-radius: 8px; padding: 8px 12px; margin-bottom: 4px; color: var(--text-primary); font: 13px var(--font-ui); overflow-wrap: anywhere; }
.reader-legacy-anchor { display: block; height: 0; }


body { transition: background-color 0.25s ease, color 0.25s ease; }
#theme-popover { animation: reader-pop-in 0.15s ease-out; transform-origin: bottom right; font-family: var(--font-ui); }
@keyframes reader-pop-in { from { opacity: 0; transform: translateY(6px) scale(0.97); } to { opacity: 1; transform: none; } }
#theme-fab:focus-visible, .theme-pop-item:focus-visible, .toc-close:focus-visible, .toc-item:focus-visible { outline: 2px solid var(--link-color); outline-offset: 2px; }
#toc-drawer { display: flex !important; position: fixed; top: 0; right: 0; transform: translateX(100%); }
body.toc-open #toc-drawer { transform: translateX(0); }
body.toc-open #reader-wrap { padding-right: 300px; }
@media (max-width: 650px) {
    body.toc-open #reader-wrap { padding-right: 20px; }
    body.toc-open #theme-fab, body.toc-open #theme-popover { right: 16px; }
}
@media (max-width: 160px) {
    .toc-title { display: none; }
    .toc-header { padding: 2px; justify-content: flex-end; }
    .toc-close { width: 20px; flex: 0 0 20px; padding: 2px; }
}
@media print { #theme-fab, #theme-popover, #toc-drawer, #toc-backdrop, #toast-msg, #reader-warnings { display: none !important; } }
@media (prefers-reduced-motion: reduce) { body, #theme-fab, #theme-popover, #toc-drawer, .toast-notification { animation: none; transition: none; } }

)THEME_CSS";
inline constexpr const char* s_readerSettingsHtml = R"SETTINGS_HTML(
<button id="theme-fab" title="Settings" aria-label="Settings" aria-expanded="false" aria-controls="theme-popover" onclick="toggleThemePopover(event)">
  <svg viewBox="0 0 24 24" width="16" height="16" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 1 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 1 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 1 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 1 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 1 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 1 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 1 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 1 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
</button>
<div id="theme-popover" style="display: none;">
  <div class="theme-pop-title">Theme</div>
  <div id="theme-pop-list"></div>
</div>


<div id="toast-msg" class="toast-notification" role="status"></div>
<div id="reader-warnings"><div id="reader-settings-warning" role="alert" hidden></div><div id="reader-operation-warning" role="alert" hidden></div></div>
<div id="toc-backdrop" onclick="closeToc()"></div>
<aside id="toc-drawer" aria-label="Outline / Table of Contents" inert>
  <div class="toc-header"><span class="toc-title">Outline / Table of Contents</span><button class="toc-close" aria-label="Close outline" onclick="closeToc()">&#215;</button></div>
  <div class="toc-content"></div>
</aside>

)SETTINGS_HTML";
inline constexpr const char* s_readerThemeScript = R"THEME_JS(
window._syncEnabled = true; window._bidiEnabled = true; window._autoShowPanel = true;
window._zoomPercent = 100; window._darkMode = -1; var nativeDark = false;
window._fontSizePx = 15; window._lineHeightPercent = 170; window._maxWidthPx = 860;
var pendingExport = null, exportCounter = 0;
var exportPrefix = Date.now().toString(36) + ':' + Math.random().toString(36).slice(2, 8);
// Theme toggle
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
    if (themeName === 'follow') themeName = nativeDark ? 'dark' : 'light';
    if (themeName && themeName !== 'light') {
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
    refreshThemePopover();
}

function toggleThemeFromMenu() {
    toggleTheme();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleTheme");
    }
}

// Select specific theme
function selectThemeFromMenu(themeName) {
    closeThemePopover();
    window._themeId = themeName;
    applyThemeClass(themeName);
    refreshThemePopover();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("selectTheme:" + themeName);
    }
}

/* Floating theme picker (gear button): settings menu root + theme list,
   Bearded variants, zoom and dark mode subviews */
var _themePopView = 'settings';

function positionThemePopover() {
    var pop = document.getElementById('theme-popover');
    if (!pop || pop.style.display !== 'block') return;
    pop.style.left = 'auto'; pop.style.top = 'auto'; pop.style.bottom = '58px';
    var bounds = pop.getBoundingClientRect();
    if (bounds.left < 16) pop.style.left = '16px';
    bounds = pop.getBoundingClientRect();
    if (bounds.top < 8) { pop.style.top = '8px'; pop.style.bottom = 'auto'; }
    pop.style.maxHeight = Math.max(24, window.innerHeight - 74) + 'px';
}

function toggleThemePopover(ev) {
    if (ev) ev.stopPropagation();
    var pop = document.getElementById('theme-popover');
    if (!pop) return;
    if (pop.style.display === 'block') { closeThemePopover(); return; }
    _themePopView = 'settings';
    refreshThemePopover();
    pop.style.top = 'auto'; pop.style.bottom = '58px';
    pop.style.display = 'block';
    positionThemePopover();
    document.getElementById('theme-fab').setAttribute('aria-expanded', 'true');
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
    document.getElementById('theme-fab').setAttribute('aria-expanded', 'false');
}

function openSettingsPopover() {
    var pop = document.getElementById('theme-popover');
    if (!pop) return;
    _themePopView = 'settings';
    refreshThemePopover();
    pop.style.top = 'auto'; pop.style.bottom = '58px';
    pop.style.display = 'block';
    document.getElementById('theme-fab').setAttribute('aria-expanded', 'true');
    positionThemePopover();
}

function openSettingsFromMenu() {
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

function themePopRow(label, onclick, checked, chevron, themeId, disabled) {
    return '<div role="button" tabindex="' + (disabled ? '-1' : '0') + '" aria-disabled="' + (disabled ? 'true' : 'false') + '" aria-pressed="' + (checked ? 'true' : 'false') + '" class="theme-pop-item' + (checked ? ' checked' : '') + '"'
         + (themeId ? ' data-theme="' + themeId + '"' : '')
         + ' onclick="' + (disabled ? '' : onclick) + '">'
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
        html += themePopRow('Typography', "openThemeView('typography')", false, true);
        html += themePopRow('Copy HTML', 'copyHtmlFromMenu(); closeThemePopover();', false, false, null, !!pendingExport);
        html += themePopRow('Save HTML...', 'closeThemePopover(); saveAsHtmlFromMenu();', false, false, null, !!pendingExport);
        html += themePopRow('Dark mode: ' + darkModeLabel() + (current === 'follow' ? '' : ' (Follow Notepad++ only)'), "openThemeView('darkmode')", false, true, null, current !== 'follow');
        html += themePopRow('About NppMarkdownNext', "openThemeView('about')", false, false);
    } else if (_themePopView === 'typography') {
        if (title) title.textContent = 'Typography';
        html += themePopRow('< Back', 'backToThemeList()', false, false, '__back');
        html += themePopRow('Text size: ' + window._fontSizePx + 'px', "openThemeView('fontSize')", false, true);
        html += themePopRow('Line height: ' + window._lineHeightPercent + '%', "openThemeView('lineHeight')", false, true);
        html += themePopRow('Reading width: ' + window._maxWidthPx + 'px', "openThemeView('readingWidth')", false, true);
    } else if (['fontSize', 'lineHeight', 'readingWidth'].includes(_themePopView)) {
        var presets = _themePopView === 'fontSize' ? [15, 17, 19] : (_themePopView === 'lineHeight' ? [150, 170, 190] : [680, 860, 1100]);
        var field = _themePopView === 'fontSize' ? '_fontSizePx' : (_themePopView === 'lineHeight' ? '_lineHeightPercent' : '_maxWidthPx');
        if (title) title.textContent = _themePopView === 'fontSize' ? 'Text size' : (_themePopView === 'lineHeight' ? 'Line height' : 'Reading width');
        html += themePopRow('< Back', "openThemeView('typography')", false, false, '__back');
        presets.forEach(function(value) { html += themePopRow(value + (_themePopView === 'lineHeight' ? '%' : 'px'), "setTypographyFromMenu('" + _themePopView + "'," + value + ')', window[field] === value, false); });
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
        html += themePopRow('NppMarkdownNext v1.6.0', '', false, false);
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
    positionThemePopover();
}

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

function toggleSyncFromMenu() {
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
    var container = document.getElementById("reader-wrap");
    if (container) {
        container.style.zoom = _clientZoom;
    }
}

function zoomInFromMenu() {
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomIn");
    } else {
        applyClientZoom(_clientZoom + 0.1);
    }
}

function zoomOutFromMenu() {
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("zoomOut");
    } else {
        applyClientZoom(_clientZoom - 0.1);
    }
}

function zoomResetFromMenu() {
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
    window._bidiEnabled = !window._bidiEnabled;
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("toggleBiDi");
    } else {
        var container = document.getElementById("reader-wrap");
        if (container) {
            var isRtl = container.getAttribute("dir") === "rtl";
            container.setAttribute("dir", isRtl ? "ltr" : "rtl");
            showToast(isRtl ? "🌐 LTR direction set" : "🌐 RTL direction set");
        }
    }
}

function beginHtmlOperation(operation) {
    if (pendingExport) return false;
    pendingExport = {operation: operation, requestId: exportPrefix + ':' + (++exportCounter)};
    window.chrome.webview.postMessage(operation + ':' + pendingExport.requestId);
    refreshThemePopover();
    return true;
}

function showOperationResult(data) {
    if (data.operation === 'saveSettings') {
        var warning = document.getElementById('reader-settings-warning');
        warning.hidden = data.status === 'succeeded';
        warning.textContent = warning.hidden ? '' : ('Settings could not be saved. ' + (data.detail || 'Change a setting to retry.'));
        return;
    }
    if (pendingExport) {
        if (!data.requestId || pendingExport.requestId !== data.requestId || pendingExport.operation !== data.operation) return;
        pendingExport = null;
    } else if (data.requestId) return;
    refreshThemePopover();
    var warning = document.getElementById('reader-operation-warning');
    warning.hidden = data.status === 'succeeded' || data.status === 'cancelled';
    warning.textContent = warning.hidden ? '' : ((data.status === 'partial' ? 'Only part of the clipboard data was copied. ' : 'Operation failed. ') + (data.detail || '') + (data.recoveryPath ? ' Recovery file: ' + data.recoveryPath : ''));
    if (data.status === 'succeeded') showToast(data.operation === 'copyHtml' ? 'Rendered HTML copied' : 'HTML saved');
}

function saveAsHtmlFromMenu() {
    if (window.chrome && window.chrome.webview) {
        beginHtmlOperation('saveAsHtml');
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
    if (window.chrome && window.chrome.webview) {
        beginHtmlOperation('copyHtml');
    } else {
        showToast('⚠ Copy is available in the preview panel');
    }
}

function setDarkModeFromMenu(mode) {
    if (detectedThemeId() !== 'follow') return;
    closeThemePopover();
    window._darkMode = mode;
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('darkMode:' + mode);
    }
}

function setTypographyFromMenu(kind, value) {
    var options = {fontSize: {values: [15, 17, 19], command: 'readingFontSize:', field: '_fontSizePx'}, lineHeight: {values: [150, 170, 190], command: 'readingLineHeight:', field: '_lineHeightPercent'}, readingWidth: {values: [680, 860, 1100], command: 'readingWidth:', field: '_maxWidthPx'}};
    var option = options[kind];
    if (!option || !option.values.includes(value)) return;
    if (window.chrome && window.chrome.webview) window.chrome.webview.postMessage(option.command + value);
    else { window[option.field] = value; applyContentTypography(); refreshThemePopover(); }
}

function applyContentTypography() {
    var content = document.getElementById('content');
    if (!content) return;
    content.style.setProperty('--reader-font-size', window._fontSizePx + 'px');
    content.style.setProperty('--reader-line-height', window._lineHeightPercent / 100);
    content.style.setProperty('--reader-max-width', window._maxWidthPx + 'px');
    scheduleCurrentSection();
}

function closeToc() {
    document.body.classList.remove("toc-open");
    var drawer = document.getElementById("toc-drawer");
    var backdrop = document.getElementById("toc-backdrop");
    if (drawer) { drawer.classList.remove("open"); drawer.inert = true; }
    if (backdrop) backdrop.classList.remove("open");
    positionThemePopover();
    if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage("tocStateChanged:false");
    }
}

function openToc() {
    document.body.classList.add("toc-open");
    var drawer = document.getElementById("toc-drawer");
    var backdrop = document.getElementById("toc-backdrop");
    if (drawer) { drawer.classList.add("open"); drawer.inert = false; }
    if (backdrop) backdrop.classList.add("open");
    positionThemePopover();
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


var readerIndex = {headings: [], blocks: []};
var headingObserver = null, currentSectionFrame = 0;
function scheduleCurrentSection() {
    if (currentSectionFrame) return;
    currentSectionFrame = requestAnimationFrame(function() {
        currentSectionFrame = 0;
        var active = readerIndex.headings[0];
        readerIndex.headings.forEach(function(heading) { if (heading.element.getBoundingClientRect().top <= 40) active = heading; });
        document.querySelectorAll('#toc-drawer .toc-item').forEach(function(link) {
            if (active && link.dataset.headingId === active.id) link.setAttribute('aria-current', 'location');
            else link.removeAttribute('aria-current');
        });
    });
}

function updateReaderOutline(index) {
    var toc = document.querySelector('#toc-drawer .toc-content');
    if (!toc || !index) return;
    readerIndex = index;
    var oldLinks = new Map(Array.from(toc.querySelectorAll('.toc-item')).map(function(link) { return [link.dataset.headingId, link]; }));
    var focused = document.activeElement;
    var focusedIndex = Array.from(oldLinks.values()).indexOf(focused);
    var retained = new Set();
    if (!oldLinks.size) toc.textContent = '';
    index.headings.forEach(function(heading) {
        var link = oldLinks.get(heading.id) || document.createElement('a');
        link.className = 'toc-item';
        link.dataset.headingId = heading.id;
        link.href = '#' + heading.id;
        link.textContent = heading.text;
        link.title = heading.text;
        link.style.paddingInlineStart = (12 + (heading.level - 1) * 12) + 'px';
        link.onclick = function(event) { event.preventDefault(); onTocClick(heading.id); };
        var expected = toc.children[retained.size];
        if (expected !== link) toc.insertBefore(link, expected || null);
        retained.add(heading.id);
    });
    oldLinks.forEach(function(link, id) { if (!retained.has(id)) link.remove(); });
    if (!index.headings.length) toc.textContent = 'No headings in this document';
    if (focusedIndex >= 0 && !focused.isConnected) {
        var links = toc.querySelectorAll('.toc-item');
        (links[Math.min(focusedIndex, links.length - 1)] || document.querySelector('.toc-close')).focus();
    } else if (focusedIndex >= 0 && document.activeElement !== focused) {
        focused.focus();
    }
    if (headingObserver) headingObserver.disconnect();
    if (typeof IntersectionObserver !== 'undefined') {
        headingObserver = new IntersectionObserver(scheduleCurrentSection);
        index.headings.forEach(function(heading) { headingObserver.observe(heading.element); });
    }
    scheduleCurrentSection();
}
function applyReaderSettings(data) {
    window._syncEnabled = !!data.sync;
    window._bidiEnabled = !!data.bidi;
    window._darkMode = data.darkMode;
    window._zoomPercent = data.zoomPercent;
    window._autoShowPanel = !!data.autoShow;
    if ([15, 17, 19].includes(data.fontSizePx)) window._fontSizePx = data.fontSizePx;
    if ([150, 170, 190].includes(data.lineHeightPercent)) window._lineHeightPercent = data.lineHeightPercent;
    if ([680, 860, 1100].includes(data.maxWidthPx)) window._maxWidthPx = data.maxWidthPx;
    applyContentTypography();
    document.body.classList.toggle('toc-open', !!data.outline);
    var drawer = document.getElementById('toc-drawer');
    if (drawer) { drawer.classList.toggle('open', !!data.outline); drawer.inert = !data.outline; }
    var backdrop = document.getElementById('toc-backdrop');
    if (backdrop) backdrop.classList.toggle('open', !!data.outline);
    refreshThemePopover();
}
if (window.chrome && window.chrome.webview) {
    window.chrome.webview.addEventListener('message', function(event) {
        var data = event.data;
        if (typeof data === 'string') { try { data = JSON.parse(data); } catch(e) { return; } }
        if (!data || !document.body) return;
        if (data.type === 'render' || data.type === 'setTheme') {
            endThemePreview();
            nativeDark = !!data.isDark;
            window._themeId = data.themeId || window._themeId || 'follow';
            applyThemeClass(window._themeId);
            refreshThemePopover();
        } else if (data.type === 'settings') {
            applyReaderSettings(data);
        } else if (data.type === 'operationResult') {
            showOperationResult(data);
        }
    });
}
function initializeReaderSettings() {
    var pop = document.getElementById('theme-popover');
    pop.addEventListener('click', function(event) { event.stopPropagation(); });
    pop.addEventListener('mouseover', function(event) {
        var row = event.target.closest('.theme-pop-item');
        if (row) startThemePreview(row.getAttribute('data-theme'));
    });
    pop.addEventListener('mouseleave', endThemePreview);
    pop.addEventListener('keydown', function(event) {
        var row = event.target.closest('.theme-pop-item');
        if (!row || row.getAttribute('aria-disabled') === 'true') return;
        if (event.key === 'Enter' || event.key === ' ') { event.preventDefault(); row.click(); }
        if (event.key === 'ArrowDown' || event.key === 'ArrowUp') {
            event.preventDefault();
            var rows = Array.from(pop.querySelectorAll('.theme-pop-item:not([aria-disabled="true"])'));
            var step = event.key === 'ArrowDown' ? 1 : -1;
            rows[(rows.indexOf(row) + step + rows.length) % rows.length].focus();
        }
    });
    document.addEventListener('click', function(event) {
        if (!pop.contains(event.target) && !event.target.closest('#theme-fab')) closeThemePopover();
    });
    document.addEventListener('keydown', function(event) {
        if (event.key !== 'Escape') return;
        if (pop.style.display === 'block') { closeThemePopover(); document.getElementById('theme-fab').focus(); }
        else if (document.body.classList.contains('toc-open')) closeToc();
    });
    document.addEventListener('readerRendered', function(event) { updateReaderOutline(event.detail); });
    document.addEventListener('scroll', scheduleCurrentSection, {passive: true, capture: true});
    window.addEventListener('resize', function() { positionThemePopover(); scheduleCurrentSection(); });
    if (window.readerDocumentIndex) updateReaderOutline(window.readerDocumentIndex);
    applyContentTypography();
}
if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', initializeReaderSettings);
else initializeReaderSettings();

)THEME_JS";

inline std::string ReaderShellHtml() {
    std::string shell(s_kelkReaderShellHtml);
    shell.insert(shell.rfind("</head>"), std::string("<style>") + s_readerPaletteCss + s_readerThemeCss + s_readerSemanticCss +
                 "</style><script>" + s_readerCatalogJs + s_readerThemeScript + "</script>");
    shell.insert(shell.rfind("</body>"), s_readerSettingsHtml);
    return shell;
}
