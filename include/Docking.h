#pragma once
#include <windows.h>

#define CONT_LEFT               0
#define CONT_RIGHT              1
#define CONT_TOP                2
#define CONT_BOTTOM             3
#define DOCKCONT_MAX            4

// mask params for plugins of internal dialogs
#define DWS_ICONTAB             0x00000001
#define DWS_ICONBAR             0x00000002
#define DWS_ADDINFO             0x00000004
#define DWS_USEOWNDARKMODE      0x00000008
#define DWS_PARAMSALL           (DWS_ICONTAB | DWS_ICONBAR | DWS_ADDINFO)

// Docking notifications sent to hClient via WM_NOTIFY
#define DMN_FIRST               1050
#define DMN_CLOSE               (DMN_FIRST + 1)
#define DMN_DOCK                (DMN_FIRST + 2)
#define DMN_FLOAT               (DMN_FIRST + 3)

// default docking values for first call of plugin
#define DWS_DF_CONT_LEFT        (CONT_LEFT << 28)
#define DWS_DF_CONT_RIGHT       (CONT_RIGHT << 28)
#define DWS_DF_CONT_TOP         (CONT_TOP << 28)
#define DWS_DF_CONT_BOTTOM      (CONT_BOTTOM << 28)
#define DWS_DF_FLOATING         0x80000000

#ifndef NPPM_ADDTOOLBARICON
#define NPPM_ADDTOOLBARICON     NPPM_ADDTOOLBARICON_DEPRECATED
#endif

struct tTbData {
    HWND            hClient;
    const wchar_t*  pszName;
    int             dlgID;
    UINT            uMask;
    HICON           hIconTab;
    const wchar_t*  pszAddInfo;
    RECT            rcFloat;
    int             iPrevCont;
    const wchar_t*  pszModuleName;
};
