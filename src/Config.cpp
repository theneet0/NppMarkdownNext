#include "../include/Config.h"
#include "../include/ThemeCatalog.h"
#include <sstream>
#include <algorithm>
#include <iterator>

namespace {
int ReadingChoice(int value, std::initializer_list<int> choices, int fallback) {
    return std::find(choices.begin(), choices.end(), value) != choices.end() ? value : fallback;
}
}

std::wstring PluginConfig::ThemeId() const {
    if (theme == ReaderTheme::Bearded && !beardedVariant.empty()) {
        if (const auto* entry = FindTheme(L"bearded-v-" + beardedVariant)) return std::wstring(entry->id);
    }
    const auto* entry = FindTheme(theme);
    return entry ? std::wstring(entry->id) : L"follow";
}

bool PluginConfig::SetThemeId(const std::wstring& id) {
    const auto* entry = FindTheme(id);
    if (!entry) return false;
    theme = entry->theme;
    constexpr std::wstring_view prefix = L"bearded-v-";
    beardedVariant = entry->id.starts_with(prefix) ? std::wstring(entry->id.substr(prefix.size())) : L"";
    return true;
}

bool PluginConfig::IsExtensionSupported(const std::wstring& ext) const {
    if (ext.empty()) return false;

    std::wstring lowerExt = ext;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::towlower);
    if (lowerExt.front() != L'.') lowerExt.insert(0, 1, L'.');

    std::wstring lowerSupported = supportedExtensions;
    std::transform(lowerSupported.begin(), lowerSupported.end(), lowerSupported.begin(), ::towlower);

    std::wstringstream ss(lowerSupported);
    std::wstring item;
    while (std::getline(ss, item, L',')) {
        while (!item.empty() && (item.front() == L' ' || item.front() == L'\t')) item.erase(0, 1);
        while (!item.empty() && (item.back() == L' ' || item.back() == L'\t')) item.pop_back();

        if (!item.empty()) {
            if (item.front() != L'.') item.insert(0, 1, L'.');
            if (item == lowerExt) return true;
        }
    }
    return false;
}

void PluginConfig::Load(const std::wstring& configPath) {
    if (configPath.empty()) return;

    const wchar_t* sec = L"Settings";
    isPanelVisible = GetPrivateProfileIntW(sec, L"IsPanelVisible", 0, configPath.c_str()) != 0;
    autoShowPanel = GetPrivateProfileIntW(sec, L"AutoShowPanel", 1, configPath.c_str()) != 0;
    syncWithCaret = GetPrivateProfileIntW(sec, L"SyncWithCaret", 1, configPath.c_str()) != 0;
    showOutline = GetPrivateProfileIntW(sec, L"ShowOutline", 0, configPath.c_str()) != 0;
    darkModeOverride = GetPrivateProfileIntW(sec, L"DarkModeOverride", -1, configPath.c_str());
    if (darkModeOverride < -1 || darkModeOverride > 1) darkModeOverride = -1;
    isSmartBiDiEnabled = GetPrivateProfileIntW(sec, L"IsSmartBiDiEnabled", 1, configPath.c_str()) != 0;

    theme = static_cast<ReaderTheme>(GetPrivateProfileIntW(sec, L"Theme", 0, configPath.c_str()));
    if (!FindTheme(theme)) theme = ReaderTheme::FollowNpp;
    wchar_t variantBuf[128] = { 0 };
    beardedVariant.clear();
    GetPrivateProfileStringW(sec, L"BeardedVariant", L"", variantBuf, 128, configPath.c_str());
    if (theme == ReaderTheme::Bearded && variantBuf[0]) {
        SetThemeId(L"bearded-v-" + std::wstring(variantBuf));
    }

    int zoomInt = GetPrivateProfileIntW(sec, L"ZoomLevelPercent", 100, configPath.c_str());
    zoomLevel = static_cast<float>(zoomInt) / 100.0f;
    if (zoomLevel < 0.3f) zoomLevel = 0.3f;
    if (zoomLevel > 3.0f) zoomLevel = 3.0f;
    contentFontSizePx = ReadingChoice(GetPrivateProfileIntW(sec, L"ContentFontSizePx", 15, configPath.c_str()), {15, 17, 19}, 15);
    contentLineHeightPercent = ReadingChoice(GetPrivateProfileIntW(sec, L"ContentLineHeightPercent", 170, configPath.c_str()), {150, 170, 190}, 170);
    contentMaxWidthPx = ReadingChoice(GetPrivateProfileIntW(sec, L"ContentMaxWidthPx", 860, configPath.c_str()), {680, 860, 1100}, 860);

    wchar_t buf[512] = { 0 };
    GetPrivateProfileStringW(sec, L"SupportedExtensions", L".md,.markdown,.mdown,.mkd,.rst", buf, 512, configPath.c_str());
    supportedExtensions = buf;
}

bool PluginConfig::Save(const std::wstring& configPath) const {
    if (configPath.empty()) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    const wchar_t* sec = L"Settings";
    DWORD firstError = ERROR_SUCCESS;
    auto write = [&](const wchar_t* key, const std::wstring& value) {
        SetLastError(ERROR_SUCCESS);
        if (!WritePrivateProfileStringW(sec, key, value.c_str(), configPath.c_str())) {
            DWORD error = GetLastError();
            if (firstError == ERROR_SUCCESS) firstError = error ? error : ERROR_WRITE_FAULT;
        }
    };
    write(L"IsPanelVisible", isPanelVisible ? L"1" : L"0");
    write(L"AutoShowPanel", autoShowPanel ? L"1" : L"0");
    write(L"SyncWithCaret", syncWithCaret ? L"1" : L"0");
    write(L"ShowOutline", showOutline ? L"1" : L"0");
    write(L"DarkModeOverride", std::to_wstring(darkModeOverride));
    write(L"IsSmartBiDiEnabled", isSmartBiDiEnabled ? L"1" : L"0");
    write(L"Theme", std::to_wstring(static_cast<int>(theme)));
    write(L"BeardedVariant", beardedVariant);

    int zoomInt = static_cast<int>(zoomLevel * 100.0f + 0.5f);
    write(L"ZoomLevelPercent", std::to_wstring(zoomInt));
    write(L"SupportedExtensions", supportedExtensions);
    write(L"ContentFontSizePx", std::to_wstring(contentFontSizePx));
    write(L"ContentLineHeightPercent", std::to_wstring(contentLineHeightPercent));
    write(L"ContentMaxWidthPx", std::to_wstring(contentMaxWidthPx));
    if (firstError != ERROR_SUCCESS) {
        SetLastError(firstError);
        return false;
    }
    return true;
}
