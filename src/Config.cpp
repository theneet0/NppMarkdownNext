#include "../include/Config.h"
#include "../include/ThemeCatalog.h"
#include "../include/ReaderPresets.inc"
#include <sstream>
#include <algorithm>
#include <iterator>

namespace {
template <size_t N> int ReadingChoice(int value, const int (&choices)[N], int fallback) {
    return std::find(std::begin(choices), std::end(choices), value) != std::end(choices) ? value : fallback;
}

// GetPrivateProfileInt reads "NaN" as 0, and 0 is a real choice (reading width = the whole pane): parse strictly.
int ReadStrictInt(const wchar_t* section, const wchar_t* key, int fallback, const std::wstring& path) {
    wchar_t raw[32] = {};
    GetPrivateProfileStringW(section, key, L"", raw, 32, path.c_str());
    wchar_t* end = nullptr;
    const long value = wcstol(raw, &end, 10);
    return raw[0] && end && *end == L'\0' ? static_cast<int>(value) : fallback;
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
    hideEditor = GetPrivateProfileIntW(sec, L"HideEditor", 0, configPath.c_str()) != 0;
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
    contentFontSizePx = ReadingChoice(ReadStrictInt(sec, L"ContentFontSizePx", 15, configPath), kFontSizePresets, 15);
    contentLineHeightPercent = ReadingChoice(ReadStrictInt(sec, L"ContentLineHeightPercent", 170, configPath), kLineHeightPresets, 170);
    contentMaxWidthPx = ReadingChoice(ReadStrictInt(sec, L"ContentMaxWidthPx", 860, configPath), kReadingWidthPresets, 860);
    wchar_t language[8] = {};
    GetPrivateProfileStringW(sec, L"ReaderLanguage", L"auto", language, 8, configPath.c_str());
    readerLanguage = (std::wstring(language) == L"en" || std::wstring(language) == L"ru") ? language : L"auto";
    outlineWidthPx = std::clamp(ReadStrictInt(sec, L"OutlineWidthPx", kOutlineWidthDefault, configPath), kOutlineWidthMin, kOutlineWidthMax);

    wchar_t buf[512] = { 0 };
    GetPrivateProfileStringW(sec, L"SupportedExtensions", L".md,.markdown,.mdown,.mkd,.rst", buf, 512, configPath.c_str());
    supportedExtensions = buf;

    // What the file holds now (absent keys stay absent, so the first Save still writes them).
    m_persisted.clear();
    m_persistedPath = configPath;
    for (const wchar_t* key : {L"IsPanelVisible", L"AutoShowPanel", L"HideEditor", L"SyncWithCaret", L"ShowOutline", L"DarkModeOverride",
                               L"IsSmartBiDiEnabled", L"Theme", L"BeardedVariant", L"ZoomLevelPercent", L"SupportedExtensions",
                               L"ContentFontSizePx", L"ContentLineHeightPercent", L"ContentMaxWidthPx", L"OutlineWidthPx", L"ReaderLanguage"}) {
        wchar_t raw[512] = {};
        const wchar_t* absent = L"\x1f<absent>";
        GetPrivateProfileStringW(sec, key, absent, raw, 512, configPath.c_str());
        if (wcscmp(raw, absent) != 0) m_persisted[key] = raw;
    }
}

bool PluginConfig::Save(const std::wstring& configPath) const {
    if (configPath.empty()) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    const wchar_t* sec = L"Settings";
    DWORD firstError = ERROR_SUCCESS;
    // Only keys this instance changed are written: a second Notepad++ instance that changed another setting
    // keeps its value instead of having it overwritten by this instance's startup snapshot.
    auto write = [&](const wchar_t* key, const std::wstring& value) {
        if (configPath == m_persistedPath) {
            auto known = m_persisted.find(key);
            if (known != m_persisted.end() && known->second == value) return;
        }
        SetLastError(ERROR_SUCCESS);
        if (!WritePrivateProfileStringW(sec, key, value.c_str(), configPath.c_str())) {
            DWORD error = GetLastError();
            if (firstError == ERROR_SUCCESS) firstError = error ? error : ERROR_WRITE_FAULT;
        } else if (configPath == m_persistedPath) {
            m_persisted[key] = value;
        }
    };
    write(L"IsPanelVisible", isPanelVisible ? L"1" : L"0");
    write(L"AutoShowPanel", autoShowPanel ? L"1" : L"0");
    write(L"HideEditor", hideEditor ? L"1" : L"0");
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
    write(L"OutlineWidthPx", std::to_wstring(outlineWidthPx));
    write(L"ReaderLanguage", readerLanguage);
    if (firstError != ERROR_SUCCESS) {
        SetLastError(firstError);
        return false;
    }
    return true;
}
