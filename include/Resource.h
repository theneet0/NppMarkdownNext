#pragma once

#define IDI_ICON_PANEL                 101
#define IDB_TOOLBAR_BMP                102

// Plugin Commands
#define CMD_TOGGLE_PANEL               0
#define CMD_SYNC_CARET                 1
#define CMD_TOGGLE_BIDI                2
#define CMD_THEME_FOLLOW_NPP           3
#define CMD_THEME_LIGHT                4
#define CMD_THEME_DARK                 5
#define CMD_ZOOM_IN                    6
#define CMD_ZOOM_OUT                   7
#define CMD_ZOOM_RESET                 8
#define CMD_COPY_HTML                  9
#define CMD_SAVE_HTML                  10
#define CMD_ABOUT                      11
#define CMD_SETTINGS                   12
#define CMD_TOGGLE_HIDE_EDITOR         13
#define NB_PLUGIN_COMMANDS             14

// Timer IDs
#define IDT_RENDER_DEBOUNCE            3001
#define RENDER_DEBOUNCE_DELAY_MS       50
#define IDT_VIEWER_LOADING             3002
#define VIEWER_LOADING_TIMEOUT_MS      20000

#define WM_APP_VIEWER_RECOVERY         (WM_APP + 0x41)
#define WM_APP_EXPORT_REQUEST          (WM_APP + 0x42)
#define WM_APP_EXPORT_RESULT           (WM_APP + 0x43)
#define WM_APP_HOST_COMMAND            (WM_APP + 0x44)
#define WM_APP_SYNC_OVERLAY            (WM_APP + 0x45)
#define IDC_RETRY_VIEWER               5001
