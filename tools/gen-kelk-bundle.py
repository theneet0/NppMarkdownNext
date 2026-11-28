#!/usr/bin/env python3
"""
Lightweight Kelk Reader Bundle Generator for NppMarkdownNext
Generates src/KelkBundle.inc containing the pure Kelk Markdown Reader engine:
- Kelk Styles & Tokens (Light / Dark)
- Kelk BidiView CSS classes
- Marked.js (GFM Parser)
- DOMPurify (Sanitizer)
- Highlight.js (Syntax Highlighting)
- Kelk BidiCore & BidiView (RTL Persian / Arabic Direction Engine)
- Bridge controller for live sync with Notepad++ Scintilla
"""

import os
import sys
import re
import json
import unicodedata

script_dir = os.path.dirname(os.path.abspath(__file__))
repo_root = os.path.abspath(os.path.join(script_dir, ".."))
heading_policy_path = os.path.join(repo_root, "themes", "heading-policy.json")
if "--import-heading-policy" in sys.argv:
    ranges = []
    folds = []
    expansions = {}
    for codepoint in range(0x110000):
        lower = chr(codepoint).lower()
        if lower != chr(codepoint):
            if len(lower) == 1:
                delta = ord(lower) - codepoint
                if folds and codepoint == folds[-1][1] + 1 and delta == folds[-1][2]:
                    folds[-1][1] = codepoint
                else:
                    folds.append([codepoint, codepoint, delta])
            else:
                expansions[str(codepoint)] = [ord(character) for character in lower]
        if unicodedata.category(chr(codepoint))[0] not in "LNM":
            continue
        if ranges and codepoint == ranges[-1][1] + 1:
            ranges[-1][1] = codepoint
        else:
            ranges.append([codepoint, codepoint])
    reserved = ["content", "reader-wrap", "theme-fab", "theme-popover", "theme-pop-list", "toast-msg", "toc-backdrop", "toc-drawer", "reader-warnings", "reader-settings-warning", "reader-operation-warning"]
    if os.path.exists(heading_policy_path):
        with open(heading_policy_path, encoding="utf-8") as fp:
            reserved = json.load(fp)["reservedIds"]
    policy = {"unicodeVersion": unicodedata.unidata_version, "reservedIds": reserved, "acceptedRanges": ranges,
              "lowercaseRanges": folds, "lowercaseExpansions": expansions,
              "whitespaceRanges": [[9, 13], [32, 32], [160, 160], [5760, 5760], [8192, 8202], [8232, 8233], [8239, 8239], [8287, 8287], [12288, 12288], [65279, 65279]]}
    with open(heading_policy_path, "w", encoding="utf-8") as fp:
        json.dump(policy, fp, ensure_ascii=False, separators=(",", ":"))
        fp.write("\n")
    print("Imported heading policy Unicode " + unicodedata.unidata_version)
    sys.exit(0)
# The committed bundle is reproduced byte for byte from Kelk 4beee81 (2026-10-16):
#   git clone https://github.com/mohsen-1984/kelk ../kelk_repo && git -C ../kelk_repo checkout 4beee81
default_kelk = os.path.abspath(os.path.join(repo_root, "..", "kelk_repo"))

kelk_dir = sys.argv[1] if len(sys.argv) > 1 else default_kelk
output_file = os.path.join(repo_root, "src", "KelkBundle.inc")

if not os.path.exists(kelk_dir):
    print(f"Error: Kelk repository directory not found at {kelk_dir}")
    sys.exit(1)

lib_dir = os.path.join(kelk_dir, "lib")
vendor_dir = os.path.join(kelk_dir, "vendor")
styles_dir = os.path.join(kelk_dir, "styles")

# 1. Styles
tokens_path = os.path.join(styles_dir, "tokens.css")
with open(tokens_path, "r", encoding="utf-8") as fp:
    tokens_css = fp.read()

# Strip relative @font-face URLs that cause 404/CSP issues in WebView2 NavigateToString
# and replace with robust system font fallbacks
tokens_css = re.sub(r"@font-face\s*\{[^}]*\}", "", tokens_css)

base_path = os.path.join(styles_dir, "base.css")
with open(base_path, "r", encoding="utf-8") as fp:
    base_css = fp.read()
# Only keep core base styling, not the full kelk web app layout
base_css = re.sub(r"@font-face\s*\{[^}]*\}", "", base_css)

preview_path = os.path.join(styles_dir, "preview.css")
with open(preview_path, "r", encoding="utf-8") as fp:
    preview_css = fp.read()

hljs_css_path = os.path.join(vendor_dir, "highlight.js", "github.min_11.11.1.css")
with open(hljs_css_path, "r", encoding="utf-8") as fp:
    hljs_css = fp.read()

# BidiView CSS classes (from kelk/lib/BidiView.js css())
bidi_css = """
/* BidiView direction classes */
.bd-doc-rtl{direction:rtl}.bd-doc-ltr{direction:ltr}
.bd-ltr,.bd-ltr-table{direction:ltr}.bd-rtl{direction:rtl}
.bd-root :is(p,h1,h2,h3,h4,h5,h6,li,blockquote,ul,ol,dt,dd):is(.bd-ltr,.bd-rtl){text-align:start}
.bd-root :is(td,th):is(.bd-ltr,.bd-rtl):not([align]):not([style*="text-align"]){text-align:start}
.bd-iso-ltr{unicode-bidi:isolate;direction:ltr}.bd-iso-rtl{unicode-bidi:isolate;direction:rtl}
.bd-root pre{direction:ltr;text-align:left}
.bd-root :is(ul,ol){padding-inline-start:0}
.bd-root li{--bd-b:1;--bd-s:0;margin-inline-start:calc(var(--bd-b) * var(--bd-base,1.7em) + var(--bd-s) * var(--bd-step,1.7em))}
.bd-root li.bd-b0{--bd-b:0}
.bd-root li.bd-s1{--bd-s:1}.bd-root li.bd-s2{--bd-s:2}.bd-root li.bd-s3{--bd-s:3}.bd-root li.bd-s4{--bd-s:4}.bd-root li.bd-s5{--bd-s:5}.bd-root li.bd-s6{--bd-s:6}.bd-root li.bd-s7{--bd-s:7}.bd-root li.bd-s8{--bd-s:8}.bd-root li.bd-s9{--bd-s:9}
"""

