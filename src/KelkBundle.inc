// Auto-generated Kelk Reader bundle for NppMarkdownNext
// Ultra-lightweight: Marked + DOMPurify + Highlight.js + Kelk BidiCore & BidiView

inline constexpr const char* s_kelkCss = R"KELK_CSS_EOF(
/* Kelk — design tokens: colors (light / dark), fonts, radii, spacing.
   Two font variables drive the whole page:
     --font-ui       interface (buttons, labels, settings)
     --font-content  the document: editor + preview (set from Settings → Typography)
   --font-mono is the code font of the preview (and of the exports). */











:root {
  --font-ui: 'Vazirmatn', system-ui, sans-serif;
  --font-content: 'Sahel', 'Vazirmatn', system-ui, sans-serif;
  --font-latin: 'Vazirmatn', system-ui, sans-serif;
  --font-mono: 'Vazir Code Hack', 'DejaVu Sans Mono', ui-monospace, Consolas, monospace;

  --bg: #f7f6f3;
  --panel: #ffffff;
  --panel-2: #fcfbf9;
  --ink: #1c1b1a;
  --muted: #6b6862;
  --placeholder: #b6b1a8;
  --line: #e6e3dd;
  --line-strong: #d4d0c8;
  --accent: #1f6f5c;
  --accent-soft: #e7f1ee;
  --accent-ink: #114b3e;
  --on-accent: #ffffff;
  --danger: #b42318;
  --warn: #b45309;
  --code-bg: #f4f2ee;
  --code-head: #ebe8e2;
  --quote-bar: #c9a227;
  --table-head: #e2efd9;
  --toast-bg: #114b3e;
  --toast-ink: #ffffff;

  --radius: 10px;
  --radius-sm: 6px;
  --shadow: 0 1px 2px rgba(28, 27, 26, .04), 0 4px 16px rgba(28, 27, 26, .05);
  --gap: 14px;
  --pane-w: 340px;
  --rail-w: 44px;
}

[data-theme="dark"] {
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
  --danger: #f97066;
  --warn: #f0b429;
  --code-bg: #25292d;
  --code-head: #2d3136;
  --quote-bar: #d4b43f;
  --table-head: #24382c;
  --toast-bg: #4db396;
  --toast-ink: #0f1416;
  --shadow: 0 1px 2px rgba(0, 0, 0, .25), 0 4px 16px rgba(0, 0, 0, .3);
}


/* Kelk — the preview pane.
   The document's own look comes from HtmlBuilder's stylesheet (lib/PreviewBuilder → <style id="kelk-doc-css">),
   the same one the .html export embeds. This file adds only what belongs to the pane: spacing, the
   empty hint, the copy buttons, the source view, and the dark theme of the app. */

.doc {
  padding: 18px 22px 28px;
  color: var(--ink);
  overflow-wrap: anywhere;
}
.doc:empty::before { content: attr(data-empty); color: var(--placeholder); font-family: var(--font-ui); }
.doc > :first-child { margin-top: 0; }

/* wide tables scroll inside the pane */
.doc table:not(.kelk-fit) { display: block; overflow-x: auto; }

/* code frame bar: the language on one side, the copy button on the other */
.doc .code-lang { display: flex; align-items: center; justify-content: space-between; gap: 8px; min-height: 1.6em; }
.code-copy-btn {
  display: inline-flex; align-items: center; gap: 4px; border: 0; background: transparent; color: inherit; cursor: pointer;
  font-size: 12px; padding: 1px 6px; border-radius: 4px; font-family: var(--font-ui); margin-inline-start: auto;
}
.code-copy-btn:hover { background: var(--accent-soft); color: var(--accent-ink); }
.code-copy-btn svg { width: 14px; height: 14px; }
.code-copy-btn.copied { color: var(--accent); }
.doc .code-nested { position: relative; }
.code-nested-copy { position: absolute; top: 2px; right: 4px; padding: 1px 3px; opacity: .55; }
.code-nested:hover > .code-nested-copy { opacity: 1; }

/* Markdown shown as source (reverse view) */
.doc.source { font-family: var(--font-mono); font-size: 13px; white-space: pre-wrap; direction: ltr; text-align: left; unicode-bidi: plaintext; }

/* dark theme of the app. The document's own colors (tables, code, quotes, ToC) are translated
   for the dark page by scripts/preview.js (darkPaper) — the exports keep the chosen colors; here,
   only what the document stylesheet does not color: text on the page, code text and syntax colors. */
[data-theme="dark"] .doc h1, [data-theme="dark"] .doc h2, [data-theme="dark"] .doc h3,
[data-theme="dark"] .doc h4, [data-theme="dark"] .doc h5, [data-theme="dark"] .doc h6 { color: var(--accent-ink); }
[data-theme="dark"] .doc :not(pre) > code { background: var(--code-bg); color: var(--ink); }
[data-theme="dark"] .doc a { color: var(--accent-ink); }
[data-theme="dark"] .doc .code, [data-theme="dark"] .doc .code code.hljs { color: var(--ink); }
/* syntax colors for a dark box (GitHub dark) */
[data-theme="dark"] .doc .code .hljs-keyword, [data-theme="dark"] .doc .code .hljs-doctag, [data-theme="dark"] .doc .code .hljs-type,
[data-theme="dark"] .doc .code .hljs-meta .hljs-keyword, [data-theme="dark"] .doc .code .hljs-template-tag { color: #ff7b72; }
[data-theme="dark"] .doc .code .hljs-string, [data-theme="dark"] .doc .code .hljs-regexp, [data-theme="dark"] .doc .code .hljs-meta .hljs-string { color: #a5d6ff; }
[data-theme="dark"] .doc .code .hljs-title, [data-theme="dark"] .doc .code .hljs-title.function_, [data-theme="dark"] .doc .code .hljs-title.class_ { color: #d2a8ff; }
[data-theme="dark"] .doc .code .hljs-number, [data-theme="dark"] .doc .code .hljs-literal, [data-theme="dark"] .doc .code .hljs-attr,
[data-theme="dark"] .doc .code .hljs-attribute, [data-theme="dark"] .doc .code .hljs-variable, [data-theme="dark"] .doc .code .hljs-built_in,
[data-theme="dark"] .doc .code .hljs-selector-class, [data-theme="dark"] .doc .code .hljs-operator { color: #79c0ff; }
[data-theme="dark"] .doc .code .hljs-comment, [data-theme="dark"] .doc .code .hljs-quote { color: #8b949e; }
[data-theme="dark"] .doc .code .hljs-name, [data-theme="dark"] .doc .code .hljs-selector-tag, [data-theme="dark"] .doc .code .hljs-section { color: #7ee787; }
[data-theme="dark"] .doc .code .hljs-subst, [data-theme="dark"] .doc .code .hljs-params { color: var(--ink); }

/* highlight.js's page theme adds padding and a background to code.hljs: the frame has its own */
.doc pre code.hljs { padding: 0; background: none; }


pre code.hljs{display:block;overflow-x:auto;padding:1em}code.hljs{padding:3px 5px}/*!
  Theme: GitHub
  Description: Light theme as seen on github.com
  Author: github.com
  Maintainer: @Hirse
  Updated: 2021-05-15

  Outdated base version: https://github.com/primer/github-syntax-light
  Current colors taken from GitHub's CSS
*/.hljs{color:#24292e;background:#fff}.hljs-doctag,.hljs-keyword,.hljs-meta .hljs-keyword,.hljs-template-tag,.hljs-template-variable,.hljs-type,.hljs-variable.language_{color:#d73a49}.hljs-title,.hljs-title.class_,.hljs-title.class_.inherited__,.hljs-title.function_{color:#6f42c1}.hljs-attr,.hljs-attribute,.hljs-literal,.hljs-meta,.hljs-number,.hljs-operator,.hljs-selector-attr,.hljs-selector-class,.hljs-selector-id,.hljs-variable{color:#005cc5}.hljs-meta .hljs-string,.hljs-regexp,.hljs-string{color:#032f62}.hljs-built_in,.hljs-symbol{color:#e36209}.hljs-code,.hljs-comment,.hljs-formula{color:#6a737d}.hljs-name,.hljs-quote,.hljs-selector-pseudo,.hljs-selector-tag{color:#22863a}.hljs-subst{color:#24292e}.hljs-section{color:#005cc5;font-weight:700}.hljs-bullet{color:#735c0f}.hljs-emphasis{color:#24292e;font-style:italic}.hljs-strong{color:#24292e;font-weight:700}.hljs-addition{color:#22863a;background-color:#f0fff4}.hljs-deletion{color:#b31d28;background-color:#ffeef0}


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
  text-align: start;
}
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

)KELK_CSS_EOF";

inline constexpr const char* s_kelkScripts = R"KELK_JS_EOF(
/* === marked.umd.min_18.0.14.js === */
/**
 * Minified by jsDelivr using Terser v5.48.0.
 * Original file: /npm/marked@18.0.14/lib/marked.umd.js
 *
 * Do NOT use SRI with dynamically generated files! More information: https://www.jsdelivr.com/using-sri-with-dynamic-files
 */
!function(e,t){"object"==typeof exports&&typeof module<"u"?module.exports=t():"function"==typeof define&&define.amd?define("marked",t):e.marked=t()}(typeof globalThis<"u"?globalThis:typeof self<"u"?self:this,function(){var e,t={},n=t,r={exports:t},s=Object.defineProperty,l=Object.getOwnPropertyDescriptor,i=Object.getOwnPropertyNames,a=Object.prototype.hasOwnProperty,o={};function c(){return{async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null}}((e,t)=>{for(var n in t)s(e,n,{get:t[n],enumerable:!0})})(o,{Hooks:()=>De,Lexer:()=>Be,Marked:()=>Oe,Parser:()=>ve,Renderer:()=>qe,TextRenderer:()=>Ze,Tokenizer:()=>Ce,defaults:()=>p,getDefaults:()=>c,lexer:()=>We,marked:()=>Qe,options:()=>Ne,parse:()=>Ue,parseInline:()=>Xe,parser:()=>Fe,setOptions:()=>He,use:()=>Me,walkTokens:()=>Ge}),r.exports=(e=o,((e,t,n,r)=>{if(t&&"object"==typeof t||"function"==typeof t)for(let o of i(t))!a.call(e,o)&&o!==n&&s(e,o,{get:()=>t[o],enumerable:!(r=l(t,o))||r.enumerable});return e})(s({},"__esModule",{value:!0}),e));var p={async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null};function h(e){p=e}var u={exec:()=>null};function g(e){let t=[];return n=>{let r=Math.max(0,Math.min(3,n-1)),s=t[r];return s||(s=e(r),t[r]=s),s}}function k(e,t=""){let n="string"==typeof e?e:e.source,r={replace:(e,t)=>{let s="string"==typeof t?t:t.source;return s=s.replace(d.caret,"$1"),n=n.replace(e,s),r},getRegex:()=>new RegExp(n,t)};return r}var f=((e="")=>{try{return!!new RegExp("(?<=1)(?<!1)"+e)}catch{return!1}})(),d={codeRemoveIndent:/^(?: {0,3}\t| {1,4})/gm,outputLinkReplace:/\\([\[\]])/g,indentCodeCompensation:/^(\s+)(?:```)/,beginningSpace:/^\s+/,endingHash:/#$/,startingSpaceChar:/^ /,endingSpaceChar:/ $/,endingSpaceTabChar:/[ \t]$/,nonSpaceChar:/[^ ]/,newLineCharGlobal:/\n/g,tabCharGlobal:/\t/g,leadingSpaceTab:/^[ \t]+/,multipleSpaceGlobal:/\s+/g,blankLine:/^[ \t]*$/,doubleBlankLine:/\n[ \t]*\n[ \t]*$/,blockquoteStart:/^ {0,3}>/,blockquoteSetextReplace:/\n {0,3}((?:=+|-+) *)(?=\n|$)/g,blockquoteSetextReplace2:/^ {0,3}>[ \t]?/gm,listReplaceNesting:/^ {1,4}(?=( {4})*[^ ])/g,listIsTask:/^\[[ xX]\] +\S/,listReplaceTask:/^\[[ xX]\] +/,listTaskCheckbox:/\[[ xX]\]/,anyLine:/\n.*\n/,hrefBrackets:/^<(.*)>$/,tableDelimiter:/[:|]/,tableAlignChars:/^\||\| *$/g,tableRowBlankLine:/\n[ \t]*$/,tableAlignRight:/^ *-+: *$/,tableAlignCenter:/^ *:-+: *$/,tableAlignLeft:/^ *:-+ *$/,startATag:/^<a /i,endATag:/^<\/a>/i,startPreScriptTag:/^<(pre|code|kbd|script)(\s|>)/i,endPreScriptTag:/^<\/(pre|code|kbd|script)(\s|>)/i,startAngleBracket:/^</,endAngleBracket:/>$/,pedanticHrefTitle:/^([^'"]*[^\s])\s+(['"])(.*)\2/,unicodeAlphaNumeric:/[\p{L}\p{N}]/u,numericCharacterReference:/&#(?:(\d{1,7})|[Xx]([A-Fa-f0-9]{1,6}));/g,escapeTest:/[&<>"']/,escapeReplace:/[&<>"']/g,escapeTestNoEncode:/[<>"']|&(?!(#\d{1,7}|#[Xx][a-fA-F0-9]{1,6}|\w+);)/,escapeReplaceNoEncode:/[<>"']|&(?!(#\d{1,7}|#[Xx][a-fA-F0-9]{1,6}|\w+);)/g,caret:/(^|[^\[])\^/g,percentDecode:/%25/g,findPipe:/\|/g,splitPipe:/ \|/,slashPipe:/\\\|/g,carriageReturn:/\r\n|\r/g,spaceLine:/^ +$/gm,notSpaceStart:/^\S*/,endingNewline:/\n$/,listItemRegex:e=>new RegExp(`^( {0,3}${e})((?:[\t ][^\\n]*)?(?:\\n|$))`),nextBulletRegex:g(e=>new RegExp(`^ {0,${e}}(?:[*+-]|\\d{1,9}[.)])((?:[ \t][^\\n]*)?(?:\\n|$))`)),hrRegex:g(e=>new RegExp(`^ {0,${e}}((?:-[ \t]*){3,}|(?:_[ \t]*){3,}|(?:\\*[ \t]*){3,})(?:\\n+|$)`)),fencesBeginRegex:g(e=>new RegExp(`^ {0,${e}}(?:\`\`\`|~~~)`)),headingBeginRegex:g(e=>new RegExp(`^ {0,${e}}#`)),htmlBeginRegex:g(e=>new RegExp(`^ {0,${e}}(?:</?(?:${z})(?: +|$|/?>)|<(?:script|pre|style|textarea|!--))`,"i")),blockquoteBeginRegex:g(e=>new RegExp(`^ {0,${e}}>`))},x=/^ {0,3}((?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/,b=/ {0,3}(?:[*+-]|\d{1,9}[.)])/,m=/^(?!bull |blockCode|fences|blockquote|heading|html|table)((?:.|\n(?!\s*?\n|bull |fences|blockquote|heading|hr|html|table))+?)\n {0,3}(=+|-+) *(?:\n+|$)/,w=k(m).replace(/bull/g,b).replace(/blockCode/g,/(?: {4}| {0,3}\t)/).replace(/fences/g,/ {0,3}(?:`{3,}|~{3,})/).replace(/blockquote/g,/ {0,3}>/).replace(/heading/g,/ {0,3}#{1,6}(?:\s|$)/).replace(/hr/g,/ {0,3}(?:(?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/).replace(/html/g,/ {0,3}<[^\n>]+>\n/).replace(/\|table/g,"").getRegex(),y=k(m).replace(/bull/g,b).replace(/blockCode/g,/(?: {4}| {0,3}\t)/).replace(/fences/g,/ {0,3}(?:`{3,}|~{3,})/).replace(/blockquote/g,/ {0,3}>/).replace(/heading/g,/ {0,3}#{1,6}(?:\s|$)/).replace(/hr/g,/ {0,3}(?:(?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/).replace(/html/g,/ {0,3}<[^\n>]+>\n/).replace(/table/g,/ {0,3}\|?(?:[:\- ]*\|)+[\:\- ]*\n/).getRegex(),S=/^([^\n]+(?:\n(?!hr|heading|lheading|blockquote|fences|list|html|table|[ \t]+\n)[^\n]+)*)/,$=/(?!\s*\])(?:\\[\s\S]|[^\[\]\\])+/,R=k(/^ {0,3}\[(label)\]: *(?:\n[ \t]*)?([^<\s][^\s]*|<.*?>)(?:(?: +(?:\n[ \t]*)?| *\n[ \t]*)(title))? *(?:\n+|$)/).replace("label",$).replace("title",/(?:"(?:\\"?|[^"\\])*"|'[^'\n]*(?:\n[^'\n]+)*\n?'|\([^()]*\))/).getRegex(),T=k(/^(bull)([ \t][^\n]*?)?(?:\n|$)/).replace(/bull/g,b).getRegex(),z="address|article|aside|base|basefont|blockquote|body|caption|center|col|colgroup|dd|details|dialog|dir|div|dl|dt|fieldset|figcaption|figure|footer|form|frame|frameset|h[1-6]|head|header|hr|html|iframe|legend|li|link|main|menu|menuitem|meta|nav|noframes|ol|optgroup|option|p|param|search|section|summary|table|tbody|td|tfoot|th|thead|title|tr|track|ul",_=/<!--(?:-?>|[\s\S]*?(?:-->|$))/,A=k("^ {0,3}(?:<(script|pre|style|textarea)[\\s>][\\s\\S]*?(?:</\\1>[^\\n]*\\n*|$)|comment[^\\n]*(\\n+|$)|<\\?[\\s\\S]*?(?:\\?>[^\\n]*\\n*|$)|<![A-Z][\\s\\S]*?(?:>[^\\n]*\\n*|$)|<!\\[CDATA\\[[\\s\\S]*?(?:\\]\\]>[^\\n]*\\n*|$)|</?(tag)(?: +|\\n|/?>)[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$)|<(?!script|pre|style|textarea)([a-z][a-z0-9-]*)(?:attribute)*? */?>(?=[ \\t]*(?:\\n|$))[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$)|</(?!script|pre|style|textarea)[a-z][a-z0-9-]*\\s*>(?=[ \\t]*(?:\\n|$))[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$))","i").replace("comment",_).replace("tag",z).replace("attribute",/ +[a-zA-Z:_][\w.:-]*(?: *= *"[^"\n]*"| *= *'[^'\n]*'| *= *[^\s"'=<>`]+)?/).getRegex(),P=e=>k(S).replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("|lheading","").replace("|table","").replace("blockquote"," {0,3}>").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list",e).replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex(),L=P(/ {0,3}(?:[*+-]|1[.)])[ \t]+[^ \t\n]/),I=P(/ {0,3}(?:[*+-]|\d{1,9}[.)])(?:[ \t]|\n|$)/),E={blockquote:k(/^( {0,3}> ?(paragraph|[^\n]*)(?:\n|$))+/).replace("paragraph",I).getRegex(),code:/^((?: {4}| {0,3}\t)[^\n]+(?:\n(?:[ \t]*(?:\n|$))*)?)+/,def:R,fences:/^ {0,3}(`{3,}(?=[^`\n]*(?:\n|$))|~{3,})([^\n]*)(?:\n|$)(?:|([\s\S]*?)(?:\n|$))(?: {0,3}\1[~`]* *(?=\n|$)|$)/,heading:/^ {0,3}(#{1,6})(?=\s|$)(.*)(?:\n+|$)/,hr:x,html:A,lheading:w,list:T,newline:/^(?:[ \t]*(?:\n|$))+/,paragraph:L,table:u,text:/^[^\n]+/},C=k("^ *([^\\n ].*)\\n {0,3}((?:\\| *)?:?-+:? *(?:\\| *:?-+:? *)*(?:\\| *)?)(?:\\n((?:(?! *\\n|hr|heading|blockquote|code|fences|list|html).*(?:\\n|$))*)\\n*|$)").replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("blockquote"," {0,3}>").replace("code","(?: {4}| {0,3}\t)[^\\n]").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list"," {0,3}(?:[*+-]|1[.)])[ \\t]").replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex(),B={...E,lheading:y,table:C,paragraph:k(S).replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("|lheading","").replace("table",C).replace("blockquote"," {0,3}>").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list"," {0,3}(?:[*+-]|1[.)])[ \\t]+[^ \\t\\n]").replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex()},q={...E,html:k("^ *(?:comment *(?:\\n|\\s*$)|<(tag)[\\s\\S]+?</\\1> *(?:\\n{2,}|\\s*$)|<tag(?:\"[^\"]*\"|'[^']*'|\\s[^'\"/>\\s]*)*?/?> *(?:\\n{2,}|\\s*$))").replace("comment",_).replace(/tag/g,"(?!(?:a|em|strong|small|s|cite|q|dfn|abbr|data|time|code|var|samp|kbd|sub|sup|i|b|u|mark|ruby|rt|rp|bdi|bdo|span|br|wbr|ins|del|img)\\b)\\w+(?!:|[^\\w\\s@]*@)\\b").getRegex(),def:/^ *\[([^\]]+)\]: *<?([^\s>]+)>?(?: +(["(][^\n]+[")]))? *(?:\n+|$)/,heading:/^(#{1,6})(.*)(?:\n+|$)/,fences:u,lheading:/^(.+?)\n {0,3}(=+|-+) *(?:\n+|$)/,paragraph:k(S).replace("hr",x).replace("heading"," *#{1,6} *[^\n]").replace("lheading",w).replace("|table","").replace("blockquote"," {0,3}>").replace("|fences","").replace("|list","").replace("|html","").replace("|tag","").getRegex()},Z=/^( {2,}|\\)\n(?!\s*$)[ \t]*/,v=/[\p{P}\p{S}]/u,D=/[\s\p{P}\p{S}]/u,O=/[^\s\p{P}\p{S}]/u,j=k(/^((?![*_])punctSpace)/,"u").replace(/punctSpace/g,D).getRegex(),Q=/(?!~)[\p{P}\p{S}]/u,M=k(/link|precode-code|html/,"g").replace("link",/\[(?:[^\[\]`]|(?<a>`+)[^`]+\k<a>(?!`))*?\]\((?:\\[\s\S]|[^\\\(\)]|\((?:\\[\s\S]|[^\\\(\)])*\))*\)/).replace("precode-",f?"(?<!`)()":"(^^|[^`])").replace("code",/(?<b>`+)[^`]+\k<b>(?!`)/).replace("html",/<(?! )[^<>]*?>/).getRegex(),N=/^(?:\*+(?:((?!\*)punct)|([^\s*]))?)|^_+(?:((?!_)punct)|([^\s_]))?/,H=k(N,"u").replace(/punct/g,v).getRegex(),G=k(N,"u").replace(/punct/g,Q).getRegex(),X=k(/^(?:\*+(?:((?!\*)(?!openQuote)punct)|([^\s*]))?)|^_+(?:((?!_)(?!openQuote)punct)|([^\s_]))?/,"u").replace(/openQuote/g,/[\p{Pi}\p{Ps}"']/u).replace(/punct/g,v).getRegex(),U="^[^_*]*?__[^_*]*?\\*[^_*]*?(?=__)|[^*]+(?=[^*])|(?!\\*)punct(\\*+)(?=[\\s]|$)|notPunctSpace(\\*+)(?!\\*)(?=punctSpace|$)|(?!\\*)punctSpace(\\*+)(?=notPunctSpace)|[\\s](\\*+)(?!\\*)(?=punct)|(?!\\*)punct(\\*+)(?!\\*)(?=punct)|notPunctSpace(\\*+)(?=notPunctSpace)",F=k(U,"gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),W=k(U,"gu").replace(/notPunctSpace/g,/(?:[^\s\p{P}\p{S}]|~)/u).replace(/punctSpace/g,/(?!~)[\s\p{P}\p{S}]/u).replace(/punct/g,Q).getRegex(),J=k("^[^_*]*?__[^_*]*?\\*[^_*]*?(?=__)|[^*]+(?=[^*])|(?!\\*)punct(\\*+)(?=[\\s]|$)|notPunctSpace(\\*+)(?!\\*)(?=punctSpace|$)|(?!\\*)[\\s](\\*+)(?=notPunctSpace)|[\\s](\\*+)(?!\\*)(?=punct)|(?!\\*)punct(\\*+)(?!\\*)(?=punct)|(?:(?!\\*)punct|notPunctSpace)(\\*+)(?!\\*)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),K=k("^[^_*]*?\\*\\*[^_*]*?_[^_*]*?(?=\\*\\*)|[^_]+(?=[^_])|(?!_)punct(_+)(?=[\\s]|$)|notPunctSpace(_+)(?!_)(?=punctSpace|$)|(?!_)punctSpace(_+)(?=notPunctSpace)|[\\s](_+)(?!_)(?=punct)|(?!_)punct(_+)(?!_)(?=punct)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),V=k("^[^_*]*?\\*\\*[^_*]*?_[^_*]*?(?=\\*\\*)|[^_]+(?=[^_])|(?!_)punct(_+)(?=[\\s]|$)|notPunctSpace(_+)(?!_)(?=punctSpace|$)|(?!_)[\\s](_+)(?=notPunctSpace)|[\\s](_+)(?!_)(?=punct)|(?!_)punct(_+)(?!_)(?=punct)|(?:(?!_)punct|notPunctSpace)(_+)(?!_)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),Y=k(/^~~?(?:((?!~)punct)|[^\s~])/,"u").replace(/punct/g,v).getRegex(),ee=k("^[^~]+(?=[^~])|(?!~)punct(~~?)(?=[\\s]|$)|notPunctSpace(~~?)(?!~)(?=punctSpace|$)|(?!~)punctSpace(~~?)(?=notPunctSpace)|[\\s](~~?)(?!~)(?=punct)|(?!~)punct(~~?)(?!~)(?=punct)|notPunctSpace(~~?)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),te=k(/\\(punct)/,"gu").replace(/punct/g,v).getRegex(),ne=k(/^<(scheme:[^\s\x00-\x1f<>]*|email)>/).replace("scheme",/[a-zA-Z][a-zA-Z0-9+.-]{1,31}/).replace("email",/[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+(@)[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)+(?![-_])/).getRegex(),re=k(_).replace("(?:--\x3e|$)","--\x3e").getRegex(),se=k("^comment|^</[a-zA-Z][a-zA-Z0-9-]*\\s*>|^<[a-zA-Z][a-zA-Z0-9-]*(?:attribute)*?\\s*/?>|^<\\?[\\s\\S]*?\\?>|^<![a-zA-Z]+\\s[\\s\\S]*?>|^<!\\[CDATA\\[[\\s\\S]*?\\]\\]>").replace("comment",re).replace("attribute",/\s+[a-zA-Z:_][\w.:-]*(?:\s*=\s*"[^"]*"|\s*=\s*'[^']*'|\s*=\s*[^\s"'=<>`]+)?/).getRegex(),le=/\[(?:\\[\s\S]|[^\[\]\\])*\]/,ie=k(/(?:\[(?:brackets|\\[\s\S]|[^\[\]\\])*\]|\\[\s\S]|`+(?!`)[^`]*?`+(?!`)|``+(?=\])|[^\[\]\\`])*?/).replace("brackets",le).getRegex(),ae=k(/^!?\[(label)\]\(\s*(href)(?:(?:[ \t]+(?:\n[ \t]*)?|\n[ \t]*)(title))?\s*\)/).replace("label",ie).replace("href",/<(?:\\.|[^\n<>\\])+>|[^ \t\n\x00-\x1f]+|(?=\))/).replace("title",/"(?:\\"?|[^"\\])*"|'(?:\\'?|[^'\\])*'|\((?:\\\)?|[^)\\])*\)/).getRegex(),oe=k(/^!?\[(label)\]\[(ref)\]/).replace("label",ie).replace("ref",$).getRegex(),ce=k(/^!?\[(ref)\](?:\[\])?/).replace("ref",$).getRegex(),pe=/(?!\s*\])(?:\\[\s\S]|[^\[\]\\]){1,999}/,he=k(/(?:[^\[\]\\`]*(?:\[(?:brackets|\\[\s\S]|[^\[\]\\])*\]|\\[\s\S]|`+(?!`)[^`]*?`+(?!`)|``+(?=\]))){0,999}?[^\[\]\\`]*?/).replace("brackets",le).getRegex(),ue=k("reflink|nolink(?!\\()","g").replace("reflink",k(/^!?\[(label)\]\[(ref)\]/).replace("label",he).replace("ref",pe).getRegex()).replace("nolink",k(/^!?\[(ref)\](?:\[\])?/).replace("ref",pe).getRegex()).getRegex(),ge=/[hH][tT][tT][pP][sS]?|[fF][tT][pP]/,ke=k(/(?:mailto:email|xmpp:email(?:\/[A-Za-z0-9@.]+)?)/).replace(/email/g,/[A-Za-z0-9._+-]+@[a-zA-Z0-9-_]+(?:\.[a-zA-Z0-9-_]*[a-zA-Z0-9])+(?![\w-])/).getRegex(),fe={_backpedal:u,anyPunctuation:te,autolink:ne,blockSkip:M,br:Z,code:/^(`+)([^`]|[^`][\s\S]*?[^`])\1(?!`)/,del:u,delLDelim:u,delRDelim:u,emStrongLDelim:H,emStrongRDelimAst:F,emStrongRDelimUnd:K,escape:/^\\([!"#$%&'()*+,\-./:;<=>?@\[\]\\^_`{|}~])/,link:ae,nolink:ce,punctuation:j,reflink:oe,reflinkSearch:ue,tag:se,text:/^(`+|[^`])(?:(?= {2,}\n)|[\s\S]*?(?:(?=[\\<!\[`*_]|\b_|$)|[^ ](?= {2,}\n)))/,url:u},de={...fe,emStrongLDelim:X,emStrongRDelimAst:J,emStrongRDelimUnd:V,link:k(/^!?\[(label)\]\((.*?)\)/).replace("label",ie).getRegex(),reflink:k(/^!?\[(label)\]\s*\[([^\]]*)\]/).replace("label",ie).getRegex()},xe={...fe,emStrongRDelimAst:W,emStrongLDelim:G,delLDelim:Y,delRDelim:ee,url:k(/^emailProtocol|^((?:protocol):\/\/|www\.)(?:[a-zA-Z0-9\-]+\.?)+[^\s<]*|^email/).replace("emailProtocol",ke).replace("protocol",ge).replace("email",/[A-Za-z0-9._+-]+(@)[a-zA-Z0-9-_]+(?:\.[a-zA-Z0-9-_]*[a-zA-Z0-9])+(?![\w-])/).getRegex(),_backpedal:/(?:[^?!.,:;*_'"~()&]+|\([^)]*\)|&(?![a-zA-Z0-9]+;$)|[?!.,:;*_'"~)]+(?!$))+/,del:/^(~~?)(?=[^\s~])((?:\\[\s\S]|[^\\])*?(?:\\[\s\S]|[^\s~\\]))\1(?=[^~]|$)/,text:k(/^(?:[^a-zA-Z0-9](?=emailProtocol)|(`+|~+|[^`~])(?:(?=[`~])|(?= {2,}\n)|(?=[a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-]+@)|[\s\S]*?(?:(?=[\\<!\[`*~_]|\b_|protocol:\/\/|www\.|$)|[^ ](?= {2,}\n)|[^a-zA-Z0-9](?=emailProtocol)|[^a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-](?=[a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-]+@))))/).replace("protocol",ge).replace(/emailProtocol/g,/(?:mailto|xmpp):/).getRegex()},be={...xe,br:k(Z).replace("{2,}","*").getRegex(),text:k(xe.text).replace("\\b_","\\b_| {2,}\\n").replace(/\{2,\}/g,"*").getRegex()},me={normal:E,gfm:B,pedantic:q},we={normal:fe,gfm:xe,breaks:be,pedantic:de},ye={"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"},Se=e=>ye[e];function $e(e,t){if(t){if(d.escapeTest.test(e))return e.replace(d.escapeReplace,Se)}else if(d.escapeTestNoEncode.test(e))return e.replace(d.escapeReplaceNoEncode,Se);return e}function Re(e){return e.replace(d.numericCharacterReference,(e,t,n)=>{let r=void 0===t?Number.parseInt(n,16):Number.parseInt(t,10);return 0===r||r>1114111||r>=55296&&r<=57343?"�":String.fromCodePoint(r)})}function Te(e){try{e=encodeURI(e).replace(d.percentDecode,"%")}catch{return null}return e}function ze(e,t){let n=e.replace(d.findPipe,(e,t,n)=>{let r=!1,s=t;for(;--s>=0&&"\\"===n[s];)r=!r;return r?"|":" |"}).split(d.splitPipe),r=0;if(n[0].trim()||n.shift(),n.length>0&&!n.at(-1)?.trim()&&n.pop(),t)if(n.length>t)n.splice(t);else for(;n.length<t;)n.push("");for(;r<n.length;r++)n[r]=n[r].trim().replace(d.slashPipe,"|");return n}function _e(e,t,n){let r=e.length;if(0===r)return"";let s=0;for(;s<r;){let l=e.charAt(r-s-1);if(l!==t||n){if(l===t||!n)break;s++}else s++}return e.slice(0,r-s)}function Ae(e){let t=e.split("\n"),n=t.length-1;for(;n>=0&&d.blankLine.test(t[n]);)n--;return t.length-n<=2?e:t.slice(0,n+1).join("\n")}function Pe(e){return e.trim().toLowerCase().toUpperCase().toLowerCase()}function Le(e,t=0){let n=t,r="";for(let t of e)if("\t"===t){let e=4-n%4;r+=" ".repeat(e),n+=e}else r+=t,n++;return r}function Ie(e,t,n,r,s){let l=t.href,i=t.title||null,a=e[1].replace(s.other.outputLinkReplace,"$1"),o="!"===e[0].charAt(0);r.state.inLink=!0;let c=r.state.linkEmitted,p=r.state.inRawBlock;r.state.linkEmitted=!1;let h=r.inlineTokens(a),u=r.state.linkEmitted;if(r.state.linkEmitted=c,r.state.inLink=!1,!o){if(u)return void(r.state.inRawBlock=p);r.state.linkEmitted=!0}return{type:o?"image":"link",raw:n,href:l,title:i,text:a,tokens:h}}function Ee(e,t,n,r){if(!t.includes("<"))return!1;for(let s=0;s<t.length;s++){if("\\"===t[s]){s++;continue}if("`"===t[s]){let e=r.inline.code.exec(t.slice(s));if(e){s+=e[0].length-1;continue}}if("<"!==t[s])continue;let l=e.slice(n+s),i=r.inline.tag.exec(l)||r.inline.autolink.exec(l);if(i){if(i[0].length>t.length-s)return!0;s+=i[0].length-1}}return!1}var Ce=class{options;rules;lexer;constructor(e){this.options=e||p}space(e){let t=this.rules.block.newline.exec(e);if(t&&t[0].length>0)return{type:"space",raw:t[0]}}code(e){let t=this.rules.block.code.exec(e);if(t){let e=this.options.pedantic?t[0]:Ae(t[0]),n=e.replace(this.rules.other.codeRemoveIndent,"");return{type:"code",raw:e,codeBlockStyle:"indented",text:n}}}fences(e){let t=this.rules.block.fences.exec(e);if(t){let e=t[0],n=function(e,t,n){let r=e.match(n.other.indentCodeCompensation);if(null===r)return t;let s=r[1];return t.split("\n").map(e=>{let t=e.match(n.other.beginningSpace);if(null===t)return e;let[r]=t;return e.slice(Math.min(r.length,s.length))}).join("\n")}(e,t[3]||"",this.rules);return{type:"code",raw:e,lang:t[2]?t[2].trim().replace(this.rules.inline.anyPunctuation,"$1"):t[2],text:n}}}heading(e){let t=this.rules.block.heading.exec(e);if(t){let e=t[2].trim();if(this.rules.other.endingHash.test(e)){let t=_e(e,"#");(this.options.pedantic||!t||this.rules.other.endingSpaceTabChar.test(t))&&(e=t.trim())}return{type:"heading",raw:_e(t[0],"\n"),depth:t[1].length,text:e,tokens:this.lexer.inline(e)}}}hr(e){let t=this.rules.block.hr.exec(e);if(t)return{type:"hr",raw:_e(t[0],"\n")}}blockquote(e){let t=this.rules.block.blockquote.exec(e);if(t){let e=_e(t[0],"\n").split("\n"),n="",r="",s=[];for(;e.length>0;){let t,l=!1,i=[];for(t=0;t<e.length;t++)if(this.rules.other.blockquoteStart.test(e[t]))i.push(e[t]),l=!0;else{if(l)break;i.push(e[t])}e=e.slice(t);let a=i.join("\n"),o=a.replace(this.rules.other.blockquoteSetextReplace,"\n    $1").replace(this.rules.other.blockquoteSetextReplace2,"");n=n?`${n}\n${a}`:a,r=r?`${r}\n${o}`:o;let c=this.lexer.state.top;if(this.lexer.state.top=!0,this.lexer.blockTokens(o,s,!0),this.lexer.state.top=c,0===e.length)break;let p=s.at(-1);if("code"===p?.type)break;if("blockquote"===p?.type){let t=p,l=e.join("\n"),i=t.raw+"\n"+l.replace(this.rules.other.blockquoteSetextReplace2,""),a=this.blockquote(i);s[s.length-1]=a;let o=i.substring(a.raw.length).replace(/^\n/,""),c=o?o.split("\n").length:0,h=c?e.slice(0,-c):e;h.length>0&&(n=`${n}\n${h.join("\n")}`),r=r.substring(0,r.length-t.text.length)+a.text;break}if("list"===p?.type){let t=p,l=t.raw+"\n"+e.join("\n"),i=this.list(l);s[s.length-1]=i,n=n.substring(0,n.length-p.raw.length)+i.raw,r=r.substring(0,r.length-t.raw.length)+i.raw,e=l.substring(s.at(-1).raw.length).split("\n");continue}}return{type:"blockquote",raw:n,tokens:s,text:r}}}list(e){let t=this.rules.block.list.exec(e);if(t){let n=t[1].trim(),r=n.length>1,s={type:"list",raw:"",ordered:r,start:r?+n.slice(0,-1):"",loose:!1,items:[]};n=r?`\\d{1,9}\\${n.slice(-1)}`:`\\${n}`,this.options.pedantic&&(n=r?n:"[*+-]");let l=this.rules.other.listItemRegex(n),i=!1;for(;e;){let n=!1,r="",a="";if(!(t=l.exec(e))||this.rules.block.hr.test(e))break;r=t[0],e=e.substring(r.length);let o=t[2].split("\n",1)[0],c=t[1].length,p=this.options.pedantic?Le(o,c):o.replace(this.rules.other.leadingSpaceTab,e=>Le(e,c)),h=e.split("\n",1)[0],u=!p.trim(),g=0;if(this.options.pedantic?(g=2,a=p.trimStart()):u?g=c+1:(g=p.search(this.rules.other.nonSpaceChar),g=g>4?1:g,a=p.slice(g),g+=c),u&&this.rules.other.blankLine.test(h)&&(r+=h+"\n",e=e.substring(h.length+1),n=!0),!n){let t=this.rules.other.nextBulletRegex(g),n=this.rules.other.hrRegex(g),s=this.rules.other.fencesBeginRegex(g),l=this.rules.other.headingBeginRegex(g),i=this.rules.other.htmlBeginRegex(g),o=this.rules.other.blockquoteBeginRegex(g);for(;e;){let c,k=e.split("\n",1)[0];if(h=k,this.options.pedantic?(h=h.replace(this.rules.other.listReplaceNesting,"  "),c=h):c=h.replace(this.rules.other.leadingSpaceTab,e=>e.replace(this.rules.other.tabCharGlobal,"    ")),s.test(h)||l.test(h)||i.test(h)||o.test(h)||t.test(h)||n.test(h))break;if(c.search(this.rules.other.nonSpaceChar)>=g||!h.trim())a+="\n"+c.slice(g);else{if(u||p.replace(this.rules.other.tabCharGlobal,"    ").search(this.rules.other.nonSpaceChar)>=4||s.test(p)||l.test(p)||n.test(p))break;a+="\n"+h}u=!h.trim(),r+=k+"\n",e=e.substring(k.length+1),p=c.slice(g)}}s.loose||(i?s.loose=!0:this.rules.other.doubleBlankLine.test(r)&&(i=!0)),s.items.push({type:"list_item",raw:r,task:!!this.options.gfm&&this.rules.other.listIsTask.test(a),loose:!1,text:a,tokens:[]}),s.raw+=r}let a=s.items.at(-1);if(!a)return;a.raw=a.raw.trimEnd(),a.text=a.text.trimEnd(),s.raw=s.raw.trimEnd();for(let e of s.items)if(this.lexer.state.top=!1,e.tokens=this.lexer.blockTokens(e.text,[]),!s.loose){let t=e.tokens.filter(e=>"space"===e.type),n=t.length>0&&t.some(e=>this.rules.other.anyLine.test(e.raw));s.loose=n}for(let e of s.items){let t=e.tokens[0];if(!e.task||"text"!==t?.type&&"paragraph"!==t?.type)e.task&&(e.task=!1);else{e.text=e.text.replace(this.rules.other.listReplaceTask,""),t.raw=t.raw.replace(this.rules.other.listReplaceTask,""),t.text=t.text.replace(this.rules.other.listReplaceTask,"");for(let e=this.lexer.inlineQueue.length-1;e>=0;e--)if(this.rules.other.listIsTask.test(this.lexer.inlineQueue[e].src)){this.lexer.inlineQueue[e].src=this.lexer.inlineQueue[e].src.replace(this.rules.other.listReplaceTask,"");break}let n=this.rules.other.listTaskCheckbox.exec(e.raw);if(n){let t={type:"checkbox",raw:n[0]+" ",checked:"[ ]"!==n[0]};e.checked=t.checked,s.loose?e.tokens[0]&&["paragraph","text"].includes(e.tokens[0].type)&&"tokens"in e.tokens[0]&&e.tokens[0].tokens?(e.tokens[0].raw=t.raw+e.tokens[0].raw,e.tokens[0].text=t.raw+e.tokens[0].text,e.tokens[0].tokens.unshift(t)):e.tokens.unshift({type:"paragraph",raw:t.raw,text:t.raw,tokens:[t]}):e.tokens.unshift(t)}}}if(s.loose)for(let e of s.items){e.loose=!0;for(let t of e.tokens)"text"===t.type&&(t.type="paragraph")}return s}}html(e){let t=this.rules.block.html.exec(e);if(t){let e=Ae(t[0]);return{type:"html",block:!0,raw:e,pre:"pre"===t[1]||"script"===t[1]||"style"===t[1],text:e}}}def(e){let t=this.rules.block.def.exec(e);if(t){let e=Pe(t[1]).replace(this.rules.other.multipleSpaceGlobal," "),n=t[2]?t[2].replace(this.rules.other.hrefBrackets,"$1").replace(this.rules.inline.anyPunctuation,"$1"):"",r=t[3]?t[3].substring(1,t[3].length-1).replace(this.rules.inline.anyPunctuation,"$1"):t[3];return{type:"def",tag:e,raw:_e(t[0],"\n"),href:n,title:r}}}table(e){let t=this.rules.block.table.exec(e);if(!t||!this.rules.other.tableDelimiter.test(t[2]))return;let n=ze(t[1]),r=t[2].replace(this.rules.other.tableAlignChars,"").split("|"),s=t[3]?.trim()?t[3].replace(this.rules.other.tableRowBlankLine,"").split("\n"):[],l={type:"table",raw:_e(t[0],"\n"),header:[],align:[],rows:[]};if(n.length===r.length){for(let e of r)this.rules.other.tableAlignRight.test(e)?l.align.push("right"):this.rules.other.tableAlignCenter.test(e)?l.align.push("center"):this.rules.other.tableAlignLeft.test(e)?l.align.push("left"):l.align.push(null);for(let e=0;e<n.length;e++)l.header.push({text:n[e],tokens:this.lexer.inline(n[e]),header:!0,align:l.align[e]});for(let e of s)l.rows.push(ze(e,l.header.length).map((e,t)=>({text:e,tokens:this.lexer.inline(e),header:!1,align:l.align[t]})));return l}}lheading(e){let t=this.rules.block.lheading.exec(e);if(t){let e=t[1].trim();return{type:"heading",raw:_e(t[0],"\n"),depth:"="===t[2].charAt(0)?1:2,text:e,tokens:this.lexer.inline(e)}}}paragraph(e){let t=this.rules.block.paragraph.exec(e);if(t){let e="\n"===t[1].charAt(t[1].length-1)?t[1].slice(0,-1):t[1];return{type:"paragraph",raw:t[0],text:e,tokens:this.lexer.inline(e)}}}text(e){let t=this.rules.block.text.exec(e);if(t)return{type:"text",raw:t[0],text:t[0],tokens:this.lexer.inline(t[0])}}escape(e){let t=this.rules.inline.escape.exec(e);if(t)return{type:"escape",raw:t[0],text:t[1]}}tag(e){let t=this.rules.inline.tag.exec(e);if(t)return!this.lexer.state.inLink&&this.rules.other.startATag.test(t[0])?this.lexer.state.inLink=!0:this.lexer.state.inLink&&this.rules.other.endATag.test(t[0])&&(this.lexer.state.inLink=!1),!this.lexer.state.inRawBlock&&this.rules.other.startPreScriptTag.test(t[0])?this.lexer.state.inRawBlock=!0:this.lexer.state.inRawBlock&&this.rules.other.endPreScriptTag.test(t[0])&&(this.lexer.state.inRawBlock=!1),{type:"html",raw:t[0],inLink:this.lexer.state.inLink,inRawBlock:this.lexer.state.inRawBlock,block:!1,text:t[0]}}link(e){let t=this.rules.inline.link.exec(e);if(t){let n="!"===t[0].charAt(0)?2:1;if(!this.options.pedantic&&Ee(e,t[1],n,this.rules))return;let r=t[2].trim();if(!this.options.pedantic&&this.rules.other.startAngleBracket.test(r)){if(!this.rules.other.endAngleBracket.test(r))return;let e=_e(r.slice(0,-1),"\\");if((r.length-e.length)%2==0)return}else{let e=function(e,t){if(-1===e.indexOf(t[1]))return-1;let n=0;for(let r=0;r<e.length;r++)if("\\"===e[r])r++;else if(e[r]===t[0])n++;else if(e[r]===t[1]&&(n--,n<0))return r;return n>0?-2:-1}(t[2],"()");if(-2===e)return;if(e>-1){let n=(0===t[0].indexOf("!")?5:4)+t[1].length+e;t[2]=t[2].substring(0,e),t[0]=t[0].substring(0,n).trim(),t[3]=""}}let s=t[2],l="";if(this.options.pedantic){let e=this.rules.other.pedanticHrefTitle.exec(s);e&&(s=e[1],l=e[3])}else l=t[3]?t[3].slice(1,-1):"";return s=s.trim(),this.rules.other.startAngleBracket.test(s)&&(s=this.options.pedantic&&!this.rules.other.endAngleBracket.test(r)?s.slice(1):s.slice(1,-1)),Ie(t,{href:s&&s.replace(this.rules.inline.anyPunctuation,"$1"),title:l&&l.replace(this.rules.inline.anyPunctuation,"$1")},t[0],this.lexer,this.rules)}}reflink(e,t){let n;if((n=this.rules.inline.reflink.exec(e))||(n=this.rules.inline.nolink.exec(e))){let r="!"===n[0].charAt(0)?2:1;if(!this.options.pedantic&&Ee(e,n[1],r,this.rules))return;let s=t[Pe((n[2]||n[1]).replace(this.rules.other.multipleSpaceGlobal," "))];if(!s){let e=n[0].charAt(0);return{type:"text",raw:e,text:e}}return Ie(n,s,n[0],this.lexer,this.rules)}}emStrong(e,t,n=""){let r=this.rules.inline.emStrongLDelim.exec(e);if(r&&(r[1]||r[2]||r[3]||r[4])&&(!r[4]||!n.match(this.rules.other.unicodeAlphaNumeric))&&(!r[1]&&!r[3]||!n||this.rules.inline.punctuation.exec(n))){let s,l,i=[...r[0]].length-1,a=i,o=0,c=r[0][0],p=n===c,h="*"===c?this.rules.inline.emStrongRDelimAst:this.rules.inline.emStrongRDelimUnd;for(h.lastIndex=0,t=t.slice(-1*e.length+i);null!==(r=h.exec(t));){if(s=r[1]||r[2]||r[3]||r[4]||r[5]||r[6],!s)continue;if(l=[...s].length,r[3]||r[4]){a+=l;continue}if(r[5]||r[6]){if(i%3&&!((i+l)%3)){o+=l;continue}if(p)break}if(a-=l,a>0)continue;l=Math.min(l,l+a+o);let t=[...r[0]][0].length,n=e.slice(0,i+r.index+t+l);if(Math.min(i,l)%2){let e=n.slice(1,-1);return{type:"em",raw:n,text:e,tokens:this.lexer.inlineTokens(e)}}let c=n.slice(2,-2);return{type:"strong",raw:n,text:c,tokens:this.lexer.inlineTokens(c)}}}}codespan(e){let t=this.rules.inline.code.exec(e);if(t){let e=t[2].replace(this.rules.other.newLineCharGlobal," "),n=this.rules.other.nonSpaceChar.test(e),r=this.rules.other.startingSpaceChar.test(e)&&this.rules.other.endingSpaceChar.test(e);return n&&r&&(e=e.substring(1,e.length-1)),{type:"codespan",raw:t[0],text:e}}}br(e){let t=this.rules.inline.br.exec(e);if(t)return{type:"br",raw:t[0]}}del(e,t,n=""){let r=this.rules.inline.delLDelim.exec(e);if(r&&(!r[1]||!n||this.rules.inline.punctuation.exec(n))){let n,s,l=[...r[0]].length-1,i=l,a=this.rules.inline.delRDelim;for(a.lastIndex=0,t=t.slice(-1*e.length+l);null!==(r=a.exec(t));){if(n=r[1]||r[2]||r[3]||r[4]||r[5]||r[6],!n||(s=[...n].length,s!==l))continue;if(r[3]||r[4]){i+=s;continue}if(i-=s,i>0)continue;s=Math.min(s,s+i);let t=[...r[0]][0].length,a=e.slice(0,l+r.index+t+s),o=a.slice(l,-l);return{type:"del",raw:a,text:o,tokens:this.lexer.inlineTokens(o)}}}}autolink(e){let t=this.rules.inline.autolink.exec(e);if(t){let e,n;return"@"===t[2]?(e=t[1],n="mailto:"+e):(e=t[1],n=e),{type:"link",raw:t[0],text:e,href:n,autolink:!0,tokens:[{type:"text",raw:e,text:e}]}}}url(e){let t;if(t=this.rules.inline.url.exec(e)){let e,n;if("@"===t[2])e=t[0],n="mailto:"+e;else{let r;do{r=t[0],t[0]=this.rules.inline._backpedal.exec(t[0])?.[0]??""}while(r!==t[0]);e=t[0],n="www."===t[1]?"\u0068\u0074\u0074\u0070://"+t[0]:t[0]}return{type:"link",raw:t[0],text:e,href:n,autolink:!0,tokens:[{type:"text",raw:e,text:e}]}}}inlineText(e){let t=this.rules.inline.text.exec(e);if(t){let e=this.lexer.state.inRawBlock;return{type:"text",raw:t[0],text:e?t[0]:Re(t[0]),escaped:e}}}},Be=class e{tokens;options;state;inlineQueue;tokenizer;constructor(e){this.tokens=[],this.tokens.links=Object.create(null),this.options=e||p,this.options.tokenizer=this.options.tokenizer||new Ce,this.tokenizer=this.options.tokenizer,this.tokenizer.options=this.options,this.tokenizer.lexer=this,this.inlineQueue=[],this.state={inLink:!1,inRawBlock:!1,linkEmitted:!1,top:!0};let t={other:d,block:me.normal,inline:we.normal};this.options.pedantic?(t.block=me.pedantic,t.inline=we.pedantic):this.options.gfm&&(t.block=me.gfm,this.options.breaks?t.inline=we.breaks:t.inline=we.gfm),this.tokenizer.rules=t}static get rules(){return{block:me,inline:we}}static lex(t,n){return new e(n).lex(t)}static lexInline(t,n){return new e(n).inlineTokens(t)}lex(e){e=e.replace(d.carriageReturn,"\n"),this.blockTokens(e,this.tokens);for(let e=0;e<this.inlineQueue.length;e++){let t=this.inlineQueue[e];this.inlineTokens(t.src,t.tokens)}return this.inlineQueue=[],this.tokens}blockTokens(e,t=[],n=!1){this.tokenizer.lexer=this,this.options.pedantic&&(e=e.replace(d.tabCharGlobal,"    ").replace(d.spaceLine,""));let r=1/0;for(;e;){if(!(e.length<r)){this.infiniteLoopError(e.charCodeAt(0));break}let s;if(r=e.length,this.options.extensions?.block?.some(n=>!!(s=n.call({lexer:this},e,t))&&(e=e.substring(s.raw.length),t.push(s),!0)))continue;if(s=this.tokenizer.space(e)){e=e.substring(s.raw.length);let n=t.at(-1);1===s.raw.length&&void 0!==n?n.raw+="\n":t.push(s);continue}if(s=this.tokenizer.code(e)){e=e.substring(s.raw.length);let n=t.at(-1);"paragraph"===n?.type||"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.text,this.inlineQueue.at(-1).src=n.text):t.push(s);continue}if(s=this.tokenizer.fences(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.heading(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.hr(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.blockquote(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.list(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.html(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.def(e)){e=e.substring(s.raw.length);let n=t.at(-1);"paragraph"===n?.type||"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.raw,this.inlineQueue.at(-1).src=n.text):this.tokens.links[s.tag]||(this.tokens.links[s.tag]={href:s.href,title:s.title},t.push(s));continue}if(s=this.tokenizer.table(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.lheading(e)){e=e.substring(s.raw.length),t.push(s);continue}let l=e;if(this.options.extensions?.startBlock){let t,n=1/0,r=e.slice(1);this.options.extensions.startBlock.forEach(e=>{t=e.call({lexer:this},r),"number"==typeof t&&t>=0&&(n=Math.min(n,t))}),n<1/0&&n>=0&&(l=e.substring(0,n+1))}if(this.state.top&&(s=this.tokenizer.paragraph(l))){let r=t.at(-1);n&&"paragraph"===r?.type?(r.raw+=(r.raw.endsWith("\n")?"":"\n")+s.raw,r.text+="\n"+s.text,this.inlineQueue.pop(),this.inlineQueue.at(-1).src=r.text):t.push(s),n=l.length!==e.length,e=e.substring(s.raw.length);continue}if(s=this.tokenizer.text(e)){e=e.substring(s.raw.length);let n=t.at(-1);"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.text,this.inlineQueue.pop(),this.inlineQueue.at(-1).src=n.text):t.push(s);continue}if(e){this.infiniteLoopError(e.charCodeAt(0));break}}return this.state.top=!0,t}inline(e,t=[]){return this.inlineQueue.push({src:e,tokens:t}),t}linkInText(e){if(!e.includes("["))return!1;let t=this.tokenizer.rules.inline.link;for(let n of e.matchAll(this.tokenizer.rules.inline.blockSkip))if(t.test(n[0])&&"!"!==e.charAt(n.index-1))return!0;for(let t of e.matchAll(this.tokenizer.rules.inline.reflinkSearch)){let e=t[0],n=e.lastIndexOf("[");if("!"!==e.charAt(0)&&Object.hasOwn(this.tokens.links,Pe(e.slice(n+1,-1)))&&!(n>1&&this.linkInText(e.slice(1,n-1))))return!0}return!1}inlineTokens(e,t=[]){this.tokenizer.lexer=this;let n=e;if(this.tokens.links&&e.includes("[")){let e=this.tokenizer.rules.inline.reflinkSearch,t=n=>{let r=n.lastIndexOf("[");if(!Object.hasOwn(this.tokens.links,Pe(n.slice(r+1,-1))))return n;if(r>1&&"!"!==n.charAt(0)){let s=n.slice(1,r-1);if(this.linkInText(s))return"["+s.replace(e,t)+"]["+"a".repeat(n.length-r-2)+"]"}return"["+"a".repeat(n.length-2)+"]"};n=n.replace(e,t)}n=n.replace(this.tokenizer.rules.inline.anyPunctuation,e=>"+".repeat(e.length)),n=n.replace(this.tokenizer.rules.inline.blockSkip,(e,t,n)=>{let r=n?n.length:0;return e.slice(0,r)+"["+"a".repeat(e.length-r-2)+"]"}),n=this.options.hooks?.emStrongMask?.call({lexer:this},n)??n;let r=!1,s="",l=1/0;for(;e;){if(!(e.length<l)){this.infiniteLoopError(e.charCodeAt(0));break}let i;if(l=e.length,r||(s=""),r=!1,this.options.extensions?.inline?.some(n=>!!(i=n.call({lexer:this},e,t))&&(e=e.substring(i.raw.length),t.push(i),!0)))continue;if(i=this.tokenizer.escape(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.tag(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.link(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.reflink(e,this.tokens.links)){e=e.substring(i.raw.length);let n=t.at(-1);"text"===i.type&&"text"===n?.type?(n.raw+=i.raw,n.text+=i.text):t.push(i);continue}if(i=this.tokenizer.emStrong(e,n,s)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.codespan(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.br(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.del(e,n,s)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.autolink(e)){e=e.substring(i.raw.length),t.push(i);continue}if(!this.state.inLink&&(i=this.tokenizer.url(e))){e=e.substring(i.raw.length),t.push(i);continue}let a=e;if(this.options.extensions?.startInline){let t,n=1/0,r=e.slice(1);this.options.extensions.startInline.forEach(e=>{t=e.call({lexer:this},r),"number"==typeof t&&t>=0&&(n=Math.min(n,t))}),n<1/0&&n>=0&&(a=e.substring(0,n+1))}if(i=this.tokenizer.inlineText(a)){e=e.substring(i.raw.length),"_"!==i.raw.slice(-1)&&(s=i.raw.slice(-1)),r=!0;let n=t.at(-1);"text"===n?.type?(n.raw+=i.raw,n.text+=i.text):t.push(i);continue}if(e){this.infiniteLoopError(e.charCodeAt(0));break}}return t}infiniteLoopError(e){let t="Infinite loop on byte: "+e;if(!this.options.silent)throw new Error(t);console.error(t)}},qe=class{options;parser;constructor(e){this.options=e||p}space(e){return""}code({text:e,lang:t,escaped:n}){let r=(t||"").match(d.notSpaceStart)?.[0],s=e?e.replace(d.endingNewline,"")+"\n":"";return r?'<pre><code class="language-'+$e(r)+'">'+(n?s:$e(s,!0))+"</code></pre>\n":"<pre><code>"+(n?s:$e(s,!0))+"</code></pre>\n"}blockquote({tokens:e}){return`<blockquote>\n${this.parser.parse(e)}</blockquote>\n`}html({text:e}){return e}def(e){return""}heading({tokens:e,depth:t}){return`<h${t}>${this.parser.parseInline(e)}</h${t}>\n`}hr(e){return"<hr>\n"}list(e){let t=e.ordered,n=e.start,r="";for(let t=0;t<e.items.length;t++){let n=e.items[t];r+=this.listitem(n)}let s=t?"ol":"ul";return"<"+s+(t&&1!==n?' start="'+n+'"':"")+">\n"+r+"</"+s+">\n"}listitem(e){return`<li>${this.parser.parse(e.tokens)}</li>\n`}checkbox({checked:e}){return"<input "+(e?'checked="" ':"")+'disabled="" type="checkbox"> '}paragraph({tokens:e}){return`<p>${this.parser.parseInline(e)}</p>\n`}table(e){let t="",n="";for(let t=0;t<e.header.length;t++)n+=this.tablecell(e.header[t]);t+=this.tablerow({text:n});let r="";for(let t=0;t<e.rows.length;t++){let s=e.rows[t];n="";for(let e=0;e<s.length;e++)n+=this.tablecell(s[e]);r+=this.tablerow({text:n})}return r&&(r=`<tbody>${r}</tbody>`),"<table>\n<thead>\n"+t+"</thead>\n"+r+"</table>\n"}tablerow({text:e}){return`<tr>\n${e}</tr>\n`}tablecell(e){let t=this.parser.parseInline(e.tokens),n=e.header?"th":"td";return(e.align?`<${n} align="${e.align}">`:`<${n}>`)+t+`</${n}>\n`}strong({tokens:e}){return`<strong>${this.parser.parseInline(e)}</strong>`}em({tokens:e}){return`<em>${this.parser.parseInline(e)}</em>`}codespan({text:e}){return`<code>${$e(e,!0)}</code>`}br(e){return"<br>"}del({tokens:e}){return`<del>${this.parser.parseInline(e)}</del>`}link({href:e,title:t,text:n,tokens:r,autolink:s}){let l=s?$e(n,!0):this.parser.parseInline(r),i=Te(e);if(null===i)return l;let a='<a href="'+(e=$e(i,s))+'"';return t&&(a+=' title="'+$e(t)+'"'),a+=">"+l+"</a>",a}image({href:e,title:t,text:n,tokens:r}){r&&(n=this.parser.parseInline(r,this.parser.textRenderer));let s=Te(e);if(null===s)return $e(n);let l=`<img src="${$e(e=s)}" alt="${$e(n)}"`;return t&&(l+=` title="${$e(t)}"`),l+=">",l}text(e){return"tokens"in e&&e.tokens?this.parser.parseInline(e.tokens):"escaped"in e&&e.escaped?e.text:$e(e.text)}},Ze=class{strong({text:e}){return e}em({text:e}){return e}codespan({text:e}){return e}del({text:e}){return e}html({text:e}){return e}text({text:e}){return e}link({text:e}){return""+e}image({text:e}){return""+e}br(){return""}checkbox({raw:e}){return e}},ve=class e{options;renderer;textRenderer;constructor(e){this.options=e||p,this.options.renderer=this.options.renderer||new qe,this.renderer=this.options.renderer,this.renderer.options=this.options,this.renderer.parser=this,this.textRenderer=new Ze}static parse(t,n){return new e(n).parse(t)}static parseInline(t,n){return new e(n).parseInline(t)}parse(e){this.renderer.parser=this;let t="";for(let n=0;n<e.length;n++){let r=e[n];if(this.options.extensions?.renderers?.[r.type]){let e=r,n=this.options.extensions.renderers[e.type].call({parser:this},e);if(!1!==n||!["space","hr","heading","code","table","blockquote","list","checkbox","html","def","paragraph","text"].includes(e.type)){t+=n||"";continue}}let s=r;switch(s.type){case"space":t+=this.renderer.space(s);break;case"hr":t+=this.renderer.hr(s);break;case"heading":t+=this.renderer.heading(s);break;case"code":t+=this.renderer.code(s);break;case"table":t+=this.renderer.table(s);break;case"blockquote":t+=this.renderer.blockquote(s);break;case"list":t+=this.renderer.list(s);break;case"checkbox":t+=this.renderer.checkbox(s);break;case"html":t+=this.renderer.html(s);break;case"def":t+=this.renderer.def(s);break;case"paragraph":t+=this.renderer.paragraph(s);break;case"text":t+=this.renderer.text(s);break;default:{let e='Token with "'+s.type+'" type was not found.';if(this.options.silent)return console.error(e),"";throw new Error(e)}}}return t}parseInline(e,t=this.renderer){this.renderer.parser=this;let n="";for(let r=0;r<e.length;r++){let s=e[r];if(this.options.extensions?.renderers?.[s.type]){let e=this.options.extensions.renderers[s.type].call({parser:this},s);if(!1!==e||!["escape","html","link","image","checkbox","strong","em","codespan","br","del","text"].includes(s.type)){n+=e||"";continue}}let l=s;switch(l.type){case"escape":case"text":n+=t.text(l);break;case"html":n+=t.html(l);break;case"link":n+=t.link(l);break;case"image":n+=t.image(l);break;case"checkbox":n+=t.checkbox(l);break;case"strong":n+=t.strong(l);break;case"em":n+=t.em(l);break;case"codespan":n+=t.codespan(l);break;case"br":n+=t.br(l);break;case"del":n+=t.del(l);break;default:{let e='Token with "'+l.type+'" type was not found.';if(this.options.silent)return console.error(e),"";throw new Error(e)}}}return n}},De=class{options;block;constructor(e){this.options=e||p}static passThroughHooks=new Set(["preprocess","postprocess","processAllTokens","emStrongMask"]);static passThroughHooksRespectAsync=new Set(["preprocess","postprocess","processAllTokens"]);preprocess(e){return e}postprocess(e){return e}processAllTokens(e){return e}emStrongMask(e){return e}provideLexer(e=this.block){return e?Be.lex:Be.lexInline}provideParser(e=this.block){return e?ve.parse:ve.parseInline}},Oe=class{defaults={async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null};options=this.setOptions;parse=this.parseMarkdown(!0);parseInline=this.parseMarkdown(!1);Parser=ve;Renderer=qe;TextRenderer=Ze;Lexer=Be;Tokenizer=Ce;Hooks=De;constructor(...e){this.use(...e)}walkTokens(e,t){let n=[];for(let r of e)switch(n=n.concat(t.call(this,r)),r.type){case"table":{let e=r;for(let r of e.header)n=n.concat(this.walkTokens(r.tokens,t));for(let r of e.rows)for(let e of r)n=n.concat(this.walkTokens(e.tokens,t));break}case"list":{let e=r;n=n.concat(this.walkTokens(e.items,t));break}default:{let e=r;this.defaults.extensions?.childTokens?.[e.type]?this.defaults.extensions.childTokens[e.type].forEach(r=>{let s=e[r].flat(1/0);n=n.concat(this.walkTokens(s,t))}):e.tokens&&(n=n.concat(this.walkTokens(e.tokens,t)))}}return n}use(...e){let t=this.defaults.extensions||{renderers:{},childTokens:{}};return e.forEach(e=>{let n={...e};if(n.async=this.defaults.async||n.async||!1,e.extensions&&(e.extensions.forEach(e=>{if(!e.name)throw new Error("extension name required");if("renderer"in e){let n=t.renderers[e.name];t.renderers[e.name]=n?function(...t){let r=e.renderer.apply(this,t);return!1===r&&(r=n.apply(this,t)),r}:e.renderer}if("tokenizer"in e){if(!e.level||"block"!==e.level&&"inline"!==e.level)throw new Error("extension level must be 'block' or 'inline'");let n=t[e.level];n?n.unshift(e.tokenizer):t[e.level]=[e.tokenizer],e.start&&("block"===e.level?t.startBlock?t.startBlock.push(e.start):t.startBlock=[e.start]:"inline"===e.level&&(t.startInline?t.startInline.push(e.start):t.startInline=[e.start]))}"childTokens"in e&&e.childTokens&&(t.childTokens[e.name]=e.childTokens)}),n.extensions=t),e.renderer){let t=this.defaults.renderer||new qe(this.defaults);for(let n in e.renderer){if(!(n in t))throw new Error(`renderer '${n}' does not exist`);if(["options","parser"].includes(n))continue;let r=n,s=e.renderer[r],l=t[r];t[r]=(...e)=>{let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n||""}}n.renderer=t}if(e.tokenizer){let t=this.defaults.tokenizer||new Ce(this.defaults);for(let n in e.tokenizer){if(!(n in t))throw new Error(`tokenizer '${n}' does not exist`);if(["options","rules","lexer"].includes(n))continue;let r=n,s=e.tokenizer[r],l=t[r];t[r]=(...e)=>{let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n}}n.tokenizer=t}if(e.hooks){let t=this.defaults.hooks||new De;for(let n in e.hooks){if(!(n in t))throw new Error(`hook '${n}' does not exist`);if(["options","block"].includes(n))continue;let r=n,s=e.hooks[r],l=t[r];De.passThroughHooks.has(n)?t[r]=e=>{if(this.defaults.async&&De.passThroughHooksRespectAsync.has(n))return(async()=>{let n=await s.call(t,e);return l.call(t,n)})();let r=s.call(t,e);return l.call(t,r)}:t[r]=(...e)=>{if(this.defaults.async)return(async()=>{let n=await s.apply(t,e);return!1===n&&(n=await l.apply(t,e)),n})();let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n}}n.hooks=t}if(e.walkTokens){let t=this.defaults.walkTokens,r=e.walkTokens;n.walkTokens=function(e){let n=[];return n.push(r.call(this,e)),t&&(n=n.concat(t.call(this,e))),n}}this.defaults={...this.defaults,...n}}),this}setOptions(e){return this.defaults={...this.defaults,...e},this}lexer(e,t){return Be.lex(e,t??this.defaults)}parser(e,t){return ve.parse(e,t??this.defaults)}parseMarkdown(e){return(t,n)=>{let r={...n},s={...this.defaults,...r},l=this.onError(!!s.silent,!!s.async);if(!0===this.defaults.async&&!1===r.async)return l(new Error("marked(): The async option was set to true by an extension. Remove async: false from the parse options object to return a Promise."));if(typeof t>"u"||null===t)return l(new Error("marked(): input parameter is undefined or null"));if("string"!=typeof t)return l(new Error("marked(): input parameter is of type "+Object.prototype.toString.call(t)+", string expected"));if(s.hooks&&(s.hooks.options=s,s.hooks.block=e),s.async)return(async()=>{let n=s.hooks?await s.hooks.preprocess(t):t,r=await(s.hooks?await s.hooks.provideLexer(e):e?Be.lex:Be.lexInline)(n,s),l=s.hooks?await s.hooks.processAllTokens(r):r;s.walkTokens&&await Promise.all(this.walkTokens(l,s.walkTokens));let i=await(s.hooks?await s.hooks.provideParser(e):e?ve.parse:ve.parseInline)(l,s);return s.hooks?await s.hooks.postprocess(i):i})().catch(l);try{s.hooks&&(t=s.hooks.preprocess(t));let n=(s.hooks?s.hooks.provideLexer(e):e?Be.lex:Be.lexInline)(t,s);s.hooks&&(n=s.hooks.processAllTokens(n)),s.walkTokens&&this.walkTokens(n,s.walkTokens);let r=(s.hooks?s.hooks.provideParser(e):e?ve.parse:ve.parseInline)(n,s);return s.hooks&&(r=s.hooks.postprocess(r)),r}catch(e){return l(e)}}}onError(e,t){return n=>{if(n.message+="\nPlease report this to https://github.com/markedjs/marked.",e){let e="<p>An error occurred:</p><pre>"+$e(n.message+"",!0)+"</pre>";return t?Promise.resolve(e):e}if(t)return Promise.reject(n);throw n}}},je=new Oe;function Qe(e,t){return je.parse(e,t)}function Me(...e){return je.use(...e),Qe.defaults=je.defaults,h(Qe.defaults),Qe}Qe.options=Qe.setOptions=function(e){return je.setOptions(e),Qe.defaults=je.defaults,h(Qe.defaults),Qe},Qe.getDefaults=c,Qe.defaults=p,Qe.use=Me,Qe.walkTokens=function(e,t){return je.walkTokens(e,t)},Qe.parseInline=je.parseInline,Qe.Parser=ve,Qe.parser=ve.parse,Qe.Renderer=qe,Qe.TextRenderer=Ze,Qe.Lexer=Be,Qe.lexer=Be.lex,Qe.Tokenizer=Ce,Qe.Hooks=De,Qe.parse=Qe;var Ne=Qe.options,He=Qe.setOptions,Ge=Qe.walkTokens,Xe=Qe.parseInline,Ue=Qe,Fe=ve.parse,We=Be.lex;return n!=t&&(r.exports=t),r.exports});
//# sourceMappingURL=/sm/d7299159e67c7c80ba99fc2715ddbcb78cc0161bbaf576d54ff7508d1ad287e0.map

/* === purify.min_3.4.16.js === */
/*! @license DOMPurify 3.4.16 | (c) Cure53 and other contributors | Released under the Apache license 2.0 and Mozilla Public License 2.0 | github.com/cure53/DOMPurify/blob/3.4.16/LICENSE */
(function(e,t){typeof exports==`object`&&typeof module<`u`?module.exports=t():typeof define==`function`&&define.amd?define([],t):(e=typeof globalThis<`u`?globalThis:e||self,e.DOMPurify=t())})(this,function(){"use strict";function e(e,t){this.v=e,this.k=t}function t(e,t){(t==null||t>e.length)&&(t=e.length);for(var n=0,r=Array(t);n<t;n++)r[n]=e[n];return r}function n(e){if(Array.isArray(e))return e}function r(e,t){var n=e==null?null:typeof Symbol<`u`&&e[Symbol.iterator]||e[`@@iterator`];if(n!=null){var r,i,a,o,s=[],c=!0,l=!1;try{if(a=(n=n.call(e)).next,t===0){if(Object(n)!==n)return;c=!1}else for(;!(c=(r=a.call(n)).done)&&(s.push(r.value),s.length!==t);c=!0);}catch(e){l=!0,i=e}finally{try{if(!c&&n.return!=null&&(o=n.return(),Object(o)!==o))return}finally{if(l)throw i}}return s}}function i(){throw TypeError(`Invalid attempt to destructure non-iterable instance.
In order to be iterable, non-array objects must have a [Symbol.iterator]() method.`)}
/*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/babel/babel/blob/main/packages/babel-helpers/LICENSE */
function a(e,t){return n(e)||r(e,t)||o(e,t)||i()}function o(e,n){if(e){if(typeof e==`string`)return t(e,n);var r={}.toString.call(e).slice(8,-1);return r===`Object`&&e.constructor&&(r=e.constructor.name),r===`Map`||r===`Set`?Array.from(e):r===`Arguments`||/^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(r)?t(e,n):void 0}}function s(t){var n,r;function i(n,r){try{var o=t[n](r),s=o.value,c=s instanceof e;Promise.resolve(c?s.v:s).then(function(e){if(c){var r=n===`return`&&s.k?n:`next`;if(!s.k||e.done)return i(r,e);e=t[r](e).value}a(!!o.done,e)},function(e){i(`throw`,e)})}catch(e){a(2,e)}}function a(e,t){e===2?n.reject(t):n.resolve({value:t,done:e}),(n=n.next)?i(n.key,n.arg):r=null}this._invoke=function(e,t){return new Promise(function(a,o){var s={key:e,arg:t,resolve:a,reject:o,next:null};r?r=r.next=s:(n=r=s,i(e,t))})},typeof t.return!=`function`&&(this.return=void 0)}s.prototype[typeof Symbol==`function`&&Symbol.asyncIterator||`@@asyncIterator`]=function(){return this},s.prototype.next=function(e){return this._invoke(`next`,e)},s.prototype.throw=function(e){return this._invoke(`throw`,e)},s.prototype.return=function(e){return this._invoke(`return`,e)};let c=Object.entries,l=Object.setPrototypeOf,u=Object.isFrozen,d=Object.getPrototypeOf,f=Object.getOwnPropertyDescriptor,p=Object.freeze,m=Object.seal,h=Object.create,ee=typeof Reflect<`u`&&Reflect,g=ee.apply,te=ee.construct;p||(p=function(e){return e}),m||(m=function(e){return e}),g||(g=function(e,t){var n=[...arguments].slice(2);return e.apply(t,n)}),te||(te=function(e){return new e(...[...arguments].slice(1))});let _=T(Array.prototype.forEach);Array.prototype.indexOf;let ne=T(Array.prototype.lastIndexOf),re=T(Array.prototype.pop),ie=T(Array.prototype.push);Array.prototype.slice;let ae=T(Array.prototype.splice),v=Array.isArray,oe=T(String.prototype.toLowerCase),se=T(String.prototype.toString),ce=T(String.prototype.match),le=T(String.prototype.replace),ue=T(String.prototype.indexOf),de=T(String.prototype.trim),fe=T(Number.prototype.toString),y=T(Boolean.prototype.toString),b=typeof BigInt>`u`?null:T(BigInt.prototype.toString),pe=typeof Symbol>`u`?null:T(Symbol.prototype.toString),x=T(Object.prototype.hasOwnProperty),S=T(Object.prototype.toString),C=T(RegExp.prototype.test),w=E(TypeError);function T(e){return function(t){t instanceof RegExp&&(t.lastIndex=0);var n=[...arguments].slice(1);return g(e,t,n)}}function E(e){return function(){return te(e,[...arguments])}}function D(e,t){let n=arguments.length>2&&arguments[2]!==void 0?arguments[2]:oe;if(l&&l(e,null),!v(t))return e;let r=t.length;for(;r--;){let i=t[r];if(typeof i==`string`){let e=n(i);e!==i&&(u(t)||(t[r]=e),i=e)}e[i]=!0}return e}function me(e){for(let t=0;t<e.length;t++)x(e,t)||(e[t]=null);return e}function O(e){let t=h(null);for(let r of c(e)){var n=a(r,2);let i=n[0],o=n[1];x(e,i)&&(t[i]=v(o)?me(o):o&&typeof o==`object`&&o.constructor===Object?O(o):o)}return t}function he(e){switch(typeof e){case`string`:return e;case`number`:return fe(e);case`boolean`:return y(e);case`bigint`:return b?b(e):`0`;case`symbol`:return pe?pe(e):`Symbol()`;case`undefined`:return S(e);case`function`:case`object`:{if(e===null)return S(e);let t=e,n=k(t,`toString`);if(typeof n==`function`){let e=n(t);return typeof e==`string`?e:S(e)}return S(e)}default:return S(e)}}function k(e,t){for(;e!==null;){let n=f(e,t);if(n){if(n.get)return T(n.get);if(typeof n.value==`function`)return T(n.value)}e=d(e)}function n(){return null}return n}function ge(e){try{return C(e,``),!0}catch(e){return!1}}let _e=p(/* @__PURE__ */ `a.abbr.acronym.address.area.article.aside.audio.b.bdi.bdo.big.blink.blockquote.body.br.button.canvas.caption.center.cite.code.col.colgroup.content.data.datalist.dd.decorator.del.details.dfn.dialog.dir.div.dl.dt.element.em.fieldset.figcaption.figure.font.footer.form.h1.h2.h3.h4.h5.h6.head.header.hgroup.hr.html.i.img.input.ins.kbd.label.legend.li.main.map.mark.marquee.menu.menuitem.meter.nav.nobr.ol.optgroup.option.output.p.picture.pre.progress.q.rp.rt.ruby.s.samp.search.section.select.shadow.slot.small.source.spacer.span.strike.strong.style.sub.summary.sup.table.tbody.td.template.textarea.tfoot.th.thead.time.tr.track.tt.u.ul.var.video.wbr`.split(`.`)),ve=p(/* @__PURE__ */ `svg.a.altglyph.altglyphdef.altglyphitem.animatecolor.animatemotion.animatetransform.circle.clippath.defs.desc.ellipse.enterkeyhint.exportparts.filter.font.g.glyph.glyphref.hkern.image.inputmode.line.lineargradient.marker.mask.metadata.mpath.part.path.pattern.polygon.polyline.radialgradient.rect.stop.style.switch.symbol.text.textpath.title.tref.tspan.view.vkern`.split(`.`)),ye=p([`feBlend`,`feColorMatrix`,`feComponentTransfer`,`feComposite`,`feConvolveMatrix`,`feDiffuseLighting`,`feDisplacementMap`,`feDistantLight`,`feDropShadow`,`feFlood`,`feFuncA`,`feFuncB`,`feFuncG`,`feFuncR`,`feGaussianBlur`,`feImage`,`feMerge`,`feMergeNode`,`feMorphology`,`feOffset`,`fePointLight`,`feSpecularLighting`,`feSpotLight`,`feTile`,`feTurbulence`]),be=p([`animate`,`color-profile`,`cursor`,`discard`,`font-face`,`font-face-format`,`font-face-name`,`font-face-src`,`font-face-uri`,`foreignobject`,`hatch`,`hatchpath`,`mesh`,`meshgradient`,`meshpatch`,`meshrow`,`missing-glyph`,`script`,`set`,`solidcolor`,`unknown`,`use`]),xe=p(/* @__PURE__ */ `math.menclose.merror.mfenced.mfrac.mglyph.mi.mlabeledtr.mmultiscripts.mn.mo.mover.mpadded.mphantom.mroot.mrow.ms.mspace.msqrt.mstyle.msub.msup.msubsup.mtable.mtd.mtext.mtr.munder.munderover.mprescripts`.split(`.`)),Se=p([`maction`,`maligngroup`,`malignmark`,`mlongdiv`,`mscarries`,`mscarry`,`msgroup`,`mstack`,`msline`,`msrow`,`semantics`,`annotation`,`annotation-xml`,`mprescripts`,`none`]),Ce=p([`#text`]),we=p(/* @__PURE__ */ `accept.action.align.alt.autocapitalize.autocomplete.autopictureinpicture.autoplay.background.bgcolor.border.capture.cellpadding.cellspacing.checked.cite.class.clear.color.cols.colspan.command.commandfor.controls.controlslist.coords.crossorigin.datetime.decoding.default.dir.disabled.disablepictureinpicture.disableremoteplayback.download.draggable.enctype.enterkeyhint.exportparts.face.for.headers.height.hidden.high.href.hreflang.id.inert.inputmode.integrity.ismap.kind.label.lang.list.loading.loop.low.max.maxlength.media.method.min.minlength.multiple.muted.name.nonce.noshade.novalidate.nowrap.open.optimum.part.pattern.placeholder.playsinline.popover.popovertarget.popovertargetaction.poster.preload.pubdate.radiogroup.readonly.rel.required.rev.reversed.role.rows.rowspan.spellcheck.scope.selected.shape.size.sizes.slot.span.srclang.start.src.srcset.step.style.summary.tabindex.title.translate.type.usemap.valign.value.width.wrap.xmlns`.split(`.`)),Te=p(/* @__PURE__ */ `accent-height.accumulate.additive.alignment-baseline.amplitude.ascent.attributename.attributetype.azimuth.basefrequency.baseline-shift.begin.bias.by.class.clip.clippathunits.clip-path.clip-rule.color.color-interpolation.color-interpolation-filters.color-profile.color-rendering.cx.cy.d.dx.dy.diffuseconstant.direction.display.divisor.dominant-baseline.dur.edgemode.elevation.end.exponent.fill.fill-opacity.fill-rule.filter.filterunits.flood-color.flood-opacity.font-family.font-size.font-size-adjust.font-stretch.font-style.font-variant.font-weight.fx.fy.g1.g2.glyph-name.glyphref.gradientunits.gradienttransform.height.href.id.image-rendering.in.in2.intercept.k.k1.k2.k3.k4.kerning.keypoints.keysplines.keytimes.lang.lengthadjust.letter-spacing.kernelmatrix.kernelunitlength.lighting-color.local.marker-end.marker-mid.marker-start.markerheight.markerunits.markerwidth.maskcontentunits.maskunits.max.mask.mask-type.media.method.mode.min.name.numoctaves.offset.operator.opacity.order.orient.orientation.origin.overflow.paint-order.path.pathlength.patterncontentunits.patterntransform.patternunits.pointer-events.points.preservealpha.preserveaspectratio.primitiveunits.r.rx.ry.radius.refx.refy.repeatcount.repeatdur.restart.result.rotate.scale.seed.shape-rendering.slope.specularconstant.specularexponent.spreadmethod.startoffset.stddeviation.stitchtiles.stop-color.stop-opacity.stroke-dasharray.stroke-dashoffset.stroke-linecap.stroke-linejoin.stroke-miterlimit.stroke-opacity.stroke.stroke-width.style.surfacescale.systemlanguage.tabindex.tablevalues.targetx.targety.transform.transform-origin.text-anchor.text-decoration.text-orientation.text-rendering.textlength.type.u1.u2.unicode.values.vector-effect.viewbox.visibility.version.vert-adv-y.vert-origin-x.vert-origin-y.width.word-spacing.wrap.writing-mode.xchannelselector.ychannelselector.x.x1.x2.xmlns.y.y1.y2.z.zoomandpan`.split(`.`)),Ee=p(/* @__PURE__ */ `accent.accentunder.align.bevelled.close.columnalign.columnlines.columnspacing.columnspan.denomalign.depth.dir.display.displaystyle.encoding.fence.frame.height.href.id.largeop.length.linethickness.lquote.lspace.mathbackground.mathcolor.mathsize.mathvariant.maxsize.minsize.movablelimits.notation.numalign.open.rowalign.rowlines.rowspacing.rowspan.rspace.rquote.scriptlevel.scriptminsize.scriptsizemultiplier.selection.separator.separators.stretchy.subscriptshift.supscriptshift.symmetric.voffset.width.xmlns`.split(`.`)),De=p([`xlink:href`,`xml:id`,`xlink:title`,`xml:space`,`xmlns:xlink`]),Oe=m(/{{[\w\W]*|^[\w\W]*}}/g),ke=m(/<%[\w\W]*|^[\w\W]*%>/g),Ae=m(/\${[\w\W]*/g),je=m(/^data-[\-\w.\u00B7-\uFFFF]+$/),Me=m(/^aria-[\-\w]+$/),Ne=m(/^(?:(?:(?:f|ht)tps?|mailto|tel|callto|sms|cid|xmpp|matrix):|[^a-z]|[a-z+.\-]+(?:[^a-z+.\-:]|$))/i),Pe=m(/^(?:\w+script|data):/i),Fe=m(/[\u0000-\u0020\u00A0\u1680\u180E\u2000-\u2029\u205F\u3000]/g),Ie=m(/^html$/i),Le=m(/^[a-z][.\w]*(-[.\w]+)+$/i),Re=m(/<[/\w!]/g),ze=m(/<[/\w]/g),Be=m(/<\/no(script|embed|frames)/i),Ve=m(/\/>/i),A={element:1,attribute:2,text:3,cdataSection:4,entityReference:5,entityNode:6,processingInstruction:7,comment:8,document:9,documentType:10,documentFragment:11,notation:12},j=[`style`,`script`,`xmp`,`iframe`,`noembed`,`noframes`,`plaintext`,`noscript`],He=p(D({},j)),Ue=function(){let e={};return _(j,t=>{e[t]=m(RegExp(`</`+t+`(?=[\\t\\n\\f\\r />])`,`i`))}),p(e)}(),We=function(){return typeof window>`u`?null:window},Ge=function(e,t){if(typeof e!=`object`||typeof e.createPolicy!=`function`)return null;let n=null,r=`data-tt-policy-suffix`;t&&t.hasAttribute(r)&&(n=t.getAttribute(r));let i=`dompurify`+(n?`#`+n:``);try{return e.createPolicy(i,{createHTML(e){return e},createScriptURL(e){return e}})}catch(e){return console.warn(`TrustedTypes policy `+i+` could not be created.`),null}},Ke=function(){return{afterSanitizeAttributes:[],afterSanitizeElements:[],afterSanitizeShadowDOM:[],beforeSanitizeAttributes:[],beforeSanitizeElements:[],beforeSanitizeShadowDOM:[],uponSanitizeAttribute:[],uponSanitizeElement:[],uponSanitizeShadowNode:[]}},M=function(e,t,n,r){return x(e,t)&&v(e[t])?D(r.base?O(r.base):{},e[t],r.transform):n},qe=function(e,t,n){let r=x(e,t)?e[t]:void 0;return r&&typeof r==`object`?O(r):n()};function Je(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:We(),t=e=>Je(e);if(t.version=`3.4.16`,t.removed=[],!e||!e.document||e.document.nodeType!==A.document||!e.Element)return t.isSupported=!1,t;let n=e.document,r=n,i=r.currentScript;e.DocumentFragment;let a=e.HTMLTemplateElement,o=e.Node,s=e.Element,l=e.NodeFilter;e.NamedNodeMap===void 0&&(e.NamedNodeMap||e.MozNamedAttrMap),e.HTMLFormElement;let u=e.DOMParser,d=e.trustedTypes,f=s.prototype,ee=k(f,`cloneNode`),g=k(f,`remove`),te=k(f,`removeAttributeNode`),fe=k(f,`nextSibling`),y=k(f,`childNodes`),b=k(f,`parentNode`),pe=k(f,`shadowRoot`),S=k(f,`attributes`),T=o&&o.prototype?k(o.prototype,`nodeType`):null,E=o&&o.prototype?k(o.prototype,`nodeName`):null,me=o&&o.prototype?k(o.prototype,`ownerDocument`):null,j=function(e){return T?T(e):e.nodeType},Ye=function(e){return E?E(e):e.nodeName};if(typeof a==`function`){let e=n.createElement(`template`);e.content&&e.content.ownerDocument&&(n=e.content.ownerDocument)}let N,P=``,Xe,Ze=!1,Qe=0,$e=function(){if(Qe>0)throw w(`A configured TRUSTED_TYPES_POLICY callback (createHTML or createScriptURL) must not call DOMPurify.sanitize, as that causes infinite recursion. Do not pass a policy whose callbacks wrap DOMPurify as TRUSTED_TYPES_POLICY; see the "DOMPurify and Trusted Types" section of the README.`)},F=function(e){$e(),Qe++;try{return N.createHTML(e)}finally{Qe--}},et=function(e){$e(),Qe++;try{return N.createScriptURL(e)}finally{Qe--}},tt=function(){return Ze||(Xe=Ge(d,i),Ze=!0),Xe},nt=n,rt=nt.implementation,it=nt.createNodeIterator,at=nt.createDocumentFragment,ot=nt.getElementsByTagName,st=r.importNode,I=Ke();t.isSupported=typeof c==`function`&&typeof b==`function`&&rt&&rt.createHTMLDocument!==void 0;let ct=Oe,lt=ke,ut=Ae,dt=je,ft=Me,pt=Pe,mt=Fe,ht=Le,gt=Ne,L=null,_t=D({},[..._e,...ve,...ye,...xe,...Ce]),R=null,vt=D({},[...we,...Te,...Ee,...De]),z=Object.seal(h(null,{tagNameCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},attributeNameCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},allowCustomizedBuiltInElements:{writable:!0,configurable:!1,enumerable:!0,value:!1}})),yt=null,bt=null,B=Object.seal(h(null,{tagCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},attributeCheck:{writable:!0,configurable:!1,enumerable:!0,value:null}})),xt=!0,St=!0,Ct=!1,wt=!0,V=!1,H=!0,U=!1,Tt=!1,Et=null,Dt=null,Ot=!1,W=!1,kt=!1,At=!1,jt=!0,Mt=!1,Nt=`user-content-`,Pt=!0,Ft=!1,G={},K=null,It=D({},/* @__PURE__ */ `annotation-xml.audio.colgroup.desc.foreignobject.head.iframe.math.mi.mn.mo.ms.mtext.noembed.noframes.noscript.plaintext.script.selectedcontent.style.svg.template.thead.title.video.xmp`.split(`.`)),Lt=null,Rt=D({},[`audio`,`video`,`img`,`source`,`image`,`track`]),zt=null,Bt=D({},[`alt`,`class`,`for`,`id`,`label`,`name`,`pattern`,`placeholder`,`role`,`summary`,`title`,`value`,`style`,`xmlns`]),Vt=`\u0068\u0074\u0074\u0070://www.w3.org/1998/Math/MathML`,Ht=`\u0068\u0074\u0074\u0070://www.w3.org/2000/svg`,q=`\u0068\u0074\u0074\u0070://www.w3.org/1999/xhtml`,J=q,Ut=!1,Wt=null,Gt=D({},[Vt,Ht,q],se),Kt=p([`mi`,`mo`,`mn`,`ms`,`mtext`]),qt=D({},Kt),Jt=p([`annotation-xml`]),Yt=D({},Jt),Xt=D({},[`title`,`style`,`font`,`a`,`script`]),Zt=null,Qt=[`application/xhtml+xml`,`text/html`],Y=null,$t=null,en=n.createElement(`form`),tn=function(e){return e instanceof RegExp||e instanceof Function},nn=function(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:{};if($t&&$t===e)return;(!e||typeof e!=`object`)&&(e={}),e=O(e),Zt=Qt.indexOf(e.PARSER_MEDIA_TYPE)===-1?`text/html`:e.PARSER_MEDIA_TYPE,Y=Zt===`application/xhtml+xml`?se:oe,L=M(e,`ALLOWED_TAGS`,_t,{transform:Y}),R=M(e,`ALLOWED_ATTR`,vt,{transform:Y}),Wt=M(e,`ALLOWED_NAMESPACES`,Gt,{transform:se}),zt=M(e,`ADD_URI_SAFE_ATTR`,Bt,{transform:Y,base:Bt}),Lt=M(e,`ADD_DATA_URI_TAGS`,Rt,{transform:Y,base:Rt}),K=M(e,`FORBID_CONTENTS`,It,{transform:Y}),yt=M(e,`FORBID_TAGS`,O({}),{transform:Y}),bt=M(e,`FORBID_ATTR`,O({}),{transform:Y}),G=x(e,`USE_PROFILES`)?e.USE_PROFILES&&typeof e.USE_PROFILES==`object`?O(e.USE_PROFILES):e.USE_PROFILES:!1,xt=e.ALLOW_ARIA_ATTR!==!1,St=e.ALLOW_DATA_ATTR!==!1,Ct=e.ALLOW_UNKNOWN_PROTOCOLS||!1,wt=e.ALLOW_SELF_CLOSE_IN_ATTR!==!1,V=e.SAFE_FOR_TEMPLATES||!1,H=e.SAFE_FOR_XML!==!1,U=e.WHOLE_DOCUMENT||!1,W=e.RETURN_DOM||!1,kt=e.RETURN_DOM_FRAGMENT||!1,At=e.RETURN_TRUSTED_TYPE||!1,Ot=e.FORCE_BODY||!1,jt=e.SANITIZE_DOM!==!1,Mt=e.SANITIZE_NAMED_PROPS||!1,Pt=e.KEEP_CONTENT!==!1,Ft=e.IN_PLACE||!1,gt=ge(e.ALLOWED_URI_REGEXP)?e.ALLOWED_URI_REGEXP:Ne,J=typeof e.NAMESPACE==`string`?e.NAMESPACE:q,qt=qe(e,`MATHML_TEXT_INTEGRATION_POINTS`,()=>D({},Kt)),Yt=qe(e,`HTML_INTEGRATION_POINTS`,()=>D({},Jt));let t=qe(e,`CUSTOM_ELEMENT_HANDLING`,()=>h(null));if(z=h(null),x(t,`tagNameCheck`)&&tn(t.tagNameCheck)&&(z.tagNameCheck=t.tagNameCheck),x(t,`attributeNameCheck`)&&tn(t.attributeNameCheck)&&(z.attributeNameCheck=t.attributeNameCheck),x(t,`allowCustomizedBuiltInElements`)&&typeof t.allowCustomizedBuiltInElements==`boolean`&&(z.allowCustomizedBuiltInElements=t.allowCustomizedBuiltInElements),m(z),V&&(St=!1),kt&&(W=!0),G&&(L=D({},Ce),R=h(null),G.html===!0&&(D(L,_e),D(R,we)),G.svg===!0&&(D(L,ve),D(R,Te),D(R,De)),G.svgFilters===!0&&(D(L,ye),D(R,Te),D(R,De)),G.mathMl===!0&&(D(L,xe),D(R,Ee),D(R,De))),B.tagCheck=null,B.attributeCheck=null,x(e,`ADD_TAGS`)&&(typeof e.ADD_TAGS==`function`?B.tagCheck=e.ADD_TAGS:v(e.ADD_TAGS)&&(L===_t&&(L=O(L)),D(L,e.ADD_TAGS,Y))),x(e,`ADD_ATTR`)&&(typeof e.ADD_ATTR==`function`?B.attributeCheck=e.ADD_ATTR:v(e.ADD_ATTR)&&(R===vt&&(R=O(R)),D(R,e.ADD_ATTR,Y))),x(e,`ADD_FORBID_CONTENTS`)&&v(e.ADD_FORBID_CONTENTS)&&(K===It&&(K=O(K)),D(K,e.ADD_FORBID_CONTENTS,Y)),Pt&&(L[`#text`]=!0),U&&D(L,[`html`,`head`,`body`]),L.table&&(D(L,[`tbody`]),delete yt.tbody),e.TRUSTED_TYPES_POLICY){if(typeof e.TRUSTED_TYPES_POLICY.createHTML!=`function`)throw w(`TRUSTED_TYPES_POLICY configuration option must provide a "createHTML" hook.`);if(typeof e.TRUSTED_TYPES_POLICY.createScriptURL!=`function`)throw w(`TRUSTED_TYPES_POLICY configuration option must provide a "createScriptURL" hook.`);let t=N;N=e.TRUSTED_TYPES_POLICY;try{P=F(``)}catch(e){throw N=t,e}}else e.TRUSTED_TYPES_POLICY===null?(N=void 0,P=``):(N===void 0&&(N=tt()),N&&typeof P==`string`&&(P=F(``)));p&&p(e),$t=e},rn=D({},[...ve,...ye,...be]),an=D({},[...xe,...Se]),on=function(e,t,n){return t.namespaceURI===q?e===`svg`:t.namespaceURI===Vt?e===`svg`&&(n===`annotation-xml`||qt[n]):!!rn[e]},sn=function(e,t,n){return t.namespaceURI===q?e===`math`:t.namespaceURI===Ht?e===`math`&&Yt[n]:!!an[e]},cn=function(e,t,n){return t.namespaceURI===Ht&&!Yt[n]||t.namespaceURI===Vt&&!qt[n]?!1:!an[e]&&(Xt[e]||!rn[e])},ln=function(e){let t=b(e);(!t||!t.tagName)&&(t={namespaceURI:J,tagName:`template`});let n=oe(e.tagName),r=oe(t.tagName);return Wt[e.namespaceURI]?e.namespaceURI===Ht?on(n,t,r):e.namespaceURI===Vt?sn(n,t,r):e.namespaceURI===q?cn(n,t,r):!!(Zt===`application/xhtml+xml`&&Wt[e.namespaceURI]):!1},X=function(e){ie(t.removed,{element:e});try{b(e).removeChild(e)}catch(t){if(g(e),!b(e))throw w(`a node selected for removal could not be detached from its tree and cannot be safely returned; refusing to sanitize in place`)}},un=function(e,t,n){try{te(e,t)}catch(t){try{e.removeAttribute(n)}catch(e){}}},dn=function(e){pn(e);let t=y(e);if(t){let e=[];_(t,t=>{ie(e,t)}),_(e,e=>{try{g(e)}catch(e){}})}let n=S(e);if(n)for(let t=n.length-1;t>=0;--t){let r=n[t],i=r&&r.name;typeof i==`string`&&un(e,r,i)}},Z=function(e,n,r){if(!r)try{r=n.getAttributeNode(e)}catch(e){r=null}ie(t.removed,{attribute:r||null,from:n});try{r?te(n,r):n.removeAttribute(e)}catch(t){try{n.removeAttribute(e)}catch(e){}}if(e===`is`){if(W||kt)try{X(n)}catch(e){}else try{n.setAttribute(e,``)}catch(e){}}},fn=function(e){let t=S(e);if(t)for(let n=t.length-1;n>=0;--n){let r=t[n],i=r&&r.name;typeof i!=`string`||R[Y(i)]||un(e,r,i)}},pn=function(e){let t=[e];for(;t.length>0;){let e=t.pop();j(e)===A.element&&fn(e);let n=y(e);if(n)for(let e=n.length-1;e>=0;--e)t.push(n[e])}},mn=function(e,t){return H?e===`patchsrc`||e===`for`&&t!==`label`&&t!==`output`:!1},hn=function(e){if(!H)return;let t=[e];for(;t.length>0;){let e=t.pop(),n=j(e);if(n===A.processingInstruction||n===A.comment&&C(ze,e.data)){try{g(e)}catch(e){}continue}if(n===A.element){let t=e,n=Y(Ye(e));try{t.hasAttribute&&t.hasAttribute(`patchsrc`)&&t.removeAttribute(`patchsrc`),t.hasAttribute&&t.hasAttribute(`for`)&&mn(`for`,n)&&t.removeAttribute(`for`)}catch(e){}}let r=y(e);if(r)for(let e=r.length-1;e>=0;--e)t.push(r[e])}},gn=function(e){let t=null,r=null;if(Ot)e=`<remove></remove>`+e;else{let t=ce(e,/^[\r\n\t ]+/);r=t&&t[0]}Zt===`application/xhtml+xml`&&J===q&&(e=`<html xmlns="\u0068\u0074\u0074\u0070://www.w3.org/1999/xhtml"><head></head><body>`+e+`</body></html>`);let i=N?F(e):e;if(J===q)try{t=new u().parseFromString(i,Zt)}catch(e){}if(!t||!t.documentElement){t=rt.createDocument(J,`template`,null);try{t.documentElement.innerHTML=Ut?P:i}catch(e){}}let a=t.body||t.documentElement;return e&&r&&a.insertBefore(n.createTextNode(r),a.childNodes[0]||null),J===q?ot.call(t,U?`html`:`body`)[0]:U?t.documentElement:a},_n=function(e){let t=me?me(e):e.ownerDocument;return it.call(t||e,e,l.SHOW_ELEMENT|l.SHOW_COMMENT|l.SHOW_TEXT|l.SHOW_PROCESSING_INSTRUCTION|l.SHOW_CDATA_SECTION,null)},vn=function(e){return e=le(e,ct,` `),e=le(e,lt,` `),e=le(e,ut,` `),e},yn=function(e){var t;e.normalize();let n=me?me(e):e.ownerDocument,r=it.call(n||e,e,l.SHOW_TEXT|l.SHOW_COMMENT|l.SHOW_CDATA_SECTION|l.SHOW_PROCESSING_INSTRUCTION,null),i=r.nextNode();for(;i;)i.data=vn(i.data),i=r.nextNode();let a=(t=e.querySelectorAll)==null?void 0:t.call(e,`template`);a&&_(a,e=>{xn(e.content)&&yn(e.content)})},bn=function(e){let t=E?E(e):null;return typeof t!=`string`||Y(t)!==`form`?!1:typeof e.nodeName!=`string`||typeof e.textContent!=`string`||typeof e.removeChild!=`function`||e.attributes!==S(e)||typeof e.removeAttribute!=`function`||typeof e.removeAttributeNode!=`function`||typeof e.getAttributeNode!=`function`||typeof e.setAttribute!=`function`||typeof e.namespaceURI!=`string`||typeof e.insertBefore!=`function`||typeof e.hasChildNodes!=`function`||e.nodeType!==T(e)||e.childNodes!==y(e)},xn=function(e){if(!T||typeof e!=`object`||!e)return!1;try{return T(e)===A.documentFragment}catch(e){return!1}},Sn=function(e){if(!T||typeof e!=`object`||!e)return!1;try{return typeof T(e)==`number`}catch(e){return!1}};function Q(e,n,r){e.length!==0&&_(e,e=>{e.call(t,n,r,$t)})}let Cn=function(e,t){return!!(H&&e.hasChildNodes()&&!Sn(e.firstElementChild)&&C(Re,e.textContent)&&C(Re,e.innerHTML)||H&&e.namespaceURI===q&&He[t]&&(Sn(e.firstElementChild)||typeof e.textContent==`string`&&C(Ue[t],e.textContent))||e.nodeType===A.processingInstruction||H&&e.nodeType===A.comment&&C(ze,e.data))},wn=function(e,t){return e instanceof RegExp?C(e,t):e instanceof Function&&!!e(t,...[...arguments].slice(2))},Tn=function(e,t,n){if(!yt[t]&&An(t)&&wn(z.tagNameCheck,t))return!1;if(Pt&&!K[t]){let t=b(e),r=y(e);if(r&&t){let i=r.length;for(let a=i-1;a>=0;--a){let i=e===n?ee(r[a],!0):r[a];t.insertBefore(i,fe(e))}}}return X(e),!0},En=function(e,t,n,r){return e.length===0?t:t===n||t===r?O(t):t},$=function(e,t){return e===t||b(e)!==null?!1:(Ft&&pn(e),!0)},Dn=function(e,n){if(Q(I.beforeSanitizeElements,e,null),$(e,n))return!0;if(bn(e))return X(e),!0;let r=Y(Ye(e));if(L=En(I.uponSanitizeElement,L,_t,Et),Q(I.uponSanitizeElement,e,{tagName:r,allowedTags:L}),$(e,n))return!0;if(Cn(e,r))return X(e),!0;if(yt[r]||!(B.tagCheck instanceof Function&&B.tagCheck(r))&&!L[r]){let t=Tn(e,r,n);return t===!1&&(Q(I.afterSanitizeElements,e,null),$(e,n))?!0:t}if(j(e)===A.element&&!ln(e)||(r===`noscript`||r===`noembed`||r===`noframes`)&&C(Be,e.innerHTML))return X(e),!0;if(V&&e.nodeType===A.text){let n=vn(e.textContent);e.textContent!==n&&(ie(t.removed,{element:e.cloneNode()}),e.textContent=n)}return Q(I.afterSanitizeElements,e,null),$(e,n)},On=function(e,t,r){if(bt[t]||mn(t,e)||jt&&(t===`id`||t===`name`)&&(r in n||r in en))return!1;let i=R[t]||B.attributeCheck instanceof Function&&B.attributeCheck(t,e);return St&&C(dt,t)||xt&&C(ft,t)?!0:i?zt[t]||C(gt,le(r,mt,``))||(t===`src`||t===`xlink:href`||t===`href`)&&e!==`script`&&ue(r,`data:`)===0&&Lt[e]||Ct&&!C(pt,le(r,mt,``))?!0:!r:An(e)&&wn(z.tagNameCheck,e)&&wn(z.attributeNameCheck,t,e)||t===`is`&&z.allowCustomizedBuiltInElements&&wn(z.tagNameCheck,r)},kn=D({},[`annotation-xml`,`color-profile`,`font-face`,`font-face-format`,`font-face-name`,`font-face-src`,`font-face-uri`,`missing-glyph`]),An=function(e){return!kn[oe(e)]&&C(ht,e)},jn=function(e,t,n,r){if(N&&typeof d==`object`&&typeof d.getAttributeType==`function`&&!n)switch(d.getAttributeType(e,t)){case`TrustedHTML`:return F(r);case`TrustedScriptURL`:return et(r)}return r},Mn=function(e,t,n,r){try{return n?e.setAttributeNS(n,t,r):e.setAttribute(t,r),!bn(e)||(X(e),!1)}catch(n){return Z(t,e),!1}},Nn=function(e,n){if(Q(I.beforeSanitizeAttributes,e,null),$(e,n))return;let r=e.attributes;if(!r||bn(e))return;R=En(I.uponSanitizeAttribute,R,vt,Dt);let i={attrName:``,attrValue:``,keepAttr:!0,allowedAttributes:R,forceKeepAttr:void 0},a=r.length,o=Y(e.nodeName);for(;a--;){let n=r[a],s=n.name,c=n.namespaceURI,l=n.value,u=Y(s),d=l,f=s===`value`?d:de(d),p=!1;if(i.attrName=u,i.attrValue=f,i.keepAttr=!0,i.forceKeepAttr=void 0,Q(I.uponSanitizeAttribute,e,i),f=i.attrValue,Mt&&(u===`id`||u===`name`)&&ue(f,Nt)!==0&&(Z(s,e,n),f=Nt+f,p=!0),H&&C(/((--!?|])>)|<\/(style|script|title|xmp|textarea|noscript|iframe|noembed|noframes)/i,f)){Z(s,e,n);continue}if(u===`attributename`&&ce(f,`href`)){Z(s,e,n);continue}if(!i.forceKeepAttr){if(!i.keepAttr){Z(s,e,n);continue}if(!wt&&C(Ve,f)){Z(s,e,n);continue}if(V&&(f=vn(f)),!On(o,u,f)){Z(s,e,n);continue}f=jn(o,u,c,f),f!==d&&Mn(e,s,c,f)&&p&&re(t.removed)}}Q(I.afterSanitizeAttributes,e,null),$(e,n)},Pn=function(e){let t=null,n=_n(e);for(Q(I.beforeSanitizeShadowDOM,e,null);t=n.nextNode();)if(Q(I.uponSanitizeShadowNode,t,null),Dn(t,e),Nn(t,e),xn(t.content)&&Pn(t.content),j(t)===A.element){let e=pe(t);xn(e)&&(Fn(e),Pn(e))}Q(I.afterSanitizeShadowDOM,e,null)},Fn=function(e){let t=[{node:e,shadow:null}];for(;t.length>0;){let e=t.pop();if(e.shadow){Pn(e.shadow);continue}let n=e.node,r=j(n)===A.element,i=y(n);if(i)for(let e=i.length-1;e>=0;--e)t.push({node:i[e],shadow:null});if(r){let e=E?E(n):null;if(typeof e==`string`&&Y(e)===`template`){let e=n.content;xn(e)&&t.push({node:e,shadow:null})}}if(r){let e=pe(n);xn(e)&&t.push({node:null,shadow:e},{node:e,shadow:null})}}};return t.sanitize=function(e){let n=arguments.length>1&&arguments[1]!==void 0?arguments[1]:{},i=null,a=null,o=null,s=null;if(Ut=!e,Ut&&(e=`<!-->`),typeof e!=`string`&&!Sn(e)&&(e=he(e),typeof e!=`string`))throw w(`dirty is not a string, aborting`);if(!t.isSupported)return e;Tt?(L=Et,R=Dt):nn(n),(I.uponSanitizeElement.length>0||I.uponSanitizeAttribute.length>0)&&(L=O(L)),I.uponSanitizeAttribute.length>0&&(R=O(R)),t.removed=[];let c=Ft&&typeof e!=`string`&&Sn(e);if(c){hn(e);let t=Ye(e);if(typeof t==`string`){let n=Y(t);if(!L[n]||yt[n])throw dn(e),w(`root node is forbidden and cannot be sanitized in-place`)}if(bn(e))throw dn(e),w(`root node is clobbered and cannot be sanitized in-place`);try{Fn(e)}catch(t){throw dn(e),t}}else if(Sn(e))i=gn(`<!---->`),a=i.ownerDocument.importNode(e,!0),a.nodeType===A.element&&a.nodeName===`BODY`||a.nodeName===`HTML`?i=a:i.appendChild(a),Fn(i);else{if(!W&&!V&&!U&&e.indexOf(`<`)===-1)return N&&At?F(e):e;if(i=gn(e),!i)return W?null:At?P:``}i&&Ot&&X(i.firstChild);let l=c?e:i;try{let e=_n(l);for(;o=e.nextNode();)Dn(o,l),Nn(o,l),xn(o.content)&&Pn(o.content)}catch(n){throw c&&(dn(e),_(t.removed,e=>{e.element&&pn(e.element)})),n}if(c){let n=!1;if(_(t.removed,t=>{t.element&&(t.element===e&&(n=!0),pn(t.element))}),n)throw w(`a node selected for removal could not be safely returned; refusing to sanitize in place`);return V&&yn(e),e}if(W){if(V&&yn(i),kt)for(s=at.call(i.ownerDocument);i.firstChild;)s.appendChild(i.firstChild);else s=i;return(R.shadowroot||R.shadowrootmode)&&(s=st.call(r,s,!0)),s}let u=U?i.outerHTML:i.innerHTML;return U&&L[`!doctype`]&&i.ownerDocument&&i.ownerDocument.doctype&&i.ownerDocument.doctype.name&&C(Ie,i.ownerDocument.doctype.name)&&(u=`<!DOCTYPE `+i.ownerDocument.doctype.name+`>
`+u),V&&(u=vn(u)),N&&At?F(u):u},t.setConfig=function(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:{};nn(e),Tt=!0,Et=L,Dt=R},t.clearConfig=function(){$t=null,Tt=!1,Et=null,Dt=null,N=Xe,P=``},t.isValidAttribute=function(e,t,n){$t||nn({});let r=Y(e),i=Y(t);return On(r,i,n)},t.addHook=function(e,t){typeof t==`function`&&x(I,e)&&ie(I[e],t)},t.removeHook=function(e,t){if(x(I,e)){if(t!==void 0){let n=ne(I[e],t);return n===-1?void 0:ae(I[e],n,1)[0]}return re(I[e])}},t.removeHooks=function(e){x(I,e)&&(I[e]=[])},t.removeAllHooks=function(){I=Ke()},t}return Je()});
//# sourceMappingURL=purify.min.js.map

/* === highlight.min_11.12.0.js === */
/*!
  Highlight.js v11.12.0 (git: f7f7d3803b)
  (c) 2006-2026 Josh Goebel <hello@joshgoebel.com> and other contributors
  License: BSD-3-Clause
 */
var hljs=function(){"use strict";function e(n){
return n instanceof Map?n.clear=n.delete=n.set=()=>{
throw Error("map is read-only")}:n instanceof Set&&(n.add=n.clear=n.delete=()=>{
throw Error("set is read-only")
}),Object.freeze(n),Object.getOwnPropertyNames(n).forEach(t=>{
const a=n[t],i=typeof a;"object"!==i&&"function"!==i||Object.isFrozen(a)||e(a)
}),n}class n{constructor(e){
void 0===e.data&&(e.data={}),this.data=e.data,this.isMatchIgnored=!1}
ignoreMatch(){this.isMatchIgnored=!0}}function t(e){
return e.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/"/g,"&quot;").replace(/'/g,"&#x27;")
}function a(e,...n){const t=Object.create(null);for(const n in e)t[n]=e[n]
;return n.forEach(e=>{for(const n in e)t[n]=e[n]}),t}const i=e=>!!e.scope
;class r{constructor(e,n){
this.buffer="",this.classPrefix=n.classPrefix,e.walk(this)}addText(e){
this.buffer+=t(e)}openNode(e){if(!i(e))return;const n=((e,{prefix:n})=>{
if(e.startsWith("language:"))return e.replace("language:","language-")
;if(e.includes(".")){const t=e.split(".")
;return[`${n}${t.shift()}`,...t.map((e,n)=>`${e}${"_".repeat(n+1)}`)].join(" ")}
return`${n}${e}`})(e.scope,{prefix:this.classPrefix});this.span(n)}closeNode(e){
i(e)&&(this.buffer+="</span>")}value(){return this.buffer}span(e){
this.buffer+=`<span class="${e}">`}}const s=(e={})=>{const n={children:[]}
;return Object.assign(n,e),n};class o{constructor(){
this.rootNode=s(),this.stack=[this.rootNode]}get top(){
return this.stack[this.stack.length-1]}get root(){return this.rootNode}add(e){
this.top.children.push(e)}openNode(e){const n=s({scope:e})
;this.add(n),this.stack.push(n)}closeNode(){
if(this.stack.length>1)return this.stack.pop()}closeAllNodes(){
for(;this.closeNode(););}toJSON(){return JSON.stringify(this.rootNode,null,4)}
walk(e){return this.constructor._walk(e,this.rootNode)}static _walk(e,n){
return"string"==typeof n?e.addText(n):n.children&&(e.openNode(n),
n.children.forEach(n=>this._walk(e,n)),e.closeNode(n)),e}static _collapse(e){
"string"!=typeof e&&e.children&&(e.children.every(e=>"string"==typeof e)?e.children=[e.children.join("")]:e.children.forEach(e=>{
o._collapse(e)}))}}class c extends o{constructor(e){super(),this.options=e}
addText(e){""!==e&&this.add(e)}startScope(e){this.openNode(e)}endScope(){
this.closeNode()}__addSublanguage(e,n){const t=e.root
;n&&(t.scope="language:"+n),this.add(t)}toHTML(){
return new r(this,this.options).value()}finalize(){
return this.closeAllNodes(),!0}}function l(e){
return e?"string"==typeof e?e:e.source:null}function d(e){return b("(?=",e,")")}
function g(e){return b("(?:",e,")*")}function u(e){return b("(?:",e,")?")}
function b(...e){return e.map(e=>l(e)).join("")}function m(...e){const n=(e=>{
const n=e[e.length-1]
;return"object"==typeof n&&n.constructor===Object?(e.splice(e.length-1,1),n):{}
})(e);return"("+(n.capture?"":"?:")+e.map(e=>l(e)).join("|")+")"}function p(e){
return RegExp(e.toString()+"|").exec("").length-1}
const _=RegExp(m(/\[(?:[^\\\]]|\\.)*\]/,/\(\?<(?![=!])[^>]+>/,/\(\?'[^']+'/,/\(\??/,/\\([1-9][0-9]*)/,/\\./))
;function h(e,{joinWith:n}){let t=0;return e.map(e=>{t+=1;const n=t
;let a=l(e),i="";for(;a.length>0;){const e=_.exec(a);if(!e){i+=a;break}
i+=a.substring(0,e.index),
a=a.substring(e.index+e[0].length),"\\"===e[0][0]&&e[1]?i+="\\"+(Number(e[1])+n):(i+=e[0],
("("===e[0]||/^\(\?[<']/.test(e[0]))&&t++)}return i}).map(e=>`(${e})`).join(n)}
const f="[a-zA-Z]\\w*",E="[a-zA-Z_]\\w*",y="\\b\\d+(\\.\\d+)?",w="(-?)(\\b0[xX][a-fA-F0-9]+|(\\b\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?)",v="\\b(0b[01]+)",N={
begin:"\\\\[\\s\\S]",relevance:0},k={scope:"string",begin:"'",end:"'",
illegal:"\\n",contains:[N]},x={scope:"string",begin:'"',end:'"',illegal:"\\n",
contains:[N]},O=(e,n,t={})=>{const i=a({scope:"comment",begin:e,end:n,
contains:[]},t);i.contains.push({scope:"doctag",
begin:"[ ]*(?=(TODO|FIXME|NOTE|BUG|OPTIMIZE|HACK|XXX):)",
end:/(TODO|FIXME|NOTE|BUG|OPTIMIZE|HACK|XXX):/,excludeBegin:!0,relevance:0})
;const r=m("I","a","is","so","us","to","at","if","in","it","on",/[A-Za-z]+['](d|ve|re|ll|t|s|n)/,/[A-Za-z]+[-][a-z]+/,/[A-Za-z][a-z]{2,}/)
;return i.contains.push({begin:b(/[ ]+/,"(",r,/[.]?[:]?([.][ ]|[ ])/,"){3}")}),i
},M=O("//","$"),A=O("/\\*","\\*/"),S=O("#","$");var C=Object.freeze({
__proto__:null,APOS_STRING_MODE:k,BACKSLASH_ESCAPE:N,BINARY_NUMBER_MODE:{
scope:"number",begin:v,relevance:0},BINARY_NUMBER_RE:v,COMMENT:O,
C_BLOCK_COMMENT_MODE:A,C_LINE_COMMENT_MODE:M,C_NUMBER_MODE:{scope:"number",
begin:w,relevance:0},C_NUMBER_RE:w,END_SAME_AS_BEGIN:e=>Object.assign(e,{
"on:begin":(e,n)=>{n.data._beginMatch=e[1]},"on:end":(e,n)=>{
n.data._beginMatch!==e[1]&&n.ignoreMatch()}}),HASH_COMMENT_MODE:S,IDENT_RE:f,
MATCH_NOTHING_RE:/\b\B/,METHOD_GUARD:{begin:"\\.\\s*"+E,relevance:0},
NUMBER_MODE:{scope:"number",begin:y,relevance:0},NUMBER_RE:y,
PHRASAL_WORDS_MODE:{
begin:/\b(a|an|the|are|I'm|isn't|don't|doesn't|won't|but|just|should|pretty|simply|enough|gonna|going|wtf|so|such|will|you|your|they|like|more)\b/
},QUOTE_STRING_MODE:x,REGEXP_MODE:{scope:"regexp",begin:/\/(?=[^/\n]*\/)/,
end:/\/[gimuy]*/,contains:[N,{begin:/\[/,end:/\]/,relevance:0,contains:[N]}]},
RE_STARTERS_RE:"!|!=|!==|%|%=|&|&&|&=|\\*|\\*=|\\+|\\+=|,|-|-=|/=|/|:|;|<<|<<=|<=|<|===|==|=|>>>=|>>=|>=|>>>|>>|>|\\?|\\[|\\{|\\(|\\^|\\^=|\\||\\|=|\\|\\||~",
SHEBANG:(e={})=>{const n=/^#![ ]*\//
;return e.binary&&(e.begin=b(n,/.*\b/,e.binary,/\b.*/)),a({scope:"meta",begin:n,
end:/$/,relevance:0,"on:begin":(e,n)=>{0!==e.index&&n.ignoreMatch()}},e)},
TITLE_MODE:{scope:"title",begin:f,relevance:0},UNDERSCORE_IDENT_RE:E,
UNDERSCORE_TITLE_MODE:{scope:"title",begin:E,relevance:0}});function T(e,n){
"."===e.input[e.index-1]&&n.ignoreMatch()}function R(e,n){
void 0!==e.className&&(e.scope=e.className,delete e.className)}function D(e,n){
n&&e.beginKeywords&&(e.begin="\\b("+e.beginKeywords.split(" ").join("|")+")(?!\\.)(?=\\b|\\s)",
e.__beforeBegin=T,e.keywords=e.keywords||e.beginKeywords,delete e.beginKeywords,
void 0===e.relevance&&(e.relevance=0))}function I(e,n){
Array.isArray(e.illegal)&&(e.illegal=m(...e.illegal))}function L(e,n){
if(e.match){
if(e.begin||e.end)throw Error("begin & end are not supported with match")
;e.begin=e.match,delete e.match}}function B(e,n){
void 0===e.relevance&&(e.relevance=1)}const F=(e,n)=>{if(!e.beforeMatch)return
;if(e.starts)throw Error("beforeMatch cannot be used with starts")
;const t=Object.assign({},e);Object.keys(e).forEach(n=>{delete e[n]
}),e.keywords=t.keywords,e.begin=b(t.beforeMatch,d(t.begin)),e.starts={
relevance:0,contains:[Object.assign(t,{endsParent:!0})]
},e.relevance=0,delete t.beforeMatch
},$=["of","and","for","in","not","or","if","then","parent","list","value"]
;function z(e,n,t="keyword"){const a=Object.create(null)
;return"string"==typeof e?i(t,e.split(" ")):Array.isArray(e)?i(t,e):Object.keys(e).forEach(t=>{
Object.assign(a,z(e[t],n,t))}),a;function i(e,t){
n&&(t=t.map(e=>e.toLowerCase())),t.forEach(n=>{const t=n.split("|")
;a[t[0]]=[e,j(t[0],t[1])]})}}function j(e,n){
return n?Number(n):(e=>$.includes(e.toLowerCase()))(e)?0:1}const U={},P=e=>{
console.error(e)},K=(e,...n)=>{console.log("WARN: "+e,...n)},H=(e,n)=>{
U[`${e}/${n}`]||(console.log(`Deprecated as of ${e}. ${n}`),U[`${e}/${n}`]=!0)
},q=Error();function G(e,n,{key:t}){let a=0;const i=e[t],r={},s={}
;for(let e=1;e<=n.length;e++)s[e+a]=i[e],r[e+a]=!0,a+=p(n[e-1])
;e[t]=s,e[t]._emit=r,e[t]._multi=!0}function Z(e){(e=>{
e.scope&&"object"==typeof e.scope&&null!==e.scope&&(e.beginScope=e.scope,
delete e.scope)})(e),"string"==typeof e.beginScope&&(e.beginScope={
_wrap:e.beginScope}),"string"==typeof e.endScope&&(e.endScope={_wrap:e.endScope
}),(e=>{if(Array.isArray(e.begin)){
if(e.skip||e.excludeBegin||e.returnBegin)throw P("skip, excludeBegin, returnBegin not compatible with beginScope: {}"),
q
;if("object"!=typeof e.beginScope||null===e.beginScope)throw P("beginScope must be object"),
q;G(e,e.begin,{key:"beginScope"}),e.begin=h(e.begin,{joinWith:""})}})(e),(e=>{
if(Array.isArray(e.end)){
if(e.skip||e.excludeEnd||e.returnEnd)throw P("skip, excludeEnd, returnEnd not compatible with endScope: {}"),
q
;if("object"!=typeof e.endScope||null===e.endScope)throw P("endScope must be object"),
q;G(e,e.end,{key:"endScope"}),e.end=h(e.end,{joinWith:""})}})(e)}function W(e){
function n(n,t){
return RegExp(l(n),"m"+(e.case_insensitive?"i":"")+(e.unicodeRegex?"u":"")+(t?"g":""))
}class t{constructor(){
this.matchIndexes={},this.regexes=[],this.matchAt=1,this.position=0}
addRule(e,n){
n.position=this.position++,this.matchIndexes[this.matchAt]=n,this.regexes.push([n,e]),
this.matchAt+=p(e)+1}compile(){0===this.regexes.length&&(this.exec=()=>null)
;const e=this.regexes.map(e=>e[1]);this.matcherRe=n(h(e,{joinWith:"|"
}),!0),this.lastIndex=0}exec(e){this.matcherRe.lastIndex=this.lastIndex
;const n=this.matcherRe.exec(e);if(!n)return null
;const t=n.findIndex((e,n)=>n>0&&void 0!==e),a=this.matchIndexes[t]
;return n.splice(0,t),Object.assign(n,a)}}class i{constructor(){
this.rules=[],this.multiRegexes=[],
this.count=0,this.lastIndex=0,this.regexIndex=0}getMatcher(e){
if(this.multiRegexes[e])return this.multiRegexes[e];const n=new t
;return this.rules.slice(e).forEach(([e,t])=>n.addRule(e,t)),
n.compile(),this.multiRegexes[e]=n,n}resumingScanAtSamePosition(){
return 0!==this.regexIndex}considerAll(){this.regexIndex=0}addRule(e,n){
this.rules.push([e,n]),"begin"===n.type&&this.count++}exec(e){
const n=this.getMatcher(this.regexIndex);n.lastIndex=this.lastIndex
;let t=n.exec(e)
;if(this.resumingScanAtSamePosition())if(t&&t.index===this.lastIndex);else{
const n=this.getMatcher(0);n.lastIndex=this.lastIndex+1,t=n.exec(e)}
return t&&(this.regexIndex+=t.position+1,
this.regexIndex===this.count&&this.considerAll()),t}}
if(e.compilerExtensions||(e.compilerExtensions=[]),
e.contains&&e.contains.includes("self"))throw Error("ERR: contains `self` is not supported at the top-level of a language.  See documentation.")
;return e.classNameAliases=a(e.classNameAliases||{}),function t(r,s){const o=r
;if(r.isCompiled)return o
;[R,L,Z,F].forEach(e=>e(r,s)),e.compilerExtensions.forEach(e=>e(r,s)),
r.__beforeBegin=null,[D,I,B].forEach(e=>e(r,s)),r.isCompiled=!0;let c=null
;return"object"==typeof r.keywords&&r.keywords.$pattern&&(r.keywords=Object.assign({},r.keywords),
c=r.keywords.$pattern,
delete r.keywords.$pattern),c=c||/\w+/,r.keywords&&(r.keywords=z(r.keywords,e.case_insensitive)),
o.keywordPatternRe=n(c,!0),
s&&(r.begin||(r.begin=/\B|\b/),o.beginRe=n(o.begin),r.end||r.endsWithParent||(r.end=/\B|\b/),
r.end&&(o.endRe=n(o.end)),
o.terminatorEnd=l(o.end)||"",r.endsWithParent&&s.terminatorEnd&&(o.terminatorEnd+=(r.end?"|":"")+s.terminatorEnd)),
r.illegal&&(o.illegalRe=n(r.illegal)),
r.contains||(r.contains=[]),r.contains=[].concat(...r.contains.map(e=>(e=>(e.variants&&!e.cachedVariants&&(e.cachedVariants=e.variants.map(n=>a(e,{
variants:null},n))),e.cachedVariants?e.cachedVariants:Q(e)?a(e,{
starts:e.starts?a(e.starts):null
}):Object.isFrozen(e)?a(e):e))("self"===e?r:e))),r.contains.forEach(e=>{t(e,o)
}),r.starts&&t(r.starts,s),o.matcher=(e=>{const n=new i
;return e.contains.forEach(e=>n.addRule(e.begin,{rule:e,type:"begin"
})),e.terminatorEnd&&n.addRule(e.terminatorEnd,{type:"end"
}),e.illegal&&n.addRule(e.illegal,{type:"illegal"}),n})(o),o}(e)}function Q(e){
return!!e&&(e.endsWithParent||Q(e.starts))}class X extends Error{
constructor(e,n){super(e),this.name="HTMLInjectionError",this.html=n}}
const V=t,J=a,Y=Symbol("nomatch"),ee=t=>{
const a=Object.create(null),i=Object.create(null),r=[];let s=!0
;const o="Could not find the language '{}', did you forget to load/include a language module?",l={
disableAutodetect:!0,name:"Plain text",contains:[]};let p={
ignoreUnescapedHTML:!1,throwUnescapedHTML:!1,noHighlightRe:/^(no-?highlight)$/i,
languageDetectRe:/\blang(?:uage)?-([\w-]+)\b/i,classPrefix:"hljs-",
cssSelector:"pre code",languages:null,__emitter:c};function _(e){
return p.noHighlightRe.test(e)}function h(e,n,t){let a="",i=""
;"object"==typeof n?(a=e,
t=n.ignoreIllegals,i=n.language):(H("10.7.0","highlight(lang, code, ...args) has been deprecated."),
H("10.7.0","Please use highlight(code, options) instead.\nhttps://github.com/highlightjs/highlight.js/issues/2277"),
i=e,a=n),void 0===t&&(t=!0);const r={code:a,language:i};O("before:highlight",r)
;const s=r.result?r.result:f(r.language,r.code,t)
;return s.code=r.code,O("after:highlight",s),s}function f(e,t,i,r){
const c=Object.create(null);function l(e,n){return e.keywords[n]}function d(){
if(!M.keywords)return void S.addText(C);let e=0;M.keywordPatternRe.lastIndex=0
;let n=M.keywordPatternRe.exec(C),t="";for(;n;){t+=C.substring(e,n.index)
;const a=k.case_insensitive?n[0].toLowerCase():n[0],i=l(M,a);if(i){const[e,r]=i
;if(S.addText(t),
t="",c[a]=(c[a]||0)+1,c[a]<=7&&(T+=r),e.startsWith("_"))t+=n[0];else{
const t=k.classNameAliases[e]||e;u(n[0],t)}}else t+=n[0]
;e=M.keywordPatternRe.lastIndex,n=M.keywordPatternRe.exec(C)}
t+=C.substring(e),S.addText(t)}function g(){null!=M.subLanguage?(()=>{
if(""===C)return;let e=null;if("string"==typeof M.subLanguage){
if(!a[M.subLanguage])return void S.addText(C)
;e=f(M.subLanguage,C,!0,A[M.subLanguage]),A[M.subLanguage]=e._top
}else e=E(C,M.subLanguage.length?M.subLanguage:null)
;M.relevance>0&&(T+=e.relevance),S.__addSublanguage(e._emitter,e.language)
})():d(),C=""}function u(e,n){
""!==e&&(S.startScope(n),S.addText(e),S.endScope())}function b(e,n){let t=1
;const a=n.length-1;for(;t<=a;){if(!e._emit[t]){t++;continue}
const a=k.classNameAliases[e[t]]||e[t],i=n[t];a?u(i,a):(C=i,d(),C=""),t++}}
function m(e,n){
return e.scope&&"string"==typeof e.scope&&S.openNode(k.classNameAliases[e.scope]||e.scope),
e.beginScope&&(e.beginScope._wrap?(u(C,k.classNameAliases[e.beginScope._wrap]||e.beginScope._wrap),
C=""):e.beginScope._multi&&(b(e.beginScope,n),C="")),M=Object.create(e,{parent:{
value:M}}),M}function _(e,t,a){let i=((e,n)=>{const t=e&&e.exec(n)
;return t&&0===t.index})(e.endRe,a);if(i){if(e["on:end"]){const a=new n(e)
;e["on:end"](t,a),a.isMatchIgnored&&(i=!1)}if(i){
for(;e.endsParent&&e.parent;)e=e.parent;return e}}
if(e.endsWithParent)return _(e.parent,t,a)}function h(e){
return 0===M.matcher.regexIndex?(C+=e[0],1):(I=!0,0)}function y(e){
const n=e[0],a=t.substring(e.index),i=_(M,e,a);if(!i)return Y;const r=M
;M.endScope&&M.endScope._wrap?(g(),
u(n,M.endScope._wrap)):M.endScope&&M.endScope._multi?(g(),
b(M.endScope,e)):r.skip?C+=n:(r.returnEnd||r.excludeEnd||(C+=n),
g(),r.excludeEnd&&(C=n));do{
M.scope&&S.closeNode(),M.skip||M.subLanguage||(T+=M.relevance),M=M.parent
}while(M!==i.parent);return i.starts&&m(i.starts,e),r.returnEnd?0:n.length}
let w={};function v(a,r){const o=r&&r[0];if(C+=a,null==o)return g(),0
;if("begin"===w.type&&"end"===r.type&&w.index===r.index&&""===o){
if(C+=t.slice(r.index,r.index+1),!s){const n=Error(`0 width match regex (${e})`)
;throw n.languageName=e,n.badRule=w.rule,n}return 1}
if(w=r,"begin"===r.type)return(e=>{
const t=e[0],a=e.rule,i=new n(a),r=[a.__beforeBegin,a["on:begin"]]
;for(const n of r)if(n&&(n(e,i),i.isMatchIgnored))return h(t)
;return a.skip?C+=t:(a.excludeBegin&&(C+=t),
g(),a.returnBegin||a.excludeBegin||(C=t)),m(a,e),a.returnBegin?0:t.length})(r)
;if("illegal"===r.type&&!i){
const e=Error('Illegal lexeme "'+o+'" for mode "'+(M.scope||"<unnamed>")+'"')
;throw e.mode=M,e}if("end"===r.type){const e=y(r);if(e!==Y)return e}
if("illegal"===r.type&&""===o)return r.index===t.length||(C+="\n"),1
;if(D>1e5&&D>3*r.index)throw Error("potential infinite loop, way more iterations than matches")
;return C+=o,o.length}const k=N(e)
;if(!k)throw P(o.replace("{}",e)),Error('Unknown language: "'+e+'"')
;const x=W(k);let O="",M=r||x;const A={},S=new p.__emitter(p);(()=>{const e=[]
;for(let n=M;n!==k;n=n.parent)n.scope&&e.unshift(n.scope)
;e.forEach(e=>S.openNode(e))})();let C="",T=0,R=0,D=0,I=!1;try{
if(k.__emitTokens)k.__emitTokens(t,S);else{for(M.matcher.considerAll();;){
D++,I?I=!1:M.matcher.considerAll(),M.matcher.lastIndex=R
;const e=M.matcher.exec(t);if(!e)break;const n=v(t.substring(R,e.index),e)
;R=e.index+n}v(t.substring(R))}return S.finalize(),O=S.toHTML(),{language:e,
value:O,relevance:T,illegal:!1,_emitter:S,_top:M}}catch(n){
if(n.message&&n.message.includes("Illegal"))return{language:e,value:V(t),
illegal:!0,relevance:0,_illegalBy:{message:n.message,index:R,
context:t.slice(R-100,R+100),mode:n.mode,resultSoFar:O},_emitter:S};if(s)return{
language:e,value:V(t),illegal:!1,relevance:0,errorRaised:n,_emitter:S,_top:M}
;throw n}}function E(e,n){n=n||p.languages||Object.keys(a);const t=(e=>{
const n={value:V(e),illegal:!1,relevance:0,_top:l,_emitter:new p.__emitter(p)}
;return n._emitter.addText(e),n})(e),i=n.filter(N).filter(x).map(n=>f(n,e,!1))
;i.unshift(t);const r=i.sort((e,n)=>{
if(e.relevance!==n.relevance)return n.relevance-e.relevance
;if(e.language&&n.language){if(N(e.language).supersetOf===n.language)return 1
;if(N(n.language).supersetOf===e.language)return-1}return 0}),[s,o]=r,c=s
;return c.secondBest=o,c}function y(e){let n=null;const t=(e=>{
let n=e.className+" ";n+=e.parentNode?e.parentNode.className:""
;const t=p.languageDetectRe.exec(n);if(t){const n=N(t[1])
;return n||(K(o.replace("{}",t[1])),
K("Falling back to no-highlight mode for this block.",e)),n?t[1]:"no-highlight"}
return n.split(/\s+/).find(e=>_(e)||N(e))})(e);if(_(t))return
;if(O("before:highlightElement",{el:e,language:t
}),e.dataset.highlighted)return void console.log("Element previously highlighted. To highlight again, first unset `dataset.highlighted`.",e)
;if(e.children.length>0&&(p.ignoreUnescapedHTML||(console.warn("One of your code blocks includes unescaped HTML. This is a potentially serious security risk."),
console.warn("https://github.com/highlightjs/highlight.js/wiki/security"),
console.warn("The element with unescaped HTML:"),
console.warn(e)),p.throwUnescapedHTML))throw new X("One of your code blocks includes unescaped HTML.",e.innerHTML)
;n=e;const a=n.textContent,r=t?h(a,{language:t,ignoreIllegals:!0}):E(a)
;e.innerHTML=r.value,e.dataset.highlighted="yes",((e,n,t)=>{const a=n&&i[n]||t
;e.classList.add("hljs"),e.classList.add("language-"+a)
})(e,t,r.language),e.result={language:r.language,re:r.relevance,
relevance:r.relevance},r.secondBest&&(e.secondBest={
language:r.secondBest.language,relevance:r.secondBest.relevance
}),O("after:highlightElement",{el:e,result:r,text:a})}let w=!1;function v(){
if("loading"===document.readyState)return w||window.addEventListener("DOMContentLoaded",()=>{
v()},!1),void(w=!0);document.querySelectorAll(p.cssSelector).forEach(y)}
function N(e){return e=(e||"").toLowerCase(),a[e]||a[i[e]]}
function k(e,{languageName:n}){"string"==typeof e&&(e=[e]),e.forEach(e=>{
i[e.toLowerCase()]=n})}function x(e){const n=N(e);return n&&!n.disableAutodetect
}function O(e,n){const t=e;r.forEach(e=>{e[t]&&e[t](n)})}Object.assign(t,{
highlight:h,highlightAuto:E,highlightAll:v,highlightElement:y,
highlightBlock:e=>(H("10.7.0","highlightBlock will be removed entirely in v12.0"),
H("10.7.0","Please use highlightElement now."),y(e)),configure:e=>{p=J(p,e)},
initHighlighting:()=>{
v(),H("10.6.0","initHighlighting() deprecated.  Use highlightAll() now.")},
initHighlightingOnLoad:()=>{
v(),H("10.6.0","initHighlightingOnLoad() deprecated.  Use highlightAll() now.")
},registerLanguage:(e,n)=>{let i=null;try{i=n(t)}catch(n){
if(P("Language definition for '{}' could not be registered.".replace("{}",e)),
!s)throw n;P(n),i=l}
i.name||(i.name=e),a[e]=i,i.rawDefinition=n.bind(null,t),i.aliases&&k(i.aliases,{
languageName:e})},unregisterLanguage:e=>{delete a[e]
;for(const n of Object.keys(i))i[n]===e&&delete i[n]},
listLanguages:()=>Object.keys(a),getLanguage:N,registerAliases:k,
autoDetection:x,inherit:J,addPlugin:e=>{(e=>{
e["before:highlightBlock"]&&!e["before:highlightElement"]&&(e["before:highlightElement"]=n=>{
e["before:highlightBlock"](Object.assign({block:n.el},n))
}),e["after:highlightBlock"]&&!e["after:highlightElement"]&&(e["after:highlightElement"]=n=>{
e["after:highlightBlock"](Object.assign({block:n.el},n))})})(e),r.push(e)},
removePlugin:e=>{const n=r.indexOf(e);-1!==n&&r.splice(n,1)}}),t.debugMode=()=>{
s=!1},t.safeMode=()=>{s=!0},t.versionString="11.12.0",t.regex={concat:b,
lookahead:d,either:m,optional:u,anyNumberOfTimes:g}
;for(const n in C)"object"==typeof C[n]&&e(C[n]);return Object.assign(t,C),t
},ne=ee({});ne.newInstance=()=>ee({});const te="[A-Za-z$_][0-9A-Za-z$_]*",ae={
scope:"number",
match:"([-+]?)(\\b0[xX][a-fA-F0-9]+|(\\b\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?)|NaN|[-+]?Infinity",
relevance:0
},ie=["as","in","of","if","for","while","finally","var","new","function","do","return","void","else","break","catch","instanceof","with","throw","case","default","try","switch","continue","typeof","delete","let","yield","const","class","debugger","async","await","static","import","from","export","extends","using"],re=["true","false","null","undefined","NaN","Infinity"],se=["Object","Function","Boolean","Symbol","Math","Date","Number","BigInt","String","RegExp","Array","Float32Array","Float64Array","Int8Array","Uint8Array","Uint8ClampedArray","Int16Array","Int32Array","Uint16Array","Uint32Array","BigInt64Array","BigUint64Array","Set","Map","WeakSet","WeakMap","ArrayBuffer","SharedArrayBuffer","Atomics","DataView","JSON","Promise","Generator","GeneratorFunction","AsyncFunction","Reflect","Proxy","Intl","WebAssembly"],oe=["Error","EvalError","InternalError","RangeError","ReferenceError","SyntaxError","TypeError","URIError"],ce=["setInterval","setTimeout","clearInterval","clearTimeout","require","exports","eval","isFinite","isNaN","parseFloat","parseInt","decodeURI","decodeURIComponent","encodeURI","encodeURIComponent","escape","unescape"],le=["arguments","this","super","console","window","document","localStorage","sessionStorage","module","self","global"],de=[].concat(ce,se,oe)
;function ge(e){const n=e.regex,t=te,a={begin:/<[A-Za-z0-9\\._:-]+/,
end:/\/[A-Za-z0-9\\._:-]+>|\/>/,isTrulyOpeningTag:(e,n)=>{
const t=e[0].length+e.index,a=e.input[t]
;if("<"===a||","===a)return void n.ignoreMatch();let i
;">"===a&&(((e,{after:n})=>{const t="</"+e[0].slice(1)
;return-1!==e.input.indexOf(t,n)})(e,{after:t})||n.ignoreMatch())
;const r=e.input.substring(t)
;((i=r.match(/^\s*=/))||(i=r.match(/^\s+extends\s+/))&&0===i.index)&&n.ignoreMatch()
}},i={$pattern:te,keyword:ie,literal:re,built_in:de,"variable.language":le
},r="[0-9](_?[0-9])*",s=`\\.(${r})`,o="0|[1-9](_?[0-9])*|0[0-7]*[89][0-9]*",c={
className:"number",variants:[{
begin:`(\\b(${o})((${s})|\\.)?|(${s}))[eE][+-]?(${r})\\b`},{
begin:`\\b(${o})\\b((${s})\\b|\\.)?|(${s})\\b`},{
begin:"\\b(0|[1-9](_?[0-9])*)n\\b"},{
begin:"\\b0[xX][0-9a-fA-F](_?[0-9a-fA-F])*n?\\b"},{
begin:"\\b0[bB][0-1](_?[0-1])*n?\\b"},{begin:"\\b0[oO][0-7](_?[0-7])*n?\\b"},{
begin:"\\b0[0-7]+n?\\b"}],relevance:0},l={className:"subst",begin:"\\$\\{",
end:"\\}",keywords:i,contains:[]},d={begin:".?html`",end:"",starts:{end:"`",
returnEnd:!1,contains:[e.BACKSLASH_ESCAPE,l],subLanguage:"xml"}},g={
begin:".?css`",end:"",starts:{end:"`",returnEnd:!1,
contains:[e.BACKSLASH_ESCAPE,l],subLanguage:"css"}},u={begin:".?gql`",end:"",
starts:{end:"`",returnEnd:!1,contains:[e.BACKSLASH_ESCAPE,l],
subLanguage:"graphql"}},b={className:"string",begin:"`",end:"`",
contains:[e.BACKSLASH_ESCAPE,l]},m={className:"comment",
variants:[e.COMMENT(/\/\*\*(?!\/)/,"\\*/",{relevance:0,contains:[{
begin:"(?=@[A-Za-z]+)",relevance:0,contains:[{className:"doctag",
begin:"@[A-Za-z]+"},{className:"type",begin:"\\{",end:"\\}",excludeEnd:!0,
excludeBegin:!0,relevance:0},{className:"variable",begin:t+"(?=\\s*(-)|$)",
endsParent:!0,relevance:0},{begin:/(?=[^\n])\s/,relevance:0}]}]
}),e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE]
},p=[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,d,g,u,b,{match:/\$\d+/},c]
;l.contains=p.concat({begin:/\{/,end:/\}/,keywords:i,contains:["self"].concat(p)
});const _=[].concat(m,l.contains),h=_.concat([{begin:/(\s*)\(/,end:/\)/,
keywords:i,contains:["self"].concat(_)}]),f={className:"params",begin:/(\s*)\(/,
end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:i,contains:h},E={variants:[{
match:[/class/,/\s+/,t,/\s+/,/extends/,/\s+/,n.concat(t,"(",n.concat(/\./,t),")*")],
scope:{1:"keyword",3:"title.class",5:"keyword",7:"title.class.inherited"}},{
match:[/class/,/\s+/,t],scope:{1:"keyword",3:"title.class"}}]},y={relevance:0,
match:n.either(/\bJSON/,/\b[A-Z][a-z]+([A-Z][a-z]*|\d)*/,/\b[A-Z]{2,}([A-Z][a-z]+|\d)+([A-Z][a-z]*)*/,/\b[A-Z]{2,}[a-z]+([A-Z][a-z]+|\d)*([A-Z][a-z]*)*/),
className:"title.class",keywords:{_:[...se,...oe]}},w={variants:[{
match:[/function/,/\s+/,t,/(?=\s*\()/]},{match:[/function/,/\s*(?=\()/]}],
className:{1:"keyword",3:"title.function"},label:"func.def",contains:[f],
illegal:/%/},v={
match:n.concat(/\b/,(N=[...ce,"super","import","await"].map(e=>e+"\\s*\\("),
n.concat("(?!",N.join("|"),")")),t,n.lookahead(/\s*\(/)),
className:"title.function",relevance:0};var N;const k={
begin:n.concat(/\./,n.lookahead(n.concat(t,/(?![0-9A-Za-z$_(])/))),end:t,
excludeBegin:!0,keywords:"prototype",className:"property",relevance:0},x={
match:[/get|set/,/\s+/,t,/(?=\()/],className:{1:"keyword",3:"title.function"},
contains:[{begin:/\(\)/},f]
},O="(\\([^()]*(\\([^()]*(\\([^()]*\\)[^()]*)*\\)[^()]*)*\\)|"+e.UNDERSCORE_IDENT_RE+")\\s*=>",M={
match:[/const|var|let/,/\s+/,t,/\s*/,/=\s*/,/(async\s*)?/,n.lookahead(O)],
keywords:"async",className:{1:"keyword",3:"title.function"},contains:[f]}
;return{name:"JavaScript",aliases:["js","jsx","mjs","cjs"],keywords:i,exports:{
PARAMS_CONTAINS:h,CLASS_REFERENCE:y},illegal:/#(?![$_A-Za-z])/,
contains:[e.SHEBANG({label:"shebang",binary:"node",relevance:5}),{
label:"use_strict",className:"meta",relevance:10,
begin:/^\s*['"]use (strict|asm)['"]/
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,d,g,u,b,m,{match:/\$\d+/},c,y,{
scope:"attr",match:t+n.lookahead(":"),relevance:0},M,{
begin:"("+e.RE_STARTERS_RE+"|\\b(case|return|throw)\\b)\\s*",
keywords:"return throw case",relevance:0,contains:[m,e.REGEXP_MODE,{
className:"function",begin:O,returnBegin:!0,end:"\\s*=>",contains:[{
className:"params",variants:[{begin:e.UNDERSCORE_IDENT_RE,relevance:0},{
className:null,begin:/\(\s*\)/,skip:!0},{begin:/(\s*)\(/,end:/\)/,
excludeBegin:!0,excludeEnd:!0,keywords:i,contains:h}]}]},{begin:/,/,relevance:0
},{match:/\s+/,relevance:0},{variants:[{begin:"<>",end:"</>"},{
match:/<[A-Za-z0-9\\._:-]+\s*\/>/},{begin:a.begin,
"on:begin":a.isTrulyOpeningTag,end:a.end}],subLanguage:"xml",contains:[{
begin:a.begin,end:a.end,skip:!0,contains:["self"]}]}]},w,{
beginKeywords:"while if switch catch for"},{
begin:"\\b(?!function)"+e.UNDERSCORE_IDENT_RE+"\\([^()]*(\\([^()]*(\\([^()]*\\)[^()]*)*\\)[^()]*)*\\)\\s*\\{",
returnBegin:!0,label:"func.def",contains:[f,e.inherit(e.TITLE_MODE,{begin:t,
className:"title.function"})]},{match:/\.\.\./,relevance:0},k,{match:"\\$"+t,
relevance:0},{match:[/\bconstructor(?=\s*\()/],className:{1:"title.function"},
contains:[f]},v,{relevance:0,match:/\b[A-Z][A-Z_0-9]+\b/,
className:"variable.constant"},E,x,{match:/\$[(.]/}]}}
const ue=e=>b(/\b/,e,/\w$/.test(e)?/\b/:/\B/),be=["Protocol","Type"].map(ue),me=["init","self"].map(ue),pe=["Any","Self"],_e=["actor","any","associatedtype","async","await",/as\?/,/as!/,"as","borrowing","break","case","catch","class","consume","consuming","continue","convenience","copy","default","defer","deinit","didSet","distributed","do","dynamic","each","else","enum","extension","fallthrough",/fileprivate\(set\)/,"fileprivate","final","for","func","get","guard","if","import","indirect","infix",/init\?/,/init!/,"inout",/internal\(set\)/,"internal","in","is","isolated","nonisolated","lazy","let","macro","mutating","nonmutating",/open\(set\)/,"open","operator","optional","override","package","postfix","precedencegroup","prefix",/private\(set\)/,"private","protocol",/public\(set\)/,"public","repeat","required","rethrows","return","set","some","static","struct","subscript","super","switch","throws","throw",/try\?/,/try!/,"try","typealias",/unowned\(safe\)/,/unowned\(unsafe\)/,"unowned","var","weak","where","while","willSet"],he=["false","nil","true"],fe=["assignment","associativity","higherThan","left","lowerThan","none","right"],Ee=["#colorLiteral","#column","#dsohandle","#else","#elseif","#endif","#error","#file","#fileID","#fileLiteral","#filePath","#function","#if","#imageLiteral","#keyPath","#line","#selector","#sourceLocation","#warning"],ye=["abs","all","any","assert","assertionFailure","debugPrint","dump","fatalError","getVaList","isKnownUniquelyReferenced","max","min","numericCast","pointwiseMax","pointwiseMin","precondition","preconditionFailure","print","readLine","repeatElement","sequence","stride","swap","swift_unboxFromSwiftValueWithType","transcode","type","unsafeBitCast","unsafeDowncast","withExtendedLifetime","withUnsafeMutablePointer","withUnsafePointer","withVaList","withoutActuallyEscaping","zip"],we=m(/[/=\-+!*%<>&|^~?]/,/[\u00A1-\u00A7]/,/[\u00A9\u00AB]/,/[\u00AC\u00AE]/,/[\u00B0\u00B1]/,/[\u00B6\u00BB\u00BF\u00D7\u00F7]/,/[\u2016-\u2017]/,/[\u2020-\u2027]/,/[\u2030-\u203E]/,/[\u2041-\u2053]/,/[\u2055-\u205E]/,/[\u2190-\u23FF]/,/[\u2500-\u2775]/,/[\u2794-\u2BFF]/,/[\u2E00-\u2E7F]/,/[\u3001-\u3003]/,/[\u3008-\u3020]/,/[\u3030]/),ve=m(we,/[\u0300-\u036F]/,/[\u1DC0-\u1DFF]/,/[\u20D0-\u20FF]/,/[\uFE00-\uFE0F]/,/[\uFE20-\uFE2F]/),Ne=b(we,ve,"*"),ke=m(/[a-zA-Z_]/,/[\u00A8\u00AA\u00AD\u00AF\u00B2-\u00B5\u00B7-\u00BA]/,/[\u00BC-\u00BE\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u00FF]/,/[\u0100-\u02FF\u0370-\u167F\u1681-\u180D\u180F-\u1DBF]/,/[\u1E00-\u1FFF]/,/[\u200B-\u200D\u202A-\u202E\u203F-\u2040\u2054\u2060-\u206F]/,/[\u2070-\u20CF\u2100-\u218F\u2460-\u24FF\u2776-\u2793]/,/[\u2C00-\u2DFF\u2E80-\u2FFF]/,/[\u3004-\u3007\u3021-\u302F\u3031-\u303F\u3040-\uD7FF]/,/[\uF900-\uFD3D\uFD40-\uFDCF\uFDF0-\uFE1F\uFE30-\uFE44]/,/[\uFE47-\uFEFE\uFF00-\uFFFD]/),xe=m(ke,/\d/,/[\u0300-\u036F\u1DC0-\u1DFF\u20D0-\u20FF\uFE20-\uFE2F]/),Oe=b(ke,xe,"*"),Me=b(/[A-Z]/,xe,"*"),Ae=["attached","autoclosure",b(/convention\(/,m("swift","block","c"),/\)/),"discardableResult","dynamicCallable","dynamicMemberLookup","escaping","freestanding","frozen","GKInspectable","IBAction","IBDesignable","IBInspectable","IBOutlet","IBSegueAction","inlinable","main","nonobjc","NSApplicationMain","NSCopying","NSManaged",b(/objc\(/,Oe,/\)/),"objc","objcMembers","propertyWrapper","requires_stored_property_inits","resultBuilder","Sendable","testable","UIApplicationMain","unchecked","unknown","usableFromInline","warn_unqualified_access"],Se=["iOS","iOSApplicationExtension","macOS","macOSApplicationExtension","macCatalyst","macCatalystApplicationExtension","watchOS","watchOSApplicationExtension","tvOS","tvOSApplicationExtension","swift"],Ce=e=>({
IMPORTANT:{scope:"meta",begin:"!important"},
BLOCK_COMMENT:e.C_BLOCK_COMMENT_MODE,HEXCOLOR:{scope:"number",
begin:/#(([0-9a-fA-F]{3,4})|(([0-9a-fA-F]{2}){3,4}))\b/},UNICODE_RANGE:{
scope:"number",
begin:/\b[Uu]\+[0-9A-Fa-f][0-9A-Fa-f?]{0,5}(-[0-9A-Fa-f][0-9A-Fa-f]{0,5})?/},
FUNCTION_DISPATCH:{className:"built_in",begin:/[\w-]+(?=\()/},
ATTRIBUTE_SELECTOR_MODE:{scope:"selector-attr",begin:/\[/,end:/\]/,illegal:"$",
contains:[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]},CSS_NUMBER_MODE:{
scope:"number",
begin:e.NUMBER_RE+"(%|em|ex|ch|rem|vw|vh|vmin|vmax|cm|mm|in|pt|pc|px|deg|grad|rad|turn|s|ms|Hz|kHz|dpi|dpcm|dppx)?",
relevance:0},CSS_VARIABLE:{className:"attr",begin:/--[A-Za-z_][A-Za-z0-9_-]*/}
}),Te=["a","abbr","address","article","aside","audio","b","blockquote","body","button","canvas","caption","cite","code","dd","del","details","dfn","div","dl","dt","em","fieldset","figcaption","figure","footer","form","h1","h2","h3","h4","h5","h6","header","hgroup","html","i","iframe","img","input","ins","kbd","label","legend","li","main","mark","menu","nav","object","ol","optgroup","option","p","picture","q","quote","samp","section","select","source","span","strong","summary","sup","table","tbody","td","textarea","tfoot","th","thead","time","tr","ul","var","video","defs","g","marker","mask","pattern","svg","switch","symbol","feBlend","feColorMatrix","feComponentTransfer","feComposite","feConvolveMatrix","feDiffuseLighting","feDisplacementMap","feFlood","feGaussianBlur","feImage","feMerge","feMorphology","feOffset","feSpecularLighting","feTile","feTurbulence","linearGradient","radialGradient","stop","circle","ellipse","image","line","path","polygon","polyline","rect","text","use","textPath","tspan","foreignObject","clipPath"],Re=["any-hover","any-pointer","aspect-ratio","color","color-gamut","color-index","device-aspect-ratio","device-height","device-width","display-mode","forced-colors","grid","height","hover","inverted-colors","monochrome","orientation","overflow-block","overflow-inline","pointer","prefers-color-scheme","prefers-contrast","prefers-reduced-motion","prefers-reduced-transparency","resolution","scan","scripting","update","width","min-width","max-width","min-height","max-height"].sort().reverse(),De=["active","any-link","blank","checked","current","default","defined","dir","disabled","drop","empty","enabled","first","first-child","first-of-type","fullscreen","future","focus","focus-visible","focus-within","has","host","host-context","hover","indeterminate","in-range","invalid","is","lang","last-child","last-of-type","left","link","local-link","not","nth-child","nth-col","nth-last-child","nth-last-col","nth-last-of-type","nth-of-type","only-child","only-of-type","optional","out-of-range","past","placeholder-shown","read-only","read-write","required","right","root","scope","target","target-within","user-invalid","valid","visited","where"].sort().reverse(),Ie=["after","backdrop","before","cue","cue-region","first-letter","first-line","grammar-error","marker","part","placeholder","selection","slotted","spelling-error"].sort().reverse(),Le=["accent-color","align-content","align-items","align-self","alignment-baseline","all","anchor-name","animation","animation-composition","animation-delay","animation-direction","animation-duration","animation-fill-mode","animation-iteration-count","animation-name","animation-play-state","animation-range","animation-range-end","animation-range-start","animation-timeline","animation-timing-function","appearance","aspect-ratio","backdrop-filter","backface-visibility","background","background-attachment","background-blend-mode","background-clip","background-color","background-image","background-origin","background-position","background-position-x","background-position-y","background-repeat","background-size","baseline-shift","block-size","border","border-block","border-block-color","border-block-end","border-block-end-color","border-block-end-style","border-block-end-width","border-block-start","border-block-start-color","border-block-start-style","border-block-start-width","border-block-style","border-block-width","border-bottom","border-bottom-color","border-bottom-left-radius","border-bottom-right-radius","border-bottom-style","border-bottom-width","border-collapse","border-color","border-end-end-radius","border-end-start-radius","border-image","border-image-outset","border-image-repeat","border-image-slice","border-image-source","border-image-width","border-inline","border-inline-color","border-inline-end","border-inline-end-color","border-inline-end-style","border-inline-end-width","border-inline-start","border-inline-start-color","border-inline-start-style","border-inline-start-width","border-inline-style","border-inline-width","border-left","border-left-color","border-left-style","border-left-width","border-radius","border-right","border-right-color","border-right-style","border-right-width","border-spacing","border-start-end-radius","border-start-start-radius","border-style","border-top","border-top-color","border-top-left-radius","border-top-right-radius","border-top-style","border-top-width","border-width","bottom","box-align","box-decoration-break","box-direction","box-flex","box-flex-group","box-lines","box-ordinal-group","box-orient","box-pack","box-shadow","box-sizing","break-after","break-before","break-inside","caption-side","caret-color","clear","clip","clip-path","clip-rule","color","color-interpolation","color-interpolation-filters","color-profile","color-rendering","color-scheme","column-count","column-fill","column-gap","column-rule","column-rule-color","column-rule-style","column-rule-width","column-span","column-width","columns","contain","contain-intrinsic-block-size","contain-intrinsic-height","contain-intrinsic-inline-size","contain-intrinsic-size","contain-intrinsic-width","container","container-name","container-type","content","content-visibility","corner-bottom-left-shape","corner-bottom-right-shape","corner-shape","corner-top-left-shape","corner-top-right-shape","counter-increment","counter-reset","counter-set","cue","cue-after","cue-before","cursor","cx","cy","direction","display","dominant-baseline","empty-cells","enable-background","field-sizing","fill","fill-opacity","fill-rule","filter","flex","flex-basis","flex-direction","flex-flow","flex-grow","flex-shrink","flex-wrap","float","flood-color","flood-opacity","flow","font","font-display","font-family","font-feature-settings","font-kerning","font-language-override","font-optical-sizing","font-palette","font-size","font-size-adjust","font-smooth","font-smoothing","font-stretch","font-style","font-synthesis","font-synthesis-position","font-synthesis-small-caps","font-synthesis-style","font-synthesis-weight","font-variant","font-variant-alternates","font-variant-caps","font-variant-east-asian","font-variant-emoji","font-variant-ligatures","font-variant-numeric","font-variant-position","font-variation-settings","font-weight","forced-color-adjust","gap","glyph-orientation-horizontal","glyph-orientation-vertical","grid","grid-area","grid-auto-columns","grid-auto-flow","grid-auto-rows","grid-column","grid-column-end","grid-column-start","grid-gap","grid-row","grid-row-end","grid-row-start","grid-template","grid-template-areas","grid-template-columns","grid-template-rows","hanging-punctuation","height","hyphenate-character","hyphenate-limit-chars","hyphens","icon","image-orientation","image-rendering","image-resolution","ime-mode","initial-letter","initial-letter-align","inline-size","inset","inset-area","inset-block","inset-block-end","inset-block-start","inset-inline","inset-inline-end","inset-inline-start","isolation","justify-content","justify-items","justify-self","kerning","left","letter-spacing","lighting-color","line-break","line-height","line-height-step","list-style","list-style-image","list-style-position","list-style-type","margin","margin-block","margin-block-end","margin-block-start","margin-bottom","margin-inline","margin-inline-end","margin-inline-start","margin-left","margin-right","margin-top","margin-trim","marker","marker-end","marker-mid","marker-start","marks","mask","mask-border","mask-border-mode","mask-border-outset","mask-border-repeat","mask-border-slice","mask-border-source","mask-border-width","mask-clip","mask-composite","mask-image","mask-mode","mask-origin","mask-position","mask-repeat","mask-size","mask-type","masonry-auto-flow","math-depth","math-shift","math-style","max-block-size","max-height","max-inline-size","max-width","min-block-size","min-height","min-inline-size","min-width","mix-blend-mode","nav-down","nav-index","nav-left","nav-right","nav-up","none","normal","object-fit","object-position","offset","offset-anchor","offset-distance","offset-path","offset-position","offset-rotate","opacity","order","orphans","outline","outline-color","outline-offset","outline-style","outline-width","overflow","overflow-anchor","overflow-block","overflow-clip-margin","overflow-inline","overflow-wrap","overflow-x","overflow-y","overlay","overscroll-behavior","overscroll-behavior-block","overscroll-behavior-inline","overscroll-behavior-x","overscroll-behavior-y","padding","padding-block","padding-block-end","padding-block-start","padding-bottom","padding-inline","padding-inline-end","padding-inline-start","padding-left","padding-right","padding-top","page","page-break-after","page-break-before","page-break-inside","paint-order","pause","pause-after","pause-before","perspective","perspective-origin","place-content","place-items","place-self","pointer-events","position","position-anchor","position-visibility","print-color-adjust","quotes","r","resize","rest","rest-after","rest-before","right","rotate","row-gap","ruby-align","ruby-position","scale","scroll-behavior","scroll-margin","scroll-margin-block","scroll-margin-block-end","scroll-margin-block-start","scroll-margin-bottom","scroll-margin-inline","scroll-margin-inline-end","scroll-margin-inline-start","scroll-margin-left","scroll-margin-right","scroll-margin-top","scroll-padding","scroll-padding-block","scroll-padding-block-end","scroll-padding-block-start","scroll-padding-bottom","scroll-padding-inline","scroll-padding-inline-end","scroll-padding-inline-start","scroll-padding-left","scroll-padding-right","scroll-padding-top","scroll-snap-align","scroll-snap-stop","scroll-snap-type","scroll-timeline","scroll-timeline-axis","scroll-timeline-name","scrollbar-color","scrollbar-gutter","scrollbar-width","shape-image-threshold","shape-margin","shape-outside","shape-rendering","speak","speak-as","src","stop-color","stop-opacity","stroke","stroke-dasharray","stroke-dashoffset","stroke-linecap","stroke-linejoin","stroke-miterlimit","stroke-opacity","stroke-width","tab-size","table-layout","text-align","text-align-all","text-align-last","text-anchor","text-combine-upright","text-decoration","text-decoration-color","text-decoration-line","text-decoration-skip","text-decoration-skip-ink","text-decoration-style","text-decoration-thickness","text-emphasis","text-emphasis-color","text-emphasis-position","text-emphasis-style","text-indent","text-justify","text-orientation","text-overflow","text-rendering","text-shadow","text-size-adjust","text-transform","text-underline-offset","text-underline-position","text-wrap","text-wrap-mode","text-wrap-style","timeline-scope","top","touch-action","transform","transform-box","transform-origin","transform-style","transition","transition-behavior","transition-delay","transition-duration","transition-property","transition-timing-function","translate","unicode-bidi","unicode-range","user-modify","user-select","vector-effect","vertical-align","view-timeline","view-timeline-axis","view-timeline-inset","view-timeline-name","view-transition-name","visibility","voice-balance","voice-duration","voice-family","voice-pitch","voice-range","voice-rate","voice-stress","voice-volume","white-space","white-space-collapse","widows","width","will-change","word-break","word-spacing","word-wrap","writing-mode","x","y","z-index","zoom"].sort().reverse(),Be=De.concat(Ie).sort().reverse()
;var Fe="[0-9](_*[0-9])*",$e=`\\.(${Fe})`,ze="[0-9a-fA-F](_*[0-9a-fA-F])*",je={
className:"number",variants:[{
begin:`(\\b(${Fe})((${$e})|\\.)?|(${$e}))[eE][+-]?(${Fe})[fFdD]?\\b`},{
begin:`\\b(${Fe})((${$e})[fFdD]?\\b|\\.([fFdD]\\b)?)`},{
begin:`(${$e})[fFdD]?\\b`},{begin:`\\b(${Fe})[fFdD]\\b`},{
begin:`\\b0[xX]((${ze})\\.?|(${ze})?\\.(${ze}))[pP][+-]?(${Fe})[fFdD]?\\b`},{
begin:"\\b(0|[1-9](_*[0-9])*)[lL]?\\b"},{begin:`\\b0[xX](${ze})[lL]?\\b`},{
begin:"\\b0(_*[0-7])*[lL]?\\b"},{begin:"\\b0[bB][01](_*[01])*[lL]?\\b"}],
relevance:0};function Ue(e,n,t){return-1===t?"":e.replace(n,a=>Ue(e,n,t-1))}
var Pe=Object.freeze({__proto__:null,grmr_bash:e=>{const n=e.regex,t={},a={
begin:/\$\{/,end:/\}/,contains:["self",{begin:/:-/,contains:[t]}]}
;Object.assign(t,{className:"variable",variants:[{
begin:n.concat(/\$[\w\d#@][\w\d_]*/,"(?![\\w\\d])(?![$])")},a]});const i={
className:"subst",begin:/\$\(/,end:/\)/,contains:[e.BACKSLASH_ESCAPE]
},r=e.inherit(e.COMMENT(),{match:[/(^|\s)/,/#.*$/],scope:{2:"comment"}}),s={
begin:/<<-?\s*(?=\w+)/,starts:{contains:[e.END_SAME_AS_BEGIN({begin:/(\w+)/,
end:/(\w+)/,className:"string"})]}},o={className:"string",begin:/"/,end:/"/,
contains:[e.BACKSLASH_ESCAPE,t,i]};i.contains.push(o);const c={begin:/\$?\(\(/,
end:/\)\)/,contains:[{begin:/\d+#[0-9a-f]+/,className:"number"},e.NUMBER_MODE,t]
},l=e.SHEBANG({binary:"(fish|bash|zsh|sh|csh|ksh|tcsh|dash|scsh)",relevance:10
}),d={className:"function",begin:/\w[\w\d_]*\s*\(\s*\)\s*\{/,returnBegin:!0,
contains:[e.inherit(e.TITLE_MODE,{begin:/\w[\w\d_]*/})],relevance:0};return{
name:"Bash",aliases:["sh","zsh"],keywords:{$pattern:/\b[a-z][a-z0-9._-]+\b/,
keyword:["if","then","else","elif","fi","time","for","while","until","in","do","done","case","esac","coproc","function","select"],
literal:["true","false"],
built_in:["break","cd","continue","eval","exec","exit","export","getopts","hash","pwd","readonly","return","shift","test","times","trap","umask","unset","alias","bind","builtin","caller","command","declare","echo","enable","help","let","local","logout","mapfile","printf","read","readarray","source","sudo","type","typeset","ulimit","unalias","set","shopt","autoload","bg","bindkey","bye","cap","chdir","clone","comparguments","compcall","compctl","compdescribe","compfiles","compgroups","compquote","comptags","comptry","compvalues","dirs","disable","disown","echotc","echoti","emulate","fc","fg","float","functions","getcap","getln","history","integer","jobs","kill","limit","log","noglob","popd","print","pushd","pushln","rehash","sched","setcap","setopt","stat","suspend","ttyctl","unfunction","unhash","unlimit","unsetopt","vared","wait","whence","where","which","zcompile","zformat","zftp","zle","zmodload","zparseopts","zprof","zpty","zregexparse","zsocket","zstyle","ztcp","chcon","chgrp","chown","chmod","cp","dd","df","dir","dircolors","ln","ls","mkdir","mkfifo","mknod","mktemp","mv","realpath","rm","rmdir","shred","sync","touch","truncate","vdir","b2sum","base32","base64","cat","cksum","comm","csplit","cut","expand","fmt","fold","head","join","md5sum","nl","numfmt","od","paste","ptx","pr","sha1sum","sha224sum","sha256sum","sha384sum","sha512sum","shuf","sort","split","sum","tac","tail","tr","tsort","unexpand","uniq","wc","arch","basename","chroot","date","dirname","du","echo","env","expr","factor","groups","hostid","id","link","logname","nice","nohup","nproc","pathchk","pinky","printenv","printf","pwd","readlink","runcon","seq","sleep","stat","stdbuf","stty","tee","test","timeout","tty","uname","unlink","uptime","users","who","whoami","yes"]
},contains:[l,e.SHEBANG(),d,c,r,s,{match:/(\/[a-z._-]+)+/},o,{match:/\\"/},{
className:"string",begin:/'/,end:/'/},{match:/\\'/},t]}},grmr_c:e=>{
const n=e.regex,t=e.COMMENT("//","$",{contains:[{begin:/\\\n/}]
}),a="decltype\\(auto\\)",i="[a-zA-Z_]\\w*::",r="("+a+"|"+n.optional(i)+"[a-zA-Z_]\\w*"+n.optional("<[^<>]+>")+")",s={
className:"type",variants:[{begin:"\\b[a-z\\d_]*_t\\b"},{
match:n.concat(/\batomic_/,n.either("bool","char","schar","uchar","short","ushort","int","uint","long","ulong","llong","ullong","char16_t","char32_t","wchar_t","int_least8_t","uint_least8_t","int_least16_t","uint_least16_t","int_least32_t","uint_least32_t","int_least64_t","uint_least64_t","int_fast8_t","uint_fast8_t","int_fast16_t","uint_fast16_t","int_fast32_t","uint_fast32_t","int_fast64_t","uint_fast64_t","intptr_t","uintptr_t","size_t","ptrdiff_t","intmax_t","uintmax_t"),/\b/)
}]},o={className:"string",variants:[{begin:'(u8?|U|L)?"',end:'"',illegal:"\\n",
contains:[e.BACKSLASH_ESCAPE]},{
begin:"(u8?|U|L)?'(\\\\(x[0-9A-Fa-f]{2}|u[0-9A-Fa-f]{4,8}|[0-7]{3}|\\S)|.)",
end:"'",illegal:"."},e.END_SAME_AS_BEGIN({
begin:/(?:u8?|U|L)?R"([^()\\\s"]{0,16})\(/,end:/\)([^()\\\s"]{0,16})"/})]},c={
className:"number",variants:[{match:/\b(0b[01']+)/},{
match:/(-?)\b([\d']+(\.[\d']*)?|\.[\d']+)((ll|LL|l|L)(u|U)?|(u|U)(ll|LL|l|L)?|f|F|b|B)/
},{
match:/(-?)\b(0[xX][a-fA-F0-9]+(?:'[a-fA-F0-9]+)*(?:\.[a-fA-F0-9]*(?:'[a-fA-F0-9]*)*)?(?:[pP][-+]?[0-9]+)?(l|L)?(u|U)?)/
},{match:/(-?)\b\d+(?:'\d+)*(?:\.\d*(?:'\d*)*)?(?:[eE][-+]?\d+)?/}],relevance:0
},l={scope:"meta",begin:/#\s*include\b/,end:/$/,keywords:{keyword:"include"},
contains:[{begin:/\\\n/},o,{scope:"string",begin:/<.*?>/
},t,e.C_BLOCK_COMMENT_MODE]},d={className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma _Pragma ifdef ifndef elifdef elifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(o,{className:"string"
}),t,e.C_BLOCK_COMMENT_MODE]},g=[l,d],u={className:"title",
begin:n.optional(i)+e.IDENT_RE,relevance:0
},b=n.optional(i)+e.IDENT_RE+"\\s*\\(",m={
keyword:["asm","auto","break","case","continue","default","do","else","enum","extern","for","fortran","goto","if","inline","register","restrict","return","sizeof","typeof","typeof_unqual","struct","switch","typedef","union","volatile","while","_Alignas","_Alignof","_Atomic","_Generic","_Noreturn","_Static_assert","_Thread_local","alignas","alignof","noreturn","static_assert","thread_local","_Pragma"],
type:["float","double","signed","unsigned","int","short","long","char","void","_Bool","_BitInt","_Complex","_Imaginary","_Decimal32","_Decimal64","_Decimal96","_Decimal128","_Decimal64x","_Decimal128x","_Float16","_Float32","_Float64","_Float128","_Float32x","_Float64x","_Float128x","const","static","constexpr","complex","bool","imaginary"],
literal:"true false NULL",
built_in:"std string wstring cin cout cerr clog stdin stdout stderr stringstream istringstream ostringstream auto_ptr deque list queue stack vector map set pair bitset multiset multimap unordered_set unordered_map unordered_multiset unordered_multimap priority_queue make_pair array shared_ptr abort terminate abs acos asin atan2 atan calloc ceil cosh cos exit exp fabs floor fmod fprintf fputs free frexp fscanf future isalnum isalpha iscntrl isdigit isgraph islower isprint ispunct isspace isupper isxdigit tolower toupper labs ldexp log10 log malloc realloc memchr memcmp memcpy memset modf pow printf putchar puts scanf sinh sin snprintf sprintf sqrt sscanf strcat strchr strcmp strcpy strcspn strlen strncat strncmp strncpy strpbrk strrchr strspn strstr tanh tan vfprintf vprintf vsprintf endl initializer_list unique_ptr"
},p=[...g,s,t,e.C_BLOCK_COMMENT_MODE,c,o],_={variants:[{begin:/=/,end:/;/},{
begin:/\(/,end:/\)/},{beginKeywords:"new throw return else",end:/;/}],
keywords:m,contains:p.concat([{begin:/\(/,end:/\)/,keywords:m,
contains:p.concat(["self"]),relevance:0}]),relevance:0},h={
begin:"("+r+"[\\*&\\s]+){1,12}"+b,returnBegin:!0,end:/[{;=]/,excludeEnd:!0,
keywords:m,illegal:/[^\w\s\*&:<>.]/,contains:[{begin:a,keywords:m,relevance:0},{
begin:b,returnBegin:!0,contains:[e.inherit(u,{className:"title.function"})],
relevance:0},{relevance:0,match:/,/},{className:"params",begin:/\(/,end:/\)/,
keywords:m,relevance:0,contains:[t,e.C_BLOCK_COMMENT_MODE,o,c,s,{begin:/\(/,
end:/\)/,keywords:m,relevance:0,contains:["self",t,e.C_BLOCK_COMMENT_MODE,o,c,s]
}]},s,t,e.C_BLOCK_COMMENT_MODE,...g]};return{name:"C",aliases:["h"],keywords:m,
disableAutodetect:!0,illegal:"</",contains:[].concat(_,h,p,[...g,{
begin:e.IDENT_RE+"::",keywords:m},{className:"class",
beginKeywords:"enum class struct union",end:/[{;:<>=]/,contains:[{
beginKeywords:"final class struct"},e.TITLE_MODE]}]),exports:{preprocessor:d,
strings:o,keywords:m}}},grmr_cpp:e=>{const n=e.regex,t=e.COMMENT("//","$",{
contains:[{begin:/\\\n/}]
}),a="decltype\\(auto\\)",i="[a-zA-Z_]\\w*::",r="(?!struct)("+a+"|"+n.optional(i)+"[a-zA-Z_]\\w*"+n.optional("<[^<>]+>")+")",s={
className:"type",begin:"\\b[a-z\\d_]*_t\\b"},o={className:"string",variants:[{
begin:'(u8?|U|L)?"',end:'"',illegal:"\\n",contains:[e.BACKSLASH_ESCAPE]},{
begin:"(u8?|U|L)?'(\\\\(x[0-9A-Fa-f]{2}|u[0-9A-Fa-f]{4,8}|[0-7]{3}|\\S)|.)",
end:"'",illegal:"."},e.END_SAME_AS_BEGIN({
begin:/(?:u8?|U|L)?R"([^()\\\s"]{0,16})\(/,end:/\)([^()\\\s"]{0,16})"/})]},c={
className:"number",variants:[{
begin:"[+-]?(?:(?:\\b[0-9](?:'?[0-9])*\\.(?:[0-9](?:'?[0-9])*)?|\\.[0-9](?:'?[0-9])*)(?:[Ee][+-]?[0-9](?:'?[0-9])*)?|\\b[0-9](?:'?[0-9])*[Ee][+-]?[0-9](?:'?[0-9])*|\\b0[Xx](?:[0-9A-Fa-f](?:'?[0-9A-Fa-f])*(?:\\.(?:[0-9A-Fa-f](?:'?[0-9A-Fa-f])*)?)?|\\.[0-9A-Fa-f](?:'?[0-9A-Fa-f])*)[Pp][+-]?[0-9](?:'?[0-9])*)(?:[Ff](?:16|32|64|128)?|(BF|bf)16|[Ll]|)"
},{
begin:"[+-]?\\b(?:0[Bb][01](?:'?[01])*|0[Xx][0-9A-Fa-f](?:'?[0-9A-Fa-f])*|0(?:'?[0-7])*|[1-9](?:'?[0-9])*)(?:[Uu](?:LL?|ll?)|[Uu][Zz]?|(?:LL?|ll?)[Uu]?|[Zz][Uu]|)"
}],relevance:0},l=[{scope:"meta",begin:/#\s*include\b/,end:/$/,keywords:{
keyword:"include"},contains:[{begin:/\\\n/},o,{scope:"string",begin:/<.*?>/
},t,e.C_BLOCK_COMMENT_MODE]},{className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma _Pragma ifdef ifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(o,{className:"string"
}),t,e.C_BLOCK_COMMENT_MODE]}],d={className:"title",
begin:n.optional(i)+e.IDENT_RE,relevance:0
},g=n.optional(i)+e.IDENT_RE+"\\s*\\(",u=["alignas","alignof","and","and_eq","asm","atomic_cancel","atomic_commit","atomic_noexcept","auto","bitand","bitor","break","case","catch","class","co_await","co_return","co_yield","compl","concept","const_cast|10","consteval","constexpr","constinit","continue","decltype","default","delete","do","dynamic_cast|10","else","enum","explicit","export","extern","false","final","for","friend","goto","if","import","inline","module","mutable","namespace","new","noexcept","not","not_eq","nullptr","operator","or","or_eq","override","private","protected","public","reflexpr","register","reinterpret_cast|10","requires","return","sizeof","static_assert","static_cast|10","struct","switch","synchronized","template","this","thread_local","throw","transaction_safe","transaction_safe_dynamic","true","try","typedef","typeid","typename","union","using","virtual","volatile","while","xor","xor_eq"],b={
type:["bool","char","char16_t","char32_t","char8_t","double","float","int","long","short","void","wchar_t","unsigned","signed","const","static"],
keyword:u,literal:["NULL","false","nullopt","nullptr","true"],
built_in:["_Pragma"],
_type_hints:["any","auto_ptr","barrier","binary_semaphore","bitset","complex","condition_variable","condition_variable_any","counting_semaphore","deque","false_type","flat_map","flat_set","future","imaginary","initializer_list","istringstream","jthread","latch","lock_guard","multimap","multiset","mutex","optional","ostringstream","packaged_task","pair","promise","priority_queue","queue","recursive_mutex","recursive_timed_mutex","scoped_lock","set","shared_future","shared_lock","shared_mutex","shared_timed_mutex","shared_ptr","stack","string_view","stringstream","timed_mutex","thread","true_type","tuple","unique_lock","unique_ptr","unordered_map","unordered_multimap","unordered_multiset","unordered_set","variant","vector","weak_ptr","wstring","wstring_view"]
},m={className:"function.dispatch",relevance:0,keywords:{
_hint:["abort","abs","acos","apply","as_const","asin","atan","atan2","calloc","ceil","cerr","cin","clog","cos","cosh","cout","declval","endl","exchange","exit","exp","fabs","floor","fmod","forward","fprintf","fputs","free","frexp","fscanf","future","invoke","isalnum","isalpha","iscntrl","isdigit","isgraph","islower","isprint","ispunct","isspace","isupper","isxdigit","labs","launder","ldexp","log","log10","make_pair","make_shared","make_shared_for_overwrite","make_tuple","make_unique","malloc","memchr","memcmp","memcpy","memset","modf","move","pow","printf","putchar","puts","realloc","scanf","sin","sinh","snprintf","sprintf","sqrt","sscanf","std","stderr","stdin","stdout","strcat","strchr","strcmp","strcpy","strcspn","strlen","strncat","strncmp","strncpy","strpbrk","strrchr","strspn","strstr","swap","tan","tanh","terminate","to_underlying","tolower","toupper","vfprintf","visit","vprintf","vsprintf"]
},
begin:n.concat(/\b/,`(?!${u.join("|")})`,e.IDENT_RE,n.lookahead(/(<[^<>]+>|)\s*\(/))
},p=[m,...l,s,t,e.C_BLOCK_COMMENT_MODE,c,o],_={variants:[{begin:/=/,end:/;/},{
begin:/\(/,end:/\)/},{beginKeywords:"new throw return else",end:/;/}],
keywords:b,contains:p.concat([{begin:/\(/,end:/\)/,keywords:b,
contains:p.concat(["self"]),relevance:0}]),relevance:0},h={className:"function",
begin:"("+r+"[\\*&\\s]+){1,12}"+g,returnBegin:!0,end:/[{;=]/,excludeEnd:!0,
keywords:b,illegal:/[^\w\s\*&:<>.]/,contains:[{begin:a,keywords:b,relevance:0},{
begin:g,returnBegin:!0,contains:[d],relevance:0},{begin:/::/,relevance:0},{
begin:/:/,endsWithParent:!0,contains:[o,c]},{relevance:0,match:/,/},{
className:"params",begin:/\(/,end:/\)/,keywords:b,relevance:0,
contains:[t,e.C_BLOCK_COMMENT_MODE,o,c,s,{begin:/\(/,end:/\)/,keywords:b,
relevance:0,contains:["self",t,e.C_BLOCK_COMMENT_MODE,o,c,s]}]
},s,t,e.C_BLOCK_COMMENT_MODE,...l]};return{name:"C++",
aliases:["cc","c++","h++","hpp","hh","hxx","cxx"],keywords:b,illegal:"</",
classNameAliases:{"function.dispatch":"built_in"},
contains:[].concat(_,h,m,p,[...l,{
begin:"\\b(deque|list|queue|priority_queue|pair|stack|vector|map|set|bitset|multiset|multimap|unordered_map|unordered_set|unordered_multiset|unordered_multimap|array|tuple|optional|variant|function|flat_map|flat_set)\\s*<(?!<)",
end:">",keywords:b,contains:["self",s]},{begin:e.IDENT_RE+"::",keywords:b},{
match:[/\b(?:enum(?:\s+(?:class|struct))?|class|struct|union)/,/\s+/,/\w+/],
className:{1:"keyword",3:"title.class"}}])}},grmr_csharp:e=>{const n={
keyword:["abstract","as","base","break","case","catch","class","const","continue","do","else","event","explicit","extern","finally","fixed","for","foreach","goto","if","implicit","in","interface","internal","is","lock","namespace","new","operator","out","override","params","private","protected","public","readonly","record","ref","return","scoped","sealed","sizeof","stackalloc","static","struct","switch","this","throw","try","typeof","unchecked","unsafe","using","virtual","void","volatile","while"].concat(["add","alias","and","ascending","args","async","await","by","descending","dynamic","equals","file","from","get","global","group","init","into","join","let","nameof","not","notnull","on","or","orderby","partial","record","remove","required","scoped","select","set","unmanaged","value|0","var","when","where","with","yield"]),
built_in:["bool","byte","char","decimal","delegate","double","dynamic","enum","float","int","long","nint","nuint","object","sbyte","short","string","ulong","uint","ushort"],
literal:["default","false","null","true"]},t=e.inherit(e.TITLE_MODE,{
begin:"[a-zA-Z](\\.?\\w)*"}),a="\\d(_*\\d)*",i="([uU][lL]?|[lL][uU]?)?",r={
className:"number",variants:[{begin:"\\b0[bB]_*[01](_*[01])*"+i},{
begin:"(-?)\\b0[xX]_*[a-fA-F0-9](_*[a-fA-F0-9])*"+i},{
begin:"(-?)(\\b"+a+"(\\.("+a+")?)?|\\."+a+")([eE][-+]?"+a+")?([fFdDmM]|[uU][lL]?|[lL][uU]?)?"
}],relevance:0},s={className:"string",begin:'@"',end:'"',contains:[{begin:'""'}]
},o=e.inherit(s,{illegal:/\n/}),c={className:"subst",begin:/\{/,end:/\}/,
keywords:n},l=e.inherit(c,{illegal:/\n/}),d={className:"string",begin:/\$"/,
end:'"',illegal:/\n/,contains:[{begin:/\{\{/},{begin:/\}\}/
},e.BACKSLASH_ESCAPE,l]},g={className:"string",begin:/\$@"/,end:'"',contains:[{
begin:/\{\{/},{begin:/\}\}/},{begin:'""'},c]},u=e.inherit(g,{illegal:/\n/,
contains:[{begin:/\{\{/},{begin:/\}\}/},{begin:'""'},l]})
;c.contains=[g,d,s,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,r,e.C_BLOCK_COMMENT_MODE],
l.contains=[u,d,o,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,r,e.inherit(e.C_BLOCK_COMMENT_MODE,{
illegal:/\n/})];const b={variants:[{className:"string",
begin:/"""("*)(?!")(.|\n)*?"""\1/,relevance:1
},g,d,s,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]},m={begin:"<",end:">",
contains:[{beginKeywords:"in out"},t]
},p=e.IDENT_RE+"(<"+e.IDENT_RE+"(\\s*,\\s*"+e.IDENT_RE+")*>)?(\\[\\])?",_={
begin:"@"+e.IDENT_RE,relevance:0};return{name:"C#",aliases:["cs","c#"],
keywords:n,illegal:/::/,contains:[e.COMMENT("///","$",{returnBegin:!0,
contains:[{className:"doctag",variants:[{begin:"///",relevance:0},{
begin:"\x3c!--|--\x3e"},{begin:"</?",end:">"}]}]
}),e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{className:"meta",begin:"#",
end:"$",keywords:{
keyword:"if else elif endif define undef warning error line region endregion pragma checksum"
}},b,r,{beginKeywords:"class interface",relevance:0,end:/[{;=]/,
illegal:/[^\s:,]/,contains:[{beginKeywords:"where class"
},t,m,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{beginKeywords:"namespace",
relevance:0,end:/[{;=]/,illegal:/[^\s:]/,
contains:[t,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{
beginKeywords:"record",relevance:0,end:/[{;=]/,illegal:/[^\s:]/,
contains:[t,m,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{className:"meta",
begin:"^\\s*\\[(?=[\\w])",excludeBegin:!0,end:"\\]",excludeEnd:!0,contains:[{
className:"string",begin:/"/,end:/"/}]},{
beginKeywords:"new return throw await else",relevance:0},{className:"function",
begin:"("+p+"\\s+)+"+e.IDENT_RE+"\\s*(<[^=]+>\\s*)?\\(",returnBegin:!0,
end:/\s*[{;=]/,excludeEnd:!0,keywords:n,contains:[{
beginKeywords:"public private protected static internal protected abstract async extern override unsafe virtual new sealed partial",
relevance:0},{begin:e.IDENT_RE+"\\s*(<[^=]+>\\s*)?\\(",returnBegin:!0,
contains:[e.TITLE_MODE,m],relevance:0},{match:/\(\)/},{className:"params",
begin:/\(/,end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:n,relevance:0,
contains:[b,r,e.C_BLOCK_COMMENT_MODE]
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},_]}},grmr_css:e=>{
const n=e.regex,t=Ce(e),a=[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE];return{
name:"CSS",case_insensitive:!0,illegal:/[=|'\$]/,keywords:{
keyframePosition:"from to"},classNameAliases:{keyframePosition:"selector-tag"},
contains:[t.BLOCK_COMMENT,{begin:/-(webkit|moz|ms|o)-(?=[a-z])/
},t.CSS_NUMBER_MODE,{className:"selector-id",begin:/#[A-Za-z0-9_-]+/,relevance:0
},{className:"selector-class",begin:"\\.[a-zA-Z-][a-zA-Z0-9_-]*",relevance:0
},t.ATTRIBUTE_SELECTOR_MODE,{className:"selector-pseudo",variants:[{
begin:":("+De.join("|")+")"},{begin:":(:)?("+Ie.join("|")+")"}]
},t.CSS_VARIABLE,{className:"attribute",begin:"\\b("+Le.join("|")+")\\b"},{
begin:/:/,end:/[;}{]/,
contains:[t.BLOCK_COMMENT,t.HEXCOLOR,t.IMPORTANT,t.CSS_NUMBER_MODE,t.UNICODE_RANGE,...a,{
begin:/(url|data-uri)\(/,end:/\)/,relevance:0,keywords:{built_in:"url data-uri"
},contains:[...a,{className:"string",begin:/[^)]/,endsWithParent:!0,
excludeEnd:!0}]},t.FUNCTION_DISPATCH]},{begin:n.lookahead(/@/),end:"[{;]",
relevance:0,illegal:/:/,contains:[{className:"keyword",begin:/@-?\w[\w]*(-\w+)*/
},{begin:/\s/,endsWithParent:!0,excludeEnd:!0,relevance:0,keywords:{
$pattern:/[a-z-]+/,keyword:"and or not only",attribute:Re.join(" ")},contains:[{
begin:/[a-z-]+(?=:)/,className:"attribute"},...a,t.CSS_NUMBER_MODE]}]},{
className:"selector-tag",begin:"\\b("+Te.join("|")+")\\b"}]}},grmr_diff:e=>{
const n=e.regex;return{name:"Diff",aliases:["patch"],contains:[{
className:"meta",relevance:10,
match:n.either(/^@@ +-\d+,\d+ +\+\d+,\d+ +@@/,/^@@ +-\d+ +\+\d+,\d+ +@@/,/^@@ +-\d+,\d+ +\+\d+ +@@/,/^@@ +-\d+ +\+\d+ +@@/,/^\*\*\* +\d+,\d+ +\*\*\*\*$/,/^--- +\d+,\d+ +----$/)
},{className:"comment",variants:[{
begin:n.either(/Index: /,/^index/,/={3,}/,/^-{3}/,/^\*{3} /,/^\+{3}/,/^diff --git/),
end:/$/},{match:/^\*{15}$/}]},{className:"addition",begin:/^\+/,end:/$/},{
className:"deletion",begin:/^-/,end:/$/},{className:"addition",begin:/^!/,
end:/$/}]}},grmr_go:e=>{const n={
keyword:["break","case","chan","const","continue","default","defer","else","fallthrough","for","func","go","goto","if","import","interface","map","package","range","return","select","struct","switch","type","var"],
type:["bool","byte","complex64","complex128","error","float32","float64","int8","int16","int32","int64","string","uint8","uint16","uint32","uint64","int","uint","uintptr","rune"],
literal:["true","false","iota","nil"],
built_in:["append","cap","close","complex","copy","imag","len","make","new","panic","print","println","real","recover","delete"]
};return{name:"Go",aliases:["golang"],keywords:n,illegal:"</",
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{className:"string",
variants:[e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,{begin:"`",end:"`"}]},{
className:"number",variants:[{
match:/-?\b0[xX]\.[a-fA-F0-9](_?[a-fA-F0-9])*[pP][+-]?\d(_?\d)*i?/,relevance:0
},{
match:/-?\b0[xX](_?[a-fA-F0-9])+((\.([a-fA-F0-9](_?[a-fA-F0-9])*)?)?[pP][+-]?\d(_?\d)*)?i?/,
relevance:0},{match:/-?\b0[oO](_?[0-7])*i?/,relevance:0},{
match:/-?\b0[bB](_?[01])*i?/,relevance:0},{
match:/-?\.\d(_?\d)*([eE][+-]?\d(_?\d)*)?i?/,relevance:0},{
match:/-?\b\d(_?\d)*(\.(\d(_?\d)*)?)?([eE][+-]?\d(_?\d)*)?i?/,relevance:0}]},{
begin:/:=/},{className:"function",beginKeywords:"func",end:"\\s*(\\{|$)",
excludeEnd:!0,contains:[e.TITLE_MODE,{className:"params",begin:/\(/,end:/\)/,
endsParent:!0,keywords:n,illegal:/["']/}]}]}},grmr_graphql:e=>{const n=e.regex
;return{name:"GraphQL",aliases:["gql"],case_insensitive:!0,disableAutodetect:!1,
keywords:{
keyword:["query","mutation","subscription","type","input","schema","directive","interface","union","scalar","fragment","enum","on"],
literal:["true","false","null"]},
contains:[e.HASH_COMMENT_MODE,e.QUOTE_STRING_MODE,e.NUMBER_MODE,{
scope:"punctuation",match:/[.]{3}/,relevance:0},{scope:"punctuation",
begin:/[\!\(\)\:\=\[\]\{\|\}]{1}/,relevance:0},{scope:"variable",begin:/\$/,
end:/\W/,excludeEnd:!0,relevance:0},{scope:"meta",match:/@\w+/,excludeEnd:!0},{
scope:"symbol",begin:n.concat(/[_A-Za-z][_0-9A-Za-z]*/,n.lookahead(/\s*:/)),
relevance:0}],illegal:[/[;<']/,/BEGIN/]}},grmr_ini:e=>{const n=e.regex,t={
className:"number",relevance:0,variants:[{begin:/([+-]+)?[\d]+_[\d_]+/},{
begin:e.NUMBER_RE}]},a=e.COMMENT();a.variants=[{begin:/;/,end:/$/},{begin:/#/,
end:/$/}];const i={className:"variable",variants:[{begin:/\$[\w\d"][\w\d_]*/},{
begin:/\$\{(.*?)\}/}]},r={className:"literal",
begin:/\bon|off|true|false|yes|no\b/},s={className:"string",
contains:[e.BACKSLASH_ESCAPE],variants:[{begin:"'''",end:"'''",relevance:10},{
begin:'"""',end:'"""',relevance:10},{begin:'"',end:'"'},{begin:"'",end:"'"}]
},o={begin:/\[/,end:/\]/,contains:[a,r,i,s,t,"self"],relevance:0
},c=n.either(/[A-Za-z0-9_-]+/,/"(\\"|[^"])*"/,/'[^']*'/);return{
name:"TOML, also INI",aliases:["toml"],case_insensitive:!0,illegal:/\S/,
contains:[a,{className:"section",begin:/\[+/,end:/\]+/},{
begin:n.concat(c,"(\\s*\\.\\s*",c,")*",n.lookahead(/\s*=\s*[^#\s]/)),
className:"attr",starts:{end:/$/,contains:[a,o,r,i,s,t]}}]}},grmr_java:e=>{
const n=e.regex,t="[\xc0-\u02b8a-zA-Z_$][\xc0-\u02b8a-zA-Z_$0-9]*",a="(?:(?:\\s*\\[\\s*])+)?",i=t+"<@@@>"+a,r="(?:\\?(?:\\s+(?:extends|super)\\s+"+i+")?|"+i+")",s=Ue("(?:\\s*<\\s*"+r+"(?:\\s*,\\s*"+r+")*\\s*>)?",/<@@@>/g,2),o={
keyword:["synchronized","abstract","private","var","static","if","const ","for","while","strictfp","finally","protected","import","native","final","void","enum","else","break","transient","catch","instanceof","volatile","case","assert","package","default","public","try","switch","continue","throws","protected","public","private","module","requires","exports","do","sealed","yield","permits","goto","when"],
literal:["false","true","null"],
type:["char","boolean","long","float","int","byte","short","double"],
built_in:["super","this"]},c={className:"meta",begin:"@"+t,contains:[{
begin:/\(/,end:/\)/,contains:["self"]}]},l={className:"params",begin:/\(/,
end:/\)/,keywords:o,relevance:0,contains:[e.C_BLOCK_COMMENT_MODE],endsParent:!0}
;return{name:"Java",aliases:["jsp"],keywords:o,illegal:/<\/|#/,
contains:[e.COMMENT("/\\*\\*","\\*/",{relevance:0,contains:[{begin:/\w+@/,
relevance:0},{className:"doctag",begin:"@[A-Za-z]+"}]}),{
begin:/import java\.[a-z]+\./,keywords:"import",relevance:2
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{begin:/"""/,end:/"""/,
className:"string",contains:[e.BACKSLASH_ESCAPE]
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,{
match:[/\b(?:class|interface|enum|extends|implements|new)/,/\s+/,t],className:{
1:"keyword",3:"title.class"}},{match:/non-sealed/,scope:"keyword"},{
beginKeywords:"new throw return else yield assert",relevance:0},{
begin:[t,n.concat(s,a,/\s+/),t,a,/\s*/,/=(?!=)/],className:{1:"type",
3:"variable",6:"operator"}},{begin:[/record/,/\s+/,t],className:{1:"keyword",
3:"title.class"},contains:[l,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{
begin:[t,n.concat(s,a,/\s+/),t,/\s*(?=\()/],className:{1:"type",
3:"title.function"},keywords:o,contains:[{className:"params",begin:/\(/,
end:/\)/,keywords:o,relevance:0,
contains:[c,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,je,e.C_BLOCK_COMMENT_MODE]
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},je,c]}},grmr_javascript:ge,
grmr_json:e=>{const n=["true","false","null"],t={scope:"literal",
beginKeywords:n.join(" ")};return{name:"JSON",aliases:["jsonc","json5"],
keywords:{literal:n},contains:[{className:"attr",
begin:/(("(\\.|[^\\"\r\n])*")|('(\\.|[^\\'\r\n])*'))(?=\s*:)/,relevance:1.01},{
match:/[{}[\],:]/,className:"punctuation",relevance:0
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,t,ae,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE],
illegal:"\\S"}},grmr_kotlin:e=>{const n={
keyword:"abstract as val var vararg get set class object open private protected public noinline crossinline dynamic final enum if else do while for when throw try catch finally import package is in fun override companion reified inline lateinit init interface annotation data sealed internal infix operator out by constructor super tailrec where const inner suspend typealias external expect actual",
built_in:"Byte Short Char Int Long Boolean Float Double Void Unit Nothing",
literal:"true false null"},t={className:"symbol",begin:e.UNDERSCORE_IDENT_RE+"@"
},a={className:"subst",begin:/\$\{/,end:/\}/,contains:[e.C_NUMBER_MODE]},i={
className:"variable",begin:"\\$"+e.UNDERSCORE_IDENT_RE},r={className:"string",
variants:[{begin:'"""',end:'"""(?=[^"])',contains:[i,a]},{begin:"'",end:"'",
illegal:/\n/,contains:[e.BACKSLASH_ESCAPE]},{begin:'"',end:'"',illegal:/\n/,
contains:[e.BACKSLASH_ESCAPE,i,a]}]};a.contains.push(r);const s={
className:"meta",
begin:"@(?:file|property|field|get|set|receiver|param|setparam|delegate)\\s*:(?:\\s*"+e.UNDERSCORE_IDENT_RE+")?"
},o={className:"meta",begin:"@"+e.UNDERSCORE_IDENT_RE,contains:[{begin:/\(/,
end:/\)/,contains:[e.inherit(r,{className:"string"}),"self"]}]
},c=je,l=e.COMMENT("/\\*","\\*/",{contains:[e.C_BLOCK_COMMENT_MODE]}),d={
variants:[{className:"type",begin:e.UNDERSCORE_IDENT_RE},{begin:/\(/,end:/\)/,
contains:[]}]},g=d;return g.variants[1].contains=[d],d.variants[1].contains=[g],
{name:"Kotlin",aliases:["kt","kts","ktm","ktx"],keywords:n,
contains:[e.COMMENT("/\\*\\*","\\*/",{relevance:0,contains:[{className:"doctag",
begin:"@[A-Za-z]+"}]}),e.C_LINE_COMMENT_MODE,l,{className:"keyword",
begin:/\b(break|continue|return|this)\b/,starts:{contains:[{className:"symbol",
begin:/@\w+/}]}},t,s,o,{className:"function",beginKeywords:"fun",end:"[(]|$",
returnBegin:!0,excludeEnd:!0,keywords:n,relevance:5,contains:[{
begin:e.UNDERSCORE_IDENT_RE+"\\s*\\(",returnBegin:!0,relevance:0,
contains:[e.UNDERSCORE_TITLE_MODE]},{className:"type",begin:/</,end:/>/,
keywords:"reified",relevance:0},{className:"params",begin:/\(/,end:/\)/,
endsParent:!0,keywords:n,relevance:0,contains:[{begin:/:/,end:/[=,\/]/,
endsWithParent:!0,contains:[d,e.C_LINE_COMMENT_MODE,l],relevance:0
},e.C_LINE_COMMENT_MODE,l,s,o,r,e.C_NUMBER_MODE]},l]},{
begin:[/class|interface|trait/,/\s+/,e.UNDERSCORE_IDENT_RE],beginScope:{
3:"title.class"},keywords:"class interface trait",end:/[:\{(]|$/,excludeEnd:!0,
illegal:"extends implements",contains:[{
beginKeywords:"public protected internal private constructor"
},e.UNDERSCORE_TITLE_MODE,{className:"type",begin:/</,end:/>/,excludeBegin:!0,
excludeEnd:!0,relevance:0},{className:"type",begin:/[,:]\s*/,end:/[<\(,){\s]|$/,
excludeBegin:!0,returnEnd:!0},s,o]},r,{className:"meta",begin:"^#!/usr/bin/env",
end:"$",illegal:"\n"},c]}},grmr_less:e=>{
const n=Ce(e),t=Be,a="[\\w-]+",i="("+a+"|@\\{"+a+"\\})",r=[],s=[],o=e=>({
className:"string",begin:"~?"+e+".*?"+e}),c=(e,n,t)=>({className:e,begin:n,
relevance:t}),l={$pattern:/[a-z-]+/,keyword:"and or not only",
attribute:Re.join(" ")},d={begin:"\\(",end:"\\)",contains:s,keywords:l,
relevance:0}
;s.push(e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,o("'"),o('"'),n.CSS_NUMBER_MODE,{
begin:"(url|data-uri)\\(",starts:{className:"string",end:"[\\)\\n]",
excludeEnd:!0}
},n.UNICODE_RANGE,n.HEXCOLOR,d,c("variable","@@?"+a,10),c("variable","@\\{"+a+"\\}"),c("built_in","~?`[^`]*?`"),{
className:"attribute",begin:a+"\\s*:",end:":",returnBegin:!0,excludeEnd:!0
},n.IMPORTANT,{beginKeywords:"and not"},n.FUNCTION_DISPATCH);const g=s.concat({
begin:/\{/,end:/\}/,contains:r}),u={beginKeywords:"when",endsWithParent:!0,
contains:[{beginKeywords:"and not"}].concat(s)},b={begin:i+"\\s*:",
returnBegin:!0,end:/[;}]/,relevance:0,contains:[{begin:/-(webkit|moz|ms|o)-/
},n.CSS_VARIABLE,{className:"attribute",begin:"\\b("+Le.join("|")+")\\b",
end:/(?=:)/,starts:{endsWithParent:!0,illegal:"[<=$]",relevance:0,contains:s}}]
},m={className:"keyword",
begin:"@(import|media|charset|font-face|(-[a-z]+-)?keyframes|supports|document|namespace|page|viewport|host)\\b",
starts:{end:"[;{}]",keywords:l,returnEnd:!0,contains:s,relevance:0}},p={
className:"variable",variants:[{begin:"@"+a+"\\s*:",relevance:15},{begin:"@"+a
}],starts:{end:"[;}]",returnEnd:!0,contains:g}},_={variants:[{
begin:"[\\.#:&\\[>]",end:"[;{}]"},{begin:i,end:/\{/}],returnBegin:!0,
returnEnd:!0,illegal:"[<='$\"]",relevance:0,
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,u,c("keyword","all\\b"),c("variable","@\\{"+a+"\\}"),{
begin:"\\b("+Te.join("|")+")\\b",className:"selector-tag"
},n.CSS_NUMBER_MODE,c("selector-tag",i,0),c("selector-id","#"+i),c("selector-class","\\."+i,0),c("selector-tag","&",0),n.ATTRIBUTE_SELECTOR_MODE,{
className:"selector-pseudo",begin:":("+De.join("|")+")"},{
className:"selector-pseudo",begin:":(:)?("+Ie.join("|")+")"},{begin:/\(/,
end:/\)/,relevance:0,contains:g},{begin:"!important"},n.FUNCTION_DISPATCH]},h={
begin:a+":(:)?"+`(${t.join("|")})`,returnBegin:!0,contains:[_]}
;return r.push(e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,m,p,h,b,_,u,n.FUNCTION_DISPATCH),
{name:"Less",case_insensitive:!0,illegal:"[=>'/<($\"]",contains:r}},
grmr_lua:e=>{const n="\\[=*\\[",t="\\]=*\\]",a={begin:n,end:t,contains:["self"]
},i=[e.COMMENT("--(?!"+n+")","$"),e.COMMENT("--"+n,t,{contains:[a],relevance:10
})];return{name:"Lua",aliases:["pluto"],keywords:{
$pattern:e.UNDERSCORE_IDENT_RE,literal:"true false nil",
keyword:"and break do else elseif end for goto if in local global not or repeat return then until while",
built_in:"_G _ENV _VERSION __index __newindex __mode __call __metatable __tostring __len __gc __add __sub __mul __div __mod __pow __concat __unm __eq __lt __le assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawset require select setfenv setmetatable tonumber tostring type unpack xpcall arg self coroutine resume yield status wrap create running debug getupvalue debug sethook getmetatable gethook setmetatable setlocal traceback setfenv getinfo setupvalue getlocal getregistry getfenv io lines write close flush open output type read stderr stdin input stdout popen tmpfile math log max acos huge ldexp pi cos tanh pow deg tan cosh sinh random randomseed frexp ceil floor rad abs sqrt modf asin min mod fmod log10 atan2 exp sin atan os exit setlocale date getenv difftime remove time clock tmpname rename execute package preload loadlib loaded loaders cpath config path seeall string sub upper len gfind rep find match char dump gmatch reverse byte format gsub lower table setn insert getn foreachi maxn foreach concat sort remove"
},contains:i.concat([{className:"function",beginKeywords:"function",end:"\\)",
contains:[e.inherit(e.TITLE_MODE,{
begin:"([_a-zA-Z]\\w*\\.)*([_a-zA-Z]\\w*:)?[_a-zA-Z]\\w*"}),{className:"params",
begin:"\\(",endsWithParent:!0,contains:i}].concat(i)
},e.C_NUMBER_MODE,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,{className:"string",
begin:n,end:t,contains:[a],relevance:5}])}},grmr_makefile:e=>{const n={
className:"variable",variants:[{begin:"\\$\\("+e.UNDERSCORE_IDENT_RE+"\\)",
contains:[e.BACKSLASH_ESCAPE]},{begin:/\$[@%<?\^\+\*]/}]},t={className:"string",
begin:/"/,end:/"/,contains:[e.BACKSLASH_ESCAPE,n]},a={className:"variable",
begin:/\$\([\w-]+\s/,end:/\)/,keywords:{
built_in:"subst patsubst strip findstring filter filter-out sort word wordlist firstword lastword dir notdir suffix basename addsuffix addprefix join wildcard realpath abspath error warning shell origin flavor foreach if or and call eval file value"
},contains:[n,t]},i={begin:"^"+e.UNDERSCORE_IDENT_RE+"\\s*(?=[:+?]?=)"},r={
className:"section",begin:/^[^\s]+:/,end:/$/,contains:[n]};return{
name:"Makefile",aliases:["mk","mak","make"],keywords:{$pattern:/[\w-]+/,
keyword:"define endef undefine ifdef ifndef ifeq ifneq else endif include -include sinclude override export unexport private vpath"
},contains:[e.HASH_COMMENT_MODE,n,t,a,i,{className:"meta",begin:/^\.PHONY:/,
end:/$/,keywords:{$pattern:/[\.\w]+/,keyword:".PHONY"}},r]}},grmr_markdown:e=>{
const n={begin:/<\/?[A-Za-z_]/,end:">",subLanguage:"xml",relevance:0},t={
variants:[{begin:/\[.+?\]\[.*?\]/,relevance:0},{
begin:/\[.+?\]\(((data|javascript|mailto):|(?:http|ftp)s?:\/\/).*?\)/,
relevance:2},{
begin:e.regex.concat(/\[.+?\]\(/,/[A-Za-z][A-Za-z0-9+.-]*/,/:\/\/.*?\)/),
relevance:2},{begin:/\[.+?\]\([./?&#].*?\)/,relevance:1},{
begin:/\[.*?\]\(.*?\)/,relevance:0}],returnBegin:!0,contains:[{match:/\[(?=\])/
},{className:"string",relevance:0,begin:"\\[",end:"\\]",excludeBegin:!0,
returnEnd:!0},{className:"link",relevance:0,begin:"\\]\\(",end:"\\)",
excludeBegin:!0,excludeEnd:!0},{className:"symbol",relevance:0,begin:"\\]\\[",
end:"\\]",excludeBegin:!0,excludeEnd:!0}]},a={className:"strong",contains:[],
variants:[{begin:/_{2}(?!\s)/,end:/_{2}/},{begin:/\*{2}(?!\s)/,end:/\*{2}/}]
},i={className:"emphasis",contains:[],variants:[{begin:/\*(?![*\s])/,end:/\*/},{
begin:/_(?![_\s])/,end:/_/,relevance:0}]},r=e.inherit(a,{contains:[]
}),s=e.inherit(i,{contains:[]});a.contains.push(s),i.contains.push(r)
;let o=[n,t];return[a,i,r,s].forEach(e=>{e.contains=e.contains.concat(o)
}),o=o.concat(a,i),{name:"Markdown",aliases:["md","mkdown","mkd"],contains:[{
className:"section",variants:[{begin:"^#{1,6}",end:"$",contains:o},{
begin:"(?=^.+?\\n[=-]{2,}$)",contains:[{begin:"^[=-]*$"},{begin:"^",end:"\\n",
contains:o}]}]},n,{className:"bullet",begin:"^[ \t]*([*+-]|(\\d+\\.))(?=\\s+)",
end:"\\s+",excludeEnd:!0},{match:/^ {0,3}([-*_])[ \t]*(?:\1[ \t]*){2,}$/},a,i,{
className:"quote",begin:"^>\\s+",contains:o,end:"$"},{className:"code",
variants:[{begin:"(`{3,})[^`](.|\\n)*?\\1`*[ ]*"},{
begin:"(~{3,})[^~](.|\\n)*?\\1~*[ ]*"},{begin:"```",end:"```+[ ]*$"},{
begin:"~~~",end:"~~~+[ ]*$"},{begin:"`.+?`"},{begin:"(?=^( {4}|\\t))",
contains:[{begin:"^( {4}|\\t)",end:"(\\n)$"}],relevance:0}]},t,{
begin:/^\[[^\n]+\]:/,returnBegin:!0,contains:[{className:"symbol",begin:/\[/,
end:/\]/,excludeBegin:!0,excludeEnd:!0},{className:"link",begin:/:\s*/,end:/$/,
excludeBegin:!0}]},{scope:"literal",
match:/&([a-zA-Z0-9]+|#[0-9]{1,7}|#[Xx][0-9a-fA-F]{1,6});/}]}},
grmr_objectivec:e=>{const n=/[a-zA-Z@][a-zA-Z0-9_]*/,t={$pattern:n,
keyword:["@interface","@class","@protocol","@implementation"]};return{
name:"Objective-C",aliases:["mm","objc","obj-c","obj-c++","objective-c++"],
keywords:{"variable.language":["this","super"],$pattern:n,
keyword:["while","export","sizeof","typedef","const","struct","for","union","volatile","static","mutable","if","do","return","goto","enum","else","break","extern","asm","case","default","register","explicit","typename","switch","continue","inline","readonly","assign","readwrite","self","@synchronized","id","typeof","nonatomic","IBOutlet","IBAction","strong","weak","copy","in","out","inout","bycopy","byref","oneway","__strong","__weak","__block","__autoreleasing","@private","@protected","@public","@try","@property","@end","@throw","@catch","@finally","@autoreleasepool","@synthesize","@dynamic","@selector","@optional","@required","@encode","@package","@import","@defs","@compatibility_alias","__bridge","__bridge_transfer","__bridge_retained","__bridge_retain","__covariant","__contravariant","__kindof","_Nonnull","_Nullable","_Null_unspecified","__FUNCTION__","__PRETTY_FUNCTION__","__attribute__","getter","setter","retain","unsafe_unretained","nonnull","nullable","null_unspecified","null_resettable","class","instancetype","NS_DESIGNATED_INITIALIZER","NS_UNAVAILABLE","NS_REQUIRES_SUPER","NS_RETURNS_INNER_POINTER","NS_INLINE","NS_AVAILABLE","NS_DEPRECATED","NS_ENUM","NS_OPTIONS","NS_SWIFT_UNAVAILABLE","NS_ASSUME_NONNULL_BEGIN","NS_ASSUME_NONNULL_END","NS_REFINED_FOR_SWIFT","NS_SWIFT_NAME","NS_SWIFT_NOTHROW","NS_DURING","NS_HANDLER","NS_ENDHANDLER","NS_VALUERETURN","NS_VOIDRETURN"],
literal:["false","true","FALSE","TRUE","nil","YES","NO","NULL"],
built_in:["dispatch_once_t","dispatch_queue_t","dispatch_sync","dispatch_async","dispatch_once"],
type:["int","float","char","unsigned","signed","short","long","double","wchar_t","unichar","void","bool","BOOL","id|0","_Bool"]
},illegal:"</",contains:[{className:"built_in",
begin:"\\b(AV|CA|CF|CG|CI|CL|CM|CN|CT|MK|MP|MTK|MTL|NS|SCN|SK|UI|WK|XC)\\w+"
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,e.C_NUMBER_MODE,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,{
className:"string",variants:[{begin:'@"',end:'"',illegal:"\\n",
contains:[e.BACKSLASH_ESCAPE]}]},{className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma ifdef ifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(e.QUOTE_STRING_MODE,{
className:"string"}),{className:"string",begin:/<.*?>/,end:/$/,illegal:"\\n"
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{className:"class",
begin:"("+t.keyword.join("|")+")\\b",end:/(\{|$)/,excludeEnd:!0,keywords:t,
contains:[e.UNDERSCORE_TITLE_MODE]},{begin:"\\."+e.UNDERSCORE_IDENT_RE,
relevance:0}]}},grmr_perl:e=>{const n=e.regex,t=/[dualxmsipngr]{0,12}/,a={
$pattern:/[\w.]+/,
keyword:"abs accept alarm and atan2 bind binmode bless break caller chdir chmod chomp chop chown chr chroot class close closedir connect continue cos crypt dbmclose dbmopen defined delete die do dump each else elsif endgrent endhostent endnetent endprotoent endpwent endservent eof eval exec exists exit exp fcntl field fileno flock for foreach fork format formline getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname gethostent getlogin getnetbyaddr getnetbyname getnetent getpeername getpgrp getpriority getprotobyname getprotobynumber getprotoent getpwent getpwnam getpwuid getservbyname getservbyport getservent getsockname getsockopt given glob gmtime goto grep gt hex if index int ioctl join keys kill last lc lcfirst length link listen local localtime log lstat lt ma map method mkdir msgctl msgget msgrcv msgsnd my ne next no not oct open opendir or ord our pack package pipe pop pos print printf prototype push q|0 qq quotemeta qw qx rand read readdir readline readlink readpipe recv redo ref rename require reset return reverse rewinddir rindex rmdir say scalar seek seekdir select semctl semget semop send setgrent sethostent setnetent setpgrp setpriority setprotoent setpwent setservent setsockopt shift shmctl shmget shmread shmwrite shutdown sin sleep socket socketpair sort splice split sprintf sqrt srand stat state study sub substr symlink syscall sysopen sysread sysseek system syswrite tell telldir tie tied time times tr truncate uc ucfirst umask undef unless unlink unpack unshift untie until use utime values vec wait waitpid wantarray warn when while write x|0 xor y|0"
},i={className:"subst",begin:"[$@]\\{",end:"\\}",keywords:a},r={begin:/->\{/,
end:/\}/},s={scope:"attr",match:/\s+:\s*\w+(\s*\(.*?\))?/},o={scope:"variable",
variants:[{begin:/\$\d/},{
begin:n.concat(/[$%@](?!")(\^\w\b|#\w+(::\w+)*|\{\w+\}|\w+(::\w*)*)/,"(?![A-Za-z])(?![@$%])")
},{begin:/[$%@](?!")[^\s\w{=]|\$=/,relevance:0}],contains:[s]},c={
className:"number",variants:[{match:/0?\.[0-9][0-9_]+\b/},{
match:/\bv?(0|[1-9][0-9_]*(\.[0-9_]+)?|[1-9][0-9_]*)\b/},{
match:/\b0[0-7][0-7_]*\b/},{match:/\b0x[0-9a-fA-F][0-9a-fA-F_]*\b/},{
match:/\b0b[0-1][0-1_]*\b/}],relevance:0
},l=[e.BACKSLASH_ESCAPE,i,o],d=[/!/,/\//,/\|/,/\?/,/'/,/"/,/#/],g=(e,a,i="\\1")=>{
const r="\\1"===i?i:n.concat(i,a)
;return n.concat(n.concat("(?:",e,")"),a,/(?:\\.|[^\\\/])*?/,r,/(?:\\.|[^\\\/])*?/,i,t)
},u=(e,a,i)=>n.concat(n.concat("(?:",e,")"),a,/(?:\\.|[^\\\/])*?/,i,t),b=[o,e.HASH_COMMENT_MODE,e.COMMENT(/^=\w/,/=cut/,{
endsWithParent:!0}),r,{className:"string",contains:l,variants:[{
begin:"q[qwxr]?\\s*\\(",end:"\\)",relevance:5},{begin:"q[qwxr]?\\s*\\[",
end:"\\]",relevance:5},{begin:"q[qwxr]?\\s*\\{",end:"\\}",relevance:5},{
begin:"q[qwxr]?\\s*\\|",end:"\\|",relevance:5},{begin:"q[qwxr]?\\s*<",end:">",
relevance:5},{begin:"qw\\s+q",end:"q",relevance:5},{begin:"'",end:"'",
contains:[e.BACKSLASH_ESCAPE]},{begin:'"',end:'"'},{begin:"`",end:"`",
contains:[e.BACKSLASH_ESCAPE]},{begin:/\{\w+\}/,relevance:0},{
begin:"-?\\w+\\s*=>",relevance:0}]},c,{
begin:"(\\/\\/|"+e.RE_STARTERS_RE+"|\\b(split|return|print|reverse|grep)\\b)\\s*",
keywords:"split return print reverse grep",relevance:0,
contains:[e.HASH_COMMENT_MODE,{className:"regexp",variants:[{
begin:g("s|tr|y",n.either(...d,{capture:!0}))},{begin:g("s|tr|y","\\(","\\)")},{
begin:g("s|tr|y","\\[","\\]")},{begin:g("s|tr|y","\\{","\\}")}],relevance:2},{
className:"regexp",variants:[{begin:/(m|qr)\/\//,relevance:0},{
begin:u("(?:m|qr)?",/\//,/\//)},{begin:u("m|qr",n.either(...d,{capture:!0
}),/\1/)},{begin:u("m|qr",/\(/,/\)/)},{begin:u("m|qr",/\[/,/\]/)},{
begin:u("m|qr",/\{/,/\}/)}]}]},{className:"function",beginKeywords:"sub method",
end:"(\\s*\\(.*?\\))?[;{]",excludeEnd:!0,relevance:5,contains:[e.TITLE_MODE,s]
},{className:"class",beginKeywords:"class",end:"[;{]",excludeEnd:!0,relevance:5,
contains:[e.TITLE_MODE,s,c]},{begin:"-\\w\\b",relevance:0},{begin:"^__DATA__$",
end:"^__END__$",subLanguage:"mojolicious",contains:[{begin:"^@@.*",end:"$",
className:"comment"}]}];return i.contains=b,r.contains=b,{name:"Perl",
aliases:["pl","pm"],keywords:a,contains:b}},grmr_php:e=>{
const n=e.regex,t=/(?![A-Za-z0-9])(?![$])/,a=n.concat(/[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*/,t),i=n.concat(/(\\?[A-Z][a-z0-9_\x7f-\xff]+|\\?[A-Z]+(?=[A-Z][a-z0-9_\x7f-\xff])){1,}/,t),r=n.concat(/[A-Z]+/,t),s={
scope:"variable",match:"\\$+"+a},o={scope:"subst",variants:[{begin:/\$\w+/},{
begin:/\{\$/,end:/\}/}]},c=e.inherit(e.APOS_STRING_MODE,{illegal:null
}),l="[ \t\n]",d={scope:"string",variants:[e.inherit(e.QUOTE_STRING_MODE,{
illegal:null,contains:e.QUOTE_STRING_MODE.contains.concat(o)}),c,{
begin:/<<<[ \t]*(?:(\w+)|"(\w+)")\n/,end:/[ \t]*(\w+)\b/,
contains:e.QUOTE_STRING_MODE.contains.concat(o),"on:begin":(e,n)=>{
n.data._beginMatch=e[1]||e[2]},"on:end":(e,n)=>{
n.data._beginMatch!==e[1]&&n.ignoreMatch()}},e.END_SAME_AS_BEGIN({
begin:/<<<[ \t]*'(\w+)'\n/,end:/[ \t]*(\w+)\b/})]},g={scope:"number",variants:[{
begin:"\\b0[bB][01]+(?:_[01]+)*\\b"},{begin:"\\b0[oO][0-7]+(?:_[0-7]+)*\\b"},{
begin:"\\b0[xX][\\da-fA-F]+(?:_[\\da-fA-F]+)*\\b"},{
begin:"(?:\\b\\d+(?:_\\d+)*(\\.(?:\\d+(?:_\\d+)*))?|\\B\\.\\d+)(?:[eE][+-]?\\d+)?"
}],relevance:0
},u=["false","null","true"],b=["__CLASS__","__DIR__","__FILE__","__FUNCTION__","__COMPILER_HALT_OFFSET__","__LINE__","__METHOD__","__NAMESPACE__","__TRAIT__","die","echo","exit","include","include_once","print","require","require_once","array","abstract","and","as","binary","bool","boolean","break","callable","case","catch","class","clone","const","continue","declare","default","do","double","else","elseif","empty","enddeclare","endfor","endforeach","endif","endswitch","endwhile","enum","eval","extends","final","finally","float","for","foreach","from","global","goto","if","implements","instanceof","insteadof","int","integer","interface","isset","iterable","list","match|0","mixed","new","never","object","or","private","protected","public","readonly","real","return","string","switch","throw","trait","try","unset","use","var","void","while","xor","yield"],m=["Error|0","AppendIterator","ArgumentCountError","ArithmeticError","ArrayIterator","ArrayObject","AssertionError","BadFunctionCallException","BadMethodCallException","CachingIterator","CallbackFilterIterator","CompileError","Countable","DirectoryIterator","DivisionByZeroError","DomainException","EmptyIterator","ErrorException","Exception","FilesystemIterator","FilterIterator","GlobIterator","InfiniteIterator","InvalidArgumentException","IteratorIterator","LengthException","LimitIterator","LogicException","MultipleIterator","NoRewindIterator","OutOfBoundsException","OutOfRangeException","OuterIterator","OverflowException","ParentIterator","ParseError","RangeException","RecursiveArrayIterator","RecursiveCachingIterator","RecursiveCallbackFilterIterator","RecursiveDirectoryIterator","RecursiveFilterIterator","RecursiveIterator","RecursiveIteratorIterator","RecursiveRegexIterator","RecursiveTreeIterator","RegexIterator","RuntimeException","SeekableIterator","SplDoublyLinkedList","SplFileInfo","SplFileObject","SplFixedArray","SplHeap","SplMaxHeap","SplMinHeap","SplObjectStorage","SplObserver","SplPriorityQueue","SplQueue","SplStack","SplSubject","SplTempFileObject","TypeError","UnderflowException","UnexpectedValueException","UnhandledMatchError","ArrayAccess","BackedEnum","Closure","Fiber","Generator","Iterator","IteratorAggregate","Serializable","Stringable","Throwable","Traversable","UnitEnum","WeakReference","WeakMap","Directory","__PHP_Incomplete_Class","parent","php_user_filter","self","static","stdClass"],p={
keyword:b,literal:(e=>{const n=[];return e.forEach(e=>{
n.push(e),e.toLowerCase()===e?n.push(e.toUpperCase()):n.push(e.toLowerCase())}),
n})(u),built_in:m},_=e=>e.map(e=>e.replace(/\|\d+$/,"")),h={variants:[{
match:[/new/,n.concat(l,"+"),n.concat("(?!",_(m).join("\\b|"),"\\b)"),i],scope:{
1:"keyword",4:"title.class"}}]},f=n.concat(a,"\\b(?!\\()"),E={variants:[{
match:[n.concat(/::/,n.lookahead(/(?!class\b)/)),f],scope:{2:"variable.constant"
}},{match:[/::/,/class/],scope:{2:"variable.language"}},{
match:[i,n.concat(/::/,n.lookahead(/(?!class\b)/)),f],scope:{1:"title.class",
3:"variable.constant"}},{match:[i,n.concat("::",n.lookahead(/(?!class\b)/))],
scope:{1:"title.class"}},{match:[i,/::/,/class/],scope:{1:"title.class",
3:"variable.language"}}]},y={scope:"attr",
match:n.concat(a,n.lookahead(":"),n.lookahead(/(?!::)/))},w={relevance:0,
begin:/\(/,end:/\)/,keywords:p,
contains:[y,s,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g,h]
},v={relevance:0,
match:[/\b/,n.concat("(?!fn\\b|function\\b|",_(b).join("\\b|"),"|",_(m).join("\\b|"),"\\b)"),a,n.concat(l,"*"),n.lookahead(/(?=\()/)],
scope:{3:"title.function.invoke"},contains:[w]};w.contains.push(v)
;const N=[y,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g,h],k={
begin:n.concat(/#\[\s*\\?/,n.either(i,r)),beginScope:"meta",end:/]/,
endScope:"meta",keywords:{literal:u,keyword:["new","array"]},contains:[{
begin:/\[/,end:/]/,keywords:{literal:u,keyword:["new","array"]},
contains:["self",...N]},...N,{scope:"meta",variants:[{match:i},{match:r}]}]}
;return{case_insensitive:!1,keywords:p,
contains:[k,e.HASH_COMMENT_MODE,e.COMMENT("//","$"),e.COMMENT("/\\*","\\*/",{
contains:[{scope:"doctag",match:"@[A-Za-z]+"}]}),{match:/__halt_compiler\(\);/,
keywords:"__halt_compiler",starts:{scope:"comment",end:e.MATCH_NOTHING_RE,
contains:[{match:/\?>/,scope:"meta",endsParent:!0}]}},{scope:"meta",variants:[{
begin:/<\?php/,relevance:10},{begin:/<\?=/},{begin:/<\?/,relevance:.1},{
begin:/\?>/}]},{scope:"variable.language",match:/\$this\b/},s,v,E,{
match:[/const/,/\s/,a],scope:{1:"keyword",3:"variable.constant"}},h,{
scope:"function",relevance:0,beginKeywords:"fn function",end:/[;{]/,
excludeEnd:!0,illegal:"[$%\\[]",contains:[{beginKeywords:"use"
},e.UNDERSCORE_TITLE_MODE,{begin:"=>",endsParent:!0},{scope:"params",
begin:"\\(",end:"\\)",excludeBegin:!0,excludeEnd:!0,keywords:p,
contains:["self",k,s,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g]
}]},{scope:"class",variants:[{beginKeywords:"enum",illegal:/[($"]/},{
beginKeywords:"class interface trait",illegal:/[:($"]/}],relevance:0,end:/\{/,
excludeEnd:!0,contains:[{beginKeywords:"extends implements"
},e.UNDERSCORE_TITLE_MODE]},{beginKeywords:"namespace",relevance:0,end:";",
illegal:/[.']/,contains:[e.inherit(e.UNDERSCORE_TITLE_MODE,{scope:"title.class"
})]},{beginKeywords:"use",relevance:0,end:";",contains:[{
match:/\b(as|const|function)\b/,scope:"keyword"},e.UNDERSCORE_TITLE_MODE]},d,g]}
},grmr_php_template:e=>({name:"PHP template",subLanguage:"xml",contains:[{
begin:/<\?(php|=)?/,end:/\?>/,subLanguage:"php",contains:[{begin:"/\\*",
end:"\\*/",skip:!0},{begin:'b"',end:'"',skip:!0},{begin:"b'",end:"'",skip:!0
},e.inherit(e.APOS_STRING_MODE,{illegal:null,className:null,contains:null,
skip:!0}),e.inherit(e.QUOTE_STRING_MODE,{illegal:null,className:null,
contains:null,skip:!0})]}]}),grmr_plaintext:e=>({name:"Plain text",
aliases:["text","txt"],disableAutodetect:!0}),grmr_python:e=>{
const n=e.regex,t=/[\p{XID_Start}_]\p{XID_Continue}*/u,a=["and","as","assert","async","await","break","case","class","continue","def","del","elif","else","except","finally","for","from","global","if","import","in","is","lambda","lazy","match","nonlocal|10","not","or","pass","raise","return","try","while","with","yield"],i={
$pattern:/[A-Za-z]\w+|__\w+__/,keyword:a,
built_in:["__import__","abs","aiter","all","anext","any","ascii","bin","bool","breakpoint","bytearray","bytes","callable","chr","classmethod","compile","complex","delattr","dict","dir","divmod","enumerate","eval","exec","filter","float","format","frozendict","frozenset","getattr","globals","hasattr","hash","help","hex","id","input","int","isinstance","issubclass","iter","len","list","locals","map","max","memoryview","min","next","object","oct","open","ord","pow","print","property","range","repr","reversed","round","sentinel","set","setattr","slice","sorted","staticmethod","str","sum","super","tuple","type","vars","zip"],
literal:["__debug__","Ellipsis","False","None","NotImplemented","True"],
type:["Any","Callable","Coroutine","Dict","List","Literal","Generic","Optional","Sequence","Set","Tuple","Type","Union"]
},r={className:"meta",begin:/^(>>>|\.\.\.) /},s={className:"subst",begin:/\{/,
end:/\}/,keywords:i,illegal:/#/},o={begin:/\{\{/,relevance:0},c={
className:"string",contains:[e.BACKSLASH_ESCAPE],variants:[{
begin:/([uU]|[bB]|[rR]|[bB][rR]|[rR][bB])?'''/,end:/'''/,
contains:[e.BACKSLASH_ESCAPE,r],relevance:10},{
begin:/([uU]|[bB]|[rR]|[bB][rR]|[rR][bB])?"""/,end:/"""/,
contains:[e.BACKSLASH_ESCAPE,r],relevance:10},{
begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])'''/,end:/'''/,
contains:[e.BACKSLASH_ESCAPE,r,o,s]},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])"""/,
end:/"""/,contains:[e.BACKSLASH_ESCAPE,r,o,s]},{begin:/([uU]|[rR])'/,end:/'/,
relevance:10},{begin:/([uU]|[rR])"/,end:/"/,relevance:10},{
begin:/([bB]|[bB][rR]|[rR][bB])'/,end:/'/},{begin:/([bB]|[bB][rR]|[rR][bB])"/,
end:/"/},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])'/,end:/'/,
contains:[e.BACKSLASH_ESCAPE,o,s]},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])"/,
end:/"/,contains:[e.BACKSLASH_ESCAPE,o,s]
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]
},l="[0-9](_?[0-9])*",d=`(\\b(${l}))?\\.(${l})|\\b(${l})\\.`,g="\\b|"+a.join("|"),u={
className:"number",relevance:0,variants:[{
begin:`(\\b(${l})|(${d}))[eE][+-]?(${l})[jJ]?(?=${g})`},{begin:`(${d})[jJ]?`},{
begin:`\\b([1-9](_?[0-9])*|0+(_?0)*)[lLjJ]?(?=${g})`},{
begin:`\\b0[bB](_?[01])+[lL]?(?=${g})`},{begin:`\\b0[oO](_?[0-7])+[lL]?(?=${g})`
},{begin:`\\b0[xX](_?[0-9a-fA-F])+[lL]?(?=${g})`},{begin:`\\b(${l})[jJ](?=${g})`
}]},b={className:"comment",begin:n.lookahead(/# type:/),end:/$/,keywords:i,
contains:[{begin:/# type:/},{begin:/#/,end:/\b\B/,endsWithParent:!0}]},m={
className:"params",variants:[{className:"",begin:/\(\s*\)/,skip:!0},{begin:/\(/,
end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:i,
contains:["self",r,u,c,e.HASH_COMMENT_MODE]}]};return s.contains=[c,u,r],{
name:"Python",aliases:["py","gyp","ipython"],unicodeRegex:!0,keywords:i,
illegal:/(<\/|\?)|=>/,contains:[r,u,{scope:"variable.language",match:/\bself\b/
},{beginKeywords:"if",relevance:0},{match:/\bor\b/,scope:"keyword"
},c,b,e.HASH_COMMENT_MODE,{match:[/\bdef/,/\s+/,t],scope:{1:"keyword",
3:"title.function"},contains:[m]},{variants:[{
match:[/\bclass/,/\s+/,t,/\s*/,/\(\s*/,t,/\s*\)/]},{match:[/\bclass/,/\s+/,t]}],
scope:{1:"keyword",3:"title.class",6:"title.class.inherited"}},{
className:"meta",begin:/^[\t ]*@/,end:/(?=#)|$/,contains:[u,m,c]}]}},
grmr_python_repl:e=>({aliases:["pycon"],contains:[{className:"meta.prompt",
starts:{end:/ |$/,starts:{end:"$",subLanguage:"python"}},variants:[{
begin:/^>>>(?=[ ]|$)/},{begin:/^\.\.\.(?=[ ]|$)/}]}]}),grmr_r:e=>{
const n=e.regex,t=/(?:(?:[a-zA-Z]|\.[._a-zA-Z])[._a-zA-Z0-9]*)|\.(?!\d)/,a=n.either(/0[xX][0-9a-fA-F]+\.[0-9a-fA-F]*[pP][+-]?\d+i?/,/0[xX][0-9a-fA-F]+(?:[pP][+-]?\d+)?[Li]?/,/(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?[Li]?/),i=/[=!<>:]=|\|\||&&|:::?|<-|<<-|->>|->|\|>|[-+*\/?!$&|:<=>@^~]|\*\*/,r=n.either(/[()]/,/[{}]/,/\[\[/,/[[\]]/,/\\/,/,/)
;return{name:"R",keywords:{$pattern:t,
keyword:"function if in break next repeat else for while",
literal:"NULL NA TRUE FALSE Inf NaN NA_integer_|10 NA_real_|10 NA_character_|10 NA_complex_|10",
built_in:"LETTERS letters month.abb month.name pi T F abs acos acosh all any anyNA Arg as.call as.character as.complex as.double as.environment as.integer as.logical as.null.default as.numeric as.raw asin asinh atan atanh attr attributes baseenv browser c call ceiling class Conj cos cosh cospi cummax cummin cumprod cumsum digamma dim dimnames emptyenv exp expression floor forceAndCall gamma gc.time globalenv Im interactive invisible is.array is.atomic is.call is.character is.complex is.double is.environment is.expression is.finite is.function is.infinite is.integer is.language is.list is.logical is.matrix is.na is.name is.nan is.null is.numeric is.object is.pairlist is.raw is.recursive is.single is.symbol lazyLoadDBfetch length lgamma list log max min missing Mod names nargs nzchar oldClass on.exit pos.to.env proc.time prod quote range Re rep retracemem return round seq_along seq_len seq.int sign signif sin sinh sinpi sqrt standardGeneric substitute sum switch tan tanh tanpi tracemem trigamma trunc unclass untracemem UseMethod xtfrm"
},contains:[e.COMMENT(/#'/,/$/,{contains:[{scope:"doctag",match:/@examples/,
starts:{end:n.lookahead(n.either(/\n^#'\s*(?=@[a-zA-Z]+)/,/\n^(?!#')/)),
endsParent:!0}},{scope:"doctag",begin:"@param",end:/$/,contains:[{
scope:"variable",variants:[{match:t},{match:/`(?:\\.|[^`\\])+`/}],endsParent:!0
}]},{scope:"doctag",match:/@[a-zA-Z]+/},{scope:"keyword",match:/\\[a-zA-Z]+/}]
}),e.HASH_COMMENT_MODE,{scope:"string",contains:[e.BACKSLASH_ESCAPE],
variants:[e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\(/,end:/\)(-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\{/,end:/\}(-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\[/,end:/\](-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\(/,end:/\)(-*)'/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\{/,end:/\}(-*)'/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\[/,end:/\](-*)'/}),{begin:'"',end:'"',
relevance:0},{begin:"'",end:"'",relevance:0}]},{relevance:0,variants:[{scope:{
1:"operator",2:"number"},match:[i,a]},{scope:{1:"operator",2:"number"},
match:[/%[^%]*%/,a]},{scope:{1:"punctuation",2:"number"},match:[r,a]},{scope:{
2:"number"},match:[/[^a-zA-Z0-9._]|^/,a]}]},{scope:{3:"operator"},
match:[t,/\s+/,/<-/,/\s+/]},{scope:"operator",relevance:0,variants:[{match:i},{
match:/%[^%]*%/}]},{scope:"punctuation",relevance:0,match:r},{begin:"`",end:"`",
contains:[{begin:/\\./}]}]}},grmr_ruby:e=>{
const n=e.regex,t="([a-zA-Z_]\\w*[!?=]?|[-+~]@|<<|>>|=~|===?|<=>|[<>]=?|\\*\\*|[-/+%^&*~`|]|\\[\\]=?)",a=n.either(/\b([A-Z]+[a-z0-9]+)+/,/\b([A-Z]+[a-z0-9]+)+[A-Z]+/),i=n.concat(a,/(::\w+)*/),r={
"variable.constant":["__FILE__","__LINE__","__ENCODING__"],
"variable.language":["self","super"],
keyword:["alias","and","begin","BEGIN","break","case","class","defined","do","else","elsif","end","END","ensure","for","if","in","module","next","not","or","redo","require","rescue","retry","return","then","undef","unless","until","when","while","yield","include","extend","prepend","public","private","protected","raise","throw"],
built_in:["proc","lambda","attr_accessor","attr_reader","attr_writer","define_method","private_constant","module_function"],
literal:["true","false","nil"]},s={className:"doctag",begin:"@[A-Za-z]+"},o={
begin:"#<",end:">"},c=[e.COMMENT("#","$",{contains:[s]
}),e.COMMENT("^=begin","^=end",{contains:[s],relevance:10
}),e.COMMENT("^__END__",e.MATCH_NOTHING_RE)],l={className:"subst",begin:/#\{/,
end:/\}/,keywords:r},d={className:"string",contains:[e.BACKSLASH_ESCAPE,l],
variants:[{begin:/'/,end:/'/},{begin:/"/,end:/"/},{begin:/`/,end:/`/},{
begin:/%[qQwWx]?\(/,end:/\)/},{begin:/%[qQwWx]?\[/,end:/\]/},{
begin:/%[qQwWx]?\{/,end:/\}/},{begin:/%[qQwWx]?</,end:/>/},{begin:/%[qQwWx]?\//,
end:/\//},{begin:/%[qQwWx]?%/,end:/%/},{begin:/%[qQwWx]?-/,end:/-/},{
begin:/%[qQwWx]?\|/,end:/\|/},{begin:/\B\?(\\\d{1,3})/},{
begin:/\B\?(\\x[A-Fa-f0-9]{1,2})/},{begin:/\B\?(\\u\{?[A-Fa-f0-9]{1,6}\}?)/},{
begin:/\B\?(\\M-\\C-|\\M-\\c|\\c\\M-|\\M-|\\C-\\M-)[\x20-\x7e]/},{
begin:/\B\?\\(c|C-)[\x20-\x7e]/},{begin:/\B\?\\?\S/},{
begin:n.concat(/<<[-~]?'?/,n.lookahead(/(\w+)(?=\W)[^\n]*\n(?:[^\n]*\n)*?\s*\1\b/)),
contains:[e.END_SAME_AS_BEGIN({begin:/(\w+)/,end:/(\w+)/,
contains:[e.BACKSLASH_ESCAPE,l]})]}]},g="[0-9](_?[0-9])*",u={className:"number",
relevance:0,variants:[{
begin:`\\b([1-9](_?[0-9])*|0)(\\.(${g}))?([eE][+-]?(${g})|r)?i?\\b`},{
begin:"\\b0[dD][0-9](_?[0-9])*r?i?\\b"},{begin:"\\b0[bB][0-1](_?[0-1])*r?i?\\b"
},{begin:"\\b0[oO][0-7](_?[0-7])*r?i?\\b"},{
begin:"\\b0[xX][0-9a-fA-F](_?[0-9a-fA-F])*r?i?\\b"},{
begin:"\\b0(_?[0-7])+r?i?\\b"}]},b={variants:[{match:/\(\)/},{
className:"params",begin:/\(/,end:/(?=\))/,excludeBegin:!0,endsParent:!0,
keywords:r}]},m=[d,{variants:[{match:[/class\s+/,i,/\s+<\s+/,i]},{
match:[/\b(class|module)\s+/,i]}],scope:{2:"title.class",
4:"title.class.inherited"},keywords:r},{match:[/(include|extend)\s+/,i],scope:{
2:"title.class"},keywords:r},{relevance:0,match:[i,/\.new[. (]/],scope:{
1:"title.class"}},{relevance:0,match:/\b[A-Z][A-Z_0-9]+\b/,
className:"variable.constant"},{relevance:0,match:a,scope:"title.class"},{
match:[/def/,/\s+/,t],scope:{1:"keyword",3:"title.function"},contains:[b]},{
begin:"::"},{className:"symbol",begin:e.UNDERSCORE_IDENT_RE+"(!|\\?)?:",
relevance:0},{className:"symbol",begin:":(?!\\s)",contains:[d,{begin:t}],
relevance:0},u,{className:"variable",
begin:"(\\$\\W)|((\\$|@@?)(\\w+))(?=[^@$?])(?![A-Za-z])(?![@$?'])"},{
className:"params",begin:/\|(?!=)/,end:/\|/,excludeBegin:!0,excludeEnd:!0,
relevance:0,keywords:r},{begin:"("+e.RE_STARTERS_RE+"|unless)\\s*",
keywords:"unless",contains:[{className:"regexp",contains:[e.BACKSLASH_ESCAPE,l],
illegal:/\n/,variants:[{begin:"/",end:"/[a-z]*"},{begin:/%r\{/,end:/\}[a-z]*/},{
begin:"%r\\(",end:"\\)[a-z]*"},{begin:"%r!",end:"![a-z]*"},{begin:"%r\\[",
end:"\\][a-z]*"}]}].concat(o,c),relevance:0}].concat(o,c)
;l.contains=m,b.contains=m;const p=[{begin:/^\s*=>/,starts:{end:"$",contains:m}
},{className:"meta.prompt",
begin:"^([>?]>|[\\w#]+\\(\\w+\\):\\d+:\\d+[>*]|(\\w+-)?\\d+\\.\\d+\\.\\d+(p\\d+)?[^\\d][^>]+>)(?=[ ])",
starts:{end:"$",keywords:r,contains:m}}];return c.unshift(o),{name:"Ruby",
aliases:["rb","gemspec","podspec","thor","irb"],keywords:r,illegal:/\/\*/,
contains:[e.SHEBANG({binary:"ruby"})].concat(p).concat(c).concat(m)}},
grmr_rust:e=>{
const n=e.regex,t=/(r#)?/,a=n.concat(t,e.UNDERSCORE_IDENT_RE),i=n.concat(t,e.IDENT_RE),r={
scope:"title.function.invoke",relevance:0,
begin:n.concat(/\b/,/(?!(?:let|for|while|if|else|match)\b)/,i,n.lookahead(/\s*\(/))
},s="([ui](8|16|32|64|128|size)|f(16|32|64|128))?",o=["drop ","Copy","Send","Sized","Sync","Drop","Fn","FnMut","FnOnce","ToOwned","Clone","Debug","PartialEq","PartialOrd","Eq","Ord","AsRef","AsMut","Into","From","Default","Iterator","Extend","IntoIterator","DoubleEndedIterator","ExactSizeIterator","SliceConcatExt","ToString","assert!","assert_eq!","bitflags!","bytes!","cfg!","col!","concat!","concat_idents!","debug_assert!","debug_assert_eq!","env!","eprintln!","panic!","file!","format!","format_args!","include_bytes!","include_str!","line!","local_data_key!","module_path!","option_env!","print!","println!","select!","stringify!","try!","unimplemented!","unreachable!","vec!","write!","writeln!","macro_rules!","assert_ne!","debug_assert_ne!"],c=["i8","i16","i32","i64","i128","isize","u8","u16","u32","u64","u128","usize","f16","f32","f64","f128","str","char","bool","Box","Option","Result","String","Vec"]
;return{name:"Rust",aliases:["rs"],keywords:{$pattern:e.IDENT_RE+"!?",type:c,
keyword:["abstract","as","async","await","become","box","break","const","continue","crate","do","dyn","else","enum","extern","false","final","fn","for","if","impl","in","let","loop","macro","match","mod","move","mut","override","priv","pub","raw","ref","return","self","Self","static","struct","super","trait","true","try","type","typeof","union","unsafe","unsized","use","virtual","where","while","yield"],
literal:["true","false","Some","None","Ok","Err"],built_in:o},illegal:"</",
contains:[e.C_LINE_COMMENT_MODE,e.COMMENT("/\\*","\\*/",{contains:["self"]
}),e.inherit(e.QUOTE_STRING_MODE,{begin:/b?"/,illegal:null}),{scope:"symbol",
begin:/'[a-zA-Z_][a-zA-Z0-9_]*(?!')/},{scope:"string",variants:[{
begin:/b?r(#*)"(.|\n)*?"\1(?!#)/},{begin:/b?'/,end:/'/,contains:[{
scope:"char.escape",match:/\\('|"|\\|\w|x\w{2}|u\w{4}|U\w{8})/}]}]},{
scope:"number",variants:[{begin:"\\b0b([01_]+)"+s},{begin:"\\b0o([0-7_]+)"+s},{
begin:"\\b0x([A-Fa-f0-9_]+)"+s},{
begin:"\\b(\\d[\\d_]*(\\.[0-9_]+)?([eE][+-]?[0-9_]+)?)"+s}],relevance:0},{
begin:[/\bsafe/,/\s+/,/extern/],scope:{1:"keyword",3:"keyword"}},{
begin:[/fn/,/\s+/,a],scope:{1:"keyword",3:"title.function"}},{scope:"meta",
begin:"#!?\\[",end:"\\]",contains:[{scope:"string",begin:/"/,end:/"/,
contains:[e.BACKSLASH_ESCAPE]}]},{begin:[/let/,/\s+/,/(?:mut\s+)?/,a],scope:{
1:"keyword",3:"keyword",4:"variable"}},{begin:[/for/,/\s+/,a,/\s+/,/in/],scope:{
1:"keyword",3:"variable",5:"keyword"}},{begin:[/type/,/\s+/,a],scope:{
1:"keyword",3:"title.class"}},{
begin:[/(?:trait|enum|struct|union|impl|for)/,/\s+/,a],scope:{1:"keyword",
3:"title.class"}},{begin:e.IDENT_RE+"::",keywords:{keyword:"Self",built_in:o,
type:c}},{scope:"punctuation",begin:"->"},r]}},grmr_scss:e=>{
const n=Ce(e),t=Ie,a=De,i="@[a-z-]+",r={className:"variable",
begin:"(\\$[a-zA-Z-][a-zA-Z0-9_-]*)\\b",relevance:0};return{name:"SCSS",
case_insensitive:!0,illegal:"[=/|']",
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,n.CSS_NUMBER_MODE,{
className:"selector-id",begin:"#[A-Za-z0-9_-]+",relevance:0},{
className:"selector-class",begin:"\\.[A-Za-z0-9_-]+",relevance:0
},n.ATTRIBUTE_SELECTOR_MODE,{className:"selector-tag",
begin:"\\b("+Te.join("|")+")\\b",relevance:0},{className:"selector-pseudo",
begin:":("+a.join("|")+")"},{className:"selector-pseudo",
begin:":(:)?("+t.join("|")+")"},r,{begin:/\(/,end:/\)/,
contains:[n.CSS_NUMBER_MODE]},n.CSS_VARIABLE,{className:"attribute",
begin:"\\b("+Le.join("|")+")\\b"},{
begin:"\\b(whitespace|wait|w-resize|visible|vertical-text|vertical-ideographic|uppercase|upper-roman|upper-alpha|underline|transparent|top|thin|thick|text|text-top|text-bottom|tb-rl|table-header-group|table-footer-group|sw-resize|super|strict|static|square|solid|small-caps|separate|se-resize|scroll|s-resize|rtl|row-resize|ridge|right|repeat|repeat-y|repeat-x|relative|progress|pointer|overline|outside|outset|oblique|nowrap|not-allowed|normal|none|nw-resize|no-repeat|no-drop|newspaper|ne-resize|n-resize|move|middle|medium|ltr|lr-tb|lowercase|lower-roman|lower-alpha|loose|list-item|line|line-through|line-edge|lighter|left|keep-all|justify|italic|inter-word|inter-ideograph|inside|inset|inline|inline-block|inherit|inactive|ideograph-space|ideograph-parenthesis|ideograph-numeric|ideograph-alpha|horizontal|hidden|help|hand|groove|fixed|ellipsis|e-resize|double|dotted|distribute|distribute-space|distribute-letter|distribute-all-lines|disc|disabled|default|decimal|dashed|crosshair|collapse|col-resize|circle|char|center|capitalize|break-word|break-all|bottom|both|bolder|bold|block|bidi-override|below|baseline|auto|always|all-scroll|absolute|table|table-cell)\\b"
},{begin:/:/,end:/[;}{]/,relevance:0,
contains:[n.BLOCK_COMMENT,r,n.HEXCOLOR,n.CSS_NUMBER_MODE,n.UNICODE_RANGE,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,n.IMPORTANT,n.FUNCTION_DISPATCH]
},{begin:"@(page|font-face)",keywords:{$pattern:i,keyword:"@page @font-face"}},{
begin:"@",end:"[{;]",returnBegin:!0,keywords:{$pattern:/[a-z-]+/,
keyword:"and or not only",attribute:Re.join(" ")},contains:[{begin:i,
className:"keyword"},{begin:/[a-z-]+(?=:)/,className:"attribute"
},r,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,n.HEXCOLOR,n.CSS_NUMBER_MODE]
},n.FUNCTION_DISPATCH]}},grmr_shell:e=>({name:"Shell Session",
aliases:["console","shellsession"],contains:[{className:"meta.prompt",
begin:/^\s{0,3}[./~\w\d[\]()@-]*[>%$#][ ]?/,starts:{end:/[^\\](?=\s*$)/,
subLanguage:"bash"}}]}),grmr_sql:e=>{
const n=e.regex,t=e.COMMENT("--","$"),a=["abs","acos","array_agg","asin","atan","avg","cast","ceil","ceiling","coalesce","corr","cos","cosh","count","covar_pop","covar_samp","cume_dist","dense_rank","deref","element","exp","extract","first_value","floor","json_array","json_arrayagg","json_exists","json_object","json_objectagg","json_query","json_table","json_table_primitive","json_value","lag","last_value","lead","listagg","ln","log","log10","lower","max","min","mod","nth_value","ntile","nullif","percent_rank","percentile_cont","percentile_disc","position","position_regex","power","rank","regr_avgx","regr_avgy","regr_count","regr_intercept","regr_r2","regr_slope","regr_sxx","regr_sxy","regr_syy","row_number","sin","sinh","sqrt","stddev_pop","stddev_samp","substring","substring_regex","sum","tan","tanh","translate","translate_regex","treat","trim","trim_array","unnest","upper","value_of","var_pop","var_samp","width_bucket"],i=a,r=["abs","acos","all","allocate","alter","and","any","are","array","array_agg","array_max_cardinality","as","asensitive","asin","asymmetric","at","atan","atomic","authorization","avg","begin","begin_frame","begin_partition","between","bigint","binary","blob","boolean","both","by","call","called","cardinality","cascaded","case","cast","ceil","ceiling","char","char_length","character","character_length","check","classifier","clob","close","coalesce","collate","collect","column","commit","condition","connect","constraint","contains","convert","copy","corr","corresponding","cos","cosh","count","covar_pop","covar_samp","create","cross","cube","cume_dist","current","current_catalog","current_date","current_default_transform_group","current_path","current_role","current_row","current_schema","current_time","current_timestamp","current_path","current_role","current_transform_group_for_type","current_user","cursor","cycle","date","day","deallocate","dec","decimal","decfloat","declare","default","define","delete","dense_rank","deref","describe","deterministic","disconnect","distinct","double","drop","dynamic","each","element","else","empty","end","end_frame","end_partition","end-exec","equals","escape","every","except","exec","execute","exists","exp","external","extract","false","fetch","filter","first_value","float","floor","for","foreign","frame_row","free","from","full","function","fusion","get","global","grant","group","grouping","groups","having","hold","hour","identity","in","indicator","initial","inner","inout","insensitive","insert","int","integer","intersect","intersection","interval","into","is","join","json_array","json_arrayagg","json_exists","json_object","json_objectagg","json_query","json_table","json_table_primitive","json_value","lag","language","large","last_value","lateral","lead","leading","left","like","like_regex","listagg","ln","local","localtime","localtimestamp","log","log10","lower","match","match_number","match_recognize","matches","max","member","merge","method","min","minute","mod","modifies","module","month","multiset","national","natural","nchar","nclob","new","no","none","normalize","not","nth_value","ntile","null","nullif","numeric","octet_length","occurrences_regex","of","offset","old","omit","on","one","only","open","or","order","out","outer","over","overlaps","overlay","parameter","partition","pattern","per","percent","percent_rank","percentile_cont","percentile_disc","period","portion","position","position_regex","power","precedes","precision","prepare","primary","procedure","ptf","range","rank","reads","real","recursive","ref","references","referencing","regr_avgx","regr_avgy","regr_count","regr_intercept","regr_r2","regr_slope","regr_sxx","regr_sxy","regr_syy","release","result","return","returns","revoke","right","rollback","rollup","row","row_number","rows","running","savepoint","scope","scroll","search","second","seek","select","sensitive","session_user","set","show","similar","sin","sinh","skip","smallint","some","specific","specifictype","sql","sqlexception","sqlstate","sqlwarning","sqrt","start","static","stddev_pop","stddev_samp","submultiset","subset","substring","substring_regex","succeeds","sum","symmetric","system","system_time","system_user","table","tablesample","tan","tanh","then","time","timestamp","timezone_hour","timezone_minute","to","trailing","translate","translate_regex","translation","treat","trigger","trim","trim_array","true","truncate","uescape","union","unique","unknown","unnest","update","upper","user","using","value","values","value_of","var_pop","var_samp","varbinary","varchar","varying","versioning","when","whenever","where","width_bucket","window","with","within","without","year","add","asc","collation","desc","final","first","last","view"].filter(e=>!a.includes(e)),s={
match:n.concat(/\b/,n.either(...i),/\s*\(/),relevance:0,keywords:{built_in:i}}
;function o(e){
return n.concat(/\b/,n.either(...e.map(e=>e.replace(/\s+/,"\\s+"))),/\b/)}
const c={scope:"keyword",
match:o(["create table","insert into","primary key","foreign key","not null","alter table","add constraint","grouping sets","on overflow","character set","respect nulls","ignore nulls","nulls first","nulls last","depth first","breadth first"]),
relevance:0};return{name:"SQL",case_insensitive:!0,illegal:/[{}]|<\//,keywords:{
$pattern:/\b[\w\.]+/,keyword:((e,{exceptions:n,when:t}={})=>{const a=t
;return n=n||[],e.map(e=>e.match(/\|\d+$/)||n.includes(e)?e:a(e)?e+"|0":e)})(r,{
when:e=>e.length<3}),literal:["true","false","unknown"],
type:["bigint","binary","blob","boolean","char","character","clob","date","dec","decfloat","decimal","float","int","integer","interval","nchar","nclob","national","numeric","real","row","smallint","time","timestamp","varchar","varying","varbinary"],
built_in:["current_catalog","current_date","current_default_transform_group","current_path","current_role","current_schema","current_transform_group_for_type","current_user","session_user","system_time","system_user","current_time","localtime","current_timestamp","localtimestamp"]
},contains:[{scope:"type",
match:o(["double precision","large object","with timezone","without timezone"])
},c,s,{scope:"variable",match:/@[a-z0-9][a-z0-9_]*/},{scope:"string",variants:[{
begin:/'/,end:/'/,contains:[{match:/''/}]}]},{begin:/"/,end:/"/,contains:[{
match:/""/}]},e.C_NUMBER_MODE,e.C_BLOCK_COMMENT_MODE,t,{scope:"operator",
match:/[-+*/=%^~]|&&?|\|\|?|!=?|<(?:=>?|<|>)?|>[>=]?/,relevance:0}]}},
grmr_swift:e=>{const n={match:/\s+/,relevance:0},t=e.COMMENT("/\\*","\\*/",{
contains:["self"]}),a=[e.C_LINE_COMMENT_MODE,t],i={match:[/\./,m(...be,...me)],
className:{2:"keyword"}},r={match:b(/\./,m(..._e)),relevance:0
},s=_e.filter(e=>"string"==typeof e).concat(["_|0"]),o={variants:[{
className:"keyword",
match:m(..._e.filter(e=>"string"!=typeof e).concat(pe).map(ue),...me)}]},c={
$pattern:m(/\b\w+/,/#\w+/),keyword:s.concat(Ee),literal:he},l=[i,r,o],g=[{
match:b(/\./,m(...ye)),relevance:0},{className:"built_in",
match:b(/\b/,m(...ye),/(?=\()/)}],u={match:/->/,relevance:0},p=[u,{
className:"operator",relevance:0,variants:[{match:Ne},{match:`\\.(\\.|${ve})+`}]
}],_="([0-9]_*)+",h="([0-9a-fA-F]_*)+",f={className:"number",relevance:0,
variants:[{match:`\\b(${_})(\\.(${_}))?([eE][+-]?(${_}))?\\b`},{
match:`\\b0x(${h})(\\.(${h}))?([pP][+-]?(${_}))?\\b`},{match:/\b0o([0-7]_*)+\b/
},{match:/\b0b([01]_*)+\b/}]},E=(e="")=>({className:"subst",variants:[{
match:b(/\\/,e,/[0\\tnr"']/)},{match:b(/\\/,e,/u\{[0-9a-fA-F]{1,8}\}/)}]
}),y=(e="")=>({className:"subst",match:b(/\\/,e,/[\t ]*(?:[\r\n]|\r\n)/)
}),w=(e="")=>({className:"subst",label:"interpol",begin:b(/\\/,e,/\(/),end:/\)/
}),v=(e="")=>({begin:b(e,/"""/),end:b(/"""/,e),contains:[E(e),y(e),w(e)]
}),N=(e="")=>({begin:b(e,/"/),end:b(/"/,e),contains:[E(e),w(e)]}),k={
className:"string",
variants:[v(),v("#"),v("##"),v("###"),N(),N("#"),N("##"),N("###")]
},x=[e.BACKSLASH_ESCAPE,{begin:/\[/,end:/\]/,relevance:0,
contains:[e.BACKSLASH_ESCAPE]}],O={begin:/\/[^\s](?=[^/\n]*\/)/,end:/\//,
contains:x},M=e=>{const n=b(e,/\//),t=b(/\//,e);return{begin:n,end:t,
contains:[...x,{scope:"comment",begin:`#(?!.*${t})`,end:/$/}]}},A={
scope:"regexp",variants:[M("###"),M("##"),M("#"),O]},S={match:b(/`/,Oe,/`/)
},C=[S,{className:"variable",match:/\$\d+/},{className:"variable",
match:`\\$${xe}+`}],T=[{match:/(@|#(un)?)available/,scope:"keyword",starts:{
contains:[{begin:/\(/,end:/\)/,keywords:Se,contains:[...p,f,k]}]}},{
scope:"keyword",match:b(/@/,m(...Ae),d(m(/\(/,/\s+/)))},{scope:"meta",
match:b(/@/,Oe)}],R={match:d(/\b[A-Z]/),relevance:0,contains:[{className:"type",
match:b(/(AV|CA|CF|CG|CI|CL|CM|CN|CT|MK|MP|MTK|MTL|NS|SCN|SK|UI|WK|XC)/,xe,"+")
},{className:"type",match:Me,relevance:0},{match:/[?!]+/,relevance:0},{
match:/\.\.\./,relevance:0},{match:b(/\s+&\s+/,d(Me)),relevance:0}]},D={
begin:/</,end:/>/,keywords:c,contains:[...a,...l,...T,u,R]};R.contains.push(D)
;const I={begin:/\(/,end:/\)/,relevance:0,keywords:c,contains:["self",{
match:b(Oe,/\s*:/),keywords:"_|0",relevance:0
},...a,A,...l,...g,...p,f,k,...C,...T,R]},L={begin:/</,end:/>/,
keywords:"repeat each",contains:[...a,R]},B={begin:/\(/,end:/\)/,keywords:c,
contains:[{begin:m(d(b(Oe,/\s*:/)),d(b(Oe,/\s+/,Oe,/\s*:/))),end:/:/,
relevance:0,contains:[{className:"keyword",match:/\b_\b/},{className:"params",
match:Oe}]},...a,...l,...p,f,k,...T,R,I],endsParent:!0,illegal:/["']/},F={
match:[/(func|macro)/,/\s+/,m(S.match,Oe,Ne)],className:{1:"keyword",
3:"title.function"},contains:[L,B,n],illegal:[/\[/,/%/]},$={
match:[/\b(?:subscript|init[?!]?)/,/\s*(?=[<(])/],className:{1:"keyword"},
contains:[L,B,n],illegal:/\[|%/},z={match:[/operator/,/\s+/,Ne],className:{
1:"keyword",3:"title"}},j={begin:[/precedencegroup/,/\s+/,Me],className:{
1:"keyword",3:"title"},contains:[R],keywords:[...fe,...he],end:/}/},U={
begin:[/(struct|protocol|class|extension|enum|actor)/,/\s+/,Oe,/\s*/],
beginScope:{1:"keyword",3:"title.class"},keywords:c,contains:[L,...l,{begin:/:/,
end:/\{/,keywords:c,contains:[{scope:"title.class.inherited",match:Me},...l],
relevance:0}]};for(const e of k.variants){
const n=e.contains.find(e=>"interpol"===e.label);n.keywords=c
;const t=[...l,...g,...p,f,k,...C];n.contains=[...t,{begin:/\(/,end:/\)/,
contains:["self",...t]}]}return{name:"Swift",keywords:c,contains:[...a,F,$,{
match:[/class\b/,/\s+/,/func\b/,/\s+/,/\b[A-Za-z_][A-Za-z0-9_]*\b/],scope:{
1:"keyword",3:"keyword",5:"title.function"}},{match:[/class\b/,/\s+/,/var\b/],
scope:{1:"keyword",3:"keyword"}},U,z,j,{beginKeywords:"import",end:/$/,
contains:[...a],relevance:0},A,...l,...g,...p,f,k,...C,...T,R,I]}},
grmr_typescript:e=>{
const n=e.regex,t=ge(e),a=te,i=["any","void","number","boolean","string","object","never","symbol","bigint","unknown"],r={
begin:[/namespace/,/\s+/,e.IDENT_RE],beginScope:{1:"keyword",3:"title.class"}
},s={beginKeywords:"interface",end:/\{/,excludeEnd:!0,keywords:{
keyword:"interface extends",built_in:i},contains:[t.exports.CLASS_REFERENCE]
},o={$pattern:te,
keyword:ie.concat(["type","interface","public","private","protected","implements","declare","abstract","readonly","enum","override","satisfies"]),
literal:re,built_in:de.concat(i),"variable.language":le},c={className:"meta",
begin:"@"+a},l=(e,n,t)=>{const a=e.contains.findIndex(e=>e.label===n)
;if(-1===a)throw Error("can not find mode to replace");e.contains.splice(a,1,t)}
;Object.assign(t.keywords,o),t.exports.PARAMS_CONTAINS.push(c)
;const d=t.contains.find(e=>"attr"===e.scope),g=Object.assign({},d,{
match:n.concat(a,n.lookahead(/\s*\?:/))})
;return t.exports.PARAMS_CONTAINS.push([t.exports.CLASS_REFERENCE,d,g]),
t.contains=t.contains.concat([c,r,s,g]),
l(t,"shebang",e.SHEBANG()),l(t,"use_strict",{className:"meta",relevance:10,
begin:/^\s*['"]use strict['"]/
}),t.contains.find(e=>"func.def"===e.label).relevance=0,Object.assign(t,{
name:"TypeScript",aliases:["ts","tsx","mts","cts"]}),t},grmr_vbnet:e=>{
const n=e.regex,t=/\d{1,2}\/\d{1,2}\/\d{4}/,a=/\d{4}-\d{1,2}-\d{1,2}/,i=/(\d|1[012])(:\d+){0,2} *(AM|PM)/,r=/\d{1,2}(:\d{1,2}){1,2}/,s={
className:"literal",variants:[{begin:n.concat(/# */,n.either(a,t),/ *#/)},{
begin:n.concat(/# */,r,/ *#/)},{begin:n.concat(/# */,i,/ *#/)},{
begin:n.concat(/# */,n.either(a,t),/ +/,n.either(i,r),/ *#/)}]
},o=e.COMMENT(/'''/,/$/,{contains:[{className:"doctag",begin:/<\/?/,end:/>/}]
}),c=e.COMMENT(null,/$/,{variants:[{begin:/'/},{begin:/([\t ]|^)REM(?=\s)/}]})
;return{name:"Visual Basic .NET",aliases:["vb"],case_insensitive:!0,
classNameAliases:{label:"symbol"},keywords:{
keyword:"addhandler alias aggregate ansi as async assembly auto binary by byref byval call case catch class compare const continue custom declare default delegate dim distinct do each equals else elseif end enum erase error event exit explicit finally for friend from function get global goto group handles if implements imports in inherits interface into iterator join key let lib loop me mid module mustinherit mustoverride mybase myclass namespace narrowing new next notinheritable notoverridable of off on operator option optional order overloads overridable overrides paramarray partial preserve private property protected public raiseevent readonly redim removehandler resume return select set shadows shared skip static step stop structure strict sub synclock take text then throw to try unicode until using when where while widening with withevents writeonly yield",
built_in:"addressof and andalso await directcast gettype getxmlnamespace is isfalse isnot istrue like mod nameof new not or orelse trycast typeof xor cbool cbyte cchar cdate cdbl cdec cint clng cobj csbyte cshort csng cstr cuint culng cushort",
type:"boolean byte char date decimal double integer long object sbyte short single string uinteger ulong ushort",
literal:"true false nothing"},
illegal:"//|\\{|\\}|endif|gosub|variant|wend|^\\$ ",contains:[{
className:"string",begin:/"(""|[^/n])"C\b/},{className:"string",begin:/"/,
end:/"/,illegal:/\n/,contains:[{begin:/""/}]},s,{className:"number",relevance:0,
variants:[{begin:/\b\d[\d_]*((\.[\d_]+(E[+-]?[\d_]+)?)|(E[+-]?[\d_]+))[RFD@!#]?/
},{begin:/\b\d[\d_]*((U?[SIL])|[%&])?/},{begin:/&H[\dA-F_]+((U?[SIL])|[%&])?/},{
begin:/&O[0-7_]+((U?[SIL])|[%&])?/},{begin:/&B[01_]+((U?[SIL])|[%&])?/}]},{
className:"label",begin:/^\w+:/},o,c,{className:"meta",
begin:/[\t ]*#(const|disable|else|elseif|enable|end|externalsource|if|region)\b/,
end:/$/,keywords:{
keyword:"const disable else elseif enable end externalsource if region then"},
contains:[c]}]}},grmr_wasm:e=>{e.regex;const n=e.COMMENT(/\(;/,/;\)/)
;return n.contains.push("self"),{name:"WebAssembly",keywords:{$pattern:/[\w.]+/,
keyword:["anyfunc","block","br","br_if","br_table","call","call_indirect","data","drop","elem","else","end","export","func","global.get","global.set","local.get","local.set","local.tee","get_global","get_local","global","if","import","local","loop","memory","memory.grow","memory.size","module","mut","nop","offset","param","result","return","select","set_global","set_local","start","table","tee_local","then","type","unreachable"]
},contains:[e.COMMENT(/;;/,/$/),n,{match:[/(?:offset|align)/,/\s*/,/=/],
className:{1:"keyword",3:"operator"}},{className:"variable",begin:/\$[\w_]+/},{
match:/(\((?!;)|\))+/,className:"punctuation",relevance:0},{
begin:[/(?:func|call|call_indirect)/,/\s+/,/\$[^\s)]+/],className:{1:"keyword",
3:"title.function"}},e.QUOTE_STRING_MODE,{match:/(i32|i64|f32|f64)(?!\.)/,
className:"type"},{className:"keyword",
match:/\b(f32|f64|i32|i64)(?:\.(?:abs|add|and|ceil|clz|const|convert_[su]\/i(?:32|64)|copysign|ctz|demote\/f64|div(?:_[su])?|eqz?|extend_[su]\/i32|floor|ge(?:_[su])?|gt(?:_[su])?|le(?:_[su])?|load(?:(?:8|16|32)_[su])?|lt(?:_[su])?|max|min|mul|nearest|neg?|or|popcnt|promote\/f32|reinterpret\/[fi](?:32|64)|rem_[su]|rot[lr]|shl|shr_[su]|store(?:8|16|32)?|sqrt|sub|trunc(?:_[su]\/f(?:32|64))?|wrap\/i64|xor))\b/
},{className:"number",relevance:0,
match:/[+-]?\b(?:\d(?:_?\d)*(?:\.\d(?:_?\d)*)?(?:[eE][+-]?\d(?:_?\d)*)?|0x[\da-fA-F](?:_?[\da-fA-F])*(?:\.[\da-fA-F](?:_?[\da-fA-D])*)?(?:[pP][+-]?\d(?:_?\d)*)?)\b|\binf\b|\bnan(?::0x[\da-fA-F](?:_?[\da-fA-D])*)?\b/
}]}},grmr_xml:e=>{
const n=e.regex,t=n.concat(/[\p{L}_]/u,n.optional(/[\p{L}0-9_.-]*:/u),/[\p{L}0-9_.-]*/u),a={
className:"symbol",begin:/&[a-z]+;|&#[0-9]+;|&#x[a-f0-9]+;/},i={begin:/\s/,
contains:[{className:"keyword",begin:/#?[a-z_][a-z1-9_-]+/,illegal:/\n/}]
},r=e.inherit(i,{begin:/\(/,end:/\)/}),s=e.inherit(e.APOS_STRING_MODE,{
className:"string"}),o=e.inherit(e.QUOTE_STRING_MODE,{className:"string"}),c={
endsWithParent:!0,illegal:/</,relevance:0,contains:[{className:"attr",
begin:/[\p{L}0-9._:-]+/u,relevance:0},{begin:/=\s*/,relevance:0,contains:[{
className:"string",endsParent:!0,variants:[{begin:/"/,end:/"/,contains:[a]},{
begin:/'/,end:/'/,contains:[a]},{begin:/[^\s"'=<>`]+/}]}]}]};return{
name:"HTML, XML",
aliases:["html","xhtml","rss","atom","xjb","xsd","xsl","plist","wsf","svg"],
case_insensitive:!0,unicodeRegex:!0,contains:[{className:"meta",begin:/<![a-z]/,
end:/>/,relevance:10,contains:[i,o,s,r,{begin:/\[/,end:/\]/,contains:[{
className:"meta",begin:/<![a-z]/,end:/>/,contains:[i,r,o,s]}]}]
},e.COMMENT(/<!--/,/-->/,{relevance:10}),{begin:/<!\[CDATA\[/,end:/\]\]>/,
relevance:10},a,{className:"meta",end:/\?>/,variants:[{begin:/<\?xml/,
relevance:10,contains:[o]},{begin:/<\?[a-z][a-z0-9]+/}]},{className:"tag",
begin:/<style(?=\s|>)/,end:/>/,keywords:{name:"style"},contains:[c],starts:{
end:/<\/style>/,returnEnd:!0,subLanguage:"css"}},{className:"tag",
begin:/<script(?=\s|>)/,end:/>/,keywords:{name:"script"},contains:[c],starts:{
end:/<\/script>/,returnEnd:!0,subLanguage:"javascript"}},{className:"tag",
begin:/<>|<\/>/},{className:"tag",
begin:n.concat(/</,n.lookahead(n.concat(t,n.either(/\/>/,/>/,/\s/)))),
end:/\/?>/,contains:[{className:"name",begin:t,relevance:0,starts:c}]},{
className:"tag",begin:n.concat(/<\//,n.lookahead(n.concat(t,/>/))),contains:[{
className:"name",begin:t,relevance:0},{begin:/>/,relevance:0,endsParent:!0}]}]}
},grmr_yaml:e=>{
const n="true false yes no null",t="[\\w#;/?:@&=+$,.~*'()[\\]]+",a={
className:"string",relevance:0,variants:[{begin:/"/,end:/"/},{begin:/\S+/}],
contains:[e.BACKSLASH_ESCAPE,{className:"template-variable",variants:[{
begin:/\{\{/,end:/\}\}/},{begin:/%\{/,end:/\}/}]}]},i=e.inherit(a,{variants:[{
begin:/'/,end:/'/,contains:[{begin:/''/,relevance:0}]},{begin:/"/,end:/"/},{
begin:/[^\s,{}[\]]+/}]}),r={end:",",endsWithParent:!0,excludeEnd:!0,keywords:n,
relevance:0},s={begin:/\{/,end:/\}/,contains:[r],illegal:"\\n",relevance:0},o={
begin:"\\[",end:"\\]",contains:[r],illegal:"\\n",relevance:0},c=[{
className:"attr",variants:[{begin:/[\w*@][\w*@ :()\./-]*:(?=[ \t]|$)/},{
begin:/"[\w*@][\w*@ :()\./-]*":(?=[ \t]|$)/},{
begin:/'[\w*@][\w*@ :()\./-]*':(?=[ \t]|$)/}]},{className:"meta",
begin:"^---\\s*$",relevance:10},{className:"string",
begin:"[\\|>]([1-9]?[+-])?[ ]*\\n( +)[^ ][^\\n]*\\n(\\2[^\\n]+\\n?)*"},{
begin:"<%[%=-]?",end:"[%-]?%>",subLanguage:"ruby",excludeBegin:!0,excludeEnd:!0,
relevance:0},{className:"type",begin:"!\\w+!"+t},{className:"type",
begin:"!<"+t+">"},{className:"type",begin:"!"+t},{className:"type",begin:"!!"+t
},{className:"meta",begin:"&"+e.UNDERSCORE_IDENT_RE+"$"},{className:"meta",
begin:"\\*"+e.UNDERSCORE_IDENT_RE+"$"},{className:"bullet",begin:"-(?=[ ]|$)",
relevance:0},e.HASH_COMMENT_MODE,{beginKeywords:n,keywords:{literal:n}},{
className:"number",
begin:"\\b[0-9]{4}(-[0-9][0-9]){0,2}([Tt \\t][0-9][0-9]?(:[0-9][0-9]){2})?(\\.[0-9]*)?([ \\t])*(Z|[-+][0-9][0-9]?(:[0-9][0-9])?)?\\b"
},{className:"number",begin:e.C_NUMBER_RE+"\\b",relevance:0},s,o,{
className:"string",relevance:0,begin:/'/,end:/'/,contains:[{match:/''/,
scope:"char.escape",relevance:0}]},a],l=[...c]
;return l.pop(),l.push(i),r.contains=l,{name:"YAML",case_insensitive:!0,
aliases:["yml"],contains:c}}});const Ke=ne;for(const e of Object.keys(Pe)){
const n=e.replace("grmr_","").replace("_","-");Ke.registerLanguage(n,Pe[e])}
return Ke}()
;"object"==typeof exports&&"undefined"!=typeof module&&(module.exports=hljs);

/* === BidiCore.js === */
/**
 * BidiCore - Shared direction & BiDi analysis for WordHtmlBuilder, DocxBuilder and PdfBuilder
 * ==============================================================================
 *
 * Version: 1.5
 *
 * One engine, two renderers. Everything here DECIDES; nothing here renders:
 *
 *   detectDirection(root)            document-level direction ('rtl' | 'ltr')
 *   planDirections(root, docIsRTL)   ordered per-element direction decisions
 *   isComplexQuote(bq)               simple (paragraphs) vs table-form quote
 *   flattenBlock(el)                 a block's own text as ONE string + node map
 *   isMathAtom(node)                 a formula (.kelk-math, <svg>, <math>): an opaque LTR atom
 *   findIsolateRanges(text, dir, o)  counter-direction runs inside a block
 *   codeLanguage(pre)                code block language ('' if none)
 *   unwrapCodeBlocks(root)           strip preview wrappers (mutates)
 *   codeRtlRanges(text)              RTL runs inside code (UBA, LTR context)
 *   codeForWord(text)                code text with LRE/LRM marks for Word (no isolates there)
 *   nestedFences / codeSegments      fenced blocks written inside a code block
 *   codeFragment(code, from, to)     lines of a highlighted code element
 *
 * WordHtmlBuilder turns the decisions into inline CSS / <span dir>, DocxBuilder
 * into w:bidi paragraphs and w:rtl runs. Keeping the rules in one place means
 * both outputs always agree on what is RTL and what is LTR.
 *
 * All functions are pure with respect to the DOM (they read, never mutate),
 * except where noted. Browser-only parts need `document`.
 */

(function (global) {
    'use strict';

    // =========================================================================
    // Character classes — the single source of truth
    // =========================================================================

    /**
     * Liberal RTL test, used for DOCUMENT-level detection only:
     *   Arabic \u0600-\u06FF, \uFB50-\uFDFF, \uFE70-\uFEFF;
     *   Hebrew \u0590-\u05FF, \uFB1D-\uFB4F; ZWNJ \u200C.
     */
    const RTL_CHAR_PATTERN = /[\u0590-\u05FF\u0600-\u06FF\uFB1D-\uFDFF\uFE70-\uFEFF\u200C]/;

    /**
     * STRONG letter classes, shared by per-element direction and the BiDi run
     * engine so both always agree. Digits (ASCII, Arabic-Indic, Persian), ZWNJ,
     * marks and punctuation (، ؛ ؟ …) are intentionally NOT strong.
     *
     * RTL — the letters of every right-to-left script in the BMP (Unicode
     * Bidi_Class R / AL): Arabic (+ Supplement, Extended-A/B, presentation
     * forms), Hebrew (+ presentation forms), Syriac, Thaana, N'Ko, Samaritan,
     * Mandaic. Persian is just one user of it.
     */
    const RTL_LETTER_PATTERN = new RegExp('[' +
        '\\u0621-\\u063A\\u0641-\\u064A\\u0671-\\u06D3\\u06D5\\u06FA-\\u06FC' +   // Arabic, Persian, Urdu letters
        '\\u0750-\\u077F\\u0870-\\u088E\\u08A0-\\u08C9' +                            // Arabic Supplement, Extended-B/A
        '\\uFB50-\\uFDFF\\uFE70-\\uFEFF' +                                           // Arabic presentation forms A/B
        '\\u05BE\\u05C0\\u05C3\\u05C6\\u05D0-\\u05EA\\u05EF-\\u05F4' +                    // Hebrew letters (+ maqaf, geresh …)
        '\\uFB1D\\uFB1F-\\uFB28\\uFB2A-\\uFB4F' +                                     // Hebrew presentation forms
        '\\u0710\\u0712-\\u072F\\u074D-\\u074F' +                                     // Syriac
        '\\u0780-\\u07A5\\u07B1\\u07CA-\\u07EA' +                                      // Thaana, N'Ko
        '\\u0800-\\u0815\\u0840-\\u0858' +                                            // Samaritan, Mandaic
        ']');

    /**
     * LTR — letters of the left-to-right scripts a document here is likely to
     * mix with RTL text: Latin (incl. Extended-A/B, IPA, Extended Additional:
     * Vietnamese …), Greek (+ Extended), Cyrillic, Armenian, Georgian.
     * × and ÷ (U+00D7, U+00F7) sit inside Latin-1 but are symbols, not letters.
     * (Named LATIN_* for history; it is the generic strong-LTR letter class.)
     */
    const LATIN_LETTER_PATTERN = new RegExp('[' +
        'A-Za-z\\u00AA\\u00B5\\u00BA\\u00C0-\\u00D6\\u00D8-\\u00F6\\u00F8-\\u02AF' +       // Latin, IPA
        '\\u1E00-\\u1EFF' +                                                        // Latin Extended Additional
        '\\u0386\\u0388-\\u03FF\\u1F00-\\u1FBC\\u1FC2-\\u1FCC\\u1FD0-\\u1FDB\\u1FE0-\\u1FEC\\u1FF2-\\u1FFC' + // Greek
        '\\u0400-\\u0482\\u048A-\\u052F' +                                            // Cyrillic
        '\\u0531-\\u0556\\u0561-\\u0587' +                                            // Armenian
        '\\u10A0-\\u10FF' +                                                        // Georgian
        ']');

    /**
     * "Numbers-only" text: digits of any script (0-9, ٠-٩, ۰-۹) with common
     * separators ("1,234", "98.6%", "۱۴۰۵/۰۷/۰۹", "٣٫٥"). Kept for API users;
     * tables now go further: any cell without a letter follows its table.
     */
    const NUMBERS_ONLY_PATTERN = /^[\s0-9\u0660-\u0669\u06F0-\u06F9%\u066A-\u066C\u060C.,#\-–—\/:\\+×x()]+$/;

    /** Blocks the BiDi engine processes, and what a block walk must not enter. */
    const BLOCK_SEL = 'p, h1, h2, h3, h4, h5, h6, li, td, th, blockquote';
    const NON_DESCEND = /^(P|H1|H2|H3|H4|H5|H6|LI|TD|TH|BLOCKQUOTE|CODE|PRE)$/;

    /** Quote content a paragraph border cannot follow in one straight line. */
    const COMPLEX_QUOTE_SEL = 'ul, ol, blockquote, pre, table, hr, dl, figure';

    /**
     * A formula (MathCore's .kelk-math placeholder or its rendering, an <svg>
     * or <math>): an opaque left-to-right atom. Direction decisions, isolate
     * ranges and document detection never read inside it — its TeX lives in
     * data-tex, and the text an SVG draws is not document text.
     */
    function isMathAtom(n) {
        return !!n && n.nodeType === 1 && ((n.classList && n.classList.contains('kelk-math')) || /^(svg|math)$/i.test(n.tagName));
    }

    function hasRTL(text) { return RTL_CHAR_PATTERN.test(text); }
    function hasLatin(text) { return LATIN_LETTER_PATTERN.test(text); }
    /** Has a strong RTL letter (Arabic script, Hebrew …)? */
    function hasRtlLetter(text) { return RTL_LETTER_PATTERN.test(text); }
    function hasMix(t) { return LATIN_LETTER_PATTERN.test(t) && RTL_LETTER_PATTERN.test(t); }

    const RTL_LETTERS_G = new RegExp(RTL_LETTER_PATTERN.source, 'g');
    const LATIN_LETTERS_G = new RegExp(LATIN_LETTER_PATTERN.source, 'g');
    const STRONG_ANY = new RegExp(RTL_LETTER_PATTERN.source + '|' + LATIN_LETTER_PATTERN.source);

    /**
     * Should a paragraph NOT be justified? True when it holds a long unbreakable
     * run — a URL, a path, a long identifier — whatever the language: justifying
     * the line before it would stretch the spaces into gaps.
     * @param {string} text
     */
    function noJustify(text) {
        const t = String(text || '');
        return /(?:https?:\/\/|www\.)\S{16,}/i.test(t) || /\S{32,}/.test(t);
    }

    // =========================================================================
    // Table of contents (shared by the builders)
    // =========================================================================

    /** Default ToC title by document direction. */
    const TOC_TITLES = { rtl: 'فهرست مطالب', ltr: 'Contents' };

    /**
     * Insert a table of contents into a parsed document (MUTATES root) and give
     * every listed heading an id. The layout is the same in every output:
     *     <h1>            (when the document opens with one: its title)
     *     <hr>
     *     <div class="kelk-toc"> title + one entry per heading </div>
     *     <hr>
     *     <p class="kelk-spacer">  (an empty paragraph)
     * Without an opening H1 the ToC comes first and the first <hr> is left out.
     * Headings inside quotes, lists and tables are not listed. Each builder then
     * renders div.kelk-toc its own way (Word field, docx SDT, PDF page numbers,
     * HTML links); p.kelk-toc-entry carries data-level and data-target (the id).
     * @param {Element} root
     * @param {{ levels?: number, title?: string, dir?: 'rtl'|'ltr' }} opts
     * @returns {Array<{ id, level, text }>} the entries (empty → nothing inserted)
     */
    function insertToc(root, opts) {
        const o = opts || {};
        const levels = Math.max(1, Math.min(6, parseInt(o.levels, 10) || 2));
        const sel = Array.from({ length: levels }, function (_, i) { return 'h' + (i + 1); }).join(',');
        const doc = root.ownerDocument;
        let n = 0;
        const entries = [];
        root.querySelectorAll(sel).forEach(function (h) {
            if (h.closest('blockquote, li, td, th, pre')) return;
            const text = h.textContent.replace(/\s+/g, ' ').trim();
            if (!text) return;
            if (!h.id) h.id = '_Toc' + (++n);
            entries.push({ id: h.id, level: +h.tagName.charAt(1), text: text });
        });
        if (!entries.length) return entries;

        const box = doc.createElement('div');
        box.className = 'kelk-toc';
        box.setAttribute('data-levels', String(levels));
        const title = doc.createElement('p');
        title.className = 'kelk-toc-title';
        const side = o.dir === 'ltr' ? 'ltr' : 'rtl';
        const own = o.title && typeof o.title === 'object' ? o.title[side] : o.title;     // text, or { rtl, ltr }
        title.textContent = own || TOC_TITLES[side];
        box.appendChild(title);
        entries.forEach(function (e) {
            const p = doc.createElement('p');
            p.className = 'kelk-toc-entry';
            p.setAttribute('data-level', String(e.level));
            p.setAttribute('data-target', e.id);
            p.textContent = e.text;
            box.appendChild(p);
        });
        const hr = function () { return doc.createElement('hr'); };
        const spacer = doc.createElement('p');
        spacer.className = 'kelk-spacer';
        spacer.innerHTML = '&nbsp;';

        let first = root.firstElementChild;
        const frag = doc.createDocumentFragment();
        if (first && first.tagName === 'H1') {
            frag.appendChild(hr()); frag.appendChild(box); frag.appendChild(hr()); frag.appendChild(spacer);
            // an <hr> right after the title in the text itself is not doubled
            const next = first.nextElementSibling;
            if (next && next.tagName === 'HR') next.parentNode.removeChild(next);
            first.parentNode.insertBefore(frag, first.nextSibling);
        } else {
            frag.appendChild(box); frag.appendChild(hr()); frag.appendChild(spacer);
            root.insertBefore(frag, root.firstChild);
        }
        return entries;
    }

    // =========================================================================
    // Table column widths (shared by the builders)
    // =========================================================================

    /**
     * Distribute a table width over columns from their min/max content widths
     * (the browser's auto layout in short): everything fits → by max; nothing
     * fits → by min; else min plus the rest by (max − min).
     * @returns {number[]} widths summing to `total`
     */
    function distributeColumns(minC, maxC, total) {
        const sumMin = minC.reduce(function (s, x) { return s + x; }, 0);
        const sumMax = maxC.reduce(function (s, x) { return s + x; }, 0);
        if (sumMax <= total) return maxC.map(function (x) { return x * total / sumMax; });
        if (sumMin >= total) return minC.map(function (x) { return x * total / sumMin; });
        const extra = total - sumMin, span = sumMax - sumMin;
        return minC.map(function (x, k) { return x + extra * (maxC[k] - x) / span; });
    }

    /**
     * Table width from the setting: 'NN%' of the available width (default
     * 98%), or 'auto' — as wide as its content wants, up to 100%, and at least
     * `min` of it (a table of two numbers is not a sliver).
     * @param {string} setting  '98%' | 'auto' | …
     * @param {number} full     available width
     * @param {number} natural  the content's preferred width (sum of max widths)
     * @param {string} [min]    'auto' only: the least width, 'NN%' of `full` (default '20%')
     */
    function resolveTableWidth(setting, full, natural, min) {
        const s = String(setting == null ? '98%' : setting).trim().toLowerCase();
        if (s === 'auto') {
            const m = parseFloat(min == null ? '20%' : min);
            const least = full * (isFinite(m) && m > 0 ? Math.min(100, m) : 20) / 100;
            return Math.min(full, Math.max(natural || 0, least));
        }
        const p = parseFloat(s);
        return full * (isFinite(p) && p > 0 ? Math.min(100, p) : 98) / 100;
    }

    /** Text of a cell with <br> as line breaks, code included. */
    function cellLines(cell, em) {
        let t = '';
        // a formula or an image: one unbreakable "word" as wide as it is (em: points per character)
        const atom = function (pt) { return pt > 0 && em > 0 ? ' ' + 'x'.repeat(Math.max(1, Math.ceil(pt / em))) + ' ' : ''; };
        (function walk(n) {
            for (let c = n.firstChild; c; c = c.nextSibling) {
                if (c.nodeType === 3) t += c.nodeValue;
                else if (c.nodeType === 1) {
                    if (c.tagName === 'IMG') { t += atom((parseFloat(c.getAttribute('width')) || 0) * 0.75); continue; }
                    if (isMathAtom(c)) {
                        // width in em (MathCore.typeset writes it), else a guess from the TeX
                        const fs = em / 0.6, tex = c.getAttribute('data-tex') || '';
                        const w = parseFloat(c.getAttribute('data-width')) || tex.replace(/\\[a-zA-Z]+|[{}^_\s]/g, '').length * 0.55;
                        t += atom(w * 1.2 * fs);
                        continue;
                    }
                    if (c.tagName === 'BR') t += '\n';
                    else if (c.tagName !== 'TABLE') { if (/^(P|DIV|LI|H[1-6])$/.test(c.tagName) && t && !/\n$/.test(t)) t += '\n'; walk(c); }
                }
            }
        })(cell);
        return t.split('\n').map(function (l) { return l.replace(/\s+/g, ' ').trim(); });
    }

    /**
     * Column shares of a table from its text (no fonts needed): a character is
     * about half an em, a line is as wide as its characters, the narrowest a
     * column may be is its longest word. Same algorithm as PdfBuilder, which
     * measures with the real fonts.
     * @param {HTMLTableElement} table
     * @param {{ width?: number, fontSize?: number, pad?: number, tableWidth?: string, tableMinWidth?: string }} [opts] - points;
     *        tableWidth '98%' | 'auto' … and tableMinWidth (resolveTableWidth) → fraction of `width`
     * @returns {{ shares: number[], cols: Map<Element, number> }} shares sum to 1;
     *          cols: first column index of each cell
     */
    function tableColumnShares(table, opts) {
        const o = Object.assign({ width: 450, fontSize: 11, pad: 16 }, opts || {});
        const em = o.fontSize * 0.6;              // a little generous: bold headers, cell margins
        const rows = Array.from(table.rows);
        const occ = [], cells = [], cols = new Map();
        rows.forEach(function (tr, r) {
            let c = 0;
            Array.from(tr.cells).forEach(function (cell) {
                while (occ[r] && occ[r][c]) c++;
                const span = Math.max(1, cell.colSpan || 1), rs = Math.max(1, cell.rowSpan || 1);
                for (let i = 0; i < rs; i++) for (let k = 0; k < span; k++) { (occ[r + i] = occ[r + i] || [])[c + k] = true; }
                cols.set(cell, c);
                cells.push({ col: c, span: span, el: cell });
                c += span;
            });
        });
        const n = Math.max(1, occ.reduce(function (m, r) { return Math.max(m, (r || []).length); }, 0));
        const base = o.pad + em * 2;
        const minC = Array(n).fill(base), maxC = Array(n).fill(base);
        // Arabic-script letters run wider than the 0.6 em average, and a header cell is bold
        const RTLW = /[\u0590-\u08FF\uFB1D-\uFDFF\uFE70-\uFEFF]/;
        const len = function (t, bold) {
            let n = 0;
            for (const ch of t) n += RTLW.test(ch) ? 1.15 : 1;
            return n * (bold ? 1.1 : 1);
        };
        cells.forEach(function (c) {
            const lines = cellLines(c.el, em);
            const bold = c.el.tagName === 'TH';
            const maxW = Math.max.apply(null, lines.map(function (l) { return len(l, bold); }).concat([0])) * em + o.pad;
            const minW = Math.max.apply(null, lines.join(' ').split(/\s+/).map(function (w) { return len(w, bold); }).concat([0])) * em + o.pad;
            for (let k = 0; k < c.span; k++) {
                minC[c.col + k] = Math.max(minC[c.col + k], minW / c.span);
                maxC[c.col + k] = Math.max(maxC[c.col + k], maxW / c.span);
            }
        });
        const natural = maxC.reduce(function (s, x) { return s + x; }, 0);
        const width = o.tableWidth ? resolveTableWidth(o.tableWidth, o.width, natural, o.tableMinWidth) : o.width;
        const w = distributeColumns(minC, maxC, width);
        const total = w.reduce(function (s, x) { return s + x; }, 0) || 1;
        // fraction: the table's width as a part of the available width
        return { shares: w.map(function (x) { return x / total; }), cols: cols, fraction: width / o.width, natural: natural };
    }

    /** Direction of the first strong letter ('rtl' | 'ltr' | null) — UBA rule P2. */
    function firstStrong(t) {
        const m = STRONG_ANY.exec(t);
        if (!m) return null;
        return RTL_LETTER_PATTERN.test(m[0]) ? 'rtl' : 'ltr';
    }

    /**
     * Is this block text LTR? True when it has LTR letters and either no
     * RTL letter, or it STARTS with an LTR letter AND LTR letters are
     * the majority — an English sentence quoting a few Persian words
     * ("… with «شد» and «می‌شود» …") is English, while a Persian sentence
     * with a long English phrase inside stays Persian.
     */
    function isLtrText(t) {
        if (!LATIN_LETTER_PATTERN.test(t)) return false;
        if (!RTL_LETTER_PATTERN.test(t)) return true;
        if (firstStrong(t) !== 'ltr') return false;
        return (t.match(LATIN_LETTERS_G) || []).length > (t.match(RTL_LETTERS_G) || []).length;
    }

    /** Is this block text RTL (has RTL letters and is not LTR by isLtrText)? */
    function isRtlText(t) {
        return RTL_LETTER_PATTERN.test(t) && !isLtrText(t);
    }

    // =========================================================================
    // Text extraction
    // =========================================================================

    /**
     * Visible text of an element, excluding <pre> and <code> subtrees.
     * @param {Element} el
     * @param {WeakMap} [memo] - optional cache (valid only while text is unchanged)
     * @returns {string}
     */
    function getVisibleText(el, memo) {
        if (memo) {
            const hit = memo.get(el);
            if (hit !== undefined) return hit;
        }
        let text = '';
        const childNodes = el.childNodes;
        for (let i = 0; i < childNodes.length; i++) {
            const node = childNodes[i];
            if (node.nodeType === 3) {
                text += node.nodeValue;
            } else if (node.nodeType === 1) {
                const tag = node.tagName.toLowerCase();
                if (tag === 'pre' || tag === 'code' || isMathAtom(node)) continue;
                text += getVisibleText(node, memo);
            }
        }
        if (memo) memo.set(el, text);
        return text;
    }

    /**
     * Document-level direction: the direction of the FIRST block that has
     * letters (p, h1-h6, li, blockquote — its own text, code excluded; table
     * cells only when there is no such block), decided like every block
     * (isRtlText). This is the UBA's own
     * rule (P2, as dir="auto" does) lifted from the first character to the
     * first block, so "API یک رابط …" or "# ISO 27001 گزارش ممیزی" still
     * open an RTL document, and an English document keeps its Persian words,
     * quotes and cells without turning RTL. No letters at all: any RTL
     * character → rtl. (Before: any RTL character made the document RTL.)
     * An explicit setDirection('rtl'|'ltr') always wins over this.
     * @param {Element|string} source - parsed root (browser) or raw HTML (fallback)
     * @returns {'rtl'|'ltr'}
     */
    function detectDirection(source) {
        const decide = function (t) {
            if (!hasRtlLetter(t) && !hasLatin(t)) return null;
            return isRtlText(t) ? 'rtl' : 'ltr';
        };
        if (typeof source !== 'string') {
            if (source.querySelectorAll) {
                // blocks and loose top-level text, in document order
                const units = Array.from(source.querySelectorAll(BLOCK_SEL + ', pre'));
                for (let n = source.firstChild; n; n = n.nextSibling) if (n.nodeType === 3) units.push(n);
                units.sort(function (x, y) { return x === y ? 0 : (x.compareDocumentPosition(y) & 4 ? -1 : 1); });
                // prose first; table cells (often codes, names, numbers) only if there is no prose
                for (let pass = 0; pass < 2; pass++) {
                    for (let k = 0; k < units.length; k++) {
                        const u = units[k];
                        if (u.nodeType === 1 && (u.tagName === 'PRE' || u.closest('pre'))) continue;
                        if (pass === 0 && u.nodeType === 1 && u.closest('td, th')) continue;
                        const d = decide(u.nodeType === 3 ? u.nodeValue : flattenBlock(u).text);
                        if (d) return d;
                    }
                }
            }
            return hasRTL(getVisibleText(source)) ? 'rtl' : 'ltr';
        }
        const stripped = source
            .replace(/<pre[\s>][\s\S]*?<\/pre>/gi, '')
            .replace(/<code[\s>][\s\S]*?<\/code>/gi, '');
        const prose = stripped.replace(/<table[\s>][\s\S]*?<\/table>/gi, '');
        const tries = [prose, stripped];
        for (let t = 0; t < tries.length; t++) {
            const parts = tries[t].split(/<\/?(?:p|h[1-6]|li|ul|ol|td|th|tr|table|blockquote|div)\b[^>]*>|<br\s*\/?>|\n/i);
            for (let k = 0; k < parts.length; k++) {
                const d = decide(parts[k].replace(/<[^>]+>/g, ''));
                if (d) return d;
            }
        }
        return hasRTL(stripped.replace(/<[^>]+>/g, '')) ? 'rtl' : 'ltr';
    }

    // =========================================================================
    // Per-element direction plan
    // =========================================================================

    /**
     * Decide per-element direction for a document with mixed content.
     * Returns an ORDERED list of actions (renderers apply them in order):
     *
     *   { op: 'ltr',      el }          block/cell/list/li is LTR
     *   { op: 'rtl',      el }          RTL paragraph inside an LTR quote
     *   { op: 'quoteDir', el, dir }     counter-direction blockquote
     *   { op: 'ltrTable', el }          whole table is LTR (column order too)
     *
     * Rules:
     *   1. p, h1-h6 (outside lists/tables/quotes/pre): LTR unless RTL by
     *      isRtlText (RTL letters, and not an LTR-led, LTR-majority text);
     *      in an LTR document the mirror: RTL by isRtlText → explicit RTL.
     *   2. blockquote: counter-direction quotes get quoteDir; their paragraphs
     *      follow. In an RTL quote, English-only paragraphs → LTR; in an LTR
     *      quote of an LTR document, Persian paragraphs → RTL.
     *   3. tables: cells with RTL letters stay (explicit RTL in an LTR
     *      document); cells without any letter (digits of every script, signs,
     *      symbols — no code or formula) take the table's direction; every
     *      other cell → LTR. A table without RTL letters → ltrTable.
     *   4. lists: each item by its OWN text (nested lists, tables, quotes and
     *      code excluded), strong letters only. No RTL letter in any item →
     *      whole tree LTR; otherwise LTR-only items (and English-only
     *      paragraphs of RTL items) → LTR, RTL items under an LTR parent (or
     *      in an LTR document) → explicit RTL, numbers-only items follow their
     *      parent. An item's own paragraphs (loose lists) follow the item.
     *
     * Decisions depend on text only, so planning first and applying afterwards
     * is equivalent to deciding while mutating.
     *
     * @param {Element} root
     * @param {boolean} docIsRTL
     * @returns {Array<object>}
     */
    function planDirections(root, docIsRTL) {
        const memo = new WeakMap();
        const actions = [];
        const text = function (el) { return getVisibleText(el, memo); };
        const elementHasRTL = function (el) { return isRtlText(text(el)); };
        /** A table cell with no letter (digits, signs, symbols, empty) and no code, formula or image. */
        const neutralCell = function (el) {
            const t = text(el);
            if (RTL_LETTER_PATTERN.test(t) || LATIN_LETTER_PATTERN.test(t)) return false;
            return !el.querySelector('code, pre, img, .kelk-math, mjx-container');
        };
        const ltr = function (el) { actions.push({ op: 'ltr', el: el }); };

        // An LTR list item and its own paragraphs (loose items: <li><p>…</p>).
        // In Word HTML a <p class=MsoNormal> does NOT inherit the <li>'s
        // direction — the class sets the document direction explicitly — so
        // the paragraphs need their own decision.
        function ltrItem(li) {
            ltr(li);
            for (let c = li.firstElementChild; c; c = c.nextElementSibling) {
                if (/^(P|H[1-6])$/.test(c.tagName)) ltr(c);
            }
        }

        // English-only direct child paragraphs of an RTL container
        function ltrOnlyChildBlocks(container) {
            for (let c = container.firstElementChild; c; c = c.nextElementSibling) {
                if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
                if (isLtrText(text(c))) ltr(c);
            }
        }
        // Persian direct child paragraphs of an LTR container (LTR document)
        function rtlOnlyChildBlocks(container) {
            for (let c = container.firstElementChild; c; c = c.nextElementSibling) {
                if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
                if (isRtlText(text(c))) actions.push({ op: 'rtl', el: c });
            }
        }

        // 1. Standalone blocks
        root.querySelectorAll('p, h1, h2, h3, h4, h5, h6').forEach(function (el) {
            if (el.closest('pre')) return;
            if (el.closest('ul') || el.closest('ol')) return;
            if (el.closest('td') || el.closest('th')) return;
            if (el.closest('blockquote')) return;
            // the empty paragraph after the table of contents (insertToc): the document's
            // direction, as the empty paragraphs the Word builders put at its start and end
            if (el.classList && el.classList.contains('kelk-spacer')) return;
            // a paragraph holding only images (or formulas) keeps the document direction
            // (an image in an RTL document sits at the right, as in the preview)
            if (!/\S/.test(getVisibleText(el)) && el.querySelector('img, .kelk-math')) return;
            const r = elementHasRTL(el);
            if (!r) ltr(el);
            else if (!docIsRTL) actions.push({ op: 'rtl', el: el });        // Persian paragraph in an LTR document
        });

        // 2. Blockquotes
        root.querySelectorAll('blockquote').forEach(function (bq) {
            const bqHasRTL = elementHasRTL(bq);
            if (docIsRTL && !bqHasRTL) {
                actions.push({ op: 'quoteDir', el: bq, dir: 'ltr' });
                bq.querySelectorAll('p, h1, h2, h3, h4, h5, h6').forEach(function (el) {
                    // a Persian paragraph in an English quote stays RTL — explicitly,
                    // since renderers give children the quote's direction
                    if (isRtlText(text(el))) actions.push({ op: 'rtl', el: el });
                    else ltr(el);
                });
            } else if (!docIsRTL && bqHasRTL) {
                actions.push({ op: 'quoteDir', el: bq, dir: 'rtl' });
                ltrOnlyChildBlocks(bq);
            } else if (docIsRTL && bqHasRTL) {
                ltrOnlyChildBlocks(bq);
            } else {
                rtlOnlyChildBlocks(bq);                        // LTR quote of an LTR document
            }
        });

        // 3. Tables
        root.querySelectorAll('table').forEach(function (table) {
            if (table.id === 'hrdftrtbl') return;
            const tableHasRTL = hasRtlLetter(text(table));    // column order: any RTL letter
            table.querySelectorAll('td, th').forEach(function (cell) {
                if (elementHasRTL(cell)) {
                    if (!docIsRTL) actions.push({ op: 'rtl', el: cell });   // Persian cell in an LTR document
                    return;
                }
                // no letter at all (digits of any script, signs, symbols, empty) and no code or
                // formula: the cell takes the TABLE's direction, in every table and document
                if (neutralCell(cell)) { if (!tableHasRTL) ltr(cell); return; }
                ltr(cell);
            });
            if (!tableHasRTL) actions.push({ op: 'ltrTable', el: table });
        });

        // 4. Lists. An item is decided by its OWN text — nested lists, tables,
        // quotes and code are decided on their own and do not vote for it.
        // Only strong letters count: a Persian comma or ؟ in an English item
        // does not make the list RTL.
        function ownText(li) {
            let t = '';
            li.childNodes.forEach(function (n) {
                if (n.nodeType === 3) t += n.nodeValue;
                else if (n.nodeType === 1 && !/^(UL|OL|TABLE|BLOCKQUOTE|PRE)$/.test(n.tagName)) t += text(n);
            });
            return t;
        }
        // An RTL item where the inherited direction is LTR (LTR document, or
        // inside an LTR item) says so explicitly; its own paragraphs too.
        function rtlItem(li) {
            actions.push({ op: 'rtl', el: li });
            for (let c = li.firstElementChild; c; c = c.nextElementSibling) {
                if (/^(P|H[1-6])$/.test(c.tagName) && !isLtrText(text(c))) actions.push({ op: 'rtl', el: c });
            }
        }
        root.querySelectorAll('ul, ol').forEach(function (list) {
            if (list.parentNode.closest('ul, ol')) return;
            const allLis = Array.from(list.querySelectorAll('li'));
            const own = new Map();
            allLis.forEach(function (li) { own.set(li, ownText(li)); });
            const treeHasRTL = allLis.some(function (li) { return hasRtlLetter(own.get(li)); });
            if (!treeHasRTL) {
                ltr(list);
                list.querySelectorAll('ul, ol').forEach(ltr);
                allLis.forEach(ltrItem);
                return;
            }
            // Mixed tree. Document order puts parents before children.
            const isLtr = new Set();
            allLis.forEach(function (li) {
                const t = own.get(li);
                const parentLi = li.parentElement ? li.parentElement.closest('li') : null;
                const inheritedLtr = (parentLi && list.contains(parentLi)) ? isLtr.has(parentLi) : !docIsRTL;
                if (hasLatin(t) && !hasRtlLetter(t)) {
                    isLtr.add(li); ltrItem(li);
                } else if (hasRtlLetter(t)) {
                    if (inheritedLtr) rtlItem(li);
                    ltrOnlyChildBlocks(li);
                } else if (inheritedLtr) {
                    isLtr.add(li); ltrItem(li);              // numbers only: follow the parent
                }
            });
        });

        return actions;
    }

    /**
     * Collapse a plan into element → direction. For renderers that need a
     * lookup (DocxBuilder) rather than ordered style mutations.
     * @param {Array<object>} actions
     * @returns {{ dir: Map<Element,'rtl'|'ltr'>, ltrTables: Set<Element> }}
     */
    function planToMaps(actions) {
        const dir = new Map();
        const ltrTables = new Set();
        actions.forEach(function (a) {
            if (a.op === 'ltr') dir.set(a.el, 'ltr');
            else if (a.op === 'rtl') dir.set(a.el, 'rtl');
            else if (a.op === 'quoteDir') dir.set(a.el, a.dir);
            else if (a.op === 'ltrTable') ltrTables.add(a.el);
        });
        return { dir: dir, ltrTables: ltrTables };
    }

    /**
     * Does this blockquote need the table form?
     *   - it contains lists, nested quotes, code, tables … (own indents), or
     *   - (RTL document) its paragraphs end up in DIFFERENT directions — an
     *     English paragraph in a Persian quote, or a Persian one in an English
     *     quote. A paragraph border is
     *     placed relative to each paragraph's own direction/indent, so an RTL
     *     and an LTR paragraph never share one straight border line; a table
     *     cell border does.
     * Mirrors rule 2 of planDirections (ltrOnlyChildBlocks).
     * @param {Element} bq
     * @param {boolean} [docIsRTL]
     */
    function isComplexQuote(bq, docIsRTL) {
        if (bq.querySelector(COMPLEX_QUOTE_SEL)) return true;
        if (!docIsRTL) return false;
        const quoteRtl = isRtlText(getVisibleText(bq));
        for (let c = bq.firstElementChild; c; c = c.nextElementSibling) {
            if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
            const t = getVisibleText(c);
            // a paragraph whose direction differs from the quote's
            if (quoteRtl ? isLtrText(t) : isRtlText(t)) return true;
        }
        return false;
    }

    // =========================================================================
    // BiDi run engine (see WordHtmlBuilder._wrapBidiRuns for the full rationale)
    // =========================================================================

    // Conservative STRONG classes for BiDi: LETTERS only. Persian digits
    // (U+06F0..F9), ZWNJ (U+200C), and Persian punctuation are intentionally
    // NOT strong here, so an English line containing them is still single-script.
    const STRONG_R = RTL_LETTER_PATTERN;
    const STRONG_L = LATIN_LETTER_PATTERN;
    const FA_PUNCT = /[\u060C\u061B\u061F]/; // ، ؛ ؟ — RTL-owned hard boundary
    // Marks / joiners that stay with the RTL letter before them (code runs)
    const CODE_RUN_TAIL = /[\u0591-\u05C7\u0610-\u061A\u064B-\u065F\u0670\u06D6-\u06ED\u08D3-\u08FF\u200C\u200D]/;
    // Digits glued (no space) to the FRONT of a Latin word belong to that
    // LTR word: "4k", "3.5GHz", "1,000mAh", "4-bit", "2_x". Internal . or ,
    // only between digits; one optional - or _ as a digit→letter joiner.
    const DIGIT_PREFIX = /[0-9]+(?:[.,][0-9]+)*[-_]?$/;
    // Math operators (∑ √ ∫ ∂ ∇ ≤ ≠ ∈ …, ± × ÷) opening a formula before its
    // first Latin letter. ASCII operators and arrows do not count: "قیمت = Price",
    // "انجام شد → next" keep their plain UBA behavior.
    const MATH_START = /[\u2200-\u22FF\u2A00-\u2AFF\u00B1\u00D7\u00F7]/;
    // An opening quote glued to the front of the counter-direction run
    // ("Future …" / "حرم …") belongs INSIDE the isolate; left outside at a
    // block start it resolves to the base direction and jumps to the far
    // edge. Pulling it in also lets balanceCore see the pair's real
    // parity, so a following ` / "` is trimmed instead of absorbed.
    // Guillemets « » are excluded on purpose (Persian-owned, mirrored).
    const OPEN_QUOTE = /["'\u201C\u2018]$/;
    const OPEN  = { '(':')', '[':']', '{':'}', '\u00AB':'\u00BB', '\u201C':'\u201D', '\u2039':'\u203A' };
    const CLOSE = { ')':'(', ']':'[', '}':'{', '\u00BB':'\u00AB', '\u201D':'\u201C', '\u203A':'\u2039' };
    const SYM   = { '"':1, "'":1, '\u0060':1, '\u2018':1, '\u2019':1 };

    function hasL(t){ return STRONG_L.test(t); }
    function hasR(t){ return STRONG_R.test(t); }
    function hasMix(t){ return STRONG_L.test(t) && STRONG_R.test(t); }
    function isOppStrong(c, baseDir){ return baseDir === 'rtl' ? STRONG_L.test(c) : STRONG_R.test(c); }
    function isBaseStrong(c, baseDir){ return baseDir === 'rtl' ? STRONG_R.test(c) : STRONG_L.test(c); }
    /** Any counter-direction letter in text[from, to)? */
    function STRONG_L_OR_R_IN(text, from, to, baseDir){
        for (let k = from; k < to; k++) if (isOppStrong(text[k], baseDir)) return true;
        return false;
    }

    // A lone apostrophe touching a letter/digit on EITHER side ("org's",
    // "workers'", "'90s") is a contraction, possessive, or elision mark, not
    // a paired quotation delimiter — the only case with a letter/digit on
    // NEITHER side would be a truly orphaned apostrophe, which doesn't occur
    // in real text. Gate 3 exists to push an unmatched enclosure OUTSIDE the
    // isolate so the UBA can pair it with its real partner elsewhere; but for
    // this kind of apostrophe there is no real partner, so trimming here does
    // the opposite of that goal — it splits one logical LTR word/phrase into
    // two separate <span dir="ltr"> runs with a bare RTL character between
    // them, which Word's bidi engine can then reorder relative to each other,
    // scrambling the on-screen reading order. Same failure mode the
    // block-level flattening below guards against — any artificial split of
    // one logical direction-run, whether caused by a symbol-pairing heuristic
    // or an inline tag boundary, risks reordering.
    // Scoped to ' and ’ (U+2019, the "smart" apostrophe Word/typography tools
    // commonly substitute for it) — " ` ‘ are left untouched since they never
    // occur mid-word or word-adjacent this way.
    function isNonPairingApostrophe(c, region, i){
        if (c !== "'" && c !== '\u2019') return false;
        const isWordChar = function(ch){ return !!ch && /[A-Za-z0-9]/.test(ch); };
        return isWordChar(region[i - 1]) || isWordChar(region[i + 1]);
    }

    // Trim a region down to an enclosure-balanced core (Gate 3).
    function balanceCore(region){
        let start = 0, end = region.length;
        function check(s, e){
            const stack = [], sym = {};
            for (let i = s; i < e; i++){
                const c = region[i];
                if (OPEN[c]) stack.push(i);
                else if (CLOSE[c]){ if (stack.length) stack.pop(); else return { ok:false, bad:i }; }
                else if (SYM[c] && !isNonPairingApostrophe(c, region, i)) sym[c] = (sym[c] || 0) + 1;
            }
            if (stack.length) return { ok:false, bad: stack[stack.length - 1] };
            for (const k in sym){
                if (sym[k] % 2 === 1){
                    for (let t = e - 1; t >= s; t--){
                        if (region[t] === k && !isNonPairingApostrophe(k, region, t)) return { ok:false, bad:t };
                    }
                }
            }
            return { ok:true };
        }
        while (start < end){ const r = check(start, end); if (r.ok) break; end = r.bad; }
        while (start < end && CLOSE[region[start]]) start++;
        return { start: start, end: end };
    }

    // Find counter-direction isolate spans in a block's flattened text.
    // Operates on ONE continuous string per block (built by the caller below)
    // rather than per DOM text node. Returns [start, end) ranges in TEXT's
    // own coordinates.
    function findIsolateRangesImpl(text, baseDir, endRe){
        const hasOpp = (baseDir === 'rtl') ? STRONG_L.test(text) : STRONG_R.test(text);
        if (!hasOpp) return [];
        const ranges = [];
        const n = text.length;
        let i = 0;
        let scanFrom = 0; // end of the previous region — backtracking never crosses it
        while (i < n){
            const ch = text[i];
            if (!isOppStrong(ch, baseDir)){ i++; continue; }
            let j = i, region = '';
            // Digit-prefix rule (RTL base only): pull digits attached to the
            // first Latin letter INTO the isolate. Otherwise "4k" becomes
            // "4" (outside, resolved as Arabic number after Persian letters →
            // placed to the RIGHT of k) + <span dir=ltr>k</span>, and Word
            // shows "k4". Trailing digits ("k4", "USB3") were already absorbed
            // by the greedy scan below; this closes the leading-digit gap.
            // Formula rule (RTL base only): "مجموع: ∑ (i=1 → n) i = n(n+1)/2".
            // A math operator between the last RTL letter (or ، ؛ ؟) and the
            // first Latin letter starts the formula: the run begins THERE, so
            // the whole formula is one LTR isolate. Otherwise the run starts
            // at "i", its "(" is left outside, the balance guard cuts at ")"
            // and the rest of the formula is reordered ("n)n+1(/2").
            if (baseDir === 'rtl'){
                let b = scanFrom;
                for (let k = i - 1; k >= scanFrom; k--){
                    if (isBaseStrong(text[k], baseDir) || FA_PUNCT.test(text[k])){ b = k + 1; break; }
                }
                const at = text.slice(b, i).search(MATH_START);
                if (at >= 0){ region = text.slice(b + at, i); i = b + at; }
            }
            if (baseDir === 'rtl' && !region){
                const dp = text.slice(scanFrom, i).match(DIGIT_PREFIX);
                if (dp){ region = dp[0]; i -= dp[0].length; }
            }
            // Number-before-RTL rule (LTR base only): "Summary (3 صفحه تا …)".
            // A number separated only by spaces from a Persian phrase, and not
            // itself following a Latin word ("Chapter 3 صفحه" keeps its 3),
            // is read with the phrase: it joins the RTL run, so it stands at the
            // phrase's start (its right side) instead of before it on the left.
            if (baseDir === 'ltr' && !region){
                const nm = /[0-9\u0660-\u0669\u06F0-\u06F9]+(?:[.,\u066B\u066C][0-9\u0660-\u0669\u06F0-\u06F9]+)*[ \t\u00A0]+$/.exec(text.slice(scanFrom, i));
                if (nm){
                    let k = i - nm[0].length - 1;
                    while (k >= scanFrom && /[ \t\u00A0]/.test(text[k])) k--;
                    if (k < scanFrom || !(isBaseStrong(text[k], baseDir) || /[0-9]/.test(text[k]))){
                        region = nm[0]; i -= nm[0].length;
                    }
                }
            }
            // Opening-quote prefix (both directions): only when the quote
            // itself starts a word (block start, whitespace or punctuation
            // before it — not a letter/digit, which would make it a
            // closing quote or an apostrophe).
            if (i > scanFrom && OPEN_QUOTE.test(text[i - 1]) &&
                (i - 1 === 0 || !(/[0-9\u0600-\u06FF]/.test(text[i - 2]) || STRONG_ANY.test(text[i - 2])))){
                region = text[i - 1] + region; i -= 1;
            }
            j = i + region.length;
            while (j < n){
                const c = text[j];
                if (isBaseStrong(c, baseDir)) break;
                if (baseDir === 'rtl' && FA_PUNCT.test(c)) break;
                region += c; j++;
            }
            const core = balanceCore(region);
            let keepStart = i + core.start;
            let keepEnd = i + core.end;

            // Boundary punctuation fix: a trailing cluster of dashes, spaces, and
            // separator/connector punctuation (: ; . / \ | =) that sits exactly at the
            // LTR→RTL handoff must stay OUTSIDE the isolate — these characters function
            // as a label/item separator binding to what FOLLOWS (the Farsi phrase), not
            // what precedes (e.g. "B8." / "B8:" / "B8 / " / "B8 = " all read as
            // "item B8, separator, Farsi label"). Comma is deliberately excluded: unlike
            // these separator marks, a comma overwhelmingly binds to the PRECEDING word
            // in ordinary prose (lists, clauses), so trimming it here would risk pulling
            // a legitimate trailing comma out of real English text merely because Farsi
            // happens to follow — a decision to add it should wait for a concrete
            // failing example, not be inferred by analogy.
            // Guard: only trim when what follows leads back to the base direction.
            let nextIsRtl;
            if (j < n){
                nextIsRtl = isBaseStrong(text[j], baseDir) || (baseDir === 'rtl' && FA_PUNCT.test(text[j]));
            } else {
                nextIsRtl = false; // true end of block content — nothing follows, don't trim
            }
            if (nextIsRtl){
                const dm = text.slice(keepStart, keepEnd).match(/[\s\u2010-\u2015\u2212\-\u2013\u2014:;.\/\\|=]+$/);
                if (dm) keepEnd -= dm[0].length;
            }
            const tm = text.slice(keepStart, keepEnd).match(/(\s+)$/);
            if (tm) keepEnd -= tm[1].length;
            const lm = text.slice(keepStart, keepEnd).match(/^(\s+)/);
            if (lm) keepStart += lm[1].length;

            if (keepEnd > keepStart) ranges.push([keepStart, keepEnd]);
            // The balance guard cut the run short: scan the rest of it again,
            // so later Latin text in the same stretch still gets its isolate.
            const cut = i + core.end;
            i = (keepEnd > keepStart && cut < j && STRONG_L_OR_R_IN(text, cut, j, baseDir)) ? Math.max(keepEnd, cut) : j;
            scanFrom = i;
        }

        // End-punctuation rule: only the LAST isolate, only when nothing but
        // trailing whitespace follows it to the end of the block's text.
        if (baseDir === 'rtl' && endRe && ranges.length){
            const last = ranges[ranges.length - 1];
            if (/^\s*$/.test(text.slice(last[1]))){
                const inner = text.slice(last[0], last[1]);
                const m = inner.match(endRe);
                if (m && m[1]) last[1] = last[0] + m[1].length;
            }
        }

        return ranges;
    }

    /**
     * RTL runs inside CODE (an LTR context: inline code, code block lines).
     * Plain UBA for an LTR paragraph, as editors and Word show code: a run
     * goes from an RTL letter to the LAST RTL letter before the next LTR
     * letter (neutrals and numbers BETWEEN RTL letters belong to it — rule
     * N1), plus the marks / ZWNJ right after that letter. Neutrals at either
     * edge stay LTR, so `// توضیح`, `"سلام";` and `کد 😀 code` keep their
     * punctuation, quotes and symbols in place. (The prose engine
     * findIsolateRanges is tuned for Word paragraphs and absorbs trailing
     * neutrals; code needs the strict rule.)
     * @param {string} text
     * @returns {Array<[number, number]>} [start, end) ranges
     */
    const CODE_BRACKETS = /[()[\]{}<>]/;

    /**
     * Code text for Word, which has no isolates: an LTR embedding (LRE … PDF)
     * around it, and LRM marks around each neutral stretch between two RTL runs
     * (codeRtlRanges), so `![نام](نام)` inside a Persian sentence keeps its
     * order and brackets. Text without RTL letters comes back unchanged.
     */
    function codeForWord(text) {
        if (!text || !hasRtlLetter(text)) return text;
        const r = codeRtlRanges(text);
        let out = '', pos = 0;
        r.forEach(function (x, k) {
            const gap = text.slice(pos, x[0]);
            out += k > 0 && gap ? '\u200E' + gap + '\u200E' : gap;
            out += text.slice(x[0], x[1]);
            pos = x[1];
        });
        out += text.slice(pos);
        return '\u202A' + out + '\u202C';
    }

    function codeRtlRanges(text) {
        const out = [];
        const n = text.length;
        let i = 0;
        while (i < n) {
            if (!STRONG_R.test(text[i])) { i++; continue; }
            // a run goes on over spaces and neutrals, not past a Latin letter or a bracket:
            // brackets around RTL text in code take the code's LTR side (UBA N0 with an LTR
            // context), so `![نام](نام)` keeps its brackets in place
            let j = i + 1, last = i;
            while (j < n && !STRONG_L.test(text[j]) && !CODE_BRACKETS.test(text[j])) {
                if (STRONG_R.test(text[j])) last = j;
                j++;
            }
            let end = last + 1;
            while (end < n && CODE_RUN_TAIL.test(text[end])) end++;
            out.push([i, end]);
            i = end;
        }
        return out;
    }

    const endReCache = new Map();
    function endPunctuationRe(chars) {
        if (!chars || !chars.length) return null;
        const key = chars.join('\u0000');
        if (!endReCache.has(key)) {
            endReCache.set(key, new RegExp('^(.*?)([' + chars.map(function (c) {
                return c.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            }).join('') + ']+)$'));
        }
        return endReCache.get(key);
    }

    /**
     * Counter-direction isolate ranges in a block's flattened text.
     * @param {string} text - ONE continuous string for the whole block
     * @param {'rtl'|'ltr'} baseDir - the block's effective direction
     * @param {object} [opts]
     * @param {string[]} [opts.endPunctuation=['.', ':']] - end-of-block marks kept
     *        outside the last isolate in RTL blocks ([] disables)
     * @returns {Array<[number, number]>} [start, end) ranges
     */
    function findIsolateRanges(text, baseDir, opts) {
        const chars = opts && opts.endPunctuation !== undefined ? opts.endPunctuation : ['.', ':'];
        return findIsolateRangesImpl(text, baseDir, endPunctuationRe(chars));
    }

    /**
     * Flatten a block's OWN text — through inline formatting, stopping at
     * nested blocks and code/pre — into one string, recording which text node
     * each character came from.
     * @param {Element} el
     * @returns {{ text: string, nodeMap: Array<{node: Text, start: number, end: number}> }}
     */
    function flattenBlock(el) {
        let flatText = '';
        const nodeMap = [];
        (function collect(n) {
            for (let c = n.firstChild; c; c = c.nextSibling) {
                if (c.nodeType === 3) {
                    const val = c.nodeValue;
                    if (val) {
                        nodeMap.push({ node: c, start: flatText.length, end: flatText.length + val.length });
                        flatText += val;
                    }
                } else if (c.nodeType === 1 && !NON_DESCEND.test(c.tagName) && !isMathAtom(c)) {
                    collect(c);
                }
            }
        })(el);
        return { text: flatText, nodeMap: nodeMap };
    }

    /** Map a flat offset back to { node, offset } (see flattenBlock). */
    function resolvePosition(nodeMap, flatOffset) {
        for (let k = 0; k < nodeMap.length; k++) {
            const entry = nodeMap[k];
            if (flatOffset <= entry.end) return { node: entry.node, offset: flatOffset - entry.start };
        }
        const last = nodeMap[nodeMap.length - 1];
        return { node: last.node, offset: last.node.nodeValue.length };
    }

    // =========================================================================
    // Code blocks — shared by both builders and usable by the preview
    // =========================================================================

    /** Class values that name "no language". */
    const NO_LANGUAGE = /^(plaintext|plain|text|txt|nohighlight|no-highlight|none)$/i;

    /**
     * Language of a code block, from the standard CommonMark/marked class
     * (`language-xxx`, also `lang-xxx`) on <code> or <pre>, or from a
     * `data-lang` attribute. Keeps full names like "c++", "c#",
     * "objective-c", "shell-session". Returns '' when unknown or plain text.
     * @param {Element} pre - a <pre> (or its <code>)
     * @returns {string}
     */
    function codeLanguage(pre) {
        const code = pre.tagName === 'CODE' ? pre : (pre.querySelector('code') || pre);
        const host = code.closest('pre') || code;
        const classes = (code.getAttribute('class') || '') + ' ' + (host.getAttribute('class') || '');
        const m = /(?:^|\s)(?:language|lang)-(\S+)/.exec(classes);
        let lang = m ? m[1] : (host.getAttribute('data-lang') || code.getAttribute('data-lang') || '');
        lang = lang.trim();
        return NO_LANGUAGE.test(lang) ? '' : lang;
    }

    /**
     * Remove preview-only decoration around code blocks (MUTATES root):
     *   <div class="code-block-wrapper">
     *     <div class="code-block-header"><span class="code-lang">…</span><button>…</button></div>
     *     <pre>…</pre>
     *   </div>                                   →   <pre>…</pre>
     * Must run right after parsing: the header text ("javascript Copy") would
     * otherwise take part in direction decisions. A label shown only in the
     * header (no language class) is kept on the <pre> as data-lang.
     * @param {Element} root
     */
    function unwrapCodeBlocks(root) {
        // PreviewBuilder / HtmlBuilder frames: <div class="code"><div class="code-lang">lang <button></div><pre>
        root.querySelectorAll('div.code').forEach(function (w) {
            const pre = w.querySelector(':scope > pre');
            if (!pre) return;
            if (!codeLanguage(pre)) {
                const l = w.querySelector(':scope > .code-lang');
                const t = l ? Array.from(l.childNodes).filter(function (n) { return n.nodeType === 3; }).map(function (n) { return n.nodeValue; }).join('').trim() : '';
                if (t && t !== 'code') pre.setAttribute('data-lang', t);
            }
            w.parentNode.replaceChild(pre, w);
        });
        root.querySelectorAll('.code-block-wrapper').forEach(function (w) {
            const pre = w.querySelector('pre');
            if (!pre) { w.parentNode.removeChild(w); return; }
            if (!codeLanguage(pre)) {
                const l = w.querySelector('.code-lang');
                const t = l ? l.textContent.trim() : '';
                if (t && t !== 'code') pre.setAttribute('data-lang', t);
            }
            w.parentNode.replaceChild(pre, w);
        });
        root.querySelectorAll('.code-block-header, .code-copy-btn').forEach(function (e) {
            if (e.parentNode) e.parentNode.removeChild(e);
        });
        // nested-fence frames of the preview (span.code-nested): keep the text
        root.querySelectorAll('.code-nested').forEach(function (sp) {
            while (sp.firstChild) sp.parentNode.insertBefore(sp.firstChild, sp);
            sp.parentNode.removeChild(sp);
        });
    }


    /**
     * Fenced blocks written INSIDE a code block (a ````md block showing
     * ```js … ``` examples). The text stays raw; renderers only draw a frame
     * around each nested block, from its opening fence line to its closing
     * one, like nested quotes. CommonMark fences: ``` or ~~~ (3+), up to 3
     * spaces indent; a closing fence has the same character, at least the
     * same length and no info string. Only markdown blocks (md / markdown /
     * no language) are scanned for further nesting; an unclosed fence gets
     * no frame.
     * @param {string} text - code text (lines split on \n)
     * @returns {Array<{from, to, lang, children}>} line numbers, inclusive
     */
    function nestedFences(text) {
        const FENCE = /^ {0,3}(`{3,}|~{3,})(.*)$/;
        const top = [], stack = [];
        text.split('\n').forEach(function (line, i) {
            const m = FENCE.exec(line);
            if (stack.length) {
                const t = stack[stack.length - 1];
                if (m && m[1].charAt(0) === t.ch && m[1].length >= t.len && !m[2].trim()) {
                    t.to = i;
                    stack.pop();
                    (stack.length ? stack[stack.length - 1].children : top).push(t);
                    return;
                }
                if (t.lang && !/^(md|markdown)$/i.test(t.lang)) return;     // raw inside other languages
            }
            if (m && !(m[1].charAt(0) === '`' && m[2].indexOf('`') >= 0)) {
                stack.push({ from: i, to: -1, ch: m[1].charAt(0), len: m[1].length,
                             lang: m[2].trim().split(/\s+/)[0] || '', children: [] });
            }
        });
        return top;
    }

    /**
     * nestedFences as a render tree over ALL lines [0, lineCount):
     *   { lines: [from, to] }                   plain lines (inclusive)
     *   { nested: true, lang, items: [...] }    a framed nested block
     * @param {string} text
     * @returns {Array<object>}
     */
    function codeSegments(text) {
        const count = text.split('\n').length;
        function seg(nodes, from, to) {
            const out = [];
            let i = from;
            nodes.forEach(function (n) {
                if (n.from > i) out.push({ lines: [i, n.from - 1] });
                out.push({ nested: true, lang: n.lang, from: n.from, to: n.to, items: seg(n.children, n.from, n.to) });
                i = n.to + 1;
            });
            if (i <= to) out.push({ lines: [i, to] });
            return out;
        }
        return seg(nestedFences(text), 0, count - 1);
    }

    /**
     * Copy of lines [fromLine, toLine] of a (highlighted) code element as a
     * DocumentFragment — spans cut at the line edges, no trailing newline.
     * Browser only (DOM Range).
     */
    function codeFragment(codeEl, fromLine, toLine) {
        const doc = codeEl.ownerDocument;
        const walker = doc.createTreeWalker(codeEl, 4 /* NodeFilter.SHOW_TEXT */);
        let line = 0, start = null, end = null, lastNode = null;
        for (let n = walker.nextNode(); n; n = walker.nextNode()) {
            const v = n.nodeValue;
            lastNode = n;
            for (let k = 0; k <= v.length; k++) {
                if (!start && line === fromLine) start = [n, k];
                if (k === v.length) break;
                if (v.charAt(k) === '\n') {
                    if (line === toLine) { end = [n, k]; break; }
                    line++;
                }
            }
            if (end) break;
        }
        const range = doc.createRange();
        if (!start) return doc.createDocumentFragment();
        range.setStart(start[0], start[1]);
        if (end) range.setEnd(end[0], end[1]);
        else range.setEnd(lastNode, lastNode.nodeValue.length);
        return range.cloneContents();
    }


    /**
     * A detached <div> in an inert document: content assigned to innerHTML
     * is parsed, but its <img> elements do not start loading (the builders
     * parse the preview HTML; image bytes come from ImageCore). Nodes made
     * with document.createElement can still be inserted (adopted).
     */
    function inertRoot() {
        const doc = (typeof document !== 'undefined' && document.implementation)
            ? document.implementation.createHTMLDocument('') : document;
        return doc.createElement('div');
    }


    // =========================================================================
    // Custom header / footer HTML (shared model for the three builders)
    // =========================================================================
    //
    // setHeader({ html }) / setFooter({ html }) take a SUBSET of HTML that all
    // three outputs can draw the same way:
    //   blocks   <table> (<tr>, <td>/<th>, colspan, width %), <p>, <div>
    //   inline   <b>/<strong>, <i>/<em>, <u>, <span>, <a href>, <img>, <br>
    //   style="" (inline only): text-align, vertical-align, color, background
    //            (-color), font-size, font-weight, font-style, padding(-*),
    //            border / border-top|right|bottom|left, width (%)
    //   fields   {page} {pages} {date} {title} — anywhere in the text
    // Classes, <style> and other properties are ignored (with a console.warn).

    const HF_FIELD = /\{(page|pages|date|title)\}/g;
    const HF_FIELD_TEST = /\{(page|pages|date|title)\}/;
    const HF_STYLE_KEYS = /^(text-align|vertical-align|color|background|background-color|font-size|font-weight|font-style|padding|padding-(top|right|bottom|left)|border|border-(top|right|bottom|left)|width)$/;

    /** CSS length → pt (px, pt, cm, mm, in); NaN when not a length. */
    function cssPt(v) {
        const m = /^\s*(-?[\d.]+)\s*(px|pt|cm|mm|in)?\s*$/i.exec(String(v || ''));
        if (!m) return NaN;
        return parseFloat(m[1]) * { px: 0.75, pt: 1, cm: 72 / 2.54, mm: 72 / 25.4, in: 72 }[(m[2] || 'px').toLowerCase()];
    }

    /** '1pt solid red' (any order) → { width (pt), style, color } | null (none/0) */
    function cssBorder(v) {
        const t = String(v || '').trim().toLowerCase();
        if (!t || /^(none|0|hidden)$/.test(t)) return null;
        let width = 0.75, style = 'solid', color = '#000000';
        t.replace(/rgb\([^)]*\)/g, function (m) { return m.replace(/\s+/g, ''); }).split(/\s+/).forEach(function (p) {
            if (/^(solid|dashed|dotted|double)$/.test(p)) style = p;
            else if (/^(thin|medium|thick)$/.test(p)) width = { thin: 0.5, medium: 1.5, thick: 2.25 }[p];
            else if (!isNaN(cssPt(p))) width = cssPt(p);
            else if (p) color = p;
            return p;
        });
        return width > 0 ? { width: width, style: style, color: color } : null;
    }


    const CSS_COLORS = {
        black: '000000', white: 'FFFFFF', red: 'FF0000', green: '008000', blue: '0000FF',
        darkgreen: '006400', navy: '000080', maroon: '800000', orange: 'FFA500', yellow: 'FFFF00',
        gray: '808080', grey: '808080', silver: 'C0C0C0', purple: '800080', teal: '008080',
        darkred: '8B0000', darkblue: '00008B', lightgray: 'D3D3D3', lightgrey: 'D3D3D3', windowtext: '000000'
    };
    /** CSS color (#rgb, #rrggbb, rgb(), common names) → 'RRGGBB' | null */
    function cssColorHex(v) {
        const t = String(v || '').trim().toLowerCase();
        if (CSS_COLORS[t]) return CSS_COLORS[t];
        let m = /^#([0-9a-f]{6})$/.exec(t);
        if (m) return m[1].toUpperCase();
        m = /^#([0-9a-f])([0-9a-f])([0-9a-f])$/.exec(t);
        if (m) return (m[1] + m[1] + m[2] + m[2] + m[3] + m[3]).toUpperCase();
        m = /^rgba?\(\s*(\d+)[\s,]+(\d+)[\s,]+(\d+)/.exec(t);
        if (m) return [m[1], m[2], m[3]].map(function (x) { return ('0' + Math.min(255, +x).toString(16)).slice(-2); }).join('').toUpperCase();
        return null;
    }

    /** Whitelisted inline style of an element → normalized style object. */
    function hfStyle(el, warn) {
        const out = { border: {} };
        const attrAlign = el.getAttribute && el.getAttribute('align');
        if (attrAlign) out.align = attrAlign.toLowerCase();
        const attrW = el.getAttribute && el.getAttribute('width');
        if (attrW && /%$/.test(attrW)) out.width = parseFloat(attrW);
        String((el.getAttribute && el.getAttribute('style')) || '').split(';').forEach(function (decl) {
            const i = decl.indexOf(':');
            if (i < 0) return;
            const k = decl.slice(0, i).trim().toLowerCase(), v = decl.slice(i + 1).trim();
            if (!k) return;
            if (!HF_STYLE_KEYS.test(k)) { if (warn) warn('style ' + k); return; }
            switch (k) {
                case 'text-align': out.align = v.toLowerCase(); break;
                case 'vertical-align': out.valign = v.toLowerCase(); break;
                case 'color': out.color = v; break;
                case 'background': case 'background-color': out.bg = v.split(/\s+/)[0]; break;
                case 'font-size': out.size = cssPt(v); break;
                case 'font-weight': out.bold = /bold|[6-9]00/.test(v); break;
                case 'font-style': out.italic = /italic|oblique/.test(v); break;
                case 'width': if (/%$/.test(v)) out.width = parseFloat(v); else if (!isNaN(cssPt(v))) out.widthPt = cssPt(v); break;
                case 'padding': {
                    const p = v.split(/\s+/).map(cssPt);
                    const t = p[0], r = p.length > 1 ? p[1] : t, b = p.length > 2 ? p[2] : t, l = p.length > 3 ? p[3] : r;
                    out.padding = { top: t, right: r, bottom: b, left: l };
                    break;
                }
                case 'border': ['top', 'right', 'bottom', 'left'].forEach(function (sd) { out.border[sd] = cssBorder(v); }); break;
                default:
                    if (k.indexOf('padding-') === 0) { out.padding = out.padding || {}; out.padding[k.slice(8)] = cssPt(v); }
                    else out.border[k.slice(7)] = cssBorder(v);
            }
        });
        return out;
    }

    /**
     * Parse header/footer HTML into { blocks }:
     *   { type: 'table', style, rows: [{ cells: [{ colspan, style, paras }] }] }
     *   { type: 'para', style, el }          (el: detached inline container)
     * A cell's paras: one per <p>/<div> (or the cell's own inline content),
     * split further at <br>. Placeholders stay in the text (see hfFields).
     */
    function parseHeaderFooter(html) {
        const doc = (typeof document !== 'undefined' && document.implementation)
            ? document.implementation.createHTMLDocument('') : null;
        if (!doc) return { blocks: [] };
        const root = doc.createElement('div');
        root.innerHTML = String(html || '');
        const warned = new Set();
        const warn = function (what) {
            if (warned.has(what)) return;
            warned.add(what);
            if (typeof console !== 'undefined' && console.warn) console.warn('header/footer HTML: ' + what + ' is not supported, ignored');
        };
        root.querySelectorAll('style, script, link, meta').forEach(function (e) { e.remove(); });
        root.querySelectorAll('[class]').forEach(function () { warn('class'); });
        root.querySelectorAll('*').forEach(function (e) { hfStyle(e, warn); });

        // inline content of a container → paras (split at <br> and child blocks)
        function parasOf(container, baseStyle) {
            const paras = [];
            let cur = null;
            const start = function (style) { cur = doc.createElement('span'); paras.push({ style: style || baseStyle, el: cur }); };
            (function walk(node, style) {
                for (let n = node.firstChild; n; n = n.nextSibling) {
                    if (n.nodeType === 1 && /^(P|DIV|H[1-6])$/.test(n.tagName)) {
                        const st = Object.assign({}, style, stripBox(hfStyle(n)));
                        if (/^H/.test(n.tagName)) st.bold = true;
                        cur = null;
                        start(st);
                        walk(n, st);
                        cur = null;
                        continue;
                    }
                    if (n.nodeType === 1 && n.tagName === 'BR') { start(style); continue; }
                    if (n.nodeType === 3 && !/\S/.test(n.nodeValue) && !cur) continue;
                    if (!cur) start(style);
                    cur.appendChild(n.cloneNode(true));
                }
            })(container, baseStyle);
            return paras.filter(function (p) { return p.el.childNodes.length; });
        }
        // text-level properties only (a paragraph inherits them from its cell)
        function stripBox(st) {
            const o = {};
            ['align', 'color', 'size', 'bold', 'italic'].forEach(function (k) { if (st[k] !== undefined) o[k] = st[k]; });
            return o;
        }

        const blocks = [];
        let loose = null;
        for (let n = root.firstChild; n; n = n.nextSibling) {
            if (n.nodeType === 1 && n.tagName === 'TABLE') {
                loose = null;
                const tStyle = hfStyle(n);
                const rows = [];
                n.querySelectorAll('tr').forEach(function (tr) {
                    if (tr.closest('table') !== n) return;
                    const cells = [];
                    Array.from(tr.children).forEach(function (td) {
                        if (!/^(TD|TH)$/.test(td.tagName)) return;
                        const st = hfStyle(td);
                        if (td.tagName === 'TH' && st.bold === undefined) st.bold = true;
                        cells.push({ colspan: Math.max(1, parseInt(td.getAttribute('colspan'), 10) || 1),
                                     style: st, paras: parasOf(td, stripBox(st)) });
                    });
                    if (cells.length) rows.push({ cells: cells });
                });
                if (rows.length) blocks.push({ type: 'table', style: tStyle, rows: rows });
                continue;
            }
            if (n.nodeType === 1 && /^(P|DIV|H[1-6])$/.test(n.tagName)) {
                loose = null;
                const st = hfStyle(n);
                if (/^H/.test(n.tagName)) st.bold = true;
                parasOf(n, stripBox(st)).forEach(function (p) { blocks.push({ type: 'para', style: Object.assign({}, st, p.style), el: p.el }); });
                continue;
            }
            if (n.nodeType === 3 && !/\S/.test(n.nodeValue)) continue;
            // loose inline content at the top level → one paragraph
            if (!loose) { loose = doc.createElement('span'); blocks.push({ type: 'para', style: {}, el: loose }); }
            loose.appendChild(n.cloneNode(true));
        }
        return { blocks: blocks };
    }

    /**
     * Column widths (%) of a parsed table: widths given on the cells of the
     * first row that has them (colspan split evenly), the rest shared equally.
     */
    function hfColumnWidths(table) {
        const count = Math.max.apply(null, table.rows.map(function (r) {
            return r.cells.reduce(function (s, c) { return s + c.colspan; }, 0);
        }));
        const w = new Array(count).fill(null);
        table.rows.forEach(function (r) {
            let col = 0;
            r.cells.forEach(function (c) {
                if (c.style.width && c.colspan === 1 && w[col] === null) w[col] = c.style.width;
                col += c.colspan;
            });
        });
        const given = w.reduce(function (s, x) { return s + (x || 0); }, 0);
        const free = w.filter(function (x) { return x === null; }).length;
        const rest = free ? Math.max(0, 100 - given) / free : 0;
        const out = w.map(function (x) { return x === null ? rest : x; });
        const sum = out.reduce(function (s, x) { return s + x; }, 0) || 1;
        return out.map(function (x) { return x * 100 / sum; });
    }

    /**
     * Fill the fields of an inline container (a deep clone is returned).
     * values: { page, pages, date, title } — a function value(name) returning
     * a Node places that node (Word field codes); a string replaces the text.
     */
    function hfFields(el, values) {
        const c = el.cloneNode(true);
        const doc = c.ownerDocument;
        const walker = doc.createTreeWalker(c, 4);
        const texts = [];
        for (let n = walker.nextNode(); n; n = walker.nextNode()) if (HF_FIELD_TEST.test(n.nodeValue)) texts.push(n);
        texts.forEach(function (t) {
            const frag = doc.createDocumentFragment();
            let pos = 0;
            const v = t.nodeValue;
            v.replace(HF_FIELD, function (m, name, off) {
                if (off > pos) frag.appendChild(doc.createTextNode(v.slice(pos, off)));
                const val = values[name];
                const node = typeof val === 'function' ? val(name, doc) : null;
                frag.appendChild(node || doc.createTextNode(val == null ? m : String(val)));
                pos = off + m.length;
                return m;
            });
            if (pos < v.length) frag.appendChild(doc.createTextNode(v.slice(pos)));
            t.parentNode.replaceChild(frag, t);
        });
        return c;
    }

    /** {date}: today in the header/footer language, Latin digits. */
    function hfDate(dir, locale) {
        const loc = locale || (dir === 'rtl' ? 'fa-IR-u-nu-latn' : 'en-GB');
        try { return new Intl.DateTimeFormat(loc, { year: 'numeric', month: 'long', day: 'numeric' }).format(new Date()); }
        catch (e) { return new Date().toISOString().slice(0, 10); }
    }

    // =========================================================================
    // Small shared utilities
    // =========================================================================

    const PT_PER_UNIT = { pt: 1, px: 0.75, cm: 72 / 2.54, mm: 72 / 25.4, in: 72, pc: 12 };

    /** CSS length ('1.5cm', '11pt', '0.5in', 11) → points. */
    function lengthToPt(value) {
        if (typeof value === 'number') return value;
        const m = /^\s*(-?[\d.]+)\s*(pt|px|cm|mm|in|pc)?\s*$/i.exec(String(value));
        if (!m) throw new Error('Invalid length: ' + value);
        return parseFloat(m[1]) * PT_PER_UNIT[(m[2] || 'pt').toLowerCase()];
    }

    /**
     * Text-area width in points from template options (pageSize is already
     * landscape-swapped by setPage; margin is a 1–4 value CSS shorthand).
     */
    function contentWidthPt(T) {
        const wh = String(T.pageSize).trim().split(/\s+/).map(lengthToPt);
        const w = T.pageOrientation === 'landscape' ? Math.max(wh[0], wh[1]) : Math.min(wh[0], wh[1]);
        const m = String(T.pageMargin).trim().split(/\s+/).map(lengthToPt);
        const right = m.length > 1 ? m[1] : m[0];
        const left = m.length > 3 ? m[3] : right;
        return w - left - right;
    }

    /**
     * Deep merge without sharing nested objects with `target` (so mutating the
     * result never leaks into a DEFAULTS object). Arrays are copied, not merged.
     * @param {object} target
     * @param {object} source
     * @param {string[]} [skipKeys] - keys of `source` to ignore
     */
    function deepMerge(target, source, skipKeys) {
        const isObj = function (v) { return v && typeof v === 'object' && !Array.isArray(v); };
        const copy = function (v) { return isObj(v) ? deepMerge(v, {}) : (Array.isArray(v) ? v.slice() : v); };
        const result = {};
        for (const key in target) result[key] = copy(target[key]);
        for (const key in source) {
            if (!Object.prototype.hasOwnProperty.call(source, key)) continue;
            if (skipKeys && skipKeys.indexOf(key) !== -1) continue;
            const v = source[key];
            result[key] = isObj(v) ? deepMerge(isObj(result[key]) ? result[key] : {}, v) : copy(v);
        }
        return result;
    }

    // =========================================================================
    // Export (UMD)
    // =========================================================================
    const BidiCore = {
        RTL_CHAR_PATTERN: RTL_CHAR_PATTERN,
        RTL_LETTER_PATTERN: RTL_LETTER_PATTERN,
        LATIN_LETTER_PATTERN: LATIN_LETTER_PATTERN,
        NUMBERS_ONLY_PATTERN: NUMBERS_ONLY_PATTERN,
        BLOCK_SEL: BLOCK_SEL,
        NON_DESCEND: NON_DESCEND,
        COMPLEX_QUOTE_SEL: COMPLEX_QUOTE_SEL,
        hasRTL: hasRTL,
        hasLatin: hasLatin,
        hasRtlLetter: hasRtlLetter,
        hasMix: hasMix,
        firstStrong: firstStrong,
        noJustify: noJustify,
        insertToc: insertToc,
        TOC_TITLES: TOC_TITLES,
        distributeColumns: distributeColumns,
        tableColumnShares: tableColumnShares,
        resolveTableWidth: resolveTableWidth,
        isLtrText: isLtrText,
        isRtlText: isRtlText,
        getVisibleText: getVisibleText,
        detectDirection: detectDirection,
        planDirections: planDirections,
        planToMaps: planToMaps,
        isComplexQuote: isComplexQuote,
        findIsolateRanges: findIsolateRanges,
        codeRtlRanges: codeRtlRanges,
        codeForWord: codeForWord,
        nestedFences: nestedFences,
        codeSegments: codeSegments,
        codeFragment: codeFragment,
        flattenBlock: flattenBlock,
        isMathAtom: isMathAtom,
        resolvePosition: resolvePosition,
        deepMerge: deepMerge,
        codeLanguage: codeLanguage,
        unwrapCodeBlocks: unwrapCodeBlocks,
        inertRoot: inertRoot,
        parseHeaderFooter: parseHeaderFooter,
        hfColumnWidths: hfColumnWidths,
        hfFields: hfFields,
        hfDate: hfDate,
        cssBorder: cssBorder,
        cssColorHex: cssColorHex,
        hfStyleOf: function (el) { return hfStyle(el); },
        cssPt: cssPt,
        lengthToPt: lengthToPt,
        contentWidthPt: contentWidthPt
    };

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = BidiCore;
    } else if (typeof define === 'function' && define.amd) {
        define([], function () { return BidiCore; });
    } else {
        global.BidiCore = BidiCore;
    }

})(typeof window !== 'undefined' ? window : this);


/* === BidiView.js === */
/**
 * BidiView - direction for HTML shown in a browser (preview, standalone HTML)
 * ============================================================================
 * Version: 1.5
 *
 * The browser already runs the Unicode BiDi Algorithm; BidiView applies the
 * decisions of BidiCore — the same ones the .doc / .docx / .pdf builders
 * apply — so a page shows what the files get:
 *   1. document direction   BidiCore.detectDirection
 *   2. block directions     BidiCore.planDirections (paragraphs, headings,
 *                           list items, quotes, table cells, LTR tables)
 *   3. isolates             BidiCore.findIsolateRanges over each block's own
 *                           text (BidiCore.flattenBlock) — one isolate per run,
 *                           even across <strong>/<em> boundaries
 * plus what only a browser page needs: list items indented by LEVEL on their
 * own start side (bd-b0 / bd-sN over --bd-base / --bd-step), as Word does, and
 * inline code as an isolate.
 *
 * Output is CLASSES only (bd-*), never a dir attribute, so the HTML handed to
 * other tools stays clean:
 *
 *   BidiView.apply(root, { dir: 'auto'|'rtl'|'ltr', endPunctuation: ['.', ':'],
 *                          isolate: true, inlineCode: true }) → { dir, blocks, isolates }
 *   BidiView.clear(root)          remove everything apply() added
 *   BidiView.css()                the CSS the classes need
 *   BidiView.toAttributes(root)   classes → dir attributes (for the clipboard:
 *                                 Word reads dir, not CSS classes)
 *
 * Needs BidiCore (load it first). tools/bidi-lab.html carries both inline.
 */

(function (global) {
  'use strict';

  function core() {
    var B = global.BidiCore || (typeof require === 'function' ? require('./BidiCore.js') : null);
    if (!B) throw new Error('BidiView needs BidiCore (load lib/BidiCore.js first)');
    return B;
  }

  /** Direction an element ends up with: nearest planned ancestor, else the document. */
  function dirOf(el, root, map, doc) {
    for (var p = el; p && p !== root; p = p.parentNode) if (map.has(p)) return map.get(p);
    return doc;
  }

  /**
   * List indents by LEVEL on each item's own start side, as Word does: the text
   * of level n sits at base + (n−1) × step from that side, so an RTL item nested
   * in an LTR item is indented from the RIGHT as a level-2 item, although its
   * parent's indent was on the left. Each item's margin is what its same-side
   * ancestors have not given yet, as coefficients of base and step:
   * classes bd-b0 (no base: an ancestor on this side had it) and bd-sN.
   * CSS: --bd-base / --bd-step (default 1.7em).
   */
  function indent(root, map, doc) {
    var give = new Map();                       // li → { b, s } it adds on its side
    Array.prototype.forEach.call(root.querySelectorAll('li'), function (li) {
      var d = dirOf(li, root, map, doc), depth = 1, gb = 0, gs = 0;
      for (var a = li.parentElement ? li.parentElement.closest('li') : null; a && root.contains(a); a = a.parentElement ? a.parentElement.closest('li') : null) {
        depth++;
        if (dirOf(a, root, map, doc) === d) { var g = give.get(a) || { b: 0, s: 0 }; gb += g.b; gs += g.s; }
      }
      var mb = Math.max(0, 1 - gb), ms = Math.max(0, Math.min(9, (depth - 1) - gs));
      give.set(li, { b: mb, s: ms });
      if (!mb) li.classList.add('bd-b0');
      if (ms) li.classList.add('bd-s' + ms);
    });
  }

  function clear(root) {
    Array.prototype.slice.call(root.querySelectorAll('span.bd-iso-ltr, span.bd-iso-rtl')).forEach(function (s) {
      var parent = s.parentNode;                       // unwrap, keeping <strong>/<em> inside
      while (s.firstChild) parent.insertBefore(s.firstChild, s);
      parent.removeChild(s);
    });
    Array.prototype.slice.call(root.querySelectorAll('.bd-ltr, .bd-rtl, .bd-ltr-table, .bd-iso-ltr, .bd-iso-rtl, .bd-b0, [class*="bd-s"]')).forEach(function (el) {
      el.classList.remove('bd-ltr', 'bd-rtl', 'bd-ltr-table', 'bd-iso-ltr', 'bd-iso-rtl', 'bd-b0');
      for (var i = 1; i <= 9; i++) el.classList.remove('bd-s' + i);
      if (!el.classList.length) el.removeAttribute('class');          // leave the HTML as it was
    });
    root.classList.remove('bd-root', 'bd-doc-rtl', 'bd-doc-ltr');
    if (!root.classList.length) root.removeAttribute('class');
    root.normalize();
  }

  /** Classes → dir attributes and no classes (rich copy into Word, mail …). MUTATES root. */
  function toAttributes(root) {
    var all = [root].concat(Array.prototype.slice.call(root.querySelectorAll('.bd-ltr, .bd-rtl, .bd-ltr-table, .bd-iso-ltr, .bd-iso-rtl')));
    all.forEach(function (el) {
      var c = el.classList;
      if (c.contains('bd-doc-rtl') || c.contains('bd-rtl') || c.contains('bd-iso-rtl')) el.setAttribute('dir', 'rtl');
      else if (c.contains('bd-doc-ltr') || c.contains('bd-ltr') || c.contains('bd-ltr-table') || c.contains('bd-iso-ltr')) el.setAttribute('dir', 'ltr');
    });
    Array.prototype.slice.call(root.querySelectorAll('span.bd-iso-ltr, span.bd-iso-rtl')).forEach(function (s) {
      s.classList.remove('bd-iso-ltr', 'bd-iso-rtl');
      if (!s.className) s.removeAttribute('class');
    });
    clear(root);
    Array.prototype.slice.call(root.querySelectorAll('[class=""]')).forEach(function (el) { el.removeAttribute('class'); });
    return root;
  }

  /** An inline element emptied by extractContents() (its text moved into the isolate). */
  function dropEmpty(n) {
    if (n && n.nodeType === 1 && /^(STRONG|EM|B|I|U|S|DEL|INS|MARK|SMALL|SUB|SUP|SPAN)$/.test(n.tagName) &&
        !n.textContent && !n.querySelector('img, br, input')) n.parentNode.removeChild(n);
  }

  /** Counter-direction runs of every block: BidiCore ranges → span.bd-iso-*. */
  function isolate(root, map, doc, o) {
    var B = core(), count = 0;
    Array.prototype.forEach.call(root.querySelectorAll(B.BLOCK_SEL), function (el) {
      if (el.closest('pre')) return;
      var flat = B.flattenBlock(el);
      if (!B.hasMix(flat.text)) return;
      var base = dirOf(el, root, map, doc), other = base === 'rtl' ? 'ltr' : 'rtl';
      var ranges = B.findIsolateRanges(flat.text, base, { endPunctuation: o.endPunctuation });
      // last range first: earlier ranges and the node map stay valid;
      // extractContents() splits a partly covered <strong>/<em> correctly
      for (var r = ranges.length - 1; r >= 0; r--) {
        var a = ranges[r][0], b = ranges[r][1];
        if (b <= a) continue;
        var sp = B.resolvePosition(flat.nodeMap, a), ep = B.resolvePosition(flat.nodeMap, b);
        var range = document.createRange();
        range.setStart(sp.node, sp.offset);
        range.setEnd(ep.node, ep.offset);
        var span = document.createElement('span');
        span.className = 'bd-iso-' + other;
        span.appendChild(range.extractContents());
        range.insertNode(span);
        dropEmpty(span.previousSibling); dropEmpty(span.nextSibling);   // shells left by the split
        count++;
      }
    });
    return count;
  }

  function apply(root, options) {
    var o = Object.assign({ dir: 'auto', endPunctuation: ['.', ':'], isolate: true, inlineCode: true }, options || {});
    var B = core();
    clear(root);
    var doc = o.dir === 'rtl' || o.dir === 'ltr' ? o.dir : B.detectDirection(root);
    root.classList.add('bd-root', 'bd-doc-' + doc);
    var plan = B.planToMaps(B.planDirections(root, doc === 'rtl'));
    plan.dir.forEach(function (d, el) { el.classList.add('bd-' + d); });
    plan.ltrTables.forEach(function (t) { t.classList.add('bd-ltr-table'); });
    indent(root, plan.dir, doc);
    if (o.inlineCode) {
      Array.prototype.forEach.call(root.querySelectorAll('code'), function (c) {
        if (c.closest('pre')) return;
        c.classList.add('bd-iso-' + (B.firstStrong(c.textContent) || 'ltr'));
      });
    }
    var runs = o.isolate ? isolate(root, plan.dir, doc, o) : 0;
    return { dir: doc, blocks: plan.dir.size, isolates: runs };
  }

  /** The CSS the classes need. Lists: the marker and indent follow each item. */
  function css() {
    return [
      '.bd-doc-rtl{direction:rtl}.bd-doc-ltr{direction:ltr}',
      '.bd-ltr,.bd-ltr-table{direction:ltr}.bd-rtl{direction:rtl}',
      '.bd-root :is(p,h1,h2,h3,h4,h5,h6,li,blockquote,ul,ol,dt,dd):is(.bd-ltr,.bd-rtl){text-align:start}',
      '.bd-root :is(td,th):is(.bd-ltr,.bd-rtl):not([align]):not([style*="text-align"]){text-align:start}',
      '.bd-iso-ltr{unicode-bidi:isolate;direction:ltr}.bd-iso-rtl{unicode-bidi:isolate;direction:rtl}',
      '.bd-root pre{direction:ltr;text-align:left}',
      '.bd-root :is(ul,ol){padding-inline-start:0}',
      '.bd-root li{--bd-b:1;--bd-s:0;margin-inline-start:calc(var(--bd-b) * var(--bd-base,1.7em) + var(--bd-s) * var(--bd-step,1.7em))}',
      '.bd-root li.bd-b0{--bd-b:0}' + [1, 2, 3, 4, 5, 6, 7, 8, 9].map(function (n) { return '.bd-root li.bd-s' + n + '{--bd-s:' + n + '}'; }).join('')
    ].join('\n');
  }

  var BidiView = { version: '1.5', apply: apply, clear: clear, css: css, toAttributes: toAttributes };

  if (typeof module !== 'undefined' && module.exports) module.exports = BidiView;
  else if (typeof define === 'function' && define.amd) define([], function () { return BidiView; });
  else global.BidiView = BidiView;
})(typeof window !== 'undefined' ? window : this);


/* === Controller === */

(function() {
  'use strict';

  var contentEl = null;
  var currentBiDi = true;

  // Configure marked for GFM
  if (typeof marked !== 'undefined') {
    marked.setOptions({
      gfm: true,
      breaks: false
    });
  }

  // Tokenize markdown with source line tracking
  function parseWithLines(markdown) {
    if (!markdown || !markdown.trim()) return '';
    try {
      var tokens = marked.lexer(markdown);
      var curLine = 0;
      for (var i = 0; i < tokens.length; i++) {
        var tok = tokens[i];
        tok.sourceLine = curLine;
        if (tok.type === 'list' && tok.items) {
          var itemLine = curLine;
          for (var j = 0; j < tok.items.length; j++) {
            tok.items[j].sourceLine = itemLine;
            var numLines = (tok.items[j].raw.match(/\n/g) || []).length;
            itemLine += numLines;
          }
        }
        var lines = (tok.raw.match(/\n/g) || []).length;
        curLine += lines;
      }

      var customRenderer = new marked.Renderer();
      customRenderer.heading = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<h' + obj.depth + ' data-line="' + line + '">' + text + '</h' + obj.depth + '>\n';
      };
      customRenderer.paragraph = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<p data-line="' + line + '">' + text + '</p>\n';
      };
      customRenderer.code = function(obj) {
        var validLang = (obj.lang || '').match(/^\S*/)[0];
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        var langClass = validLang ? ' language-' + validLang : '';
        return '<pre data-line="' + line + '"><code class="hljs' + langClass + '">' +
               DOMPurify.sanitize(obj.text) + '</code></pre>\n';
      };
      customRenderer.blockquote = function(obj) {
        var text = this.parser.parse(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<blockquote data-line="' + line + '">' + text + '</blockquote>\n';
      };
      customRenderer.listitem = function(obj) {
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        var text = this.parser.parse(obj.tokens);
        if (obj.task) {
          return '<li class="task-list-item" data-line="' + line + '">' +
                 '<input type="checkbox" class="task-checkbox" data-line="' + line + '" ' +
                 (obj.checked ? 'checked' : '') + '> ' + text + '</li>\n';
        }
        return '<li data-line="' + line + '">' + text + '</li>\n';
      };

      var parser = new marked.Parser({ renderer: customRenderer });
      return parser.parse(tokens);
    } catch(err) {
      console.error('Markdown parse error:', err);
      return marked.parse(markdown);
    }
  }

  function applyRender(data) {
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
      contentEl.innerHTML = '<div style="color:var(--muted);text-align:center;padding:60px 20px;"><em>(No content to preview)</em></div>';
      return;
    }

    var html = parseWithLines(mdText);
    if (typeof DOMPurify !== 'undefined') {
      html = DOMPurify.sanitize(html, { ADD_ATTR: ['data-line', 'target', 'dir', 'align'] });
    }

    contentEl.innerHTML = html;

    // External links open in default browser
    var links = contentEl.querySelectorAll('a[href^="http"]');
    for (var l = 0; l < links.length; l++) {
      links[l].target = '_blank';
      links[l].rel = 'noopener';
    }

    // Syntax highlighting with highlight.js
    if (typeof hljs !== 'undefined') {
      var codeBlocks = contentEl.querySelectorAll('pre code');
      for (var c = 0; c < codeBlocks.length; c++) {
        try { hljs.highlightElement(codeBlocks[c]); } catch(e) {}
      }
    }

    // Apply Kelk BiDi engine
    if (currentBiDi && typeof BidiView !== 'undefined' && typeof BidiCore !== 'undefined') {
      try {
        BidiView.apply(contentEl);
      } catch(e) {
        console.error('BidiView error:', e);
      }
    } else {
      contentEl.setAttribute('dir', 'auto');
    }

    // Bind interactive task checkboxes
    var cbs = contentEl.querySelectorAll('input.task-checkbox, input.task-list-item-checkbox');
    for (var k = 0; k < cbs.length; k++) {
      (function(cb) {
        cb.addEventListener('click', function(e) {
          e.stopPropagation();
          var line = cb.getAttribute('data-line');
          if (line !== null && window.chrome && window.chrome.webview) {
            window.chrome.webview.postMessage('toggleCheckbox:' + line);
          }
        });
      })(cbs[k]);
    }

    // Scroll to caret if specified
    if (typeof data.caretLine === 'number' && data.caretLine >= 0) {
      scrollToLine(data.caretLine, false);
    }
  }

  function scrollToLine(targetLine, highlight) {
    if (!contentEl) contentEl = document.getElementById('content');
    if (!contentEl) return;

    var elements = contentEl.querySelectorAll('[data-line]');
    if (elements.length === 0) return;

    var bestEl = null;
    var minDiff = 999999;
    for (var i = 0; i < elements.length; i++) {
      var l = parseInt(elements[i].getAttribute('data-line'), 10);
      var diff = Math.abs(l - targetLine);
      if (diff < minDiff) {
        minDiff = diff;
        bestEl = elements[i];
      }
    }

    if (bestEl) {
      bestEl.scrollIntoView({ behavior: 'smooth', block: 'center' });
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
      } else if (data.type === 'scrollToLine') {
        scrollToLine(data.line, true);
      } else if (data.type === 'setTheme') {
        document.body.classList.toggle('dark', !!data.isDark);
        document.documentElement.setAttribute('data-theme', data.isDark ? 'dark' : 'light');
      } else if (data.type === 'setBiDi') {
        currentBiDi = !!data.isBiDi;
        if (contentEl && typeof BidiView !== 'undefined') {
          if (currentBiDi) BidiView.apply(contentEl);
          else { BidiView.clear(contentEl); contentEl.setAttribute('dir', 'auto'); }
        }
      }
    });
  }

  document.addEventListener('DOMContentLoaded', function() {
    contentEl = document.getElementById('content');
    if (window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('pageLoaded');
    }
  });
})();

)KELK_JS_EOF";

inline constexpr const char* s_kelkReaderShellHtml = R"KELK_HTML_EOF(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Markdown Reader</title>
  <style>
/* Kelk — design tokens: colors (light / dark), fonts, radii, spacing.
   Two font variables drive the whole page:
     --font-ui       interface (buttons, labels, settings)
     --font-content  the document: editor + preview (set from Settings → Typography)
   --font-mono is the code font of the preview (and of the exports). */











:root {
  --font-ui: 'Vazirmatn', system-ui, sans-serif;
  --font-content: 'Sahel', 'Vazirmatn', system-ui, sans-serif;
  --font-latin: 'Vazirmatn', system-ui, sans-serif;
  --font-mono: 'Vazir Code Hack', 'DejaVu Sans Mono', ui-monospace, Consolas, monospace;

  --bg: #f7f6f3;
  --panel: #ffffff;
  --panel-2: #fcfbf9;
  --ink: #1c1b1a;
  --muted: #6b6862;
  --placeholder: #b6b1a8;
  --line: #e6e3dd;
  --line-strong: #d4d0c8;
  --accent: #1f6f5c;
  --accent-soft: #e7f1ee;
  --accent-ink: #114b3e;
  --on-accent: #ffffff;
  --danger: #b42318;
  --warn: #b45309;
  --code-bg: #f4f2ee;
  --code-head: #ebe8e2;
  --quote-bar: #c9a227;
  --table-head: #e2efd9;
  --toast-bg: #114b3e;
  --toast-ink: #ffffff;

  --radius: 10px;
  --radius-sm: 6px;
  --shadow: 0 1px 2px rgba(28, 27, 26, .04), 0 4px 16px rgba(28, 27, 26, .05);
  --gap: 14px;
  --pane-w: 340px;
  --rail-w: 44px;
}

[data-theme="dark"] {
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
  --danger: #f97066;
  --warn: #f0b429;
  --code-bg: #25292d;
  --code-head: #2d3136;
  --quote-bar: #d4b43f;
  --table-head: #24382c;
  --toast-bg: #4db396;
  --toast-ink: #0f1416;
  --shadow: 0 1px 2px rgba(0, 0, 0, .25), 0 4px 16px rgba(0, 0, 0, .3);
}


/* Kelk — the preview pane.
   The document's own look comes from HtmlBuilder's stylesheet (lib/PreviewBuilder → <style id="kelk-doc-css">),
   the same one the .html export embeds. This file adds only what belongs to the pane: spacing, the
   empty hint, the copy buttons, the source view, and the dark theme of the app. */

.doc {
  padding: 18px 22px 28px;
  color: var(--ink);
  overflow-wrap: anywhere;
}
.doc:empty::before { content: attr(data-empty); color: var(--placeholder); font-family: var(--font-ui); }
.doc > :first-child { margin-top: 0; }

/* wide tables scroll inside the pane */
.doc table:not(.kelk-fit) { display: block; overflow-x: auto; }

/* code frame bar: the language on one side, the copy button on the other */
.doc .code-lang { display: flex; align-items: center; justify-content: space-between; gap: 8px; min-height: 1.6em; }
.code-copy-btn {
  display: inline-flex; align-items: center; gap: 4px; border: 0; background: transparent; color: inherit; cursor: pointer;
  font-size: 12px; padding: 1px 6px; border-radius: 4px; font-family: var(--font-ui); margin-inline-start: auto;
}
.code-copy-btn:hover { background: var(--accent-soft); color: var(--accent-ink); }
.code-copy-btn svg { width: 14px; height: 14px; }
.code-copy-btn.copied { color: var(--accent); }
.doc .code-nested { position: relative; }
.code-nested-copy { position: absolute; top: 2px; right: 4px; padding: 1px 3px; opacity: .55; }
.code-nested:hover > .code-nested-copy { opacity: 1; }

/* Markdown shown as source (reverse view) */
.doc.source { font-family: var(--font-mono); font-size: 13px; white-space: pre-wrap; direction: ltr; text-align: left; unicode-bidi: plaintext; }

/* dark theme of the app. The document's own colors (tables, code, quotes, ToC) are translated
   for the dark page by scripts/preview.js (darkPaper) — the exports keep the chosen colors; here,
   only what the document stylesheet does not color: text on the page, code text and syntax colors. */
[data-theme="dark"] .doc h1, [data-theme="dark"] .doc h2, [data-theme="dark"] .doc h3,
[data-theme="dark"] .doc h4, [data-theme="dark"] .doc h5, [data-theme="dark"] .doc h6 { color: var(--accent-ink); }
[data-theme="dark"] .doc :not(pre) > code { background: var(--code-bg); color: var(--ink); }
[data-theme="dark"] .doc a { color: var(--accent-ink); }
[data-theme="dark"] .doc .code, [data-theme="dark"] .doc .code code.hljs { color: var(--ink); }
/* syntax colors for a dark box (GitHub dark) */
[data-theme="dark"] .doc .code .hljs-keyword, [data-theme="dark"] .doc .code .hljs-doctag, [data-theme="dark"] .doc .code .hljs-type,
[data-theme="dark"] .doc .code .hljs-meta .hljs-keyword, [data-theme="dark"] .doc .code .hljs-template-tag { color: #ff7b72; }
[data-theme="dark"] .doc .code .hljs-string, [data-theme="dark"] .doc .code .hljs-regexp, [data-theme="dark"] .doc .code .hljs-meta .hljs-string { color: #a5d6ff; }
[data-theme="dark"] .doc .code .hljs-title, [data-theme="dark"] .doc .code .hljs-title.function_, [data-theme="dark"] .doc .code .hljs-title.class_ { color: #d2a8ff; }
[data-theme="dark"] .doc .code .hljs-number, [data-theme="dark"] .doc .code .hljs-literal, [data-theme="dark"] .doc .code .hljs-attr,
[data-theme="dark"] .doc .code .hljs-attribute, [data-theme="dark"] .doc .code .hljs-variable, [data-theme="dark"] .doc .code .hljs-built_in,
[data-theme="dark"] .doc .code .hljs-selector-class, [data-theme="dark"] .doc .code .hljs-operator { color: #79c0ff; }
[data-theme="dark"] .doc .code .hljs-comment, [data-theme="dark"] .doc .code .hljs-quote { color: #8b949e; }
[data-theme="dark"] .doc .code .hljs-name, [data-theme="dark"] .doc .code .hljs-selector-tag, [data-theme="dark"] .doc .code .hljs-section { color: #7ee787; }
[data-theme="dark"] .doc .code .hljs-subst, [data-theme="dark"] .doc .code .hljs-params { color: var(--ink); }

/* highlight.js's page theme adds padding and a background to code.hljs: the frame has its own */
.doc pre code.hljs { padding: 0; background: none; }


pre code.hljs{display:block;overflow-x:auto;padding:1em}code.hljs{padding:3px 5px}/*!
  Theme: GitHub
  Description: Light theme as seen on github.com
  Author: github.com
  Maintainer: @Hirse
  Updated: 2021-05-15

  Outdated base version: https://github.com/primer/github-syntax-light
  Current colors taken from GitHub's CSS
*/.hljs{color:#24292e;background:#fff}.hljs-doctag,.hljs-keyword,.hljs-meta .hljs-keyword,.hljs-template-tag,.hljs-template-variable,.hljs-type,.hljs-variable.language_{color:#d73a49}.hljs-title,.hljs-title.class_,.hljs-title.class_.inherited__,.hljs-title.function_{color:#6f42c1}.hljs-attr,.hljs-attribute,.hljs-literal,.hljs-meta,.hljs-number,.hljs-operator,.hljs-selector-attr,.hljs-selector-class,.hljs-selector-id,.hljs-variable{color:#005cc5}.hljs-meta .hljs-string,.hljs-regexp,.hljs-string{color:#032f62}.hljs-built_in,.hljs-symbol{color:#e36209}.hljs-code,.hljs-comment,.hljs-formula{color:#6a737d}.hljs-name,.hljs-quote,.hljs-selector-pseudo,.hljs-selector-tag{color:#22863a}.hljs-subst{color:#24292e}.hljs-section{color:#005cc5;font-weight:700}.hljs-bullet{color:#735c0f}.hljs-emphasis{color:#24292e;font-style:italic}.hljs-strong{color:#24292e;font-weight:700}.hljs-addition{color:#22863a;background-color:#f0fff4}.hljs-deletion{color:#b31d28;background-color:#ffeef0}


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
  text-align: start;
}
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

  </style>
  <script>
/* === marked.umd.min_18.0.14.js === */
/**
 * Minified by jsDelivr using Terser v5.48.0.
 * Original file: /npm/marked@18.0.14/lib/marked.umd.js
 *
 * Do NOT use SRI with dynamically generated files! More information: https://www.jsdelivr.com/using-sri-with-dynamic-files
 */
!function(e,t){"object"==typeof exports&&typeof module<"u"?module.exports=t():"function"==typeof define&&define.amd?define("marked",t):e.marked=t()}(typeof globalThis<"u"?globalThis:typeof self<"u"?self:this,function(){var e,t={},n=t,r={exports:t},s=Object.defineProperty,l=Object.getOwnPropertyDescriptor,i=Object.getOwnPropertyNames,a=Object.prototype.hasOwnProperty,o={};function c(){return{async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null}}((e,t)=>{for(var n in t)s(e,n,{get:t[n],enumerable:!0})})(o,{Hooks:()=>De,Lexer:()=>Be,Marked:()=>Oe,Parser:()=>ve,Renderer:()=>qe,TextRenderer:()=>Ze,Tokenizer:()=>Ce,defaults:()=>p,getDefaults:()=>c,lexer:()=>We,marked:()=>Qe,options:()=>Ne,parse:()=>Ue,parseInline:()=>Xe,parser:()=>Fe,setOptions:()=>He,use:()=>Me,walkTokens:()=>Ge}),r.exports=(e=o,((e,t,n,r)=>{if(t&&"object"==typeof t||"function"==typeof t)for(let o of i(t))!a.call(e,o)&&o!==n&&s(e,o,{get:()=>t[o],enumerable:!(r=l(t,o))||r.enumerable});return e})(s({},"__esModule",{value:!0}),e));var p={async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null};function h(e){p=e}var u={exec:()=>null};function g(e){let t=[];return n=>{let r=Math.max(0,Math.min(3,n-1)),s=t[r];return s||(s=e(r),t[r]=s),s}}function k(e,t=""){let n="string"==typeof e?e:e.source,r={replace:(e,t)=>{let s="string"==typeof t?t:t.source;return s=s.replace(d.caret,"$1"),n=n.replace(e,s),r},getRegex:()=>new RegExp(n,t)};return r}var f=((e="")=>{try{return!!new RegExp("(?<=1)(?<!1)"+e)}catch{return!1}})(),d={codeRemoveIndent:/^(?: {0,3}\t| {1,4})/gm,outputLinkReplace:/\\([\[\]])/g,indentCodeCompensation:/^(\s+)(?:```)/,beginningSpace:/^\s+/,endingHash:/#$/,startingSpaceChar:/^ /,endingSpaceChar:/ $/,endingSpaceTabChar:/[ \t]$/,nonSpaceChar:/[^ ]/,newLineCharGlobal:/\n/g,tabCharGlobal:/\t/g,leadingSpaceTab:/^[ \t]+/,multipleSpaceGlobal:/\s+/g,blankLine:/^[ \t]*$/,doubleBlankLine:/\n[ \t]*\n[ \t]*$/,blockquoteStart:/^ {0,3}>/,blockquoteSetextReplace:/\n {0,3}((?:=+|-+) *)(?=\n|$)/g,blockquoteSetextReplace2:/^ {0,3}>[ \t]?/gm,listReplaceNesting:/^ {1,4}(?=( {4})*[^ ])/g,listIsTask:/^\[[ xX]\] +\S/,listReplaceTask:/^\[[ xX]\] +/,listTaskCheckbox:/\[[ xX]\]/,anyLine:/\n.*\n/,hrefBrackets:/^<(.*)>$/,tableDelimiter:/[:|]/,tableAlignChars:/^\||\| *$/g,tableRowBlankLine:/\n[ \t]*$/,tableAlignRight:/^ *-+: *$/,tableAlignCenter:/^ *:-+: *$/,tableAlignLeft:/^ *:-+ *$/,startATag:/^<a /i,endATag:/^<\/a>/i,startPreScriptTag:/^<(pre|code|kbd|script)(\s|>)/i,endPreScriptTag:/^<\/(pre|code|kbd|script)(\s|>)/i,startAngleBracket:/^</,endAngleBracket:/>$/,pedanticHrefTitle:/^([^'"]*[^\s])\s+(['"])(.*)\2/,unicodeAlphaNumeric:/[\p{L}\p{N}]/u,numericCharacterReference:/&#(?:(\d{1,7})|[Xx]([A-Fa-f0-9]{1,6}));/g,escapeTest:/[&<>"']/,escapeReplace:/[&<>"']/g,escapeTestNoEncode:/[<>"']|&(?!(#\d{1,7}|#[Xx][a-fA-F0-9]{1,6}|\w+);)/,escapeReplaceNoEncode:/[<>"']|&(?!(#\d{1,7}|#[Xx][a-fA-F0-9]{1,6}|\w+);)/g,caret:/(^|[^\[])\^/g,percentDecode:/%25/g,findPipe:/\|/g,splitPipe:/ \|/,slashPipe:/\\\|/g,carriageReturn:/\r\n|\r/g,spaceLine:/^ +$/gm,notSpaceStart:/^\S*/,endingNewline:/\n$/,listItemRegex:e=>new RegExp(`^( {0,3}${e})((?:[\t ][^\\n]*)?(?:\\n|$))`),nextBulletRegex:g(e=>new RegExp(`^ {0,${e}}(?:[*+-]|\\d{1,9}[.)])((?:[ \t][^\\n]*)?(?:\\n|$))`)),hrRegex:g(e=>new RegExp(`^ {0,${e}}((?:-[ \t]*){3,}|(?:_[ \t]*){3,}|(?:\\*[ \t]*){3,})(?:\\n+|$)`)),fencesBeginRegex:g(e=>new RegExp(`^ {0,${e}}(?:\`\`\`|~~~)`)),headingBeginRegex:g(e=>new RegExp(`^ {0,${e}}#`)),htmlBeginRegex:g(e=>new RegExp(`^ {0,${e}}(?:</?(?:${z})(?: +|$|/?>)|<(?:script|pre|style|textarea|!--))`,"i")),blockquoteBeginRegex:g(e=>new RegExp(`^ {0,${e}}>`))},x=/^ {0,3}((?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/,b=/ {0,3}(?:[*+-]|\d{1,9}[.)])/,m=/^(?!bull |blockCode|fences|blockquote|heading|html|table)((?:.|\n(?!\s*?\n|bull |fences|blockquote|heading|hr|html|table))+?)\n {0,3}(=+|-+) *(?:\n+|$)/,w=k(m).replace(/bull/g,b).replace(/blockCode/g,/(?: {4}| {0,3}\t)/).replace(/fences/g,/ {0,3}(?:`{3,}|~{3,})/).replace(/blockquote/g,/ {0,3}>/).replace(/heading/g,/ {0,3}#{1,6}(?:\s|$)/).replace(/hr/g,/ {0,3}(?:(?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/).replace(/html/g,/ {0,3}<[^\n>]+>\n/).replace(/\|table/g,"").getRegex(),y=k(m).replace(/bull/g,b).replace(/blockCode/g,/(?: {4}| {0,3}\t)/).replace(/fences/g,/ {0,3}(?:`{3,}|~{3,})/).replace(/blockquote/g,/ {0,3}>/).replace(/heading/g,/ {0,3}#{1,6}(?:\s|$)/).replace(/hr/g,/ {0,3}(?:(?:-[\t ]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})(?:\n+|$)/).replace(/html/g,/ {0,3}<[^\n>]+>\n/).replace(/table/g,/ {0,3}\|?(?:[:\- ]*\|)+[\:\- ]*\n/).getRegex(),S=/^([^\n]+(?:\n(?!hr|heading|lheading|blockquote|fences|list|html|table|[ \t]+\n)[^\n]+)*)/,$=/(?!\s*\])(?:\\[\s\S]|[^\[\]\\])+/,R=k(/^ {0,3}\[(label)\]: *(?:\n[ \t]*)?([^<\s][^\s]*|<.*?>)(?:(?: +(?:\n[ \t]*)?| *\n[ \t]*)(title))? *(?:\n+|$)/).replace("label",$).replace("title",/(?:"(?:\\"?|[^"\\])*"|'[^'\n]*(?:\n[^'\n]+)*\n?'|\([^()]*\))/).getRegex(),T=k(/^(bull)([ \t][^\n]*?)?(?:\n|$)/).replace(/bull/g,b).getRegex(),z="address|article|aside|base|basefont|blockquote|body|caption|center|col|colgroup|dd|details|dialog|dir|div|dl|dt|fieldset|figcaption|figure|footer|form|frame|frameset|h[1-6]|head|header|hr|html|iframe|legend|li|link|main|menu|menuitem|meta|nav|noframes|ol|optgroup|option|p|param|search|section|summary|table|tbody|td|tfoot|th|thead|title|tr|track|ul",_=/<!--(?:-?>|[\s\S]*?(?:-->|$))/,A=k("^ {0,3}(?:<(script|pre|style|textarea)[\\s>][\\s\\S]*?(?:</\\1>[^\\n]*\\n*|$)|comment[^\\n]*(\\n+|$)|<\\?[\\s\\S]*?(?:\\?>[^\\n]*\\n*|$)|<![A-Z][\\s\\S]*?(?:>[^\\n]*\\n*|$)|<!\\[CDATA\\[[\\s\\S]*?(?:\\]\\]>[^\\n]*\\n*|$)|</?(tag)(?: +|\\n|/?>)[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$)|<(?!script|pre|style|textarea)([a-z][a-z0-9-]*)(?:attribute)*? */?>(?=[ \\t]*(?:\\n|$))[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$)|</(?!script|pre|style|textarea)[a-z][a-z0-9-]*\\s*>(?=[ \\t]*(?:\\n|$))[\\s\\S]*?(?:(?:\\n[ \t]*)+\\n|$))","i").replace("comment",_).replace("tag",z).replace("attribute",/ +[a-zA-Z:_][\w.:-]*(?: *= *"[^"\n]*"| *= *'[^'\n]*'| *= *[^\s"'=<>`]+)?/).getRegex(),P=e=>k(S).replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("|lheading","").replace("|table","").replace("blockquote"," {0,3}>").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list",e).replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex(),L=P(/ {0,3}(?:[*+-]|1[.)])[ \t]+[^ \t\n]/),I=P(/ {0,3}(?:[*+-]|\d{1,9}[.)])(?:[ \t]|\n|$)/),E={blockquote:k(/^( {0,3}> ?(paragraph|[^\n]*)(?:\n|$))+/).replace("paragraph",I).getRegex(),code:/^((?: {4}| {0,3}\t)[^\n]+(?:\n(?:[ \t]*(?:\n|$))*)?)+/,def:R,fences:/^ {0,3}(`{3,}(?=[^`\n]*(?:\n|$))|~{3,})([^\n]*)(?:\n|$)(?:|([\s\S]*?)(?:\n|$))(?: {0,3}\1[~`]* *(?=\n|$)|$)/,heading:/^ {0,3}(#{1,6})(?=\s|$)(.*)(?:\n+|$)/,hr:x,html:A,lheading:w,list:T,newline:/^(?:[ \t]*(?:\n|$))+/,paragraph:L,table:u,text:/^[^\n]+/},C=k("^ *([^\\n ].*)\\n {0,3}((?:\\| *)?:?-+:? *(?:\\| *:?-+:? *)*(?:\\| *)?)(?:\\n((?:(?! *\\n|hr|heading|blockquote|code|fences|list|html).*(?:\\n|$))*)\\n*|$)").replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("blockquote"," {0,3}>").replace("code","(?: {4}| {0,3}\t)[^\\n]").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list"," {0,3}(?:[*+-]|1[.)])[ \\t]").replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex(),B={...E,lheading:y,table:C,paragraph:k(S).replace("hr",x).replace("heading"," {0,3}#{1,6}(?:\\s|$)").replace("|lheading","").replace("table",C).replace("blockquote"," {0,3}>").replace("fences"," {0,3}(?:`{3,}(?=[^`\\n]*(?:\\n|$))|~~~)[^\\n]*(?:\\n|$)").replace("list"," {0,3}(?:[*+-]|1[.)])[ \\t]+[^ \\t\\n]").replace("html","</?(?:tag)(?: +|\\n|/?>)|<(?:script|pre|style|textarea|!--)").replace("tag",z).getRegex()},q={...E,html:k("^ *(?:comment *(?:\\n|\\s*$)|<(tag)[\\s\\S]+?</\\1> *(?:\\n{2,}|\\s*$)|<tag(?:\"[^\"]*\"|'[^']*'|\\s[^'\"/>\\s]*)*?/?> *(?:\\n{2,}|\\s*$))").replace("comment",_).replace(/tag/g,"(?!(?:a|em|strong|small|s|cite|q|dfn|abbr|data|time|code|var|samp|kbd|sub|sup|i|b|u|mark|ruby|rt|rp|bdi|bdo|span|br|wbr|ins|del|img)\\b)\\w+(?!:|[^\\w\\s@]*@)\\b").getRegex(),def:/^ *\[([^\]]+)\]: *<?([^\s>]+)>?(?: +(["(][^\n]+[")]))? *(?:\n+|$)/,heading:/^(#{1,6})(.*)(?:\n+|$)/,fences:u,lheading:/^(.+?)\n {0,3}(=+|-+) *(?:\n+|$)/,paragraph:k(S).replace("hr",x).replace("heading"," *#{1,6} *[^\n]").replace("lheading",w).replace("|table","").replace("blockquote"," {0,3}>").replace("|fences","").replace("|list","").replace("|html","").replace("|tag","").getRegex()},Z=/^( {2,}|\\)\n(?!\s*$)[ \t]*/,v=/[\p{P}\p{S}]/u,D=/[\s\p{P}\p{S}]/u,O=/[^\s\p{P}\p{S}]/u,j=k(/^((?![*_])punctSpace)/,"u").replace(/punctSpace/g,D).getRegex(),Q=/(?!~)[\p{P}\p{S}]/u,M=k(/link|precode-code|html/,"g").replace("link",/\[(?:[^\[\]`]|(?<a>`+)[^`]+\k<a>(?!`))*?\]\((?:\\[\s\S]|[^\\\(\)]|\((?:\\[\s\S]|[^\\\(\)])*\))*\)/).replace("precode-",f?"(?<!`)()":"(^^|[^`])").replace("code",/(?<b>`+)[^`]+\k<b>(?!`)/).replace("html",/<(?! )[^<>]*?>/).getRegex(),N=/^(?:\*+(?:((?!\*)punct)|([^\s*]))?)|^_+(?:((?!_)punct)|([^\s_]))?/,H=k(N,"u").replace(/punct/g,v).getRegex(),G=k(N,"u").replace(/punct/g,Q).getRegex(),X=k(/^(?:\*+(?:((?!\*)(?!openQuote)punct)|([^\s*]))?)|^_+(?:((?!_)(?!openQuote)punct)|([^\s_]))?/,"u").replace(/openQuote/g,/[\p{Pi}\p{Ps}"']/u).replace(/punct/g,v).getRegex(),U="^[^_*]*?__[^_*]*?\\*[^_*]*?(?=__)|[^*]+(?=[^*])|(?!\\*)punct(\\*+)(?=[\\s]|$)|notPunctSpace(\\*+)(?!\\*)(?=punctSpace|$)|(?!\\*)punctSpace(\\*+)(?=notPunctSpace)|[\\s](\\*+)(?!\\*)(?=punct)|(?!\\*)punct(\\*+)(?!\\*)(?=punct)|notPunctSpace(\\*+)(?=notPunctSpace)",F=k(U,"gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),W=k(U,"gu").replace(/notPunctSpace/g,/(?:[^\s\p{P}\p{S}]|~)/u).replace(/punctSpace/g,/(?!~)[\s\p{P}\p{S}]/u).replace(/punct/g,Q).getRegex(),J=k("^[^_*]*?__[^_*]*?\\*[^_*]*?(?=__)|[^*]+(?=[^*])|(?!\\*)punct(\\*+)(?=[\\s]|$)|notPunctSpace(\\*+)(?!\\*)(?=punctSpace|$)|(?!\\*)[\\s](\\*+)(?=notPunctSpace)|[\\s](\\*+)(?!\\*)(?=punct)|(?!\\*)punct(\\*+)(?!\\*)(?=punct)|(?:(?!\\*)punct|notPunctSpace)(\\*+)(?!\\*)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),K=k("^[^_*]*?\\*\\*[^_*]*?_[^_*]*?(?=\\*\\*)|[^_]+(?=[^_])|(?!_)punct(_+)(?=[\\s]|$)|notPunctSpace(_+)(?!_)(?=punctSpace|$)|(?!_)punctSpace(_+)(?=notPunctSpace)|[\\s](_+)(?!_)(?=punct)|(?!_)punct(_+)(?!_)(?=punct)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),V=k("^[^_*]*?\\*\\*[^_*]*?_[^_*]*?(?=\\*\\*)|[^_]+(?=[^_])|(?!_)punct(_+)(?=[\\s]|$)|notPunctSpace(_+)(?!_)(?=punctSpace|$)|(?!_)[\\s](_+)(?=notPunctSpace)|[\\s](_+)(?!_)(?=punct)|(?!_)punct(_+)(?!_)(?=punct)|(?:(?!_)punct|notPunctSpace)(_+)(?!_)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),Y=k(/^~~?(?:((?!~)punct)|[^\s~])/,"u").replace(/punct/g,v).getRegex(),ee=k("^[^~]+(?=[^~])|(?!~)punct(~~?)(?=[\\s]|$)|notPunctSpace(~~?)(?!~)(?=punctSpace|$)|(?!~)punctSpace(~~?)(?=notPunctSpace)|[\\s](~~?)(?!~)(?=punct)|(?!~)punct(~~?)(?!~)(?=punct)|notPunctSpace(~~?)(?=notPunctSpace)","gu").replace(/notPunctSpace/g,O).replace(/punctSpace/g,D).replace(/punct/g,v).getRegex(),te=k(/\\(punct)/,"gu").replace(/punct/g,v).getRegex(),ne=k(/^<(scheme:[^\s\x00-\x1f<>]*|email)>/).replace("scheme",/[a-zA-Z][a-zA-Z0-9+.-]{1,31}/).replace("email",/[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+(@)[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)+(?![-_])/).getRegex(),re=k(_).replace("(?:--\x3e|$)","--\x3e").getRegex(),se=k("^comment|^</[a-zA-Z][a-zA-Z0-9-]*\\s*>|^<[a-zA-Z][a-zA-Z0-9-]*(?:attribute)*?\\s*/?>|^<\\?[\\s\\S]*?\\?>|^<![a-zA-Z]+\\s[\\s\\S]*?>|^<!\\[CDATA\\[[\\s\\S]*?\\]\\]>").replace("comment",re).replace("attribute",/\s+[a-zA-Z:_][\w.:-]*(?:\s*=\s*"[^"]*"|\s*=\s*'[^']*'|\s*=\s*[^\s"'=<>`]+)?/).getRegex(),le=/\[(?:\\[\s\S]|[^\[\]\\])*\]/,ie=k(/(?:\[(?:brackets|\\[\s\S]|[^\[\]\\])*\]|\\[\s\S]|`+(?!`)[^`]*?`+(?!`)|``+(?=\])|[^\[\]\\`])*?/).replace("brackets",le).getRegex(),ae=k(/^!?\[(label)\]\(\s*(href)(?:(?:[ \t]+(?:\n[ \t]*)?|\n[ \t]*)(title))?\s*\)/).replace("label",ie).replace("href",/<(?:\\.|[^\n<>\\])+>|[^ \t\n\x00-\x1f]+|(?=\))/).replace("title",/"(?:\\"?|[^"\\])*"|'(?:\\'?|[^'\\])*'|\((?:\\\)?|[^)\\])*\)/).getRegex(),oe=k(/^!?\[(label)\]\[(ref)\]/).replace("label",ie).replace("ref",$).getRegex(),ce=k(/^!?\[(ref)\](?:\[\])?/).replace("ref",$).getRegex(),pe=/(?!\s*\])(?:\\[\s\S]|[^\[\]\\]){1,999}/,he=k(/(?:[^\[\]\\`]*(?:\[(?:brackets|\\[\s\S]|[^\[\]\\])*\]|\\[\s\S]|`+(?!`)[^`]*?`+(?!`)|``+(?=\]))){0,999}?[^\[\]\\`]*?/).replace("brackets",le).getRegex(),ue=k("reflink|nolink(?!\\()","g").replace("reflink",k(/^!?\[(label)\]\[(ref)\]/).replace("label",he).replace("ref",pe).getRegex()).replace("nolink",k(/^!?\[(ref)\](?:\[\])?/).replace("ref",pe).getRegex()).getRegex(),ge=/[hH][tT][tT][pP][sS]?|[fF][tT][pP]/,ke=k(/(?:mailto:email|xmpp:email(?:\/[A-Za-z0-9@.]+)?)/).replace(/email/g,/[A-Za-z0-9._+-]+@[a-zA-Z0-9-_]+(?:\.[a-zA-Z0-9-_]*[a-zA-Z0-9])+(?![\w-])/).getRegex(),fe={_backpedal:u,anyPunctuation:te,autolink:ne,blockSkip:M,br:Z,code:/^(`+)([^`]|[^`][\s\S]*?[^`])\1(?!`)/,del:u,delLDelim:u,delRDelim:u,emStrongLDelim:H,emStrongRDelimAst:F,emStrongRDelimUnd:K,escape:/^\\([!"#$%&'()*+,\-./:;<=>?@\[\]\\^_`{|}~])/,link:ae,nolink:ce,punctuation:j,reflink:oe,reflinkSearch:ue,tag:se,text:/^(`+|[^`])(?:(?= {2,}\n)|[\s\S]*?(?:(?=[\\<!\[`*_]|\b_|$)|[^ ](?= {2,}\n)))/,url:u},de={...fe,emStrongLDelim:X,emStrongRDelimAst:J,emStrongRDelimUnd:V,link:k(/^!?\[(label)\]\((.*?)\)/).replace("label",ie).getRegex(),reflink:k(/^!?\[(label)\]\s*\[([^\]]*)\]/).replace("label",ie).getRegex()},xe={...fe,emStrongRDelimAst:W,emStrongLDelim:G,delLDelim:Y,delRDelim:ee,url:k(/^emailProtocol|^((?:protocol):\/\/|www\.)(?:[a-zA-Z0-9\-]+\.?)+[^\s<]*|^email/).replace("emailProtocol",ke).replace("protocol",ge).replace("email",/[A-Za-z0-9._+-]+(@)[a-zA-Z0-9-_]+(?:\.[a-zA-Z0-9-_]*[a-zA-Z0-9])+(?![\w-])/).getRegex(),_backpedal:/(?:[^?!.,:;*_'"~()&]+|\([^)]*\)|&(?![a-zA-Z0-9]+;$)|[?!.,:;*_'"~)]+(?!$))+/,del:/^(~~?)(?=[^\s~])((?:\\[\s\S]|[^\\])*?(?:\\[\s\S]|[^\s~\\]))\1(?=[^~]|$)/,text:k(/^(?:[^a-zA-Z0-9](?=emailProtocol)|(`+|~+|[^`~])(?:(?=[`~])|(?= {2,}\n)|(?=[a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-]+@)|[\s\S]*?(?:(?=[\\<!\[`*~_]|\b_|protocol:\/\/|www\.|$)|[^ ](?= {2,}\n)|[^a-zA-Z0-9](?=emailProtocol)|[^a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-](?=[a-zA-Z0-9.!#$%&'*+\/=?_`{\|}~-]+@))))/).replace("protocol",ge).replace(/emailProtocol/g,/(?:mailto|xmpp):/).getRegex()},be={...xe,br:k(Z).replace("{2,}","*").getRegex(),text:k(xe.text).replace("\\b_","\\b_| {2,}\\n").replace(/\{2,\}/g,"*").getRegex()},me={normal:E,gfm:B,pedantic:q},we={normal:fe,gfm:xe,breaks:be,pedantic:de},ye={"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"},Se=e=>ye[e];function $e(e,t){if(t){if(d.escapeTest.test(e))return e.replace(d.escapeReplace,Se)}else if(d.escapeTestNoEncode.test(e))return e.replace(d.escapeReplaceNoEncode,Se);return e}function Re(e){return e.replace(d.numericCharacterReference,(e,t,n)=>{let r=void 0===t?Number.parseInt(n,16):Number.parseInt(t,10);return 0===r||r>1114111||r>=55296&&r<=57343?"�":String.fromCodePoint(r)})}function Te(e){try{e=encodeURI(e).replace(d.percentDecode,"%")}catch{return null}return e}function ze(e,t){let n=e.replace(d.findPipe,(e,t,n)=>{let r=!1,s=t;for(;--s>=0&&"\\"===n[s];)r=!r;return r?"|":" |"}).split(d.splitPipe),r=0;if(n[0].trim()||n.shift(),n.length>0&&!n.at(-1)?.trim()&&n.pop(),t)if(n.length>t)n.splice(t);else for(;n.length<t;)n.push("");for(;r<n.length;r++)n[r]=n[r].trim().replace(d.slashPipe,"|");return n}function _e(e,t,n){let r=e.length;if(0===r)return"";let s=0;for(;s<r;){let l=e.charAt(r-s-1);if(l!==t||n){if(l===t||!n)break;s++}else s++}return e.slice(0,r-s)}function Ae(e){let t=e.split("\n"),n=t.length-1;for(;n>=0&&d.blankLine.test(t[n]);)n--;return t.length-n<=2?e:t.slice(0,n+1).join("\n")}function Pe(e){return e.trim().toLowerCase().toUpperCase().toLowerCase()}function Le(e,t=0){let n=t,r="";for(let t of e)if("\t"===t){let e=4-n%4;r+=" ".repeat(e),n+=e}else r+=t,n++;return r}function Ie(e,t,n,r,s){let l=t.href,i=t.title||null,a=e[1].replace(s.other.outputLinkReplace,"$1"),o="!"===e[0].charAt(0);r.state.inLink=!0;let c=r.state.linkEmitted,p=r.state.inRawBlock;r.state.linkEmitted=!1;let h=r.inlineTokens(a),u=r.state.linkEmitted;if(r.state.linkEmitted=c,r.state.inLink=!1,!o){if(u)return void(r.state.inRawBlock=p);r.state.linkEmitted=!0}return{type:o?"image":"link",raw:n,href:l,title:i,text:a,tokens:h}}function Ee(e,t,n,r){if(!t.includes("<"))return!1;for(let s=0;s<t.length;s++){if("\\"===t[s]){s++;continue}if("`"===t[s]){let e=r.inline.code.exec(t.slice(s));if(e){s+=e[0].length-1;continue}}if("<"!==t[s])continue;let l=e.slice(n+s),i=r.inline.tag.exec(l)||r.inline.autolink.exec(l);if(i){if(i[0].length>t.length-s)return!0;s+=i[0].length-1}}return!1}var Ce=class{options;rules;lexer;constructor(e){this.options=e||p}space(e){let t=this.rules.block.newline.exec(e);if(t&&t[0].length>0)return{type:"space",raw:t[0]}}code(e){let t=this.rules.block.code.exec(e);if(t){let e=this.options.pedantic?t[0]:Ae(t[0]),n=e.replace(this.rules.other.codeRemoveIndent,"");return{type:"code",raw:e,codeBlockStyle:"indented",text:n}}}fences(e){let t=this.rules.block.fences.exec(e);if(t){let e=t[0],n=function(e,t,n){let r=e.match(n.other.indentCodeCompensation);if(null===r)return t;let s=r[1];return t.split("\n").map(e=>{let t=e.match(n.other.beginningSpace);if(null===t)return e;let[r]=t;return e.slice(Math.min(r.length,s.length))}).join("\n")}(e,t[3]||"",this.rules);return{type:"code",raw:e,lang:t[2]?t[2].trim().replace(this.rules.inline.anyPunctuation,"$1"):t[2],text:n}}}heading(e){let t=this.rules.block.heading.exec(e);if(t){let e=t[2].trim();if(this.rules.other.endingHash.test(e)){let t=_e(e,"#");(this.options.pedantic||!t||this.rules.other.endingSpaceTabChar.test(t))&&(e=t.trim())}return{type:"heading",raw:_e(t[0],"\n"),depth:t[1].length,text:e,tokens:this.lexer.inline(e)}}}hr(e){let t=this.rules.block.hr.exec(e);if(t)return{type:"hr",raw:_e(t[0],"\n")}}blockquote(e){let t=this.rules.block.blockquote.exec(e);if(t){let e=_e(t[0],"\n").split("\n"),n="",r="",s=[];for(;e.length>0;){let t,l=!1,i=[];for(t=0;t<e.length;t++)if(this.rules.other.blockquoteStart.test(e[t]))i.push(e[t]),l=!0;else{if(l)break;i.push(e[t])}e=e.slice(t);let a=i.join("\n"),o=a.replace(this.rules.other.blockquoteSetextReplace,"\n    $1").replace(this.rules.other.blockquoteSetextReplace2,"");n=n?`${n}\n${a}`:a,r=r?`${r}\n${o}`:o;let c=this.lexer.state.top;if(this.lexer.state.top=!0,this.lexer.blockTokens(o,s,!0),this.lexer.state.top=c,0===e.length)break;let p=s.at(-1);if("code"===p?.type)break;if("blockquote"===p?.type){let t=p,l=e.join("\n"),i=t.raw+"\n"+l.replace(this.rules.other.blockquoteSetextReplace2,""),a=this.blockquote(i);s[s.length-1]=a;let o=i.substring(a.raw.length).replace(/^\n/,""),c=o?o.split("\n").length:0,h=c?e.slice(0,-c):e;h.length>0&&(n=`${n}\n${h.join("\n")}`),r=r.substring(0,r.length-t.text.length)+a.text;break}if("list"===p?.type){let t=p,l=t.raw+"\n"+e.join("\n"),i=this.list(l);s[s.length-1]=i,n=n.substring(0,n.length-p.raw.length)+i.raw,r=r.substring(0,r.length-t.raw.length)+i.raw,e=l.substring(s.at(-1).raw.length).split("\n");continue}}return{type:"blockquote",raw:n,tokens:s,text:r}}}list(e){let t=this.rules.block.list.exec(e);if(t){let n=t[1].trim(),r=n.length>1,s={type:"list",raw:"",ordered:r,start:r?+n.slice(0,-1):"",loose:!1,items:[]};n=r?`\\d{1,9}\\${n.slice(-1)}`:`\\${n}`,this.options.pedantic&&(n=r?n:"[*+-]");let l=this.rules.other.listItemRegex(n),i=!1;for(;e;){let n=!1,r="",a="";if(!(t=l.exec(e))||this.rules.block.hr.test(e))break;r=t[0],e=e.substring(r.length);let o=t[2].split("\n",1)[0],c=t[1].length,p=this.options.pedantic?Le(o,c):o.replace(this.rules.other.leadingSpaceTab,e=>Le(e,c)),h=e.split("\n",1)[0],u=!p.trim(),g=0;if(this.options.pedantic?(g=2,a=p.trimStart()):u?g=c+1:(g=p.search(this.rules.other.nonSpaceChar),g=g>4?1:g,a=p.slice(g),g+=c),u&&this.rules.other.blankLine.test(h)&&(r+=h+"\n",e=e.substring(h.length+1),n=!0),!n){let t=this.rules.other.nextBulletRegex(g),n=this.rules.other.hrRegex(g),s=this.rules.other.fencesBeginRegex(g),l=this.rules.other.headingBeginRegex(g),i=this.rules.other.htmlBeginRegex(g),o=this.rules.other.blockquoteBeginRegex(g);for(;e;){let c,k=e.split("\n",1)[0];if(h=k,this.options.pedantic?(h=h.replace(this.rules.other.listReplaceNesting,"  "),c=h):c=h.replace(this.rules.other.leadingSpaceTab,e=>e.replace(this.rules.other.tabCharGlobal,"    ")),s.test(h)||l.test(h)||i.test(h)||o.test(h)||t.test(h)||n.test(h))break;if(c.search(this.rules.other.nonSpaceChar)>=g||!h.trim())a+="\n"+c.slice(g);else{if(u||p.replace(this.rules.other.tabCharGlobal,"    ").search(this.rules.other.nonSpaceChar)>=4||s.test(p)||l.test(p)||n.test(p))break;a+="\n"+h}u=!h.trim(),r+=k+"\n",e=e.substring(k.length+1),p=c.slice(g)}}s.loose||(i?s.loose=!0:this.rules.other.doubleBlankLine.test(r)&&(i=!0)),s.items.push({type:"list_item",raw:r,task:!!this.options.gfm&&this.rules.other.listIsTask.test(a),loose:!1,text:a,tokens:[]}),s.raw+=r}let a=s.items.at(-1);if(!a)return;a.raw=a.raw.trimEnd(),a.text=a.text.trimEnd(),s.raw=s.raw.trimEnd();for(let e of s.items)if(this.lexer.state.top=!1,e.tokens=this.lexer.blockTokens(e.text,[]),!s.loose){let t=e.tokens.filter(e=>"space"===e.type),n=t.length>0&&t.some(e=>this.rules.other.anyLine.test(e.raw));s.loose=n}for(let e of s.items){let t=e.tokens[0];if(!e.task||"text"!==t?.type&&"paragraph"!==t?.type)e.task&&(e.task=!1);else{e.text=e.text.replace(this.rules.other.listReplaceTask,""),t.raw=t.raw.replace(this.rules.other.listReplaceTask,""),t.text=t.text.replace(this.rules.other.listReplaceTask,"");for(let e=this.lexer.inlineQueue.length-1;e>=0;e--)if(this.rules.other.listIsTask.test(this.lexer.inlineQueue[e].src)){this.lexer.inlineQueue[e].src=this.lexer.inlineQueue[e].src.replace(this.rules.other.listReplaceTask,"");break}let n=this.rules.other.listTaskCheckbox.exec(e.raw);if(n){let t={type:"checkbox",raw:n[0]+" ",checked:"[ ]"!==n[0]};e.checked=t.checked,s.loose?e.tokens[0]&&["paragraph","text"].includes(e.tokens[0].type)&&"tokens"in e.tokens[0]&&e.tokens[0].tokens?(e.tokens[0].raw=t.raw+e.tokens[0].raw,e.tokens[0].text=t.raw+e.tokens[0].text,e.tokens[0].tokens.unshift(t)):e.tokens.unshift({type:"paragraph",raw:t.raw,text:t.raw,tokens:[t]}):e.tokens.unshift(t)}}}if(s.loose)for(let e of s.items){e.loose=!0;for(let t of e.tokens)"text"===t.type&&(t.type="paragraph")}return s}}html(e){let t=this.rules.block.html.exec(e);if(t){let e=Ae(t[0]);return{type:"html",block:!0,raw:e,pre:"pre"===t[1]||"script"===t[1]||"style"===t[1],text:e}}}def(e){let t=this.rules.block.def.exec(e);if(t){let e=Pe(t[1]).replace(this.rules.other.multipleSpaceGlobal," "),n=t[2]?t[2].replace(this.rules.other.hrefBrackets,"$1").replace(this.rules.inline.anyPunctuation,"$1"):"",r=t[3]?t[3].substring(1,t[3].length-1).replace(this.rules.inline.anyPunctuation,"$1"):t[3];return{type:"def",tag:e,raw:_e(t[0],"\n"),href:n,title:r}}}table(e){let t=this.rules.block.table.exec(e);if(!t||!this.rules.other.tableDelimiter.test(t[2]))return;let n=ze(t[1]),r=t[2].replace(this.rules.other.tableAlignChars,"").split("|"),s=t[3]?.trim()?t[3].replace(this.rules.other.tableRowBlankLine,"").split("\n"):[],l={type:"table",raw:_e(t[0],"\n"),header:[],align:[],rows:[]};if(n.length===r.length){for(let e of r)this.rules.other.tableAlignRight.test(e)?l.align.push("right"):this.rules.other.tableAlignCenter.test(e)?l.align.push("center"):this.rules.other.tableAlignLeft.test(e)?l.align.push("left"):l.align.push(null);for(let e=0;e<n.length;e++)l.header.push({text:n[e],tokens:this.lexer.inline(n[e]),header:!0,align:l.align[e]});for(let e of s)l.rows.push(ze(e,l.header.length).map((e,t)=>({text:e,tokens:this.lexer.inline(e),header:!1,align:l.align[t]})));return l}}lheading(e){let t=this.rules.block.lheading.exec(e);if(t){let e=t[1].trim();return{type:"heading",raw:_e(t[0],"\n"),depth:"="===t[2].charAt(0)?1:2,text:e,tokens:this.lexer.inline(e)}}}paragraph(e){let t=this.rules.block.paragraph.exec(e);if(t){let e="\n"===t[1].charAt(t[1].length-1)?t[1].slice(0,-1):t[1];return{type:"paragraph",raw:t[0],text:e,tokens:this.lexer.inline(e)}}}text(e){let t=this.rules.block.text.exec(e);if(t)return{type:"text",raw:t[0],text:t[0],tokens:this.lexer.inline(t[0])}}escape(e){let t=this.rules.inline.escape.exec(e);if(t)return{type:"escape",raw:t[0],text:t[1]}}tag(e){let t=this.rules.inline.tag.exec(e);if(t)return!this.lexer.state.inLink&&this.rules.other.startATag.test(t[0])?this.lexer.state.inLink=!0:this.lexer.state.inLink&&this.rules.other.endATag.test(t[0])&&(this.lexer.state.inLink=!1),!this.lexer.state.inRawBlock&&this.rules.other.startPreScriptTag.test(t[0])?this.lexer.state.inRawBlock=!0:this.lexer.state.inRawBlock&&this.rules.other.endPreScriptTag.test(t[0])&&(this.lexer.state.inRawBlock=!1),{type:"html",raw:t[0],inLink:this.lexer.state.inLink,inRawBlock:this.lexer.state.inRawBlock,block:!1,text:t[0]}}link(e){let t=this.rules.inline.link.exec(e);if(t){let n="!"===t[0].charAt(0)?2:1;if(!this.options.pedantic&&Ee(e,t[1],n,this.rules))return;let r=t[2].trim();if(!this.options.pedantic&&this.rules.other.startAngleBracket.test(r)){if(!this.rules.other.endAngleBracket.test(r))return;let e=_e(r.slice(0,-1),"\\");if((r.length-e.length)%2==0)return}else{let e=function(e,t){if(-1===e.indexOf(t[1]))return-1;let n=0;for(let r=0;r<e.length;r++)if("\\"===e[r])r++;else if(e[r]===t[0])n++;else if(e[r]===t[1]&&(n--,n<0))return r;return n>0?-2:-1}(t[2],"()");if(-2===e)return;if(e>-1){let n=(0===t[0].indexOf("!")?5:4)+t[1].length+e;t[2]=t[2].substring(0,e),t[0]=t[0].substring(0,n).trim(),t[3]=""}}let s=t[2],l="";if(this.options.pedantic){let e=this.rules.other.pedanticHrefTitle.exec(s);e&&(s=e[1],l=e[3])}else l=t[3]?t[3].slice(1,-1):"";return s=s.trim(),this.rules.other.startAngleBracket.test(s)&&(s=this.options.pedantic&&!this.rules.other.endAngleBracket.test(r)?s.slice(1):s.slice(1,-1)),Ie(t,{href:s&&s.replace(this.rules.inline.anyPunctuation,"$1"),title:l&&l.replace(this.rules.inline.anyPunctuation,"$1")},t[0],this.lexer,this.rules)}}reflink(e,t){let n;if((n=this.rules.inline.reflink.exec(e))||(n=this.rules.inline.nolink.exec(e))){let r="!"===n[0].charAt(0)?2:1;if(!this.options.pedantic&&Ee(e,n[1],r,this.rules))return;let s=t[Pe((n[2]||n[1]).replace(this.rules.other.multipleSpaceGlobal," "))];if(!s){let e=n[0].charAt(0);return{type:"text",raw:e,text:e}}return Ie(n,s,n[0],this.lexer,this.rules)}}emStrong(e,t,n=""){let r=this.rules.inline.emStrongLDelim.exec(e);if(r&&(r[1]||r[2]||r[3]||r[4])&&(!r[4]||!n.match(this.rules.other.unicodeAlphaNumeric))&&(!r[1]&&!r[3]||!n||this.rules.inline.punctuation.exec(n))){let s,l,i=[...r[0]].length-1,a=i,o=0,c=r[0][0],p=n===c,h="*"===c?this.rules.inline.emStrongRDelimAst:this.rules.inline.emStrongRDelimUnd;for(h.lastIndex=0,t=t.slice(-1*e.length+i);null!==(r=h.exec(t));){if(s=r[1]||r[2]||r[3]||r[4]||r[5]||r[6],!s)continue;if(l=[...s].length,r[3]||r[4]){a+=l;continue}if(r[5]||r[6]){if(i%3&&!((i+l)%3)){o+=l;continue}if(p)break}if(a-=l,a>0)continue;l=Math.min(l,l+a+o);let t=[...r[0]][0].length,n=e.slice(0,i+r.index+t+l);if(Math.min(i,l)%2){let e=n.slice(1,-1);return{type:"em",raw:n,text:e,tokens:this.lexer.inlineTokens(e)}}let c=n.slice(2,-2);return{type:"strong",raw:n,text:c,tokens:this.lexer.inlineTokens(c)}}}}codespan(e){let t=this.rules.inline.code.exec(e);if(t){let e=t[2].replace(this.rules.other.newLineCharGlobal," "),n=this.rules.other.nonSpaceChar.test(e),r=this.rules.other.startingSpaceChar.test(e)&&this.rules.other.endingSpaceChar.test(e);return n&&r&&(e=e.substring(1,e.length-1)),{type:"codespan",raw:t[0],text:e}}}br(e){let t=this.rules.inline.br.exec(e);if(t)return{type:"br",raw:t[0]}}del(e,t,n=""){let r=this.rules.inline.delLDelim.exec(e);if(r&&(!r[1]||!n||this.rules.inline.punctuation.exec(n))){let n,s,l=[...r[0]].length-1,i=l,a=this.rules.inline.delRDelim;for(a.lastIndex=0,t=t.slice(-1*e.length+l);null!==(r=a.exec(t));){if(n=r[1]||r[2]||r[3]||r[4]||r[5]||r[6],!n||(s=[...n].length,s!==l))continue;if(r[3]||r[4]){i+=s;continue}if(i-=s,i>0)continue;s=Math.min(s,s+i);let t=[...r[0]][0].length,a=e.slice(0,l+r.index+t+s),o=a.slice(l,-l);return{type:"del",raw:a,text:o,tokens:this.lexer.inlineTokens(o)}}}}autolink(e){let t=this.rules.inline.autolink.exec(e);if(t){let e,n;return"@"===t[2]?(e=t[1],n="mailto:"+e):(e=t[1],n=e),{type:"link",raw:t[0],text:e,href:n,autolink:!0,tokens:[{type:"text",raw:e,text:e}]}}}url(e){let t;if(t=this.rules.inline.url.exec(e)){let e,n;if("@"===t[2])e=t[0],n="mailto:"+e;else{let r;do{r=t[0],t[0]=this.rules.inline._backpedal.exec(t[0])?.[0]??""}while(r!==t[0]);e=t[0],n="www."===t[1]?"\u0068\u0074\u0074\u0070://"+t[0]:t[0]}return{type:"link",raw:t[0],text:e,href:n,autolink:!0,tokens:[{type:"text",raw:e,text:e}]}}}inlineText(e){let t=this.rules.inline.text.exec(e);if(t){let e=this.lexer.state.inRawBlock;return{type:"text",raw:t[0],text:e?t[0]:Re(t[0]),escaped:e}}}},Be=class e{tokens;options;state;inlineQueue;tokenizer;constructor(e){this.tokens=[],this.tokens.links=Object.create(null),this.options=e||p,this.options.tokenizer=this.options.tokenizer||new Ce,this.tokenizer=this.options.tokenizer,this.tokenizer.options=this.options,this.tokenizer.lexer=this,this.inlineQueue=[],this.state={inLink:!1,inRawBlock:!1,linkEmitted:!1,top:!0};let t={other:d,block:me.normal,inline:we.normal};this.options.pedantic?(t.block=me.pedantic,t.inline=we.pedantic):this.options.gfm&&(t.block=me.gfm,this.options.breaks?t.inline=we.breaks:t.inline=we.gfm),this.tokenizer.rules=t}static get rules(){return{block:me,inline:we}}static lex(t,n){return new e(n).lex(t)}static lexInline(t,n){return new e(n).inlineTokens(t)}lex(e){e=e.replace(d.carriageReturn,"\n"),this.blockTokens(e,this.tokens);for(let e=0;e<this.inlineQueue.length;e++){let t=this.inlineQueue[e];this.inlineTokens(t.src,t.tokens)}return this.inlineQueue=[],this.tokens}blockTokens(e,t=[],n=!1){this.tokenizer.lexer=this,this.options.pedantic&&(e=e.replace(d.tabCharGlobal,"    ").replace(d.spaceLine,""));let r=1/0;for(;e;){if(!(e.length<r)){this.infiniteLoopError(e.charCodeAt(0));break}let s;if(r=e.length,this.options.extensions?.block?.some(n=>!!(s=n.call({lexer:this},e,t))&&(e=e.substring(s.raw.length),t.push(s),!0)))continue;if(s=this.tokenizer.space(e)){e=e.substring(s.raw.length);let n=t.at(-1);1===s.raw.length&&void 0!==n?n.raw+="\n":t.push(s);continue}if(s=this.tokenizer.code(e)){e=e.substring(s.raw.length);let n=t.at(-1);"paragraph"===n?.type||"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.text,this.inlineQueue.at(-1).src=n.text):t.push(s);continue}if(s=this.tokenizer.fences(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.heading(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.hr(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.blockquote(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.list(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.html(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.def(e)){e=e.substring(s.raw.length);let n=t.at(-1);"paragraph"===n?.type||"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.raw,this.inlineQueue.at(-1).src=n.text):this.tokens.links[s.tag]||(this.tokens.links[s.tag]={href:s.href,title:s.title},t.push(s));continue}if(s=this.tokenizer.table(e)){e=e.substring(s.raw.length),t.push(s);continue}if(s=this.tokenizer.lheading(e)){e=e.substring(s.raw.length),t.push(s);continue}let l=e;if(this.options.extensions?.startBlock){let t,n=1/0,r=e.slice(1);this.options.extensions.startBlock.forEach(e=>{t=e.call({lexer:this},r),"number"==typeof t&&t>=0&&(n=Math.min(n,t))}),n<1/0&&n>=0&&(l=e.substring(0,n+1))}if(this.state.top&&(s=this.tokenizer.paragraph(l))){let r=t.at(-1);n&&"paragraph"===r?.type?(r.raw+=(r.raw.endsWith("\n")?"":"\n")+s.raw,r.text+="\n"+s.text,this.inlineQueue.pop(),this.inlineQueue.at(-1).src=r.text):t.push(s),n=l.length!==e.length,e=e.substring(s.raw.length);continue}if(s=this.tokenizer.text(e)){e=e.substring(s.raw.length);let n=t.at(-1);"text"===n?.type?(n.raw+=(n.raw.endsWith("\n")?"":"\n")+s.raw,n.text+="\n"+s.text,this.inlineQueue.pop(),this.inlineQueue.at(-1).src=n.text):t.push(s);continue}if(e){this.infiniteLoopError(e.charCodeAt(0));break}}return this.state.top=!0,t}inline(e,t=[]){return this.inlineQueue.push({src:e,tokens:t}),t}linkInText(e){if(!e.includes("["))return!1;let t=this.tokenizer.rules.inline.link;for(let n of e.matchAll(this.tokenizer.rules.inline.blockSkip))if(t.test(n[0])&&"!"!==e.charAt(n.index-1))return!0;for(let t of e.matchAll(this.tokenizer.rules.inline.reflinkSearch)){let e=t[0],n=e.lastIndexOf("[");if("!"!==e.charAt(0)&&Object.hasOwn(this.tokens.links,Pe(e.slice(n+1,-1)))&&!(n>1&&this.linkInText(e.slice(1,n-1))))return!0}return!1}inlineTokens(e,t=[]){this.tokenizer.lexer=this;let n=e;if(this.tokens.links&&e.includes("[")){let e=this.tokenizer.rules.inline.reflinkSearch,t=n=>{let r=n.lastIndexOf("[");if(!Object.hasOwn(this.tokens.links,Pe(n.slice(r+1,-1))))return n;if(r>1&&"!"!==n.charAt(0)){let s=n.slice(1,r-1);if(this.linkInText(s))return"["+s.replace(e,t)+"]["+"a".repeat(n.length-r-2)+"]"}return"["+"a".repeat(n.length-2)+"]"};n=n.replace(e,t)}n=n.replace(this.tokenizer.rules.inline.anyPunctuation,e=>"+".repeat(e.length)),n=n.replace(this.tokenizer.rules.inline.blockSkip,(e,t,n)=>{let r=n?n.length:0;return e.slice(0,r)+"["+"a".repeat(e.length-r-2)+"]"}),n=this.options.hooks?.emStrongMask?.call({lexer:this},n)??n;let r=!1,s="",l=1/0;for(;e;){if(!(e.length<l)){this.infiniteLoopError(e.charCodeAt(0));break}let i;if(l=e.length,r||(s=""),r=!1,this.options.extensions?.inline?.some(n=>!!(i=n.call({lexer:this},e,t))&&(e=e.substring(i.raw.length),t.push(i),!0)))continue;if(i=this.tokenizer.escape(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.tag(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.link(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.reflink(e,this.tokens.links)){e=e.substring(i.raw.length);let n=t.at(-1);"text"===i.type&&"text"===n?.type?(n.raw+=i.raw,n.text+=i.text):t.push(i);continue}if(i=this.tokenizer.emStrong(e,n,s)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.codespan(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.br(e)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.del(e,n,s)){e=e.substring(i.raw.length),t.push(i);continue}if(i=this.tokenizer.autolink(e)){e=e.substring(i.raw.length),t.push(i);continue}if(!this.state.inLink&&(i=this.tokenizer.url(e))){e=e.substring(i.raw.length),t.push(i);continue}let a=e;if(this.options.extensions?.startInline){let t,n=1/0,r=e.slice(1);this.options.extensions.startInline.forEach(e=>{t=e.call({lexer:this},r),"number"==typeof t&&t>=0&&(n=Math.min(n,t))}),n<1/0&&n>=0&&(a=e.substring(0,n+1))}if(i=this.tokenizer.inlineText(a)){e=e.substring(i.raw.length),"_"!==i.raw.slice(-1)&&(s=i.raw.slice(-1)),r=!0;let n=t.at(-1);"text"===n?.type?(n.raw+=i.raw,n.text+=i.text):t.push(i);continue}if(e){this.infiniteLoopError(e.charCodeAt(0));break}}return t}infiniteLoopError(e){let t="Infinite loop on byte: "+e;if(!this.options.silent)throw new Error(t);console.error(t)}},qe=class{options;parser;constructor(e){this.options=e||p}space(e){return""}code({text:e,lang:t,escaped:n}){let r=(t||"").match(d.notSpaceStart)?.[0],s=e?e.replace(d.endingNewline,"")+"\n":"";return r?'<pre><code class="language-'+$e(r)+'">'+(n?s:$e(s,!0))+"</code></pre>\n":"<pre><code>"+(n?s:$e(s,!0))+"</code></pre>\n"}blockquote({tokens:e}){return`<blockquote>\n${this.parser.parse(e)}</blockquote>\n`}html({text:e}){return e}def(e){return""}heading({tokens:e,depth:t}){return`<h${t}>${this.parser.parseInline(e)}</h${t}>\n`}hr(e){return"<hr>\n"}list(e){let t=e.ordered,n=e.start,r="";for(let t=0;t<e.items.length;t++){let n=e.items[t];r+=this.listitem(n)}let s=t?"ol":"ul";return"<"+s+(t&&1!==n?' start="'+n+'"':"")+">\n"+r+"</"+s+">\n"}listitem(e){return`<li>${this.parser.parse(e.tokens)}</li>\n`}checkbox({checked:e}){return"<input "+(e?'checked="" ':"")+'disabled="" type="checkbox"> '}paragraph({tokens:e}){return`<p>${this.parser.parseInline(e)}</p>\n`}table(e){let t="",n="";for(let t=0;t<e.header.length;t++)n+=this.tablecell(e.header[t]);t+=this.tablerow({text:n});let r="";for(let t=0;t<e.rows.length;t++){let s=e.rows[t];n="";for(let e=0;e<s.length;e++)n+=this.tablecell(s[e]);r+=this.tablerow({text:n})}return r&&(r=`<tbody>${r}</tbody>`),"<table>\n<thead>\n"+t+"</thead>\n"+r+"</table>\n"}tablerow({text:e}){return`<tr>\n${e}</tr>\n`}tablecell(e){let t=this.parser.parseInline(e.tokens),n=e.header?"th":"td";return(e.align?`<${n} align="${e.align}">`:`<${n}>`)+t+`</${n}>\n`}strong({tokens:e}){return`<strong>${this.parser.parseInline(e)}</strong>`}em({tokens:e}){return`<em>${this.parser.parseInline(e)}</em>`}codespan({text:e}){return`<code>${$e(e,!0)}</code>`}br(e){return"<br>"}del({tokens:e}){return`<del>${this.parser.parseInline(e)}</del>`}link({href:e,title:t,text:n,tokens:r,autolink:s}){let l=s?$e(n,!0):this.parser.parseInline(r),i=Te(e);if(null===i)return l;let a='<a href="'+(e=$e(i,s))+'"';return t&&(a+=' title="'+$e(t)+'"'),a+=">"+l+"</a>",a}image({href:e,title:t,text:n,tokens:r}){r&&(n=this.parser.parseInline(r,this.parser.textRenderer));let s=Te(e);if(null===s)return $e(n);let l=`<img src="${$e(e=s)}" alt="${$e(n)}"`;return t&&(l+=` title="${$e(t)}"`),l+=">",l}text(e){return"tokens"in e&&e.tokens?this.parser.parseInline(e.tokens):"escaped"in e&&e.escaped?e.text:$e(e.text)}},Ze=class{strong({text:e}){return e}em({text:e}){return e}codespan({text:e}){return e}del({text:e}){return e}html({text:e}){return e}text({text:e}){return e}link({text:e}){return""+e}image({text:e}){return""+e}br(){return""}checkbox({raw:e}){return e}},ve=class e{options;renderer;textRenderer;constructor(e){this.options=e||p,this.options.renderer=this.options.renderer||new qe,this.renderer=this.options.renderer,this.renderer.options=this.options,this.renderer.parser=this,this.textRenderer=new Ze}static parse(t,n){return new e(n).parse(t)}static parseInline(t,n){return new e(n).parseInline(t)}parse(e){this.renderer.parser=this;let t="";for(let n=0;n<e.length;n++){let r=e[n];if(this.options.extensions?.renderers?.[r.type]){let e=r,n=this.options.extensions.renderers[e.type].call({parser:this},e);if(!1!==n||!["space","hr","heading","code","table","blockquote","list","checkbox","html","def","paragraph","text"].includes(e.type)){t+=n||"";continue}}let s=r;switch(s.type){case"space":t+=this.renderer.space(s);break;case"hr":t+=this.renderer.hr(s);break;case"heading":t+=this.renderer.heading(s);break;case"code":t+=this.renderer.code(s);break;case"table":t+=this.renderer.table(s);break;case"blockquote":t+=this.renderer.blockquote(s);break;case"list":t+=this.renderer.list(s);break;case"checkbox":t+=this.renderer.checkbox(s);break;case"html":t+=this.renderer.html(s);break;case"def":t+=this.renderer.def(s);break;case"paragraph":t+=this.renderer.paragraph(s);break;case"text":t+=this.renderer.text(s);break;default:{let e='Token with "'+s.type+'" type was not found.';if(this.options.silent)return console.error(e),"";throw new Error(e)}}}return t}parseInline(e,t=this.renderer){this.renderer.parser=this;let n="";for(let r=0;r<e.length;r++){let s=e[r];if(this.options.extensions?.renderers?.[s.type]){let e=this.options.extensions.renderers[s.type].call({parser:this},s);if(!1!==e||!["escape","html","link","image","checkbox","strong","em","codespan","br","del","text"].includes(s.type)){n+=e||"";continue}}let l=s;switch(l.type){case"escape":case"text":n+=t.text(l);break;case"html":n+=t.html(l);break;case"link":n+=t.link(l);break;case"image":n+=t.image(l);break;case"checkbox":n+=t.checkbox(l);break;case"strong":n+=t.strong(l);break;case"em":n+=t.em(l);break;case"codespan":n+=t.codespan(l);break;case"br":n+=t.br(l);break;case"del":n+=t.del(l);break;default:{let e='Token with "'+l.type+'" type was not found.';if(this.options.silent)return console.error(e),"";throw new Error(e)}}}return n}},De=class{options;block;constructor(e){this.options=e||p}static passThroughHooks=new Set(["preprocess","postprocess","processAllTokens","emStrongMask"]);static passThroughHooksRespectAsync=new Set(["preprocess","postprocess","processAllTokens"]);preprocess(e){return e}postprocess(e){return e}processAllTokens(e){return e}emStrongMask(e){return e}provideLexer(e=this.block){return e?Be.lex:Be.lexInline}provideParser(e=this.block){return e?ve.parse:ve.parseInline}},Oe=class{defaults={async:!1,breaks:!1,extensions:null,gfm:!0,hooks:null,pedantic:!1,renderer:null,silent:!1,tokenizer:null,walkTokens:null};options=this.setOptions;parse=this.parseMarkdown(!0);parseInline=this.parseMarkdown(!1);Parser=ve;Renderer=qe;TextRenderer=Ze;Lexer=Be;Tokenizer=Ce;Hooks=De;constructor(...e){this.use(...e)}walkTokens(e,t){let n=[];for(let r of e)switch(n=n.concat(t.call(this,r)),r.type){case"table":{let e=r;for(let r of e.header)n=n.concat(this.walkTokens(r.tokens,t));for(let r of e.rows)for(let e of r)n=n.concat(this.walkTokens(e.tokens,t));break}case"list":{let e=r;n=n.concat(this.walkTokens(e.items,t));break}default:{let e=r;this.defaults.extensions?.childTokens?.[e.type]?this.defaults.extensions.childTokens[e.type].forEach(r=>{let s=e[r].flat(1/0);n=n.concat(this.walkTokens(s,t))}):e.tokens&&(n=n.concat(this.walkTokens(e.tokens,t)))}}return n}use(...e){let t=this.defaults.extensions||{renderers:{},childTokens:{}};return e.forEach(e=>{let n={...e};if(n.async=this.defaults.async||n.async||!1,e.extensions&&(e.extensions.forEach(e=>{if(!e.name)throw new Error("extension name required");if("renderer"in e){let n=t.renderers[e.name];t.renderers[e.name]=n?function(...t){let r=e.renderer.apply(this,t);return!1===r&&(r=n.apply(this,t)),r}:e.renderer}if("tokenizer"in e){if(!e.level||"block"!==e.level&&"inline"!==e.level)throw new Error("extension level must be 'block' or 'inline'");let n=t[e.level];n?n.unshift(e.tokenizer):t[e.level]=[e.tokenizer],e.start&&("block"===e.level?t.startBlock?t.startBlock.push(e.start):t.startBlock=[e.start]:"inline"===e.level&&(t.startInline?t.startInline.push(e.start):t.startInline=[e.start]))}"childTokens"in e&&e.childTokens&&(t.childTokens[e.name]=e.childTokens)}),n.extensions=t),e.renderer){let t=this.defaults.renderer||new qe(this.defaults);for(let n in e.renderer){if(!(n in t))throw new Error(`renderer '${n}' does not exist`);if(["options","parser"].includes(n))continue;let r=n,s=e.renderer[r],l=t[r];t[r]=(...e)=>{let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n||""}}n.renderer=t}if(e.tokenizer){let t=this.defaults.tokenizer||new Ce(this.defaults);for(let n in e.tokenizer){if(!(n in t))throw new Error(`tokenizer '${n}' does not exist`);if(["options","rules","lexer"].includes(n))continue;let r=n,s=e.tokenizer[r],l=t[r];t[r]=(...e)=>{let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n}}n.tokenizer=t}if(e.hooks){let t=this.defaults.hooks||new De;for(let n in e.hooks){if(!(n in t))throw new Error(`hook '${n}' does not exist`);if(["options","block"].includes(n))continue;let r=n,s=e.hooks[r],l=t[r];De.passThroughHooks.has(n)?t[r]=e=>{if(this.defaults.async&&De.passThroughHooksRespectAsync.has(n))return(async()=>{let n=await s.call(t,e);return l.call(t,n)})();let r=s.call(t,e);return l.call(t,r)}:t[r]=(...e)=>{if(this.defaults.async)return(async()=>{let n=await s.apply(t,e);return!1===n&&(n=await l.apply(t,e)),n})();let n=s.apply(t,e);return!1===n&&(n=l.apply(t,e)),n}}n.hooks=t}if(e.walkTokens){let t=this.defaults.walkTokens,r=e.walkTokens;n.walkTokens=function(e){let n=[];return n.push(r.call(this,e)),t&&(n=n.concat(t.call(this,e))),n}}this.defaults={...this.defaults,...n}}),this}setOptions(e){return this.defaults={...this.defaults,...e},this}lexer(e,t){return Be.lex(e,t??this.defaults)}parser(e,t){return ve.parse(e,t??this.defaults)}parseMarkdown(e){return(t,n)=>{let r={...n},s={...this.defaults,...r},l=this.onError(!!s.silent,!!s.async);if(!0===this.defaults.async&&!1===r.async)return l(new Error("marked(): The async option was set to true by an extension. Remove async: false from the parse options object to return a Promise."));if(typeof t>"u"||null===t)return l(new Error("marked(): input parameter is undefined or null"));if("string"!=typeof t)return l(new Error("marked(): input parameter is of type "+Object.prototype.toString.call(t)+", string expected"));if(s.hooks&&(s.hooks.options=s,s.hooks.block=e),s.async)return(async()=>{let n=s.hooks?await s.hooks.preprocess(t):t,r=await(s.hooks?await s.hooks.provideLexer(e):e?Be.lex:Be.lexInline)(n,s),l=s.hooks?await s.hooks.processAllTokens(r):r;s.walkTokens&&await Promise.all(this.walkTokens(l,s.walkTokens));let i=await(s.hooks?await s.hooks.provideParser(e):e?ve.parse:ve.parseInline)(l,s);return s.hooks?await s.hooks.postprocess(i):i})().catch(l);try{s.hooks&&(t=s.hooks.preprocess(t));let n=(s.hooks?s.hooks.provideLexer(e):e?Be.lex:Be.lexInline)(t,s);s.hooks&&(n=s.hooks.processAllTokens(n)),s.walkTokens&&this.walkTokens(n,s.walkTokens);let r=(s.hooks?s.hooks.provideParser(e):e?ve.parse:ve.parseInline)(n,s);return s.hooks&&(r=s.hooks.postprocess(r)),r}catch(e){return l(e)}}}onError(e,t){return n=>{if(n.message+="\nPlease report this to https://github.com/markedjs/marked.",e){let e="<p>An error occurred:</p><pre>"+$e(n.message+"",!0)+"</pre>";return t?Promise.resolve(e):e}if(t)return Promise.reject(n);throw n}}},je=new Oe;function Qe(e,t){return je.parse(e,t)}function Me(...e){return je.use(...e),Qe.defaults=je.defaults,h(Qe.defaults),Qe}Qe.options=Qe.setOptions=function(e){return je.setOptions(e),Qe.defaults=je.defaults,h(Qe.defaults),Qe},Qe.getDefaults=c,Qe.defaults=p,Qe.use=Me,Qe.walkTokens=function(e,t){return je.walkTokens(e,t)},Qe.parseInline=je.parseInline,Qe.Parser=ve,Qe.parser=ve.parse,Qe.Renderer=qe,Qe.TextRenderer=Ze,Qe.Lexer=Be,Qe.lexer=Be.lex,Qe.Tokenizer=Ce,Qe.Hooks=De,Qe.parse=Qe;var Ne=Qe.options,He=Qe.setOptions,Ge=Qe.walkTokens,Xe=Qe.parseInline,Ue=Qe,Fe=ve.parse,We=Be.lex;return n!=t&&(r.exports=t),r.exports});
//# sourceMappingURL=/sm/d7299159e67c7c80ba99fc2715ddbcb78cc0161bbaf576d54ff7508d1ad287e0.map

/* === purify.min_3.4.16.js === */
/*! @license DOMPurify 3.4.16 | (c) Cure53 and other contributors | Released under the Apache license 2.0 and Mozilla Public License 2.0 | github.com/cure53/DOMPurify/blob/3.4.16/LICENSE */
(function(e,t){typeof exports==`object`&&typeof module<`u`?module.exports=t():typeof define==`function`&&define.amd?define([],t):(e=typeof globalThis<`u`?globalThis:e||self,e.DOMPurify=t())})(this,function(){"use strict";function e(e,t){this.v=e,this.k=t}function t(e,t){(t==null||t>e.length)&&(t=e.length);for(var n=0,r=Array(t);n<t;n++)r[n]=e[n];return r}function n(e){if(Array.isArray(e))return e}function r(e,t){var n=e==null?null:typeof Symbol<`u`&&e[Symbol.iterator]||e[`@@iterator`];if(n!=null){var r,i,a,o,s=[],c=!0,l=!1;try{if(a=(n=n.call(e)).next,t===0){if(Object(n)!==n)return;c=!1}else for(;!(c=(r=a.call(n)).done)&&(s.push(r.value),s.length!==t);c=!0);}catch(e){l=!0,i=e}finally{try{if(!c&&n.return!=null&&(o=n.return(),Object(o)!==o))return}finally{if(l)throw i}}return s}}function i(){throw TypeError(`Invalid attempt to destructure non-iterable instance.
In order to be iterable, non-array objects must have a [Symbol.iterator]() method.`)}
/*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/babel/babel/blob/main/packages/babel-helpers/LICENSE */
function a(e,t){return n(e)||r(e,t)||o(e,t)||i()}function o(e,n){if(e){if(typeof e==`string`)return t(e,n);var r={}.toString.call(e).slice(8,-1);return r===`Object`&&e.constructor&&(r=e.constructor.name),r===`Map`||r===`Set`?Array.from(e):r===`Arguments`||/^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(r)?t(e,n):void 0}}function s(t){var n,r;function i(n,r){try{var o=t[n](r),s=o.value,c=s instanceof e;Promise.resolve(c?s.v:s).then(function(e){if(c){var r=n===`return`&&s.k?n:`next`;if(!s.k||e.done)return i(r,e);e=t[r](e).value}a(!!o.done,e)},function(e){i(`throw`,e)})}catch(e){a(2,e)}}function a(e,t){e===2?n.reject(t):n.resolve({value:t,done:e}),(n=n.next)?i(n.key,n.arg):r=null}this._invoke=function(e,t){return new Promise(function(a,o){var s={key:e,arg:t,resolve:a,reject:o,next:null};r?r=r.next=s:(n=r=s,i(e,t))})},typeof t.return!=`function`&&(this.return=void 0)}s.prototype[typeof Symbol==`function`&&Symbol.asyncIterator||`@@asyncIterator`]=function(){return this},s.prototype.next=function(e){return this._invoke(`next`,e)},s.prototype.throw=function(e){return this._invoke(`throw`,e)},s.prototype.return=function(e){return this._invoke(`return`,e)};let c=Object.entries,l=Object.setPrototypeOf,u=Object.isFrozen,d=Object.getPrototypeOf,f=Object.getOwnPropertyDescriptor,p=Object.freeze,m=Object.seal,h=Object.create,ee=typeof Reflect<`u`&&Reflect,g=ee.apply,te=ee.construct;p||(p=function(e){return e}),m||(m=function(e){return e}),g||(g=function(e,t){var n=[...arguments].slice(2);return e.apply(t,n)}),te||(te=function(e){return new e(...[...arguments].slice(1))});let _=T(Array.prototype.forEach);Array.prototype.indexOf;let ne=T(Array.prototype.lastIndexOf),re=T(Array.prototype.pop),ie=T(Array.prototype.push);Array.prototype.slice;let ae=T(Array.prototype.splice),v=Array.isArray,oe=T(String.prototype.toLowerCase),se=T(String.prototype.toString),ce=T(String.prototype.match),le=T(String.prototype.replace),ue=T(String.prototype.indexOf),de=T(String.prototype.trim),fe=T(Number.prototype.toString),y=T(Boolean.prototype.toString),b=typeof BigInt>`u`?null:T(BigInt.prototype.toString),pe=typeof Symbol>`u`?null:T(Symbol.prototype.toString),x=T(Object.prototype.hasOwnProperty),S=T(Object.prototype.toString),C=T(RegExp.prototype.test),w=E(TypeError);function T(e){return function(t){t instanceof RegExp&&(t.lastIndex=0);var n=[...arguments].slice(1);return g(e,t,n)}}function E(e){return function(){return te(e,[...arguments])}}function D(e,t){let n=arguments.length>2&&arguments[2]!==void 0?arguments[2]:oe;if(l&&l(e,null),!v(t))return e;let r=t.length;for(;r--;){let i=t[r];if(typeof i==`string`){let e=n(i);e!==i&&(u(t)||(t[r]=e),i=e)}e[i]=!0}return e}function me(e){for(let t=0;t<e.length;t++)x(e,t)||(e[t]=null);return e}function O(e){let t=h(null);for(let r of c(e)){var n=a(r,2);let i=n[0],o=n[1];x(e,i)&&(t[i]=v(o)?me(o):o&&typeof o==`object`&&o.constructor===Object?O(o):o)}return t}function he(e){switch(typeof e){case`string`:return e;case`number`:return fe(e);case`boolean`:return y(e);case`bigint`:return b?b(e):`0`;case`symbol`:return pe?pe(e):`Symbol()`;case`undefined`:return S(e);case`function`:case`object`:{if(e===null)return S(e);let t=e,n=k(t,`toString`);if(typeof n==`function`){let e=n(t);return typeof e==`string`?e:S(e)}return S(e)}default:return S(e)}}function k(e,t){for(;e!==null;){let n=f(e,t);if(n){if(n.get)return T(n.get);if(typeof n.value==`function`)return T(n.value)}e=d(e)}function n(){return null}return n}function ge(e){try{return C(e,``),!0}catch(e){return!1}}let _e=p(/* @__PURE__ */ `a.abbr.acronym.address.area.article.aside.audio.b.bdi.bdo.big.blink.blockquote.body.br.button.canvas.caption.center.cite.code.col.colgroup.content.data.datalist.dd.decorator.del.details.dfn.dialog.dir.div.dl.dt.element.em.fieldset.figcaption.figure.font.footer.form.h1.h2.h3.h4.h5.h6.head.header.hgroup.hr.html.i.img.input.ins.kbd.label.legend.li.main.map.mark.marquee.menu.menuitem.meter.nav.nobr.ol.optgroup.option.output.p.picture.pre.progress.q.rp.rt.ruby.s.samp.search.section.select.shadow.slot.small.source.spacer.span.strike.strong.style.sub.summary.sup.table.tbody.td.template.textarea.tfoot.th.thead.time.tr.track.tt.u.ul.var.video.wbr`.split(`.`)),ve=p(/* @__PURE__ */ `svg.a.altglyph.altglyphdef.altglyphitem.animatecolor.animatemotion.animatetransform.circle.clippath.defs.desc.ellipse.enterkeyhint.exportparts.filter.font.g.glyph.glyphref.hkern.image.inputmode.line.lineargradient.marker.mask.metadata.mpath.part.path.pattern.polygon.polyline.radialgradient.rect.stop.style.switch.symbol.text.textpath.title.tref.tspan.view.vkern`.split(`.`)),ye=p([`feBlend`,`feColorMatrix`,`feComponentTransfer`,`feComposite`,`feConvolveMatrix`,`feDiffuseLighting`,`feDisplacementMap`,`feDistantLight`,`feDropShadow`,`feFlood`,`feFuncA`,`feFuncB`,`feFuncG`,`feFuncR`,`feGaussianBlur`,`feImage`,`feMerge`,`feMergeNode`,`feMorphology`,`feOffset`,`fePointLight`,`feSpecularLighting`,`feSpotLight`,`feTile`,`feTurbulence`]),be=p([`animate`,`color-profile`,`cursor`,`discard`,`font-face`,`font-face-format`,`font-face-name`,`font-face-src`,`font-face-uri`,`foreignobject`,`hatch`,`hatchpath`,`mesh`,`meshgradient`,`meshpatch`,`meshrow`,`missing-glyph`,`script`,`set`,`solidcolor`,`unknown`,`use`]),xe=p(/* @__PURE__ */ `math.menclose.merror.mfenced.mfrac.mglyph.mi.mlabeledtr.mmultiscripts.mn.mo.mover.mpadded.mphantom.mroot.mrow.ms.mspace.msqrt.mstyle.msub.msup.msubsup.mtable.mtd.mtext.mtr.munder.munderover.mprescripts`.split(`.`)),Se=p([`maction`,`maligngroup`,`malignmark`,`mlongdiv`,`mscarries`,`mscarry`,`msgroup`,`mstack`,`msline`,`msrow`,`semantics`,`annotation`,`annotation-xml`,`mprescripts`,`none`]),Ce=p([`#text`]),we=p(/* @__PURE__ */ `accept.action.align.alt.autocapitalize.autocomplete.autopictureinpicture.autoplay.background.bgcolor.border.capture.cellpadding.cellspacing.checked.cite.class.clear.color.cols.colspan.command.commandfor.controls.controlslist.coords.crossorigin.datetime.decoding.default.dir.disabled.disablepictureinpicture.disableremoteplayback.download.draggable.enctype.enterkeyhint.exportparts.face.for.headers.height.hidden.high.href.hreflang.id.inert.inputmode.integrity.ismap.kind.label.lang.list.loading.loop.low.max.maxlength.media.method.min.minlength.multiple.muted.name.nonce.noshade.novalidate.nowrap.open.optimum.part.pattern.placeholder.playsinline.popover.popovertarget.popovertargetaction.poster.preload.pubdate.radiogroup.readonly.rel.required.rev.reversed.role.rows.rowspan.spellcheck.scope.selected.shape.size.sizes.slot.span.srclang.start.src.srcset.step.style.summary.tabindex.title.translate.type.usemap.valign.value.width.wrap.xmlns`.split(`.`)),Te=p(/* @__PURE__ */ `accent-height.accumulate.additive.alignment-baseline.amplitude.ascent.attributename.attributetype.azimuth.basefrequency.baseline-shift.begin.bias.by.class.clip.clippathunits.clip-path.clip-rule.color.color-interpolation.color-interpolation-filters.color-profile.color-rendering.cx.cy.d.dx.dy.diffuseconstant.direction.display.divisor.dominant-baseline.dur.edgemode.elevation.end.exponent.fill.fill-opacity.fill-rule.filter.filterunits.flood-color.flood-opacity.font-family.font-size.font-size-adjust.font-stretch.font-style.font-variant.font-weight.fx.fy.g1.g2.glyph-name.glyphref.gradientunits.gradienttransform.height.href.id.image-rendering.in.in2.intercept.k.k1.k2.k3.k4.kerning.keypoints.keysplines.keytimes.lang.lengthadjust.letter-spacing.kernelmatrix.kernelunitlength.lighting-color.local.marker-end.marker-mid.marker-start.markerheight.markerunits.markerwidth.maskcontentunits.maskunits.max.mask.mask-type.media.method.mode.min.name.numoctaves.offset.operator.opacity.order.orient.orientation.origin.overflow.paint-order.path.pathlength.patterncontentunits.patterntransform.patternunits.pointer-events.points.preservealpha.preserveaspectratio.primitiveunits.r.rx.ry.radius.refx.refy.repeatcount.repeatdur.restart.result.rotate.scale.seed.shape-rendering.slope.specularconstant.specularexponent.spreadmethod.startoffset.stddeviation.stitchtiles.stop-color.stop-opacity.stroke-dasharray.stroke-dashoffset.stroke-linecap.stroke-linejoin.stroke-miterlimit.stroke-opacity.stroke.stroke-width.style.surfacescale.systemlanguage.tabindex.tablevalues.targetx.targety.transform.transform-origin.text-anchor.text-decoration.text-orientation.text-rendering.textlength.type.u1.u2.unicode.values.vector-effect.viewbox.visibility.version.vert-adv-y.vert-origin-x.vert-origin-y.width.word-spacing.wrap.writing-mode.xchannelselector.ychannelselector.x.x1.x2.xmlns.y.y1.y2.z.zoomandpan`.split(`.`)),Ee=p(/* @__PURE__ */ `accent.accentunder.align.bevelled.close.columnalign.columnlines.columnspacing.columnspan.denomalign.depth.dir.display.displaystyle.encoding.fence.frame.height.href.id.largeop.length.linethickness.lquote.lspace.mathbackground.mathcolor.mathsize.mathvariant.maxsize.minsize.movablelimits.notation.numalign.open.rowalign.rowlines.rowspacing.rowspan.rspace.rquote.scriptlevel.scriptminsize.scriptsizemultiplier.selection.separator.separators.stretchy.subscriptshift.supscriptshift.symmetric.voffset.width.xmlns`.split(`.`)),De=p([`xlink:href`,`xml:id`,`xlink:title`,`xml:space`,`xmlns:xlink`]),Oe=m(/{{[\w\W]*|^[\w\W]*}}/g),ke=m(/<%[\w\W]*|^[\w\W]*%>/g),Ae=m(/\${[\w\W]*/g),je=m(/^data-[\-\w.\u00B7-\uFFFF]+$/),Me=m(/^aria-[\-\w]+$/),Ne=m(/^(?:(?:(?:f|ht)tps?|mailto|tel|callto|sms|cid|xmpp|matrix):|[^a-z]|[a-z+.\-]+(?:[^a-z+.\-:]|$))/i),Pe=m(/^(?:\w+script|data):/i),Fe=m(/[\u0000-\u0020\u00A0\u1680\u180E\u2000-\u2029\u205F\u3000]/g),Ie=m(/^html$/i),Le=m(/^[a-z][.\w]*(-[.\w]+)+$/i),Re=m(/<[/\w!]/g),ze=m(/<[/\w]/g),Be=m(/<\/no(script|embed|frames)/i),Ve=m(/\/>/i),A={element:1,attribute:2,text:3,cdataSection:4,entityReference:5,entityNode:6,processingInstruction:7,comment:8,document:9,documentType:10,documentFragment:11,notation:12},j=[`style`,`script`,`xmp`,`iframe`,`noembed`,`noframes`,`plaintext`,`noscript`],He=p(D({},j)),Ue=function(){let e={};return _(j,t=>{e[t]=m(RegExp(`</`+t+`(?=[\\t\\n\\f\\r />])`,`i`))}),p(e)}(),We=function(){return typeof window>`u`?null:window},Ge=function(e,t){if(typeof e!=`object`||typeof e.createPolicy!=`function`)return null;let n=null,r=`data-tt-policy-suffix`;t&&t.hasAttribute(r)&&(n=t.getAttribute(r));let i=`dompurify`+(n?`#`+n:``);try{return e.createPolicy(i,{createHTML(e){return e},createScriptURL(e){return e}})}catch(e){return console.warn(`TrustedTypes policy `+i+` could not be created.`),null}},Ke=function(){return{afterSanitizeAttributes:[],afterSanitizeElements:[],afterSanitizeShadowDOM:[],beforeSanitizeAttributes:[],beforeSanitizeElements:[],beforeSanitizeShadowDOM:[],uponSanitizeAttribute:[],uponSanitizeElement:[],uponSanitizeShadowNode:[]}},M=function(e,t,n,r){return x(e,t)&&v(e[t])?D(r.base?O(r.base):{},e[t],r.transform):n},qe=function(e,t,n){let r=x(e,t)?e[t]:void 0;return r&&typeof r==`object`?O(r):n()};function Je(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:We(),t=e=>Je(e);if(t.version=`3.4.16`,t.removed=[],!e||!e.document||e.document.nodeType!==A.document||!e.Element)return t.isSupported=!1,t;let n=e.document,r=n,i=r.currentScript;e.DocumentFragment;let a=e.HTMLTemplateElement,o=e.Node,s=e.Element,l=e.NodeFilter;e.NamedNodeMap===void 0&&(e.NamedNodeMap||e.MozNamedAttrMap),e.HTMLFormElement;let u=e.DOMParser,d=e.trustedTypes,f=s.prototype,ee=k(f,`cloneNode`),g=k(f,`remove`),te=k(f,`removeAttributeNode`),fe=k(f,`nextSibling`),y=k(f,`childNodes`),b=k(f,`parentNode`),pe=k(f,`shadowRoot`),S=k(f,`attributes`),T=o&&o.prototype?k(o.prototype,`nodeType`):null,E=o&&o.prototype?k(o.prototype,`nodeName`):null,me=o&&o.prototype?k(o.prototype,`ownerDocument`):null,j=function(e){return T?T(e):e.nodeType},Ye=function(e){return E?E(e):e.nodeName};if(typeof a==`function`){let e=n.createElement(`template`);e.content&&e.content.ownerDocument&&(n=e.content.ownerDocument)}let N,P=``,Xe,Ze=!1,Qe=0,$e=function(){if(Qe>0)throw w(`A configured TRUSTED_TYPES_POLICY callback (createHTML or createScriptURL) must not call DOMPurify.sanitize, as that causes infinite recursion. Do not pass a policy whose callbacks wrap DOMPurify as TRUSTED_TYPES_POLICY; see the "DOMPurify and Trusted Types" section of the README.`)},F=function(e){$e(),Qe++;try{return N.createHTML(e)}finally{Qe--}},et=function(e){$e(),Qe++;try{return N.createScriptURL(e)}finally{Qe--}},tt=function(){return Ze||(Xe=Ge(d,i),Ze=!0),Xe},nt=n,rt=nt.implementation,it=nt.createNodeIterator,at=nt.createDocumentFragment,ot=nt.getElementsByTagName,st=r.importNode,I=Ke();t.isSupported=typeof c==`function`&&typeof b==`function`&&rt&&rt.createHTMLDocument!==void 0;let ct=Oe,lt=ke,ut=Ae,dt=je,ft=Me,pt=Pe,mt=Fe,ht=Le,gt=Ne,L=null,_t=D({},[..._e,...ve,...ye,...xe,...Ce]),R=null,vt=D({},[...we,...Te,...Ee,...De]),z=Object.seal(h(null,{tagNameCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},attributeNameCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},allowCustomizedBuiltInElements:{writable:!0,configurable:!1,enumerable:!0,value:!1}})),yt=null,bt=null,B=Object.seal(h(null,{tagCheck:{writable:!0,configurable:!1,enumerable:!0,value:null},attributeCheck:{writable:!0,configurable:!1,enumerable:!0,value:null}})),xt=!0,St=!0,Ct=!1,wt=!0,V=!1,H=!0,U=!1,Tt=!1,Et=null,Dt=null,Ot=!1,W=!1,kt=!1,At=!1,jt=!0,Mt=!1,Nt=`user-content-`,Pt=!0,Ft=!1,G={},K=null,It=D({},/* @__PURE__ */ `annotation-xml.audio.colgroup.desc.foreignobject.head.iframe.math.mi.mn.mo.ms.mtext.noembed.noframes.noscript.plaintext.script.selectedcontent.style.svg.template.thead.title.video.xmp`.split(`.`)),Lt=null,Rt=D({},[`audio`,`video`,`img`,`source`,`image`,`track`]),zt=null,Bt=D({},[`alt`,`class`,`for`,`id`,`label`,`name`,`pattern`,`placeholder`,`role`,`summary`,`title`,`value`,`style`,`xmlns`]),Vt=`\u0068\u0074\u0074\u0070://www.w3.org/1998/Math/MathML`,Ht=`\u0068\u0074\u0074\u0070://www.w3.org/2000/svg`,q=`\u0068\u0074\u0074\u0070://www.w3.org/1999/xhtml`,J=q,Ut=!1,Wt=null,Gt=D({},[Vt,Ht,q],se),Kt=p([`mi`,`mo`,`mn`,`ms`,`mtext`]),qt=D({},Kt),Jt=p([`annotation-xml`]),Yt=D({},Jt),Xt=D({},[`title`,`style`,`font`,`a`,`script`]),Zt=null,Qt=[`application/xhtml+xml`,`text/html`],Y=null,$t=null,en=n.createElement(`form`),tn=function(e){return e instanceof RegExp||e instanceof Function},nn=function(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:{};if($t&&$t===e)return;(!e||typeof e!=`object`)&&(e={}),e=O(e),Zt=Qt.indexOf(e.PARSER_MEDIA_TYPE)===-1?`text/html`:e.PARSER_MEDIA_TYPE,Y=Zt===`application/xhtml+xml`?se:oe,L=M(e,`ALLOWED_TAGS`,_t,{transform:Y}),R=M(e,`ALLOWED_ATTR`,vt,{transform:Y}),Wt=M(e,`ALLOWED_NAMESPACES`,Gt,{transform:se}),zt=M(e,`ADD_URI_SAFE_ATTR`,Bt,{transform:Y,base:Bt}),Lt=M(e,`ADD_DATA_URI_TAGS`,Rt,{transform:Y,base:Rt}),K=M(e,`FORBID_CONTENTS`,It,{transform:Y}),yt=M(e,`FORBID_TAGS`,O({}),{transform:Y}),bt=M(e,`FORBID_ATTR`,O({}),{transform:Y}),G=x(e,`USE_PROFILES`)?e.USE_PROFILES&&typeof e.USE_PROFILES==`object`?O(e.USE_PROFILES):e.USE_PROFILES:!1,xt=e.ALLOW_ARIA_ATTR!==!1,St=e.ALLOW_DATA_ATTR!==!1,Ct=e.ALLOW_UNKNOWN_PROTOCOLS||!1,wt=e.ALLOW_SELF_CLOSE_IN_ATTR!==!1,V=e.SAFE_FOR_TEMPLATES||!1,H=e.SAFE_FOR_XML!==!1,U=e.WHOLE_DOCUMENT||!1,W=e.RETURN_DOM||!1,kt=e.RETURN_DOM_FRAGMENT||!1,At=e.RETURN_TRUSTED_TYPE||!1,Ot=e.FORCE_BODY||!1,jt=e.SANITIZE_DOM!==!1,Mt=e.SANITIZE_NAMED_PROPS||!1,Pt=e.KEEP_CONTENT!==!1,Ft=e.IN_PLACE||!1,gt=ge(e.ALLOWED_URI_REGEXP)?e.ALLOWED_URI_REGEXP:Ne,J=typeof e.NAMESPACE==`string`?e.NAMESPACE:q,qt=qe(e,`MATHML_TEXT_INTEGRATION_POINTS`,()=>D({},Kt)),Yt=qe(e,`HTML_INTEGRATION_POINTS`,()=>D({},Jt));let t=qe(e,`CUSTOM_ELEMENT_HANDLING`,()=>h(null));if(z=h(null),x(t,`tagNameCheck`)&&tn(t.tagNameCheck)&&(z.tagNameCheck=t.tagNameCheck),x(t,`attributeNameCheck`)&&tn(t.attributeNameCheck)&&(z.attributeNameCheck=t.attributeNameCheck),x(t,`allowCustomizedBuiltInElements`)&&typeof t.allowCustomizedBuiltInElements==`boolean`&&(z.allowCustomizedBuiltInElements=t.allowCustomizedBuiltInElements),m(z),V&&(St=!1),kt&&(W=!0),G&&(L=D({},Ce),R=h(null),G.html===!0&&(D(L,_e),D(R,we)),G.svg===!0&&(D(L,ve),D(R,Te),D(R,De)),G.svgFilters===!0&&(D(L,ye),D(R,Te),D(R,De)),G.mathMl===!0&&(D(L,xe),D(R,Ee),D(R,De))),B.tagCheck=null,B.attributeCheck=null,x(e,`ADD_TAGS`)&&(typeof e.ADD_TAGS==`function`?B.tagCheck=e.ADD_TAGS:v(e.ADD_TAGS)&&(L===_t&&(L=O(L)),D(L,e.ADD_TAGS,Y))),x(e,`ADD_ATTR`)&&(typeof e.ADD_ATTR==`function`?B.attributeCheck=e.ADD_ATTR:v(e.ADD_ATTR)&&(R===vt&&(R=O(R)),D(R,e.ADD_ATTR,Y))),x(e,`ADD_FORBID_CONTENTS`)&&v(e.ADD_FORBID_CONTENTS)&&(K===It&&(K=O(K)),D(K,e.ADD_FORBID_CONTENTS,Y)),Pt&&(L[`#text`]=!0),U&&D(L,[`html`,`head`,`body`]),L.table&&(D(L,[`tbody`]),delete yt.tbody),e.TRUSTED_TYPES_POLICY){if(typeof e.TRUSTED_TYPES_POLICY.createHTML!=`function`)throw w(`TRUSTED_TYPES_POLICY configuration option must provide a "createHTML" hook.`);if(typeof e.TRUSTED_TYPES_POLICY.createScriptURL!=`function`)throw w(`TRUSTED_TYPES_POLICY configuration option must provide a "createScriptURL" hook.`);let t=N;N=e.TRUSTED_TYPES_POLICY;try{P=F(``)}catch(e){throw N=t,e}}else e.TRUSTED_TYPES_POLICY===null?(N=void 0,P=``):(N===void 0&&(N=tt()),N&&typeof P==`string`&&(P=F(``)));p&&p(e),$t=e},rn=D({},[...ve,...ye,...be]),an=D({},[...xe,...Se]),on=function(e,t,n){return t.namespaceURI===q?e===`svg`:t.namespaceURI===Vt?e===`svg`&&(n===`annotation-xml`||qt[n]):!!rn[e]},sn=function(e,t,n){return t.namespaceURI===q?e===`math`:t.namespaceURI===Ht?e===`math`&&Yt[n]:!!an[e]},cn=function(e,t,n){return t.namespaceURI===Ht&&!Yt[n]||t.namespaceURI===Vt&&!qt[n]?!1:!an[e]&&(Xt[e]||!rn[e])},ln=function(e){let t=b(e);(!t||!t.tagName)&&(t={namespaceURI:J,tagName:`template`});let n=oe(e.tagName),r=oe(t.tagName);return Wt[e.namespaceURI]?e.namespaceURI===Ht?on(n,t,r):e.namespaceURI===Vt?sn(n,t,r):e.namespaceURI===q?cn(n,t,r):!!(Zt===`application/xhtml+xml`&&Wt[e.namespaceURI]):!1},X=function(e){ie(t.removed,{element:e});try{b(e).removeChild(e)}catch(t){if(g(e),!b(e))throw w(`a node selected for removal could not be detached from its tree and cannot be safely returned; refusing to sanitize in place`)}},un=function(e,t,n){try{te(e,t)}catch(t){try{e.removeAttribute(n)}catch(e){}}},dn=function(e){pn(e);let t=y(e);if(t){let e=[];_(t,t=>{ie(e,t)}),_(e,e=>{try{g(e)}catch(e){}})}let n=S(e);if(n)for(let t=n.length-1;t>=0;--t){let r=n[t],i=r&&r.name;typeof i==`string`&&un(e,r,i)}},Z=function(e,n,r){if(!r)try{r=n.getAttributeNode(e)}catch(e){r=null}ie(t.removed,{attribute:r||null,from:n});try{r?te(n,r):n.removeAttribute(e)}catch(t){try{n.removeAttribute(e)}catch(e){}}if(e===`is`){if(W||kt)try{X(n)}catch(e){}else try{n.setAttribute(e,``)}catch(e){}}},fn=function(e){let t=S(e);if(t)for(let n=t.length-1;n>=0;--n){let r=t[n],i=r&&r.name;typeof i!=`string`||R[Y(i)]||un(e,r,i)}},pn=function(e){let t=[e];for(;t.length>0;){let e=t.pop();j(e)===A.element&&fn(e);let n=y(e);if(n)for(let e=n.length-1;e>=0;--e)t.push(n[e])}},mn=function(e,t){return H?e===`patchsrc`||e===`for`&&t!==`label`&&t!==`output`:!1},hn=function(e){if(!H)return;let t=[e];for(;t.length>0;){let e=t.pop(),n=j(e);if(n===A.processingInstruction||n===A.comment&&C(ze,e.data)){try{g(e)}catch(e){}continue}if(n===A.element){let t=e,n=Y(Ye(e));try{t.hasAttribute&&t.hasAttribute(`patchsrc`)&&t.removeAttribute(`patchsrc`),t.hasAttribute&&t.hasAttribute(`for`)&&mn(`for`,n)&&t.removeAttribute(`for`)}catch(e){}}let r=y(e);if(r)for(let e=r.length-1;e>=0;--e)t.push(r[e])}},gn=function(e){let t=null,r=null;if(Ot)e=`<remove></remove>`+e;else{let t=ce(e,/^[\r\n\t ]+/);r=t&&t[0]}Zt===`application/xhtml+xml`&&J===q&&(e=`<html xmlns="\u0068\u0074\u0074\u0070://www.w3.org/1999/xhtml"><head></head><body>`+e+`</body></html>`);let i=N?F(e):e;if(J===q)try{t=new u().parseFromString(i,Zt)}catch(e){}if(!t||!t.documentElement){t=rt.createDocument(J,`template`,null);try{t.documentElement.innerHTML=Ut?P:i}catch(e){}}let a=t.body||t.documentElement;return e&&r&&a.insertBefore(n.createTextNode(r),a.childNodes[0]||null),J===q?ot.call(t,U?`html`:`body`)[0]:U?t.documentElement:a},_n=function(e){let t=me?me(e):e.ownerDocument;return it.call(t||e,e,l.SHOW_ELEMENT|l.SHOW_COMMENT|l.SHOW_TEXT|l.SHOW_PROCESSING_INSTRUCTION|l.SHOW_CDATA_SECTION,null)},vn=function(e){return e=le(e,ct,` `),e=le(e,lt,` `),e=le(e,ut,` `),e},yn=function(e){var t;e.normalize();let n=me?me(e):e.ownerDocument,r=it.call(n||e,e,l.SHOW_TEXT|l.SHOW_COMMENT|l.SHOW_CDATA_SECTION|l.SHOW_PROCESSING_INSTRUCTION,null),i=r.nextNode();for(;i;)i.data=vn(i.data),i=r.nextNode();let a=(t=e.querySelectorAll)==null?void 0:t.call(e,`template`);a&&_(a,e=>{xn(e.content)&&yn(e.content)})},bn=function(e){let t=E?E(e):null;return typeof t!=`string`||Y(t)!==`form`?!1:typeof e.nodeName!=`string`||typeof e.textContent!=`string`||typeof e.removeChild!=`function`||e.attributes!==S(e)||typeof e.removeAttribute!=`function`||typeof e.removeAttributeNode!=`function`||typeof e.getAttributeNode!=`function`||typeof e.setAttribute!=`function`||typeof e.namespaceURI!=`string`||typeof e.insertBefore!=`function`||typeof e.hasChildNodes!=`function`||e.nodeType!==T(e)||e.childNodes!==y(e)},xn=function(e){if(!T||typeof e!=`object`||!e)return!1;try{return T(e)===A.documentFragment}catch(e){return!1}},Sn=function(e){if(!T||typeof e!=`object`||!e)return!1;try{return typeof T(e)==`number`}catch(e){return!1}};function Q(e,n,r){e.length!==0&&_(e,e=>{e.call(t,n,r,$t)})}let Cn=function(e,t){return!!(H&&e.hasChildNodes()&&!Sn(e.firstElementChild)&&C(Re,e.textContent)&&C(Re,e.innerHTML)||H&&e.namespaceURI===q&&He[t]&&(Sn(e.firstElementChild)||typeof e.textContent==`string`&&C(Ue[t],e.textContent))||e.nodeType===A.processingInstruction||H&&e.nodeType===A.comment&&C(ze,e.data))},wn=function(e,t){return e instanceof RegExp?C(e,t):e instanceof Function&&!!e(t,...[...arguments].slice(2))},Tn=function(e,t,n){if(!yt[t]&&An(t)&&wn(z.tagNameCheck,t))return!1;if(Pt&&!K[t]){let t=b(e),r=y(e);if(r&&t){let i=r.length;for(let a=i-1;a>=0;--a){let i=e===n?ee(r[a],!0):r[a];t.insertBefore(i,fe(e))}}}return X(e),!0},En=function(e,t,n,r){return e.length===0?t:t===n||t===r?O(t):t},$=function(e,t){return e===t||b(e)!==null?!1:(Ft&&pn(e),!0)},Dn=function(e,n){if(Q(I.beforeSanitizeElements,e,null),$(e,n))return!0;if(bn(e))return X(e),!0;let r=Y(Ye(e));if(L=En(I.uponSanitizeElement,L,_t,Et),Q(I.uponSanitizeElement,e,{tagName:r,allowedTags:L}),$(e,n))return!0;if(Cn(e,r))return X(e),!0;if(yt[r]||!(B.tagCheck instanceof Function&&B.tagCheck(r))&&!L[r]){let t=Tn(e,r,n);return t===!1&&(Q(I.afterSanitizeElements,e,null),$(e,n))?!0:t}if(j(e)===A.element&&!ln(e)||(r===`noscript`||r===`noembed`||r===`noframes`)&&C(Be,e.innerHTML))return X(e),!0;if(V&&e.nodeType===A.text){let n=vn(e.textContent);e.textContent!==n&&(ie(t.removed,{element:e.cloneNode()}),e.textContent=n)}return Q(I.afterSanitizeElements,e,null),$(e,n)},On=function(e,t,r){if(bt[t]||mn(t,e)||jt&&(t===`id`||t===`name`)&&(r in n||r in en))return!1;let i=R[t]||B.attributeCheck instanceof Function&&B.attributeCheck(t,e);return St&&C(dt,t)||xt&&C(ft,t)?!0:i?zt[t]||C(gt,le(r,mt,``))||(t===`src`||t===`xlink:href`||t===`href`)&&e!==`script`&&ue(r,`data:`)===0&&Lt[e]||Ct&&!C(pt,le(r,mt,``))?!0:!r:An(e)&&wn(z.tagNameCheck,e)&&wn(z.attributeNameCheck,t,e)||t===`is`&&z.allowCustomizedBuiltInElements&&wn(z.tagNameCheck,r)},kn=D({},[`annotation-xml`,`color-profile`,`font-face`,`font-face-format`,`font-face-name`,`font-face-src`,`font-face-uri`,`missing-glyph`]),An=function(e){return!kn[oe(e)]&&C(ht,e)},jn=function(e,t,n,r){if(N&&typeof d==`object`&&typeof d.getAttributeType==`function`&&!n)switch(d.getAttributeType(e,t)){case`TrustedHTML`:return F(r);case`TrustedScriptURL`:return et(r)}return r},Mn=function(e,t,n,r){try{return n?e.setAttributeNS(n,t,r):e.setAttribute(t,r),!bn(e)||(X(e),!1)}catch(n){return Z(t,e),!1}},Nn=function(e,n){if(Q(I.beforeSanitizeAttributes,e,null),$(e,n))return;let r=e.attributes;if(!r||bn(e))return;R=En(I.uponSanitizeAttribute,R,vt,Dt);let i={attrName:``,attrValue:``,keepAttr:!0,allowedAttributes:R,forceKeepAttr:void 0},a=r.length,o=Y(e.nodeName);for(;a--;){let n=r[a],s=n.name,c=n.namespaceURI,l=n.value,u=Y(s),d=l,f=s===`value`?d:de(d),p=!1;if(i.attrName=u,i.attrValue=f,i.keepAttr=!0,i.forceKeepAttr=void 0,Q(I.uponSanitizeAttribute,e,i),f=i.attrValue,Mt&&(u===`id`||u===`name`)&&ue(f,Nt)!==0&&(Z(s,e,n),f=Nt+f,p=!0),H&&C(/((--!?|])>)|<\/(style|script|title|xmp|textarea|noscript|iframe|noembed|noframes)/i,f)){Z(s,e,n);continue}if(u===`attributename`&&ce(f,`href`)){Z(s,e,n);continue}if(!i.forceKeepAttr){if(!i.keepAttr){Z(s,e,n);continue}if(!wt&&C(Ve,f)){Z(s,e,n);continue}if(V&&(f=vn(f)),!On(o,u,f)){Z(s,e,n);continue}f=jn(o,u,c,f),f!==d&&Mn(e,s,c,f)&&p&&re(t.removed)}}Q(I.afterSanitizeAttributes,e,null),$(e,n)},Pn=function(e){let t=null,n=_n(e);for(Q(I.beforeSanitizeShadowDOM,e,null);t=n.nextNode();)if(Q(I.uponSanitizeShadowNode,t,null),Dn(t,e),Nn(t,e),xn(t.content)&&Pn(t.content),j(t)===A.element){let e=pe(t);xn(e)&&(Fn(e),Pn(e))}Q(I.afterSanitizeShadowDOM,e,null)},Fn=function(e){let t=[{node:e,shadow:null}];for(;t.length>0;){let e=t.pop();if(e.shadow){Pn(e.shadow);continue}let n=e.node,r=j(n)===A.element,i=y(n);if(i)for(let e=i.length-1;e>=0;--e)t.push({node:i[e],shadow:null});if(r){let e=E?E(n):null;if(typeof e==`string`&&Y(e)===`template`){let e=n.content;xn(e)&&t.push({node:e,shadow:null})}}if(r){let e=pe(n);xn(e)&&t.push({node:null,shadow:e},{node:e,shadow:null})}}};return t.sanitize=function(e){let n=arguments.length>1&&arguments[1]!==void 0?arguments[1]:{},i=null,a=null,o=null,s=null;if(Ut=!e,Ut&&(e=`<!-->`),typeof e!=`string`&&!Sn(e)&&(e=he(e),typeof e!=`string`))throw w(`dirty is not a string, aborting`);if(!t.isSupported)return e;Tt?(L=Et,R=Dt):nn(n),(I.uponSanitizeElement.length>0||I.uponSanitizeAttribute.length>0)&&(L=O(L)),I.uponSanitizeAttribute.length>0&&(R=O(R)),t.removed=[];let c=Ft&&typeof e!=`string`&&Sn(e);if(c){hn(e);let t=Ye(e);if(typeof t==`string`){let n=Y(t);if(!L[n]||yt[n])throw dn(e),w(`root node is forbidden and cannot be sanitized in-place`)}if(bn(e))throw dn(e),w(`root node is clobbered and cannot be sanitized in-place`);try{Fn(e)}catch(t){throw dn(e),t}}else if(Sn(e))i=gn(`<!---->`),a=i.ownerDocument.importNode(e,!0),a.nodeType===A.element&&a.nodeName===`BODY`||a.nodeName===`HTML`?i=a:i.appendChild(a),Fn(i);else{if(!W&&!V&&!U&&e.indexOf(`<`)===-1)return N&&At?F(e):e;if(i=gn(e),!i)return W?null:At?P:``}i&&Ot&&X(i.firstChild);let l=c?e:i;try{let e=_n(l);for(;o=e.nextNode();)Dn(o,l),Nn(o,l),xn(o.content)&&Pn(o.content)}catch(n){throw c&&(dn(e),_(t.removed,e=>{e.element&&pn(e.element)})),n}if(c){let n=!1;if(_(t.removed,t=>{t.element&&(t.element===e&&(n=!0),pn(t.element))}),n)throw w(`a node selected for removal could not be safely returned; refusing to sanitize in place`);return V&&yn(e),e}if(W){if(V&&yn(i),kt)for(s=at.call(i.ownerDocument);i.firstChild;)s.appendChild(i.firstChild);else s=i;return(R.shadowroot||R.shadowrootmode)&&(s=st.call(r,s,!0)),s}let u=U?i.outerHTML:i.innerHTML;return U&&L[`!doctype`]&&i.ownerDocument&&i.ownerDocument.doctype&&i.ownerDocument.doctype.name&&C(Ie,i.ownerDocument.doctype.name)&&(u=`<!DOCTYPE `+i.ownerDocument.doctype.name+`>
`+u),V&&(u=vn(u)),N&&At?F(u):u},t.setConfig=function(){let e=arguments.length>0&&arguments[0]!==void 0?arguments[0]:{};nn(e),Tt=!0,Et=L,Dt=R},t.clearConfig=function(){$t=null,Tt=!1,Et=null,Dt=null,N=Xe,P=``},t.isValidAttribute=function(e,t,n){$t||nn({});let r=Y(e),i=Y(t);return On(r,i,n)},t.addHook=function(e,t){typeof t==`function`&&x(I,e)&&ie(I[e],t)},t.removeHook=function(e,t){if(x(I,e)){if(t!==void 0){let n=ne(I[e],t);return n===-1?void 0:ae(I[e],n,1)[0]}return re(I[e])}},t.removeHooks=function(e){x(I,e)&&(I[e]=[])},t.removeAllHooks=function(){I=Ke()},t}return Je()});
//# sourceMappingURL=purify.min.js.map

/* === highlight.min_11.12.0.js === */
/*!
  Highlight.js v11.12.0 (git: f7f7d3803b)
  (c) 2006-2026 Josh Goebel <hello@joshgoebel.com> and other contributors
  License: BSD-3-Clause
 */
var hljs=function(){"use strict";function e(n){
return n instanceof Map?n.clear=n.delete=n.set=()=>{
throw Error("map is read-only")}:n instanceof Set&&(n.add=n.clear=n.delete=()=>{
throw Error("set is read-only")
}),Object.freeze(n),Object.getOwnPropertyNames(n).forEach(t=>{
const a=n[t],i=typeof a;"object"!==i&&"function"!==i||Object.isFrozen(a)||e(a)
}),n}class n{constructor(e){
void 0===e.data&&(e.data={}),this.data=e.data,this.isMatchIgnored=!1}
ignoreMatch(){this.isMatchIgnored=!0}}function t(e){
return e.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/"/g,"&quot;").replace(/'/g,"&#x27;")
}function a(e,...n){const t=Object.create(null);for(const n in e)t[n]=e[n]
;return n.forEach(e=>{for(const n in e)t[n]=e[n]}),t}const i=e=>!!e.scope
;class r{constructor(e,n){
this.buffer="",this.classPrefix=n.classPrefix,e.walk(this)}addText(e){
this.buffer+=t(e)}openNode(e){if(!i(e))return;const n=((e,{prefix:n})=>{
if(e.startsWith("language:"))return e.replace("language:","language-")
;if(e.includes(".")){const t=e.split(".")
;return[`${n}${t.shift()}`,...t.map((e,n)=>`${e}${"_".repeat(n+1)}`)].join(" ")}
return`${n}${e}`})(e.scope,{prefix:this.classPrefix});this.span(n)}closeNode(e){
i(e)&&(this.buffer+="</span>")}value(){return this.buffer}span(e){
this.buffer+=`<span class="${e}">`}}const s=(e={})=>{const n={children:[]}
;return Object.assign(n,e),n};class o{constructor(){
this.rootNode=s(),this.stack=[this.rootNode]}get top(){
return this.stack[this.stack.length-1]}get root(){return this.rootNode}add(e){
this.top.children.push(e)}openNode(e){const n=s({scope:e})
;this.add(n),this.stack.push(n)}closeNode(){
if(this.stack.length>1)return this.stack.pop()}closeAllNodes(){
for(;this.closeNode(););}toJSON(){return JSON.stringify(this.rootNode,null,4)}
walk(e){return this.constructor._walk(e,this.rootNode)}static _walk(e,n){
return"string"==typeof n?e.addText(n):n.children&&(e.openNode(n),
n.children.forEach(n=>this._walk(e,n)),e.closeNode(n)),e}static _collapse(e){
"string"!=typeof e&&e.children&&(e.children.every(e=>"string"==typeof e)?e.children=[e.children.join("")]:e.children.forEach(e=>{
o._collapse(e)}))}}class c extends o{constructor(e){super(),this.options=e}
addText(e){""!==e&&this.add(e)}startScope(e){this.openNode(e)}endScope(){
this.closeNode()}__addSublanguage(e,n){const t=e.root
;n&&(t.scope="language:"+n),this.add(t)}toHTML(){
return new r(this,this.options).value()}finalize(){
return this.closeAllNodes(),!0}}function l(e){
return e?"string"==typeof e?e:e.source:null}function d(e){return b("(?=",e,")")}
function g(e){return b("(?:",e,")*")}function u(e){return b("(?:",e,")?")}
function b(...e){return e.map(e=>l(e)).join("")}function m(...e){const n=(e=>{
const n=e[e.length-1]
;return"object"==typeof n&&n.constructor===Object?(e.splice(e.length-1,1),n):{}
})(e);return"("+(n.capture?"":"?:")+e.map(e=>l(e)).join("|")+")"}function p(e){
return RegExp(e.toString()+"|").exec("").length-1}
const _=RegExp(m(/\[(?:[^\\\]]|\\.)*\]/,/\(\?<(?![=!])[^>]+>/,/\(\?'[^']+'/,/\(\??/,/\\([1-9][0-9]*)/,/\\./))
;function h(e,{joinWith:n}){let t=0;return e.map(e=>{t+=1;const n=t
;let a=l(e),i="";for(;a.length>0;){const e=_.exec(a);if(!e){i+=a;break}
i+=a.substring(0,e.index),
a=a.substring(e.index+e[0].length),"\\"===e[0][0]&&e[1]?i+="\\"+(Number(e[1])+n):(i+=e[0],
("("===e[0]||/^\(\?[<']/.test(e[0]))&&t++)}return i}).map(e=>`(${e})`).join(n)}
const f="[a-zA-Z]\\w*",E="[a-zA-Z_]\\w*",y="\\b\\d+(\\.\\d+)?",w="(-?)(\\b0[xX][a-fA-F0-9]+|(\\b\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?)",v="\\b(0b[01]+)",N={
begin:"\\\\[\\s\\S]",relevance:0},k={scope:"string",begin:"'",end:"'",
illegal:"\\n",contains:[N]},x={scope:"string",begin:'"',end:'"',illegal:"\\n",
contains:[N]},O=(e,n,t={})=>{const i=a({scope:"comment",begin:e,end:n,
contains:[]},t);i.contains.push({scope:"doctag",
begin:"[ ]*(?=(TODO|FIXME|NOTE|BUG|OPTIMIZE|HACK|XXX):)",
end:/(TODO|FIXME|NOTE|BUG|OPTIMIZE|HACK|XXX):/,excludeBegin:!0,relevance:0})
;const r=m("I","a","is","so","us","to","at","if","in","it","on",/[A-Za-z]+['](d|ve|re|ll|t|s|n)/,/[A-Za-z]+[-][a-z]+/,/[A-Za-z][a-z]{2,}/)
;return i.contains.push({begin:b(/[ ]+/,"(",r,/[.]?[:]?([.][ ]|[ ])/,"){3}")}),i
},M=O("//","$"),A=O("/\\*","\\*/"),S=O("#","$");var C=Object.freeze({
__proto__:null,APOS_STRING_MODE:k,BACKSLASH_ESCAPE:N,BINARY_NUMBER_MODE:{
scope:"number",begin:v,relevance:0},BINARY_NUMBER_RE:v,COMMENT:O,
C_BLOCK_COMMENT_MODE:A,C_LINE_COMMENT_MODE:M,C_NUMBER_MODE:{scope:"number",
begin:w,relevance:0},C_NUMBER_RE:w,END_SAME_AS_BEGIN:e=>Object.assign(e,{
"on:begin":(e,n)=>{n.data._beginMatch=e[1]},"on:end":(e,n)=>{
n.data._beginMatch!==e[1]&&n.ignoreMatch()}}),HASH_COMMENT_MODE:S,IDENT_RE:f,
MATCH_NOTHING_RE:/\b\B/,METHOD_GUARD:{begin:"\\.\\s*"+E,relevance:0},
NUMBER_MODE:{scope:"number",begin:y,relevance:0},NUMBER_RE:y,
PHRASAL_WORDS_MODE:{
begin:/\b(a|an|the|are|I'm|isn't|don't|doesn't|won't|but|just|should|pretty|simply|enough|gonna|going|wtf|so|such|will|you|your|they|like|more)\b/
},QUOTE_STRING_MODE:x,REGEXP_MODE:{scope:"regexp",begin:/\/(?=[^/\n]*\/)/,
end:/\/[gimuy]*/,contains:[N,{begin:/\[/,end:/\]/,relevance:0,contains:[N]}]},
RE_STARTERS_RE:"!|!=|!==|%|%=|&|&&|&=|\\*|\\*=|\\+|\\+=|,|-|-=|/=|/|:|;|<<|<<=|<=|<|===|==|=|>>>=|>>=|>=|>>>|>>|>|\\?|\\[|\\{|\\(|\\^|\\^=|\\||\\|=|\\|\\||~",
SHEBANG:(e={})=>{const n=/^#![ ]*\//
;return e.binary&&(e.begin=b(n,/.*\b/,e.binary,/\b.*/)),a({scope:"meta",begin:n,
end:/$/,relevance:0,"on:begin":(e,n)=>{0!==e.index&&n.ignoreMatch()}},e)},
TITLE_MODE:{scope:"title",begin:f,relevance:0},UNDERSCORE_IDENT_RE:E,
UNDERSCORE_TITLE_MODE:{scope:"title",begin:E,relevance:0}});function T(e,n){
"."===e.input[e.index-1]&&n.ignoreMatch()}function R(e,n){
void 0!==e.className&&(e.scope=e.className,delete e.className)}function D(e,n){
n&&e.beginKeywords&&(e.begin="\\b("+e.beginKeywords.split(" ").join("|")+")(?!\\.)(?=\\b|\\s)",
e.__beforeBegin=T,e.keywords=e.keywords||e.beginKeywords,delete e.beginKeywords,
void 0===e.relevance&&(e.relevance=0))}function I(e,n){
Array.isArray(e.illegal)&&(e.illegal=m(...e.illegal))}function L(e,n){
if(e.match){
if(e.begin||e.end)throw Error("begin & end are not supported with match")
;e.begin=e.match,delete e.match}}function B(e,n){
void 0===e.relevance&&(e.relevance=1)}const F=(e,n)=>{if(!e.beforeMatch)return
;if(e.starts)throw Error("beforeMatch cannot be used with starts")
;const t=Object.assign({},e);Object.keys(e).forEach(n=>{delete e[n]
}),e.keywords=t.keywords,e.begin=b(t.beforeMatch,d(t.begin)),e.starts={
relevance:0,contains:[Object.assign(t,{endsParent:!0})]
},e.relevance=0,delete t.beforeMatch
},$=["of","and","for","in","not","or","if","then","parent","list","value"]
;function z(e,n,t="keyword"){const a=Object.create(null)
;return"string"==typeof e?i(t,e.split(" ")):Array.isArray(e)?i(t,e):Object.keys(e).forEach(t=>{
Object.assign(a,z(e[t],n,t))}),a;function i(e,t){
n&&(t=t.map(e=>e.toLowerCase())),t.forEach(n=>{const t=n.split("|")
;a[t[0]]=[e,j(t[0],t[1])]})}}function j(e,n){
return n?Number(n):(e=>$.includes(e.toLowerCase()))(e)?0:1}const U={},P=e=>{
console.error(e)},K=(e,...n)=>{console.log("WARN: "+e,...n)},H=(e,n)=>{
U[`${e}/${n}`]||(console.log(`Deprecated as of ${e}. ${n}`),U[`${e}/${n}`]=!0)
},q=Error();function G(e,n,{key:t}){let a=0;const i=e[t],r={},s={}
;for(let e=1;e<=n.length;e++)s[e+a]=i[e],r[e+a]=!0,a+=p(n[e-1])
;e[t]=s,e[t]._emit=r,e[t]._multi=!0}function Z(e){(e=>{
e.scope&&"object"==typeof e.scope&&null!==e.scope&&(e.beginScope=e.scope,
delete e.scope)})(e),"string"==typeof e.beginScope&&(e.beginScope={
_wrap:e.beginScope}),"string"==typeof e.endScope&&(e.endScope={_wrap:e.endScope
}),(e=>{if(Array.isArray(e.begin)){
if(e.skip||e.excludeBegin||e.returnBegin)throw P("skip, excludeBegin, returnBegin not compatible with beginScope: {}"),
q
;if("object"!=typeof e.beginScope||null===e.beginScope)throw P("beginScope must be object"),
q;G(e,e.begin,{key:"beginScope"}),e.begin=h(e.begin,{joinWith:""})}})(e),(e=>{
if(Array.isArray(e.end)){
if(e.skip||e.excludeEnd||e.returnEnd)throw P("skip, excludeEnd, returnEnd not compatible with endScope: {}"),
q
;if("object"!=typeof e.endScope||null===e.endScope)throw P("endScope must be object"),
q;G(e,e.end,{key:"endScope"}),e.end=h(e.end,{joinWith:""})}})(e)}function W(e){
function n(n,t){
return RegExp(l(n),"m"+(e.case_insensitive?"i":"")+(e.unicodeRegex?"u":"")+(t?"g":""))
}class t{constructor(){
this.matchIndexes={},this.regexes=[],this.matchAt=1,this.position=0}
addRule(e,n){
n.position=this.position++,this.matchIndexes[this.matchAt]=n,this.regexes.push([n,e]),
this.matchAt+=p(e)+1}compile(){0===this.regexes.length&&(this.exec=()=>null)
;const e=this.regexes.map(e=>e[1]);this.matcherRe=n(h(e,{joinWith:"|"
}),!0),this.lastIndex=0}exec(e){this.matcherRe.lastIndex=this.lastIndex
;const n=this.matcherRe.exec(e);if(!n)return null
;const t=n.findIndex((e,n)=>n>0&&void 0!==e),a=this.matchIndexes[t]
;return n.splice(0,t),Object.assign(n,a)}}class i{constructor(){
this.rules=[],this.multiRegexes=[],
this.count=0,this.lastIndex=0,this.regexIndex=0}getMatcher(e){
if(this.multiRegexes[e])return this.multiRegexes[e];const n=new t
;return this.rules.slice(e).forEach(([e,t])=>n.addRule(e,t)),
n.compile(),this.multiRegexes[e]=n,n}resumingScanAtSamePosition(){
return 0!==this.regexIndex}considerAll(){this.regexIndex=0}addRule(e,n){
this.rules.push([e,n]),"begin"===n.type&&this.count++}exec(e){
const n=this.getMatcher(this.regexIndex);n.lastIndex=this.lastIndex
;let t=n.exec(e)
;if(this.resumingScanAtSamePosition())if(t&&t.index===this.lastIndex);else{
const n=this.getMatcher(0);n.lastIndex=this.lastIndex+1,t=n.exec(e)}
return t&&(this.regexIndex+=t.position+1,
this.regexIndex===this.count&&this.considerAll()),t}}
if(e.compilerExtensions||(e.compilerExtensions=[]),
e.contains&&e.contains.includes("self"))throw Error("ERR: contains `self` is not supported at the top-level of a language.  See documentation.")
;return e.classNameAliases=a(e.classNameAliases||{}),function t(r,s){const o=r
;if(r.isCompiled)return o
;[R,L,Z,F].forEach(e=>e(r,s)),e.compilerExtensions.forEach(e=>e(r,s)),
r.__beforeBegin=null,[D,I,B].forEach(e=>e(r,s)),r.isCompiled=!0;let c=null
;return"object"==typeof r.keywords&&r.keywords.$pattern&&(r.keywords=Object.assign({},r.keywords),
c=r.keywords.$pattern,
delete r.keywords.$pattern),c=c||/\w+/,r.keywords&&(r.keywords=z(r.keywords,e.case_insensitive)),
o.keywordPatternRe=n(c,!0),
s&&(r.begin||(r.begin=/\B|\b/),o.beginRe=n(o.begin),r.end||r.endsWithParent||(r.end=/\B|\b/),
r.end&&(o.endRe=n(o.end)),
o.terminatorEnd=l(o.end)||"",r.endsWithParent&&s.terminatorEnd&&(o.terminatorEnd+=(r.end?"|":"")+s.terminatorEnd)),
r.illegal&&(o.illegalRe=n(r.illegal)),
r.contains||(r.contains=[]),r.contains=[].concat(...r.contains.map(e=>(e=>(e.variants&&!e.cachedVariants&&(e.cachedVariants=e.variants.map(n=>a(e,{
variants:null},n))),e.cachedVariants?e.cachedVariants:Q(e)?a(e,{
starts:e.starts?a(e.starts):null
}):Object.isFrozen(e)?a(e):e))("self"===e?r:e))),r.contains.forEach(e=>{t(e,o)
}),r.starts&&t(r.starts,s),o.matcher=(e=>{const n=new i
;return e.contains.forEach(e=>n.addRule(e.begin,{rule:e,type:"begin"
})),e.terminatorEnd&&n.addRule(e.terminatorEnd,{type:"end"
}),e.illegal&&n.addRule(e.illegal,{type:"illegal"}),n})(o),o}(e)}function Q(e){
return!!e&&(e.endsWithParent||Q(e.starts))}class X extends Error{
constructor(e,n){super(e),this.name="HTMLInjectionError",this.html=n}}
const V=t,J=a,Y=Symbol("nomatch"),ee=t=>{
const a=Object.create(null),i=Object.create(null),r=[];let s=!0
;const o="Could not find the language '{}', did you forget to load/include a language module?",l={
disableAutodetect:!0,name:"Plain text",contains:[]};let p={
ignoreUnescapedHTML:!1,throwUnescapedHTML:!1,noHighlightRe:/^(no-?highlight)$/i,
languageDetectRe:/\blang(?:uage)?-([\w-]+)\b/i,classPrefix:"hljs-",
cssSelector:"pre code",languages:null,__emitter:c};function _(e){
return p.noHighlightRe.test(e)}function h(e,n,t){let a="",i=""
;"object"==typeof n?(a=e,
t=n.ignoreIllegals,i=n.language):(H("10.7.0","highlight(lang, code, ...args) has been deprecated."),
H("10.7.0","Please use highlight(code, options) instead.\nhttps://github.com/highlightjs/highlight.js/issues/2277"),
i=e,a=n),void 0===t&&(t=!0);const r={code:a,language:i};O("before:highlight",r)
;const s=r.result?r.result:f(r.language,r.code,t)
;return s.code=r.code,O("after:highlight",s),s}function f(e,t,i,r){
const c=Object.create(null);function l(e,n){return e.keywords[n]}function d(){
if(!M.keywords)return void S.addText(C);let e=0;M.keywordPatternRe.lastIndex=0
;let n=M.keywordPatternRe.exec(C),t="";for(;n;){t+=C.substring(e,n.index)
;const a=k.case_insensitive?n[0].toLowerCase():n[0],i=l(M,a);if(i){const[e,r]=i
;if(S.addText(t),
t="",c[a]=(c[a]||0)+1,c[a]<=7&&(T+=r),e.startsWith("_"))t+=n[0];else{
const t=k.classNameAliases[e]||e;u(n[0],t)}}else t+=n[0]
;e=M.keywordPatternRe.lastIndex,n=M.keywordPatternRe.exec(C)}
t+=C.substring(e),S.addText(t)}function g(){null!=M.subLanguage?(()=>{
if(""===C)return;let e=null;if("string"==typeof M.subLanguage){
if(!a[M.subLanguage])return void S.addText(C)
;e=f(M.subLanguage,C,!0,A[M.subLanguage]),A[M.subLanguage]=e._top
}else e=E(C,M.subLanguage.length?M.subLanguage:null)
;M.relevance>0&&(T+=e.relevance),S.__addSublanguage(e._emitter,e.language)
})():d(),C=""}function u(e,n){
""!==e&&(S.startScope(n),S.addText(e),S.endScope())}function b(e,n){let t=1
;const a=n.length-1;for(;t<=a;){if(!e._emit[t]){t++;continue}
const a=k.classNameAliases[e[t]]||e[t],i=n[t];a?u(i,a):(C=i,d(),C=""),t++}}
function m(e,n){
return e.scope&&"string"==typeof e.scope&&S.openNode(k.classNameAliases[e.scope]||e.scope),
e.beginScope&&(e.beginScope._wrap?(u(C,k.classNameAliases[e.beginScope._wrap]||e.beginScope._wrap),
C=""):e.beginScope._multi&&(b(e.beginScope,n),C="")),M=Object.create(e,{parent:{
value:M}}),M}function _(e,t,a){let i=((e,n)=>{const t=e&&e.exec(n)
;return t&&0===t.index})(e.endRe,a);if(i){if(e["on:end"]){const a=new n(e)
;e["on:end"](t,a),a.isMatchIgnored&&(i=!1)}if(i){
for(;e.endsParent&&e.parent;)e=e.parent;return e}}
if(e.endsWithParent)return _(e.parent,t,a)}function h(e){
return 0===M.matcher.regexIndex?(C+=e[0],1):(I=!0,0)}function y(e){
const n=e[0],a=t.substring(e.index),i=_(M,e,a);if(!i)return Y;const r=M
;M.endScope&&M.endScope._wrap?(g(),
u(n,M.endScope._wrap)):M.endScope&&M.endScope._multi?(g(),
b(M.endScope,e)):r.skip?C+=n:(r.returnEnd||r.excludeEnd||(C+=n),
g(),r.excludeEnd&&(C=n));do{
M.scope&&S.closeNode(),M.skip||M.subLanguage||(T+=M.relevance),M=M.parent
}while(M!==i.parent);return i.starts&&m(i.starts,e),r.returnEnd?0:n.length}
let w={};function v(a,r){const o=r&&r[0];if(C+=a,null==o)return g(),0
;if("begin"===w.type&&"end"===r.type&&w.index===r.index&&""===o){
if(C+=t.slice(r.index,r.index+1),!s){const n=Error(`0 width match regex (${e})`)
;throw n.languageName=e,n.badRule=w.rule,n}return 1}
if(w=r,"begin"===r.type)return(e=>{
const t=e[0],a=e.rule,i=new n(a),r=[a.__beforeBegin,a["on:begin"]]
;for(const n of r)if(n&&(n(e,i),i.isMatchIgnored))return h(t)
;return a.skip?C+=t:(a.excludeBegin&&(C+=t),
g(),a.returnBegin||a.excludeBegin||(C=t)),m(a,e),a.returnBegin?0:t.length})(r)
;if("illegal"===r.type&&!i){
const e=Error('Illegal lexeme "'+o+'" for mode "'+(M.scope||"<unnamed>")+'"')
;throw e.mode=M,e}if("end"===r.type){const e=y(r);if(e!==Y)return e}
if("illegal"===r.type&&""===o)return r.index===t.length||(C+="\n"),1
;if(D>1e5&&D>3*r.index)throw Error("potential infinite loop, way more iterations than matches")
;return C+=o,o.length}const k=N(e)
;if(!k)throw P(o.replace("{}",e)),Error('Unknown language: "'+e+'"')
;const x=W(k);let O="",M=r||x;const A={},S=new p.__emitter(p);(()=>{const e=[]
;for(let n=M;n!==k;n=n.parent)n.scope&&e.unshift(n.scope)
;e.forEach(e=>S.openNode(e))})();let C="",T=0,R=0,D=0,I=!1;try{
if(k.__emitTokens)k.__emitTokens(t,S);else{for(M.matcher.considerAll();;){
D++,I?I=!1:M.matcher.considerAll(),M.matcher.lastIndex=R
;const e=M.matcher.exec(t);if(!e)break;const n=v(t.substring(R,e.index),e)
;R=e.index+n}v(t.substring(R))}return S.finalize(),O=S.toHTML(),{language:e,
value:O,relevance:T,illegal:!1,_emitter:S,_top:M}}catch(n){
if(n.message&&n.message.includes("Illegal"))return{language:e,value:V(t),
illegal:!0,relevance:0,_illegalBy:{message:n.message,index:R,
context:t.slice(R-100,R+100),mode:n.mode,resultSoFar:O},_emitter:S};if(s)return{
language:e,value:V(t),illegal:!1,relevance:0,errorRaised:n,_emitter:S,_top:M}
;throw n}}function E(e,n){n=n||p.languages||Object.keys(a);const t=(e=>{
const n={value:V(e),illegal:!1,relevance:0,_top:l,_emitter:new p.__emitter(p)}
;return n._emitter.addText(e),n})(e),i=n.filter(N).filter(x).map(n=>f(n,e,!1))
;i.unshift(t);const r=i.sort((e,n)=>{
if(e.relevance!==n.relevance)return n.relevance-e.relevance
;if(e.language&&n.language){if(N(e.language).supersetOf===n.language)return 1
;if(N(n.language).supersetOf===e.language)return-1}return 0}),[s,o]=r,c=s
;return c.secondBest=o,c}function y(e){let n=null;const t=(e=>{
let n=e.className+" ";n+=e.parentNode?e.parentNode.className:""
;const t=p.languageDetectRe.exec(n);if(t){const n=N(t[1])
;return n||(K(o.replace("{}",t[1])),
K("Falling back to no-highlight mode for this block.",e)),n?t[1]:"no-highlight"}
return n.split(/\s+/).find(e=>_(e)||N(e))})(e);if(_(t))return
;if(O("before:highlightElement",{el:e,language:t
}),e.dataset.highlighted)return void console.log("Element previously highlighted. To highlight again, first unset `dataset.highlighted`.",e)
;if(e.children.length>0&&(p.ignoreUnescapedHTML||(console.warn("One of your code blocks includes unescaped HTML. This is a potentially serious security risk."),
console.warn("https://github.com/highlightjs/highlight.js/wiki/security"),
console.warn("The element with unescaped HTML:"),
console.warn(e)),p.throwUnescapedHTML))throw new X("One of your code blocks includes unescaped HTML.",e.innerHTML)
;n=e;const a=n.textContent,r=t?h(a,{language:t,ignoreIllegals:!0}):E(a)
;e.innerHTML=r.value,e.dataset.highlighted="yes",((e,n,t)=>{const a=n&&i[n]||t
;e.classList.add("hljs"),e.classList.add("language-"+a)
})(e,t,r.language),e.result={language:r.language,re:r.relevance,
relevance:r.relevance},r.secondBest&&(e.secondBest={
language:r.secondBest.language,relevance:r.secondBest.relevance
}),O("after:highlightElement",{el:e,result:r,text:a})}let w=!1;function v(){
if("loading"===document.readyState)return w||window.addEventListener("DOMContentLoaded",()=>{
v()},!1),void(w=!0);document.querySelectorAll(p.cssSelector).forEach(y)}
function N(e){return e=(e||"").toLowerCase(),a[e]||a[i[e]]}
function k(e,{languageName:n}){"string"==typeof e&&(e=[e]),e.forEach(e=>{
i[e.toLowerCase()]=n})}function x(e){const n=N(e);return n&&!n.disableAutodetect
}function O(e,n){const t=e;r.forEach(e=>{e[t]&&e[t](n)})}Object.assign(t,{
highlight:h,highlightAuto:E,highlightAll:v,highlightElement:y,
highlightBlock:e=>(H("10.7.0","highlightBlock will be removed entirely in v12.0"),
H("10.7.0","Please use highlightElement now."),y(e)),configure:e=>{p=J(p,e)},
initHighlighting:()=>{
v(),H("10.6.0","initHighlighting() deprecated.  Use highlightAll() now.")},
initHighlightingOnLoad:()=>{
v(),H("10.6.0","initHighlightingOnLoad() deprecated.  Use highlightAll() now.")
},registerLanguage:(e,n)=>{let i=null;try{i=n(t)}catch(n){
if(P("Language definition for '{}' could not be registered.".replace("{}",e)),
!s)throw n;P(n),i=l}
i.name||(i.name=e),a[e]=i,i.rawDefinition=n.bind(null,t),i.aliases&&k(i.aliases,{
languageName:e})},unregisterLanguage:e=>{delete a[e]
;for(const n of Object.keys(i))i[n]===e&&delete i[n]},
listLanguages:()=>Object.keys(a),getLanguage:N,registerAliases:k,
autoDetection:x,inherit:J,addPlugin:e=>{(e=>{
e["before:highlightBlock"]&&!e["before:highlightElement"]&&(e["before:highlightElement"]=n=>{
e["before:highlightBlock"](Object.assign({block:n.el},n))
}),e["after:highlightBlock"]&&!e["after:highlightElement"]&&(e["after:highlightElement"]=n=>{
e["after:highlightBlock"](Object.assign({block:n.el},n))})})(e),r.push(e)},
removePlugin:e=>{const n=r.indexOf(e);-1!==n&&r.splice(n,1)}}),t.debugMode=()=>{
s=!1},t.safeMode=()=>{s=!0},t.versionString="11.12.0",t.regex={concat:b,
lookahead:d,either:m,optional:u,anyNumberOfTimes:g}
;for(const n in C)"object"==typeof C[n]&&e(C[n]);return Object.assign(t,C),t
},ne=ee({});ne.newInstance=()=>ee({});const te="[A-Za-z$_][0-9A-Za-z$_]*",ae={
scope:"number",
match:"([-+]?)(\\b0[xX][a-fA-F0-9]+|(\\b\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?)|NaN|[-+]?Infinity",
relevance:0
},ie=["as","in","of","if","for","while","finally","var","new","function","do","return","void","else","break","catch","instanceof","with","throw","case","default","try","switch","continue","typeof","delete","let","yield","const","class","debugger","async","await","static","import","from","export","extends","using"],re=["true","false","null","undefined","NaN","Infinity"],se=["Object","Function","Boolean","Symbol","Math","Date","Number","BigInt","String","RegExp","Array","Float32Array","Float64Array","Int8Array","Uint8Array","Uint8ClampedArray","Int16Array","Int32Array","Uint16Array","Uint32Array","BigInt64Array","BigUint64Array","Set","Map","WeakSet","WeakMap","ArrayBuffer","SharedArrayBuffer","Atomics","DataView","JSON","Promise","Generator","GeneratorFunction","AsyncFunction","Reflect","Proxy","Intl","WebAssembly"],oe=["Error","EvalError","InternalError","RangeError","ReferenceError","SyntaxError","TypeError","URIError"],ce=["setInterval","setTimeout","clearInterval","clearTimeout","require","exports","eval","isFinite","isNaN","parseFloat","parseInt","decodeURI","decodeURIComponent","encodeURI","encodeURIComponent","escape","unescape"],le=["arguments","this","super","console","window","document","localStorage","sessionStorage","module","self","global"],de=[].concat(ce,se,oe)
;function ge(e){const n=e.regex,t=te,a={begin:/<[A-Za-z0-9\\._:-]+/,
end:/\/[A-Za-z0-9\\._:-]+>|\/>/,isTrulyOpeningTag:(e,n)=>{
const t=e[0].length+e.index,a=e.input[t]
;if("<"===a||","===a)return void n.ignoreMatch();let i
;">"===a&&(((e,{after:n})=>{const t="</"+e[0].slice(1)
;return-1!==e.input.indexOf(t,n)})(e,{after:t})||n.ignoreMatch())
;const r=e.input.substring(t)
;((i=r.match(/^\s*=/))||(i=r.match(/^\s+extends\s+/))&&0===i.index)&&n.ignoreMatch()
}},i={$pattern:te,keyword:ie,literal:re,built_in:de,"variable.language":le
},r="[0-9](_?[0-9])*",s=`\\.(${r})`,o="0|[1-9](_?[0-9])*|0[0-7]*[89][0-9]*",c={
className:"number",variants:[{
begin:`(\\b(${o})((${s})|\\.)?|(${s}))[eE][+-]?(${r})\\b`},{
begin:`\\b(${o})\\b((${s})\\b|\\.)?|(${s})\\b`},{
begin:"\\b(0|[1-9](_?[0-9])*)n\\b"},{
begin:"\\b0[xX][0-9a-fA-F](_?[0-9a-fA-F])*n?\\b"},{
begin:"\\b0[bB][0-1](_?[0-1])*n?\\b"},{begin:"\\b0[oO][0-7](_?[0-7])*n?\\b"},{
begin:"\\b0[0-7]+n?\\b"}],relevance:0},l={className:"subst",begin:"\\$\\{",
end:"\\}",keywords:i,contains:[]},d={begin:".?html`",end:"",starts:{end:"`",
returnEnd:!1,contains:[e.BACKSLASH_ESCAPE,l],subLanguage:"xml"}},g={
begin:".?css`",end:"",starts:{end:"`",returnEnd:!1,
contains:[e.BACKSLASH_ESCAPE,l],subLanguage:"css"}},u={begin:".?gql`",end:"",
starts:{end:"`",returnEnd:!1,contains:[e.BACKSLASH_ESCAPE,l],
subLanguage:"graphql"}},b={className:"string",begin:"`",end:"`",
contains:[e.BACKSLASH_ESCAPE,l]},m={className:"comment",
variants:[e.COMMENT(/\/\*\*(?!\/)/,"\\*/",{relevance:0,contains:[{
begin:"(?=@[A-Za-z]+)",relevance:0,contains:[{className:"doctag",
begin:"@[A-Za-z]+"},{className:"type",begin:"\\{",end:"\\}",excludeEnd:!0,
excludeBegin:!0,relevance:0},{className:"variable",begin:t+"(?=\\s*(-)|$)",
endsParent:!0,relevance:0},{begin:/(?=[^\n])\s/,relevance:0}]}]
}),e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE]
},p=[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,d,g,u,b,{match:/\$\d+/},c]
;l.contains=p.concat({begin:/\{/,end:/\}/,keywords:i,contains:["self"].concat(p)
});const _=[].concat(m,l.contains),h=_.concat([{begin:/(\s*)\(/,end:/\)/,
keywords:i,contains:["self"].concat(_)}]),f={className:"params",begin:/(\s*)\(/,
end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:i,contains:h},E={variants:[{
match:[/class/,/\s+/,t,/\s+/,/extends/,/\s+/,n.concat(t,"(",n.concat(/\./,t),")*")],
scope:{1:"keyword",3:"title.class",5:"keyword",7:"title.class.inherited"}},{
match:[/class/,/\s+/,t],scope:{1:"keyword",3:"title.class"}}]},y={relevance:0,
match:n.either(/\bJSON/,/\b[A-Z][a-z]+([A-Z][a-z]*|\d)*/,/\b[A-Z]{2,}([A-Z][a-z]+|\d)+([A-Z][a-z]*)*/,/\b[A-Z]{2,}[a-z]+([A-Z][a-z]+|\d)*([A-Z][a-z]*)*/),
className:"title.class",keywords:{_:[...se,...oe]}},w={variants:[{
match:[/function/,/\s+/,t,/(?=\s*\()/]},{match:[/function/,/\s*(?=\()/]}],
className:{1:"keyword",3:"title.function"},label:"func.def",contains:[f],
illegal:/%/},v={
match:n.concat(/\b/,(N=[...ce,"super","import","await"].map(e=>e+"\\s*\\("),
n.concat("(?!",N.join("|"),")")),t,n.lookahead(/\s*\(/)),
className:"title.function",relevance:0};var N;const k={
begin:n.concat(/\./,n.lookahead(n.concat(t,/(?![0-9A-Za-z$_(])/))),end:t,
excludeBegin:!0,keywords:"prototype",className:"property",relevance:0},x={
match:[/get|set/,/\s+/,t,/(?=\()/],className:{1:"keyword",3:"title.function"},
contains:[{begin:/\(\)/},f]
},O="(\\([^()]*(\\([^()]*(\\([^()]*\\)[^()]*)*\\)[^()]*)*\\)|"+e.UNDERSCORE_IDENT_RE+")\\s*=>",M={
match:[/const|var|let/,/\s+/,t,/\s*/,/=\s*/,/(async\s*)?/,n.lookahead(O)],
keywords:"async",className:{1:"keyword",3:"title.function"},contains:[f]}
;return{name:"JavaScript",aliases:["js","jsx","mjs","cjs"],keywords:i,exports:{
PARAMS_CONTAINS:h,CLASS_REFERENCE:y},illegal:/#(?![$_A-Za-z])/,
contains:[e.SHEBANG({label:"shebang",binary:"node",relevance:5}),{
label:"use_strict",className:"meta",relevance:10,
begin:/^\s*['"]use (strict|asm)['"]/
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,d,g,u,b,m,{match:/\$\d+/},c,y,{
scope:"attr",match:t+n.lookahead(":"),relevance:0},M,{
begin:"("+e.RE_STARTERS_RE+"|\\b(case|return|throw)\\b)\\s*",
keywords:"return throw case",relevance:0,contains:[m,e.REGEXP_MODE,{
className:"function",begin:O,returnBegin:!0,end:"\\s*=>",contains:[{
className:"params",variants:[{begin:e.UNDERSCORE_IDENT_RE,relevance:0},{
className:null,begin:/\(\s*\)/,skip:!0},{begin:/(\s*)\(/,end:/\)/,
excludeBegin:!0,excludeEnd:!0,keywords:i,contains:h}]}]},{begin:/,/,relevance:0
},{match:/\s+/,relevance:0},{variants:[{begin:"<>",end:"</>"},{
match:/<[A-Za-z0-9\\._:-]+\s*\/>/},{begin:a.begin,
"on:begin":a.isTrulyOpeningTag,end:a.end}],subLanguage:"xml",contains:[{
begin:a.begin,end:a.end,skip:!0,contains:["self"]}]}]},w,{
beginKeywords:"while if switch catch for"},{
begin:"\\b(?!function)"+e.UNDERSCORE_IDENT_RE+"\\([^()]*(\\([^()]*(\\([^()]*\\)[^()]*)*\\)[^()]*)*\\)\\s*\\{",
returnBegin:!0,label:"func.def",contains:[f,e.inherit(e.TITLE_MODE,{begin:t,
className:"title.function"})]},{match:/\.\.\./,relevance:0},k,{match:"\\$"+t,
relevance:0},{match:[/\bconstructor(?=\s*\()/],className:{1:"title.function"},
contains:[f]},v,{relevance:0,match:/\b[A-Z][A-Z_0-9]+\b/,
className:"variable.constant"},E,x,{match:/\$[(.]/}]}}
const ue=e=>b(/\b/,e,/\w$/.test(e)?/\b/:/\B/),be=["Protocol","Type"].map(ue),me=["init","self"].map(ue),pe=["Any","Self"],_e=["actor","any","associatedtype","async","await",/as\?/,/as!/,"as","borrowing","break","case","catch","class","consume","consuming","continue","convenience","copy","default","defer","deinit","didSet","distributed","do","dynamic","each","else","enum","extension","fallthrough",/fileprivate\(set\)/,"fileprivate","final","for","func","get","guard","if","import","indirect","infix",/init\?/,/init!/,"inout",/internal\(set\)/,"internal","in","is","isolated","nonisolated","lazy","let","macro","mutating","nonmutating",/open\(set\)/,"open","operator","optional","override","package","postfix","precedencegroup","prefix",/private\(set\)/,"private","protocol",/public\(set\)/,"public","repeat","required","rethrows","return","set","some","static","struct","subscript","super","switch","throws","throw",/try\?/,/try!/,"try","typealias",/unowned\(safe\)/,/unowned\(unsafe\)/,"unowned","var","weak","where","while","willSet"],he=["false","nil","true"],fe=["assignment","associativity","higherThan","left","lowerThan","none","right"],Ee=["#colorLiteral","#column","#dsohandle","#else","#elseif","#endif","#error","#file","#fileID","#fileLiteral","#filePath","#function","#if","#imageLiteral","#keyPath","#line","#selector","#sourceLocation","#warning"],ye=["abs","all","any","assert","assertionFailure","debugPrint","dump","fatalError","getVaList","isKnownUniquelyReferenced","max","min","numericCast","pointwiseMax","pointwiseMin","precondition","preconditionFailure","print","readLine","repeatElement","sequence","stride","swap","swift_unboxFromSwiftValueWithType","transcode","type","unsafeBitCast","unsafeDowncast","withExtendedLifetime","withUnsafeMutablePointer","withUnsafePointer","withVaList","withoutActuallyEscaping","zip"],we=m(/[/=\-+!*%<>&|^~?]/,/[\u00A1-\u00A7]/,/[\u00A9\u00AB]/,/[\u00AC\u00AE]/,/[\u00B0\u00B1]/,/[\u00B6\u00BB\u00BF\u00D7\u00F7]/,/[\u2016-\u2017]/,/[\u2020-\u2027]/,/[\u2030-\u203E]/,/[\u2041-\u2053]/,/[\u2055-\u205E]/,/[\u2190-\u23FF]/,/[\u2500-\u2775]/,/[\u2794-\u2BFF]/,/[\u2E00-\u2E7F]/,/[\u3001-\u3003]/,/[\u3008-\u3020]/,/[\u3030]/),ve=m(we,/[\u0300-\u036F]/,/[\u1DC0-\u1DFF]/,/[\u20D0-\u20FF]/,/[\uFE00-\uFE0F]/,/[\uFE20-\uFE2F]/),Ne=b(we,ve,"*"),ke=m(/[a-zA-Z_]/,/[\u00A8\u00AA\u00AD\u00AF\u00B2-\u00B5\u00B7-\u00BA]/,/[\u00BC-\u00BE\u00C0-\u00D6\u00D8-\u00F6\u00F8-\u00FF]/,/[\u0100-\u02FF\u0370-\u167F\u1681-\u180D\u180F-\u1DBF]/,/[\u1E00-\u1FFF]/,/[\u200B-\u200D\u202A-\u202E\u203F-\u2040\u2054\u2060-\u206F]/,/[\u2070-\u20CF\u2100-\u218F\u2460-\u24FF\u2776-\u2793]/,/[\u2C00-\u2DFF\u2E80-\u2FFF]/,/[\u3004-\u3007\u3021-\u302F\u3031-\u303F\u3040-\uD7FF]/,/[\uF900-\uFD3D\uFD40-\uFDCF\uFDF0-\uFE1F\uFE30-\uFE44]/,/[\uFE47-\uFEFE\uFF00-\uFFFD]/),xe=m(ke,/\d/,/[\u0300-\u036F\u1DC0-\u1DFF\u20D0-\u20FF\uFE20-\uFE2F]/),Oe=b(ke,xe,"*"),Me=b(/[A-Z]/,xe,"*"),Ae=["attached","autoclosure",b(/convention\(/,m("swift","block","c"),/\)/),"discardableResult","dynamicCallable","dynamicMemberLookup","escaping","freestanding","frozen","GKInspectable","IBAction","IBDesignable","IBInspectable","IBOutlet","IBSegueAction","inlinable","main","nonobjc","NSApplicationMain","NSCopying","NSManaged",b(/objc\(/,Oe,/\)/),"objc","objcMembers","propertyWrapper","requires_stored_property_inits","resultBuilder","Sendable","testable","UIApplicationMain","unchecked","unknown","usableFromInline","warn_unqualified_access"],Se=["iOS","iOSApplicationExtension","macOS","macOSApplicationExtension","macCatalyst","macCatalystApplicationExtension","watchOS","watchOSApplicationExtension","tvOS","tvOSApplicationExtension","swift"],Ce=e=>({
IMPORTANT:{scope:"meta",begin:"!important"},
BLOCK_COMMENT:e.C_BLOCK_COMMENT_MODE,HEXCOLOR:{scope:"number",
begin:/#(([0-9a-fA-F]{3,4})|(([0-9a-fA-F]{2}){3,4}))\b/},UNICODE_RANGE:{
scope:"number",
begin:/\b[Uu]\+[0-9A-Fa-f][0-9A-Fa-f?]{0,5}(-[0-9A-Fa-f][0-9A-Fa-f]{0,5})?/},
FUNCTION_DISPATCH:{className:"built_in",begin:/[\w-]+(?=\()/},
ATTRIBUTE_SELECTOR_MODE:{scope:"selector-attr",begin:/\[/,end:/\]/,illegal:"$",
contains:[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]},CSS_NUMBER_MODE:{
scope:"number",
begin:e.NUMBER_RE+"(%|em|ex|ch|rem|vw|vh|vmin|vmax|cm|mm|in|pt|pc|px|deg|grad|rad|turn|s|ms|Hz|kHz|dpi|dpcm|dppx)?",
relevance:0},CSS_VARIABLE:{className:"attr",begin:/--[A-Za-z_][A-Za-z0-9_-]*/}
}),Te=["a","abbr","address","article","aside","audio","b","blockquote","body","button","canvas","caption","cite","code","dd","del","details","dfn","div","dl","dt","em","fieldset","figcaption","figure","footer","form","h1","h2","h3","h4","h5","h6","header","hgroup","html","i","iframe","img","input","ins","kbd","label","legend","li","main","mark","menu","nav","object","ol","optgroup","option","p","picture","q","quote","samp","section","select","source","span","strong","summary","sup","table","tbody","td","textarea","tfoot","th","thead","time","tr","ul","var","video","defs","g","marker","mask","pattern","svg","switch","symbol","feBlend","feColorMatrix","feComponentTransfer","feComposite","feConvolveMatrix","feDiffuseLighting","feDisplacementMap","feFlood","feGaussianBlur","feImage","feMerge","feMorphology","feOffset","feSpecularLighting","feTile","feTurbulence","linearGradient","radialGradient","stop","circle","ellipse","image","line","path","polygon","polyline","rect","text","use","textPath","tspan","foreignObject","clipPath"],Re=["any-hover","any-pointer","aspect-ratio","color","color-gamut","color-index","device-aspect-ratio","device-height","device-width","display-mode","forced-colors","grid","height","hover","inverted-colors","monochrome","orientation","overflow-block","overflow-inline","pointer","prefers-color-scheme","prefers-contrast","prefers-reduced-motion","prefers-reduced-transparency","resolution","scan","scripting","update","width","min-width","max-width","min-height","max-height"].sort().reverse(),De=["active","any-link","blank","checked","current","default","defined","dir","disabled","drop","empty","enabled","first","first-child","first-of-type","fullscreen","future","focus","focus-visible","focus-within","has","host","host-context","hover","indeterminate","in-range","invalid","is","lang","last-child","last-of-type","left","link","local-link","not","nth-child","nth-col","nth-last-child","nth-last-col","nth-last-of-type","nth-of-type","only-child","only-of-type","optional","out-of-range","past","placeholder-shown","read-only","read-write","required","right","root","scope","target","target-within","user-invalid","valid","visited","where"].sort().reverse(),Ie=["after","backdrop","before","cue","cue-region","first-letter","first-line","grammar-error","marker","part","placeholder","selection","slotted","spelling-error"].sort().reverse(),Le=["accent-color","align-content","align-items","align-self","alignment-baseline","all","anchor-name","animation","animation-composition","animation-delay","animation-direction","animation-duration","animation-fill-mode","animation-iteration-count","animation-name","animation-play-state","animation-range","animation-range-end","animation-range-start","animation-timeline","animation-timing-function","appearance","aspect-ratio","backdrop-filter","backface-visibility","background","background-attachment","background-blend-mode","background-clip","background-color","background-image","background-origin","background-position","background-position-x","background-position-y","background-repeat","background-size","baseline-shift","block-size","border","border-block","border-block-color","border-block-end","border-block-end-color","border-block-end-style","border-block-end-width","border-block-start","border-block-start-color","border-block-start-style","border-block-start-width","border-block-style","border-block-width","border-bottom","border-bottom-color","border-bottom-left-radius","border-bottom-right-radius","border-bottom-style","border-bottom-width","border-collapse","border-color","border-end-end-radius","border-end-start-radius","border-image","border-image-outset","border-image-repeat","border-image-slice","border-image-source","border-image-width","border-inline","border-inline-color","border-inline-end","border-inline-end-color","border-inline-end-style","border-inline-end-width","border-inline-start","border-inline-start-color","border-inline-start-style","border-inline-start-width","border-inline-style","border-inline-width","border-left","border-left-color","border-left-style","border-left-width","border-radius","border-right","border-right-color","border-right-style","border-right-width","border-spacing","border-start-end-radius","border-start-start-radius","border-style","border-top","border-top-color","border-top-left-radius","border-top-right-radius","border-top-style","border-top-width","border-width","bottom","box-align","box-decoration-break","box-direction","box-flex","box-flex-group","box-lines","box-ordinal-group","box-orient","box-pack","box-shadow","box-sizing","break-after","break-before","break-inside","caption-side","caret-color","clear","clip","clip-path","clip-rule","color","color-interpolation","color-interpolation-filters","color-profile","color-rendering","color-scheme","column-count","column-fill","column-gap","column-rule","column-rule-color","column-rule-style","column-rule-width","column-span","column-width","columns","contain","contain-intrinsic-block-size","contain-intrinsic-height","contain-intrinsic-inline-size","contain-intrinsic-size","contain-intrinsic-width","container","container-name","container-type","content","content-visibility","corner-bottom-left-shape","corner-bottom-right-shape","corner-shape","corner-top-left-shape","corner-top-right-shape","counter-increment","counter-reset","counter-set","cue","cue-after","cue-before","cursor","cx","cy","direction","display","dominant-baseline","empty-cells","enable-background","field-sizing","fill","fill-opacity","fill-rule","filter","flex","flex-basis","flex-direction","flex-flow","flex-grow","flex-shrink","flex-wrap","float","flood-color","flood-opacity","flow","font","font-display","font-family","font-feature-settings","font-kerning","font-language-override","font-optical-sizing","font-palette","font-size","font-size-adjust","font-smooth","font-smoothing","font-stretch","font-style","font-synthesis","font-synthesis-position","font-synthesis-small-caps","font-synthesis-style","font-synthesis-weight","font-variant","font-variant-alternates","font-variant-caps","font-variant-east-asian","font-variant-emoji","font-variant-ligatures","font-variant-numeric","font-variant-position","font-variation-settings","font-weight","forced-color-adjust","gap","glyph-orientation-horizontal","glyph-orientation-vertical","grid","grid-area","grid-auto-columns","grid-auto-flow","grid-auto-rows","grid-column","grid-column-end","grid-column-start","grid-gap","grid-row","grid-row-end","grid-row-start","grid-template","grid-template-areas","grid-template-columns","grid-template-rows","hanging-punctuation","height","hyphenate-character","hyphenate-limit-chars","hyphens","icon","image-orientation","image-rendering","image-resolution","ime-mode","initial-letter","initial-letter-align","inline-size","inset","inset-area","inset-block","inset-block-end","inset-block-start","inset-inline","inset-inline-end","inset-inline-start","isolation","justify-content","justify-items","justify-self","kerning","left","letter-spacing","lighting-color","line-break","line-height","line-height-step","list-style","list-style-image","list-style-position","list-style-type","margin","margin-block","margin-block-end","margin-block-start","margin-bottom","margin-inline","margin-inline-end","margin-inline-start","margin-left","margin-right","margin-top","margin-trim","marker","marker-end","marker-mid","marker-start","marks","mask","mask-border","mask-border-mode","mask-border-outset","mask-border-repeat","mask-border-slice","mask-border-source","mask-border-width","mask-clip","mask-composite","mask-image","mask-mode","mask-origin","mask-position","mask-repeat","mask-size","mask-type","masonry-auto-flow","math-depth","math-shift","math-style","max-block-size","max-height","max-inline-size","max-width","min-block-size","min-height","min-inline-size","min-width","mix-blend-mode","nav-down","nav-index","nav-left","nav-right","nav-up","none","normal","object-fit","object-position","offset","offset-anchor","offset-distance","offset-path","offset-position","offset-rotate","opacity","order","orphans","outline","outline-color","outline-offset","outline-style","outline-width","overflow","overflow-anchor","overflow-block","overflow-clip-margin","overflow-inline","overflow-wrap","overflow-x","overflow-y","overlay","overscroll-behavior","overscroll-behavior-block","overscroll-behavior-inline","overscroll-behavior-x","overscroll-behavior-y","padding","padding-block","padding-block-end","padding-block-start","padding-bottom","padding-inline","padding-inline-end","padding-inline-start","padding-left","padding-right","padding-top","page","page-break-after","page-break-before","page-break-inside","paint-order","pause","pause-after","pause-before","perspective","perspective-origin","place-content","place-items","place-self","pointer-events","position","position-anchor","position-visibility","print-color-adjust","quotes","r","resize","rest","rest-after","rest-before","right","rotate","row-gap","ruby-align","ruby-position","scale","scroll-behavior","scroll-margin","scroll-margin-block","scroll-margin-block-end","scroll-margin-block-start","scroll-margin-bottom","scroll-margin-inline","scroll-margin-inline-end","scroll-margin-inline-start","scroll-margin-left","scroll-margin-right","scroll-margin-top","scroll-padding","scroll-padding-block","scroll-padding-block-end","scroll-padding-block-start","scroll-padding-bottom","scroll-padding-inline","scroll-padding-inline-end","scroll-padding-inline-start","scroll-padding-left","scroll-padding-right","scroll-padding-top","scroll-snap-align","scroll-snap-stop","scroll-snap-type","scroll-timeline","scroll-timeline-axis","scroll-timeline-name","scrollbar-color","scrollbar-gutter","scrollbar-width","shape-image-threshold","shape-margin","shape-outside","shape-rendering","speak","speak-as","src","stop-color","stop-opacity","stroke","stroke-dasharray","stroke-dashoffset","stroke-linecap","stroke-linejoin","stroke-miterlimit","stroke-opacity","stroke-width","tab-size","table-layout","text-align","text-align-all","text-align-last","text-anchor","text-combine-upright","text-decoration","text-decoration-color","text-decoration-line","text-decoration-skip","text-decoration-skip-ink","text-decoration-style","text-decoration-thickness","text-emphasis","text-emphasis-color","text-emphasis-position","text-emphasis-style","text-indent","text-justify","text-orientation","text-overflow","text-rendering","text-shadow","text-size-adjust","text-transform","text-underline-offset","text-underline-position","text-wrap","text-wrap-mode","text-wrap-style","timeline-scope","top","touch-action","transform","transform-box","transform-origin","transform-style","transition","transition-behavior","transition-delay","transition-duration","transition-property","transition-timing-function","translate","unicode-bidi","unicode-range","user-modify","user-select","vector-effect","vertical-align","view-timeline","view-timeline-axis","view-timeline-inset","view-timeline-name","view-transition-name","visibility","voice-balance","voice-duration","voice-family","voice-pitch","voice-range","voice-rate","voice-stress","voice-volume","white-space","white-space-collapse","widows","width","will-change","word-break","word-spacing","word-wrap","writing-mode","x","y","z-index","zoom"].sort().reverse(),Be=De.concat(Ie).sort().reverse()
;var Fe="[0-9](_*[0-9])*",$e=`\\.(${Fe})`,ze="[0-9a-fA-F](_*[0-9a-fA-F])*",je={
className:"number",variants:[{
begin:`(\\b(${Fe})((${$e})|\\.)?|(${$e}))[eE][+-]?(${Fe})[fFdD]?\\b`},{
begin:`\\b(${Fe})((${$e})[fFdD]?\\b|\\.([fFdD]\\b)?)`},{
begin:`(${$e})[fFdD]?\\b`},{begin:`\\b(${Fe})[fFdD]\\b`},{
begin:`\\b0[xX]((${ze})\\.?|(${ze})?\\.(${ze}))[pP][+-]?(${Fe})[fFdD]?\\b`},{
begin:"\\b(0|[1-9](_*[0-9])*)[lL]?\\b"},{begin:`\\b0[xX](${ze})[lL]?\\b`},{
begin:"\\b0(_*[0-7])*[lL]?\\b"},{begin:"\\b0[bB][01](_*[01])*[lL]?\\b"}],
relevance:0};function Ue(e,n,t){return-1===t?"":e.replace(n,a=>Ue(e,n,t-1))}
var Pe=Object.freeze({__proto__:null,grmr_bash:e=>{const n=e.regex,t={},a={
begin:/\$\{/,end:/\}/,contains:["self",{begin:/:-/,contains:[t]}]}
;Object.assign(t,{className:"variable",variants:[{
begin:n.concat(/\$[\w\d#@][\w\d_]*/,"(?![\\w\\d])(?![$])")},a]});const i={
className:"subst",begin:/\$\(/,end:/\)/,contains:[e.BACKSLASH_ESCAPE]
},r=e.inherit(e.COMMENT(),{match:[/(^|\s)/,/#.*$/],scope:{2:"comment"}}),s={
begin:/<<-?\s*(?=\w+)/,starts:{contains:[e.END_SAME_AS_BEGIN({begin:/(\w+)/,
end:/(\w+)/,className:"string"})]}},o={className:"string",begin:/"/,end:/"/,
contains:[e.BACKSLASH_ESCAPE,t,i]};i.contains.push(o);const c={begin:/\$?\(\(/,
end:/\)\)/,contains:[{begin:/\d+#[0-9a-f]+/,className:"number"},e.NUMBER_MODE,t]
},l=e.SHEBANG({binary:"(fish|bash|zsh|sh|csh|ksh|tcsh|dash|scsh)",relevance:10
}),d={className:"function",begin:/\w[\w\d_]*\s*\(\s*\)\s*\{/,returnBegin:!0,
contains:[e.inherit(e.TITLE_MODE,{begin:/\w[\w\d_]*/})],relevance:0};return{
name:"Bash",aliases:["sh","zsh"],keywords:{$pattern:/\b[a-z][a-z0-9._-]+\b/,
keyword:["if","then","else","elif","fi","time","for","while","until","in","do","done","case","esac","coproc","function","select"],
literal:["true","false"],
built_in:["break","cd","continue","eval","exec","exit","export","getopts","hash","pwd","readonly","return","shift","test","times","trap","umask","unset","alias","bind","builtin","caller","command","declare","echo","enable","help","let","local","logout","mapfile","printf","read","readarray","source","sudo","type","typeset","ulimit","unalias","set","shopt","autoload","bg","bindkey","bye","cap","chdir","clone","comparguments","compcall","compctl","compdescribe","compfiles","compgroups","compquote","comptags","comptry","compvalues","dirs","disable","disown","echotc","echoti","emulate","fc","fg","float","functions","getcap","getln","history","integer","jobs","kill","limit","log","noglob","popd","print","pushd","pushln","rehash","sched","setcap","setopt","stat","suspend","ttyctl","unfunction","unhash","unlimit","unsetopt","vared","wait","whence","where","which","zcompile","zformat","zftp","zle","zmodload","zparseopts","zprof","zpty","zregexparse","zsocket","zstyle","ztcp","chcon","chgrp","chown","chmod","cp","dd","df","dir","dircolors","ln","ls","mkdir","mkfifo","mknod","mktemp","mv","realpath","rm","rmdir","shred","sync","touch","truncate","vdir","b2sum","base32","base64","cat","cksum","comm","csplit","cut","expand","fmt","fold","head","join","md5sum","nl","numfmt","od","paste","ptx","pr","sha1sum","sha224sum","sha256sum","sha384sum","sha512sum","shuf","sort","split","sum","tac","tail","tr","tsort","unexpand","uniq","wc","arch","basename","chroot","date","dirname","du","echo","env","expr","factor","groups","hostid","id","link","logname","nice","nohup","nproc","pathchk","pinky","printenv","printf","pwd","readlink","runcon","seq","sleep","stat","stdbuf","stty","tee","test","timeout","tty","uname","unlink","uptime","users","who","whoami","yes"]
},contains:[l,e.SHEBANG(),d,c,r,s,{match:/(\/[a-z._-]+)+/},o,{match:/\\"/},{
className:"string",begin:/'/,end:/'/},{match:/\\'/},t]}},grmr_c:e=>{
const n=e.regex,t=e.COMMENT("//","$",{contains:[{begin:/\\\n/}]
}),a="decltype\\(auto\\)",i="[a-zA-Z_]\\w*::",r="("+a+"|"+n.optional(i)+"[a-zA-Z_]\\w*"+n.optional("<[^<>]+>")+")",s={
className:"type",variants:[{begin:"\\b[a-z\\d_]*_t\\b"},{
match:n.concat(/\batomic_/,n.either("bool","char","schar","uchar","short","ushort","int","uint","long","ulong","llong","ullong","char16_t","char32_t","wchar_t","int_least8_t","uint_least8_t","int_least16_t","uint_least16_t","int_least32_t","uint_least32_t","int_least64_t","uint_least64_t","int_fast8_t","uint_fast8_t","int_fast16_t","uint_fast16_t","int_fast32_t","uint_fast32_t","int_fast64_t","uint_fast64_t","intptr_t","uintptr_t","size_t","ptrdiff_t","intmax_t","uintmax_t"),/\b/)
}]},o={className:"string",variants:[{begin:'(u8?|U|L)?"',end:'"',illegal:"\\n",
contains:[e.BACKSLASH_ESCAPE]},{
begin:"(u8?|U|L)?'(\\\\(x[0-9A-Fa-f]{2}|u[0-9A-Fa-f]{4,8}|[0-7]{3}|\\S)|.)",
end:"'",illegal:"."},e.END_SAME_AS_BEGIN({
begin:/(?:u8?|U|L)?R"([^()\\\s"]{0,16})\(/,end:/\)([^()\\\s"]{0,16})"/})]},c={
className:"number",variants:[{match:/\b(0b[01']+)/},{
match:/(-?)\b([\d']+(\.[\d']*)?|\.[\d']+)((ll|LL|l|L)(u|U)?|(u|U)(ll|LL|l|L)?|f|F|b|B)/
},{
match:/(-?)\b(0[xX][a-fA-F0-9]+(?:'[a-fA-F0-9]+)*(?:\.[a-fA-F0-9]*(?:'[a-fA-F0-9]*)*)?(?:[pP][-+]?[0-9]+)?(l|L)?(u|U)?)/
},{match:/(-?)\b\d+(?:'\d+)*(?:\.\d*(?:'\d*)*)?(?:[eE][-+]?\d+)?/}],relevance:0
},l={scope:"meta",begin:/#\s*include\b/,end:/$/,keywords:{keyword:"include"},
contains:[{begin:/\\\n/},o,{scope:"string",begin:/<.*?>/
},t,e.C_BLOCK_COMMENT_MODE]},d={className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma _Pragma ifdef ifndef elifdef elifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(o,{className:"string"
}),t,e.C_BLOCK_COMMENT_MODE]},g=[l,d],u={className:"title",
begin:n.optional(i)+e.IDENT_RE,relevance:0
},b=n.optional(i)+e.IDENT_RE+"\\s*\\(",m={
keyword:["asm","auto","break","case","continue","default","do","else","enum","extern","for","fortran","goto","if","inline","register","restrict","return","sizeof","typeof","typeof_unqual","struct","switch","typedef","union","volatile","while","_Alignas","_Alignof","_Atomic","_Generic","_Noreturn","_Static_assert","_Thread_local","alignas","alignof","noreturn","static_assert","thread_local","_Pragma"],
type:["float","double","signed","unsigned","int","short","long","char","void","_Bool","_BitInt","_Complex","_Imaginary","_Decimal32","_Decimal64","_Decimal96","_Decimal128","_Decimal64x","_Decimal128x","_Float16","_Float32","_Float64","_Float128","_Float32x","_Float64x","_Float128x","const","static","constexpr","complex","bool","imaginary"],
literal:"true false NULL",
built_in:"std string wstring cin cout cerr clog stdin stdout stderr stringstream istringstream ostringstream auto_ptr deque list queue stack vector map set pair bitset multiset multimap unordered_set unordered_map unordered_multiset unordered_multimap priority_queue make_pair array shared_ptr abort terminate abs acos asin atan2 atan calloc ceil cosh cos exit exp fabs floor fmod fprintf fputs free frexp fscanf future isalnum isalpha iscntrl isdigit isgraph islower isprint ispunct isspace isupper isxdigit tolower toupper labs ldexp log10 log malloc realloc memchr memcmp memcpy memset modf pow printf putchar puts scanf sinh sin snprintf sprintf sqrt sscanf strcat strchr strcmp strcpy strcspn strlen strncat strncmp strncpy strpbrk strrchr strspn strstr tanh tan vfprintf vprintf vsprintf endl initializer_list unique_ptr"
},p=[...g,s,t,e.C_BLOCK_COMMENT_MODE,c,o],_={variants:[{begin:/=/,end:/;/},{
begin:/\(/,end:/\)/},{beginKeywords:"new throw return else",end:/;/}],
keywords:m,contains:p.concat([{begin:/\(/,end:/\)/,keywords:m,
contains:p.concat(["self"]),relevance:0}]),relevance:0},h={
begin:"("+r+"[\\*&\\s]+){1,12}"+b,returnBegin:!0,end:/[{;=]/,excludeEnd:!0,
keywords:m,illegal:/[^\w\s\*&:<>.]/,contains:[{begin:a,keywords:m,relevance:0},{
begin:b,returnBegin:!0,contains:[e.inherit(u,{className:"title.function"})],
relevance:0},{relevance:0,match:/,/},{className:"params",begin:/\(/,end:/\)/,
keywords:m,relevance:0,contains:[t,e.C_BLOCK_COMMENT_MODE,o,c,s,{begin:/\(/,
end:/\)/,keywords:m,relevance:0,contains:["self",t,e.C_BLOCK_COMMENT_MODE,o,c,s]
}]},s,t,e.C_BLOCK_COMMENT_MODE,...g]};return{name:"C",aliases:["h"],keywords:m,
disableAutodetect:!0,illegal:"</",contains:[].concat(_,h,p,[...g,{
begin:e.IDENT_RE+"::",keywords:m},{className:"class",
beginKeywords:"enum class struct union",end:/[{;:<>=]/,contains:[{
beginKeywords:"final class struct"},e.TITLE_MODE]}]),exports:{preprocessor:d,
strings:o,keywords:m}}},grmr_cpp:e=>{const n=e.regex,t=e.COMMENT("//","$",{
contains:[{begin:/\\\n/}]
}),a="decltype\\(auto\\)",i="[a-zA-Z_]\\w*::",r="(?!struct)("+a+"|"+n.optional(i)+"[a-zA-Z_]\\w*"+n.optional("<[^<>]+>")+")",s={
className:"type",begin:"\\b[a-z\\d_]*_t\\b"},o={className:"string",variants:[{
begin:'(u8?|U|L)?"',end:'"',illegal:"\\n",contains:[e.BACKSLASH_ESCAPE]},{
begin:"(u8?|U|L)?'(\\\\(x[0-9A-Fa-f]{2}|u[0-9A-Fa-f]{4,8}|[0-7]{3}|\\S)|.)",
end:"'",illegal:"."},e.END_SAME_AS_BEGIN({
begin:/(?:u8?|U|L)?R"([^()\\\s"]{0,16})\(/,end:/\)([^()\\\s"]{0,16})"/})]},c={
className:"number",variants:[{
begin:"[+-]?(?:(?:\\b[0-9](?:'?[0-9])*\\.(?:[0-9](?:'?[0-9])*)?|\\.[0-9](?:'?[0-9])*)(?:[Ee][+-]?[0-9](?:'?[0-9])*)?|\\b[0-9](?:'?[0-9])*[Ee][+-]?[0-9](?:'?[0-9])*|\\b0[Xx](?:[0-9A-Fa-f](?:'?[0-9A-Fa-f])*(?:\\.(?:[0-9A-Fa-f](?:'?[0-9A-Fa-f])*)?)?|\\.[0-9A-Fa-f](?:'?[0-9A-Fa-f])*)[Pp][+-]?[0-9](?:'?[0-9])*)(?:[Ff](?:16|32|64|128)?|(BF|bf)16|[Ll]|)"
},{
begin:"[+-]?\\b(?:0[Bb][01](?:'?[01])*|0[Xx][0-9A-Fa-f](?:'?[0-9A-Fa-f])*|0(?:'?[0-7])*|[1-9](?:'?[0-9])*)(?:[Uu](?:LL?|ll?)|[Uu][Zz]?|(?:LL?|ll?)[Uu]?|[Zz][Uu]|)"
}],relevance:0},l=[{scope:"meta",begin:/#\s*include\b/,end:/$/,keywords:{
keyword:"include"},contains:[{begin:/\\\n/},o,{scope:"string",begin:/<.*?>/
},t,e.C_BLOCK_COMMENT_MODE]},{className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma _Pragma ifdef ifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(o,{className:"string"
}),t,e.C_BLOCK_COMMENT_MODE]}],d={className:"title",
begin:n.optional(i)+e.IDENT_RE,relevance:0
},g=n.optional(i)+e.IDENT_RE+"\\s*\\(",u=["alignas","alignof","and","and_eq","asm","atomic_cancel","atomic_commit","atomic_noexcept","auto","bitand","bitor","break","case","catch","class","co_await","co_return","co_yield","compl","concept","const_cast|10","consteval","constexpr","constinit","continue","decltype","default","delete","do","dynamic_cast|10","else","enum","explicit","export","extern","false","final","for","friend","goto","if","import","inline","module","mutable","namespace","new","noexcept","not","not_eq","nullptr","operator","or","or_eq","override","private","protected","public","reflexpr","register","reinterpret_cast|10","requires","return","sizeof","static_assert","static_cast|10","struct","switch","synchronized","template","this","thread_local","throw","transaction_safe","transaction_safe_dynamic","true","try","typedef","typeid","typename","union","using","virtual","volatile","while","xor","xor_eq"],b={
type:["bool","char","char16_t","char32_t","char8_t","double","float","int","long","short","void","wchar_t","unsigned","signed","const","static"],
keyword:u,literal:["NULL","false","nullopt","nullptr","true"],
built_in:["_Pragma"],
_type_hints:["any","auto_ptr","barrier","binary_semaphore","bitset","complex","condition_variable","condition_variable_any","counting_semaphore","deque","false_type","flat_map","flat_set","future","imaginary","initializer_list","istringstream","jthread","latch","lock_guard","multimap","multiset","mutex","optional","ostringstream","packaged_task","pair","promise","priority_queue","queue","recursive_mutex","recursive_timed_mutex","scoped_lock","set","shared_future","shared_lock","shared_mutex","shared_timed_mutex","shared_ptr","stack","string_view","stringstream","timed_mutex","thread","true_type","tuple","unique_lock","unique_ptr","unordered_map","unordered_multimap","unordered_multiset","unordered_set","variant","vector","weak_ptr","wstring","wstring_view"]
},m={className:"function.dispatch",relevance:0,keywords:{
_hint:["abort","abs","acos","apply","as_const","asin","atan","atan2","calloc","ceil","cerr","cin","clog","cos","cosh","cout","declval","endl","exchange","exit","exp","fabs","floor","fmod","forward","fprintf","fputs","free","frexp","fscanf","future","invoke","isalnum","isalpha","iscntrl","isdigit","isgraph","islower","isprint","ispunct","isspace","isupper","isxdigit","labs","launder","ldexp","log","log10","make_pair","make_shared","make_shared_for_overwrite","make_tuple","make_unique","malloc","memchr","memcmp","memcpy","memset","modf","move","pow","printf","putchar","puts","realloc","scanf","sin","sinh","snprintf","sprintf","sqrt","sscanf","std","stderr","stdin","stdout","strcat","strchr","strcmp","strcpy","strcspn","strlen","strncat","strncmp","strncpy","strpbrk","strrchr","strspn","strstr","swap","tan","tanh","terminate","to_underlying","tolower","toupper","vfprintf","visit","vprintf","vsprintf"]
},
begin:n.concat(/\b/,`(?!${u.join("|")})`,e.IDENT_RE,n.lookahead(/(<[^<>]+>|)\s*\(/))
},p=[m,...l,s,t,e.C_BLOCK_COMMENT_MODE,c,o],_={variants:[{begin:/=/,end:/;/},{
begin:/\(/,end:/\)/},{beginKeywords:"new throw return else",end:/;/}],
keywords:b,contains:p.concat([{begin:/\(/,end:/\)/,keywords:b,
contains:p.concat(["self"]),relevance:0}]),relevance:0},h={className:"function",
begin:"("+r+"[\\*&\\s]+){1,12}"+g,returnBegin:!0,end:/[{;=]/,excludeEnd:!0,
keywords:b,illegal:/[^\w\s\*&:<>.]/,contains:[{begin:a,keywords:b,relevance:0},{
begin:g,returnBegin:!0,contains:[d],relevance:0},{begin:/::/,relevance:0},{
begin:/:/,endsWithParent:!0,contains:[o,c]},{relevance:0,match:/,/},{
className:"params",begin:/\(/,end:/\)/,keywords:b,relevance:0,
contains:[t,e.C_BLOCK_COMMENT_MODE,o,c,s,{begin:/\(/,end:/\)/,keywords:b,
relevance:0,contains:["self",t,e.C_BLOCK_COMMENT_MODE,o,c,s]}]
},s,t,e.C_BLOCK_COMMENT_MODE,...l]};return{name:"C++",
aliases:["cc","c++","h++","hpp","hh","hxx","cxx"],keywords:b,illegal:"</",
classNameAliases:{"function.dispatch":"built_in"},
contains:[].concat(_,h,m,p,[...l,{
begin:"\\b(deque|list|queue|priority_queue|pair|stack|vector|map|set|bitset|multiset|multimap|unordered_map|unordered_set|unordered_multiset|unordered_multimap|array|tuple|optional|variant|function|flat_map|flat_set)\\s*<(?!<)",
end:">",keywords:b,contains:["self",s]},{begin:e.IDENT_RE+"::",keywords:b},{
match:[/\b(?:enum(?:\s+(?:class|struct))?|class|struct|union)/,/\s+/,/\w+/],
className:{1:"keyword",3:"title.class"}}])}},grmr_csharp:e=>{const n={
keyword:["abstract","as","base","break","case","catch","class","const","continue","do","else","event","explicit","extern","finally","fixed","for","foreach","goto","if","implicit","in","interface","internal","is","lock","namespace","new","operator","out","override","params","private","protected","public","readonly","record","ref","return","scoped","sealed","sizeof","stackalloc","static","struct","switch","this","throw","try","typeof","unchecked","unsafe","using","virtual","void","volatile","while"].concat(["add","alias","and","ascending","args","async","await","by","descending","dynamic","equals","file","from","get","global","group","init","into","join","let","nameof","not","notnull","on","or","orderby","partial","record","remove","required","scoped","select","set","unmanaged","value|0","var","when","where","with","yield"]),
built_in:["bool","byte","char","decimal","delegate","double","dynamic","enum","float","int","long","nint","nuint","object","sbyte","short","string","ulong","uint","ushort"],
literal:["default","false","null","true"]},t=e.inherit(e.TITLE_MODE,{
begin:"[a-zA-Z](\\.?\\w)*"}),a="\\d(_*\\d)*",i="([uU][lL]?|[lL][uU]?)?",r={
className:"number",variants:[{begin:"\\b0[bB]_*[01](_*[01])*"+i},{
begin:"(-?)\\b0[xX]_*[a-fA-F0-9](_*[a-fA-F0-9])*"+i},{
begin:"(-?)(\\b"+a+"(\\.("+a+")?)?|\\."+a+")([eE][-+]?"+a+")?([fFdDmM]|[uU][lL]?|[lL][uU]?)?"
}],relevance:0},s={className:"string",begin:'@"',end:'"',contains:[{begin:'""'}]
},o=e.inherit(s,{illegal:/\n/}),c={className:"subst",begin:/\{/,end:/\}/,
keywords:n},l=e.inherit(c,{illegal:/\n/}),d={className:"string",begin:/\$"/,
end:'"',illegal:/\n/,contains:[{begin:/\{\{/},{begin:/\}\}/
},e.BACKSLASH_ESCAPE,l]},g={className:"string",begin:/\$@"/,end:'"',contains:[{
begin:/\{\{/},{begin:/\}\}/},{begin:'""'},c]},u=e.inherit(g,{illegal:/\n/,
contains:[{begin:/\{\{/},{begin:/\}\}/},{begin:'""'},l]})
;c.contains=[g,d,s,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,r,e.C_BLOCK_COMMENT_MODE],
l.contains=[u,d,o,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,r,e.inherit(e.C_BLOCK_COMMENT_MODE,{
illegal:/\n/})];const b={variants:[{className:"string",
begin:/"""("*)(?!")(.|\n)*?"""\1/,relevance:1
},g,d,s,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]},m={begin:"<",end:">",
contains:[{beginKeywords:"in out"},t]
},p=e.IDENT_RE+"(<"+e.IDENT_RE+"(\\s*,\\s*"+e.IDENT_RE+")*>)?(\\[\\])?",_={
begin:"@"+e.IDENT_RE,relevance:0};return{name:"C#",aliases:["cs","c#"],
keywords:n,illegal:/::/,contains:[e.COMMENT("///","$",{returnBegin:!0,
contains:[{className:"doctag",variants:[{begin:"///",relevance:0},{
begin:"\x3c!--|--\x3e"},{begin:"</?",end:">"}]}]
}),e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{className:"meta",begin:"#",
end:"$",keywords:{
keyword:"if else elif endif define undef warning error line region endregion pragma checksum"
}},b,r,{beginKeywords:"class interface",relevance:0,end:/[{;=]/,
illegal:/[^\s:,]/,contains:[{beginKeywords:"where class"
},t,m,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{beginKeywords:"namespace",
relevance:0,end:/[{;=]/,illegal:/[^\s:]/,
contains:[t,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{
beginKeywords:"record",relevance:0,end:/[{;=]/,illegal:/[^\s:]/,
contains:[t,m,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{className:"meta",
begin:"^\\s*\\[(?=[\\w])",excludeBegin:!0,end:"\\]",excludeEnd:!0,contains:[{
className:"string",begin:/"/,end:/"/}]},{
beginKeywords:"new return throw await else",relevance:0},{className:"function",
begin:"("+p+"\\s+)+"+e.IDENT_RE+"\\s*(<[^=]+>\\s*)?\\(",returnBegin:!0,
end:/\s*[{;=]/,excludeEnd:!0,keywords:n,contains:[{
beginKeywords:"public private protected static internal protected abstract async extern override unsafe virtual new sealed partial",
relevance:0},{begin:e.IDENT_RE+"\\s*(<[^=]+>\\s*)?\\(",returnBegin:!0,
contains:[e.TITLE_MODE,m],relevance:0},{match:/\(\)/},{className:"params",
begin:/\(/,end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:n,relevance:0,
contains:[b,r,e.C_BLOCK_COMMENT_MODE]
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},_]}},grmr_css:e=>{
const n=e.regex,t=Ce(e),a=[e.APOS_STRING_MODE,e.QUOTE_STRING_MODE];return{
name:"CSS",case_insensitive:!0,illegal:/[=|'\$]/,keywords:{
keyframePosition:"from to"},classNameAliases:{keyframePosition:"selector-tag"},
contains:[t.BLOCK_COMMENT,{begin:/-(webkit|moz|ms|o)-(?=[a-z])/
},t.CSS_NUMBER_MODE,{className:"selector-id",begin:/#[A-Za-z0-9_-]+/,relevance:0
},{className:"selector-class",begin:"\\.[a-zA-Z-][a-zA-Z0-9_-]*",relevance:0
},t.ATTRIBUTE_SELECTOR_MODE,{className:"selector-pseudo",variants:[{
begin:":("+De.join("|")+")"},{begin:":(:)?("+Ie.join("|")+")"}]
},t.CSS_VARIABLE,{className:"attribute",begin:"\\b("+Le.join("|")+")\\b"},{
begin:/:/,end:/[;}{]/,
contains:[t.BLOCK_COMMENT,t.HEXCOLOR,t.IMPORTANT,t.CSS_NUMBER_MODE,t.UNICODE_RANGE,...a,{
begin:/(url|data-uri)\(/,end:/\)/,relevance:0,keywords:{built_in:"url data-uri"
},contains:[...a,{className:"string",begin:/[^)]/,endsWithParent:!0,
excludeEnd:!0}]},t.FUNCTION_DISPATCH]},{begin:n.lookahead(/@/),end:"[{;]",
relevance:0,illegal:/:/,contains:[{className:"keyword",begin:/@-?\w[\w]*(-\w+)*/
},{begin:/\s/,endsWithParent:!0,excludeEnd:!0,relevance:0,keywords:{
$pattern:/[a-z-]+/,keyword:"and or not only",attribute:Re.join(" ")},contains:[{
begin:/[a-z-]+(?=:)/,className:"attribute"},...a,t.CSS_NUMBER_MODE]}]},{
className:"selector-tag",begin:"\\b("+Te.join("|")+")\\b"}]}},grmr_diff:e=>{
const n=e.regex;return{name:"Diff",aliases:["patch"],contains:[{
className:"meta",relevance:10,
match:n.either(/^@@ +-\d+,\d+ +\+\d+,\d+ +@@/,/^@@ +-\d+ +\+\d+,\d+ +@@/,/^@@ +-\d+,\d+ +\+\d+ +@@/,/^@@ +-\d+ +\+\d+ +@@/,/^\*\*\* +\d+,\d+ +\*\*\*\*$/,/^--- +\d+,\d+ +----$/)
},{className:"comment",variants:[{
begin:n.either(/Index: /,/^index/,/={3,}/,/^-{3}/,/^\*{3} /,/^\+{3}/,/^diff --git/),
end:/$/},{match:/^\*{15}$/}]},{className:"addition",begin:/^\+/,end:/$/},{
className:"deletion",begin:/^-/,end:/$/},{className:"addition",begin:/^!/,
end:/$/}]}},grmr_go:e=>{const n={
keyword:["break","case","chan","const","continue","default","defer","else","fallthrough","for","func","go","goto","if","import","interface","map","package","range","return","select","struct","switch","type","var"],
type:["bool","byte","complex64","complex128","error","float32","float64","int8","int16","int32","int64","string","uint8","uint16","uint32","uint64","int","uint","uintptr","rune"],
literal:["true","false","iota","nil"],
built_in:["append","cap","close","complex","copy","imag","len","make","new","panic","print","println","real","recover","delete"]
};return{name:"Go",aliases:["golang"],keywords:n,illegal:"</",
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{className:"string",
variants:[e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,{begin:"`",end:"`"}]},{
className:"number",variants:[{
match:/-?\b0[xX]\.[a-fA-F0-9](_?[a-fA-F0-9])*[pP][+-]?\d(_?\d)*i?/,relevance:0
},{
match:/-?\b0[xX](_?[a-fA-F0-9])+((\.([a-fA-F0-9](_?[a-fA-F0-9])*)?)?[pP][+-]?\d(_?\d)*)?i?/,
relevance:0},{match:/-?\b0[oO](_?[0-7])*i?/,relevance:0},{
match:/-?\b0[bB](_?[01])*i?/,relevance:0},{
match:/-?\.\d(_?\d)*([eE][+-]?\d(_?\d)*)?i?/,relevance:0},{
match:/-?\b\d(_?\d)*(\.(\d(_?\d)*)?)?([eE][+-]?\d(_?\d)*)?i?/,relevance:0}]},{
begin:/:=/},{className:"function",beginKeywords:"func",end:"\\s*(\\{|$)",
excludeEnd:!0,contains:[e.TITLE_MODE,{className:"params",begin:/\(/,end:/\)/,
endsParent:!0,keywords:n,illegal:/["']/}]}]}},grmr_graphql:e=>{const n=e.regex
;return{name:"GraphQL",aliases:["gql"],case_insensitive:!0,disableAutodetect:!1,
keywords:{
keyword:["query","mutation","subscription","type","input","schema","directive","interface","union","scalar","fragment","enum","on"],
literal:["true","false","null"]},
contains:[e.HASH_COMMENT_MODE,e.QUOTE_STRING_MODE,e.NUMBER_MODE,{
scope:"punctuation",match:/[.]{3}/,relevance:0},{scope:"punctuation",
begin:/[\!\(\)\:\=\[\]\{\|\}]{1}/,relevance:0},{scope:"variable",begin:/\$/,
end:/\W/,excludeEnd:!0,relevance:0},{scope:"meta",match:/@\w+/,excludeEnd:!0},{
scope:"symbol",begin:n.concat(/[_A-Za-z][_0-9A-Za-z]*/,n.lookahead(/\s*:/)),
relevance:0}],illegal:[/[;<']/,/BEGIN/]}},grmr_ini:e=>{const n=e.regex,t={
className:"number",relevance:0,variants:[{begin:/([+-]+)?[\d]+_[\d_]+/},{
begin:e.NUMBER_RE}]},a=e.COMMENT();a.variants=[{begin:/;/,end:/$/},{begin:/#/,
end:/$/}];const i={className:"variable",variants:[{begin:/\$[\w\d"][\w\d_]*/},{
begin:/\$\{(.*?)\}/}]},r={className:"literal",
begin:/\bon|off|true|false|yes|no\b/},s={className:"string",
contains:[e.BACKSLASH_ESCAPE],variants:[{begin:"'''",end:"'''",relevance:10},{
begin:'"""',end:'"""',relevance:10},{begin:'"',end:'"'},{begin:"'",end:"'"}]
},o={begin:/\[/,end:/\]/,contains:[a,r,i,s,t,"self"],relevance:0
},c=n.either(/[A-Za-z0-9_-]+/,/"(\\"|[^"])*"/,/'[^']*'/);return{
name:"TOML, also INI",aliases:["toml"],case_insensitive:!0,illegal:/\S/,
contains:[a,{className:"section",begin:/\[+/,end:/\]+/},{
begin:n.concat(c,"(\\s*\\.\\s*",c,")*",n.lookahead(/\s*=\s*[^#\s]/)),
className:"attr",starts:{end:/$/,contains:[a,o,r,i,s,t]}}]}},grmr_java:e=>{
const n=e.regex,t="[\xc0-\u02b8a-zA-Z_$][\xc0-\u02b8a-zA-Z_$0-9]*",a="(?:(?:\\s*\\[\\s*])+)?",i=t+"<@@@>"+a,r="(?:\\?(?:\\s+(?:extends|super)\\s+"+i+")?|"+i+")",s=Ue("(?:\\s*<\\s*"+r+"(?:\\s*,\\s*"+r+")*\\s*>)?",/<@@@>/g,2),o={
keyword:["synchronized","abstract","private","var","static","if","const ","for","while","strictfp","finally","protected","import","native","final","void","enum","else","break","transient","catch","instanceof","volatile","case","assert","package","default","public","try","switch","continue","throws","protected","public","private","module","requires","exports","do","sealed","yield","permits","goto","when"],
literal:["false","true","null"],
type:["char","boolean","long","float","int","byte","short","double"],
built_in:["super","this"]},c={className:"meta",begin:"@"+t,contains:[{
begin:/\(/,end:/\)/,contains:["self"]}]},l={className:"params",begin:/\(/,
end:/\)/,keywords:o,relevance:0,contains:[e.C_BLOCK_COMMENT_MODE],endsParent:!0}
;return{name:"Java",aliases:["jsp"],keywords:o,illegal:/<\/|#/,
contains:[e.COMMENT("/\\*\\*","\\*/",{relevance:0,contains:[{begin:/\w+@/,
relevance:0},{className:"doctag",begin:"@[A-Za-z]+"}]}),{
begin:/import java\.[a-z]+\./,keywords:"import",relevance:2
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,{begin:/"""/,end:/"""/,
className:"string",contains:[e.BACKSLASH_ESCAPE]
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,{
match:[/\b(?:class|interface|enum|extends|implements|new)/,/\s+/,t],className:{
1:"keyword",3:"title.class"}},{match:/non-sealed/,scope:"keyword"},{
beginKeywords:"new throw return else yield assert",relevance:0},{
begin:[t,n.concat(s,a,/\s+/),t,a,/\s*/,/=(?!=)/],className:{1:"type",
3:"variable",6:"operator"}},{begin:[/record/,/\s+/,t],className:{1:"keyword",
3:"title.class"},contains:[l,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{
begin:[t,n.concat(s,a,/\s+/),t,/\s*(?=\()/],className:{1:"type",
3:"title.function"},keywords:o,contains:[{className:"params",begin:/\(/,
end:/\)/,keywords:o,relevance:0,
contains:[c,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,je,e.C_BLOCK_COMMENT_MODE]
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},je,c]}},grmr_javascript:ge,
grmr_json:e=>{const n=["true","false","null"],t={scope:"literal",
beginKeywords:n.join(" ")};return{name:"JSON",aliases:["jsonc","json5"],
keywords:{literal:n},contains:[{className:"attr",
begin:/(("(\\.|[^\\"\r\n])*")|('(\\.|[^\\'\r\n])*'))(?=\s*:)/,relevance:1.01},{
match:/[{}[\],:]/,className:"punctuation",relevance:0
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,t,ae,e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE],
illegal:"\\S"}},grmr_kotlin:e=>{const n={
keyword:"abstract as val var vararg get set class object open private protected public noinline crossinline dynamic final enum if else do while for when throw try catch finally import package is in fun override companion reified inline lateinit init interface annotation data sealed internal infix operator out by constructor super tailrec where const inner suspend typealias external expect actual",
built_in:"Byte Short Char Int Long Boolean Float Double Void Unit Nothing",
literal:"true false null"},t={className:"symbol",begin:e.UNDERSCORE_IDENT_RE+"@"
},a={className:"subst",begin:/\$\{/,end:/\}/,contains:[e.C_NUMBER_MODE]},i={
className:"variable",begin:"\\$"+e.UNDERSCORE_IDENT_RE},r={className:"string",
variants:[{begin:'"""',end:'"""(?=[^"])',contains:[i,a]},{begin:"'",end:"'",
illegal:/\n/,contains:[e.BACKSLASH_ESCAPE]},{begin:'"',end:'"',illegal:/\n/,
contains:[e.BACKSLASH_ESCAPE,i,a]}]};a.contains.push(r);const s={
className:"meta",
begin:"@(?:file|property|field|get|set|receiver|param|setparam|delegate)\\s*:(?:\\s*"+e.UNDERSCORE_IDENT_RE+")?"
},o={className:"meta",begin:"@"+e.UNDERSCORE_IDENT_RE,contains:[{begin:/\(/,
end:/\)/,contains:[e.inherit(r,{className:"string"}),"self"]}]
},c=je,l=e.COMMENT("/\\*","\\*/",{contains:[e.C_BLOCK_COMMENT_MODE]}),d={
variants:[{className:"type",begin:e.UNDERSCORE_IDENT_RE},{begin:/\(/,end:/\)/,
contains:[]}]},g=d;return g.variants[1].contains=[d],d.variants[1].contains=[g],
{name:"Kotlin",aliases:["kt","kts","ktm","ktx"],keywords:n,
contains:[e.COMMENT("/\\*\\*","\\*/",{relevance:0,contains:[{className:"doctag",
begin:"@[A-Za-z]+"}]}),e.C_LINE_COMMENT_MODE,l,{className:"keyword",
begin:/\b(break|continue|return|this)\b/,starts:{contains:[{className:"symbol",
begin:/@\w+/}]}},t,s,o,{className:"function",beginKeywords:"fun",end:"[(]|$",
returnBegin:!0,excludeEnd:!0,keywords:n,relevance:5,contains:[{
begin:e.UNDERSCORE_IDENT_RE+"\\s*\\(",returnBegin:!0,relevance:0,
contains:[e.UNDERSCORE_TITLE_MODE]},{className:"type",begin:/</,end:/>/,
keywords:"reified",relevance:0},{className:"params",begin:/\(/,end:/\)/,
endsParent:!0,keywords:n,relevance:0,contains:[{begin:/:/,end:/[=,\/]/,
endsWithParent:!0,contains:[d,e.C_LINE_COMMENT_MODE,l],relevance:0
},e.C_LINE_COMMENT_MODE,l,s,o,r,e.C_NUMBER_MODE]},l]},{
begin:[/class|interface|trait/,/\s+/,e.UNDERSCORE_IDENT_RE],beginScope:{
3:"title.class"},keywords:"class interface trait",end:/[:\{(]|$/,excludeEnd:!0,
illegal:"extends implements",contains:[{
beginKeywords:"public protected internal private constructor"
},e.UNDERSCORE_TITLE_MODE,{className:"type",begin:/</,end:/>/,excludeBegin:!0,
excludeEnd:!0,relevance:0},{className:"type",begin:/[,:]\s*/,end:/[<\(,){\s]|$/,
excludeBegin:!0,returnEnd:!0},s,o]},r,{className:"meta",begin:"^#!/usr/bin/env",
end:"$",illegal:"\n"},c]}},grmr_less:e=>{
const n=Ce(e),t=Be,a="[\\w-]+",i="("+a+"|@\\{"+a+"\\})",r=[],s=[],o=e=>({
className:"string",begin:"~?"+e+".*?"+e}),c=(e,n,t)=>({className:e,begin:n,
relevance:t}),l={$pattern:/[a-z-]+/,keyword:"and or not only",
attribute:Re.join(" ")},d={begin:"\\(",end:"\\)",contains:s,keywords:l,
relevance:0}
;s.push(e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,o("'"),o('"'),n.CSS_NUMBER_MODE,{
begin:"(url|data-uri)\\(",starts:{className:"string",end:"[\\)\\n]",
excludeEnd:!0}
},n.UNICODE_RANGE,n.HEXCOLOR,d,c("variable","@@?"+a,10),c("variable","@\\{"+a+"\\}"),c("built_in","~?`[^`]*?`"),{
className:"attribute",begin:a+"\\s*:",end:":",returnBegin:!0,excludeEnd:!0
},n.IMPORTANT,{beginKeywords:"and not"},n.FUNCTION_DISPATCH);const g=s.concat({
begin:/\{/,end:/\}/,contains:r}),u={beginKeywords:"when",endsWithParent:!0,
contains:[{beginKeywords:"and not"}].concat(s)},b={begin:i+"\\s*:",
returnBegin:!0,end:/[;}]/,relevance:0,contains:[{begin:/-(webkit|moz|ms|o)-/
},n.CSS_VARIABLE,{className:"attribute",begin:"\\b("+Le.join("|")+")\\b",
end:/(?=:)/,starts:{endsWithParent:!0,illegal:"[<=$]",relevance:0,contains:s}}]
},m={className:"keyword",
begin:"@(import|media|charset|font-face|(-[a-z]+-)?keyframes|supports|document|namespace|page|viewport|host)\\b",
starts:{end:"[;{}]",keywords:l,returnEnd:!0,contains:s,relevance:0}},p={
className:"variable",variants:[{begin:"@"+a+"\\s*:",relevance:15},{begin:"@"+a
}],starts:{end:"[;}]",returnEnd:!0,contains:g}},_={variants:[{
begin:"[\\.#:&\\[>]",end:"[;{}]"},{begin:i,end:/\{/}],returnBegin:!0,
returnEnd:!0,illegal:"[<='$\"]",relevance:0,
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,u,c("keyword","all\\b"),c("variable","@\\{"+a+"\\}"),{
begin:"\\b("+Te.join("|")+")\\b",className:"selector-tag"
},n.CSS_NUMBER_MODE,c("selector-tag",i,0),c("selector-id","#"+i),c("selector-class","\\."+i,0),c("selector-tag","&",0),n.ATTRIBUTE_SELECTOR_MODE,{
className:"selector-pseudo",begin:":("+De.join("|")+")"},{
className:"selector-pseudo",begin:":(:)?("+Ie.join("|")+")"},{begin:/\(/,
end:/\)/,relevance:0,contains:g},{begin:"!important"},n.FUNCTION_DISPATCH]},h={
begin:a+":(:)?"+`(${t.join("|")})`,returnBegin:!0,contains:[_]}
;return r.push(e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,m,p,h,b,_,u,n.FUNCTION_DISPATCH),
{name:"Less",case_insensitive:!0,illegal:"[=>'/<($\"]",contains:r}},
grmr_lua:e=>{const n="\\[=*\\[",t="\\]=*\\]",a={begin:n,end:t,contains:["self"]
},i=[e.COMMENT("--(?!"+n+")","$"),e.COMMENT("--"+n,t,{contains:[a],relevance:10
})];return{name:"Lua",aliases:["pluto"],keywords:{
$pattern:e.UNDERSCORE_IDENT_RE,literal:"true false nil",
keyword:"and break do else elseif end for goto if in local global not or repeat return then until while",
built_in:"_G _ENV _VERSION __index __newindex __mode __call __metatable __tostring __len __gc __add __sub __mul __div __mod __pow __concat __unm __eq __lt __le assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawset require select setfenv setmetatable tonumber tostring type unpack xpcall arg self coroutine resume yield status wrap create running debug getupvalue debug sethook getmetatable gethook setmetatable setlocal traceback setfenv getinfo setupvalue getlocal getregistry getfenv io lines write close flush open output type read stderr stdin input stdout popen tmpfile math log max acos huge ldexp pi cos tanh pow deg tan cosh sinh random randomseed frexp ceil floor rad abs sqrt modf asin min mod fmod log10 atan2 exp sin atan os exit setlocale date getenv difftime remove time clock tmpname rename execute package preload loadlib loaded loaders cpath config path seeall string sub upper len gfind rep find match char dump gmatch reverse byte format gsub lower table setn insert getn foreachi maxn foreach concat sort remove"
},contains:i.concat([{className:"function",beginKeywords:"function",end:"\\)",
contains:[e.inherit(e.TITLE_MODE,{
begin:"([_a-zA-Z]\\w*\\.)*([_a-zA-Z]\\w*:)?[_a-zA-Z]\\w*"}),{className:"params",
begin:"\\(",endsWithParent:!0,contains:i}].concat(i)
},e.C_NUMBER_MODE,e.APOS_STRING_MODE,e.QUOTE_STRING_MODE,{className:"string",
begin:n,end:t,contains:[a],relevance:5}])}},grmr_makefile:e=>{const n={
className:"variable",variants:[{begin:"\\$\\("+e.UNDERSCORE_IDENT_RE+"\\)",
contains:[e.BACKSLASH_ESCAPE]},{begin:/\$[@%<?\^\+\*]/}]},t={className:"string",
begin:/"/,end:/"/,contains:[e.BACKSLASH_ESCAPE,n]},a={className:"variable",
begin:/\$\([\w-]+\s/,end:/\)/,keywords:{
built_in:"subst patsubst strip findstring filter filter-out sort word wordlist firstword lastword dir notdir suffix basename addsuffix addprefix join wildcard realpath abspath error warning shell origin flavor foreach if or and call eval file value"
},contains:[n,t]},i={begin:"^"+e.UNDERSCORE_IDENT_RE+"\\s*(?=[:+?]?=)"},r={
className:"section",begin:/^[^\s]+:/,end:/$/,contains:[n]};return{
name:"Makefile",aliases:["mk","mak","make"],keywords:{$pattern:/[\w-]+/,
keyword:"define endef undefine ifdef ifndef ifeq ifneq else endif include -include sinclude override export unexport private vpath"
},contains:[e.HASH_COMMENT_MODE,n,t,a,i,{className:"meta",begin:/^\.PHONY:/,
end:/$/,keywords:{$pattern:/[\.\w]+/,keyword:".PHONY"}},r]}},grmr_markdown:e=>{
const n={begin:/<\/?[A-Za-z_]/,end:">",subLanguage:"xml",relevance:0},t={
variants:[{begin:/\[.+?\]\[.*?\]/,relevance:0},{
begin:/\[.+?\]\(((data|javascript|mailto):|(?:http|ftp)s?:\/\/).*?\)/,
relevance:2},{
begin:e.regex.concat(/\[.+?\]\(/,/[A-Za-z][A-Za-z0-9+.-]*/,/:\/\/.*?\)/),
relevance:2},{begin:/\[.+?\]\([./?&#].*?\)/,relevance:1},{
begin:/\[.*?\]\(.*?\)/,relevance:0}],returnBegin:!0,contains:[{match:/\[(?=\])/
},{className:"string",relevance:0,begin:"\\[",end:"\\]",excludeBegin:!0,
returnEnd:!0},{className:"link",relevance:0,begin:"\\]\\(",end:"\\)",
excludeBegin:!0,excludeEnd:!0},{className:"symbol",relevance:0,begin:"\\]\\[",
end:"\\]",excludeBegin:!0,excludeEnd:!0}]},a={className:"strong",contains:[],
variants:[{begin:/_{2}(?!\s)/,end:/_{2}/},{begin:/\*{2}(?!\s)/,end:/\*{2}/}]
},i={className:"emphasis",contains:[],variants:[{begin:/\*(?![*\s])/,end:/\*/},{
begin:/_(?![_\s])/,end:/_/,relevance:0}]},r=e.inherit(a,{contains:[]
}),s=e.inherit(i,{contains:[]});a.contains.push(s),i.contains.push(r)
;let o=[n,t];return[a,i,r,s].forEach(e=>{e.contains=e.contains.concat(o)
}),o=o.concat(a,i),{name:"Markdown",aliases:["md","mkdown","mkd"],contains:[{
className:"section",variants:[{begin:"^#{1,6}",end:"$",contains:o},{
begin:"(?=^.+?\\n[=-]{2,}$)",contains:[{begin:"^[=-]*$"},{begin:"^",end:"\\n",
contains:o}]}]},n,{className:"bullet",begin:"^[ \t]*([*+-]|(\\d+\\.))(?=\\s+)",
end:"\\s+",excludeEnd:!0},{match:/^ {0,3}([-*_])[ \t]*(?:\1[ \t]*){2,}$/},a,i,{
className:"quote",begin:"^>\\s+",contains:o,end:"$"},{className:"code",
variants:[{begin:"(`{3,})[^`](.|\\n)*?\\1`*[ ]*"},{
begin:"(~{3,})[^~](.|\\n)*?\\1~*[ ]*"},{begin:"```",end:"```+[ ]*$"},{
begin:"~~~",end:"~~~+[ ]*$"},{begin:"`.+?`"},{begin:"(?=^( {4}|\\t))",
contains:[{begin:"^( {4}|\\t)",end:"(\\n)$"}],relevance:0}]},t,{
begin:/^\[[^\n]+\]:/,returnBegin:!0,contains:[{className:"symbol",begin:/\[/,
end:/\]/,excludeBegin:!0,excludeEnd:!0},{className:"link",begin:/:\s*/,end:/$/,
excludeBegin:!0}]},{scope:"literal",
match:/&([a-zA-Z0-9]+|#[0-9]{1,7}|#[Xx][0-9a-fA-F]{1,6});/}]}},
grmr_objectivec:e=>{const n=/[a-zA-Z@][a-zA-Z0-9_]*/,t={$pattern:n,
keyword:["@interface","@class","@protocol","@implementation"]};return{
name:"Objective-C",aliases:["mm","objc","obj-c","obj-c++","objective-c++"],
keywords:{"variable.language":["this","super"],$pattern:n,
keyword:["while","export","sizeof","typedef","const","struct","for","union","volatile","static","mutable","if","do","return","goto","enum","else","break","extern","asm","case","default","register","explicit","typename","switch","continue","inline","readonly","assign","readwrite","self","@synchronized","id","typeof","nonatomic","IBOutlet","IBAction","strong","weak","copy","in","out","inout","bycopy","byref","oneway","__strong","__weak","__block","__autoreleasing","@private","@protected","@public","@try","@property","@end","@throw","@catch","@finally","@autoreleasepool","@synthesize","@dynamic","@selector","@optional","@required","@encode","@package","@import","@defs","@compatibility_alias","__bridge","__bridge_transfer","__bridge_retained","__bridge_retain","__covariant","__contravariant","__kindof","_Nonnull","_Nullable","_Null_unspecified","__FUNCTION__","__PRETTY_FUNCTION__","__attribute__","getter","setter","retain","unsafe_unretained","nonnull","nullable","null_unspecified","null_resettable","class","instancetype","NS_DESIGNATED_INITIALIZER","NS_UNAVAILABLE","NS_REQUIRES_SUPER","NS_RETURNS_INNER_POINTER","NS_INLINE","NS_AVAILABLE","NS_DEPRECATED","NS_ENUM","NS_OPTIONS","NS_SWIFT_UNAVAILABLE","NS_ASSUME_NONNULL_BEGIN","NS_ASSUME_NONNULL_END","NS_REFINED_FOR_SWIFT","NS_SWIFT_NAME","NS_SWIFT_NOTHROW","NS_DURING","NS_HANDLER","NS_ENDHANDLER","NS_VALUERETURN","NS_VOIDRETURN"],
literal:["false","true","FALSE","TRUE","nil","YES","NO","NULL"],
built_in:["dispatch_once_t","dispatch_queue_t","dispatch_sync","dispatch_async","dispatch_once"],
type:["int","float","char","unsigned","signed","short","long","double","wchar_t","unichar","void","bool","BOOL","id|0","_Bool"]
},illegal:"</",contains:[{className:"built_in",
begin:"\\b(AV|CA|CF|CG|CI|CL|CM|CN|CT|MK|MP|MTK|MTL|NS|SCN|SK|UI|WK|XC)\\w+"
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,e.C_NUMBER_MODE,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,{
className:"string",variants:[{begin:'@"',end:'"',illegal:"\\n",
contains:[e.BACKSLASH_ESCAPE]}]},{className:"meta",begin:/#\s*[a-z]+\b/,end:/$/,
keywords:{
keyword:"if else elif endif define undef warning error line pragma ifdef ifndef include"
},contains:[{begin:/\\\n/,relevance:0},e.inherit(e.QUOTE_STRING_MODE,{
className:"string"}),{className:"string",begin:/<.*?>/,end:/$/,illegal:"\\n"
},e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE]},{className:"class",
begin:"("+t.keyword.join("|")+")\\b",end:/(\{|$)/,excludeEnd:!0,keywords:t,
contains:[e.UNDERSCORE_TITLE_MODE]},{begin:"\\."+e.UNDERSCORE_IDENT_RE,
relevance:0}]}},grmr_perl:e=>{const n=e.regex,t=/[dualxmsipngr]{0,12}/,a={
$pattern:/[\w.]+/,
keyword:"abs accept alarm and atan2 bind binmode bless break caller chdir chmod chomp chop chown chr chroot class close closedir connect continue cos crypt dbmclose dbmopen defined delete die do dump each else elsif endgrent endhostent endnetent endprotoent endpwent endservent eof eval exec exists exit exp fcntl field fileno flock for foreach fork format formline getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname gethostent getlogin getnetbyaddr getnetbyname getnetent getpeername getpgrp getpriority getprotobyname getprotobynumber getprotoent getpwent getpwnam getpwuid getservbyname getservbyport getservent getsockname getsockopt given glob gmtime goto grep gt hex if index int ioctl join keys kill last lc lcfirst length link listen local localtime log lstat lt ma map method mkdir msgctl msgget msgrcv msgsnd my ne next no not oct open opendir or ord our pack package pipe pop pos print printf prototype push q|0 qq quotemeta qw qx rand read readdir readline readlink readpipe recv redo ref rename require reset return reverse rewinddir rindex rmdir say scalar seek seekdir select semctl semget semop send setgrent sethostent setnetent setpgrp setpriority setprotoent setpwent setservent setsockopt shift shmctl shmget shmread shmwrite shutdown sin sleep socket socketpair sort splice split sprintf sqrt srand stat state study sub substr symlink syscall sysopen sysread sysseek system syswrite tell telldir tie tied time times tr truncate uc ucfirst umask undef unless unlink unpack unshift untie until use utime values vec wait waitpid wantarray warn when while write x|0 xor y|0"
},i={className:"subst",begin:"[$@]\\{",end:"\\}",keywords:a},r={begin:/->\{/,
end:/\}/},s={scope:"attr",match:/\s+:\s*\w+(\s*\(.*?\))?/},o={scope:"variable",
variants:[{begin:/\$\d/},{
begin:n.concat(/[$%@](?!")(\^\w\b|#\w+(::\w+)*|\{\w+\}|\w+(::\w*)*)/,"(?![A-Za-z])(?![@$%])")
},{begin:/[$%@](?!")[^\s\w{=]|\$=/,relevance:0}],contains:[s]},c={
className:"number",variants:[{match:/0?\.[0-9][0-9_]+\b/},{
match:/\bv?(0|[1-9][0-9_]*(\.[0-9_]+)?|[1-9][0-9_]*)\b/},{
match:/\b0[0-7][0-7_]*\b/},{match:/\b0x[0-9a-fA-F][0-9a-fA-F_]*\b/},{
match:/\b0b[0-1][0-1_]*\b/}],relevance:0
},l=[e.BACKSLASH_ESCAPE,i,o],d=[/!/,/\//,/\|/,/\?/,/'/,/"/,/#/],g=(e,a,i="\\1")=>{
const r="\\1"===i?i:n.concat(i,a)
;return n.concat(n.concat("(?:",e,")"),a,/(?:\\.|[^\\\/])*?/,r,/(?:\\.|[^\\\/])*?/,i,t)
},u=(e,a,i)=>n.concat(n.concat("(?:",e,")"),a,/(?:\\.|[^\\\/])*?/,i,t),b=[o,e.HASH_COMMENT_MODE,e.COMMENT(/^=\w/,/=cut/,{
endsWithParent:!0}),r,{className:"string",contains:l,variants:[{
begin:"q[qwxr]?\\s*\\(",end:"\\)",relevance:5},{begin:"q[qwxr]?\\s*\\[",
end:"\\]",relevance:5},{begin:"q[qwxr]?\\s*\\{",end:"\\}",relevance:5},{
begin:"q[qwxr]?\\s*\\|",end:"\\|",relevance:5},{begin:"q[qwxr]?\\s*<",end:">",
relevance:5},{begin:"qw\\s+q",end:"q",relevance:5},{begin:"'",end:"'",
contains:[e.BACKSLASH_ESCAPE]},{begin:'"',end:'"'},{begin:"`",end:"`",
contains:[e.BACKSLASH_ESCAPE]},{begin:/\{\w+\}/,relevance:0},{
begin:"-?\\w+\\s*=>",relevance:0}]},c,{
begin:"(\\/\\/|"+e.RE_STARTERS_RE+"|\\b(split|return|print|reverse|grep)\\b)\\s*",
keywords:"split return print reverse grep",relevance:0,
contains:[e.HASH_COMMENT_MODE,{className:"regexp",variants:[{
begin:g("s|tr|y",n.either(...d,{capture:!0}))},{begin:g("s|tr|y","\\(","\\)")},{
begin:g("s|tr|y","\\[","\\]")},{begin:g("s|tr|y","\\{","\\}")}],relevance:2},{
className:"regexp",variants:[{begin:/(m|qr)\/\//,relevance:0},{
begin:u("(?:m|qr)?",/\//,/\//)},{begin:u("m|qr",n.either(...d,{capture:!0
}),/\1/)},{begin:u("m|qr",/\(/,/\)/)},{begin:u("m|qr",/\[/,/\]/)},{
begin:u("m|qr",/\{/,/\}/)}]}]},{className:"function",beginKeywords:"sub method",
end:"(\\s*\\(.*?\\))?[;{]",excludeEnd:!0,relevance:5,contains:[e.TITLE_MODE,s]
},{className:"class",beginKeywords:"class",end:"[;{]",excludeEnd:!0,relevance:5,
contains:[e.TITLE_MODE,s,c]},{begin:"-\\w\\b",relevance:0},{begin:"^__DATA__$",
end:"^__END__$",subLanguage:"mojolicious",contains:[{begin:"^@@.*",end:"$",
className:"comment"}]}];return i.contains=b,r.contains=b,{name:"Perl",
aliases:["pl","pm"],keywords:a,contains:b}},grmr_php:e=>{
const n=e.regex,t=/(?![A-Za-z0-9])(?![$])/,a=n.concat(/[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*/,t),i=n.concat(/(\\?[A-Z][a-z0-9_\x7f-\xff]+|\\?[A-Z]+(?=[A-Z][a-z0-9_\x7f-\xff])){1,}/,t),r=n.concat(/[A-Z]+/,t),s={
scope:"variable",match:"\\$+"+a},o={scope:"subst",variants:[{begin:/\$\w+/},{
begin:/\{\$/,end:/\}/}]},c=e.inherit(e.APOS_STRING_MODE,{illegal:null
}),l="[ \t\n]",d={scope:"string",variants:[e.inherit(e.QUOTE_STRING_MODE,{
illegal:null,contains:e.QUOTE_STRING_MODE.contains.concat(o)}),c,{
begin:/<<<[ \t]*(?:(\w+)|"(\w+)")\n/,end:/[ \t]*(\w+)\b/,
contains:e.QUOTE_STRING_MODE.contains.concat(o),"on:begin":(e,n)=>{
n.data._beginMatch=e[1]||e[2]},"on:end":(e,n)=>{
n.data._beginMatch!==e[1]&&n.ignoreMatch()}},e.END_SAME_AS_BEGIN({
begin:/<<<[ \t]*'(\w+)'\n/,end:/[ \t]*(\w+)\b/})]},g={scope:"number",variants:[{
begin:"\\b0[bB][01]+(?:_[01]+)*\\b"},{begin:"\\b0[oO][0-7]+(?:_[0-7]+)*\\b"},{
begin:"\\b0[xX][\\da-fA-F]+(?:_[\\da-fA-F]+)*\\b"},{
begin:"(?:\\b\\d+(?:_\\d+)*(\\.(?:\\d+(?:_\\d+)*))?|\\B\\.\\d+)(?:[eE][+-]?\\d+)?"
}],relevance:0
},u=["false","null","true"],b=["__CLASS__","__DIR__","__FILE__","__FUNCTION__","__COMPILER_HALT_OFFSET__","__LINE__","__METHOD__","__NAMESPACE__","__TRAIT__","die","echo","exit","include","include_once","print","require","require_once","array","abstract","and","as","binary","bool","boolean","break","callable","case","catch","class","clone","const","continue","declare","default","do","double","else","elseif","empty","enddeclare","endfor","endforeach","endif","endswitch","endwhile","enum","eval","extends","final","finally","float","for","foreach","from","global","goto","if","implements","instanceof","insteadof","int","integer","interface","isset","iterable","list","match|0","mixed","new","never","object","or","private","protected","public","readonly","real","return","string","switch","throw","trait","try","unset","use","var","void","while","xor","yield"],m=["Error|0","AppendIterator","ArgumentCountError","ArithmeticError","ArrayIterator","ArrayObject","AssertionError","BadFunctionCallException","BadMethodCallException","CachingIterator","CallbackFilterIterator","CompileError","Countable","DirectoryIterator","DivisionByZeroError","DomainException","EmptyIterator","ErrorException","Exception","FilesystemIterator","FilterIterator","GlobIterator","InfiniteIterator","InvalidArgumentException","IteratorIterator","LengthException","LimitIterator","LogicException","MultipleIterator","NoRewindIterator","OutOfBoundsException","OutOfRangeException","OuterIterator","OverflowException","ParentIterator","ParseError","RangeException","RecursiveArrayIterator","RecursiveCachingIterator","RecursiveCallbackFilterIterator","RecursiveDirectoryIterator","RecursiveFilterIterator","RecursiveIterator","RecursiveIteratorIterator","RecursiveRegexIterator","RecursiveTreeIterator","RegexIterator","RuntimeException","SeekableIterator","SplDoublyLinkedList","SplFileInfo","SplFileObject","SplFixedArray","SplHeap","SplMaxHeap","SplMinHeap","SplObjectStorage","SplObserver","SplPriorityQueue","SplQueue","SplStack","SplSubject","SplTempFileObject","TypeError","UnderflowException","UnexpectedValueException","UnhandledMatchError","ArrayAccess","BackedEnum","Closure","Fiber","Generator","Iterator","IteratorAggregate","Serializable","Stringable","Throwable","Traversable","UnitEnum","WeakReference","WeakMap","Directory","__PHP_Incomplete_Class","parent","php_user_filter","self","static","stdClass"],p={
keyword:b,literal:(e=>{const n=[];return e.forEach(e=>{
n.push(e),e.toLowerCase()===e?n.push(e.toUpperCase()):n.push(e.toLowerCase())}),
n})(u),built_in:m},_=e=>e.map(e=>e.replace(/\|\d+$/,"")),h={variants:[{
match:[/new/,n.concat(l,"+"),n.concat("(?!",_(m).join("\\b|"),"\\b)"),i],scope:{
1:"keyword",4:"title.class"}}]},f=n.concat(a,"\\b(?!\\()"),E={variants:[{
match:[n.concat(/::/,n.lookahead(/(?!class\b)/)),f],scope:{2:"variable.constant"
}},{match:[/::/,/class/],scope:{2:"variable.language"}},{
match:[i,n.concat(/::/,n.lookahead(/(?!class\b)/)),f],scope:{1:"title.class",
3:"variable.constant"}},{match:[i,n.concat("::",n.lookahead(/(?!class\b)/))],
scope:{1:"title.class"}},{match:[i,/::/,/class/],scope:{1:"title.class",
3:"variable.language"}}]},y={scope:"attr",
match:n.concat(a,n.lookahead(":"),n.lookahead(/(?!::)/))},w={relevance:0,
begin:/\(/,end:/\)/,keywords:p,
contains:[y,s,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g,h]
},v={relevance:0,
match:[/\b/,n.concat("(?!fn\\b|function\\b|",_(b).join("\\b|"),"|",_(m).join("\\b|"),"\\b)"),a,n.concat(l,"*"),n.lookahead(/(?=\()/)],
scope:{3:"title.function.invoke"},contains:[w]};w.contains.push(v)
;const N=[y,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g,h],k={
begin:n.concat(/#\[\s*\\?/,n.either(i,r)),beginScope:"meta",end:/]/,
endScope:"meta",keywords:{literal:u,keyword:["new","array"]},contains:[{
begin:/\[/,end:/]/,keywords:{literal:u,keyword:["new","array"]},
contains:["self",...N]},...N,{scope:"meta",variants:[{match:i},{match:r}]}]}
;return{case_insensitive:!1,keywords:p,
contains:[k,e.HASH_COMMENT_MODE,e.COMMENT("//","$"),e.COMMENT("/\\*","\\*/",{
contains:[{scope:"doctag",match:"@[A-Za-z]+"}]}),{match:/__halt_compiler\(\);/,
keywords:"__halt_compiler",starts:{scope:"comment",end:e.MATCH_NOTHING_RE,
contains:[{match:/\?>/,scope:"meta",endsParent:!0}]}},{scope:"meta",variants:[{
begin:/<\?php/,relevance:10},{begin:/<\?=/},{begin:/<\?/,relevance:.1},{
begin:/\?>/}]},{scope:"variable.language",match:/\$this\b/},s,v,E,{
match:[/const/,/\s/,a],scope:{1:"keyword",3:"variable.constant"}},h,{
scope:"function",relevance:0,beginKeywords:"fn function",end:/[;{]/,
excludeEnd:!0,illegal:"[$%\\[]",contains:[{beginKeywords:"use"
},e.UNDERSCORE_TITLE_MODE,{begin:"=>",endsParent:!0},{scope:"params",
begin:"\\(",end:"\\)",excludeBegin:!0,excludeEnd:!0,keywords:p,
contains:["self",k,s,E,e.C_BLOCK_COMMENT_MODE,e.C_LINE_COMMENT_MODE,e.HASH_COMMENT_MODE,d,g]
}]},{scope:"class",variants:[{beginKeywords:"enum",illegal:/[($"]/},{
beginKeywords:"class interface trait",illegal:/[:($"]/}],relevance:0,end:/\{/,
excludeEnd:!0,contains:[{beginKeywords:"extends implements"
},e.UNDERSCORE_TITLE_MODE]},{beginKeywords:"namespace",relevance:0,end:";",
illegal:/[.']/,contains:[e.inherit(e.UNDERSCORE_TITLE_MODE,{scope:"title.class"
})]},{beginKeywords:"use",relevance:0,end:";",contains:[{
match:/\b(as|const|function)\b/,scope:"keyword"},e.UNDERSCORE_TITLE_MODE]},d,g]}
},grmr_php_template:e=>({name:"PHP template",subLanguage:"xml",contains:[{
begin:/<\?(php|=)?/,end:/\?>/,subLanguage:"php",contains:[{begin:"/\\*",
end:"\\*/",skip:!0},{begin:'b"',end:'"',skip:!0},{begin:"b'",end:"'",skip:!0
},e.inherit(e.APOS_STRING_MODE,{illegal:null,className:null,contains:null,
skip:!0}),e.inherit(e.QUOTE_STRING_MODE,{illegal:null,className:null,
contains:null,skip:!0})]}]}),grmr_plaintext:e=>({name:"Plain text",
aliases:["text","txt"],disableAutodetect:!0}),grmr_python:e=>{
const n=e.regex,t=/[\p{XID_Start}_]\p{XID_Continue}*/u,a=["and","as","assert","async","await","break","case","class","continue","def","del","elif","else","except","finally","for","from","global","if","import","in","is","lambda","lazy","match","nonlocal|10","not","or","pass","raise","return","try","while","with","yield"],i={
$pattern:/[A-Za-z]\w+|__\w+__/,keyword:a,
built_in:["__import__","abs","aiter","all","anext","any","ascii","bin","bool","breakpoint","bytearray","bytes","callable","chr","classmethod","compile","complex","delattr","dict","dir","divmod","enumerate","eval","exec","filter","float","format","frozendict","frozenset","getattr","globals","hasattr","hash","help","hex","id","input","int","isinstance","issubclass","iter","len","list","locals","map","max","memoryview","min","next","object","oct","open","ord","pow","print","property","range","repr","reversed","round","sentinel","set","setattr","slice","sorted","staticmethod","str","sum","super","tuple","type","vars","zip"],
literal:["__debug__","Ellipsis","False","None","NotImplemented","True"],
type:["Any","Callable","Coroutine","Dict","List","Literal","Generic","Optional","Sequence","Set","Tuple","Type","Union"]
},r={className:"meta",begin:/^(>>>|\.\.\.) /},s={className:"subst",begin:/\{/,
end:/\}/,keywords:i,illegal:/#/},o={begin:/\{\{/,relevance:0},c={
className:"string",contains:[e.BACKSLASH_ESCAPE],variants:[{
begin:/([uU]|[bB]|[rR]|[bB][rR]|[rR][bB])?'''/,end:/'''/,
contains:[e.BACKSLASH_ESCAPE,r],relevance:10},{
begin:/([uU]|[bB]|[rR]|[bB][rR]|[rR][bB])?"""/,end:/"""/,
contains:[e.BACKSLASH_ESCAPE,r],relevance:10},{
begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])'''/,end:/'''/,
contains:[e.BACKSLASH_ESCAPE,r,o,s]},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])"""/,
end:/"""/,contains:[e.BACKSLASH_ESCAPE,r,o,s]},{begin:/([uU]|[rR])'/,end:/'/,
relevance:10},{begin:/([uU]|[rR])"/,end:/"/,relevance:10},{
begin:/([bB]|[bB][rR]|[rR][bB])'/,end:/'/},{begin:/([bB]|[bB][rR]|[rR][bB])"/,
end:/"/},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])'/,end:/'/,
contains:[e.BACKSLASH_ESCAPE,o,s]},{begin:/([fFtT][rR]|[rR][fFtT]|[fFtT])"/,
end:/"/,contains:[e.BACKSLASH_ESCAPE,o,s]
},e.APOS_STRING_MODE,e.QUOTE_STRING_MODE]
},l="[0-9](_?[0-9])*",d=`(\\b(${l}))?\\.(${l})|\\b(${l})\\.`,g="\\b|"+a.join("|"),u={
className:"number",relevance:0,variants:[{
begin:`(\\b(${l})|(${d}))[eE][+-]?(${l})[jJ]?(?=${g})`},{begin:`(${d})[jJ]?`},{
begin:`\\b([1-9](_?[0-9])*|0+(_?0)*)[lLjJ]?(?=${g})`},{
begin:`\\b0[bB](_?[01])+[lL]?(?=${g})`},{begin:`\\b0[oO](_?[0-7])+[lL]?(?=${g})`
},{begin:`\\b0[xX](_?[0-9a-fA-F])+[lL]?(?=${g})`},{begin:`\\b(${l})[jJ](?=${g})`
}]},b={className:"comment",begin:n.lookahead(/# type:/),end:/$/,keywords:i,
contains:[{begin:/# type:/},{begin:/#/,end:/\b\B/,endsWithParent:!0}]},m={
className:"params",variants:[{className:"",begin:/\(\s*\)/,skip:!0},{begin:/\(/,
end:/\)/,excludeBegin:!0,excludeEnd:!0,keywords:i,
contains:["self",r,u,c,e.HASH_COMMENT_MODE]}]};return s.contains=[c,u,r],{
name:"Python",aliases:["py","gyp","ipython"],unicodeRegex:!0,keywords:i,
illegal:/(<\/|\?)|=>/,contains:[r,u,{scope:"variable.language",match:/\bself\b/
},{beginKeywords:"if",relevance:0},{match:/\bor\b/,scope:"keyword"
},c,b,e.HASH_COMMENT_MODE,{match:[/\bdef/,/\s+/,t],scope:{1:"keyword",
3:"title.function"},contains:[m]},{variants:[{
match:[/\bclass/,/\s+/,t,/\s*/,/\(\s*/,t,/\s*\)/]},{match:[/\bclass/,/\s+/,t]}],
scope:{1:"keyword",3:"title.class",6:"title.class.inherited"}},{
className:"meta",begin:/^[\t ]*@/,end:/(?=#)|$/,contains:[u,m,c]}]}},
grmr_python_repl:e=>({aliases:["pycon"],contains:[{className:"meta.prompt",
starts:{end:/ |$/,starts:{end:"$",subLanguage:"python"}},variants:[{
begin:/^>>>(?=[ ]|$)/},{begin:/^\.\.\.(?=[ ]|$)/}]}]}),grmr_r:e=>{
const n=e.regex,t=/(?:(?:[a-zA-Z]|\.[._a-zA-Z])[._a-zA-Z0-9]*)|\.(?!\d)/,a=n.either(/0[xX][0-9a-fA-F]+\.[0-9a-fA-F]*[pP][+-]?\d+i?/,/0[xX][0-9a-fA-F]+(?:[pP][+-]?\d+)?[Li]?/,/(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?[Li]?/),i=/[=!<>:]=|\|\||&&|:::?|<-|<<-|->>|->|\|>|[-+*\/?!$&|:<=>@^~]|\*\*/,r=n.either(/[()]/,/[{}]/,/\[\[/,/[[\]]/,/\\/,/,/)
;return{name:"R",keywords:{$pattern:t,
keyword:"function if in break next repeat else for while",
literal:"NULL NA TRUE FALSE Inf NaN NA_integer_|10 NA_real_|10 NA_character_|10 NA_complex_|10",
built_in:"LETTERS letters month.abb month.name pi T F abs acos acosh all any anyNA Arg as.call as.character as.complex as.double as.environment as.integer as.logical as.null.default as.numeric as.raw asin asinh atan atanh attr attributes baseenv browser c call ceiling class Conj cos cosh cospi cummax cummin cumprod cumsum digamma dim dimnames emptyenv exp expression floor forceAndCall gamma gc.time globalenv Im interactive invisible is.array is.atomic is.call is.character is.complex is.double is.environment is.expression is.finite is.function is.infinite is.integer is.language is.list is.logical is.matrix is.na is.name is.nan is.null is.numeric is.object is.pairlist is.raw is.recursive is.single is.symbol lazyLoadDBfetch length lgamma list log max min missing Mod names nargs nzchar oldClass on.exit pos.to.env proc.time prod quote range Re rep retracemem return round seq_along seq_len seq.int sign signif sin sinh sinpi sqrt standardGeneric substitute sum switch tan tanh tanpi tracemem trigamma trunc unclass untracemem UseMethod xtfrm"
},contains:[e.COMMENT(/#'/,/$/,{contains:[{scope:"doctag",match:/@examples/,
starts:{end:n.lookahead(n.either(/\n^#'\s*(?=@[a-zA-Z]+)/,/\n^(?!#')/)),
endsParent:!0}},{scope:"doctag",begin:"@param",end:/$/,contains:[{
scope:"variable",variants:[{match:t},{match:/`(?:\\.|[^`\\])+`/}],endsParent:!0
}]},{scope:"doctag",match:/@[a-zA-Z]+/},{scope:"keyword",match:/\\[a-zA-Z]+/}]
}),e.HASH_COMMENT_MODE,{scope:"string",contains:[e.BACKSLASH_ESCAPE],
variants:[e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\(/,end:/\)(-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\{/,end:/\}(-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]"(-*)\[/,end:/\](-*)"/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\(/,end:/\)(-*)'/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\{/,end:/\}(-*)'/
}),e.END_SAME_AS_BEGIN({begin:/[rR]'(-*)\[/,end:/\](-*)'/}),{begin:'"',end:'"',
relevance:0},{begin:"'",end:"'",relevance:0}]},{relevance:0,variants:[{scope:{
1:"operator",2:"number"},match:[i,a]},{scope:{1:"operator",2:"number"},
match:[/%[^%]*%/,a]},{scope:{1:"punctuation",2:"number"},match:[r,a]},{scope:{
2:"number"},match:[/[^a-zA-Z0-9._]|^/,a]}]},{scope:{3:"operator"},
match:[t,/\s+/,/<-/,/\s+/]},{scope:"operator",relevance:0,variants:[{match:i},{
match:/%[^%]*%/}]},{scope:"punctuation",relevance:0,match:r},{begin:"`",end:"`",
contains:[{begin:/\\./}]}]}},grmr_ruby:e=>{
const n=e.regex,t="([a-zA-Z_]\\w*[!?=]?|[-+~]@|<<|>>|=~|===?|<=>|[<>]=?|\\*\\*|[-/+%^&*~`|]|\\[\\]=?)",a=n.either(/\b([A-Z]+[a-z0-9]+)+/,/\b([A-Z]+[a-z0-9]+)+[A-Z]+/),i=n.concat(a,/(::\w+)*/),r={
"variable.constant":["__FILE__","__LINE__","__ENCODING__"],
"variable.language":["self","super"],
keyword:["alias","and","begin","BEGIN","break","case","class","defined","do","else","elsif","end","END","ensure","for","if","in","module","next","not","or","redo","require","rescue","retry","return","then","undef","unless","until","when","while","yield","include","extend","prepend","public","private","protected","raise","throw"],
built_in:["proc","lambda","attr_accessor","attr_reader","attr_writer","define_method","private_constant","module_function"],
literal:["true","false","nil"]},s={className:"doctag",begin:"@[A-Za-z]+"},o={
begin:"#<",end:">"},c=[e.COMMENT("#","$",{contains:[s]
}),e.COMMENT("^=begin","^=end",{contains:[s],relevance:10
}),e.COMMENT("^__END__",e.MATCH_NOTHING_RE)],l={className:"subst",begin:/#\{/,
end:/\}/,keywords:r},d={className:"string",contains:[e.BACKSLASH_ESCAPE,l],
variants:[{begin:/'/,end:/'/},{begin:/"/,end:/"/},{begin:/`/,end:/`/},{
begin:/%[qQwWx]?\(/,end:/\)/},{begin:/%[qQwWx]?\[/,end:/\]/},{
begin:/%[qQwWx]?\{/,end:/\}/},{begin:/%[qQwWx]?</,end:/>/},{begin:/%[qQwWx]?\//,
end:/\//},{begin:/%[qQwWx]?%/,end:/%/},{begin:/%[qQwWx]?-/,end:/-/},{
begin:/%[qQwWx]?\|/,end:/\|/},{begin:/\B\?(\\\d{1,3})/},{
begin:/\B\?(\\x[A-Fa-f0-9]{1,2})/},{begin:/\B\?(\\u\{?[A-Fa-f0-9]{1,6}\}?)/},{
begin:/\B\?(\\M-\\C-|\\M-\\c|\\c\\M-|\\M-|\\C-\\M-)[\x20-\x7e]/},{
begin:/\B\?\\(c|C-)[\x20-\x7e]/},{begin:/\B\?\\?\S/},{
begin:n.concat(/<<[-~]?'?/,n.lookahead(/(\w+)(?=\W)[^\n]*\n(?:[^\n]*\n)*?\s*\1\b/)),
contains:[e.END_SAME_AS_BEGIN({begin:/(\w+)/,end:/(\w+)/,
contains:[e.BACKSLASH_ESCAPE,l]})]}]},g="[0-9](_?[0-9])*",u={className:"number",
relevance:0,variants:[{
begin:`\\b([1-9](_?[0-9])*|0)(\\.(${g}))?([eE][+-]?(${g})|r)?i?\\b`},{
begin:"\\b0[dD][0-9](_?[0-9])*r?i?\\b"},{begin:"\\b0[bB][0-1](_?[0-1])*r?i?\\b"
},{begin:"\\b0[oO][0-7](_?[0-7])*r?i?\\b"},{
begin:"\\b0[xX][0-9a-fA-F](_?[0-9a-fA-F])*r?i?\\b"},{
begin:"\\b0(_?[0-7])+r?i?\\b"}]},b={variants:[{match:/\(\)/},{
className:"params",begin:/\(/,end:/(?=\))/,excludeBegin:!0,endsParent:!0,
keywords:r}]},m=[d,{variants:[{match:[/class\s+/,i,/\s+<\s+/,i]},{
match:[/\b(class|module)\s+/,i]}],scope:{2:"title.class",
4:"title.class.inherited"},keywords:r},{match:[/(include|extend)\s+/,i],scope:{
2:"title.class"},keywords:r},{relevance:0,match:[i,/\.new[. (]/],scope:{
1:"title.class"}},{relevance:0,match:/\b[A-Z][A-Z_0-9]+\b/,
className:"variable.constant"},{relevance:0,match:a,scope:"title.class"},{
match:[/def/,/\s+/,t],scope:{1:"keyword",3:"title.function"},contains:[b]},{
begin:"::"},{className:"symbol",begin:e.UNDERSCORE_IDENT_RE+"(!|\\?)?:",
relevance:0},{className:"symbol",begin:":(?!\\s)",contains:[d,{begin:t}],
relevance:0},u,{className:"variable",
begin:"(\\$\\W)|((\\$|@@?)(\\w+))(?=[^@$?])(?![A-Za-z])(?![@$?'])"},{
className:"params",begin:/\|(?!=)/,end:/\|/,excludeBegin:!0,excludeEnd:!0,
relevance:0,keywords:r},{begin:"("+e.RE_STARTERS_RE+"|unless)\\s*",
keywords:"unless",contains:[{className:"regexp",contains:[e.BACKSLASH_ESCAPE,l],
illegal:/\n/,variants:[{begin:"/",end:"/[a-z]*"},{begin:/%r\{/,end:/\}[a-z]*/},{
begin:"%r\\(",end:"\\)[a-z]*"},{begin:"%r!",end:"![a-z]*"},{begin:"%r\\[",
end:"\\][a-z]*"}]}].concat(o,c),relevance:0}].concat(o,c)
;l.contains=m,b.contains=m;const p=[{begin:/^\s*=>/,starts:{end:"$",contains:m}
},{className:"meta.prompt",
begin:"^([>?]>|[\\w#]+\\(\\w+\\):\\d+:\\d+[>*]|(\\w+-)?\\d+\\.\\d+\\.\\d+(p\\d+)?[^\\d][^>]+>)(?=[ ])",
starts:{end:"$",keywords:r,contains:m}}];return c.unshift(o),{name:"Ruby",
aliases:["rb","gemspec","podspec","thor","irb"],keywords:r,illegal:/\/\*/,
contains:[e.SHEBANG({binary:"ruby"})].concat(p).concat(c).concat(m)}},
grmr_rust:e=>{
const n=e.regex,t=/(r#)?/,a=n.concat(t,e.UNDERSCORE_IDENT_RE),i=n.concat(t,e.IDENT_RE),r={
scope:"title.function.invoke",relevance:0,
begin:n.concat(/\b/,/(?!(?:let|for|while|if|else|match)\b)/,i,n.lookahead(/\s*\(/))
},s="([ui](8|16|32|64|128|size)|f(16|32|64|128))?",o=["drop ","Copy","Send","Sized","Sync","Drop","Fn","FnMut","FnOnce","ToOwned","Clone","Debug","PartialEq","PartialOrd","Eq","Ord","AsRef","AsMut","Into","From","Default","Iterator","Extend","IntoIterator","DoubleEndedIterator","ExactSizeIterator","SliceConcatExt","ToString","assert!","assert_eq!","bitflags!","bytes!","cfg!","col!","concat!","concat_idents!","debug_assert!","debug_assert_eq!","env!","eprintln!","panic!","file!","format!","format_args!","include_bytes!","include_str!","line!","local_data_key!","module_path!","option_env!","print!","println!","select!","stringify!","try!","unimplemented!","unreachable!","vec!","write!","writeln!","macro_rules!","assert_ne!","debug_assert_ne!"],c=["i8","i16","i32","i64","i128","isize","u8","u16","u32","u64","u128","usize","f16","f32","f64","f128","str","char","bool","Box","Option","Result","String","Vec"]
;return{name:"Rust",aliases:["rs"],keywords:{$pattern:e.IDENT_RE+"!?",type:c,
keyword:["abstract","as","async","await","become","box","break","const","continue","crate","do","dyn","else","enum","extern","false","final","fn","for","if","impl","in","let","loop","macro","match","mod","move","mut","override","priv","pub","raw","ref","return","self","Self","static","struct","super","trait","true","try","type","typeof","union","unsafe","unsized","use","virtual","where","while","yield"],
literal:["true","false","Some","None","Ok","Err"],built_in:o},illegal:"</",
contains:[e.C_LINE_COMMENT_MODE,e.COMMENT("/\\*","\\*/",{contains:["self"]
}),e.inherit(e.QUOTE_STRING_MODE,{begin:/b?"/,illegal:null}),{scope:"symbol",
begin:/'[a-zA-Z_][a-zA-Z0-9_]*(?!')/},{scope:"string",variants:[{
begin:/b?r(#*)"(.|\n)*?"\1(?!#)/},{begin:/b?'/,end:/'/,contains:[{
scope:"char.escape",match:/\\('|"|\\|\w|x\w{2}|u\w{4}|U\w{8})/}]}]},{
scope:"number",variants:[{begin:"\\b0b([01_]+)"+s},{begin:"\\b0o([0-7_]+)"+s},{
begin:"\\b0x([A-Fa-f0-9_]+)"+s},{
begin:"\\b(\\d[\\d_]*(\\.[0-9_]+)?([eE][+-]?[0-9_]+)?)"+s}],relevance:0},{
begin:[/\bsafe/,/\s+/,/extern/],scope:{1:"keyword",3:"keyword"}},{
begin:[/fn/,/\s+/,a],scope:{1:"keyword",3:"title.function"}},{scope:"meta",
begin:"#!?\\[",end:"\\]",contains:[{scope:"string",begin:/"/,end:/"/,
contains:[e.BACKSLASH_ESCAPE]}]},{begin:[/let/,/\s+/,/(?:mut\s+)?/,a],scope:{
1:"keyword",3:"keyword",4:"variable"}},{begin:[/for/,/\s+/,a,/\s+/,/in/],scope:{
1:"keyword",3:"variable",5:"keyword"}},{begin:[/type/,/\s+/,a],scope:{
1:"keyword",3:"title.class"}},{
begin:[/(?:trait|enum|struct|union|impl|for)/,/\s+/,a],scope:{1:"keyword",
3:"title.class"}},{begin:e.IDENT_RE+"::",keywords:{keyword:"Self",built_in:o,
type:c}},{scope:"punctuation",begin:"->"},r]}},grmr_scss:e=>{
const n=Ce(e),t=Ie,a=De,i="@[a-z-]+",r={className:"variable",
begin:"(\\$[a-zA-Z-][a-zA-Z0-9_-]*)\\b",relevance:0};return{name:"SCSS",
case_insensitive:!0,illegal:"[=/|']",
contains:[e.C_LINE_COMMENT_MODE,e.C_BLOCK_COMMENT_MODE,n.CSS_NUMBER_MODE,{
className:"selector-id",begin:"#[A-Za-z0-9_-]+",relevance:0},{
className:"selector-class",begin:"\\.[A-Za-z0-9_-]+",relevance:0
},n.ATTRIBUTE_SELECTOR_MODE,{className:"selector-tag",
begin:"\\b("+Te.join("|")+")\\b",relevance:0},{className:"selector-pseudo",
begin:":("+a.join("|")+")"},{className:"selector-pseudo",
begin:":(:)?("+t.join("|")+")"},r,{begin:/\(/,end:/\)/,
contains:[n.CSS_NUMBER_MODE]},n.CSS_VARIABLE,{className:"attribute",
begin:"\\b("+Le.join("|")+")\\b"},{
begin:"\\b(whitespace|wait|w-resize|visible|vertical-text|vertical-ideographic|uppercase|upper-roman|upper-alpha|underline|transparent|top|thin|thick|text|text-top|text-bottom|tb-rl|table-header-group|table-footer-group|sw-resize|super|strict|static|square|solid|small-caps|separate|se-resize|scroll|s-resize|rtl|row-resize|ridge|right|repeat|repeat-y|repeat-x|relative|progress|pointer|overline|outside|outset|oblique|nowrap|not-allowed|normal|none|nw-resize|no-repeat|no-drop|newspaper|ne-resize|n-resize|move|middle|medium|ltr|lr-tb|lowercase|lower-roman|lower-alpha|loose|list-item|line|line-through|line-edge|lighter|left|keep-all|justify|italic|inter-word|inter-ideograph|inside|inset|inline|inline-block|inherit|inactive|ideograph-space|ideograph-parenthesis|ideograph-numeric|ideograph-alpha|horizontal|hidden|help|hand|groove|fixed|ellipsis|e-resize|double|dotted|distribute|distribute-space|distribute-letter|distribute-all-lines|disc|disabled|default|decimal|dashed|crosshair|collapse|col-resize|circle|char|center|capitalize|break-word|break-all|bottom|both|bolder|bold|block|bidi-override|below|baseline|auto|always|all-scroll|absolute|table|table-cell)\\b"
},{begin:/:/,end:/[;}{]/,relevance:0,
contains:[n.BLOCK_COMMENT,r,n.HEXCOLOR,n.CSS_NUMBER_MODE,n.UNICODE_RANGE,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,n.IMPORTANT,n.FUNCTION_DISPATCH]
},{begin:"@(page|font-face)",keywords:{$pattern:i,keyword:"@page @font-face"}},{
begin:"@",end:"[{;]",returnBegin:!0,keywords:{$pattern:/[a-z-]+/,
keyword:"and or not only",attribute:Re.join(" ")},contains:[{begin:i,
className:"keyword"},{begin:/[a-z-]+(?=:)/,className:"attribute"
},r,e.QUOTE_STRING_MODE,e.APOS_STRING_MODE,n.HEXCOLOR,n.CSS_NUMBER_MODE]
},n.FUNCTION_DISPATCH]}},grmr_shell:e=>({name:"Shell Session",
aliases:["console","shellsession"],contains:[{className:"meta.prompt",
begin:/^\s{0,3}[./~\w\d[\]()@-]*[>%$#][ ]?/,starts:{end:/[^\\](?=\s*$)/,
subLanguage:"bash"}}]}),grmr_sql:e=>{
const n=e.regex,t=e.COMMENT("--","$"),a=["abs","acos","array_agg","asin","atan","avg","cast","ceil","ceiling","coalesce","corr","cos","cosh","count","covar_pop","covar_samp","cume_dist","dense_rank","deref","element","exp","extract","first_value","floor","json_array","json_arrayagg","json_exists","json_object","json_objectagg","json_query","json_table","json_table_primitive","json_value","lag","last_value","lead","listagg","ln","log","log10","lower","max","min","mod","nth_value","ntile","nullif","percent_rank","percentile_cont","percentile_disc","position","position_regex","power","rank","regr_avgx","regr_avgy","regr_count","regr_intercept","regr_r2","regr_slope","regr_sxx","regr_sxy","regr_syy","row_number","sin","sinh","sqrt","stddev_pop","stddev_samp","substring","substring_regex","sum","tan","tanh","translate","translate_regex","treat","trim","trim_array","unnest","upper","value_of","var_pop","var_samp","width_bucket"],i=a,r=["abs","acos","all","allocate","alter","and","any","are","array","array_agg","array_max_cardinality","as","asensitive","asin","asymmetric","at","atan","atomic","authorization","avg","begin","begin_frame","begin_partition","between","bigint","binary","blob","boolean","both","by","call","called","cardinality","cascaded","case","cast","ceil","ceiling","char","char_length","character","character_length","check","classifier","clob","close","coalesce","collate","collect","column","commit","condition","connect","constraint","contains","convert","copy","corr","corresponding","cos","cosh","count","covar_pop","covar_samp","create","cross","cube","cume_dist","current","current_catalog","current_date","current_default_transform_group","current_path","current_role","current_row","current_schema","current_time","current_timestamp","current_path","current_role","current_transform_group_for_type","current_user","cursor","cycle","date","day","deallocate","dec","decimal","decfloat","declare","default","define","delete","dense_rank","deref","describe","deterministic","disconnect","distinct","double","drop","dynamic","each","element","else","empty","end","end_frame","end_partition","end-exec","equals","escape","every","except","exec","execute","exists","exp","external","extract","false","fetch","filter","first_value","float","floor","for","foreign","frame_row","free","from","full","function","fusion","get","global","grant","group","grouping","groups","having","hold","hour","identity","in","indicator","initial","inner","inout","insensitive","insert","int","integer","intersect","intersection","interval","into","is","join","json_array","json_arrayagg","json_exists","json_object","json_objectagg","json_query","json_table","json_table_primitive","json_value","lag","language","large","last_value","lateral","lead","leading","left","like","like_regex","listagg","ln","local","localtime","localtimestamp","log","log10","lower","match","match_number","match_recognize","matches","max","member","merge","method","min","minute","mod","modifies","module","month","multiset","national","natural","nchar","nclob","new","no","none","normalize","not","nth_value","ntile","null","nullif","numeric","octet_length","occurrences_regex","of","offset","old","omit","on","one","only","open","or","order","out","outer","over","overlaps","overlay","parameter","partition","pattern","per","percent","percent_rank","percentile_cont","percentile_disc","period","portion","position","position_regex","power","precedes","precision","prepare","primary","procedure","ptf","range","rank","reads","real","recursive","ref","references","referencing","regr_avgx","regr_avgy","regr_count","regr_intercept","regr_r2","regr_slope","regr_sxx","regr_sxy","regr_syy","release","result","return","returns","revoke","right","rollback","rollup","row","row_number","rows","running","savepoint","scope","scroll","search","second","seek","select","sensitive","session_user","set","show","similar","sin","sinh","skip","smallint","some","specific","specifictype","sql","sqlexception","sqlstate","sqlwarning","sqrt","start","static","stddev_pop","stddev_samp","submultiset","subset","substring","substring_regex","succeeds","sum","symmetric","system","system_time","system_user","table","tablesample","tan","tanh","then","time","timestamp","timezone_hour","timezone_minute","to","trailing","translate","translate_regex","translation","treat","trigger","trim","trim_array","true","truncate","uescape","union","unique","unknown","unnest","update","upper","user","using","value","values","value_of","var_pop","var_samp","varbinary","varchar","varying","versioning","when","whenever","where","width_bucket","window","with","within","without","year","add","asc","collation","desc","final","first","last","view"].filter(e=>!a.includes(e)),s={
match:n.concat(/\b/,n.either(...i),/\s*\(/),relevance:0,keywords:{built_in:i}}
;function o(e){
return n.concat(/\b/,n.either(...e.map(e=>e.replace(/\s+/,"\\s+"))),/\b/)}
const c={scope:"keyword",
match:o(["create table","insert into","primary key","foreign key","not null","alter table","add constraint","grouping sets","on overflow","character set","respect nulls","ignore nulls","nulls first","nulls last","depth first","breadth first"]),
relevance:0};return{name:"SQL",case_insensitive:!0,illegal:/[{}]|<\//,keywords:{
$pattern:/\b[\w\.]+/,keyword:((e,{exceptions:n,when:t}={})=>{const a=t
;return n=n||[],e.map(e=>e.match(/\|\d+$/)||n.includes(e)?e:a(e)?e+"|0":e)})(r,{
when:e=>e.length<3}),literal:["true","false","unknown"],
type:["bigint","binary","blob","boolean","char","character","clob","date","dec","decfloat","decimal","float","int","integer","interval","nchar","nclob","national","numeric","real","row","smallint","time","timestamp","varchar","varying","varbinary"],
built_in:["current_catalog","current_date","current_default_transform_group","current_path","current_role","current_schema","current_transform_group_for_type","current_user","session_user","system_time","system_user","current_time","localtime","current_timestamp","localtimestamp"]
},contains:[{scope:"type",
match:o(["double precision","large object","with timezone","without timezone"])
},c,s,{scope:"variable",match:/@[a-z0-9][a-z0-9_]*/},{scope:"string",variants:[{
begin:/'/,end:/'/,contains:[{match:/''/}]}]},{begin:/"/,end:/"/,contains:[{
match:/""/}]},e.C_NUMBER_MODE,e.C_BLOCK_COMMENT_MODE,t,{scope:"operator",
match:/[-+*/=%^~]|&&?|\|\|?|!=?|<(?:=>?|<|>)?|>[>=]?/,relevance:0}]}},
grmr_swift:e=>{const n={match:/\s+/,relevance:0},t=e.COMMENT("/\\*","\\*/",{
contains:["self"]}),a=[e.C_LINE_COMMENT_MODE,t],i={match:[/\./,m(...be,...me)],
className:{2:"keyword"}},r={match:b(/\./,m(..._e)),relevance:0
},s=_e.filter(e=>"string"==typeof e).concat(["_|0"]),o={variants:[{
className:"keyword",
match:m(..._e.filter(e=>"string"!=typeof e).concat(pe).map(ue),...me)}]},c={
$pattern:m(/\b\w+/,/#\w+/),keyword:s.concat(Ee),literal:he},l=[i,r,o],g=[{
match:b(/\./,m(...ye)),relevance:0},{className:"built_in",
match:b(/\b/,m(...ye),/(?=\()/)}],u={match:/->/,relevance:0},p=[u,{
className:"operator",relevance:0,variants:[{match:Ne},{match:`\\.(\\.|${ve})+`}]
}],_="([0-9]_*)+",h="([0-9a-fA-F]_*)+",f={className:"number",relevance:0,
variants:[{match:`\\b(${_})(\\.(${_}))?([eE][+-]?(${_}))?\\b`},{
match:`\\b0x(${h})(\\.(${h}))?([pP][+-]?(${_}))?\\b`},{match:/\b0o([0-7]_*)+\b/
},{match:/\b0b([01]_*)+\b/}]},E=(e="")=>({className:"subst",variants:[{
match:b(/\\/,e,/[0\\tnr"']/)},{match:b(/\\/,e,/u\{[0-9a-fA-F]{1,8}\}/)}]
}),y=(e="")=>({className:"subst",match:b(/\\/,e,/[\t ]*(?:[\r\n]|\r\n)/)
}),w=(e="")=>({className:"subst",label:"interpol",begin:b(/\\/,e,/\(/),end:/\)/
}),v=(e="")=>({begin:b(e,/"""/),end:b(/"""/,e),contains:[E(e),y(e),w(e)]
}),N=(e="")=>({begin:b(e,/"/),end:b(/"/,e),contains:[E(e),w(e)]}),k={
className:"string",
variants:[v(),v("#"),v("##"),v("###"),N(),N("#"),N("##"),N("###")]
},x=[e.BACKSLASH_ESCAPE,{begin:/\[/,end:/\]/,relevance:0,
contains:[e.BACKSLASH_ESCAPE]}],O={begin:/\/[^\s](?=[^/\n]*\/)/,end:/\//,
contains:x},M=e=>{const n=b(e,/\//),t=b(/\//,e);return{begin:n,end:t,
contains:[...x,{scope:"comment",begin:`#(?!.*${t})`,end:/$/}]}},A={
scope:"regexp",variants:[M("###"),M("##"),M("#"),O]},S={match:b(/`/,Oe,/`/)
},C=[S,{className:"variable",match:/\$\d+/},{className:"variable",
match:`\\$${xe}+`}],T=[{match:/(@|#(un)?)available/,scope:"keyword",starts:{
contains:[{begin:/\(/,end:/\)/,keywords:Se,contains:[...p,f,k]}]}},{
scope:"keyword",match:b(/@/,m(...Ae),d(m(/\(/,/\s+/)))},{scope:"meta",
match:b(/@/,Oe)}],R={match:d(/\b[A-Z]/),relevance:0,contains:[{className:"type",
match:b(/(AV|CA|CF|CG|CI|CL|CM|CN|CT|MK|MP|MTK|MTL|NS|SCN|SK|UI|WK|XC)/,xe,"+")
},{className:"type",match:Me,relevance:0},{match:/[?!]+/,relevance:0},{
match:/\.\.\./,relevance:0},{match:b(/\s+&\s+/,d(Me)),relevance:0}]},D={
begin:/</,end:/>/,keywords:c,contains:[...a,...l,...T,u,R]};R.contains.push(D)
;const I={begin:/\(/,end:/\)/,relevance:0,keywords:c,contains:["self",{
match:b(Oe,/\s*:/),keywords:"_|0",relevance:0
},...a,A,...l,...g,...p,f,k,...C,...T,R]},L={begin:/</,end:/>/,
keywords:"repeat each",contains:[...a,R]},B={begin:/\(/,end:/\)/,keywords:c,
contains:[{begin:m(d(b(Oe,/\s*:/)),d(b(Oe,/\s+/,Oe,/\s*:/))),end:/:/,
relevance:0,contains:[{className:"keyword",match:/\b_\b/},{className:"params",
match:Oe}]},...a,...l,...p,f,k,...T,R,I],endsParent:!0,illegal:/["']/},F={
match:[/(func|macro)/,/\s+/,m(S.match,Oe,Ne)],className:{1:"keyword",
3:"title.function"},contains:[L,B,n],illegal:[/\[/,/%/]},$={
match:[/\b(?:subscript|init[?!]?)/,/\s*(?=[<(])/],className:{1:"keyword"},
contains:[L,B,n],illegal:/\[|%/},z={match:[/operator/,/\s+/,Ne],className:{
1:"keyword",3:"title"}},j={begin:[/precedencegroup/,/\s+/,Me],className:{
1:"keyword",3:"title"},contains:[R],keywords:[...fe,...he],end:/}/},U={
begin:[/(struct|protocol|class|extension|enum|actor)/,/\s+/,Oe,/\s*/],
beginScope:{1:"keyword",3:"title.class"},keywords:c,contains:[L,...l,{begin:/:/,
end:/\{/,keywords:c,contains:[{scope:"title.class.inherited",match:Me},...l],
relevance:0}]};for(const e of k.variants){
const n=e.contains.find(e=>"interpol"===e.label);n.keywords=c
;const t=[...l,...g,...p,f,k,...C];n.contains=[...t,{begin:/\(/,end:/\)/,
contains:["self",...t]}]}return{name:"Swift",keywords:c,contains:[...a,F,$,{
match:[/class\b/,/\s+/,/func\b/,/\s+/,/\b[A-Za-z_][A-Za-z0-9_]*\b/],scope:{
1:"keyword",3:"keyword",5:"title.function"}},{match:[/class\b/,/\s+/,/var\b/],
scope:{1:"keyword",3:"keyword"}},U,z,j,{beginKeywords:"import",end:/$/,
contains:[...a],relevance:0},A,...l,...g,...p,f,k,...C,...T,R,I]}},
grmr_typescript:e=>{
const n=e.regex,t=ge(e),a=te,i=["any","void","number","boolean","string","object","never","symbol","bigint","unknown"],r={
begin:[/namespace/,/\s+/,e.IDENT_RE],beginScope:{1:"keyword",3:"title.class"}
},s={beginKeywords:"interface",end:/\{/,excludeEnd:!0,keywords:{
keyword:"interface extends",built_in:i},contains:[t.exports.CLASS_REFERENCE]
},o={$pattern:te,
keyword:ie.concat(["type","interface","public","private","protected","implements","declare","abstract","readonly","enum","override","satisfies"]),
literal:re,built_in:de.concat(i),"variable.language":le},c={className:"meta",
begin:"@"+a},l=(e,n,t)=>{const a=e.contains.findIndex(e=>e.label===n)
;if(-1===a)throw Error("can not find mode to replace");e.contains.splice(a,1,t)}
;Object.assign(t.keywords,o),t.exports.PARAMS_CONTAINS.push(c)
;const d=t.contains.find(e=>"attr"===e.scope),g=Object.assign({},d,{
match:n.concat(a,n.lookahead(/\s*\?:/))})
;return t.exports.PARAMS_CONTAINS.push([t.exports.CLASS_REFERENCE,d,g]),
t.contains=t.contains.concat([c,r,s,g]),
l(t,"shebang",e.SHEBANG()),l(t,"use_strict",{className:"meta",relevance:10,
begin:/^\s*['"]use strict['"]/
}),t.contains.find(e=>"func.def"===e.label).relevance=0,Object.assign(t,{
name:"TypeScript",aliases:["ts","tsx","mts","cts"]}),t},grmr_vbnet:e=>{
const n=e.regex,t=/\d{1,2}\/\d{1,2}\/\d{4}/,a=/\d{4}-\d{1,2}-\d{1,2}/,i=/(\d|1[012])(:\d+){0,2} *(AM|PM)/,r=/\d{1,2}(:\d{1,2}){1,2}/,s={
className:"literal",variants:[{begin:n.concat(/# */,n.either(a,t),/ *#/)},{
begin:n.concat(/# */,r,/ *#/)},{begin:n.concat(/# */,i,/ *#/)},{
begin:n.concat(/# */,n.either(a,t),/ +/,n.either(i,r),/ *#/)}]
},o=e.COMMENT(/'''/,/$/,{contains:[{className:"doctag",begin:/<\/?/,end:/>/}]
}),c=e.COMMENT(null,/$/,{variants:[{begin:/'/},{begin:/([\t ]|^)REM(?=\s)/}]})
;return{name:"Visual Basic .NET",aliases:["vb"],case_insensitive:!0,
classNameAliases:{label:"symbol"},keywords:{
keyword:"addhandler alias aggregate ansi as async assembly auto binary by byref byval call case catch class compare const continue custom declare default delegate dim distinct do each equals else elseif end enum erase error event exit explicit finally for friend from function get global goto group handles if implements imports in inherits interface into iterator join key let lib loop me mid module mustinherit mustoverride mybase myclass namespace narrowing new next notinheritable notoverridable of off on operator option optional order overloads overridable overrides paramarray partial preserve private property protected public raiseevent readonly redim removehandler resume return select set shadows shared skip static step stop structure strict sub synclock take text then throw to try unicode until using when where while widening with withevents writeonly yield",
built_in:"addressof and andalso await directcast gettype getxmlnamespace is isfalse isnot istrue like mod nameof new not or orelse trycast typeof xor cbool cbyte cchar cdate cdbl cdec cint clng cobj csbyte cshort csng cstr cuint culng cushort",
type:"boolean byte char date decimal double integer long object sbyte short single string uinteger ulong ushort",
literal:"true false nothing"},
illegal:"//|\\{|\\}|endif|gosub|variant|wend|^\\$ ",contains:[{
className:"string",begin:/"(""|[^/n])"C\b/},{className:"string",begin:/"/,
end:/"/,illegal:/\n/,contains:[{begin:/""/}]},s,{className:"number",relevance:0,
variants:[{begin:/\b\d[\d_]*((\.[\d_]+(E[+-]?[\d_]+)?)|(E[+-]?[\d_]+))[RFD@!#]?/
},{begin:/\b\d[\d_]*((U?[SIL])|[%&])?/},{begin:/&H[\dA-F_]+((U?[SIL])|[%&])?/},{
begin:/&O[0-7_]+((U?[SIL])|[%&])?/},{begin:/&B[01_]+((U?[SIL])|[%&])?/}]},{
className:"label",begin:/^\w+:/},o,c,{className:"meta",
begin:/[\t ]*#(const|disable|else|elseif|enable|end|externalsource|if|region)\b/,
end:/$/,keywords:{
keyword:"const disable else elseif enable end externalsource if region then"},
contains:[c]}]}},grmr_wasm:e=>{e.regex;const n=e.COMMENT(/\(;/,/;\)/)
;return n.contains.push("self"),{name:"WebAssembly",keywords:{$pattern:/[\w.]+/,
keyword:["anyfunc","block","br","br_if","br_table","call","call_indirect","data","drop","elem","else","end","export","func","global.get","global.set","local.get","local.set","local.tee","get_global","get_local","global","if","import","local","loop","memory","memory.grow","memory.size","module","mut","nop","offset","param","result","return","select","set_global","set_local","start","table","tee_local","then","type","unreachable"]
},contains:[e.COMMENT(/;;/,/$/),n,{match:[/(?:offset|align)/,/\s*/,/=/],
className:{1:"keyword",3:"operator"}},{className:"variable",begin:/\$[\w_]+/},{
match:/(\((?!;)|\))+/,className:"punctuation",relevance:0},{
begin:[/(?:func|call|call_indirect)/,/\s+/,/\$[^\s)]+/],className:{1:"keyword",
3:"title.function"}},e.QUOTE_STRING_MODE,{match:/(i32|i64|f32|f64)(?!\.)/,
className:"type"},{className:"keyword",
match:/\b(f32|f64|i32|i64)(?:\.(?:abs|add|and|ceil|clz|const|convert_[su]\/i(?:32|64)|copysign|ctz|demote\/f64|div(?:_[su])?|eqz?|extend_[su]\/i32|floor|ge(?:_[su])?|gt(?:_[su])?|le(?:_[su])?|load(?:(?:8|16|32)_[su])?|lt(?:_[su])?|max|min|mul|nearest|neg?|or|popcnt|promote\/f32|reinterpret\/[fi](?:32|64)|rem_[su]|rot[lr]|shl|shr_[su]|store(?:8|16|32)?|sqrt|sub|trunc(?:_[su]\/f(?:32|64))?|wrap\/i64|xor))\b/
},{className:"number",relevance:0,
match:/[+-]?\b(?:\d(?:_?\d)*(?:\.\d(?:_?\d)*)?(?:[eE][+-]?\d(?:_?\d)*)?|0x[\da-fA-F](?:_?[\da-fA-F])*(?:\.[\da-fA-F](?:_?[\da-fA-D])*)?(?:[pP][+-]?\d(?:_?\d)*)?)\b|\binf\b|\bnan(?::0x[\da-fA-F](?:_?[\da-fA-D])*)?\b/
}]}},grmr_xml:e=>{
const n=e.regex,t=n.concat(/[\p{L}_]/u,n.optional(/[\p{L}0-9_.-]*:/u),/[\p{L}0-9_.-]*/u),a={
className:"symbol",begin:/&[a-z]+;|&#[0-9]+;|&#x[a-f0-9]+;/},i={begin:/\s/,
contains:[{className:"keyword",begin:/#?[a-z_][a-z1-9_-]+/,illegal:/\n/}]
},r=e.inherit(i,{begin:/\(/,end:/\)/}),s=e.inherit(e.APOS_STRING_MODE,{
className:"string"}),o=e.inherit(e.QUOTE_STRING_MODE,{className:"string"}),c={
endsWithParent:!0,illegal:/</,relevance:0,contains:[{className:"attr",
begin:/[\p{L}0-9._:-]+/u,relevance:0},{begin:/=\s*/,relevance:0,contains:[{
className:"string",endsParent:!0,variants:[{begin:/"/,end:/"/,contains:[a]},{
begin:/'/,end:/'/,contains:[a]},{begin:/[^\s"'=<>`]+/}]}]}]};return{
name:"HTML, XML",
aliases:["html","xhtml","rss","atom","xjb","xsd","xsl","plist","wsf","svg"],
case_insensitive:!0,unicodeRegex:!0,contains:[{className:"meta",begin:/<![a-z]/,
end:/>/,relevance:10,contains:[i,o,s,r,{begin:/\[/,end:/\]/,contains:[{
className:"meta",begin:/<![a-z]/,end:/>/,contains:[i,r,o,s]}]}]
},e.COMMENT(/<!--/,/-->/,{relevance:10}),{begin:/<!\[CDATA\[/,end:/\]\]>/,
relevance:10},a,{className:"meta",end:/\?>/,variants:[{begin:/<\?xml/,
relevance:10,contains:[o]},{begin:/<\?[a-z][a-z0-9]+/}]},{className:"tag",
begin:/<style(?=\s|>)/,end:/>/,keywords:{name:"style"},contains:[c],starts:{
end:/<\/style>/,returnEnd:!0,subLanguage:"css"}},{className:"tag",
begin:/<script(?=\s|>)/,end:/>/,keywords:{name:"script"},contains:[c],starts:{
end:/<\/script>/,returnEnd:!0,subLanguage:"javascript"}},{className:"tag",
begin:/<>|<\/>/},{className:"tag",
begin:n.concat(/</,n.lookahead(n.concat(t,n.either(/\/>/,/>/,/\s/)))),
end:/\/?>/,contains:[{className:"name",begin:t,relevance:0,starts:c}]},{
className:"tag",begin:n.concat(/<\//,n.lookahead(n.concat(t,/>/))),contains:[{
className:"name",begin:t,relevance:0},{begin:/>/,relevance:0,endsParent:!0}]}]}
},grmr_yaml:e=>{
const n="true false yes no null",t="[\\w#;/?:@&=+$,.~*'()[\\]]+",a={
className:"string",relevance:0,variants:[{begin:/"/,end:/"/},{begin:/\S+/}],
contains:[e.BACKSLASH_ESCAPE,{className:"template-variable",variants:[{
begin:/\{\{/,end:/\}\}/},{begin:/%\{/,end:/\}/}]}]},i=e.inherit(a,{variants:[{
begin:/'/,end:/'/,contains:[{begin:/''/,relevance:0}]},{begin:/"/,end:/"/},{
begin:/[^\s,{}[\]]+/}]}),r={end:",",endsWithParent:!0,excludeEnd:!0,keywords:n,
relevance:0},s={begin:/\{/,end:/\}/,contains:[r],illegal:"\\n",relevance:0},o={
begin:"\\[",end:"\\]",contains:[r],illegal:"\\n",relevance:0},c=[{
className:"attr",variants:[{begin:/[\w*@][\w*@ :()\./-]*:(?=[ \t]|$)/},{
begin:/"[\w*@][\w*@ :()\./-]*":(?=[ \t]|$)/},{
begin:/'[\w*@][\w*@ :()\./-]*':(?=[ \t]|$)/}]},{className:"meta",
begin:"^---\\s*$",relevance:10},{className:"string",
begin:"[\\|>]([1-9]?[+-])?[ ]*\\n( +)[^ ][^\\n]*\\n(\\2[^\\n]+\\n?)*"},{
begin:"<%[%=-]?",end:"[%-]?%>",subLanguage:"ruby",excludeBegin:!0,excludeEnd:!0,
relevance:0},{className:"type",begin:"!\\w+!"+t},{className:"type",
begin:"!<"+t+">"},{className:"type",begin:"!"+t},{className:"type",begin:"!!"+t
},{className:"meta",begin:"&"+e.UNDERSCORE_IDENT_RE+"$"},{className:"meta",
begin:"\\*"+e.UNDERSCORE_IDENT_RE+"$"},{className:"bullet",begin:"-(?=[ ]|$)",
relevance:0},e.HASH_COMMENT_MODE,{beginKeywords:n,keywords:{literal:n}},{
className:"number",
begin:"\\b[0-9]{4}(-[0-9][0-9]){0,2}([Tt \\t][0-9][0-9]?(:[0-9][0-9]){2})?(\\.[0-9]*)?([ \\t])*(Z|[-+][0-9][0-9]?(:[0-9][0-9])?)?\\b"
},{className:"number",begin:e.C_NUMBER_RE+"\\b",relevance:0},s,o,{
className:"string",relevance:0,begin:/'/,end:/'/,contains:[{match:/''/,
scope:"char.escape",relevance:0}]},a],l=[...c]
;return l.pop(),l.push(i),r.contains=l,{name:"YAML",case_insensitive:!0,
aliases:["yml"],contains:c}}});const Ke=ne;for(const e of Object.keys(Pe)){
const n=e.replace("grmr_","").replace("_","-");Ke.registerLanguage(n,Pe[e])}
return Ke}()
;"object"==typeof exports&&"undefined"!=typeof module&&(module.exports=hljs);

/* === BidiCore.js === */
/**
 * BidiCore - Shared direction & BiDi analysis for WordHtmlBuilder, DocxBuilder and PdfBuilder
 * ==============================================================================
 *
 * Version: 1.5
 *
 * One engine, two renderers. Everything here DECIDES; nothing here renders:
 *
 *   detectDirection(root)            document-level direction ('rtl' | 'ltr')
 *   planDirections(root, docIsRTL)   ordered per-element direction decisions
 *   isComplexQuote(bq)               simple (paragraphs) vs table-form quote
 *   flattenBlock(el)                 a block's own text as ONE string + node map
 *   isMathAtom(node)                 a formula (.kelk-math, <svg>, <math>): an opaque LTR atom
 *   findIsolateRanges(text, dir, o)  counter-direction runs inside a block
 *   codeLanguage(pre)                code block language ('' if none)
 *   unwrapCodeBlocks(root)           strip preview wrappers (mutates)
 *   codeRtlRanges(text)              RTL runs inside code (UBA, LTR context)
 *   codeForWord(text)                code text with LRE/LRM marks for Word (no isolates there)
 *   nestedFences / codeSegments      fenced blocks written inside a code block
 *   codeFragment(code, from, to)     lines of a highlighted code element
 *
 * WordHtmlBuilder turns the decisions into inline CSS / <span dir>, DocxBuilder
 * into w:bidi paragraphs and w:rtl runs. Keeping the rules in one place means
 * both outputs always agree on what is RTL and what is LTR.
 *
 * All functions are pure with respect to the DOM (they read, never mutate),
 * except where noted. Browser-only parts need `document`.
 */

(function (global) {
    'use strict';

    // =========================================================================
    // Character classes — the single source of truth
    // =========================================================================

    /**
     * Liberal RTL test, used for DOCUMENT-level detection only:
     *   Arabic \u0600-\u06FF, \uFB50-\uFDFF, \uFE70-\uFEFF;
     *   Hebrew \u0590-\u05FF, \uFB1D-\uFB4F; ZWNJ \u200C.
     */
    const RTL_CHAR_PATTERN = /[\u0590-\u05FF\u0600-\u06FF\uFB1D-\uFDFF\uFE70-\uFEFF\u200C]/;

    /**
     * STRONG letter classes, shared by per-element direction and the BiDi run
     * engine so both always agree. Digits (ASCII, Arabic-Indic, Persian), ZWNJ,
     * marks and punctuation (، ؛ ؟ …) are intentionally NOT strong.
     *
     * RTL — the letters of every right-to-left script in the BMP (Unicode
     * Bidi_Class R / AL): Arabic (+ Supplement, Extended-A/B, presentation
     * forms), Hebrew (+ presentation forms), Syriac, Thaana, N'Ko, Samaritan,
     * Mandaic. Persian is just one user of it.
     */
    const RTL_LETTER_PATTERN = new RegExp('[' +
        '\\u0621-\\u063A\\u0641-\\u064A\\u0671-\\u06D3\\u06D5\\u06FA-\\u06FC' +   // Arabic, Persian, Urdu letters
        '\\u0750-\\u077F\\u0870-\\u088E\\u08A0-\\u08C9' +                            // Arabic Supplement, Extended-B/A
        '\\uFB50-\\uFDFF\\uFE70-\\uFEFF' +                                           // Arabic presentation forms A/B
        '\\u05BE\\u05C0\\u05C3\\u05C6\\u05D0-\\u05EA\\u05EF-\\u05F4' +                    // Hebrew letters (+ maqaf, geresh …)
        '\\uFB1D\\uFB1F-\\uFB28\\uFB2A-\\uFB4F' +                                     // Hebrew presentation forms
        '\\u0710\\u0712-\\u072F\\u074D-\\u074F' +                                     // Syriac
        '\\u0780-\\u07A5\\u07B1\\u07CA-\\u07EA' +                                      // Thaana, N'Ko
        '\\u0800-\\u0815\\u0840-\\u0858' +                                            // Samaritan, Mandaic
        ']');

    /**
     * LTR — letters of the left-to-right scripts a document here is likely to
     * mix with RTL text: Latin (incl. Extended-A/B, IPA, Extended Additional:
     * Vietnamese …), Greek (+ Extended), Cyrillic, Armenian, Georgian.
     * × and ÷ (U+00D7, U+00F7) sit inside Latin-1 but are symbols, not letters.
     * (Named LATIN_* for history; it is the generic strong-LTR letter class.)
     */
    const LATIN_LETTER_PATTERN = new RegExp('[' +
        'A-Za-z\\u00AA\\u00B5\\u00BA\\u00C0-\\u00D6\\u00D8-\\u00F6\\u00F8-\\u02AF' +       // Latin, IPA
        '\\u1E00-\\u1EFF' +                                                        // Latin Extended Additional
        '\\u0386\\u0388-\\u03FF\\u1F00-\\u1FBC\\u1FC2-\\u1FCC\\u1FD0-\\u1FDB\\u1FE0-\\u1FEC\\u1FF2-\\u1FFC' + // Greek
        '\\u0400-\\u0482\\u048A-\\u052F' +                                            // Cyrillic
        '\\u0531-\\u0556\\u0561-\\u0587' +                                            // Armenian
        '\\u10A0-\\u10FF' +                                                        // Georgian
        ']');

    /**
     * "Numbers-only" text: digits of any script (0-9, ٠-٩, ۰-۹) with common
     * separators ("1,234", "98.6%", "۱۴۰۵/۰۷/۰۹", "٣٫٥"). Kept for API users;
     * tables now go further: any cell without a letter follows its table.
     */
    const NUMBERS_ONLY_PATTERN = /^[\s0-9\u0660-\u0669\u06F0-\u06F9%\u066A-\u066C\u060C.,#\-–—\/:\\+×x()]+$/;

    /** Blocks the BiDi engine processes, and what a block walk must not enter. */
    const BLOCK_SEL = 'p, h1, h2, h3, h4, h5, h6, li, td, th, blockquote';
    const NON_DESCEND = /^(P|H1|H2|H3|H4|H5|H6|LI|TD|TH|BLOCKQUOTE|CODE|PRE)$/;

    /** Quote content a paragraph border cannot follow in one straight line. */
    const COMPLEX_QUOTE_SEL = 'ul, ol, blockquote, pre, table, hr, dl, figure';

    /**
     * A formula (MathCore's .kelk-math placeholder or its rendering, an <svg>
     * or <math>): an opaque left-to-right atom. Direction decisions, isolate
     * ranges and document detection never read inside it — its TeX lives in
     * data-tex, and the text an SVG draws is not document text.
     */
    function isMathAtom(n) {
        return !!n && n.nodeType === 1 && ((n.classList && n.classList.contains('kelk-math')) || /^(svg|math)$/i.test(n.tagName));
    }

    function hasRTL(text) { return RTL_CHAR_PATTERN.test(text); }
    function hasLatin(text) { return LATIN_LETTER_PATTERN.test(text); }
    /** Has a strong RTL letter (Arabic script, Hebrew …)? */
    function hasRtlLetter(text) { return RTL_LETTER_PATTERN.test(text); }
    function hasMix(t) { return LATIN_LETTER_PATTERN.test(t) && RTL_LETTER_PATTERN.test(t); }

    const RTL_LETTERS_G = new RegExp(RTL_LETTER_PATTERN.source, 'g');
    const LATIN_LETTERS_G = new RegExp(LATIN_LETTER_PATTERN.source, 'g');
    const STRONG_ANY = new RegExp(RTL_LETTER_PATTERN.source + '|' + LATIN_LETTER_PATTERN.source);

    /**
     * Should a paragraph NOT be justified? True when it holds a long unbreakable
     * run — a URL, a path, a long identifier — whatever the language: justifying
     * the line before it would stretch the spaces into gaps.
     * @param {string} text
     */
    function noJustify(text) {
        const t = String(text || '');
        return /(?:https?:\/\/|www\.)\S{16,}/i.test(t) || /\S{32,}/.test(t);
    }

    // =========================================================================
    // Table of contents (shared by the builders)
    // =========================================================================

    /** Default ToC title by document direction. */
    const TOC_TITLES = { rtl: 'فهرست مطالب', ltr: 'Contents' };

    /**
     * Insert a table of contents into a parsed document (MUTATES root) and give
     * every listed heading an id. The layout is the same in every output:
     *     <h1>            (when the document opens with one: its title)
     *     <hr>
     *     <div class="kelk-toc"> title + one entry per heading </div>
     *     <hr>
     *     <p class="kelk-spacer">  (an empty paragraph)
     * Without an opening H1 the ToC comes first and the first <hr> is left out.
     * Headings inside quotes, lists and tables are not listed. Each builder then
     * renders div.kelk-toc its own way (Word field, docx SDT, PDF page numbers,
     * HTML links); p.kelk-toc-entry carries data-level and data-target (the id).
     * @param {Element} root
     * @param {{ levels?: number, title?: string, dir?: 'rtl'|'ltr' }} opts
     * @returns {Array<{ id, level, text }>} the entries (empty → nothing inserted)
     */
    function insertToc(root, opts) {
        const o = opts || {};
        const levels = Math.max(1, Math.min(6, parseInt(o.levels, 10) || 2));
        const sel = Array.from({ length: levels }, function (_, i) { return 'h' + (i + 1); }).join(',');
        const doc = root.ownerDocument;
        let n = 0;
        const entries = [];
        root.querySelectorAll(sel).forEach(function (h) {
            if (h.closest('blockquote, li, td, th, pre')) return;
            const text = h.textContent.replace(/\s+/g, ' ').trim();
            if (!text) return;
            if (!h.id) h.id = '_Toc' + (++n);
            entries.push({ id: h.id, level: +h.tagName.charAt(1), text: text });
        });
        if (!entries.length) return entries;

        const box = doc.createElement('div');
        box.className = 'kelk-toc';
        box.setAttribute('data-levels', String(levels));
        const title = doc.createElement('p');
        title.className = 'kelk-toc-title';
        const side = o.dir === 'ltr' ? 'ltr' : 'rtl';
        const own = o.title && typeof o.title === 'object' ? o.title[side] : o.title;     // text, or { rtl, ltr }
        title.textContent = own || TOC_TITLES[side];
        box.appendChild(title);
        entries.forEach(function (e) {
            const p = doc.createElement('p');
            p.className = 'kelk-toc-entry';
            p.setAttribute('data-level', String(e.level));
            p.setAttribute('data-target', e.id);
            p.textContent = e.text;
            box.appendChild(p);
        });
        const hr = function () { return doc.createElement('hr'); };
        const spacer = doc.createElement('p');
        spacer.className = 'kelk-spacer';
        spacer.innerHTML = '&nbsp;';

        let first = root.firstElementChild;
        const frag = doc.createDocumentFragment();
        if (first && first.tagName === 'H1') {
            frag.appendChild(hr()); frag.appendChild(box); frag.appendChild(hr()); frag.appendChild(spacer);
            // an <hr> right after the title in the text itself is not doubled
            const next = first.nextElementSibling;
            if (next && next.tagName === 'HR') next.parentNode.removeChild(next);
            first.parentNode.insertBefore(frag, first.nextSibling);
        } else {
            frag.appendChild(box); frag.appendChild(hr()); frag.appendChild(spacer);
            root.insertBefore(frag, root.firstChild);
        }
        return entries;
    }

    // =========================================================================
    // Table column widths (shared by the builders)
    // =========================================================================

    /**
     * Distribute a table width over columns from their min/max content widths
     * (the browser's auto layout in short): everything fits → by max; nothing
     * fits → by min; else min plus the rest by (max − min).
     * @returns {number[]} widths summing to `total`
     */
    function distributeColumns(minC, maxC, total) {
        const sumMin = minC.reduce(function (s, x) { return s + x; }, 0);
        const sumMax = maxC.reduce(function (s, x) { return s + x; }, 0);
        if (sumMax <= total) return maxC.map(function (x) { return x * total / sumMax; });
        if (sumMin >= total) return minC.map(function (x) { return x * total / sumMin; });
        const extra = total - sumMin, span = sumMax - sumMin;
        return minC.map(function (x, k) { return x + extra * (maxC[k] - x) / span; });
    }

    /**
     * Table width from the setting: 'NN%' of the available width (default
     * 98%), or 'auto' — as wide as its content wants, up to 100%, and at least
     * `min` of it (a table of two numbers is not a sliver).
     * @param {string} setting  '98%' | 'auto' | …
     * @param {number} full     available width
     * @param {number} natural  the content's preferred width (sum of max widths)
     * @param {string} [min]    'auto' only: the least width, 'NN%' of `full` (default '20%')
     */
    function resolveTableWidth(setting, full, natural, min) {
        const s = String(setting == null ? '98%' : setting).trim().toLowerCase();
        if (s === 'auto') {
            const m = parseFloat(min == null ? '20%' : min);
            const least = full * (isFinite(m) && m > 0 ? Math.min(100, m) : 20) / 100;
            return Math.min(full, Math.max(natural || 0, least));
        }
        const p = parseFloat(s);
        return full * (isFinite(p) && p > 0 ? Math.min(100, p) : 98) / 100;
    }

    /** Text of a cell with <br> as line breaks, code included. */
    function cellLines(cell, em) {
        let t = '';
        // a formula or an image: one unbreakable "word" as wide as it is (em: points per character)
        const atom = function (pt) { return pt > 0 && em > 0 ? ' ' + 'x'.repeat(Math.max(1, Math.ceil(pt / em))) + ' ' : ''; };
        (function walk(n) {
            for (let c = n.firstChild; c; c = c.nextSibling) {
                if (c.nodeType === 3) t += c.nodeValue;
                else if (c.nodeType === 1) {
                    if (c.tagName === 'IMG') { t += atom((parseFloat(c.getAttribute('width')) || 0) * 0.75); continue; }
                    if (isMathAtom(c)) {
                        // width in em (MathCore.typeset writes it), else a guess from the TeX
                        const fs = em / 0.6, tex = c.getAttribute('data-tex') || '';
                        const w = parseFloat(c.getAttribute('data-width')) || tex.replace(/\\[a-zA-Z]+|[{}^_\s]/g, '').length * 0.55;
                        t += atom(w * 1.2 * fs);
                        continue;
                    }
                    if (c.tagName === 'BR') t += '\n';
                    else if (c.tagName !== 'TABLE') { if (/^(P|DIV|LI|H[1-6])$/.test(c.tagName) && t && !/\n$/.test(t)) t += '\n'; walk(c); }
                }
            }
        })(cell);
        return t.split('\n').map(function (l) { return l.replace(/\s+/g, ' ').trim(); });
    }

    /**
     * Column shares of a table from its text (no fonts needed): a character is
     * about half an em, a line is as wide as its characters, the narrowest a
     * column may be is its longest word. Same algorithm as PdfBuilder, which
     * measures with the real fonts.
     * @param {HTMLTableElement} table
     * @param {{ width?: number, fontSize?: number, pad?: number, tableWidth?: string, tableMinWidth?: string }} [opts] - points;
     *        tableWidth '98%' | 'auto' … and tableMinWidth (resolveTableWidth) → fraction of `width`
     * @returns {{ shares: number[], cols: Map<Element, number> }} shares sum to 1;
     *          cols: first column index of each cell
     */
    function tableColumnShares(table, opts) {
        const o = Object.assign({ width: 450, fontSize: 11, pad: 16 }, opts || {});
        const em = o.fontSize * 0.6;              // a little generous: bold headers, cell margins
        const rows = Array.from(table.rows);
        const occ = [], cells = [], cols = new Map();
        rows.forEach(function (tr, r) {
            let c = 0;
            Array.from(tr.cells).forEach(function (cell) {
                while (occ[r] && occ[r][c]) c++;
                const span = Math.max(1, cell.colSpan || 1), rs = Math.max(1, cell.rowSpan || 1);
                for (let i = 0; i < rs; i++) for (let k = 0; k < span; k++) { (occ[r + i] = occ[r + i] || [])[c + k] = true; }
                cols.set(cell, c);
                cells.push({ col: c, span: span, el: cell });
                c += span;
            });
        });
        const n = Math.max(1, occ.reduce(function (m, r) { return Math.max(m, (r || []).length); }, 0));
        const base = o.pad + em * 2;
        const minC = Array(n).fill(base), maxC = Array(n).fill(base);
        // Arabic-script letters run wider than the 0.6 em average, and a header cell is bold
        const RTLW = /[\u0590-\u08FF\uFB1D-\uFDFF\uFE70-\uFEFF]/;
        const len = function (t, bold) {
            let n = 0;
            for (const ch of t) n += RTLW.test(ch) ? 1.15 : 1;
            return n * (bold ? 1.1 : 1);
        };
        cells.forEach(function (c) {
            const lines = cellLines(c.el, em);
            const bold = c.el.tagName === 'TH';
            const maxW = Math.max.apply(null, lines.map(function (l) { return len(l, bold); }).concat([0])) * em + o.pad;
            const minW = Math.max.apply(null, lines.join(' ').split(/\s+/).map(function (w) { return len(w, bold); }).concat([0])) * em + o.pad;
            for (let k = 0; k < c.span; k++) {
                minC[c.col + k] = Math.max(minC[c.col + k], minW / c.span);
                maxC[c.col + k] = Math.max(maxC[c.col + k], maxW / c.span);
            }
        });
        const natural = maxC.reduce(function (s, x) { return s + x; }, 0);
        const width = o.tableWidth ? resolveTableWidth(o.tableWidth, o.width, natural, o.tableMinWidth) : o.width;
        const w = distributeColumns(minC, maxC, width);
        const total = w.reduce(function (s, x) { return s + x; }, 0) || 1;
        // fraction: the table's width as a part of the available width
        return { shares: w.map(function (x) { return x / total; }), cols: cols, fraction: width / o.width, natural: natural };
    }

    /** Direction of the first strong letter ('rtl' | 'ltr' | null) — UBA rule P2. */
    function firstStrong(t) {
        const m = STRONG_ANY.exec(t);
        if (!m) return null;
        return RTL_LETTER_PATTERN.test(m[0]) ? 'rtl' : 'ltr';
    }

    /**
     * Is this block text LTR? True when it has LTR letters and either no
     * RTL letter, or it STARTS with an LTR letter AND LTR letters are
     * the majority — an English sentence quoting a few Persian words
     * ("… with «شد» and «می‌شود» …") is English, while a Persian sentence
     * with a long English phrase inside stays Persian.
     */
    function isLtrText(t) {
        if (!LATIN_LETTER_PATTERN.test(t)) return false;
        if (!RTL_LETTER_PATTERN.test(t)) return true;
        if (firstStrong(t) !== 'ltr') return false;
        return (t.match(LATIN_LETTERS_G) || []).length > (t.match(RTL_LETTERS_G) || []).length;
    }

    /** Is this block text RTL (has RTL letters and is not LTR by isLtrText)? */
    function isRtlText(t) {
        return RTL_LETTER_PATTERN.test(t) && !isLtrText(t);
    }

    // =========================================================================
    // Text extraction
    // =========================================================================

    /**
     * Visible text of an element, excluding <pre> and <code> subtrees.
     * @param {Element} el
     * @param {WeakMap} [memo] - optional cache (valid only while text is unchanged)
     * @returns {string}
     */
    function getVisibleText(el, memo) {
        if (memo) {
            const hit = memo.get(el);
            if (hit !== undefined) return hit;
        }
        let text = '';
        const childNodes = el.childNodes;
        for (let i = 0; i < childNodes.length; i++) {
            const node = childNodes[i];
            if (node.nodeType === 3) {
                text += node.nodeValue;
            } else if (node.nodeType === 1) {
                const tag = node.tagName.toLowerCase();
                if (tag === 'pre' || tag === 'code' || isMathAtom(node)) continue;
                text += getVisibleText(node, memo);
            }
        }
        if (memo) memo.set(el, text);
        return text;
    }

    /**
     * Document-level direction: the direction of the FIRST block that has
     * letters (p, h1-h6, li, blockquote — its own text, code excluded; table
     * cells only when there is no such block), decided like every block
     * (isRtlText). This is the UBA's own
     * rule (P2, as dir="auto" does) lifted from the first character to the
     * first block, so "API یک رابط …" or "# ISO 27001 گزارش ممیزی" still
     * open an RTL document, and an English document keeps its Persian words,
     * quotes and cells without turning RTL. No letters at all: any RTL
     * character → rtl. (Before: any RTL character made the document RTL.)
     * An explicit setDirection('rtl'|'ltr') always wins over this.
     * @param {Element|string} source - parsed root (browser) or raw HTML (fallback)
     * @returns {'rtl'|'ltr'}
     */
    function detectDirection(source) {
        const decide = function (t) {
            if (!hasRtlLetter(t) && !hasLatin(t)) return null;
            return isRtlText(t) ? 'rtl' : 'ltr';
        };
        if (typeof source !== 'string') {
            if (source.querySelectorAll) {
                // blocks and loose top-level text, in document order
                const units = Array.from(source.querySelectorAll(BLOCK_SEL + ', pre'));
                for (let n = source.firstChild; n; n = n.nextSibling) if (n.nodeType === 3) units.push(n);
                units.sort(function (x, y) { return x === y ? 0 : (x.compareDocumentPosition(y) & 4 ? -1 : 1); });
                // prose first; table cells (often codes, names, numbers) only if there is no prose
                for (let pass = 0; pass < 2; pass++) {
                    for (let k = 0; k < units.length; k++) {
                        const u = units[k];
                        if (u.nodeType === 1 && (u.tagName === 'PRE' || u.closest('pre'))) continue;
                        if (pass === 0 && u.nodeType === 1 && u.closest('td, th')) continue;
                        const d = decide(u.nodeType === 3 ? u.nodeValue : flattenBlock(u).text);
                        if (d) return d;
                    }
                }
            }
            return hasRTL(getVisibleText(source)) ? 'rtl' : 'ltr';
        }
        const stripped = source
            .replace(/<pre[\s>][\s\S]*?<\/pre>/gi, '')
            .replace(/<code[\s>][\s\S]*?<\/code>/gi, '');
        const prose = stripped.replace(/<table[\s>][\s\S]*?<\/table>/gi, '');
        const tries = [prose, stripped];
        for (let t = 0; t < tries.length; t++) {
            const parts = tries[t].split(/<\/?(?:p|h[1-6]|li|ul|ol|td|th|tr|table|blockquote|div)\b[^>]*>|<br\s*\/?>|\n/i);
            for (let k = 0; k < parts.length; k++) {
                const d = decide(parts[k].replace(/<[^>]+>/g, ''));
                if (d) return d;
            }
        }
        return hasRTL(stripped.replace(/<[^>]+>/g, '')) ? 'rtl' : 'ltr';
    }

    // =========================================================================
    // Per-element direction plan
    // =========================================================================

    /**
     * Decide per-element direction for a document with mixed content.
     * Returns an ORDERED list of actions (renderers apply them in order):
     *
     *   { op: 'ltr',      el }          block/cell/list/li is LTR
     *   { op: 'rtl',      el }          RTL paragraph inside an LTR quote
     *   { op: 'quoteDir', el, dir }     counter-direction blockquote
     *   { op: 'ltrTable', el }          whole table is LTR (column order too)
     *
     * Rules:
     *   1. p, h1-h6 (outside lists/tables/quotes/pre): LTR unless RTL by
     *      isRtlText (RTL letters, and not an LTR-led, LTR-majority text);
     *      in an LTR document the mirror: RTL by isRtlText → explicit RTL.
     *   2. blockquote: counter-direction quotes get quoteDir; their paragraphs
     *      follow. In an RTL quote, English-only paragraphs → LTR; in an LTR
     *      quote of an LTR document, Persian paragraphs → RTL.
     *   3. tables: cells with RTL letters stay (explicit RTL in an LTR
     *      document); cells without any letter (digits of every script, signs,
     *      symbols — no code or formula) take the table's direction; every
     *      other cell → LTR. A table without RTL letters → ltrTable.
     *   4. lists: each item by its OWN text (nested lists, tables, quotes and
     *      code excluded), strong letters only. No RTL letter in any item →
     *      whole tree LTR; otherwise LTR-only items (and English-only
     *      paragraphs of RTL items) → LTR, RTL items under an LTR parent (or
     *      in an LTR document) → explicit RTL, numbers-only items follow their
     *      parent. An item's own paragraphs (loose lists) follow the item.
     *
     * Decisions depend on text only, so planning first and applying afterwards
     * is equivalent to deciding while mutating.
     *
     * @param {Element} root
     * @param {boolean} docIsRTL
     * @returns {Array<object>}
     */
    function planDirections(root, docIsRTL) {
        const memo = new WeakMap();
        const actions = [];
        const text = function (el) { return getVisibleText(el, memo); };
        const elementHasRTL = function (el) { return isRtlText(text(el)); };
        /** A table cell with no letter (digits, signs, symbols, empty) and no code, formula or image. */
        const neutralCell = function (el) {
            const t = text(el);
            if (RTL_LETTER_PATTERN.test(t) || LATIN_LETTER_PATTERN.test(t)) return false;
            return !el.querySelector('code, pre, img, .kelk-math, mjx-container');
        };
        const ltr = function (el) { actions.push({ op: 'ltr', el: el }); };

        // An LTR list item and its own paragraphs (loose items: <li><p>…</p>).
        // In Word HTML a <p class=MsoNormal> does NOT inherit the <li>'s
        // direction — the class sets the document direction explicitly — so
        // the paragraphs need their own decision.
        function ltrItem(li) {
            ltr(li);
            for (let c = li.firstElementChild; c; c = c.nextElementSibling) {
                if (/^(P|H[1-6])$/.test(c.tagName)) ltr(c);
            }
        }

        // English-only direct child paragraphs of an RTL container
        function ltrOnlyChildBlocks(container) {
            for (let c = container.firstElementChild; c; c = c.nextElementSibling) {
                if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
                if (isLtrText(text(c))) ltr(c);
            }
        }
        // Persian direct child paragraphs of an LTR container (LTR document)
        function rtlOnlyChildBlocks(container) {
            for (let c = container.firstElementChild; c; c = c.nextElementSibling) {
                if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
                if (isRtlText(text(c))) actions.push({ op: 'rtl', el: c });
            }
        }

        // 1. Standalone blocks
        root.querySelectorAll('p, h1, h2, h3, h4, h5, h6').forEach(function (el) {
            if (el.closest('pre')) return;
            if (el.closest('ul') || el.closest('ol')) return;
            if (el.closest('td') || el.closest('th')) return;
            if (el.closest('blockquote')) return;
            // the empty paragraph after the table of contents (insertToc): the document's
            // direction, as the empty paragraphs the Word builders put at its start and end
            if (el.classList && el.classList.contains('kelk-spacer')) return;
            // a paragraph holding only images (or formulas) keeps the document direction
            // (an image in an RTL document sits at the right, as in the preview)
            if (!/\S/.test(getVisibleText(el)) && el.querySelector('img, .kelk-math')) return;
            const r = elementHasRTL(el);
            if (!r) ltr(el);
            else if (!docIsRTL) actions.push({ op: 'rtl', el: el });        // Persian paragraph in an LTR document
        });

        // 2. Blockquotes
        root.querySelectorAll('blockquote').forEach(function (bq) {
            const bqHasRTL = elementHasRTL(bq);
            if (docIsRTL && !bqHasRTL) {
                actions.push({ op: 'quoteDir', el: bq, dir: 'ltr' });
                bq.querySelectorAll('p, h1, h2, h3, h4, h5, h6').forEach(function (el) {
                    // a Persian paragraph in an English quote stays RTL — explicitly,
                    // since renderers give children the quote's direction
                    if (isRtlText(text(el))) actions.push({ op: 'rtl', el: el });
                    else ltr(el);
                });
            } else if (!docIsRTL && bqHasRTL) {
                actions.push({ op: 'quoteDir', el: bq, dir: 'rtl' });
                ltrOnlyChildBlocks(bq);
            } else if (docIsRTL && bqHasRTL) {
                ltrOnlyChildBlocks(bq);
            } else {
                rtlOnlyChildBlocks(bq);                        // LTR quote of an LTR document
            }
        });

        // 3. Tables
        root.querySelectorAll('table').forEach(function (table) {
            if (table.id === 'hrdftrtbl') return;
            const tableHasRTL = hasRtlLetter(text(table));    // column order: any RTL letter
            table.querySelectorAll('td, th').forEach(function (cell) {
                if (elementHasRTL(cell)) {
                    if (!docIsRTL) actions.push({ op: 'rtl', el: cell });   // Persian cell in an LTR document
                    return;
                }
                // no letter at all (digits of any script, signs, symbols, empty) and no code or
                // formula: the cell takes the TABLE's direction, in every table and document
                if (neutralCell(cell)) { if (!tableHasRTL) ltr(cell); return; }
                ltr(cell);
            });
            if (!tableHasRTL) actions.push({ op: 'ltrTable', el: table });
        });

        // 4. Lists. An item is decided by its OWN text — nested lists, tables,
        // quotes and code are decided on their own and do not vote for it.
        // Only strong letters count: a Persian comma or ؟ in an English item
        // does not make the list RTL.
        function ownText(li) {
            let t = '';
            li.childNodes.forEach(function (n) {
                if (n.nodeType === 3) t += n.nodeValue;
                else if (n.nodeType === 1 && !/^(UL|OL|TABLE|BLOCKQUOTE|PRE)$/.test(n.tagName)) t += text(n);
            });
            return t;
        }
        // An RTL item where the inherited direction is LTR (LTR document, or
        // inside an LTR item) says so explicitly; its own paragraphs too.
        function rtlItem(li) {
            actions.push({ op: 'rtl', el: li });
            for (let c = li.firstElementChild; c; c = c.nextElementSibling) {
                if (/^(P|H[1-6])$/.test(c.tagName) && !isLtrText(text(c))) actions.push({ op: 'rtl', el: c });
            }
        }
        root.querySelectorAll('ul, ol').forEach(function (list) {
            if (list.parentNode.closest('ul, ol')) return;
            const allLis = Array.from(list.querySelectorAll('li'));
            const own = new Map();
            allLis.forEach(function (li) { own.set(li, ownText(li)); });
            const treeHasRTL = allLis.some(function (li) { return hasRtlLetter(own.get(li)); });
            if (!treeHasRTL) {
                ltr(list);
                list.querySelectorAll('ul, ol').forEach(ltr);
                allLis.forEach(ltrItem);
                return;
            }
            // Mixed tree. Document order puts parents before children.
            const isLtr = new Set();
            allLis.forEach(function (li) {
                const t = own.get(li);
                const parentLi = li.parentElement ? li.parentElement.closest('li') : null;
                const inheritedLtr = (parentLi && list.contains(parentLi)) ? isLtr.has(parentLi) : !docIsRTL;
                if (hasLatin(t) && !hasRtlLetter(t)) {
                    isLtr.add(li); ltrItem(li);
                } else if (hasRtlLetter(t)) {
                    if (inheritedLtr) rtlItem(li);
                    ltrOnlyChildBlocks(li);
                } else if (inheritedLtr) {
                    isLtr.add(li); ltrItem(li);              // numbers only: follow the parent
                }
            });
        });

        return actions;
    }

    /**
     * Collapse a plan into element → direction. For renderers that need a
     * lookup (DocxBuilder) rather than ordered style mutations.
     * @param {Array<object>} actions
     * @returns {{ dir: Map<Element,'rtl'|'ltr'>, ltrTables: Set<Element> }}
     */
    function planToMaps(actions) {
        const dir = new Map();
        const ltrTables = new Set();
        actions.forEach(function (a) {
            if (a.op === 'ltr') dir.set(a.el, 'ltr');
            else if (a.op === 'rtl') dir.set(a.el, 'rtl');
            else if (a.op === 'quoteDir') dir.set(a.el, a.dir);
            else if (a.op === 'ltrTable') ltrTables.add(a.el);
        });
        return { dir: dir, ltrTables: ltrTables };
    }

    /**
     * Does this blockquote need the table form?
     *   - it contains lists, nested quotes, code, tables … (own indents), or
     *   - (RTL document) its paragraphs end up in DIFFERENT directions — an
     *     English paragraph in a Persian quote, or a Persian one in an English
     *     quote. A paragraph border is
     *     placed relative to each paragraph's own direction/indent, so an RTL
     *     and an LTR paragraph never share one straight border line; a table
     *     cell border does.
     * Mirrors rule 2 of planDirections (ltrOnlyChildBlocks).
     * @param {Element} bq
     * @param {boolean} [docIsRTL]
     */
    function isComplexQuote(bq, docIsRTL) {
        if (bq.querySelector(COMPLEX_QUOTE_SEL)) return true;
        if (!docIsRTL) return false;
        const quoteRtl = isRtlText(getVisibleText(bq));
        for (let c = bq.firstElementChild; c; c = c.nextElementSibling) {
            if (!/^(P|H[1-6])$/.test(c.tagName)) continue;
            const t = getVisibleText(c);
            // a paragraph whose direction differs from the quote's
            if (quoteRtl ? isLtrText(t) : isRtlText(t)) return true;
        }
        return false;
    }

    // =========================================================================
    // BiDi run engine (see WordHtmlBuilder._wrapBidiRuns for the full rationale)
    // =========================================================================

    // Conservative STRONG classes for BiDi: LETTERS only. Persian digits
    // (U+06F0..F9), ZWNJ (U+200C), and Persian punctuation are intentionally
    // NOT strong here, so an English line containing them is still single-script.
    const STRONG_R = RTL_LETTER_PATTERN;
    const STRONG_L = LATIN_LETTER_PATTERN;
    const FA_PUNCT = /[\u060C\u061B\u061F]/; // ، ؛ ؟ — RTL-owned hard boundary
    // Marks / joiners that stay with the RTL letter before them (code runs)
    const CODE_RUN_TAIL = /[\u0591-\u05C7\u0610-\u061A\u064B-\u065F\u0670\u06D6-\u06ED\u08D3-\u08FF\u200C\u200D]/;
    // Digits glued (no space) to the FRONT of a Latin word belong to that
    // LTR word: "4k", "3.5GHz", "1,000mAh", "4-bit", "2_x". Internal . or ,
    // only between digits; one optional - or _ as a digit→letter joiner.
    const DIGIT_PREFIX = /[0-9]+(?:[.,][0-9]+)*[-_]?$/;
    // Math operators (∑ √ ∫ ∂ ∇ ≤ ≠ ∈ …, ± × ÷) opening a formula before its
    // first Latin letter. ASCII operators and arrows do not count: "قیمت = Price",
    // "انجام شد → next" keep their plain UBA behavior.
    const MATH_START = /[\u2200-\u22FF\u2A00-\u2AFF\u00B1\u00D7\u00F7]/;
    // An opening quote glued to the front of the counter-direction run
    // ("Future …" / "حرم …") belongs INSIDE the isolate; left outside at a
    // block start it resolves to the base direction and jumps to the far
    // edge. Pulling it in also lets balanceCore see the pair's real
    // parity, so a following ` / "` is trimmed instead of absorbed.
    // Guillemets « » are excluded on purpose (Persian-owned, mirrored).
    const OPEN_QUOTE = /["'\u201C\u2018]$/;
    const OPEN  = { '(':')', '[':']', '{':'}', '\u00AB':'\u00BB', '\u201C':'\u201D', '\u2039':'\u203A' };
    const CLOSE = { ')':'(', ']':'[', '}':'{', '\u00BB':'\u00AB', '\u201D':'\u201C', '\u203A':'\u2039' };
    const SYM   = { '"':1, "'":1, '\u0060':1, '\u2018':1, '\u2019':1 };

    function hasL(t){ return STRONG_L.test(t); }
    function hasR(t){ return STRONG_R.test(t); }
    function hasMix(t){ return STRONG_L.test(t) && STRONG_R.test(t); }
    function isOppStrong(c, baseDir){ return baseDir === 'rtl' ? STRONG_L.test(c) : STRONG_R.test(c); }
    function isBaseStrong(c, baseDir){ return baseDir === 'rtl' ? STRONG_R.test(c) : STRONG_L.test(c); }
    /** Any counter-direction letter in text[from, to)? */
    function STRONG_L_OR_R_IN(text, from, to, baseDir){
        for (let k = from; k < to; k++) if (isOppStrong(text[k], baseDir)) return true;
        return false;
    }

    // A lone apostrophe touching a letter/digit on EITHER side ("org's",
    // "workers'", "'90s") is a contraction, possessive, or elision mark, not
    // a paired quotation delimiter — the only case with a letter/digit on
    // NEITHER side would be a truly orphaned apostrophe, which doesn't occur
    // in real text. Gate 3 exists to push an unmatched enclosure OUTSIDE the
    // isolate so the UBA can pair it with its real partner elsewhere; but for
    // this kind of apostrophe there is no real partner, so trimming here does
    // the opposite of that goal — it splits one logical LTR word/phrase into
    // two separate <span dir="ltr"> runs with a bare RTL character between
    // them, which Word's bidi engine can then reorder relative to each other,
    // scrambling the on-screen reading order. Same failure mode the
    // block-level flattening below guards against — any artificial split of
    // one logical direction-run, whether caused by a symbol-pairing heuristic
    // or an inline tag boundary, risks reordering.
    // Scoped to ' and ’ (U+2019, the "smart" apostrophe Word/typography tools
    // commonly substitute for it) — " ` ‘ are left untouched since they never
    // occur mid-word or word-adjacent this way.
    function isNonPairingApostrophe(c, region, i){
        if (c !== "'" && c !== '\u2019') return false;
        const isWordChar = function(ch){ return !!ch && /[A-Za-z0-9]/.test(ch); };
        return isWordChar(region[i - 1]) || isWordChar(region[i + 1]);
    }

    // Trim a region down to an enclosure-balanced core (Gate 3).
    function balanceCore(region){
        let start = 0, end = region.length;
        function check(s, e){
            const stack = [], sym = {};
            for (let i = s; i < e; i++){
                const c = region[i];
                if (OPEN[c]) stack.push(i);
                else if (CLOSE[c]){ if (stack.length) stack.pop(); else return { ok:false, bad:i }; }
                else if (SYM[c] && !isNonPairingApostrophe(c, region, i)) sym[c] = (sym[c] || 0) + 1;
            }
            if (stack.length) return { ok:false, bad: stack[stack.length - 1] };
            for (const k in sym){
                if (sym[k] % 2 === 1){
                    for (let t = e - 1; t >= s; t--){
                        if (region[t] === k && !isNonPairingApostrophe(k, region, t)) return { ok:false, bad:t };
                    }
                }
            }
            return { ok:true };
        }
        while (start < end){ const r = check(start, end); if (r.ok) break; end = r.bad; }
        while (start < end && CLOSE[region[start]]) start++;
        return { start: start, end: end };
    }

    // Find counter-direction isolate spans in a block's flattened text.
    // Operates on ONE continuous string per block (built by the caller below)
    // rather than per DOM text node. Returns [start, end) ranges in TEXT's
    // own coordinates.
    function findIsolateRangesImpl(text, baseDir, endRe){
        const hasOpp = (baseDir === 'rtl') ? STRONG_L.test(text) : STRONG_R.test(text);
        if (!hasOpp) return [];
        const ranges = [];
        const n = text.length;
        let i = 0;
        let scanFrom = 0; // end of the previous region — backtracking never crosses it
        while (i < n){
            const ch = text[i];
            if (!isOppStrong(ch, baseDir)){ i++; continue; }
            let j = i, region = '';
            // Digit-prefix rule (RTL base only): pull digits attached to the
            // first Latin letter INTO the isolate. Otherwise "4k" becomes
            // "4" (outside, resolved as Arabic number after Persian letters →
            // placed to the RIGHT of k) + <span dir=ltr>k</span>, and Word
            // shows "k4". Trailing digits ("k4", "USB3") were already absorbed
            // by the greedy scan below; this closes the leading-digit gap.
            // Formula rule (RTL base only): "مجموع: ∑ (i=1 → n) i = n(n+1)/2".
            // A math operator between the last RTL letter (or ، ؛ ؟) and the
            // first Latin letter starts the formula: the run begins THERE, so
            // the whole formula is one LTR isolate. Otherwise the run starts
            // at "i", its "(" is left outside, the balance guard cuts at ")"
            // and the rest of the formula is reordered ("n)n+1(/2").
            if (baseDir === 'rtl'){
                let b = scanFrom;
                for (let k = i - 1; k >= scanFrom; k--){
                    if (isBaseStrong(text[k], baseDir) || FA_PUNCT.test(text[k])){ b = k + 1; break; }
                }
                const at = text.slice(b, i).search(MATH_START);
                if (at >= 0){ region = text.slice(b + at, i); i = b + at; }
            }
            if (baseDir === 'rtl' && !region){
                const dp = text.slice(scanFrom, i).match(DIGIT_PREFIX);
                if (dp){ region = dp[0]; i -= dp[0].length; }
            }
            // Number-before-RTL rule (LTR base only): "Summary (3 صفحه تا …)".
            // A number separated only by spaces from a Persian phrase, and not
            // itself following a Latin word ("Chapter 3 صفحه" keeps its 3),
            // is read with the phrase: it joins the RTL run, so it stands at the
            // phrase's start (its right side) instead of before it on the left.
            if (baseDir === 'ltr' && !region){
                const nm = /[0-9\u0660-\u0669\u06F0-\u06F9]+(?:[.,\u066B\u066C][0-9\u0660-\u0669\u06F0-\u06F9]+)*[ \t\u00A0]+$/.exec(text.slice(scanFrom, i));
                if (nm){
                    let k = i - nm[0].length - 1;
                    while (k >= scanFrom && /[ \t\u00A0]/.test(text[k])) k--;
                    if (k < scanFrom || !(isBaseStrong(text[k], baseDir) || /[0-9]/.test(text[k]))){
                        region = nm[0]; i -= nm[0].length;
                    }
                }
            }
            // Opening-quote prefix (both directions): only when the quote
            // itself starts a word (block start, whitespace or punctuation
            // before it — not a letter/digit, which would make it a
            // closing quote or an apostrophe).
            if (i > scanFrom && OPEN_QUOTE.test(text[i - 1]) &&
                (i - 1 === 0 || !(/[0-9\u0600-\u06FF]/.test(text[i - 2]) || STRONG_ANY.test(text[i - 2])))){
                region = text[i - 1] + region; i -= 1;
            }
            j = i + region.length;
            while (j < n){
                const c = text[j];
                if (isBaseStrong(c, baseDir)) break;
                if (baseDir === 'rtl' && FA_PUNCT.test(c)) break;
                region += c; j++;
            }
            const core = balanceCore(region);
            let keepStart = i + core.start;
            let keepEnd = i + core.end;

            // Boundary punctuation fix: a trailing cluster of dashes, spaces, and
            // separator/connector punctuation (: ; . / \ | =) that sits exactly at the
            // LTR→RTL handoff must stay OUTSIDE the isolate — these characters function
            // as a label/item separator binding to what FOLLOWS (the Farsi phrase), not
            // what precedes (e.g. "B8." / "B8:" / "B8 / " / "B8 = " all read as
            // "item B8, separator, Farsi label"). Comma is deliberately excluded: unlike
            // these separator marks, a comma overwhelmingly binds to the PRECEDING word
            // in ordinary prose (lists, clauses), so trimming it here would risk pulling
            // a legitimate trailing comma out of real English text merely because Farsi
            // happens to follow — a decision to add it should wait for a concrete
            // failing example, not be inferred by analogy.
            // Guard: only trim when what follows leads back to the base direction.
            let nextIsRtl;
            if (j < n){
                nextIsRtl = isBaseStrong(text[j], baseDir) || (baseDir === 'rtl' && FA_PUNCT.test(text[j]));
            } else {
                nextIsRtl = false; // true end of block content — nothing follows, don't trim
            }
            if (nextIsRtl){
                const dm = text.slice(keepStart, keepEnd).match(/[\s\u2010-\u2015\u2212\-\u2013\u2014:;.\/\\|=]+$/);
                if (dm) keepEnd -= dm[0].length;
            }
            const tm = text.slice(keepStart, keepEnd).match(/(\s+)$/);
            if (tm) keepEnd -= tm[1].length;
            const lm = text.slice(keepStart, keepEnd).match(/^(\s+)/);
            if (lm) keepStart += lm[1].length;

            if (keepEnd > keepStart) ranges.push([keepStart, keepEnd]);
            // The balance guard cut the run short: scan the rest of it again,
            // so later Latin text in the same stretch still gets its isolate.
            const cut = i + core.end;
            i = (keepEnd > keepStart && cut < j && STRONG_L_OR_R_IN(text, cut, j, baseDir)) ? Math.max(keepEnd, cut) : j;
            scanFrom = i;
        }

        // End-punctuation rule: only the LAST isolate, only when nothing but
        // trailing whitespace follows it to the end of the block's text.
        if (baseDir === 'rtl' && endRe && ranges.length){
            const last = ranges[ranges.length - 1];
            if (/^\s*$/.test(text.slice(last[1]))){
                const inner = text.slice(last[0], last[1]);
                const m = inner.match(endRe);
                if (m && m[1]) last[1] = last[0] + m[1].length;
            }
        }

        return ranges;
    }

    /**
     * RTL runs inside CODE (an LTR context: inline code, code block lines).
     * Plain UBA for an LTR paragraph, as editors and Word show code: a run
     * goes from an RTL letter to the LAST RTL letter before the next LTR
     * letter (neutrals and numbers BETWEEN RTL letters belong to it — rule
     * N1), plus the marks / ZWNJ right after that letter. Neutrals at either
     * edge stay LTR, so `// توضیح`, `"سلام";` and `کد 😀 code` keep their
     * punctuation, quotes and symbols in place. (The prose engine
     * findIsolateRanges is tuned for Word paragraphs and absorbs trailing
     * neutrals; code needs the strict rule.)
     * @param {string} text
     * @returns {Array<[number, number]>} [start, end) ranges
     */
    const CODE_BRACKETS = /[()[\]{}<>]/;

    /**
     * Code text for Word, which has no isolates: an LTR embedding (LRE … PDF)
     * around it, and LRM marks around each neutral stretch between two RTL runs
     * (codeRtlRanges), so `![نام](نام)` inside a Persian sentence keeps its
     * order and brackets. Text without RTL letters comes back unchanged.
     */
    function codeForWord(text) {
        if (!text || !hasRtlLetter(text)) return text;
        const r = codeRtlRanges(text);
        let out = '', pos = 0;
        r.forEach(function (x, k) {
            const gap = text.slice(pos, x[0]);
            out += k > 0 && gap ? '\u200E' + gap + '\u200E' : gap;
            out += text.slice(x[0], x[1]);
            pos = x[1];
        });
        out += text.slice(pos);
        return '\u202A' + out + '\u202C';
    }

    function codeRtlRanges(text) {
        const out = [];
        const n = text.length;
        let i = 0;
        while (i < n) {
            if (!STRONG_R.test(text[i])) { i++; continue; }
            // a run goes on over spaces and neutrals, not past a Latin letter or a bracket:
            // brackets around RTL text in code take the code's LTR side (UBA N0 with an LTR
            // context), so `![نام](نام)` keeps its brackets in place
            let j = i + 1, last = i;
            while (j < n && !STRONG_L.test(text[j]) && !CODE_BRACKETS.test(text[j])) {
                if (STRONG_R.test(text[j])) last = j;
                j++;
            }
            let end = last + 1;
            while (end < n && CODE_RUN_TAIL.test(text[end])) end++;
            out.push([i, end]);
            i = end;
        }
        return out;
    }

    const endReCache = new Map();
    function endPunctuationRe(chars) {
        if (!chars || !chars.length) return null;
        const key = chars.join('\u0000');
        if (!endReCache.has(key)) {
            endReCache.set(key, new RegExp('^(.*?)([' + chars.map(function (c) {
                return c.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            }).join('') + ']+)$'));
        }
        return endReCache.get(key);
    }

    /**
     * Counter-direction isolate ranges in a block's flattened text.
     * @param {string} text - ONE continuous string for the whole block
     * @param {'rtl'|'ltr'} baseDir - the block's effective direction
     * @param {object} [opts]
     * @param {string[]} [opts.endPunctuation=['.', ':']] - end-of-block marks kept
     *        outside the last isolate in RTL blocks ([] disables)
     * @returns {Array<[number, number]>} [start, end) ranges
     */
    function findIsolateRanges(text, baseDir, opts) {
        const chars = opts && opts.endPunctuation !== undefined ? opts.endPunctuation : ['.', ':'];
        return findIsolateRangesImpl(text, baseDir, endPunctuationRe(chars));
    }

    /**
     * Flatten a block's OWN text — through inline formatting, stopping at
     * nested blocks and code/pre — into one string, recording which text node
     * each character came from.
     * @param {Element} el
     * @returns {{ text: string, nodeMap: Array<{node: Text, start: number, end: number}> }}
     */
    function flattenBlock(el) {
        let flatText = '';
        const nodeMap = [];
        (function collect(n) {
            for (let c = n.firstChild; c; c = c.nextSibling) {
                if (c.nodeType === 3) {
                    const val = c.nodeValue;
                    if (val) {
                        nodeMap.push({ node: c, start: flatText.length, end: flatText.length + val.length });
                        flatText += val;
                    }
                } else if (c.nodeType === 1 && !NON_DESCEND.test(c.tagName) && !isMathAtom(c)) {
                    collect(c);
                }
            }
        })(el);
        return { text: flatText, nodeMap: nodeMap };
    }

    /** Map a flat offset back to { node, offset } (see flattenBlock). */
    function resolvePosition(nodeMap, flatOffset) {
        for (let k = 0; k < nodeMap.length; k++) {
            const entry = nodeMap[k];
            if (flatOffset <= entry.end) return { node: entry.node, offset: flatOffset - entry.start };
        }
        const last = nodeMap[nodeMap.length - 1];
        return { node: last.node, offset: last.node.nodeValue.length };
    }

    // =========================================================================
    // Code blocks — shared by both builders and usable by the preview
    // =========================================================================

    /** Class values that name "no language". */
    const NO_LANGUAGE = /^(plaintext|plain|text|txt|nohighlight|no-highlight|none)$/i;

    /**
     * Language of a code block, from the standard CommonMark/marked class
     * (`language-xxx`, also `lang-xxx`) on <code> or <pre>, or from a
     * `data-lang` attribute. Keeps full names like "c++", "c#",
     * "objective-c", "shell-session". Returns '' when unknown or plain text.
     * @param {Element} pre - a <pre> (or its <code>)
     * @returns {string}
     */
    function codeLanguage(pre) {
        const code = pre.tagName === 'CODE' ? pre : (pre.querySelector('code') || pre);
        const host = code.closest('pre') || code;
        const classes = (code.getAttribute('class') || '') + ' ' + (host.getAttribute('class') || '');
        const m = /(?:^|\s)(?:language|lang)-(\S+)/.exec(classes);
        let lang = m ? m[1] : (host.getAttribute('data-lang') || code.getAttribute('data-lang') || '');
        lang = lang.trim();
        return NO_LANGUAGE.test(lang) ? '' : lang;
    }

    /**
     * Remove preview-only decoration around code blocks (MUTATES root):
     *   <div class="code-block-wrapper">
     *     <div class="code-block-header"><span class="code-lang">…</span><button>…</button></div>
     *     <pre>…</pre>
     *   </div>                                   →   <pre>…</pre>
     * Must run right after parsing: the header text ("javascript Copy") would
     * otherwise take part in direction decisions. A label shown only in the
     * header (no language class) is kept on the <pre> as data-lang.
     * @param {Element} root
     */
    function unwrapCodeBlocks(root) {
        // PreviewBuilder / HtmlBuilder frames: <div class="code"><div class="code-lang">lang <button></div><pre>
        root.querySelectorAll('div.code').forEach(function (w) {
            const pre = w.querySelector(':scope > pre');
            if (!pre) return;
            if (!codeLanguage(pre)) {
                const l = w.querySelector(':scope > .code-lang');
                const t = l ? Array.from(l.childNodes).filter(function (n) { return n.nodeType === 3; }).map(function (n) { return n.nodeValue; }).join('').trim() : '';
                if (t && t !== 'code') pre.setAttribute('data-lang', t);
            }
            w.parentNode.replaceChild(pre, w);
        });
        root.querySelectorAll('.code-block-wrapper').forEach(function (w) {
            const pre = w.querySelector('pre');
            if (!pre) { w.parentNode.removeChild(w); return; }
            if (!codeLanguage(pre)) {
                const l = w.querySelector('.code-lang');
                const t = l ? l.textContent.trim() : '';
                if (t && t !== 'code') pre.setAttribute('data-lang', t);
            }
            w.parentNode.replaceChild(pre, w);
        });
        root.querySelectorAll('.code-block-header, .code-copy-btn').forEach(function (e) {
            if (e.parentNode) e.parentNode.removeChild(e);
        });
        // nested-fence frames of the preview (span.code-nested): keep the text
        root.querySelectorAll('.code-nested').forEach(function (sp) {
            while (sp.firstChild) sp.parentNode.insertBefore(sp.firstChild, sp);
            sp.parentNode.removeChild(sp);
        });
    }


    /**
     * Fenced blocks written INSIDE a code block (a ````md block showing
     * ```js … ``` examples). The text stays raw; renderers only draw a frame
     * around each nested block, from its opening fence line to its closing
     * one, like nested quotes. CommonMark fences: ``` or ~~~ (3+), up to 3
     * spaces indent; a closing fence has the same character, at least the
     * same length and no info string. Only markdown blocks (md / markdown /
     * no language) are scanned for further nesting; an unclosed fence gets
     * no frame.
     * @param {string} text - code text (lines split on \n)
     * @returns {Array<{from, to, lang, children}>} line numbers, inclusive
     */
    function nestedFences(text) {
        const FENCE = /^ {0,3}(`{3,}|~{3,})(.*)$/;
        const top = [], stack = [];
        text.split('\n').forEach(function (line, i) {
            const m = FENCE.exec(line);
            if (stack.length) {
                const t = stack[stack.length - 1];
                if (m && m[1].charAt(0) === t.ch && m[1].length >= t.len && !m[2].trim()) {
                    t.to = i;
                    stack.pop();
                    (stack.length ? stack[stack.length - 1].children : top).push(t);
                    return;
                }
                if (t.lang && !/^(md|markdown)$/i.test(t.lang)) return;     // raw inside other languages
            }
            if (m && !(m[1].charAt(0) === '`' && m[2].indexOf('`') >= 0)) {
                stack.push({ from: i, to: -1, ch: m[1].charAt(0), len: m[1].length,
                             lang: m[2].trim().split(/\s+/)[0] || '', children: [] });
            }
        });
        return top;
    }

    /**
     * nestedFences as a render tree over ALL lines [0, lineCount):
     *   { lines: [from, to] }                   plain lines (inclusive)
     *   { nested: true, lang, items: [...] }    a framed nested block
     * @param {string} text
     * @returns {Array<object>}
     */
    function codeSegments(text) {
        const count = text.split('\n').length;
        function seg(nodes, from, to) {
            const out = [];
            let i = from;
            nodes.forEach(function (n) {
                if (n.from > i) out.push({ lines: [i, n.from - 1] });
                out.push({ nested: true, lang: n.lang, from: n.from, to: n.to, items: seg(n.children, n.from, n.to) });
                i = n.to + 1;
            });
            if (i <= to) out.push({ lines: [i, to] });
            return out;
        }
        return seg(nestedFences(text), 0, count - 1);
    }

    /**
     * Copy of lines [fromLine, toLine] of a (highlighted) code element as a
     * DocumentFragment — spans cut at the line edges, no trailing newline.
     * Browser only (DOM Range).
     */
    function codeFragment(codeEl, fromLine, toLine) {
        const doc = codeEl.ownerDocument;
        const walker = doc.createTreeWalker(codeEl, 4 /* NodeFilter.SHOW_TEXT */);
        let line = 0, start = null, end = null, lastNode = null;
        for (let n = walker.nextNode(); n; n = walker.nextNode()) {
            const v = n.nodeValue;
            lastNode = n;
            for (let k = 0; k <= v.length; k++) {
                if (!start && line === fromLine) start = [n, k];
                if (k === v.length) break;
                if (v.charAt(k) === '\n') {
                    if (line === toLine) { end = [n, k]; break; }
                    line++;
                }
            }
            if (end) break;
        }
        const range = doc.createRange();
        if (!start) return doc.createDocumentFragment();
        range.setStart(start[0], start[1]);
        if (end) range.setEnd(end[0], end[1]);
        else range.setEnd(lastNode, lastNode.nodeValue.length);
        return range.cloneContents();
    }


    /**
     * A detached <div> in an inert document: content assigned to innerHTML
     * is parsed, but its <img> elements do not start loading (the builders
     * parse the preview HTML; image bytes come from ImageCore). Nodes made
     * with document.createElement can still be inserted (adopted).
     */
    function inertRoot() {
        const doc = (typeof document !== 'undefined' && document.implementation)
            ? document.implementation.createHTMLDocument('') : document;
        return doc.createElement('div');
    }


    // =========================================================================
    // Custom header / footer HTML (shared model for the three builders)
    // =========================================================================
    //
    // setHeader({ html }) / setFooter({ html }) take a SUBSET of HTML that all
    // three outputs can draw the same way:
    //   blocks   <table> (<tr>, <td>/<th>, colspan, width %), <p>, <div>
    //   inline   <b>/<strong>, <i>/<em>, <u>, <span>, <a href>, <img>, <br>
    //   style="" (inline only): text-align, vertical-align, color, background
    //            (-color), font-size, font-weight, font-style, padding(-*),
    //            border / border-top|right|bottom|left, width (%)
    //   fields   {page} {pages} {date} {title} — anywhere in the text
    // Classes, <style> and other properties are ignored (with a console.warn).

    const HF_FIELD = /\{(page|pages|date|title)\}/g;
    const HF_FIELD_TEST = /\{(page|pages|date|title)\}/;
    const HF_STYLE_KEYS = /^(text-align|vertical-align|color|background|background-color|font-size|font-weight|font-style|padding|padding-(top|right|bottom|left)|border|border-(top|right|bottom|left)|width)$/;

    /** CSS length → pt (px, pt, cm, mm, in); NaN when not a length. */
    function cssPt(v) {
        const m = /^\s*(-?[\d.]+)\s*(px|pt|cm|mm|in)?\s*$/i.exec(String(v || ''));
        if (!m) return NaN;
        return parseFloat(m[1]) * { px: 0.75, pt: 1, cm: 72 / 2.54, mm: 72 / 25.4, in: 72 }[(m[2] || 'px').toLowerCase()];
    }

    /** '1pt solid red' (any order) → { width (pt), style, color } | null (none/0) */
    function cssBorder(v) {
        const t = String(v || '').trim().toLowerCase();
        if (!t || /^(none|0|hidden)$/.test(t)) return null;
        let width = 0.75, style = 'solid', color = '#000000';
        t.replace(/rgb\([^)]*\)/g, function (m) { return m.replace(/\s+/g, ''); }).split(/\s+/).forEach(function (p) {
            if (/^(solid|dashed|dotted|double)$/.test(p)) style = p;
            else if (/^(thin|medium|thick)$/.test(p)) width = { thin: 0.5, medium: 1.5, thick: 2.25 }[p];
            else if (!isNaN(cssPt(p))) width = cssPt(p);
            else if (p) color = p;
            return p;
        });
        return width > 0 ? { width: width, style: style, color: color } : null;
    }


    const CSS_COLORS = {
        black: '000000', white: 'FFFFFF', red: 'FF0000', green: '008000', blue: '0000FF',
        darkgreen: '006400', navy: '000080', maroon: '800000', orange: 'FFA500', yellow: 'FFFF00',
        gray: '808080', grey: '808080', silver: 'C0C0C0', purple: '800080', teal: '008080',
        darkred: '8B0000', darkblue: '00008B', lightgray: 'D3D3D3', lightgrey: 'D3D3D3', windowtext: '000000'
    };
    /** CSS color (#rgb, #rrggbb, rgb(), common names) → 'RRGGBB' | null */
    function cssColorHex(v) {
        const t = String(v || '').trim().toLowerCase();
        if (CSS_COLORS[t]) return CSS_COLORS[t];
        let m = /^#([0-9a-f]{6})$/.exec(t);
        if (m) return m[1].toUpperCase();
        m = /^#([0-9a-f])([0-9a-f])([0-9a-f])$/.exec(t);
        if (m) return (m[1] + m[1] + m[2] + m[2] + m[3] + m[3]).toUpperCase();
        m = /^rgba?\(\s*(\d+)[\s,]+(\d+)[\s,]+(\d+)/.exec(t);
        if (m) return [m[1], m[2], m[3]].map(function (x) { return ('0' + Math.min(255, +x).toString(16)).slice(-2); }).join('').toUpperCase();
        return null;
    }

    /** Whitelisted inline style of an element → normalized style object. */
    function hfStyle(el, warn) {
        const out = { border: {} };
        const attrAlign = el.getAttribute && el.getAttribute('align');
        if (attrAlign) out.align = attrAlign.toLowerCase();
        const attrW = el.getAttribute && el.getAttribute('width');
        if (attrW && /%$/.test(attrW)) out.width = parseFloat(attrW);
        String((el.getAttribute && el.getAttribute('style')) || '').split(';').forEach(function (decl) {
            const i = decl.indexOf(':');
            if (i < 0) return;
            const k = decl.slice(0, i).trim().toLowerCase(), v = decl.slice(i + 1).trim();
            if (!k) return;
            if (!HF_STYLE_KEYS.test(k)) { if (warn) warn('style ' + k); return; }
            switch (k) {
                case 'text-align': out.align = v.toLowerCase(); break;
                case 'vertical-align': out.valign = v.toLowerCase(); break;
                case 'color': out.color = v; break;
                case 'background': case 'background-color': out.bg = v.split(/\s+/)[0]; break;
                case 'font-size': out.size = cssPt(v); break;
                case 'font-weight': out.bold = /bold|[6-9]00/.test(v); break;
                case 'font-style': out.italic = /italic|oblique/.test(v); break;
                case 'width': if (/%$/.test(v)) out.width = parseFloat(v); else if (!isNaN(cssPt(v))) out.widthPt = cssPt(v); break;
                case 'padding': {
                    const p = v.split(/\s+/).map(cssPt);
                    const t = p[0], r = p.length > 1 ? p[1] : t, b = p.length > 2 ? p[2] : t, l = p.length > 3 ? p[3] : r;
                    out.padding = { top: t, right: r, bottom: b, left: l };
                    break;
                }
                case 'border': ['top', 'right', 'bottom', 'left'].forEach(function (sd) { out.border[sd] = cssBorder(v); }); break;
                default:
                    if (k.indexOf('padding-') === 0) { out.padding = out.padding || {}; out.padding[k.slice(8)] = cssPt(v); }
                    else out.border[k.slice(7)] = cssBorder(v);
            }
        });
        return out;
    }

    /**
     * Parse header/footer HTML into { blocks }:
     *   { type: 'table', style, rows: [{ cells: [{ colspan, style, paras }] }] }
     *   { type: 'para', style, el }          (el: detached inline container)
     * A cell's paras: one per <p>/<div> (or the cell's own inline content),
     * split further at <br>. Placeholders stay in the text (see hfFields).
     */
    function parseHeaderFooter(html) {
        const doc = (typeof document !== 'undefined' && document.implementation)
            ? document.implementation.createHTMLDocument('') : null;
        if (!doc) return { blocks: [] };
        const root = doc.createElement('div');
        root.innerHTML = String(html || '');
        const warned = new Set();
        const warn = function (what) {
            if (warned.has(what)) return;
            warned.add(what);
            if (typeof console !== 'undefined' && console.warn) console.warn('header/footer HTML: ' + what + ' is not supported, ignored');
        };
        root.querySelectorAll('style, script, link, meta').forEach(function (e) { e.remove(); });
        root.querySelectorAll('[class]').forEach(function () { warn('class'); });
        root.querySelectorAll('*').forEach(function (e) { hfStyle(e, warn); });

        // inline content of a container → paras (split at <br> and child blocks)
        function parasOf(container, baseStyle) {
            const paras = [];
            let cur = null;
            const start = function (style) { cur = doc.createElement('span'); paras.push({ style: style || baseStyle, el: cur }); };
            (function walk(node, style) {
                for (let n = node.firstChild; n; n = n.nextSibling) {
                    if (n.nodeType === 1 && /^(P|DIV|H[1-6])$/.test(n.tagName)) {
                        const st = Object.assign({}, style, stripBox(hfStyle(n)));
                        if (/^H/.test(n.tagName)) st.bold = true;
                        cur = null;
                        start(st);
                        walk(n, st);
                        cur = null;
                        continue;
                    }
                    if (n.nodeType === 1 && n.tagName === 'BR') { start(style); continue; }
                    if (n.nodeType === 3 && !/\S/.test(n.nodeValue) && !cur) continue;
                    if (!cur) start(style);
                    cur.appendChild(n.cloneNode(true));
                }
            })(container, baseStyle);
            return paras.filter(function (p) { return p.el.childNodes.length; });
        }
        // text-level properties only (a paragraph inherits them from its cell)
        function stripBox(st) {
            const o = {};
            ['align', 'color', 'size', 'bold', 'italic'].forEach(function (k) { if (st[k] !== undefined) o[k] = st[k]; });
            return o;
        }

        const blocks = [];
        let loose = null;
        for (let n = root.firstChild; n; n = n.nextSibling) {
            if (n.nodeType === 1 && n.tagName === 'TABLE') {
                loose = null;
                const tStyle = hfStyle(n);
                const rows = [];
                n.querySelectorAll('tr').forEach(function (tr) {
                    if (tr.closest('table') !== n) return;
                    const cells = [];
                    Array.from(tr.children).forEach(function (td) {
                        if (!/^(TD|TH)$/.test(td.tagName)) return;
                        const st = hfStyle(td);
                        if (td.tagName === 'TH' && st.bold === undefined) st.bold = true;
                        cells.push({ colspan: Math.max(1, parseInt(td.getAttribute('colspan'), 10) || 1),
                                     style: st, paras: parasOf(td, stripBox(st)) });
                    });
                    if (cells.length) rows.push({ cells: cells });
                });
                if (rows.length) blocks.push({ type: 'table', style: tStyle, rows: rows });
                continue;
            }
            if (n.nodeType === 1 && /^(P|DIV|H[1-6])$/.test(n.tagName)) {
                loose = null;
                const st = hfStyle(n);
                if (/^H/.test(n.tagName)) st.bold = true;
                parasOf(n, stripBox(st)).forEach(function (p) { blocks.push({ type: 'para', style: Object.assign({}, st, p.style), el: p.el }); });
                continue;
            }
            if (n.nodeType === 3 && !/\S/.test(n.nodeValue)) continue;
            // loose inline content at the top level → one paragraph
            if (!loose) { loose = doc.createElement('span'); blocks.push({ type: 'para', style: {}, el: loose }); }
            loose.appendChild(n.cloneNode(true));
        }
        return { blocks: blocks };
    }

    /**
     * Column widths (%) of a parsed table: widths given on the cells of the
     * first row that has them (colspan split evenly), the rest shared equally.
     */
    function hfColumnWidths(table) {
        const count = Math.max.apply(null, table.rows.map(function (r) {
            return r.cells.reduce(function (s, c) { return s + c.colspan; }, 0);
        }));
        const w = new Array(count).fill(null);
        table.rows.forEach(function (r) {
            let col = 0;
            r.cells.forEach(function (c) {
                if (c.style.width && c.colspan === 1 && w[col] === null) w[col] = c.style.width;
                col += c.colspan;
            });
        });
        const given = w.reduce(function (s, x) { return s + (x || 0); }, 0);
        const free = w.filter(function (x) { return x === null; }).length;
        const rest = free ? Math.max(0, 100 - given) / free : 0;
        const out = w.map(function (x) { return x === null ? rest : x; });
        const sum = out.reduce(function (s, x) { return s + x; }, 0) || 1;
        return out.map(function (x) { return x * 100 / sum; });
    }

    /**
     * Fill the fields of an inline container (a deep clone is returned).
     * values: { page, pages, date, title } — a function value(name) returning
     * a Node places that node (Word field codes); a string replaces the text.
     */
    function hfFields(el, values) {
        const c = el.cloneNode(true);
        const doc = c.ownerDocument;
        const walker = doc.createTreeWalker(c, 4);
        const texts = [];
        for (let n = walker.nextNode(); n; n = walker.nextNode()) if (HF_FIELD_TEST.test(n.nodeValue)) texts.push(n);
        texts.forEach(function (t) {
            const frag = doc.createDocumentFragment();
            let pos = 0;
            const v = t.nodeValue;
            v.replace(HF_FIELD, function (m, name, off) {
                if (off > pos) frag.appendChild(doc.createTextNode(v.slice(pos, off)));
                const val = values[name];
                const node = typeof val === 'function' ? val(name, doc) : null;
                frag.appendChild(node || doc.createTextNode(val == null ? m : String(val)));
                pos = off + m.length;
                return m;
            });
            if (pos < v.length) frag.appendChild(doc.createTextNode(v.slice(pos)));
            t.parentNode.replaceChild(frag, t);
        });
        return c;
    }

    /** {date}: today in the header/footer language, Latin digits. */
    function hfDate(dir, locale) {
        const loc = locale || (dir === 'rtl' ? 'fa-IR-u-nu-latn' : 'en-GB');
        try { return new Intl.DateTimeFormat(loc, { year: 'numeric', month: 'long', day: 'numeric' }).format(new Date()); }
        catch (e) { return new Date().toISOString().slice(0, 10); }
    }

    // =========================================================================
    // Small shared utilities
    // =========================================================================

    const PT_PER_UNIT = { pt: 1, px: 0.75, cm: 72 / 2.54, mm: 72 / 25.4, in: 72, pc: 12 };

    /** CSS length ('1.5cm', '11pt', '0.5in', 11) → points. */
    function lengthToPt(value) {
        if (typeof value === 'number') return value;
        const m = /^\s*(-?[\d.]+)\s*(pt|px|cm|mm|in|pc)?\s*$/i.exec(String(value));
        if (!m) throw new Error('Invalid length: ' + value);
        return parseFloat(m[1]) * PT_PER_UNIT[(m[2] || 'pt').toLowerCase()];
    }

    /**
     * Text-area width in points from template options (pageSize is already
     * landscape-swapped by setPage; margin is a 1–4 value CSS shorthand).
     */
    function contentWidthPt(T) {
        const wh = String(T.pageSize).trim().split(/\s+/).map(lengthToPt);
        const w = T.pageOrientation === 'landscape' ? Math.max(wh[0], wh[1]) : Math.min(wh[0], wh[1]);
        const m = String(T.pageMargin).trim().split(/\s+/).map(lengthToPt);
        const right = m.length > 1 ? m[1] : m[0];
        const left = m.length > 3 ? m[3] : right;
        return w - left - right;
    }

    /**
     * Deep merge without sharing nested objects with `target` (so mutating the
     * result never leaks into a DEFAULTS object). Arrays are copied, not merged.
     * @param {object} target
     * @param {object} source
     * @param {string[]} [skipKeys] - keys of `source` to ignore
     */
    function deepMerge(target, source, skipKeys) {
        const isObj = function (v) { return v && typeof v === 'object' && !Array.isArray(v); };
        const copy = function (v) { return isObj(v) ? deepMerge(v, {}) : (Array.isArray(v) ? v.slice() : v); };
        const result = {};
        for (const key in target) result[key] = copy(target[key]);
        for (const key in source) {
            if (!Object.prototype.hasOwnProperty.call(source, key)) continue;
            if (skipKeys && skipKeys.indexOf(key) !== -1) continue;
            const v = source[key];
            result[key] = isObj(v) ? deepMerge(isObj(result[key]) ? result[key] : {}, v) : copy(v);
        }
        return result;
    }

    // =========================================================================
    // Export (UMD)
    // =========================================================================
    const BidiCore = {
        RTL_CHAR_PATTERN: RTL_CHAR_PATTERN,
        RTL_LETTER_PATTERN: RTL_LETTER_PATTERN,
        LATIN_LETTER_PATTERN: LATIN_LETTER_PATTERN,
        NUMBERS_ONLY_PATTERN: NUMBERS_ONLY_PATTERN,
        BLOCK_SEL: BLOCK_SEL,
        NON_DESCEND: NON_DESCEND,
        COMPLEX_QUOTE_SEL: COMPLEX_QUOTE_SEL,
        hasRTL: hasRTL,
        hasLatin: hasLatin,
        hasRtlLetter: hasRtlLetter,
        hasMix: hasMix,
        firstStrong: firstStrong,
        noJustify: noJustify,
        insertToc: insertToc,
        TOC_TITLES: TOC_TITLES,
        distributeColumns: distributeColumns,
        tableColumnShares: tableColumnShares,
        resolveTableWidth: resolveTableWidth,
        isLtrText: isLtrText,
        isRtlText: isRtlText,
        getVisibleText: getVisibleText,
        detectDirection: detectDirection,
        planDirections: planDirections,
        planToMaps: planToMaps,
        isComplexQuote: isComplexQuote,
        findIsolateRanges: findIsolateRanges,
        codeRtlRanges: codeRtlRanges,
        codeForWord: codeForWord,
        nestedFences: nestedFences,
        codeSegments: codeSegments,
        codeFragment: codeFragment,
        flattenBlock: flattenBlock,
        isMathAtom: isMathAtom,
        resolvePosition: resolvePosition,
        deepMerge: deepMerge,
        codeLanguage: codeLanguage,
        unwrapCodeBlocks: unwrapCodeBlocks,
        inertRoot: inertRoot,
        parseHeaderFooter: parseHeaderFooter,
        hfColumnWidths: hfColumnWidths,
        hfFields: hfFields,
        hfDate: hfDate,
        cssBorder: cssBorder,
        cssColorHex: cssColorHex,
        hfStyleOf: function (el) { return hfStyle(el); },
        cssPt: cssPt,
        lengthToPt: lengthToPt,
        contentWidthPt: contentWidthPt
    };

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = BidiCore;
    } else if (typeof define === 'function' && define.amd) {
        define([], function () { return BidiCore; });
    } else {
        global.BidiCore = BidiCore;
    }

})(typeof window !== 'undefined' ? window : this);


/* === BidiView.js === */
/**
 * BidiView - direction for HTML shown in a browser (preview, standalone HTML)
 * ============================================================================
 * Version: 1.5
 *
 * The browser already runs the Unicode BiDi Algorithm; BidiView applies the
 * decisions of BidiCore — the same ones the .doc / .docx / .pdf builders
 * apply — so a page shows what the files get:
 *   1. document direction   BidiCore.detectDirection
 *   2. block directions     BidiCore.planDirections (paragraphs, headings,
 *                           list items, quotes, table cells, LTR tables)
 *   3. isolates             BidiCore.findIsolateRanges over each block's own
 *                           text (BidiCore.flattenBlock) — one isolate per run,
 *                           even across <strong>/<em> boundaries
 * plus what only a browser page needs: list items indented by LEVEL on their
 * own start side (bd-b0 / bd-sN over --bd-base / --bd-step), as Word does, and
 * inline code as an isolate.
 *
 * Output is CLASSES only (bd-*), never a dir attribute, so the HTML handed to
 * other tools stays clean:
 *
 *   BidiView.apply(root, { dir: 'auto'|'rtl'|'ltr', endPunctuation: ['.', ':'],
 *                          isolate: true, inlineCode: true }) → { dir, blocks, isolates }
 *   BidiView.clear(root)          remove everything apply() added
 *   BidiView.css()                the CSS the classes need
 *   BidiView.toAttributes(root)   classes → dir attributes (for the clipboard:
 *                                 Word reads dir, not CSS classes)
 *
 * Needs BidiCore (load it first). tools/bidi-lab.html carries both inline.
 */

(function (global) {
  'use strict';

  function core() {
    var B = global.BidiCore || (typeof require === 'function' ? require('./BidiCore.js') : null);
    if (!B) throw new Error('BidiView needs BidiCore (load lib/BidiCore.js first)');
    return B;
  }

  /** Direction an element ends up with: nearest planned ancestor, else the document. */
  function dirOf(el, root, map, doc) {
    for (var p = el; p && p !== root; p = p.parentNode) if (map.has(p)) return map.get(p);
    return doc;
  }

  /**
   * List indents by LEVEL on each item's own start side, as Word does: the text
   * of level n sits at base + (n−1) × step from that side, so an RTL item nested
   * in an LTR item is indented from the RIGHT as a level-2 item, although its
   * parent's indent was on the left. Each item's margin is what its same-side
   * ancestors have not given yet, as coefficients of base and step:
   * classes bd-b0 (no base: an ancestor on this side had it) and bd-sN.
   * CSS: --bd-base / --bd-step (default 1.7em).
   */
  function indent(root, map, doc) {
    var give = new Map();                       // li → { b, s } it adds on its side
    Array.prototype.forEach.call(root.querySelectorAll('li'), function (li) {
      var d = dirOf(li, root, map, doc), depth = 1, gb = 0, gs = 0;
      for (var a = li.parentElement ? li.parentElement.closest('li') : null; a && root.contains(a); a = a.parentElement ? a.parentElement.closest('li') : null) {
        depth++;
        if (dirOf(a, root, map, doc) === d) { var g = give.get(a) || { b: 0, s: 0 }; gb += g.b; gs += g.s; }
      }
      var mb = Math.max(0, 1 - gb), ms = Math.max(0, Math.min(9, (depth - 1) - gs));
      give.set(li, { b: mb, s: ms });
      if (!mb) li.classList.add('bd-b0');
      if (ms) li.classList.add('bd-s' + ms);
    });
  }

  function clear(root) {
    Array.prototype.slice.call(root.querySelectorAll('span.bd-iso-ltr, span.bd-iso-rtl')).forEach(function (s) {
      var parent = s.parentNode;                       // unwrap, keeping <strong>/<em> inside
      while (s.firstChild) parent.insertBefore(s.firstChild, s);
      parent.removeChild(s);
    });
    Array.prototype.slice.call(root.querySelectorAll('.bd-ltr, .bd-rtl, .bd-ltr-table, .bd-iso-ltr, .bd-iso-rtl, .bd-b0, [class*="bd-s"]')).forEach(function (el) {
      el.classList.remove('bd-ltr', 'bd-rtl', 'bd-ltr-table', 'bd-iso-ltr', 'bd-iso-rtl', 'bd-b0');
      for (var i = 1; i <= 9; i++) el.classList.remove('bd-s' + i);
      if (!el.classList.length) el.removeAttribute('class');          // leave the HTML as it was
    });
    root.classList.remove('bd-root', 'bd-doc-rtl', 'bd-doc-ltr');
    if (!root.classList.length) root.removeAttribute('class');
    root.normalize();
  }

  /** Classes → dir attributes and no classes (rich copy into Word, mail …). MUTATES root. */
  function toAttributes(root) {
    var all = [root].concat(Array.prototype.slice.call(root.querySelectorAll('.bd-ltr, .bd-rtl, .bd-ltr-table, .bd-iso-ltr, .bd-iso-rtl')));
    all.forEach(function (el) {
      var c = el.classList;
      if (c.contains('bd-doc-rtl') || c.contains('bd-rtl') || c.contains('bd-iso-rtl')) el.setAttribute('dir', 'rtl');
      else if (c.contains('bd-doc-ltr') || c.contains('bd-ltr') || c.contains('bd-ltr-table') || c.contains('bd-iso-ltr')) el.setAttribute('dir', 'ltr');
    });
    Array.prototype.slice.call(root.querySelectorAll('span.bd-iso-ltr, span.bd-iso-rtl')).forEach(function (s) {
      s.classList.remove('bd-iso-ltr', 'bd-iso-rtl');
      if (!s.className) s.removeAttribute('class');
    });
    clear(root);
    Array.prototype.slice.call(root.querySelectorAll('[class=""]')).forEach(function (el) { el.removeAttribute('class'); });
    return root;
  }

  /** An inline element emptied by extractContents() (its text moved into the isolate). */
  function dropEmpty(n) {
    if (n && n.nodeType === 1 && /^(STRONG|EM|B|I|U|S|DEL|INS|MARK|SMALL|SUB|SUP|SPAN)$/.test(n.tagName) &&
        !n.textContent && !n.querySelector('img, br, input')) n.parentNode.removeChild(n);
  }

  /** Counter-direction runs of every block: BidiCore ranges → span.bd-iso-*. */
  function isolate(root, map, doc, o) {
    var B = core(), count = 0;
    Array.prototype.forEach.call(root.querySelectorAll(B.BLOCK_SEL), function (el) {
      if (el.closest('pre')) return;
      var flat = B.flattenBlock(el);
      if (!B.hasMix(flat.text)) return;
      var base = dirOf(el, root, map, doc), other = base === 'rtl' ? 'ltr' : 'rtl';
      var ranges = B.findIsolateRanges(flat.text, base, { endPunctuation: o.endPunctuation });
      // last range first: earlier ranges and the node map stay valid;
      // extractContents() splits a partly covered <strong>/<em> correctly
      for (var r = ranges.length - 1; r >= 0; r--) {
        var a = ranges[r][0], b = ranges[r][1];
        if (b <= a) continue;
        var sp = B.resolvePosition(flat.nodeMap, a), ep = B.resolvePosition(flat.nodeMap, b);
        var range = document.createRange();
        range.setStart(sp.node, sp.offset);
        range.setEnd(ep.node, ep.offset);
        var span = document.createElement('span');
        span.className = 'bd-iso-' + other;
        span.appendChild(range.extractContents());
        range.insertNode(span);
        dropEmpty(span.previousSibling); dropEmpty(span.nextSibling);   // shells left by the split
        count++;
      }
    });
    return count;
  }

  function apply(root, options) {
    var o = Object.assign({ dir: 'auto', endPunctuation: ['.', ':'], isolate: true, inlineCode: true }, options || {});
    var B = core();
    clear(root);
    var doc = o.dir === 'rtl' || o.dir === 'ltr' ? o.dir : B.detectDirection(root);
    root.classList.add('bd-root', 'bd-doc-' + doc);
    var plan = B.planToMaps(B.planDirections(root, doc === 'rtl'));
    plan.dir.forEach(function (d, el) { el.classList.add('bd-' + d); });
    plan.ltrTables.forEach(function (t) { t.classList.add('bd-ltr-table'); });
    indent(root, plan.dir, doc);
    if (o.inlineCode) {
      Array.prototype.forEach.call(root.querySelectorAll('code'), function (c) {
        if (c.closest('pre')) return;
        c.classList.add('bd-iso-' + (B.firstStrong(c.textContent) || 'ltr'));
      });
    }
    var runs = o.isolate ? isolate(root, plan.dir, doc, o) : 0;
    return { dir: doc, blocks: plan.dir.size, isolates: runs };
  }

  /** The CSS the classes need. Lists: the marker and indent follow each item. */
  function css() {
    return [
      '.bd-doc-rtl{direction:rtl}.bd-doc-ltr{direction:ltr}',
      '.bd-ltr,.bd-ltr-table{direction:ltr}.bd-rtl{direction:rtl}',
      '.bd-root :is(p,h1,h2,h3,h4,h5,h6,li,blockquote,ul,ol,dt,dd):is(.bd-ltr,.bd-rtl){text-align:start}',
      '.bd-root :is(td,th):is(.bd-ltr,.bd-rtl):not([align]):not([style*="text-align"]){text-align:start}',
      '.bd-iso-ltr{unicode-bidi:isolate;direction:ltr}.bd-iso-rtl{unicode-bidi:isolate;direction:rtl}',
      '.bd-root pre{direction:ltr;text-align:left}',
      '.bd-root :is(ul,ol){padding-inline-start:0}',
      '.bd-root li{--bd-b:1;--bd-s:0;margin-inline-start:calc(var(--bd-b) * var(--bd-base,1.7em) + var(--bd-s) * var(--bd-step,1.7em))}',
      '.bd-root li.bd-b0{--bd-b:0}' + [1, 2, 3, 4, 5, 6, 7, 8, 9].map(function (n) { return '.bd-root li.bd-s' + n + '{--bd-s:' + n + '}'; }).join('')
    ].join('\n');
  }

  var BidiView = { version: '1.5', apply: apply, clear: clear, css: css, toAttributes: toAttributes };

  if (typeof module !== 'undefined' && module.exports) module.exports = BidiView;
  else if (typeof define === 'function' && define.amd) define([], function () { return BidiView; });
  else global.BidiView = BidiView;
})(typeof window !== 'undefined' ? window : this);


/* === Controller === */

(function() {
  'use strict';

  var contentEl = null;
  var currentBiDi = true;

  // Configure marked for GFM
  if (typeof marked !== 'undefined') {
    marked.setOptions({
      gfm: true,
      breaks: false
    });
  }

  // Tokenize markdown with source line tracking
  function parseWithLines(markdown) {
    if (!markdown || !markdown.trim()) return '';
    try {
      var tokens = marked.lexer(markdown);
      var curLine = 0;
      for (var i = 0; i < tokens.length; i++) {
        var tok = tokens[i];
        tok.sourceLine = curLine;
        if (tok.type === 'list' && tok.items) {
          var itemLine = curLine;
          for (var j = 0; j < tok.items.length; j++) {
            tok.items[j].sourceLine = itemLine;
            var numLines = (tok.items[j].raw.match(/\n/g) || []).length;
            itemLine += numLines;
          }
        }
        var lines = (tok.raw.match(/\n/g) || []).length;
        curLine += lines;
      }

      var customRenderer = new marked.Renderer();
      customRenderer.heading = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<h' + obj.depth + ' data-line="' + line + '">' + text + '</h' + obj.depth + '>\n';
      };
      customRenderer.paragraph = function(obj) {
        var text = this.parser.parseInline(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<p data-line="' + line + '">' + text + '</p>\n';
      };
      customRenderer.code = function(obj) {
        var validLang = (obj.lang || '').match(/^\S*/)[0];
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        var langClass = validLang ? ' language-' + validLang : '';
        return '<pre data-line="' + line + '"><code class="hljs' + langClass + '">' +
               DOMPurify.sanitize(obj.text) + '</code></pre>\n';
      };
      customRenderer.blockquote = function(obj) {
        var text = this.parser.parse(obj.tokens);
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        return '<blockquote data-line="' + line + '">' + text + '</blockquote>\n';
      };
      customRenderer.listitem = function(obj) {
        var line = obj.sourceLine !== undefined ? obj.sourceLine : 0;
        var text = this.parser.parse(obj.tokens);
        if (obj.task) {
          return '<li class="task-list-item" data-line="' + line + '">' +
                 '<input type="checkbox" class="task-checkbox" data-line="' + line + '" ' +
                 (obj.checked ? 'checked' : '') + '> ' + text + '</li>\n';
        }
        return '<li data-line="' + line + '">' + text + '</li>\n';
      };

      var parser = new marked.Parser({ renderer: customRenderer });
      return parser.parse(tokens);
    } catch(err) {
      console.error('Markdown parse error:', err);
      return marked.parse(markdown);
    }
  }

  function applyRender(data) {
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
      contentEl.innerHTML = '<div style="color:var(--muted);text-align:center;padding:60px 20px;"><em>(No content to preview)</em></div>';
      return;
    }

    var html = parseWithLines(mdText);
    if (typeof DOMPurify !== 'undefined') {
      html = DOMPurify.sanitize(html, { ADD_ATTR: ['data-line', 'target', 'dir', 'align'] });
    }

    contentEl.innerHTML = html;

    // External links open in default browser
    var links = contentEl.querySelectorAll('a[href^="http"]');
    for (var l = 0; l < links.length; l++) {
      links[l].target = '_blank';
      links[l].rel = 'noopener';
    }

    // Syntax highlighting with highlight.js
    if (typeof hljs !== 'undefined') {
      var codeBlocks = contentEl.querySelectorAll('pre code');
      for (var c = 0; c < codeBlocks.length; c++) {
        try { hljs.highlightElement(codeBlocks[c]); } catch(e) {}
      }
    }

    // Apply Kelk BiDi engine
    if (currentBiDi && typeof BidiView !== 'undefined' && typeof BidiCore !== 'undefined') {
      try {
        BidiView.apply(contentEl);
      } catch(e) {
        console.error('BidiView error:', e);
      }
    } else {
      contentEl.setAttribute('dir', 'auto');
    }

    // Bind interactive task checkboxes
    var cbs = contentEl.querySelectorAll('input.task-checkbox, input.task-list-item-checkbox');
    for (var k = 0; k < cbs.length; k++) {
      (function(cb) {
        cb.addEventListener('click', function(e) {
          e.stopPropagation();
          var line = cb.getAttribute('data-line');
          if (line !== null && window.chrome && window.chrome.webview) {
            window.chrome.webview.postMessage('toggleCheckbox:' + line);
          }
        });
      })(cbs[k]);
    }

    // Scroll to caret if specified
    if (typeof data.caretLine === 'number' && data.caretLine >= 0) {
      scrollToLine(data.caretLine, false);
    }
  }

  function scrollToLine(targetLine, highlight) {
    if (!contentEl) contentEl = document.getElementById('content');
    if (!contentEl) return;

    var elements = contentEl.querySelectorAll('[data-line]');
    if (elements.length === 0) return;

    var bestEl = null;
    var minDiff = 999999;
    for (var i = 0; i < elements.length; i++) {
      var l = parseInt(elements[i].getAttribute('data-line'), 10);
      var diff = Math.abs(l - targetLine);
      if (diff < minDiff) {
        minDiff = diff;
        bestEl = elements[i];
      }
    }

    if (bestEl) {
      bestEl.scrollIntoView({ behavior: 'smooth', block: 'center' });
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
      } else if (data.type === 'scrollToLine') {
        scrollToLine(data.line, true);
      } else if (data.type === 'setTheme') {
        document.body.classList.toggle('dark', !!data.isDark);
        document.documentElement.setAttribute('data-theme', data.isDark ? 'dark' : 'light');
      } else if (data.type === 'setBiDi') {
        currentBiDi = !!data.isBiDi;
        if (contentEl && typeof BidiView !== 'undefined') {
          if (currentBiDi) BidiView.apply(contentEl);
          else { BidiView.clear(contentEl); contentEl.setAttribute('dir', 'auto'); }
        }
      }
    });
  }

  document.addEventListener('DOMContentLoaded', function() {
    contentEl = document.getElementById('content');
    if (window.chrome && window.chrome.webview) {
      window.chrome.webview.postMessage('pageLoaded');
    }
  });
})();

  </script>
</head>
<body>
  <div id="reader-wrap">
    <div id="content" class="doc" dir="auto"></div>
  </div>
</body>
</html>

)KELK_HTML_EOF";
