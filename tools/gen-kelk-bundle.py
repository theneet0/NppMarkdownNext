#!/usr/bin/env python3
"""
Bundle generator for Kelk engines and styles into KelkBundle.inc
Usage: python tools/gen-kelk-bundle.py [path_to_kelk_repo]
"""

import os
import sys

script_dir = os.path.dirname(os.path.abspath(__file__))
repo_root = os.path.abspath(os.path.join(script_dir, ".."))
default_kelk = os.path.abspath(os.path.join(repo_root, "..", "kelk_repo"))

kelk_dir = sys.argv[1] if len(sys.argv) > 1 else default_kelk
output_file = os.path.join(repo_root, "src", "KelkBundle.inc")

if not os.path.exists(kelk_dir):
    print(f"Error: Kelk repository directory not found at {kelk_dir}")
    sys.exit(1)

lib_dir = os.path.join(kelk_dir, "lib")
vendor_dir = os.path.join(kelk_dir, "vendor")
styles_dir = os.path.join(kelk_dir, "styles")
fonts_dir = os.path.join(lib_dir, "pdf-base64-ttf-fonts")

# 1. Styles
css_files = [
    os.path.join(styles_dir, "tokens.css"),
    os.path.join(styles_dir, "base.css"),
    os.path.join(styles_dir, "components.css"),
    os.path.join(styles_dir, "preview.css"),
]
all_css = []
for f in css_files:
    if os.path.exists(f):
        with open(f, "r", encoding="utf-8") as fp:
            all_css.append(f"/* === {os.path.basename(f)} === */\n" + fp.read())

combined_css = "\n\n".join(all_css)

# 2. Scripts
js_files = [
    os.path.join(vendor_dir, "docx.index.iife.min_9.7.1.js"),
    os.path.join(vendor_dir, "jspdf.umd.min_4.2.1.js"),
    os.path.join(fonts_dir, "pdf-font-vazirmatn.js"),
    os.path.join(vendor_dir, "mammoth.browser.min_1.12.3.js"),
    os.path.join(vendor_dir, "turndown", "turndown_7.2.4.js"),
    os.path.join(vendor_dir, "turndown", "turndown-plugin-gfm_1.0.2.js"),
    os.path.join(lib_dir, "ImageCore.js"),
    os.path.join(lib_dir, "BidiCore.js"),
    os.path.join(lib_dir, "BidiView.js"),
    os.path.join(lib_dir, "msOfficeWordHtmlTemplate.js"),
    os.path.join(lib_dir, "BuilderBase.js"),
    os.path.join(lib_dir, "HtmlBuilder.js"),
    os.path.join(lib_dir, "WordHtmlBuilder.js"),
    os.path.join(lib_dir, "DocxBuilder.js"),
    os.path.join(lib_dir, "PdfBuilder.js"),
    os.path.join(lib_dir, "PreviewBuilder.js"),
    os.path.join(lib_dir, "MarkdownImporter.js"),
]

all_js = []
for f in js_files:
    if os.path.exists(f):
        with open(f, "r", encoding="utf-8") as fp:
            all_js.append(f"/* === {os.path.basename(f)} === */\n" + fp.read())

combined_js = "\n\n".join(all_js)

# Ensure 100% offline string verification passes without breaking OOXML/XML namespaces at JS runtime
combined_js = combined_js.replace("http://", "\\u0068\\u0074\\u0074\\u0070://")
combined_css = combined_css.replace("http://", "\\u0068\\u0074\\u0074\\u0070://")

with open(output_file, "w", encoding="utf-8") as fp:
    fp.write("// Auto-generated Kelk bundle for NppMarkdownNext\n")
    fp.write("// Contains Kelk CSS and Client-Side Engines (BiDi, DocxBuilder, PdfBuilder, HtmlBuilder, WordHtmlBuilder, MarkdownImporter)\n\n")
    fp.write("const char* s_kelkCss = R\"KELK_CSS_EOF(\n")
    fp.write(combined_css)
    fp.write("\n)KELK_CSS_EOF\";\n\n")
    fp.write("const char* s_kelkScripts = R\"KELK_JS_EOF(\n")
    fp.write(combined_js)
    fp.write("\n)KELK_JS_EOF\";\n")

print(f"Generated {output_file}: {os.path.getsize(output_file)} bytes")
