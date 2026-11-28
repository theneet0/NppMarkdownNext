# NppMarkdownNext (Kelk Reader Edition)

NppMarkdownNext is an ultra-lightweight native Markdown reader and live preview plugin for Notepad++, built on the core rendering engine of [Kelk](https://github.com/mohsen-1984/kelk). It embeds Microsoft Edge WebView2 (Chromium) to provide instant, high-performance rendering for GitHub Flavored Markdown (GFM) with first-class right-to-left (Persian, Arabic, Urdu, Hebrew) bidirectional support, syntax highlighting, and seamless Scintilla synchronization—with zero bloat.

## Key Features

- **Self-Contained Reader**: Bundled HTML, CSS, and JavaScript with no frontend package installation or CDN dependency.
- **Kelk BiDiCore & BidiView**: Flawless Persian, Arabic, Urdu, and Hebrew bidirectional layout with Unicode script classification, character normalization, and isolated code blocks.
- **GFM & Extensions**:
  - Headings, blockquotes, callout alerts (`[!NOTE]`, `[!TIP]`, `[!WARNING]`, `[!هشدار]`, `[!ВАЖНО]`, `[!ВНИМАНИЕ]` …).
  - GFM Tables with column alignment. A table wider than the reading column uses the free pane width; drag a header cell's right edge to resize a column (double-click resets). Widths are remembered per file across restarts.
  - Footnotes (`text[^1]` … `[^1]: note`).
  - Interactive Task Lists (`- [ ]`, `- [x]`) that synchronize directly with the Notepad++ editor buffer (your selection stays where it was).
  - Images with paths relative to the document's folder; links to other local files open in Notepad++ (documents and images in their default app).
  - Formulas and Mermaid diagrams are shown as their source text.
- **Real-Time Caret & Editor Synchronization**: Moving the editor caret scrolls the reader to the matching line. Double-clicking any element in the preview jumps the editor caret straight to the source line.
- **Preview Themes & Settings**: The animated gear menu provides Follow Notepad++, Light, Dark, Sepia, Monokai, Dracula, Solarized, Nord, GitHub, VS Code Dark, and all 64 Bearded variants, including hover previews. It also controls caret sync, outline, BiDi, auto-show, zoom, HTML copy/export, dark mode, and the language of the plugin's own text (English or Russian; by default the language Notepad++ is shown in). Open it from the preview or **Plugins > NppMarkdownNext > Settings...**. Existing settings are preserved.
- **Zero Network Dependencies**: 100% offline, private, and secure. Zero external font downloads or CDN tracking requests.
- **Export & Clipboard**: 1-click "Save As HTML" (named after the document, with a contents list for longer documents) and "Copy Rendered HTML" to clipboard.
- **Reader tools**: right-click menu (copy, copy link, copy code, copy table as HTML + tab-separated text, select all, find, print / save as PDF, outline), a copy button on code blocks and an outline button next to the settings button.

HTML saves use a temporary sibling file and preserve a recoverable original if replacement fails. Clipboard copy publishes HTML and Unicode text; the menu reports the native result, including partial copy and cancellation. Export still uses the native Markdown parser, so complex Markdown structures can differ from the live reader.

## Requirements

- **Operating System**: Windows 10, Windows 11, or Windows Server 2019+ (x64, x86, or ARM64).
- **Host Editor**: Notepad++ 8.x or later.
- **Runtime**: Microsoft Edge WebView2 Runtime (pre-installed on Windows 10/11).

## Installation

### Pre-built Binaries

1. Download the archive matching your Notepad++ architecture from [Releases](https://github.com/theneet0/NppMarkdownNext/releases):
   - `NppMarkdownNext-<version>-x64.zip` for 64-bit Notepad++
   - `NppMarkdownNext-<version>-x86.zip` for 32-bit Notepad++
   - `NppMarkdownNext-<version>-arm64.zip` for ARM64 Notepad++
2. Create a folder named `NppMarkdownNext` inside your Notepad++ `plugins` directory:
   - 64-bit: `C:\Program Files\Notepad++\plugins\NppMarkdownNext\`
   - 32-bit: `C:\Program Files (x86)\Notepad++\plugins\NppMarkdownNext\`
3. Extract `NppMarkdownNext.dll` and `WebView2Loader.dll` into that folder:
   ```text
   Notepad++\plugins\NppMarkdownNext\
       ├── NppMarkdownNext.dll
       └── WebView2Loader.dll
   ```
4. Restart Notepad++. Toggle the preview panel via **Plugins > NppMarkdownNext > Toggle Markdown Preview** or press `Ctrl+Shift+M`.

Release archives contain both DLLs at the ZIP root, plus documentation, theme notices, and `manifest.json`. The manifest records the architecture, source commit/digest, and file hashes. Hashes detect corruption; they do not authenticate the publisher.

### Optional Local Installer

The optional `tools/Install-NppMarkdownNext.ps1` script requires an explicit `-PackagePath`. From the repository root:

```powershell
.\tools\Install-NppMarkdownNext.ps1 -PackagePath .\Release\NppMarkdownNext-1.6.0-x64.zip -DryRun
.\tools\Install-NppMarkdownNext.ps1 -PackagePath .\Release\NppMarkdownNext-1.6.0-x64.zip
```

`-NppDirectory` selects a portable or alternate installation. `-DryRun` validates and displays the selected package without elevation, closing windows, or writing files. `-Force` skips the close confirmation but still requests graceful shutdown and waits for save dialogs; it never kills the editor. Only processes belonging to the selected executable are closed.

A successful installation prints its backup directory. Restore it with `-RollbackPath <backup-directory>` and the same `-NppDirectory`. Failed partial installations attempt rollback and retain a recovery directory if restoration also fails. User INI settings are not replaced.

## Keyboard Shortcuts

| Shortcut | Context | Action |
| :--- | :--- | :--- |
| `Ctrl+Shift+M` | Notepad++ | Toggle Markdown Preview dock panel |
| `Ctrl + +` / `+` | Preview panel | Zoom in |
| `Ctrl + -` / `-` | Preview panel | Zoom out |
| `Ctrl + 0` | Preview panel | Reset zoom level to 100% |
| `Ctrl + Wheel` | Preview panel | Zoom in / out |
| `Ctrl + F` | Preview panel | Open Edge's find bar (prefilled with the selected text) |
| `Ctrl + P` | Preview panel | Print or save as PDF |
| `Escape` | Preview panel | Close the context menu, find bar or Table of Contents drawer |
| `Double-Click` | Preview panel | Jump to source line in Notepad++ |

## Configuration

Settings are stored in `NppMarkdownNext.ini` located in the Notepad++ plugin configuration directory (`%APPDATA%\Notepad++\plugins\config\NppMarkdownNext.ini`).

```ini
[Settings]
; 1 = panel opened on startup, 0 = closed
IsPanelVisible=1

; 1 = preview scrolls with editor cursor, 0 = independent scrolling
SyncWithCaret=1

; 1 = show Table of Contents sidebar, 0 = hidden
ShowOutline=0

; 1 = detect RTL/LTR direction dynamically, 0 = LTR default
IsSmartBiDiEnabled=1

; -1 = follow Notepad++ theme, 0 = force Light mode, 1 = force Dark mode
DarkModeOverride=-1

; Zoom factor percentage (30 to 300)
ZoomLevelPercent=100

; Language of the plugin's menus and messages: auto (as Notepad++), en or ru
ReaderLanguage=auto

; Comma-separated list of file extensions that trigger auto-render
SupportedExtensions=.md,.markdown,.mdown,.mkd,.rst
```

Only settings changed in this Notepad++ window are written, so two open instances do not overwrite each other's choices. Per-document reader state (resized table columns, the reading place) is kept separately in `NppMarkdownNext.documents.ini` in the same folder (one line per document, keyed by a hash of its path; it follows a renamed or saved-as document).

## Architecture

NppMarkdownNext is written in native C++ using the standard Win32 API and Scintilla notification callbacks:

- **Scintilla Notification Loop**: Hooks `SCN_MODIFIED` and `SCN_UPDATEUI` to observe buffer edits. One render timer coalesces text changes with a 50 ms delay; lifecycle refreshes use a forced 1 ms request. Scintilla position fields match the pointer-sized notification ABI. Micro-caret movements that stay on the same line are filtered out before sending IPC messages.
- **AST Parser (`MarkdownParser`)**: Single-pass recursive tokenization into structured blocks (headers, lists, tables, blockquotes, code blocks, alerts, horizontal rules). The inline tokenizer handles bold/italic boundary markers, intra-word snake_case protection (e.g. `foo_bar_baz`), multi-backtick spans, and pipe escaping in table cells.
- **BiDi Processing (`BiDiEngine`)**: Analyzes text direction based on the first strong directional character, isolates directional text in code lines with `<bdi>` and directional CSS markers, and normalizes Arabic Yeh/Kaf characters to Persian equivalents.
- **WebView2 Viewer (`WebView2Viewer`)**: Hosts `ICoreWebView2Controller` and `ICoreWebView2`. Uses JSON messaging for renders and settings. Checkbox changes carry a buffer ID, render revision, source line, marker column, and expected state; the native host verifies the current source snapshot before editing.
- **Kelk Core Bundle (`KelkBundle.inc`)**: Bundled Marked.js, DOMPurify, Highlight.js, and Kelk's BiDiCore/BidiView direction engine. Shared theme, heading, and callout data generate the native and JavaScript views of those contracts.

## Building from Source

### Prerequisites

- Windows 10/11 or Windows Server.
- [LLVM](https://llvm.org/) (`clang++` on your `PATH`, or the default install location `C:\Program Files\LLVM\bin`), or [LLVM-MinGW](https://github.com/mstorsjo/llvm-mingw). `windres` is optional — the build script falls back to `llvm-rc` automatically.
- PowerShell 5.1 or PowerShell 7.
- Python 3.10+ for checking the generated theme catalog, and Git for source provenance.

### Build Instructions

1. Clone the repository:
   ```powershell
   git clone https://github.com/theneet0/NppMarkdownNext.git
   cd NppMarkdownNext
   ```

2. Run the build script:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\build.ps1
   ```
   The build script will:
   - Restore the Microsoft.Web.WebView2 NuGet package if not cached locally.
   - Compile and execute the native unit test suite (`tests/test_suite.cpp`).
   - Compile `NppMarkdownNext.dll` for x64, x86, and ARM64 architectures.
   - Copy the corresponding `WebView2Loader.dll` alongside each binary.

3. Package release archives:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\makerelease.ps1
   ```
   Release `.zip` packages for all supported architectures will be generated in the `Release/` directory.
   Packaging requires `build-info.json` from a successful build of each architecture. Source or binary changes after that build invalidate packaging; rebuild before retrying. Build metadata is invalidated before every compile attempt.

### Running Unit Tests Separately

To run only the unit test suite without building release binaries:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -TestsOnly
```

Or compile and run directly via Clang:

```powershell
clang++ -std=c++23 -O2 -Iinclude tests/test_suite.cpp src/MarkdownParser.cpp src/BiDiEngine.cpp src/SyntaxHighlighter.cpp src/HtmlExporter.cpp src/Config.cpp -luser32 -o bin/test_suite.exe
.\bin\test_suite.exe
```

`-TestsOnly` also builds and executes `test_export_io.exe` and `test_viewer_lifecycle.exe`. These support `--case <name>` for an individual scenario. Their Win32 fault hooks exist only in test builds; clipboard tests use an in-memory fake instead of changing the user's clipboard.

Run the packaging and optional installer checks separately:

```powershell
pwsh -NoProfile -File .\tests\test_package.ps1
pwsh -NoProfile -File .\tests\test_install.ps1
```

These checks create isolated fixtures under the ignored `bin` directory and validate ZIP structure, provenance, preflight failures, target process selection, and rollback. They do not install into the user's Notepad++.

### Running Settings UI Checks

Run `python tests/run_reader_ui.py` (needs `pip install playwright` and Microsoft Edge; CI runs it too).
It builds the mock-host reader page (`tests/reader_fixture.py`) and evaluates `tests/reader_ui_test.js` in Edge:
themes, settings, language switching, reader tools, export payload, privacy of remote content and layout bounds.
For the real Notepad++ and WebView2, run `pwsh tests/reader-native-smoke.ps1 -BinaryDirectory bin` after a build.

## License

The plugin source is licensed under the MIT License. See [License.txt](License.txt) for details. The bundled Bearded Theme palettes retain their GPL-3.0 license; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) and [themes/BEARDED_LICENSE](themes/BEARDED_LICENSE).
