#include <windows.h>
#include "../include/PluginInterface.h"
#include "../include/Resource.h"
#include "../include/NppMarkdownNext.h"

HINSTANCE g_hInst = nullptr;
NppData g_nppData = {};
FuncItem g_funcItems[NB_PLUGIN_COMMANDS] = {};
ShortcutKey g_shortcuts[NB_PLUGIN_COMMANDS] = {};

void CommandTogglePanel() {
    NppMarkdownNext::Instance().TogglePanel();
}

void CommandSyncCaret() {
    NppMarkdownNext::Instance().ToggleSyncWithCaret();
}

void CommandToggleOutline() {
    NppMarkdownNext::Instance().ToggleOutline();
}

void CommandCopyHtml() {
    NppMarkdownNext::Instance().CopyRenderedHtml();
}

void CommandSaveHtml() {
    NppMarkdownNext::Instance().SaveAsHtml();
}

void CommandZoomIn() {
    NppMarkdownNext::Instance().ZoomIn();
}

void CommandZoomOut() {
    NppMarkdownNext::Instance().ZoomOut();
}

void CommandZoomReset() {
    NppMarkdownNext::Instance().ZoomReset();
}

void CommandToggleBiDi() {
    NppMarkdownNext::Instance().ToggleBiDi();
}

void CommandAbout() {
    NppMarkdownNext::Instance().ShowAbout();
}

void CommandThemeFollowNpp() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::FollowNpp);
}

void CommandThemeLight() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Light);
}

void CommandThemeDark() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Dark);
}

void CommandThemeSepia() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Sepia);
}

void CommandThemeMonokai() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Monokai);
}

void CommandThemeDracula() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Dracula);
}

void CommandThemeSolarizedLight() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::SolarizedLight);
}

void CommandThemeSolarizedDark() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::SolarizedDark);
}

void CommandThemeNord() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Nord);
}

void CommandThemeGitHubLight() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::GitHubLight);
}

void CommandThemeGitHubDark() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::GitHubDark);
}

void CommandThemeVSCodeDark() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::VSCodeDark);
}

void CommandThemeBearded() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Bearded);
}

void CommandThemeBeardedGold() {
    NppMarkdownNext::Instance().SelectTheme(PreviewTheme::Bearded, L"gold-d-raynh");
}

void CommandSettings() {
    NppMarkdownNext::Instance().OpenSettings();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            g_hInst = hinstDLL;
            break;
        case DLL_PROCESS_DETACH:
            NppMarkdownNext::Instance().Cleanup();
            break;
    }
    return TRUE;
}

