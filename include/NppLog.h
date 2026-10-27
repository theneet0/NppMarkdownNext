#pragma once

#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <string>

// Append-only startup/diagnostics logger shared by the plugin translation units.
// Always on: the "panel is blank at startup" class of bugs leaves no other
// trace, and the log is tiny (rotated at 1 MB). Also mirrors to the debugger.
inline void NppLog(const char* fmt, ...) {
    wchar_t tmp[MAX_PATH] = {};
    GetTempPathW(MAX_PATH, tmp);
    std::wstring path = std::wstring(tmp) + L"NppMarkdownNext.log";

    WIN32_FILE_ATTRIBUTE_DATA fad = {};
    if (GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &fad) &&
        fad.nFileSizeHigh == 0 && fad.nFileSizeLow > 1024u * 1024u) {
        DeleteFileW(path.c_str());
    }

    char buf[1024];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    if (len < 0) {
        strncpy_s(buf, sizeof(buf), "[Log formatting error]", _TRUNCATE);
    }

    SYSTEMTIME st;
    GetLocalTime(&st);
    char line[1200];
    int n = snprintf(line, sizeof(line), "[%02u:%02u:%02u.%03u] %s\r\n",
                     st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buf);
    if (n <= 0) return;

    HANDLE h = CreateFileW(path.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, nullptr,
                           OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    WriteFile(h, line, (DWORD)n, &written, nullptr);
    CloseHandle(h);
    OutputDebugStringA(line);
}
