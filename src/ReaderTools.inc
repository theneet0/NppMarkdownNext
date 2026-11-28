// Reader tools: everything the live preview adds on top of the rendered document (not part of HTML exports).
// Tables (breakout, column resize, remembered widths and scroll), code copy buttons, the context menu,
// copy helpers, link routing, find/print requests, the outline button and dismissable warnings.

inline constexpr const char* s_readerToolsCss = R"TOOLS_CSS(
/* Tables: tables follow the document reading width (#content) together with surrounding text, align with the
   reading column, and scroll horizontally inside #content only when genuinely wider than the column. */
#reader-wrap { container-type: inline-size; }
#content { --reader-doc-pad: 0px; padding-inline: var(--reader-doc-pad); }
#content .reader-table-wrap {
    display: block;
    width: 100%;
    max-width: 100%;
    margin: 1em 0;
    margin-inline-start: 0;
    overflow-x: auto;
}
#content .reader-table-wrap > table { display: table; width: auto; max-width: 100%; margin: 0; overflow: visible; }
#content .reader-table-wrap > table.reader-cols-fixed { max-width: none; }
#content .reader-table-wrap th { position: relative; vertical-align: top; }
#content .reader-table-wrap td { vertical-align: top; }
/* Long unbroken inline code or URLs in table cells wrap instead of crushing sibling prose columns. */
#content .reader-table-wrap :is(th, td) :is(code, a) { overflow-wrap: anywhere; word-break: break-word; }
#content .reader-table-wrap > table.reader-cols-fixed :is(th, td) { overflow-wrap: anywhere; overflow: hidden; }
/* still too wide: cap the height so the horizontal scrollbar stays on screen instead of below a long table */
#content .reader-table-wrap.reader-table-overflow { max-height: calc(100vh - 48px); overflow: auto; }
#content .reader-table-overflow thead th { position: sticky; top: 0; z-index: 1; }
.reader-col-grip { position: absolute; top: 0; bottom: 0; inset-inline-end: 0; width: 7px; cursor: col-resize; touch-action: none; }
.reader-col-grip:hover, .reader-col-grip.active { background: var(--link-color); opacity: 0.45; }
body.reader-col-resizing { cursor: col-resize; user-select: none; }

/* Copy button on code blocks: visible on hover or keyboard focus */
#content pre { position: relative; }
.reader-copy-code {
    position: absolute; top: 6px; right: 6px;
    display: inline-flex; align-items: center; justify-content: center; width: 28px; height: 26px; padding: 0;
    border: 1px solid var(--border-color); border-radius: 6px; background: var(--toolbar-bg); color: var(--text-secondary);
    cursor: pointer; opacity: 0; transition: opacity 0.12s ease, color 0.12s ease;
}
#content pre:hover > .reader-copy-code, .reader-copy-code:focus-visible, .reader-copy-code.copied { opacity: 1; }
.reader-copy-code:hover { color: var(--link-color); border-color: var(--link-color); }
.reader-copy-code.copied { color: var(--link-color); }
.reader-copy-code svg { width: 15px; height: 15px; pointer-events: none; }