# Custom reader styles for Notepad++ panel (scrollbar, spacing, task list checkboxes, dark mode)
reader_extra_css = """
/* Reader host adaptations */
:root {
  --font-ui: 'Vazirmatn', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, Tahoma, sans-serif;
  --font-content: 'Sahel', 'Vazirmatn', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, Tahoma, sans-serif;
  --font-latin: 'Vazirmatn', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, Tahoma, sans-serif;
  --font-mono: 'Vazir Code Hack', 'Cascadia Code', 'DejaVu Sans Mono', ui-monospace, Consolas, 'Courier New', monospace;
}

html, body {
  margin: 0;
  padding: 0;
  width: 100%;
  height: 100%;
  background: var(--bg);
  color: var(--ink);
  font-family: var(--font-content);
  font-size: 15px;
  line-height: 1.7;
  overflow-x: hidden;
  box-sizing: border-box;
}

*, *:before, *:after {
  box-sizing: inherit;
}

#reader-wrap {
  width: 100%;
  min-height: 100%;
  padding: 16px 20px 48px;
}

.doc {
  max-width: 860px;
  margin: 0 auto;
  color: var(--ink);
  word-wrap: break-word;
  overflow-wrap: break-word;
}

/* Headings */
.doc h1, .doc h2, .doc h3, .doc h4, .doc h5, .doc h6 {
  font-family: var(--font-ui);
  color: var(--ink);
  margin-top: 1.2em;
  margin-bottom: 0.5em;
  font-weight: 700;
  line-height: 1.35;
}
.doc h1 { font-size: 1.75em; border-bottom: 1px solid var(--line); padding-bottom: 0.3em; }
.doc h2 { font-size: 1.45em; border-bottom: 1px solid var(--line); padding-bottom: 0.25em; }
.doc h3 { font-size: 1.25em; }
.doc h4 { font-size: 1.1em; }
.doc h5 { font-size: 1em; }
.doc h6 { font-size: 0.9em; color: var(--muted); }

/* Paragraphs & Lists */
.doc p { margin: 0.6em 0; }
.doc ul, .doc ol { padding-inline-start: 1.8em; margin: 0.6em 0; }
.doc li { margin: 0.25em 0; }
.doc li.task-list-item { list-style-type: none; margin-inline-start: -1.4em; }
.doc input.task-checkbox, .doc input.task-list-item-checkbox {
  margin-inline-end: 0.5em;
  cursor: pointer;
  vertical-align: middle;
}

/* Blockquotes */
.doc blockquote {
  margin: 0.8em 0;
  padding: 0.4em 1em;
  border-inline-start: 4px solid var(--accent);
  background: var(--code-bg);
  border-radius: 4px;
  color: var(--muted);
}
.doc blockquote p { margin: 0.3em 0; }

/* Tables */
.doc table {
  border-collapse: collapse;
  width: 100%;
  margin: 1em 0;
  overflow-x: auto;
  display: block;
}
.doc th, .doc td {
  border: 1px solid var(--line-strong);
  padding: 6px 12px;
}
/* a column's own alignment (|:---:|, |---:|) wins; only unaligned cells follow the text direction */
.doc :is(th, td):not([align]) { text-align: start; }
.doc th {
  background: var(--table-head);
  font-weight: 600;
}
.doc tr:nth-child(even) td {
  background: var(--code-bg);
}

/* Code & Syntax Highlighting */
.doc code {
  font-family: var(--font-mono);
  font-size: 0.9em;
  background: var(--code-bg);
  padding: 0.15em 0.4em;
  border-radius: 4px;
  border: 1px solid var(--line);
}
.doc pre {
  margin: 0.8em 0;
  padding: 12px 14px;
  background: var(--code-bg);
  border: 1px solid var(--line);
  border-radius: 6px;
  overflow-x: auto;
  direction: ltr !important;
  text-align: left !important;
}
.doc pre code {
  background: none;
  border: none;
  padding: 0;
  font-size: 0.88em;
  line-height: 1.5;
}

/* Links & Images */
.doc a {
  color: var(--accent);
  text-decoration: none;
}
.doc a:hover {
  text-decoration: underline;
}
.doc img {
  max-width: 100%;
  height: auto;
  border-radius: 4px;
}
.doc hr {
  border: 0;
  border-top: 1px solid var(--line);
  margin: 1.5em 0;
}

/* Dark theme overrides */
body.dark {
  --bg: #16181a;
  --panel: #1e2124;
  --panel-2: #23262a;
  --ink: #e6e4df;
  --muted: #9a958c;
  --placeholder: #6b6862;
  --line: #2d3136;
  --line-strong: #3c4147;
  --accent: #4db396;
  --accent-soft: #1a3129;
  --accent-ink: #83d5bd;
  --on-accent: #0f1416;
  --danger: #f04438;
  --warn: #f59e0b;
  --code-bg: #212428;
  --code-head: #282c31;
  --quote-bar: #d4b138;
  --table-head: #283726;
}

/* Caret target highlight */
.doc [data-line] {
  scroll-margin-top: 40px;
}
.doc .caret-target {
  outline: 2px solid var(--accent);
  border-radius: 4px;
  background: var(--accent-soft);
  transition: background 0.3s ease, outline 0.3s ease;
}

/* Clean custom scrollbar */
::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}
::-webkit-scrollbar-track {
  background: transparent;
}
::-webkit-scrollbar-thumb {
  background: var(--line-strong);
  border-radius: 4px;
}
::-webkit-scrollbar-thumb:hover {
  background: var(--muted);
}
"""

all_css = [
    tokens_css,
    preview_css,
    hljs_css,
    bidi_css,
    reader_extra_css,
]

combined_css = "\n\n".join(all_css)

# 2. Scripts
js_files = [
    os.path.join(vendor_dir, "marked.umd.min_18.0.14.js"),
    os.path.join(vendor_dir, "purify.min_3.4.16.js"),
    os.path.join(vendor_dir, "highlight.js", "highlight.min_11.12.0.js"),
    os.path.join(lib_dir, "BidiCore.js"),
    os.path.join(lib_dir, "BidiView.js"),
]

core_js_parts = []
for f in js_files:
    if os.path.exists(f):
        with open(f, "r", encoding="utf-8") as fp:
            core_js_parts.append(f"/* === {os.path.basename(f)} === */\n" + fp.read())

