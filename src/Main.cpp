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
