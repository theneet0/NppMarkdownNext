# NppMarkdownNext

NppMarkdownNext is a native Markdown live preview and export plugin for Notepad++. It embeds the Microsoft Edge WebView2 (Chromium) runtime for responsive HTML5/CSS preview with LaTeX math and diagram rendering, and includes a fallback Direct2D/DirectWrite engine for machines without WebView2 installed.

## Features

- **Live Preview with Scintilla Synchronization**: Real-time rendering with debounced document parsing. Scrolling or moving the caret in Notepad++ automatically tracks the corresponding section in the preview, and clicking a heading or double-clicking any block jumps the editor caret to that source line.
- **Bi-Directional Task Lists**: Interactive GFM task lists (`- [ ]` and `- [x]`). Clicking a checkbox in the preview toggles the bracket syntax directly in the Notepad++ buffer via Scintilla message dispatch.
- **BiDi and RTL Support**: Autonomous paragraph-level direction detection (RTL/LTR) tailored for Persian, Arabic, and mixed technical text. Persian glyph normalization (Kaf/Yeh) and digit conversion are applied where appropriate. Code blocks maintain strict LTR or RTL isolation per line to prevent punctuation flipping.
- **LaTeX Math Support**: Inline (`$...$`) and display (`$$...$$`) math parsed into KaTeX-compatible markup and rendered offline. Parser heuristics prevent false positives on ordinary currency values (e.g. `$50 and $20`).
- **Diagrams & Flowcharts**: Embedded offline rendering for Mermaid-compatible flowchart definitions (` ```mermaid `) using clean vector SVGs.
- **Syntax Highlighting**: Built-in offline tokenization for common programming languages (C++, C, Python, JavaScript, TypeScript, Bash, Rust, Go, HTML, CSS, JSON, SQL, etc.).
- **Theme Matching**: Synchronizes automatically with Notepad++ Dark Mode (`NPPM_ISDARKMODEENABLED`), with an optional manual toggle in the context menu.
- **Outline & Search**: Slide-out Table of Contents drawer and in-page incremental search (`Ctrl+F`).
- **Export Options**: Export the current document as a standalone offline HTML file, or copy rendered HTML fragments directly to the system clipboard (`CF_HTML`).
- **Settings Menu**: A gear menu in the preview toolbar — and **Plugins > NppMarkdownNext > Settings...** — opens a single popover with theme selection, zoom presets (80/100/125/150%), dark mode override, caret sync, outline, smart BiDi, HTML copy/export and an About page. Every change is written to the INI file immediately.

## Requirements

- **Operating System**: Windows 10, Windows 11, or Windows Server 2019+ (x64, x86, or ARM64).
- **Host Editor**: Notepad++ 8.x or later.
- **Runtime**: Microsoft Edge WebView2 Evergreen Runtime (included by default in Windows 10/11). If WebView2 is not detected, the plugin falls back to its internal Direct2D 1.1 / DirectWrite hardware-accelerated renderer.

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

## Keyboard Shortcuts

| Shortcut | Context | Action |
| :--- | :--- | :--- |
| `Ctrl+Shift+M` | Notepad++ | Toggle Markdown Preview dock panel |
| `Ctrl + +` / `+` | Preview panel | Zoom in |
| `Ctrl + -` / `-` | Preview panel | Zoom out |
| `Ctrl + 0` | Preview panel | Reset zoom level to 100% |
| `Ctrl + Wheel` | Preview panel | Zoom in / out |
| `Ctrl + F` | Preview panel | Open in-page search bar |
| `Escape` | Preview panel | Close search bar or Table of Contents drawer |
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

; Comma-separated list of file extensions that trigger auto-render
SupportedExtensions=.md,.markdown,.mdown,.mkd,.rst
```

## Architecture

NppMarkdownNext is written in native C++ using the standard Win32 API and Scintilla notification callbacks:

- **Scintilla Notification Loop**: Hooks `SCN_MODIFIED` and `SCN_UPDATEUI` to observe buffer edits. To maintain responsiveness on large files, buffer checks verify the active file's extension before triggering debounced rendering (250 ms delay). Micro-caret movements that stay on the same line are filtered out before sending IPC messages.
- **AST Parser (`MarkdownParser`)**: Single-pass recursive tokenization into structured blocks (headers, lists, tables, blockquotes, code blocks, alerts, horizontal rules). The inline tokenizer handles bold/italic boundary markers, intra-word snake_case protection (e.g. `foo_bar_baz`), multi-backtick spans, and pipe escaping in table cells.
- **BiDi Processing (`BiDiEngine`)**: Analyzes text direction based on the first strong directional character, isolates directional text in code lines with `<bdi>` and directional CSS markers, and normalizes Arabic Yeh/Kaf characters to Persian equivalents.
- **WebView2 Viewer (`WebView2Viewer`)**: Hosts `ICoreWebView2Controller` and `ICoreWebView2`. Uses JSON-free IPC string messaging (`toggleCheckbox:<line>`, `jumpToLine:<line>`, `toggleTheme`, `toggleSync`) for bidirectional communication between the web environment and the native Win32 window.
- **Direct2D Fallback (`MarkdownRenderer`)**: Uses Direct2D 1.1 and DirectWrite text layouts with RTL reading direction (`DWRITE_READING_DIRECTION_RIGHT_TO_LEFT`) and locale-specific formatters (`fa-ir`) to provide clean native rendering without requiring WebView2 runtimes.

## Building from Source

### Prerequisites

- Windows 10/11 or Windows Server.
- [LLVM](https://llvm.org/) (`clang++` on your `PATH`, or the default install location `C:\Program Files\LLVM\bin`), or [LLVM-MinGW](https://github.com/mstorsjo/llvm-mingw). `windres` is optional — the build script falls back to `llvm-rc` automatically.
- PowerShell 5.1 or PowerShell 7.

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

### Running Unit Tests Separately

To run only the unit test suite without building release binaries:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -TestsOnly
```

Or compile and run directly via Clang:

```powershell
clang++ -std=c++23 -O2 -Iinclude tests/test_suite.cpp src/MarkdownParser.cpp src/BiDiEngine.cpp src/SyntaxHighlighter.cpp src/HtmlExporter.cpp -luser32 -o bin/test_suite.exe
.\bin\test_suite.exe
```

## License

This project is licensed under the MIT License. See [License.txt](License.txt) for details.
