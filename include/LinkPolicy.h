#pragma once
#include <string>
#include <cwctype>

// The scheme of a link target as a browser would read it ("https", "javascript", ...), lower-cased; empty for
// relative paths, anchors and drive paths ("C:\..." is a drive, not a scheme). Browsers drop leading
// whitespace/control characters and tabs/newlines anywhere, so "java\tscript:" is still "javascript".
inline std::wstring LinkScheme(const std::wstring& href) {
    std::wstring url;
    size_t start = 0;
    while (start < href.size() && href[start] <= L' ') ++start;
    for (size_t i = start; i < href.size(); ++i) if (href[i] != L'\t' && href[i] != L'\r' && href[i] != L'\n') url += href[i];
    size_t end = 0;
    while (end < url.size() && (iswalnum(url[end]) || url[end] == L'+' || url[end] == L'.' || url[end] == L'-')) ++end;
    if (end < 2 || end >= url.size() || url[end] != L':' || !iswalpha(url[0])) return L"";
    std::wstring scheme = url.substr(0, end);
    for (auto& c : scheme) c = static_cast<wchar_t>(towlower(c));
    return scheme;
}

// The virtual host the reader loads the document's own folder through (pictures by relative path).
inline constexpr const wchar_t* kDocumentHost = L"npp-reader-document.local";
inline constexpr const char* kDocumentHostUrlUtf8 = "https://npp-reader-document.local/";

// Links a shared HTML file may keep: relative paths and anchors, web and mail.
inline bool IsSafeLinkScheme(const std::wstring& scheme) {
    return scheme.empty() || scheme == L"http" || scheme == L"https" || scheme == L"mailto";
}

// Image sources a shared HTML file may keep: the above plus inline data images.
inline bool IsSafeImageScheme(const std::wstring& scheme) {
    return scheme.empty() || scheme == L"http" || scheme == L"https" || scheme == L"data";
}