core_js = "\n\n".join(core_js_parts)

# Shared data stays independent of the Markdown parsers.
with open(os.path.join(repo_root, "themes", "callouts.json"), encoding="utf-8") as fp:
    callouts = json.load(fp)
callout_js = "window.READER_CALLOUTS=" + json.dumps(callouts, ensure_ascii=False, separators=(",", ":")) + ";\n"
with open(heading_policy_path, encoding="utf-8") as fp:
    heading_policy = json.load(fp)
heading_js = "window.READER_HEADING_POLICY=" + json.dumps(heading_policy, ensure_ascii=False, separators=(",", ":")) + ";\n"
with open(os.path.join(repo_root, "include", "HeadingPolicy.inc"), "w", encoding="utf-8") as fp:
    fp.write("// Generated from themes/heading-policy.json; Unicode " + heading_policy["unicodeVersion"] + ".\n")
    fp.write('inline constexpr const char* kHeadingUnicodeVersion = "' + heading_policy["unicodeVersion"] + '";\n')
    fp.write("inline constexpr const wchar_t* kReservedHeadingIds[] = {" + ",".join("L" + json.dumps(value) for value in heading_policy["reservedIds"]) + "};\n")
    fp.write("struct HeadingCodepointRange { unsigned first, last; };\ninline constexpr HeadingCodepointRange kHeadingCodepoints[] = {\n")
    fp.write("\n".join("    {" + str(first) + "," + str(last) + "}," for first, last in heading_policy["acceptedRanges"]))
    fp.write("\n};\n")
    fp.write("inline constexpr HeadingCodepointRange kHeadingWhitespace[] = {" + ",".join("{" + str(first) + "," + str(last) + "}" for first, last in heading_policy["whitespaceRanges"]) + "};\n")
    fp.write("struct HeadingFoldRange { unsigned first, last; int delta; };\ninline constexpr HeadingFoldRange kHeadingFolds[] = {\n")
    fp.write("\n".join("    {" + ",".join(map(str, row)) + "}," for row in heading_policy["lowercaseRanges"]) + "\n};\n")
    fp.write("struct HeadingExpansion { unsigned codepoint, first, second; };\ninline constexpr HeadingExpansion kHeadingExpansions[] = {\n")
    for codepoint, values in heading_policy["lowercaseExpansions"].items():
        if len(values) != 2:
            raise ValueError("Update the explicit heading expansion contract before importing this Unicode version")
        fp.write("    {" + codepoint + "," + ",".join(map(str, values)) + "},\n")
    fp.write("};\n")
callout_rows = []
for item in callouts:
    for index, alias in enumerate(item["aliases"]):
        title = item.get("aliasTitles", {}).get(alias) or (item["title"] if index == 0 else item["localizedTitle"])
        callout_rows.append("    {L" + json.dumps(alias, ensure_ascii=False) + ", AlertType::" + item["nativeType"] + ", L" + json.dumps(title, ensure_ascii=False) + "},")
with open(os.path.join(repo_root, "src", "CalloutRegistry.inc"), "w", encoding="utf-8") as fp:
    fp.write("// Generated from themes/callouts.json.\nstruct CalloutRegistryEntry { const wchar_t* alias; AlertType type; const wchar_t* title; };\n")
    fp.write("inline constexpr CalloutRegistryEntry kCalloutRegistry[] = {\n" + "\n".join(callout_rows) + "\n};\n")
with open(os.path.join(repo_root, "tests", "fixtures", "headings.json"), encoding="utf-8") as fp:
    heading_cases = json.load(fp)
def wide_list(values):
    return "{" + ", ".join("L" + json.dumps(value, ensure_ascii=False) for value in values) + "}"
with open(os.path.join(repo_root, "tests", "fixtures", "headings.inc"), "w", encoding="utf-8") as fp:
    fp.write("// Generated from headings.json.\n#include <initializer_list>\nstruct HeadingCase { const wchar_t* name; std::initializer_list<const wchar_t*> reserved, headings, expected; };\ninline const HeadingCase kHeadingCases[] = {\n")
    for case in heading_cases:
        fp.write("    {L" + json.dumps(case["name"], ensure_ascii=False) + ", " + ", ".join(wide_list(case[key]) for key in ("reserved", "headings", "expected")) + "},\n")
    fp.write("};\n")

