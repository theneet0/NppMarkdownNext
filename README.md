# NppMarkdownNext (Kelk Reader Edition)

NppMarkdownNext is an ultra-lightweight native Markdown reader and live preview plugin for Notepad++, built on the core rendering engine of [Kelk](https://github.com/mohsen-1984/kelk). It embeds Microsoft Edge WebView2 (Chromium) to provide instant, high-performance rendering for GitHub Flavored Markdown (GFM) with first-class right-to-left (Persian, Arabic, Urdu, Hebrew) bidirectional support, syntax highlighting, and seamless Scintilla synchronization—with zero bloat.

## Key Features

- **Ultra-Lightweight & Fast**: Streamlined reader shell (~300 KB HTML/JS/CSS bundle) that initializes instantly and operates with minimal memory footprint.
- **Kelk BiDiCore & BidiView**: Flawless Persian, Arabic, Urdu, and Hebrew bidirectional layout with Unicode script classification, character normalization, and isolated code blocks.
- **GFM & Extensions**:
  - Headings, blockquotes, callout alerts (`[!NOTE]`, `[!TIP]`, `[!WARNING]`, `[!هشدار]`).
  - GFM Tables with alignment and responsive styling.
  - Interactive Task Lists (`- [ ]`, `- [x]`) that synchronize directly with the Notepad++ editor buffer.
  - Math expressions and Mermaid diagram containers.
- **Real-Time Caret & Editor Synchronization**: Moving the editor caret scrolls the reader to the matching line. Double-clicking any element in the preview jumps the editor caret straight to the source line.
- **Dark Mode & Theme Adaptation**: Seamlessly matches Notepad++ theme (Follow Notepad++, Light, Dark) without bloated third-party theme packs.
- **Zero Network Dependencies**: 100% offline, private, and secure. Zero external font downloads or CDN tracking requests.
- **Export & Clipboard**: 1-click "Save As HTML" and "Copy Rendered HTML" to clipboard.

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
- **WebView2 Viewer (`WebView2Viewer`)**: Hosts `ICoreWebView2Controller` and `ICoreWebView2`. Uses efficient JSON messaging (`render`, `scrollToLine`, `setTheme`, `setBiDi`) and direct message dispatch (`jumpToLine:<line>`, `toggleCheckbox:<line>`) between the Kelk web environment and the native Win32 window.
- **Kelk Core Bundle (`KelkBundle.inc`)**: Lightweight compilation of Marked.js, DOMPurify, Highlight.js, and Kelk's BiDiCore/BidiView direction engine, fitting under 300 KB for lightning-fast loads.

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
