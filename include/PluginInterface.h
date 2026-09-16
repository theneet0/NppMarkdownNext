#pragma once
#include <windows.h>
#include "Notepad_plus_msgs.h"
#include "Scintilla.h"

struct NppData {
    HWND _nppHandle;
    HWND _scintillaMainHandle;
    HWND _scintillaSecondHandle;
};

struct NppDarkModeColors {
    COLORREF background;
    COLORREF softerBackground;
    COLORREF hotBackground;
    COLORREF pureBackground;
    COLORREF errorBackground;
    COLORREF text;
    COLORREF darkerText;
    COLORREF disabledText;
    COLORREF edge;
    COLORREF linkText;
};

#define IDM_VIEW_RTL                   44072
#define IDM_VIEW_LTR                   44073

typedef void (*PFUNCPLUGINCMD)();

struct ShortcutKey {
    bool _isCtrl;
    bool _isAlt;
    bool _isShift;
    UCHAR _key;
};

struct FuncItem {
    wchar_t _itemName[64];
    PFUNCPLUGINCMD _pFunc;
    int _cmdID;
    bool _init2Check;
    ShortcutKey *_pShKey;
};

struct SCNotification {
    NMHDR nmhdr;
    int position;
    int ch;
    int modifiers;
    int modificationType;
    const char *text;
    int length;
    int linesAdded;
    int message;
    uintptr_t wParam;
    intptr_t lParam;
    int line;
    int foldLevelNow;
    int foldLevelPrev;
    int margin;
    int listType;
    int x;
    int y;
    int token;
    int annotationLinesAdded;
    int updated;
    int listCompletionMethod;
    int characterSource;
};

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void setInfo(NppData notpadPlusData);
__declspec(dllexport) const wchar_t * getName();
__declspec(dllexport) FuncItem * getFuncsArray(int *nbF);
__declspec(dllexport) void beNotified(SCNotification *notifyCode);
__declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) BOOL isUnicode();

#ifdef __cplusplus
}
#endif