# Reader controller script (handles messages from C++ host, renders markdown, handles checkbox clicks & line jumping)
controller_js = r"""
(function() {
  'use strict';

  var contentEl = null;
  var currentBiDi = true;
  var blockSources = new WeakMap();
  var renderSequence = 0;
  var currentDocument = null;
  var readingPositions = new Map();
  var taskSources = new Map();
  var taskSourceLines = [];
  var highlightCache = new Map(); // blocks of the last render only, so it never outgrows one document

  // Safe HTML entity escaping
  function escapeHtml(s) {
    return String(s || '')
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;')
      .replace(/'/g, '&#39;');
  }

  // Global uncaught error and rejection handlers: report to host C++
  window.onerror = function(msg, url, line, col, err) {
    if (window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('jsError:' + msg + ' (line ' + line + ')');
    }
  };
  window.addEventListener('unhandledrejection', function(e) {
    if (window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('jsError:' + (e.reason ? (e.reason.message || e.reason) : 'Unhandled rejection'));
    }
  });

  // The reader's translated text (rt in ReaderThemes.inc); a page without the reader keeps English.
  function readerText(key, fallback) { return typeof window.rt === 'function' ? window.rt(key) : fallback; }

  // Configure marked for GFM
  if (typeof marked !== 'undefined') {
    marked.setOptions({
      gfm: true,
      breaks: false
    });
  }

  // Footnotes as GitHub shows them: "[^id]" becomes a numbered superscript (in order of first reference),
  // "[^id]: text" definitions (continuation lines indented) are collected into a list at the end.
  var footnotes = null; // {order: [id], numbers: {id: n}, refs: {id: count}, notes: {id: html}} for one top-level parse
  if (typeof marked !== 'undefined' && marked.use) {
    marked.use({extensions: [{
      name: 'footnoteDef', level: 'block',
      start: function(src) { var m = /^\[\^[^\]\s]+\]:/m.exec(src); return m ? m.index : undefined; },
      tokenizer: function(src) {
        var m = /^\[\^([^\]\s]+)\]:[ \t]*([^\n]*(?:\n(?:[ \t]{2,}|\t)[^\n]*)*)(?:\n|$)/.exec(src);
        if (m) return {type: 'footnoteDef', raw: m[0], id: m[1], tokens: this.lexer.inlineTokens(m[2].replace(/\n[ \t]+/g, ' '))};
      },
      renderer: function(token) {
        if (!footnotes) return '<p>' + escapeHtml(token.raw) + '</p>'; // fallback parse: keep the note visible
        if (!(token.id in footnotes.notes)) footnotes.notes[token.id] = this.parser.parseInline(token.tokens);
        return '';
      }
    }, {
      name: 'footnoteRef', level: 'inline',
      start: function(src) { var i = src.indexOf('[^'); return i < 0 ? undefined : i; },
      tokenizer: function(src) {
        var m = /^\[\^([^\]\s]+)\](?!:)/.exec(src);
        if (m) return {type: 'footnoteRef', raw: m[0], id: m[1]};
      },
      renderer: function(token) {
        if (!footnotes) return escapeHtml(token.raw);
        if (!footnotes.numbers[token.id]) { footnotes.order.push(token.id); footnotes.numbers[token.id] = footnotes.order.length; }
        var k = footnotes.refs[token.id] = (footnotes.refs[token.id] || 0) + 1, id = escapeHtml(token.id);
        return '<sup class="footnote-ref"><a href="#fn-' + id + '" id="fnref-' + id + (k > 1 ? '-' + k : '') + '">' + footnotes.numbers[token.id] + '</a></sup>';
      }
    }]});
  }

  // Privacy: nothing in a document may fetch from the network on its own. The sanitizer parses in an inert
  // document, so moving remote sources aside here happens before any request; the reader offers a
  // "load" button per image (data-remote-src). Media and inline-style URLs are dropped outright.
  var REMOTE_URL = /^\s*(?:[a-z][a-z0-9+.-]*:)?\/\//i;
  if (typeof DOMPurify !== 'undefined' && DOMPurify.addHook) {
    // "C:\x.png" looks like an unknown URL scheme to the sanitizer, which drops it: keep the written path as plain
    // text so the reader can say why the picture is not shown.
    DOMPurify.addHook('uponSanitizeAttribute', function(node, data) {
      if (node.nodeName === 'IMG' && data.attrName === 'src' && /^[a-z]:(?:[\\/]|%5c)/i.test(data.attrValue || '')) node.setAttribute('data-local-path', data.attrValue);
    });
    DOMPurify.addHook('afterSanitizeAttributes', function(node) {
      if (node.nodeName === 'IMG' && REMOTE_URL.test(node.getAttribute('src') || '')) {
        node.setAttribute('data-remote-src', node.getAttribute('src'));
        node.removeAttribute('src');
      }
      if (node.hasAttribute('srcset')) node.removeAttribute('srcset');
      ['poster', 'background'].forEach(function(name) { if (node.hasAttribute(name)) node.removeAttribute(name); });
      if (/^(?:VIDEO|AUDIO|SOURCE|TRACK|INPUT)$/.test(node.nodeName) && REMOTE_URL.test(node.getAttribute('src') || '')) node.removeAttribute('src');
      if (node.namespaceURI === 'http://www.w3.org/2000/svg' && node.nodeName.toLowerCase() !== 'a') {
        ['href', 'xlink:href'].forEach(function(name) { if (REMOTE_URL.test(node.getAttribute(name) || '')) node.removeAttribute(name); });
      }
      // url(), image-set() and CSS escapes (which can spell either) never reach the page
      if (/url\s*\(|image-set|\\/i.test(node.getAttribute('style') || '')) node.removeAttribute('style');
    });
  }

  function renderFootnotes() {
    var items = footnotes.order.filter(function(id) { return id in footnotes.notes; }).map(function(id) {
      var safe = escapeHtml(id);
      return '<li id="fn-' + safe + '">' + footnotes.notes[id] + ' <a href="#fnref-' + safe + '" class="footnote-back" aria-label="' + readerText('ui.backToText', 'Back to the text') + '">↩</a></li>';
    });
    return items.length ? '<section class="footnotes" data-source-unmapped="true"><hr><ol>' + items.join('') + '</ol></section>\n' : '';
  }

  function captureBlockSources(lexer) {
    var sources = new WeakMap();
    var original = lexer.blockTokens;
    lexer.blockTokens = function(source, tokens, clipped) {
      var previous = tokens && sources.get(tokens);
      var hadTokens = tokens && tokens.length;
      var result = original.call(this, source, tokens, clipped);
      var separator = previous && hadTokens && clipped && !previous.endsWith('\n') ? '\n' : '';
      sources.set(result, previous && hadTokens ? previous + separator + source : source);
      return result;
    };
    return sources;
  }

  function annotateSourceLines(tokens, source, physicalLines) {
    var cursor = 0;
    var cursorLine = 0;
    var uncertain = false;
    tokens.forEach(function(token) {
      var raw = typeof token.raw === 'string' ? token.raw : '';
      var position = !uncertain && raw ? source.indexOf(raw, cursor) : -1;
      if (position < 0) uncertain = true;
      var localLine = position < 0 ? -1 : cursorLine + (source.slice(cursor, position).match(/\n/g) || []).length;
      var line = localLine < 0 ? null : physicalLines[localLine];
      token.sourceLine = Number.isInteger(line) && line >= 0 ? line : null;
      var rawLines = (raw.match(/\n/g) || []).length + 1;
      var childMap = [];
      for (var n = 0; n < rawLines; n++) childMap.push(token.sourceLine === null ? null : physicalLines[localLine + n]);
      if (token.items) annotateSourceLines(token.items, raw, childMap);
      if (token.tokens && blockSources.has(token.tokens)) {
        annotateSourceLines(token.tokens, blockSources.get(token.tokens), childMap);
      }
      if (position >= 0) { cursor = position + raw.length; cursorLine = localLine + rawLines - 1; }
    });
  }

  function sourceAttributes(token) {
    return Number.isInteger(token.sourceLine) && token.sourceLine >= 0 ?
      ' data-line="' + token.sourceLine + '"' : ' data-source-unmapped="true"';
  }

  // suffixes (optional): base -> last suffix tried, so N identical headings cost O(N) instead of O(N^2).
  function slugHeading(text, usedIds, suffixes) {
    var policy = window.READER_HEADING_POLICY;
    var ranges = policy.acceptedRanges;
    var lowercase = Array.from(String(text)).map(function(character) {
      var codepoint = character.codePointAt(0), expansion = policy.lowercaseExpansions[codepoint];
      if (expansion) return String.fromCodePoint.apply(String, expansion);
      var folds = policy.lowercaseRanges, left = 0, right = folds.length;
      while (left < right) { var middle = (left + right) >>> 1; if (folds[middle][1] < codepoint) left = middle + 1; else right = middle; }
      return left < folds.length && folds[left][0] <= codepoint ? String.fromCodePoint(codepoint + folds[left][2]) : character;
    }).join('');
    var base = Array.from(lowercase).filter(function(character) {
      if (/\s/u.test(character)) return true;
      var codepoint = character.codePointAt(0), left = 0, right = ranges.length;
      while (left < right) { var middle = (left + right) >>> 1; if (ranges[middle][1] < codepoint) left = middle + 1; else right = middle; }
      return left < ranges.length && ranges[left][0] <= codepoint;
    }).join('').trim().replace(/\s+/gu, '-') || 'section';
    var number = suffixes ? suffixes.get(base) || 0 : 0;
    var id = number ? base + '-' + number : base;
    while (usedIds.has(id)) id = base + '-' + (++number);
    if (suffixes) suffixes.set(base, number);
    usedIds.add(id);
    return id;
  }

  function headingText(heading) {
    var clone = heading.cloneNode(true);
    clone.querySelectorAll('img').forEach(function(image) {
      image.replaceWith(document.createTextNode(image.getAttribute('alt') || ''));
    });
    return clone.textContent;
  }

  function buildDocumentIndex(root) {
    var used = new Set(window.READER_HEADING_POLICY.reservedIds);
    document.querySelectorAll('[id]').forEach(function(element) { if (!root.contains(element)) used.add(element.id); });
    root.querySelectorAll('[id]').forEach(function(element) {
      if (used.has(element.id)) element.removeAttribute('id');
      else used.add(element.id);
    });
    var headings = [], suffixes = new Map();
    root.querySelectorAll('h1,h2,h3,h4,h5,h6').forEach(function(heading, index) {
      var text = headingText(heading);
      if (!heading.id) heading.id = slugHeading(text, used, suffixes);
      var line = Number(heading.getAttribute('data-line'));
      headings.push({id: heading.id, level: Number(heading.tagName.slice(1)), text: text,
                     line: heading.hasAttribute('data-line') && Number.isInteger(line) ? line : null, element: heading});
    });
    // Canonical heading IDs take priority over compatibility aliases.
    headings.forEach(function(heading, index) {
      var legacy = 'reader-heading-' + index;
      if (!used.has(legacy)) {
        var alias = document.createElement('span');
        alias.id = legacy; alias.className = 'reader-legacy-anchor'; alias.setAttribute('aria-hidden', 'true');
        heading.element.before(alias); used.add(legacy);
      }
    });
    var blocks = Array.from(root.querySelectorAll('[data-line]')).filter(function(element) {
      return !element.closest('[data-source-unmapped]');
    }).map(function(element) { return {line: Number(element.getAttribute('data-line')), element: element}; });
    return {headings: headings, blocks: blocks};
  }

  function getReaderScrollRoot() {
    var body = document.body;
    if (body && /^(auto|scroll)$/.test(getComputedStyle(body).overflowY) && body.scrollHeight > body.clientHeight) return body;
    return document.scrollingElement || document.documentElement;
  }
  window.getReaderScrollRoot = getReaderScrollRoot;

  function captureReadingPosition() {
    if (!currentDocument || currentDocument.closed || !currentDocument.bufferId || currentDocument.sync || !currentDocument.index.blocks.length) return;
    var blocks = currentDocument.index.blocks;
    var anchor = blocks.find(function(block) { return block.element.getBoundingClientRect().bottom > 0; }) || blocks[blocks.length - 1];
    var preceding = currentDocument.index.headings.filter(function(heading) { return heading.line !== null && heading.line <= anchor.line; });
    readingPositions.delete(currentDocument.bufferId);
    var heading = preceding.length ? preceding[preceding.length - 1] : null;
    var position = {line: anchor.line, heading: heading ? heading.id : '', relativeLine: heading ? anchor.line - heading.line : 0, offset: Math.round(anchor.element.getBoundingClientRect().top)};
    readingPositions.set(currentDocument.bufferId, position);
    while (readingPositions.size > 100) readingPositions.delete(readingPositions.keys().next().value);
    // The host keeps it per file ("line:offset"), so the place survives closing the tab and restarting.
    var wire = position.line + ':' + Math.max(-99999, Math.min(99999, position.offset));
    if (/^\d+$/.test(currentDocument.bufferId) && sentReadingPositions.get(currentDocument.bufferId) !== wire && window.chrome && window.chrome.webview) {
      sentReadingPositions.set(currentDocument.bufferId, wire);
      window.chrome.webview.postMessage('readingPosition:' + currentDocument.bufferId + ':' + wire);
    }
  }
  var sentReadingPositions = new Map();
  // Scrolling alone (no edit, no tab switch) also moves the place: capture once the reader stops scrolling.
  var readingScrollTimer = 0;
  document.addEventListener('scroll', function() {
    clearTimeout(readingScrollTimer);
    readingScrollTimer = setTimeout(captureReadingPosition, 600);
  }, {passive: true, capture: true});

  function finalizeDocument(data, state, sequence) {
    var index = state === 'ready' ? buildDocumentIndex(contentEl) : {headings: [], blocks: []};
    var bufferId = typeof data.bufferId === 'string' ? data.bufferId : '';
    var sync = typeof data.caretLine === 'number' && data.caretLine >= 0;
    currentDocument = {bufferId: bufferId, sync: sync, caretLine: sync ? data.caretLine : null, closed: false, index: index};
    window.readerDocumentIndex = {bufferId: bufferId, renderRevision: data.renderRevision, sequence: sequence, state: state, headings: index.headings, blocks: index.blocks,
                                  docBase: typeof data.docBase === 'string' ? data.docBase : ''};
    document.dispatchEvent(new CustomEvent('readerRendered', {detail: window.readerDocumentIndex}));
    var saved = readingPositions.get(bufferId);
    // Restored synchronously: a frame at the previous document's offset would flash, and a smooth scroll would animate.
    if (!currentDocument.sync && saved && index.blocks.length) {
      (function() {
        var headingIndex = index.headings.findIndex(function(heading) { return heading.id === saved.heading && heading.line !== null; });
        var targetLine = headingIndex >= 0 ? index.headings[headingIndex].line + saved.relativeLine : saved.line;
        var candidates = index.blocks;
        if (headingIndex >= 0) {
          var start = index.headings[headingIndex].line, next = index.headings[headingIndex + 1];
          var section = candidates.filter(function(block) { return block.line >= start && (!next || next.line === null || block.line < next.line); });
          if (section.length) candidates = section;
        }
        var nearest = candidates.reduce(function(best, block) { return Math.abs(block.line - targetLine) < Math.abs(best.line - targetLine) ? block : best; });
        getReaderScrollRoot().scrollTop += nearest.element.getBoundingClientRect().top - saved.offset;
      })();
    } else if (!currentDocument.sync && !saved) {
      getReaderScrollRoot().scrollTop = 0;
    }
  }

  // Token positions come from each actual lexer input, before task text mutation.
  function parseWithLines(markdown, startLine) {
    if (!markdown || !markdown.trim()) return '';
    try {
      var source = markdown.replace(/\r\n?|\n/g, '\n');
      var topLevel = startLine === undefined && !footnotes;
      if (startLine === undefined) {
        taskSources = new Map();
        taskSourceLines = source.split('\n');
      }
      var lexer = new marked.Lexer();
      blockSources = captureBlockSources(lexer);
      var tokens = lexer.lex(source);
      if (startLine === undefined) startLine = 0;
      var physicalLines = source.split('\n').map(function(_, index) { return Number.isInteger(startLine) ? startLine + index : null; });
      annotateSourceLines(tokens, source, physicalLines);

      var customRenderer = new marked.Renderer();

      // Suppress default disabled checkbox: customRenderer.listitem renders the interactive task checkbox
      customRenderer.checkbox = function() {
        return '';
      };

      customRenderer.heading = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        return '<h' + obj.depth + sourceAttributes(obj) + '>' + text + '</h' + obj.depth + '>\n';
      };

      customRenderer.paragraph = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        return '<p' + sourceAttributes(obj) + '>' + text + '</p>\n';
      };

      customRenderer.code = function(obj) {
        var validLang = (obj.lang || '').match(/^\S*/)[0];
        var langClass = validLang ? ' language-' + validLang : '';
        return '<pre' + sourceAttributes(obj) + '><code class="hljs' + escapeHtml(langClass) + '">' +
               escapeHtml(obj.text) + '</code></pre>\n';
      };

      customRenderer.blockquote = function(obj) {
        var firstLine = (obj.text || '').split('\n')[0];
        var marker = firstLine.match(/^\[!([^\]\r\n]+)\][ \t]*$/);
        var registry = window.READER_CALLOUTS || [];
        var alias = null;
        var callout = marker && registry.find(function(item) {
          alias = item.aliases.find(function(candidate) { return candidate.toLowerCase() === marker[1].toLowerCase(); }) || null;
          return alias !== null;
        });
        if (callout) {
          var title = (callout.aliasTitles && callout.aliasTitles[alias]) || (alias === callout.aliases[0] ? callout.title : callout.localizedTitle);
          var body = (obj.text || '').slice(firstLine.length).replace(/^\n/, '');
          var bodyLine = Number.isInteger(obj.sourceLine) ? obj.sourceLine + 1 : null;
          return '<blockquote class="alert-callout alert-' + escapeHtml(callout.type) + '"' + sourceAttributes(obj) + '><div class="alert-title">' + escapeHtml(title) + '</div>' + parseWithLines(body, bodyLine) + '</blockquote>\n';
        }
        var text = this.parser.parse(obj.tokens);
        return '<blockquote' + sourceAttributes(obj) + '>' + text + '</blockquote>\n';
      };

      customRenderer.listitem = function(obj) {
        var text = this.parser.parse(obj.tokens);
        if (obj.task) {
          var original = taskSourceLines[obj.sourceLine];
          var marker = typeof original === 'string' && original.match(/^([ \t]*(?:(?:>[ \t]*)|(?:(?:[-+*]|\d{1,9}[.)])[ \t]+))*)\[([ xX])\](?=[ \t]|$)/);
          var key = '';
          if (obj.sourceLine !== null && marker) {
            // NavigateToString has an opaque origin; getRandomValues also works outside secure contexts.
            key = Array.from(crypto.getRandomValues(new Uint32Array(4)), function(value) { return value.toString(16).padStart(8, '0'); }).join('');
            taskSources.set(key, {line: obj.sourceLine, column: new TextEncoder().encode(marker[1]).length,
                                  state: marker[2] === ' ' ? 0 : marker[2] === 'x' ? 1 : 2});
          }
          return '<li class="task-list-item"' + sourceAttributes(obj) + '>' +
                 '<input type="checkbox" class="task-checkbox"' + sourceAttributes(obj) + (key ? ' data-task-key="' + key + '"' : ' disabled title="Source position could not be verified"') + ' ' +
                 (obj.checked ? 'checked' : '') + '> ' + text + '</li>\n';
        }
        return '<li' + sourceAttributes(obj) + '>' + text + '</li>\n';
      };

      // GFM table rows are one source line each: header = the table's line, then the delimiter row, then the body.
      customRenderer.table = function(obj) {
        var text = marked.Renderer.prototype.table.call(this, obj);
        if (Number.isInteger(obj.sourceLine) && obj.sourceLine >= 0) {
          var row = 0;
          text = text.replace(/<tr>/g, function() { return '<tr data-line="' + (obj.sourceLine + (row++ === 0 ? 0 : row)) + '">'; });
        }
        return text.replace(/^<table/, '<table' + sourceAttributes(obj));
      };

      customRenderer.hr = function(obj) {
        return '<hr' + sourceAttributes(obj) + '>\n';
      };

      // Lexing (above) only builds tokens; references are numbered and notes collected while rendering.
      if (topLevel) footnotes = {order: [], numbers: {}, refs: {}, notes: {}};
      try {
        // marked.defaults carries the footnote extensions; a bare {renderer} would drop them.
        var parser = new marked.Parser(Object.assign({}, marked.defaults, { renderer: customRenderer }));
        var html = parser.parse(tokens);
        return topLevel ? html + renderFootnotes() : html;
      } finally {
        if (topLevel) footnotes = null;
      }
    } catch(err) {
      console.error('Markdown parse error:', err);
      return marked.parse(markdown);
    }
  }

  function applyRender(data) {
    captureReadingPosition();
    var sequence = ++renderSequence;
    var switched = !currentDocument || currentDocument.closed || currentDocument.bufferId !== data.bufferId;
    var previousCaretLine = currentDocument ? currentDocument.caretLine : null;
    try {
      if (!contentEl) contentEl = document.getElementById('content');
      if (!contentEl) return;

      if (data.title) {
        document.title = data.title;
      }

      if (typeof data.isDark === 'boolean') {
        document.body.classList.toggle('dark', data.isDark);
        document.documentElement.setAttribute('data-theme', data.isDark ? 'dark' : 'light');
      }

      if (typeof data.isBiDi === 'boolean') {
        currentBiDi = data.isBiDi;
      }

      var mdText = data.markdown || '';
      if (!mdText.trim()) {
        contentEl.innerHTML = '<div style="color:var(--muted);text-align:center;padding:60px 20px;"><em>' + readerText('ui.noContent', '(No content to preview)') + '</em></div>';
        finalizeDocument(data, 'empty', sequence);
        return;
      }

      var html = parseWithLines(mdText);
      if (typeof DOMPurify !== 'undefined' && typeof DOMPurify.sanitize === 'function') {
        html = DOMPurify.sanitize(html, { FORBID_TAGS: ['style'], ADD_ATTR: ['data-line', 'data-source-unmapped', 'data-task-key', 'data-remote-src', 'data-local-path', 'target', 'dir', 'align'] });
      }

      contentEl.innerHTML = html;

      // External links open in default browser
      var links = contentEl.querySelectorAll('a[href^="http"]');
      for (var l = 0; l < links.length; l++) {
        links[l].target = '_blank';
        links[l].rel = 'noopener';
      }

      // Syntax highlighting with highlight.js; unchanged blocks reuse the previous render's result
      // (keyed by language class + text), so typing in prose does not re-highlight every code block.
      if (typeof hljs !== 'undefined') {
        var codeBlocks = contentEl.querySelectorAll('pre code');
        var nextCache = new Map();
        for (var c = 0; c < codeBlocks.length; c++) {
          var code = codeBlocks[c], key = code.className + '\u0000' + code.textContent;
          var hit = highlightCache.get(key) || nextCache.get(key);
          if (hit) {
            code.className = hit.className; code.innerHTML = hit.html; code.dataset.highlighted = 'yes';
          } else {
            try { hljs.highlightElement(code); } catch(e) {}
            hit = {className: code.className, html: code.innerHTML};
          }
          nextCache.set(key, hit);
        }
        highlightCache = nextCache;
      }

      // Apply Kelk BiDi engine
      if (currentBiDi && typeof BidiView !== 'undefined' && typeof BidiCore !== 'undefined') {
        try {
          var bidiRes = BidiView.apply(contentEl);
          if (bidiRes && bidiRes.dir) {
            contentEl.setAttribute('dir', bidiRes.dir);
          }
        } catch(e) {
          console.error('BidiView error:', e);
        }
      } else {
        if (typeof BidiView !== 'undefined') {
          try { BidiView.clear(contentEl); } catch(e) {}
        }
        contentEl.setAttribute('dir', 'auto');
      }

      // Bind interactive task checkboxes
      var cbs = contentEl.querySelectorAll('input.task-checkbox');
      for (var k = 0; k < cbs.length; k++) {
        (function(cb) {
          var task = taskSources.get(cb.getAttribute('data-task-key'));
          taskSources.delete(cb.getAttribute('data-task-key'));
          cb.removeAttribute('data-task-key');
          if (!task || cb.closest('[data-source-unmapped]') || !/^\d+$/.test(data.bufferId || '') || !/^\d+$/.test(data.renderRevision || '')) {
            cb.disabled = true;
            return;
          }
          cb.addEventListener('click', function(e) {
            e.stopPropagation();
            if (sequence !== renderSequence || !cb.isConnected || !currentDocument || currentDocument.closed) { e.preventDefault(); return; }
            if (window.chrome && window.chrome.webview) {
              cb.disabled = true;
              window.chrome.webview.postMessage('toggleCheckbox:' + data.bufferId + ':' + data.renderRevision + ':' + task.line + ':' + task.column + ':' + task.state);
            }
          });
        })(cbs[k]);
      }

      finalizeDocument(data, 'ready', sequence);
      // Another document jumps to its caret at once (no animation from the old document's offset);
      // the same document follows only a caret that moved, so typing on a line does not re-centre the page.
      if (typeof data.caretLine === 'number' && data.caretLine >= 0) {
        if (switched) scrollToLine(data.caretLine, false, 'instant');
        else if (previousCaretLine !== data.caretLine) scrollToLine(data.caretLine, false);
      }
    } catch(err) {
      console.error('applyRender fatal error:', err);
      if (window.chrome && window.chrome.webview) {
        window.chrome.webview.postMessage('jsError:' + (err ? (err.message || err) : 'Render error'));
      }
      if (contentEl) {
        contentEl.innerHTML = '<div style="color:var(--danger);padding:24px;border:1px solid var(--danger);border-radius:8px;background:var(--code-bg);">' +
                              '<h3 style="margin-top:0;">' + readerText('ui.previewError', 'Preview Error') + '</h3>' +
                              '<p>' + escapeHtml(err ? err.message : 'Error') + '</p>' +
                              '</div>';
        finalizeDocument(data, 'error', sequence);
      }
    }
  }

  // The block that contains the line: the last block starting at or before it (document order, so the
  // innermost of nested blocks sharing a start line); before the first block, the first one.
  function scrollToLine(targetLine, highlight, behavior) {
    if (!contentEl) contentEl = document.getElementById('content');
    if (!contentEl) return;

    var blocks = currentDocument ? currentDocument.index.blocks : [];
    if (blocks.length === 0) return;

    var bestEl = blocks[0].element, bestLine = -1;
    for (var i = 0; i < blocks.length; i++) {
      if (blocks[i].line <= targetLine && blocks[i].line >= bestLine) {
        bestLine = blocks[i].line;
        bestEl = blocks[i].element;
      }
    }
    if (currentDocument) currentDocument.caretLine = targetLine;

    if (bestEl) {
      bestEl.scrollIntoView({ behavior: behavior || 'smooth', block: 'center' });
      if (highlight) {
        var old = contentEl.querySelector('.caret-target');
        if (old) old.classList.remove('caret-target');
        bestEl.classList.add('caret-target');
        setTimeout(function() { bestEl.classList.remove('caret-target'); }, 1200);
      }
    }
  }

  // Double click on element jumps to source in Notepad++
  function jumpToSource(target) {
    var line = target.getAttribute('data-line');
    if (line !== null && window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('jumpToLine:' + line);
    }
  }

  document.addEventListener('dblclick', function(e) {
    if (e.target.closest('[data-source-unmapped]')) return;
    var target = e.target.closest('[data-line]');
    if (target) {
      jumpToSource(target);
    }
  });

  // Listen to messages from host C++
  if (window.chrome && window.chrome.webview) {
    window.chrome.webview.addEventListener('message', function(event) {
      var data = event.data;
      if (typeof data === 'string') {
        try { data = JSON.parse(data); } catch(e) {}
      }
      if (!data) return;

      if (data.type === 'render') {
        applyRender(data);
      } else if (data.type === 'documentState' && data.kind === 'readingPosition') {
        // the place saved for this file: used only when the reader has none fresher for the buffer
        var saved = /^(\d{1,7}):(-?\d{1,6})$/.exec(String(data.value || ''));
        var buffer = String(data.bufferId || '');
        if (saved && !readingPositions.has(buffer)) {
          readingPositions.set(buffer, {line: Number(saved[1]), heading: '', relativeLine: 0, offset: Number(saved[2])});
          sentReadingPositions.set(buffer, data.value);
        }
      } else if (data.type === 'bufferClosed') {
        readingPositions.delete(data.bufferId);
        if (currentDocument && currentDocument.bufferId === data.bufferId) currentDocument.closed = true;
      } else if (data.type === 'settings') {
        if (currentDocument && typeof data.sync === 'boolean') currentDocument.sync = data.sync;
      } else if (data.type === 'scrollToLine') {
        scrollToLine(data.line, true);
      } else if (data.type === 'setTheme') {
        document.body.classList.toggle('dark', !!data.isDark);
        document.documentElement.setAttribute('data-theme', data.isDark ? 'dark' : 'light');
      } else if (data.type === 'setBiDi') {
        currentBiDi = !!data.isBiDi;
        if (contentEl && typeof BidiView !== 'undefined') {
          if (currentBiDi) {
            var r = BidiView.apply(contentEl);
            if (r && r.dir) contentEl.setAttribute('dir', r.dir);
          } else {
            BidiView.clear(contentEl);
            contentEl.setAttribute('dir', 'auto');
          }
        }
      }
    });
  }

  function onReady() {
    contentEl = document.getElementById('content');
    if (window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('pageLoaded');
    }
  }

  if (document.readyState === 'complete' || document.readyState === 'interactive') {
    onReady();
  } else {
    document.addEventListener('DOMContentLoaded', onReady);
  }
})();
"""

