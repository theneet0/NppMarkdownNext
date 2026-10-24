#pragma once
#include <windows.h>
#include <string>
#include <vector>

enum class PreviewTheme {
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
    bool autoShowPanel = true; // show panel on Markdown tabs, hide it on others
    bool syncWithCaret = true;
    bool showOutline = false;
    float zoomLevel = 1.0f;
    bool isSmartBiDiEnabled = true;
    int darkModeOverride = -1; // -1: auto NPP, 0: Light, 1: Dark
    PreviewTheme theme = PreviewTheme::FollowNpp;
    std::wstring beardedVariant; // id of the Bearded variation ("" = classic/Arc)
    std::wstring supportedExtensions = L".md,.markdown,.mdown,.mkd,.rst";

    bool IsExtensionSupported(const std::wstring& ext) const;
    void Load(const std::wstring& configPath);
    void Save(const std::wstring& configPath) const;
};