/* Outline and Hide-Editor floating buttons, next to the settings button */
#toc-fab, #editor-fab {
    position: fixed; bottom: 16px; width: 34px; height: 34px; z-index: 9985;
    display: flex; align-items: center; justify-content: center;
    background: var(--toolbar-bg); backdrop-filter: blur(20px); -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color); border-radius: 50%; box-shadow: 0 4px 16px rgba(0, 0, 0, 0.18);
    color: var(--text-secondary); cursor: pointer; transition: color 0.15s ease, border-color 0.15s ease;
}
#toc-fab { right: 58px; }
#editor-fab { right: 100px; }
#toc-fab:hover, body.toc-open #toc-fab, #editor-fab:hover, body.editor-hidden #editor-fab { color: var(--link-color); border-color: var(--link-color); }
body.toc-open #toc-fab { right: calc(var(--toc-width, 280px) + 52px); }
body.toc-open #editor-fab { right: calc(var(--toc-width, 280px) + 94px); }
#toc-fab:focus-visible, #editor-fab:focus-visible, .reader-warning-close:focus-visible { outline: 2px solid var(--link-color); outline-offset: 2px; }
.reader-menu-item:focus-visible { outline: none; background: rgba(128, 128, 128, 0.16); }
@media (max-width: 650px) { body.toc-open #toc-fab { right: 58px; } body.toc-open #editor-fab { right: 100px; } }

/* Context menu: the settings popover's look */
#reader-menu {
    position: fixed; z-index: 9995; min-width: 190px; max-width: calc(100vw - 16px); padding: 4px;
    background: var(--toolbar-bg); backdrop-filter: blur(20px); -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--border-color); border-radius: 10px; box-shadow: var(--toolbar-shadow);
    font: 13px var(--font-ui); color: var(--text-primary); user-select: none;
}
#reader-menu[hidden] { display: none; }
.reader-menu-item {
    display: flex; align-items: center; gap: 16px; width: 100%; padding: 6px 10px; border: 0; border-radius: 6px;
    background: none; color: inherit; font: inherit; text-align: start; cursor: pointer;
}
.reader-menu-item:hover:not(:disabled) { background: rgba(128, 128, 128, 0.16); }
.reader-menu-item:disabled { opacity: 0.45; cursor: default; }
.reader-menu-key { margin-inline-start: auto; color: var(--text-secondary); font-size: 12px; }
.reader-menu-item > span:first-child { min-width: 0; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
/* a narrow docked panel: the labels get the room, the shortcut hints go */
@media (max-width: 300px) { #reader-menu { min-width: 0; } .reader-menu-key { display: none; } }
.reader-menu-sep { height: 1px; margin: 4px 6px; background: var(--border-color); }

/* Warnings stay readable and can be dismissed (the layer itself lets clicks through to the page) */
#reader-warnings > div:not([hidden]) { pointer-events: auto; display: flex; gap: 10px; align-items: flex-start; user-select: text; }
.reader-warning-text { flex: 1; min-width: 0; }
.reader-warning-close { flex: 0 0 auto; border: 0; background: none; color: inherit; cursor: pointer; font-size: 16px; line-height: 1; padding: 0 2px; opacity: 0.7; }
.reader-warning-close:hover { opacity: 1; }
/* Column grips are focusable (arrow keys resize): show them while focused */
.reader-col-grip:focus-visible { outline: none; background: var(--link-color); opacity: 0.6; }

/* Row filter bar above a table */
.reader-table-filter { display: flex; align-items: center; gap: 8px; margin: 1em 0 -0.6em; font: 12.5px var(--font-ui); color: var(--text-secondary); }
.reader-table-filter input, .toc-filter-box input {
    flex: 1 1 auto; min-width: 0; max-width: 320px; padding: 5px 9px; border: 1px solid var(--border-color); border-radius: 7px;
    background: var(--code-bg); color: var(--text-primary); font: inherit;
}
.reader-table-filter input:focus, .toc-filter-box input:focus { outline: 2px solid var(--link-color); outline-offset: 0; border-color: transparent; }
.reader-table-filter-count { white-space: nowrap; }

/* A picture that could not be shown names its file */
.reader-image-missing {
    display: inline-block; padding: 4px 9px; margin: 2px 0; border: 1px dashed var(--border-color); border-radius: 7px;
    color: var(--text-secondary); font: 12px var(--font-ui); overflow-wrap: anywhere;
}
#content img.reader-image-broken, #content img[data-remote-src]:not([src]) { display: none; }

/* Outline: filter field and the draggable left edge */
.toc-filter-box { padding: 10px 14px 0; font: 12.5px var(--font-ui); }
.toc-filter-box input { width: 100%; max-width: none; }
.toc-filter-empty { padding: 10px 18px 0; color: var(--text-secondary); font: 12.5px var(--font-ui); }
.toc-item[hidden] { display: none !important; }
#toc-resize { position: absolute; top: 0; bottom: 0; left: 0; width: 7px; cursor: col-resize; z-index: 2; touch-action: none; }
#toc-resize:hover, #toc-resize.active, #toc-resize:focus-visible { outline: none; background: var(--link-color); opacity: 0.45; }

/* Back after a jump */
#reader-back {
    position: fixed; left: 16px; bottom: 16px; z-index: 9985; padding: 7px 14px; border: 1px solid var(--border-color); border-radius: 18px;
    background: var(--toolbar-bg); backdrop-filter: blur(20px); -webkit-backdrop-filter: blur(20px); box-shadow: 0 4px 16px rgba(0, 0, 0, 0.18);
    color: var(--text-primary); font: 13px var(--font-ui); cursor: pointer;
}
#reader-back[hidden] { display: none; }
#reader-back:hover, #reader-back:focus-visible { color: var(--link-color); border-color: var(--link-color); outline: none; }

/* Remote image placeholder: the document asked for a network image; nothing is fetched until clicked */
.reader-remote-image {
    display: inline-flex; align-items: center; gap: 6px; max-width: 100%; padding: 6px 10px; margin: 2px 0;
    border: 1px dashed var(--border-color); border-radius: 8px; background: var(--code-bg); color: var(--text-secondary);
    font: 12px var(--font-ui); cursor: pointer; overflow: hidden; text-overflow: ellipsis; white-space: nowrap;
}
.reader-remote-image::before { content: '\1F5BC'; }
.reader-remote-image:hover { color: var(--link-color); border-color: var(--link-color); }

@media print {
    #toc-fab, #editor-fab, #reader-menu, #reader-back, .reader-copy-code, .reader-col-grip, .reader-remote-image, .reader-table-filter { display: none !important; }
    /* paper has no scrollbars: tables print whole, at page width, and the outline drawer's space is not reserved */
    body.toc-open #reader-wrap { padding-right: 20px; }
    #content .reader-table-wrap, #content .reader-table-wrap.reader-table-overflow {
        display: block; width: auto; max-height: none; overflow: visible; margin-inline-start: 0;
    }
    #content .reader-table-wrap > table { width: 100% !important; table-layout: auto !important; }
    #content .reader-table-wrap :is(th, td) { width: auto !important; }
    #content .reader-table-overflow thead th { position: static; }
    #content pre { white-space: pre-wrap; overflow-wrap: anywhere; }
}
)TOOLS_CSS";

inline constexpr const char* s_readerToolsHtml = R"TOOLS_HTML(
<button id="editor-fab" title="Hide editor (Ctrl+Shift+E)" aria-label="Hide editor (Ctrl+Shift+E)" aria-pressed="false" onclick="toggleHideEditorFromMenu()">
  <svg viewBox="0 0 24 24" width="16" height="16" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="16" rx="2"/><line x1="14" y1="4" x2="14" y2="20"/></svg>
</button>
<button id="toc-fab" title="Outline" aria-label="Outline" data-i18n-title="ui.outlineButton" data-i18n-aria="ui.outlineButton" aria-controls="toc-drawer" onclick="toggleTocFromMenu()">
  <svg viewBox="0 0 24 24" width="16" height="16" stroke="currentColor" stroke-width="2" fill="none" stroke-linecap="round"><path d="M9 6h11M9 12h11M9 18h11M4 6h.01M4 12h.01M4 18h.01"/></svg>
</button>
<div id="reader-menu" role="menu" hidden></div>
<button id="reader-back" type="button" title="Back to where you were (Alt+Left)" data-i18n="ui.back" data-i18n-title="ui.backTitle" onclick="goBackInDocument()" hidden>&#8592; Back</button>
)TOOLS_HTML";

inline constexpr const char* s_readerToolsScript = R"TOOLS_JS(
var READER_COPY_ICON = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="9" y="9" width="13" height="13" rx="2"/><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"/></svg>';
var READER_DONE_ICON = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round"><path d="M20 6 9 17l-5-5"/></svg>';

function postToHost(message) {
    if (!window.chrome || !window.chrome.webview) return false;
    window.chrome.webview.postMessage(message);
    return true;
}

// ---- Warnings ---------------------------------------------------------------------------------------------
function setReaderWarning(id, text) {
    var warning = document.getElementById(id);
    if (!warning) return;
    warning.hidden = !text;
    warning.textContent = '';
    if (!text) return;
    var span = document.createElement('span');
    span.className = 'reader-warning-text';
    span.textContent = text;
    var close = document.createElement('button');
    close.className = 'reader-warning-close';
    close.type = 'button';
    close.title = rt('warn.dismiss');
    close.setAttribute('aria-label', close.title);
    close.textContent = '×';
    close.addEventListener('click', function() { warning.hidden = true; warning.textContent = ''; });
    warning.append(span, close);
}
// A script failure outside the renderer's own error view would otherwise only reach the host's log.
window.addEventListener('error', function(event) {
    if (event.target !== window) return; // resource load errors (a missing image) are not script failures
    setReaderWarning('reader-script-warning', rt('warn.script', event.message || rt('warn.unknownError')));
});