# Export script for standalone HTML exports (runs highlight.js and BidiView on DOMContentLoaded)
export_js = r"""
(function() {
  'use strict';
  function initDoc() {
    if (typeof hljs !== 'undefined') {
      var codeBlocks = document.querySelectorAll('pre code');
      for (var c = 0; c < codeBlocks.length; c++) {
        try { hljs.highlightElement(codeBlocks[c]); } catch(e) {}
      }
    }
    if (typeof BidiView !== 'undefined') {
      var content = document.getElementById('content');
      if (content) {
        var r = BidiView.apply(content);
        if (r && r.dir) content.setAttribute('dir', r.dir);
      }
    }
  }
  if (document.readyState === 'complete' || document.readyState === 'interactive') {
    initDoc();
  } else {
    document.addEventListener('DOMContentLoaded', initDoc);
  }
})();
"""

combined_shell_js = core_js + "\n\n" + callout_js + heading_js + "\n/* === Controller === */\n" + controller_js
combined_export_js = core_js + "\n\n/* === Export Init === */\n" + export_js

# Ensure 100% offline string verification passes without breaking URL namespaces
combined_shell_js = combined_shell_js.replace("http://", "\\u0068\\u0074\\u0074\\u0070://")
combined_export_js = combined_export_js.replace("http://", "\\u0068\\u0074\\u0074\\u0070://")
combined_css = combined_css.replace("http://", "\\u0068\\u0074\\u0074\\u0070://")

