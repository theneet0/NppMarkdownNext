#pragma once
#include <windows.h>

#define SCI_GETTEXT                    2182
#define SCI_GETLENGTH                  2006
#define SCI_GETSELECTIONSTART          2143
#define SCI_GETSELECTIONEND            2145
#define SCI_GETSELTEXT                 2161
#define SCI_SETSEL                     2160
#define SCI_SETTEXT                    2181
#define SCI_REPLACESEL                 2170
#define SCI_BEGINUNDOACTION            2115
#define SCI_ENDUNDOACTION              2116
#define SCI_GETCURRENTPOS              2008
#define SCI_LINEFROMPOSITION           2166
#define SCI_POSITIONFROMLINE           2167
#define SCI_GETFIRSTVISIBLELINE        2152
#define SCI_LINESCROLL                 2168
#define SCI_SETFIRSTVISIBLELINE        2613
#define SCI_GETLINECOUNT               2154
#define SCI_LINESONSCREEN              2370
#define SCI_GETLINE                    2153
#define SCI_LINELENGTH                 2350

#define SCI_GETBIDIRECTIONAL           2700
#define SCI_SETBIDIRECTIONAL           2701
#define SC_BIDIRECTIONAL_DISABLED      0
#define SC_BIDIRECTIONAL_L2R           1
#define SC_BIDIRECTIONAL_R2L           2

#define SCI_SETTECHNOLOGY              4030
#define SC_TECHNOLOGY_DEFAULT          0
#define SC_TECHNOLOGY_DIRECTWRITE      1

// Notifications
#define SCN_STYLENEEDED                2000
#define SCN_CHARADDED                  2001
#define SCN_SAVEPOINTREACHED           2002
#define SCN_SAVEPOINTLEFT              2003
#define SCN_MODIFYATTEMPTRO            2004
#define SCN_KEY                        2005
#define SCN_DOUBLECLICK                2006
#define SCN_UPDATEUI                   2007
#define SCN_MODIFIED                   2008

#define SC_MOD_INSERTTEXT              0x01
#define SC_MOD_DELETETEXT              0x02
#define SC_MOD_CHANGESTYLE             0x04
#define SC_MOD_CHANGEFOLD              0x08
#define SC_PERFORMED_USER              0x10
#define SC_PERFORMED_UNDO              0x20
#define SC_PERFORMED_REDO              0x40
#define SC_MULTISTEPUNDOREDO           0x80
#define SC_LASTSTEPINUNDOREDO          0x100
#define SC_MOD_CHANGEMARKER            0x200
#define SC_MOD_BEFOREINSERT            0x400
#define SC_MOD_BEFOREDELETE            0x800
#define SC_MULTILINEUNDOREDO           0x1000
#define SC_STARTACTION                 0x2000
#define SC_MOD_CHANGEINDICATOR         0x4000
#define SC_MOD_CHANGELINESTATE         0x8000
#define SC_MOD_CHANGEMARGIN            0x10000
#define SC_MOD_CHANGEANNOTATION        0x20000
#define SC_MOD_CONTAINER               0x40000
#define SC_MOD_LEXERSTATE              0x80000
#define SC_MOD_INSERTCHECK             0x100000
#define SC_MOD_HIGHLIGHTRANGE          0x200000
#define SC_MODEVENTMASKALL             0x3FFFFF

#define SC_UPDATE_CONTENT              0x01
#define SC_UPDATE_SELECTION            0x02
#define SC_UPDATE_V_SCROLL             0x04
#define SC_UPDATE_H_SCROLL             0x08