// ---- Clipboard -------------------------------------------------------------------------------------------
// Runs inside the click that asked for it: execCommand('copy') with our own data works in any origin
// (the page is an opaque about:blank, where the async Clipboard API is not guaranteed).
function copyToClipboard(html, text) {
    var copied = false;
    function onCopy(event) {
        event.preventDefault();
        if (html) event.clipboardData.setData('text/html', html);
        event.clipboardData.setData('text/plain', text);
        copied = true;
    }
    document.addEventListener('copy', onCopy, true);
    try { document.execCommand('copy'); } catch (e) {} finally { document.removeEventListener('copy', onCopy, true); }
    if (!copied) setReaderWarning('reader-operation-warning', rt('warn.copyFailed'));
    return copied;
}
function codeText(pre) {
    var code = pre.querySelector('code');
    return (code || pre).textContent.replace(/\n$/, '');
}
// A copy of rendered content without what the reader adds on screen: grips, buttons, filters, table wrappers,
// widths set by dragging, source-line attributes. Remote images get their address back (the reader only held
// it aside). Shared by Copy table, Copy section and the HTML export.
var READER_DECORATIONS = '.reader-col-grip, .reader-copy-code, .reader-remote-image, .reader-image-missing, .reader-table-filter, .reader-legacy-anchor';
var READER_ATTRIBUTES = ['data-line', 'data-source-unmapped', 'data-task-key', 'data-highlighted', 'data-local-path'];
// Copies are built in a document without a window: pictures there never load, so copying or exporting fetches
// nothing the reader has not shown. Links keep only what a shared file may (LinkPolicy.h IsSafeLinkScheme):
// web, mail, in-page and relative targets.
var readerInertDocument = document.implementation.createHTMLDocument('');
var READER_SAFE_LINK = /^(?:(?:https?|mailto):|[^:]*(?:[\/?#]|$))/i;
function cleanReaderClone(node) {
    var clone = readerInertDocument.importNode(node, true);
    var all = [clone].concat(Array.from(clone.querySelectorAll('*')));
    clone.querySelectorAll(READER_DECORATIONS).forEach(function(element) { element.remove(); });
    clone.querySelectorAll('.reader-table-wrap').forEach(function(wrap) { wrap.replaceWith.apply(wrap, Array.from(wrap.childNodes)); });
    all.forEach(function(element) {
        if (!element.removeAttribute) return;
        READER_ATTRIBUTES.forEach(function(name) { element.removeAttribute(name); });
        if (element.matches('table, th, td, tr')) element.removeAttribute('style');
        if (element.matches('tr[hidden]')) element.removeAttribute('hidden');
        if (element.matches('img[data-remote-src]')) {
            var remote = element.getAttribute('data-remote-src');
            element.removeAttribute('data-remote-src');
            if (/^(?:https?:)?\/\//i.test(remote)) element.setAttribute('src', remote); // the address the reader moved aside
        }
        if (element.matches('a[href]') && !READER_SAFE_LINK.test(element.getAttribute('href').trim())) element.removeAttribute('href');
        element.classList && element.classList.remove('caret-target', 'reader-cols-fixed');
    });
    // a top-level table is its own wrapper: the copy is the table itself
    return clone.matches && clone.matches('.reader-table-wrap') ? clone.querySelector('table') || clone : clone;
}
function tableClipboard(table) {
    var clone = cleanReaderClone(table);
    // Tab-separated text pastes into spreadsheet cells; tabs/newlines inside a cell would split it.
    // From the clean copy: all rows, also those a row filter hides on screen.
    var text = Array.from(clone.rows).map(function(row) {
        return Array.from(row.cells).map(function(cell) { return cell.textContent.replace(/\s+/g, ' ').trim(); }).join('\t');
    }).join('\n');
    return {html: clone.outerHTML, text: text};
}

// ---- Code copy buttons -----------------------------------------------------------------------------------
function decorateCodeBlocks() {
    document.querySelectorAll('#content pre').forEach(function(pre) {
        if (pre.querySelector(':scope > .reader-copy-code')) return;
        var button = document.createElement('button');
        button.type = 'button';
        button.className = 'reader-copy-code';
        button.title = rt('ui.copyCode');
        button.setAttribute('aria-label', button.title);
        button.innerHTML = READER_COPY_ICON;
        button.addEventListener('click', function(event) {
            event.stopPropagation();
            if (!copyToClipboard('', codeText(pre))) return;
            button.classList.add('copied');
            button.innerHTML = READER_DONE_ICON;
            setTimeout(function() { button.classList.remove('copied'); button.innerHTML = READER_COPY_ICON; }, 1400);
        });
        button.addEventListener('dblclick', function(event) { event.stopPropagation(); });
        pre.prepend(button);
    });
}

// ---- Links -----------------------------------------------------------------------------------------------
// In-page anchors scroll here; everything else (web, mail, local files such as other.md) goes to the host,
// which opens web/mail links in the default app and local files in Notepad++.
function scrollToAnchor(id) {
    var target = document.getElementById(id) || document.querySelector('[name="' + CSS.escape(id) + '"]');
    if (target) { rememberReadingPlace(); target.scrollIntoView({block: 'start'}); }
    return !!target;
}

// ---- Back after a jump -----------------------------------------------------------------------------------
// Anchor links, footnotes and outline clicks jump; "Back" (button or Alt+Left) returns to where reading was.
var readerBackStack = [], readerBackBuffer = '';
function rememberReadingPlace() {
    readerBackStack.push(getReaderScrollRoot().scrollTop);
    if (readerBackStack.length > 30) readerBackStack.shift();
    updateBackButton();
}
function goBackInDocument() {
    if (!readerBackStack.length) return;
    getReaderScrollRoot().scrollTop = readerBackStack.pop();
    updateBackButton();
}
function updateBackButton() {
    var button = document.getElementById('reader-back');
    if (button) button.hidden = !readerBackStack.length;
}
document.addEventListener('keydown', function(event) {
    if (event.altKey && !event.ctrlKey && event.key === 'ArrowLeft' && readerBackStack.length) { event.preventDefault(); goBackInDocument(); }
});

// ---- Copy a section --------------------------------------------------------------------------------------
// The heading a top-level block belongs to, with everything up to the next heading of the same or higher level.
function sectionOf(target) {
    var content = document.getElementById('content');
    var node = target === content ? null : target.closest('#content > *');
    while (node && !/^H[1-6]$/.test(node.tagName)) node = node.previousElementSibling;
    if (!node) return null;
    var level = Number(node.tagName.charAt(1)), nodes = [node];
    for (var next = node.nextElementSibling; next && !(/^H[1-6]$/.test(next.tagName) && Number(next.tagName.charAt(1)) <= level); next = next.nextElementSibling) {
        if (!next.matches(READER_DECORATIONS)) nodes.push(next);
    }
    return {heading: node, nodes: nodes};
}
// Plain text of clean content with line breaks where blocks end: innerText needs layout, so it is measured on an
// off-screen copy styled like the document.
function renderedText(node) {
    var probe = node.cloneNode(true);
    probe.className = 'doc';
    probe.removeAttribute('id');
    probe.style.cssText = 'position:fixed;left:-100000px;top:0;width:800px';
    probe.querySelectorAll('[src]').forEach(function(element) { element.removeAttribute('src'); }); // text only: nothing loads on the way
    document.body.appendChild(probe);
    var text = probe.innerText.trim();
    probe.remove();
    return text;
}
function sectionClipboard(section) {
    var holder = readerInertDocument.createElement('div');
    section.nodes.forEach(function(node) { holder.appendChild(cleanReaderClone(node)); });
    return {html: holder.innerHTML, text: renderedText(holder)};
}

// ---- Export what the reader shows ------------------------------------------------------------------------
// Save/Copy HTML send the rendered body (one renderer for screen and file) and its plain text; the host wraps it
// in the theme's page, embeds the document's own pictures and writes the file or the clipboard.
function exportPayload() {
    var body = cleanReaderClone(document.getElementById('content'));
    var text = renderedText(body); // before the contents list, so headings are not listed twice
    var headings = window.readerDocumentIndex ? window.readerDocumentIndex.headings : [];
    if (headings.length >= 3) { // the same rule as the native exporter: longer documents get a contents list
        var nav = document.createElement('nav');
        nav.className = 'export-toc';
        nav.setAttribute('aria-label', rt('ui.contents'));
        headings.forEach(function(heading) {
            var link = document.createElement('a');
            link.className = 'toc-item toc-l' + heading.level;
            link.href = '#' + heading.id;
            link.textContent = heading.text;
            nav.appendChild(link);
        });
        body.prepend(nav);
    }
    var html = body.innerHTML;
    return '\n' + html.length + '\n' + html + text;
}
document.addEventListener('click', function(event) {
    if (event.button !== 0 || event.defaultPrevented) return;
    var link = event.target.closest && event.target.closest('#content a[href]');
    if (!link) return;
    var href = link.getAttribute('href') || '';
    event.preventDefault();
    if (href.charAt(0) === '#') {
        var id = href.slice(1);
        try { id = decodeURIComponent(id); } catch (e) {}
        scrollToAnchor(id);
        return;
    }
    if (href) postToHost('openLink:' + href);
});
// A middle click would ask WebView2 for a new window; links open only through the click route above.
document.addEventListener('auxclick', function(event) { if (event.target.closest && event.target.closest('#content a[href]')) event.preventDefault(); });

// ---- Find / print ----------------------------------------------------------------------------------------
function findInPreview() {
    var selection = String(window.getSelection ? window.getSelection() : '').trim().slice(0, 256);
    if (!postToHost(selection && selection.indexOf('\n') < 0 ? 'find:' + selection : 'find')) showToast(rt('toast.findPanelOnly'));
}
function printPreview() { closeReaderMenu(); if (!postToHost('print')) window.print(); }
// Ctrl+<letter> as the user means it: the typed letter on Latin layouts (Dvorak, AZERTY move keys), and the
// physical key where the layout types no Latin letter (Russian: Ctrl+А is the F key).
function shortcutLetter(event, letter) {
    var key = String(event.key || '').toLowerCase();
    return key === letter || (!/^[a-z]$/.test(key) && event.code === 'Key' + letter.toUpperCase());
}
document.addEventListener('keydown', function(event) {
    if (!event.ctrlKey || event.altKey || event.shiftKey) return;
    if (shortcutLetter(event, 'f')) { event.preventDefault(); findInPreview(); }
    else if (shortcutLetter(event, 'p')) { event.preventDefault(); printPreview(); }
});

// ---- Context menu ----------------------------------------------------------------------------------------
var readerMenuActions = [];
function closeReaderMenu() {
    var menu = document.getElementById('reader-menu');
    if (menu && !menu.hidden) { menu.hidden = true; menu.textContent = ''; readerMenuActions = []; }
}
function readerMenuItems(target) {
    var selection = String(window.getSelection ? window.getSelection() : '');
    var link = target.closest('#content a[href]');
    var pre = target.closest('#content pre');
    var table = target.closest('#content table');
    var section = sectionOf(target);
    var bufferId = window.readerDocumentIndex ? window.readerDocumentIndex.bufferId : '';
    var anyWidths = Array.from(readerColumnWidths.keys()).some(function(key) { return key.indexOf(bufferId + '|') === 0; });
    var items = [
        {label: rt('menu.copy'), key: 'Ctrl+C', disabled: !selection, run: function() { document.execCommand('copy'); }},
        link ? {label: rt('menu.copyLink'), run: function() { copyToClipboard('', link.getAttribute('href')); }} : null,
        pre ? {label: rt('menu.copyCode'), run: function() { copyToClipboard('', codeText(pre)); }} : null,
        table ? {label: rt('menu.copyTable'), run: function() { var data = tableClipboard(table); if (copyToClipboard(data.html, data.text)) showToast(rt('toast.tableCopied')); }} : null,
        section ? {label: rt('menu.copySection'), run: function() { var data = sectionClipboard(section); if (copyToClipboard(data.html, data.text)) showToast(rt('toast.sectionCopied')); }} : null,
        {label: rt('menu.selectAll'), key: 'Ctrl+A', run: function() { var range = document.createRange(); range.selectNodeContents(document.getElementById('content')); var s = window.getSelection(); s.removeAllRanges(); s.addRange(range); }},
        table ? 'separator' : null,
        table ? {label: rt('ui.filterRows'), run: function() { openTableFilter(table); }} : null,
        table && readerColumnWidths.has(table.readerKey) ? {label: rt('menu.resetWidths'), run: function() { resetColumnWidths(table.readerKey); }} : null,
        anyWidths ? {label: rt('menu.resetAllWidths'), run: function() { resetColumnWidths(bufferId + '|'); }} : null,
        'separator',
        {label: rt('menu.find'), key: 'Ctrl+F', run: findInPreview},
        {label: rt('menu.print'), key: 'Ctrl+P', run: printPreview},
        {label: rt(document.body.classList.contains('toc-open') ? 'menu.hideOutline' : 'menu.showOutline'), run: toggleTocFromMenu},
        {label: rt(window._hideEditor ? 'menu.showEditorCtx' : 'menu.hideEditorCtx'), key: 'Ctrl+Shift+E', run: toggleHideEditorFromMenu}
    ];
    return items.filter(Boolean);
}
function openReaderMenu(x, y, target) {
    var menu = document.getElementById('reader-menu');
    if (!menu) return;
    closeThemePopover();
    menu.textContent = '';
    readerMenuActions = [];
    readerMenuItems(target).forEach(function(item) {
        if (item === 'separator') { var sep = document.createElement('div'); sep.className = 'reader-menu-sep'; sep.setAttribute('role', 'separator'); menu.append(sep); return; }
        var button = document.createElement('button');
        button.type = 'button';
        button.className = 'reader-menu-item';
        button.setAttribute('role', 'menuitem');
        button.disabled = !!item.disabled;
        var label = document.createElement('span');
        label.textContent = item.label;
        label.title = item.label; // readable in full when a narrow panel ellipsizes it
        button.append(label);
        if (item.key) { var key = document.createElement('span'); key.className = 'reader-menu-key'; key.textContent = item.key; button.append(key); }
        button.addEventListener('click', function(event) { event.stopPropagation(); closeReaderMenu(); item.run(); });
        readerMenuActions.push(button);
        menu.append(button);
    });
    menu.hidden = false;
    var bounds = menu.getBoundingClientRect();
    menu.style.left = Math.max(8, Math.min(x, window.innerWidth - bounds.width - 8)) + 'px';
    menu.style.top = Math.max(8, Math.min(y, window.innerHeight - bounds.height - 8)) + 'px';
    var first = readerMenuActions.find(function(button) { return !button.disabled; });
    if (first) first.focus({preventScroll: true});
}
document.addEventListener('contextmenu', function(event) {
    if (event.target.closest('#theme-popover, #reader-menu, #toc-drawer')) { event.preventDefault(); return; }
    event.preventDefault();
    openReaderMenu(event.clientX, event.clientY, event.target);
});
document.addEventListener('mousedown', function(event) { if (!event.target.closest('#reader-menu')) closeReaderMenu(); }, true);
document.addEventListener('keydown', function(event) {
    var menu = document.getElementById('reader-menu');
    if (!menu || menu.hidden) return;
    if (event.key === 'Escape') { event.preventDefault(); event.stopPropagation(); closeReaderMenu(); return; }
    if (event.key === 'ArrowDown' || event.key === 'ArrowUp') {
        event.preventDefault();
        var buttons = readerMenuActions.filter(function(button) { return !button.disabled; });
        var index = buttons.indexOf(document.activeElement);
        buttons[(index + (event.key === 'ArrowDown' ? 1 : -1) + buttons.length) % buttons.length].focus();
    }
}, true);
window.addEventListener('blur', closeReaderMenu);
window.addEventListener('resize', closeReaderMenu);
document.addEventListener('scroll', closeReaderMenu, {passive: true, capture: true});

// ---- Tables ----------------------------------------------------------------------------------------------
// Column widths the user dragged: "bufferId|tableFingerprint" -> [px]. The native host persists them per file
// (tableWidths messages both ways), so they survive re-renders and Notepad++ restarts.
var readerColumnWidths = new Map();
// Scroll offsets inside overflowing tables, same keys; kept for the session so an edit does not reset them.
var readerTableScroll = new Map();
var readerTableObserver = typeof ResizeObserver === 'function' ? new ResizeObserver(markOverflowingTables) : null;

function markOverflowingTables() {
    document.querySelectorAll('#content .reader-table-wrap').forEach(function(wrap) {
        wrap.classList.toggle('reader-table-overflow', wrap.scrollWidth > wrap.clientWidth + 1);
    });
}
function applyColumnWidths(table, widths) {
    var cells = Array.from(table.rows[0].cells);
    table.classList.toggle('reader-cols-fixed', !!widths);
    table.style.tableLayout = widths ? 'fixed' : '';
    table.style.width = widths ? widths.reduce(function(sum, w) { return sum + w; }, 0) + 'px' : '';
    cells.forEach(function(cell, i) { cell.style.width = widths ? widths[i] + 'px' : ''; });
}
// A pointer drag on a handle: onMove(dx) receives the horizontal distance from where the drag started,
// onEnd() runs once when it is released. Shared by column grips and the outline's edge.
function trackHorizontalDrag(event, handle, onMove, onEnd) {
    if (event.button !== 0) return;
    event.preventDefault(); event.stopPropagation();
    var startX = event.clientX;
    handle.setPointerCapture(event.pointerId);
    handle.classList.add('active');
    document.body.classList.add('reader-col-resizing');
    function move(e) { onMove(e.clientX - startX); }
    function end() {
        handle.removeEventListener('pointermove', move); handle.removeEventListener('pointerup', end); handle.removeEventListener('pointercancel', end);
        handle.classList.remove('active');
        document.body.classList.remove('reader-col-resizing');
        onEnd();
    }
    handle.addEventListener('pointermove', move); handle.addEventListener('pointerup', end); handle.addEventListener('pointercancel', end);
}
function measuredColumnWidths(table) {
    return Array.from(table.rows[0].cells).map(function(cell) { return Math.round(cell.getBoundingClientRect().width); });
}
function commitColumnWidths(table, key, widths) {
    readerColumnWidths.delete(key); // re-insert: the most recently resized table goes last
    readerColumnWidths.set(key, widths.slice());
    applyColumnWidths(table, widths);
    persistTableWidths(key);
    markOverflowingTables();
}
function startColumnResize(event, table, key, column) {
    var widths = measuredColumnWidths(table), startWidth = widths[column];
    var rtl = getComputedStyle(table).direction === 'rtl';
    trackHorizontalDrag(event, event.currentTarget, function(dx) {
        widths[column] = Math.max(40, startWidth + Math.round(rtl ? -dx : dx));
        applyColumnWidths(table, widths);
    }, function() { commitColumnWidths(table, key, widths); });
}
// Keyboard on a focused grip: arrows widen/narrow by 16px (Shift: 64px), Delete/Backspace resets the table.
function columnGripKey(event, table, key, column) {
    var step = event.shiftKey ? 64 : 16, rtl = getComputedStyle(table).direction === 'rtl';
    var delta = {ArrowRight: step, ArrowLeft: -step}[event.key];
    if (delta !== undefined) {
        event.preventDefault();
        var widths = readerColumnWidths.get(key) || measuredColumnWidths(table);
        widths = widths.slice();
        widths[column] = Math.max(40, widths[column] + (rtl ? -delta : delta));
        commitColumnWidths(table, key, widths);
    } else if (event.key === 'Delete' || event.key === 'Backspace') {
        event.preventDefault();
        resetColumnWidths(key);
    }
}
// key: one table ("buffer|fp") or every table of a document ("buffer|").
function resetColumnWidths(key) {
    Array.from(readerColumnWidths.keys()).forEach(function(k) { if (k === key || (key.slice(-1) === '|' && k.indexOf(key) === 0)) readerColumnWidths.delete(k); });
    document.querySelectorAll('#content table').forEach(function(table) {
        if (table.readerKey && (table.readerKey === key || (key.slice(-1) === '|' && table.readerKey.indexOf(key) === 0))) applyColumnWidths(table, null);
    });
    persistTableWidths(key);
    markOverflowingTables();
}

// ---- Row filter ------------------------------------------------------------------------------------------
// Hides body rows that do not contain the typed text (case- and ё/е-insensitive); kept per table for the session.
var readerTableFilters = new Map();
function normalizedText(text) { return String(text).toLowerCase().replace(/ё/g, 'е'); }
function applyTableFilter(table, query) {
    var needle = normalizedText(query.trim()), shown = 0, total = 0;
    Array.from(table.tBodies).forEach(function(body) {
        Array.from(body.rows).forEach(function(row) {
            total++;
            var visible = !needle || normalizedText(row.textContent).indexOf(needle) >= 0;
            row.hidden = !visible;
            if (visible) shown++;
        });
    });
    return {shown: shown, total: total};
}
function openTableFilter(table, initial) {
    var wrap = table.closest('.reader-table-wrap') || table;
    var bar = wrap.previousElementSibling && wrap.previousElementSibling.classList.contains('reader-table-filter') ? wrap.previousElementSibling : null;
    if (!bar) {
        bar = document.createElement('div');
        bar.className = 'reader-table-filter';
        var input = document.createElement('input');
        input.type = 'text'; // the bar has its own close button (a search field would add a second ×)
        input.placeholder = rt('ui.filterRows');
        input.setAttribute('aria-label', input.placeholder);
        var count = document.createElement('span');
        count.className = 'reader-table-filter-count';
        var close = document.createElement('button');
        close.type = 'button';
        close.className = 'reader-warning-close';
        close.title = rt('ui.closeFilter');
        close.setAttribute('aria-label', close.title);
        close.textContent = '×';
        var update = function() {
            var result = applyTableFilter(table, input.value);
            count.textContent = input.value.trim() ? result.shown + ' / ' + result.total : '';
            if (input.value.trim()) readerTableFilters.set(table.readerKey, input.value); else readerTableFilters.delete(table.readerKey);
            markOverflowingTables();
        };
        var dismiss = function() { readerTableFilters.delete(table.readerKey); applyTableFilter(table, ''); bar.remove(); markOverflowingTables(); };
        input.addEventListener('input', update);
        input.addEventListener('keydown', function(event) { if (event.key === 'Escape') { event.preventDefault(); event.stopPropagation(); dismiss(); } });
        close.addEventListener('click', dismiss);
        bar.append(input, count, close);
        bar.addEventListener('dblclick', function(event) { event.stopPropagation(); });
        wrap.before(bar);
        if (initial) { input.value = initial; update(); }
    }
    if (!initial) bar.querySelector('input').focus();
}
// A table is identified by its header text (+ the column count and its occurrence among same-header tables),
// so widths follow the table when other tables are added or removed around it.
function tableFingerprint(table, seen) {
    var text = Array.from(table.rows[0].cells).map(function(cell) { return cell.textContent.trim(); }).join('\u0001');
    seen[text] = (seen[text] || 0) + 1;
    text += '\u0002' + table.rows[0].cells.length + '\u0002' + seen[text];
    var hash = 0x811c9dc5;
    for (var i = 0; i < text.length; i++) { hash ^= text.charCodeAt(i); hash = Math.imul(hash, 0x01000193) >>> 0; }
    return ('0000000' + hash.toString(16)).slice(-8);
}
// Wire format (validated natively): "fp:w,w,...;fp:w,..." with fp = 8 hex digits.
function persistTableWidths(key) {
    var bufferId = key.slice(0, key.indexOf('|'));
    if (!/^\d+$/.test(bufferId)) return;
    var entries = [];
    readerColumnWidths.forEach(function(widths, k) {
        if (k.indexOf(bufferId + '|') === 0) entries.push(k.slice(bufferId.length + 1) + ':' + widths.join(','));
    });
    // Oldest first (a resize moves its table to the end): keep the most recently resized tables when a document
    // collects more than the host stores. A table whose header is being edited keeps its widths meanwhile.
    var value = entries.slice(-60).join(';');
    while (value.length > 4096) value = value.indexOf(';') < 0 ? '' : value.slice(value.indexOf(';') + 1);
    postToHost('tableWidths:' + bufferId + ':' + value);
}
function receiveTableWidths(bufferId, value) {
    if (!/^\d+$/.test(bufferId) || !/^[0-9a-f:,;]*$/.test(value)) return;
    Array.from(readerColumnWidths.keys()).forEach(function(k) { if (k.indexOf(bufferId + '|') === 0) readerColumnWidths.delete(k); });
    value.split(';').forEach(function(entry) {
        var match = /^([0-9a-f]{8}):(\d{1,4}(?:,\d{1,4})*)$/.exec(entry);
        if (match) readerColumnWidths.set(bufferId + '|' + match[1], match[2].split(',').map(Number));
    });
    if (window.readerDocumentIndex && window.readerDocumentIndex.bufferId === bufferId) layoutReaderTables(window.readerDocumentIndex);
}
function layoutReaderTables(detail) {
    var content = document.getElementById('content');
    if (!content) return;
    var bufferId = detail && detail.bufferId || '';
    var seen = Object.create(null), restore = [];
    Array.from(content.querySelectorAll('table')).filter(function(table) {
        return table.id !== 'hrdftrtbl' && !table.parentElement.closest('table') && table.rows.length;
    }).forEach(function(table) {
        var cells = Array.from(table.rows[0].cells);
        var key = bufferId + '|' + tableFingerprint(table, seen);
        var wrap = table.parentElement;
        if (!wrap.classList.contains('reader-table-wrap')) {
            wrap = document.createElement('div');
            wrap.className = 'reader-table-wrap';
            table.before(wrap); wrap.appendChild(table);
            wrap.addEventListener('scroll', function() {
                readerTableScroll.delete(key);
                if (wrap.scrollLeft || wrap.scrollTop) readerTableScroll.set(key, {left: wrap.scrollLeft, top: wrap.scrollTop});
                while (readerTableScroll.size > 200) readerTableScroll.delete(readerTableScroll.keys().next().value);
            }, {passive: true});
        }
        table.readerKey = key;
        cells.forEach(function(cell, column) {
            if (cell.querySelector(':scope > .reader-col-grip')) return;
            var grip = document.createElement('span');
            grip.className = 'reader-col-grip';
            grip.title = rt('ui.resizeColumn');
            grip.tabIndex = 0;
            grip.setAttribute('role', 'separator');
            grip.setAttribute('aria-orientation', 'vertical');
            grip.setAttribute('aria-label', rt('ui.resizeColumnLabel', column + 1));
            grip.addEventListener('pointerdown', function(e) { startColumnResize(e, table, key, column); });
            grip.addEventListener('keydown', function(e) { columnGripKey(e, table, key, column); });
            grip.addEventListener('dblclick', function(e) { e.stopPropagation(); resetColumnWidths(key); });
            cell.appendChild(grip);
        });
        var saved = readerColumnWidths.get(key);
        applyColumnWidths(table, saved && saved.length === cells.length ? saved : null);
        if (readerTableFilters.has(key)) openTableFilter(table, readerTableFilters.get(key));
        if (readerTableScroll.has(key)) restore.push([wrap, readerTableScroll.get(key)]);
    });
    if (readerTableObserver) {
        readerTableObserver.observe(document.getElementById('reader-wrap'));
        readerTableObserver.observe(content);
    }
    markOverflowingTables();
    // after the overflow class: only then do overflowing wraps have their own vertical scroll range
    restore.forEach(function(pair) { pair[0].scrollLeft = pair[1].left; pair[0].scrollTop = pair[1].top; });
}

// ---- Images ----------------------------------------------------------------------------------------------
// A relative src ("img/a.png", "./a.png", "sub/../a.png") is served from the document's folder through the
// host's virtual folder mapping (docBase). Paths that climb above that folder, absolute paths and URLs stay as
// written: the mapping exposes nothing outside the document's own folder tree.
function documentRelativeUrl(src, base) {
    if (!base || !src || /^(?:[a-z][a-z0-9+.-]*:|\/\/|\/|\\|#)/i.test(src)) return null;
    var path = src.replace(/\\/g, '/').split(/[?#]/)[0], parts = [];
    var segments = path.split('/');
    for (var i = 0; i < segments.length; i++) {
        var segment = segments[i];
        if (!segment || segment === '.') continue;
        if (segment === '..') { if (!parts.length) return null; parts.pop(); continue; }
        var decoded = segment;
        try { decoded = decodeURIComponent(segment); } catch (e) {}
        // an encoded separator, a stream name, a device name or a dot/space-trimmed ".." (Windows drops trailing
        // dots and spaces) must not climb or reach a device
        if (/[\\/:\0]/.test(decoded) || /[. ]$/.test(decoded) || /^(?:con|prn|aux|nul|conin\$|conout\$|com[\d¹²³]|lpt[\d¹²³])(?:\.|$)/i.test(decoded)) return null;
        parts.push(decoded);
    }
    if (!parts.length) return null;
    return base + parts.map(encodeURIComponent).join('/');
}
// A picture that cannot be shown says which file and why, instead of leaving an empty gap.
function markMissingImage(image, written, reason) {
    if (image.nextElementSibling && image.nextElementSibling.classList.contains('reader-image-missing')) return;
    var note = document.createElement('span');
    note.className = 'reader-image-missing';
    note.textContent = reason + ': ' + written;
    note.title = written;
    image.classList.add('reader-image-broken');
    image.after(note);
}
function resolveDocumentImages(detail) {
    var base = detail && detail.docBase || '';
    document.querySelectorAll('#content img[data-local-path]').forEach(function(image) {
        var path = image.getAttribute('data-local-path');
        try { path = decodeURIComponent(path); } catch (e) {}
        markMissingImage(image, path, rt('ui.imageAbsolute'));
    });
    document.querySelectorAll('#content img[src]').forEach(function(image) {
        var written = image.getAttribute('src');
        if (/^[a-z]:(?:[\\/]|%5c)/i.test(written)) { // Marked writes "C:\x" as "C:%5Cx"
            markMissingImage(image, written, rt('ui.imageAbsolute'));
            image.removeAttribute('src');
            return;
        }
        if (/^(?:[a-z][a-z0-9+.-]*:|\/\/)/i.test(written)) return; // data:, already resolved, or remote (held aside)
        var url = documentRelativeUrl(written, base);
        if (!url) {
            // relative but not servable: an unsaved document has no folder; a path above the folder is refused
            markMissingImage(image, written, rt(base ? 'ui.imageOutside' : 'ui.imageUnsaved'));
            image.removeAttribute('src');
            return;
        }
        // The host maps the folder just before the render, and the mapping API has no "applied" signal: a request
        // that races it fails once. One delayed retry (of the same URL) covers that; a second failure is real.
        image.addEventListener('error', function() {
            setTimeout(function() {
                if (!image.isConnected || image.getAttribute('src') !== url) return;
                image.addEventListener('error', function() { markMissingImage(image, written, rt('ui.imageNotFound')); }, {once: true});
                image.src = url;
            }, 400);
        }, {once: true});
        image.src = url;
    });
}

// Remote images arrive without src (the sanitizer moved it to data-remote-src): offer to load each on request.
function decorateRemoteImages() {
    document.querySelectorAll('#content img[data-remote-src]:not([src])').forEach(function(image) {
        if (image.previousElementSibling && image.previousElementSibling.classList.contains('reader-remote-image')) return;
        var url = image.getAttribute('data-remote-src'), host = '';
        try { host = new URL(url, 'https://invalid.local/').host; } catch (e) {}
        var button = document.createElement('button');
        button.type = 'button';
        button.className = 'reader-remote-image';
        button.textContent = rt('ui.showImage', host || rt('ui.theInternet')) + (image.alt ? ': ' + image.alt : '');
        button.title = url;
        button.addEventListener('click', function(event) {
            event.stopPropagation();
            image.src = url;
            button.remove();
        });
        button.addEventListener('dblclick', function(event) { event.stopPropagation(); });
        image.before(button);
    });
}

// ---- Outline: filter and width ---------------------------------------------------------------------------
// A filter field under the outline's title and a draggable left edge (width kept in the settings).
var OUTLINE_WIDTH = {min: window.READER_SETTINGS.outlineWidthPx.min, max: window.READER_SETTINGS.outlineWidthPx.max, step: 24};
function applyOutlineFilter() {
    var input = document.getElementById('toc-filter');
    var needle = input ? normalizedText(input.value.trim()) : '';
    var links = document.querySelectorAll('#toc-drawer .toc-item'), shown = 0;
    links.forEach(function(link) {
        var visible = !needle || normalizedText(link.textContent).indexOf(needle) >= 0;
        link.hidden = !visible;
        if (visible) shown++;
    });
    var empty = document.getElementById('toc-filter-empty');
    if (empty) empty.hidden = !(needle && links.length && !shown);
}
function setOutlineWidth(width, persist) {
    width = Math.max(OUTLINE_WIDTH.min, Math.min(OUTLINE_WIDTH.max, Math.round(width)));
    document.body.style.setProperty('--toc-width', width + 'px');
    if (persist) postToHost('outlineWidth:' + width);
    return width;
}
function currentOutlineWidth() {
    var drawer = document.getElementById('toc-drawer');
    return drawer ? Math.round(drawer.getBoundingClientRect().width) : 280;
}
function decorateOutline() {
    var drawer = document.getElementById('toc-drawer');
    if (!drawer || document.getElementById('toc-filter')) return;
    var box = document.createElement('div');
    box.className = 'toc-filter-box';
    var input = document.createElement('input');
    input.id = 'toc-filter';
    input.type = 'search';
    input.placeholder = rt('ui.filterHeadings');
    input.setAttribute('aria-label', input.placeholder);
    input.dataset.i18nPlaceholder = input.dataset.i18nAria = 'ui.filterHeadings';
    input.addEventListener('input', applyOutlineFilter);
    input.addEventListener('keydown', function(event) {
        if (event.key === 'Escape' && input.value) { event.preventDefault(); event.stopPropagation(); input.value = ''; applyOutlineFilter(); }
        if (event.key === 'Enter') { var first = drawer.querySelector('.toc-item:not([hidden])'); if (first) { event.preventDefault(); first.click(); } }
    });
    var empty = document.createElement('div');
    empty.id = 'toc-filter-empty';
    empty.className = 'toc-filter-empty';
    empty.hidden = true;
    empty.textContent = rt('ui.noMatchingHeadings');
    empty.dataset.i18n = 'ui.noMatchingHeadings';
    box.append(input);
    var header = drawer.querySelector('.toc-header');
    if (header) header.after(box); else drawer.prepend(box);
    var content = drawer.querySelector('.toc-content');
    if (content) content.before(empty);
    var edge = document.createElement('div');
    edge.id = 'toc-resize';
    edge.tabIndex = 0;
    edge.title = rt('ui.resizeOutline');
    edge.setAttribute('role', 'separator');
    edge.setAttribute('aria-orientation', 'vertical');
    edge.setAttribute('aria-label', rt('ui.resizeOutlineLabel'));
    edge.dataset.i18nTitle = 'ui.resizeOutline';
    edge.dataset.i18nAria = 'ui.resizeOutlineLabel';
    edge.addEventListener('pointerdown', function(event) {
        var start = currentOutlineWidth(), width = start;
        // the drawer sits on the right: dragging its left edge leftwards widens it
        trackHorizontalDrag(event, edge, function(dx) { width = setOutlineWidth(start - dx, false); }, function() { setOutlineWidth(width, true); });
    });
    edge.addEventListener('keydown', function(event) {
        var delta = {ArrowLeft: OUTLINE_WIDTH.step, ArrowRight: -OUTLINE_WIDTH.step}[event.key];
        if (delta !== undefined) { event.preventDefault(); setOutlineWidth(currentOutlineWidth() + delta, true); }
    });
    drawer.prepend(edge);
}

// ---- Wiring ----------------------------------------------------------------------------------------------
function decorateReaderDocument(detail) {
    closeReaderMenu();
    var bufferId = detail && detail.bufferId || '';
    if (bufferId !== readerBackBuffer) { readerBackBuffer = bufferId; readerBackStack = []; updateBackButton(); }
    applyOutlineFilter();
    decorateRemoteImages();
    resolveDocumentImages(detail);
    layoutReaderTables(detail);
    decorateCodeBlocks();
}
if (window.chrome && window.chrome.webview) {
    window.chrome.webview.addEventListener('message', function(event) {
        var data = event.data;
        if (typeof data === 'string') { try { data = JSON.parse(data); } catch (e) { return; } }
        if (data && data.type === 'notice' && typeof data.text === 'string') showToast(data.text, 6000);
        if (data && data.type === 'scrollToAnchor' && typeof data.id === 'string') {
            var id = data.id;
            try { id = decodeURIComponent(id); } catch (e) {}
            if (!scrollToAnchor(id)) showToast(rt('toast.sectionMissing', id), 5000);
        }
        if (data && data.type === 'settings' && Number.isInteger(data.outlineWidthPx)) setOutlineWidth(data.outlineWidthPx, false);
    });
}
document.addEventListener('readerRendered', function(event) { decorateReaderDocument(event.detail); });
function initializeReaderTools() {
    decorateOutline();
    if (window.readerDocumentIndex) decorateReaderDocument(window.readerDocumentIndex);
}
if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', initializeReaderTools);
else initializeReaderTools();
)TOOLS_JS";
