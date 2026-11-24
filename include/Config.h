#pragma once
#include <windows.h>
#include <string>

enum class ReaderTheme {
    FollowNpp = 0,
    Light = 1,
    Dark = 2,
    Sepia = 3,
    Monokai = 4,
    Dracula = 5,
    SolarizedLight = 6,
    SolarizedDark = 7,
    Nord = 8,
    GitHubLight = 9,
    GitHubDark = 10,
    VSCodeDark = 11,
    Bearded = 12
};

struct PluginConfig {
    bool isPanelVisible = false;
    bool autoShowPanel = true;
    bool syncWithCaret = true;
    bool showOutline = false;
    int darkModeOverride = -1;
    float zoomLevel = 1.0f;
    int contentFontSizePx = 15;
    int contentLineHeightPercent = 170;
    int contentMaxWidthPx = 860;
    bool isSmartBiDiEnabled = true;
    ReaderTheme theme = ReaderTheme::FollowNpp;
    std::wstring beardedVariant;
    std::wstring supportedExtensions = L".md,.markdown,.mdown,.mkd,.rst";

    bool IsExtensionSupported(const std::wstring& ext) const;
    std::wstring ThemeId() const;
    bool SetThemeId(const std::wstring& id);
    void Load(const std::wstring& configPath);
    bool Save(const std::wstring& configPath) const;
};
