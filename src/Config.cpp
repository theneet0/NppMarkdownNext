#include "../include/Config.h"
#include <sstream>
#include <algorithm>

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
        // Trim whitespace
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
    syncWithCaret = GetPrivateProfileIntW(sec, L"SyncWithCaret", 1, configPath.c_str()) != 0;
    showOutline = GetPrivateProfileIntW(sec, L"ShowOutline", 0, configPath.c_str()) != 0;
    isSmartBiDiEnabled = GetPrivateProfileIntW(sec, L"IsSmartBiDiEnabled", 1, configPath.c_str()) != 0;
    darkModeOverride = GetPrivateProfileIntW(sec, L"DarkModeOverride", -1, configPath.c_str());

    int zoomInt = GetPrivateProfileIntW(sec, L"ZoomLevelPercent", 100, configPath.c_str());
    zoomLevel = static_cast<float>(zoomInt) / 100.0f;
    if (zoomLevel < 0.3f) zoomLevel = 0.3f;
    if (zoomLevel > 3.0f) zoomLevel = 3.0f;

    wchar_t buf[512] = { 0 };
    GetPrivateProfileStringW(sec, L"SupportedExtensions", L".md,.markdown,.mdown,.mkd,.rst", buf, 512, configPath.c_str());
    supportedExtensions = buf;
}

void PluginConfig::Save(const std::wstring& configPath) const {
    if (configPath.empty()) return;

    const wchar_t* sec = L"Settings";
    WritePrivateProfileStringW(sec, L"IsPanelVisible", isPanelVisible ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(sec, L"SyncWithCaret", syncWithCaret ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(sec, L"ShowOutline", showOutline ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(sec, L"IsSmartBiDiEnabled", isSmartBiDiEnabled ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(sec, L"DarkModeOverride", std::to_wstring(darkModeOverride).c_str(), configPath.c_str());

    int zoomInt = static_cast<int>(zoomLevel * 100.0f + 0.5f);
    WritePrivateProfileStringW(sec, L"ZoomLevelPercent", std::to_wstring(zoomInt).c_str(), configPath.c_str());
    WritePrivateProfileStringW(sec, L"SupportedExtensions", supportedExtensions.c_str(), configPath.c_str());
}