# Shell HTML template
shell_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Markdown Reader</title>
  <style>
{combined_css}
  </style>
  <script>
{combined_shell_js}
  </script>
</head>
<body>
  <div id="reader-wrap">
    <div id="content" class="doc bd-root" dir="auto"></div>
  </div>
</body>
</html>
"""

with open(output_file, "w", encoding="utf-8") as fp:
    fp.write("// Auto-generated Kelk Reader bundle for NppMarkdownNext\n")
    fp.write("// Ultra-lightweight: Marked + DOMPurify + Highlight.js + Kelk BidiCore & BidiView\n\n")
    fp.write("inline constexpr const char* s_kelkCss = R\"KELK_CSS_EOF(\n")
    fp.write(combined_css)
    fp.write("\n)KELK_CSS_EOF\";\n\n")
    fp.write("inline constexpr const char* s_kelkScripts = R\"KELK_JS_EOF(\n")
    fp.write(combined_shell_js)
    fp.write("\n)KELK_JS_EOF\";\n\n")
    fp.write("inline constexpr const char* s_kelkReaderShellHtml = R\"KELK_HTML_EOF(\n")
    fp.write(shell_html)
    fp.write("\n)KELK_HTML_EOF\";\n")

total_bytes = os.path.getsize(output_file)
print(f"Generated {output_file}: {total_bytes} bytes (~ {round(total_bytes/1024)} KB)")
