#pragma once
#include <windows.h>
#include <string>
#include <vector>

struct PluginConfig {
    bool isPanelVisible = false;
    bool syncWithCaret = true;
    bool showOutline = false;
    float zoomLevel = 1.0f;
    bool isSmartBiDiEnabled = true;
    int darkModeOverride = -1; // -1: auto NPP, 0: Light, 1: Dark
    std::wstring supportedExtensions = L".md,.markdown,.mdown,.mkd,.rst";

    bool IsExtensionSupported(const std::wstring& ext) const;
    void Load(const std::wstring& configPath);
    void Save(const std::wstring& configPath) const;
};
