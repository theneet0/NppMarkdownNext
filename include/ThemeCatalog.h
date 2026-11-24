#pragma once
#include "Config.h"
#include <string_view>

struct ThemeCatalogEntry {
    std::wstring_view id;
    std::wstring_view label;
    ReaderTheme theme;
    bool isDark;
};

#include "../src/ThemeCatalog.inc"
