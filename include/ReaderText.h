#pragma once
#include <string>
#include <cstring>
#include "ReaderStrings.inc"

// The language of the plugin's own text, chosen by NppMarkdownNext (its setting, or Notepad++'s language).
inline bool& ReaderTextRussian() { static bool russian = false; return russian; }

// The Plugins menu text by command index (CMD_* in Resource.h).
inline constexpr const char* kPluginMenuKeys[] = {"menu.toggle", "menu.sync", "menu.bidi", "menu.themeFollow", "menu.themeLight",
    "menu.themeDark", "menu.zoomIn", "menu.zoomOut", "menu.zoomReset", "menu.copyHtml", "menu.saveHtml", "menu.about", "menu.settings",
    "menu.hideEditor"};

// The text for a key of themes/reader-strings.json in the current language; "{0}" takes the argument.
// An unknown key returns the key itself: visible in the UI, never an empty message.
inline std::wstring Tr(const char* key, const std::wstring& argument = L"") {
    for (const auto& entry : kReaderStrings) {
        if (std::strcmp(entry.key, key) != 0) continue;
        std::wstring text = ReaderTextRussian() ? entry.ru : entry.en;
        const size_t at = text.find(L"{0}");
        if (at != std::wstring::npos) text.replace(at, 3, argument);
        return text;
    }
    return std::wstring(key, key + std::strlen(key));
}
