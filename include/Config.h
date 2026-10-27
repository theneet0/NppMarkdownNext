#pragma once
#include <windows.h>
#include <string>

enum class ReaderTheme {
    FollowNpp = 0,
    Light = 1,
    Dark = 2
};

struct PluginConfig {
    bool isPanelVisible = false;
    bool autoShowPanel = true;
    bool syncWithCaret = true;
    float zoomLevel = 1.0f;
    bool isSmartBiDiEnabled = true;
    ReaderTheme theme = ReaderTheme::FollowNpp;
    std::wstring supportedExtensions = L".md,.markdown,.mdown,.mkd,.rst";

    bool IsExtensionSupported(const std::wstring& ext) const;
    void Load(const std::wstring& configPath);
    void Save(const std::wstring& configPath) const;
};
