#pragma once
#include <iterator>
#include <set>
#include <string>
#include <string_view>
#include "HeadingPolicy.inc"

inline std::set<std::wstring> ReservedReaderHeadingIds() {
    return std::set<std::wstring>(std::begin(kReservedHeadingIds), std::end(kReservedHeadingIds));
}

inline std::wstring MakeHeadingId(std::wstring_view text, std::set<std::wstring>& usedIds) {
    std::wstring base;
    bool separator = false;
    auto append = [&](unsigned codepoint) {
        for (const auto& range : kHeadingWhitespace) {
            if (codepoint >= range.first && codepoint <= range.last) { separator = !base.empty(); return; }
        }
        size_t left = 0, right = std::size(kHeadingCodepoints);
        while (left < right) {
            size_t middle = (left + right) / 2;
            if (kHeadingCodepoints[middle].last < codepoint) left = middle + 1;
            else right = middle;
        }
        if (left == std::size(kHeadingCodepoints) || kHeadingCodepoints[left].first > codepoint) return;
        if (separator) base += L'-';
        separator = false;
        if (sizeof(wchar_t) == 2 && codepoint > 0xffff) {
            base += static_cast<wchar_t>(0xd800 + ((codepoint - 0x10000) >> 10));
            base += static_cast<wchar_t>(0xdc00 + ((codepoint - 0x10000) & 0x3ff));
        } else base += static_cast<wchar_t>(codepoint);
    };
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned codepoint = static_cast<unsigned>(text[i]);
        if (codepoint >= 0xd800 && codepoint <= 0xdbff && i + 1 < text.size() && text[i + 1] >= 0xdc00 && text[i + 1] <= 0xdfff) {
            codepoint = 0x10000 + ((codepoint - 0xd800) << 10) + (text[++i] - 0xdc00);
        }
        bool expanded = false;
        for (const auto& expansion : kHeadingExpansions) {
            if (expansion.codepoint == codepoint) { append(expansion.first); append(expansion.second); expanded = true; break; }
        }
        if (expanded) continue;
        size_t left = 0, right = std::size(kHeadingFolds);
        while (left < right) {
            size_t middle = (left + right) / 2;
            if (kHeadingFolds[middle].last < codepoint) left = middle + 1;
            else right = middle;
        }
        if (left < std::size(kHeadingFolds) && kHeadingFolds[left].first <= codepoint) codepoint += kHeadingFolds[left].delta;
        append(codepoint);
    }
    if (base.empty()) base = L"section";
    std::wstring id = base;
    for (unsigned suffix = 1; !usedIds.insert(id).second; ++suffix) id = base + L"-" + std::to_wstring(suffix);
    return id;
}