extern "C" {

__declspec(dllexport) void setInfo(NppData notpadPlusData) {
    g_nppData = notpadPlusData;
    NppMarkdownNext::Instance().Init(g_hInst, g_nppData);
}

__declspec(dllexport) const wchar_t * getName() {
    return L"NppMarkdownNext";
}

__declspec(dllexport) FuncItem * getFuncsArray(int *nbF) {
    *nbF = NB_PLUGIN_COMMANDS;

    // 0: Toggle Markdown Preview (Ctrl+Shift+M)
    wcscpy_s(g_funcItems[CMD_TOGGLE_PANEL]._itemName, L"Toggle Markdown Preview");
    g_funcItems[CMD_TOGGLE_PANEL]._pFunc = CommandTogglePanel;
    g_funcItems[CMD_TOGGLE_PANEL]._cmdID = CMD_TOGGLE_PANEL;
    g_funcItems[CMD_TOGGLE_PANEL]._init2Check = false;
    g_shortcuts[CMD_TOGGLE_PANEL]._isCtrl = true;
    g_shortcuts[CMD_TOGGLE_PANEL]._isAlt = false;
    g_shortcuts[CMD_TOGGLE_PANEL]._isShift = true;
    g_shortcuts[CMD_TOGGLE_PANEL]._key = 'M';
    g_funcItems[CMD_TOGGLE_PANEL]._pShKey = &g_shortcuts[CMD_TOGGLE_PANEL];

    // 1: Synchronize with Editor Caret
    wcscpy_s(g_funcItems[CMD_SYNC_CARET]._itemName, L"Synchronize viewer with caret position");
    g_funcItems[CMD_SYNC_CARET]._pFunc = CommandSyncCaret;
    g_funcItems[CMD_SYNC_CARET]._cmdID = CMD_SYNC_CARET;
    g_funcItems[CMD_SYNC_CARET]._init2Check = true;
    g_funcItems[CMD_SYNC_CARET]._pShKey = nullptr;

    // 2: Document Outline
    wcscpy_s(g_funcItems[CMD_TOGGLE_OUTLINE]._itemName, L"Show Outline / Table of Contents");
    g_funcItems[CMD_TOGGLE_OUTLINE]._pFunc = CommandToggleOutline;
    g_funcItems[CMD_TOGGLE_OUTLINE]._cmdID = CMD_TOGGLE_OUTLINE;
    g_funcItems[CMD_TOGGLE_OUTLINE]._init2Check = false;
    g_funcItems[CMD_TOGGLE_OUTLINE]._pShKey = nullptr;

    // 3: Copy HTML
    wcscpy_s(g_funcItems[CMD_COPY_HTML]._itemName, L"Copy to clipboard (HTML)");
    g_funcItems[CMD_COPY_HTML]._pFunc = CommandCopyHtml;
    g_funcItems[CMD_COPY_HTML]._cmdID = CMD_COPY_HTML;
    g_funcItems[CMD_COPY_HTML]._init2Check = false;
    g_funcItems[CMD_COPY_HTML]._pShKey = nullptr;

    // 4: Save As HTML
    wcscpy_s(g_funcItems[CMD_SAVE_HTML]._itemName, L"Save As HTML...");
    g_funcItems[CMD_SAVE_HTML]._pFunc = CommandSaveHtml;
    g_funcItems[CMD_SAVE_HTML]._cmdID = CMD_SAVE_HTML;
    g_funcItems[CMD_SAVE_HTML]._init2Check = false;
    g_funcItems[CMD_SAVE_HTML]._pShKey = nullptr;

    // 5: Zoom In
    wcscpy_s(g_funcItems[CMD_ZOOM_IN]._itemName, L"Zoom In");
    g_funcItems[CMD_ZOOM_IN]._pFunc = CommandZoomIn;
    g_funcItems[CMD_ZOOM_IN]._cmdID = CMD_ZOOM_IN;
    g_funcItems[CMD_ZOOM_IN]._init2Check = false;
    g_funcItems[CMD_ZOOM_IN]._pShKey = nullptr;

    // 6: Zoom Out
    wcscpy_s(g_funcItems[CMD_ZOOM_OUT]._itemName, L"Zoom Out");
    g_funcItems[CMD_ZOOM_OUT]._pFunc = CommandZoomOut;
    g_funcItems[CMD_ZOOM_OUT]._cmdID = CMD_ZOOM_OUT;
    g_funcItems[CMD_ZOOM_OUT]._init2Check = false;
    g_funcItems[CMD_ZOOM_OUT]._pShKey = nullptr;

    // 7: Reset Zoom
    wcscpy_s(g_funcItems[CMD_ZOOM_RESET]._itemName, L"Reset Zoom (100%)");
    g_funcItems[CMD_ZOOM_RESET]._pFunc = CommandZoomReset;
    g_funcItems[CMD_ZOOM_RESET]._cmdID = CMD_ZOOM_RESET;
    g_funcItems[CMD_ZOOM_RESET]._init2Check = false;
    g_funcItems[CMD_ZOOM_RESET]._pShKey = nullptr;

    // 8: Smart BiDi
    wcscpy_s(g_funcItems[CMD_TOGGLE_BIDI]._itemName, L"Smart BiDi (Persian/Arabic RTL)");
    g_funcItems[CMD_TOGGLE_BIDI]._pFunc = CommandToggleBiDi;
    g_funcItems[CMD_TOGGLE_BIDI]._cmdID = CMD_TOGGLE_BIDI;
    g_funcItems[CMD_TOGGLE_BIDI]._init2Check = true;
    g_funcItems[CMD_TOGGLE_BIDI]._pShKey = nullptr;

    // 9: About
    wcscpy_s(g_funcItems[CMD_ABOUT]._itemName, L"About NppMarkdownNext...");
    g_funcItems[CMD_ABOUT]._pFunc = CommandAbout;
    g_funcItems[CMD_ABOUT]._cmdID = CMD_ABOUT;
    g_funcItems[CMD_ABOUT]._init2Check = false;
    g_funcItems[CMD_ABOUT]._pShKey = nullptr;

    // 10: Theme - Follow Notepad++
    wcscpy_s(g_funcItems[CMD_THEME_FOLLOW_NPP]._itemName, L"Theme: Follow Notepad++");
    g_funcItems[CMD_THEME_FOLLOW_NPP]._pFunc = CommandThemeFollowNpp;
    g_funcItems[CMD_THEME_FOLLOW_NPP]._cmdID = CMD_THEME_FOLLOW_NPP;
    g_funcItems[CMD_THEME_FOLLOW_NPP]._init2Check = false;
    g_funcItems[CMD_THEME_FOLLOW_NPP]._pShKey = nullptr;

    // 11: Theme - Light
    wcscpy_s(g_funcItems[CMD_THEME_LIGHT]._itemName, L"Theme: Light");
    g_funcItems[CMD_THEME_LIGHT]._pFunc = CommandThemeLight;
    g_funcItems[CMD_THEME_LIGHT]._cmdID = CMD_THEME_LIGHT;
    g_funcItems[CMD_THEME_LIGHT]._init2Check = false;
    g_funcItems[CMD_THEME_LIGHT]._pShKey = nullptr;

    // 12: Theme - Dark
    wcscpy_s(g_funcItems[CMD_THEME_DARK]._itemName, L"Theme: Dark");
    g_funcItems[CMD_THEME_DARK]._pFunc = CommandThemeDark;
    g_funcItems[CMD_THEME_DARK]._cmdID = CMD_THEME_DARK;
    g_funcItems[CMD_THEME_DARK]._init2Check = false;
    g_funcItems[CMD_THEME_DARK]._pShKey = nullptr;

    // 13: Theme - Sepia
    wcscpy_s(g_funcItems[CMD_THEME_SEPIA]._itemName, L"Theme: Sepia");
    g_funcItems[CMD_THEME_SEPIA]._pFunc = CommandThemeSepia;
    g_funcItems[CMD_THEME_SEPIA]._cmdID = CMD_THEME_SEPIA;
    g_funcItems[CMD_THEME_SEPIA]._init2Check = false;
    g_funcItems[CMD_THEME_SEPIA]._pShKey = nullptr;

    // 14: Theme - Monokai
    wcscpy_s(g_funcItems[CMD_THEME_MONOKAI]._itemName, L"Theme: Monokai");
    g_funcItems[CMD_THEME_MONOKAI]._pFunc = CommandThemeMonokai;
    g_funcItems[CMD_THEME_MONOKAI]._cmdID = CMD_THEME_MONOKAI;
    g_funcItems[CMD_THEME_MONOKAI]._init2Check = false;
    g_funcItems[CMD_THEME_MONOKAI]._pShKey = nullptr;

    // 15: Theme - Dracula
    wcscpy_s(g_funcItems[CMD_THEME_DRACULA]._itemName, L"Theme: Dracula");
    g_funcItems[CMD_THEME_DRACULA]._pFunc = CommandThemeDracula;
    g_funcItems[CMD_THEME_DRACULA]._cmdID = CMD_THEME_DRACULA;
    g_funcItems[CMD_THEME_DRACULA]._init2Check = false;
    g_funcItems[CMD_THEME_DRACULA]._pShKey = nullptr;

    // 16: Theme - Solarized Light
    wcscpy_s(g_funcItems[CMD_THEME_SOLARIZED_LIGHT]._itemName, L"Theme: Solarized Light");
    g_funcItems[CMD_THEME_SOLARIZED_LIGHT]._pFunc = CommandThemeSolarizedLight;
    g_funcItems[CMD_THEME_SOLARIZED_LIGHT]._cmdID = CMD_THEME_SOLARIZED_LIGHT;
    g_funcItems[CMD_THEME_SOLARIZED_LIGHT]._init2Check = false;
    g_funcItems[CMD_THEME_SOLARIZED_LIGHT]._pShKey = nullptr;

    // 17: Theme - Solarized Dark
    wcscpy_s(g_funcItems[CMD_THEME_SOLARIZED_DARK]._itemName, L"Theme: Solarized Dark");
    g_funcItems[CMD_THEME_SOLARIZED_DARK]._pFunc = CommandThemeSolarizedDark;
    g_funcItems[CMD_THEME_SOLARIZED_DARK]._cmdID = CMD_THEME_SOLARIZED_DARK;
    g_funcItems[CMD_THEME_SOLARIZED_DARK]._init2Check = false;
    g_funcItems[CMD_THEME_SOLARIZED_DARK]._pShKey = nullptr;

    // 18: Theme - Nord
    wcscpy_s(g_funcItems[CMD_THEME_NORD]._itemName, L"Theme: Nord");
    g_funcItems[CMD_THEME_NORD]._pFunc = CommandThemeNord;
    g_funcItems[CMD_THEME_NORD]._cmdID = CMD_THEME_NORD;
    g_funcItems[CMD_THEME_NORD]._init2Check = false;
    g_funcItems[CMD_THEME_NORD]._pShKey = nullptr;

    // 19: Theme - GitHub Light
    wcscpy_s(g_funcItems[CMD_THEME_GITHUB_LIGHT]._itemName, L"Theme: GitHub Light");
    g_funcItems[CMD_THEME_GITHUB_LIGHT]._pFunc = CommandThemeGitHubLight;
    g_funcItems[CMD_THEME_GITHUB_LIGHT]._cmdID = CMD_THEME_GITHUB_LIGHT;
    g_funcItems[CMD_THEME_GITHUB_LIGHT]._init2Check = false;
    g_funcItems[CMD_THEME_GITHUB_LIGHT]._pShKey = nullptr;

    // 20: Theme - GitHub Dark
    wcscpy_s(g_funcItems[CMD_THEME_GITHUB_DARK]._itemName, L"Theme: GitHub Dark");
    g_funcItems[CMD_THEME_GITHUB_DARK]._pFunc = CommandThemeGitHubDark;
    g_funcItems[CMD_THEME_GITHUB_DARK]._cmdID = CMD_THEME_GITHUB_DARK;
    g_funcItems[CMD_THEME_GITHUB_DARK]._init2Check = false;
    g_funcItems[CMD_THEME_GITHUB_DARK]._pShKey = nullptr;

    // 21: Theme - VSCode Dark
    wcscpy_s(g_funcItems[CMD_THEME_VSCODE_DARK]._itemName, L"Theme: VSCode Dark");
    g_funcItems[CMD_THEME_VSCODE_DARK]._pFunc = CommandThemeVSCodeDark;
    g_funcItems[CMD_THEME_VSCODE_DARK]._cmdID = CMD_THEME_VSCODE_DARK;
    g_funcItems[CMD_THEME_VSCODE_DARK]._init2Check = false;
    g_funcItems[CMD_THEME_VSCODE_DARK]._pShKey = nullptr;

    // 22: Theme - Bearded
    wcscpy_s(g_funcItems[CMD_THEME_BEARDED]._itemName, L"Theme: Bearded");
    g_funcItems[CMD_THEME_BEARDED]._pFunc = CommandThemeBearded;
    g_funcItems[CMD_THEME_BEARDED]._cmdID = CMD_THEME_BEARDED;
    g_funcItems[CMD_THEME_BEARDED]._init2Check = false;
    g_funcItems[CMD_THEME_BEARDED]._pShKey = nullptr;

    // 23: Theme - Bearded Gold D Raynh
    wcscpy_s(g_funcItems[CMD_THEME_BEARDED_GOLD]._itemName, L"Theme: Bearded Gold D Raynh");
    g_funcItems[CMD_THEME_BEARDED_GOLD]._pFunc = CommandThemeBeardedGold;
    g_funcItems[CMD_THEME_BEARDED_GOLD]._cmdID = CMD_THEME_BEARDED_GOLD;
    g_funcItems[CMD_THEME_BEARDED_GOLD]._init2Check = false;
    g_funcItems[CMD_THEME_BEARDED_GOLD]._pShKey = nullptr;

    // 24: Settings (opens the settings popover in the preview panel)
    wcscpy_s(g_funcItems[CMD_SETTINGS]._itemName, L"Settings...");
    g_funcItems[CMD_SETTINGS]._pFunc = CommandSettings;
    g_funcItems[CMD_SETTINGS]._cmdID = CMD_SETTINGS;
    g_funcItems[CMD_SETTINGS]._init2Check = false;
    g_funcItems[CMD_SETTINGS]._pShKey = nullptr;

    return g_funcItems;
}

int GetPluginCmdId(int index) {
    if (index >= 0 && index < NB_PLUGIN_COMMANDS) {
        return g_funcItems[index]._cmdID;
    }
    return -1;
}

__declspec(dllexport) void beNotified(SCNotification *notifyCode) {
    if (!notifyCode) return;

    switch (notifyCode->nmhdr.code) {
        case NPPN_TBMODIFICATION: {
            // Register toolbar icon in Notepad++
            toolbarIcons tbIcons = {};
            tbIcons.hToolbarBmp = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TOOLBAR_BMP));
            tbIcons.hToolbarIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_PANEL));
            SendMessage(g_nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)g_funcItems[CMD_TOGGLE_PANEL]._cmdID, (LPARAM)&tbIcons);
            break;
        }

        case NPPN_READY:
            NppMarkdownNext::Instance().OnNppReady();
            break;

        case NPPN_DARKMODECHANGED:
            NppMarkdownNext::Instance().OnDarkModeChanged();
            break;

        case NPPN_BUFFERACTIVATED:
            NppMarkdownNext::Instance().OnBufferActivated();
            break;

        default:
            NppMarkdownNext::Instance().OnNotification(notifyCode);
            break;
    }
}

__declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam) {
    return TRUE;
}

__declspec(dllexport) BOOL isUnicode() {
    return TRUE;
}

}
