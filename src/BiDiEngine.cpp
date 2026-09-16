#include "../include/BiDiEngine.h"
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>

bool BiDiEngine::IsRTLChar(wchar_t ch) noexcept {
    // Arabic, Persian, Urdu, Hebrew Unicode blocks
    if (ch >= 0x0600 && ch <= 0x06FF) return true; // Arabic & Persian
    if (ch >= 0x0750 && ch <= 0x077F) return true; // Arabic Supplement
    if (ch >= 0x08A0 && ch <= 0x08FF) return true; // Arabic Extended-A
    if (ch >= 0xFB50 && ch <= 0xFDFF) return true; // Arabic Presentation Forms-A
    if (ch >= 0xFE70 && ch <= 0xFEFF) return true; // Arabic Presentation Forms-B
    if (ch >= 0x0590 && ch <= 0x05FF) return true; // Hebrew
    return false;
}

bool BiDiEngine::IsLTRChar(wchar_t ch) noexcept {
    if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z')) return true;
    if (ch >= 0x00C0 && ch <= 0x024F) return true; // Latin Supplement & Extended
    if (ch >= 0x0370 && ch <= 0x052F) return true; // Greek & Cyrillic
    return false;
}

bool BiDiEngine::IsStrongChar(wchar_t ch) noexcept {
    return IsRTLChar(ch) || IsLTRChar(ch);
}

bool BiDiEngine::IsNeutralChar(wchar_t ch) noexcept {
    if (ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n') return true;
    if (ch == L'.' || ch == L',' || ch == L'!' || ch == L'?' || ch == L':' || ch == L';') return true;
    if (ch == L'(' || ch == L')' || ch == L'[' || ch == L']' || ch == L'{' || ch == L'}') return true;
    if (ch == L'<' || ch == L'>' || ch == L'/' || ch == L'\\' || ch == L'|' || ch == L'-' || ch == L'_') return true;
    if (ch == L'+' || ch == L'=' || ch == L'*' || ch == L'%' || ch == L'&' || ch == L'^' || ch == L'$' || ch == L'#' || ch == L'@' || ch == L'~') return true;
    if (ch == L'\'' || ch == L'\"' || ch == L'`' || ch == L'«' || ch == L'»' || ch == L'،' || ch == L'؛' || ch == L'؟') return true;
    return false;
}

bool BiDiEngine::IsPersianDigit(wchar_t ch) noexcept {
    return (ch >= 0x06F0 && ch <= 0x06F9);
}

bool BiDiEngine::IsArabicDigit(wchar_t ch) noexcept {
    return (ch >= 0x0660 && ch <= 0x0669);
}

int BiDiEngine::DigitToInt(wchar_t ch) noexcept {
    if (ch >= L'0' && ch <= L'9') return ch - L'0';
    if (ch >= 0x06F0 && ch <= 0x06F9) return ch - 0x06F0; // Persian digits ۰-۹
    if (ch >= 0x0660 && ch <= 0x0669) return ch - 0x0660; // Arabic digits ٠-٩
    return -1;
}

std::wstring BiDiEngine::ToPersianDigits(int number) {
    if (number == 0) return L"۰";
    bool negative = number < 0;
    long long n = (number < 0) ? -static_cast<long long>(number) : static_cast<long long>(number);
    std::wstring result;
    while (n > 0) {
        wchar_t pDigit = static_cast<wchar_t>(0x06F0 + (n % 10));
        result.push_back(pDigit);
        n /= 10;
    }
    if (negative) result.push_back(L'-');
    std::reverse(result.begin(), result.end());
    return result;
}

std::wstring BiDiEngine::ToPersianDigits(const std::wstring& input) {
    std::wstring result = input;
    for (wchar_t& ch : result) {
        if (ch >= L'0' && ch <= L'9') {
            ch = static_cast<wchar_t>(0x06F0 + (ch - L'0'));
        }
    }
    return result;
}

bool BiDiEngine::ParseNumber(const std::wstring& text, size_t startPos, int& outNumber, size_t& outCharsConsumed) {
    outNumber = 0;
    outCharsConsumed = 0;
    size_t i = startPos;
    while (i < text.size()) {
        int val = DigitToInt(text[i]);
        if (val < 0) break;
        outNumber = outNumber * 10 + val;
        i++;
    }
    outCharsConsumed = i - startPos;
    return outCharsConsumed > 0;
}

TextDirection BiDiEngine::DetectFirstStrongDirection(const std::wstring& text) noexcept {
    for (wchar_t ch : text) {
        if (IsRTLChar(ch)) return TextDirection::RTL;
        if (IsLTRChar(ch)) return TextDirection::LTR;
    }
    return TextDirection::Neutral;
}

TextDirection BiDiEngine::DetectDominantDirection(const std::wstring& text) noexcept {
    size_t rtlCount = 0;
    size_t ltrCount = 0;
    for (wchar_t ch : text) {
        if (IsRTLChar(ch)) rtlCount++;
        else if (IsLTRChar(ch)) ltrCount++;
    }
    if (rtlCount > ltrCount) return TextDirection::RTL;
    if (ltrCount > rtlCount) return TextDirection::LTR;
    return DetectFirstStrongDirection(text);
}

bool BiDiEngine::IsParagraphRTL(const std::wstring& paragraph) noexcept {
    return DetectFirstStrongDirection(paragraph) == TextDirection::RTL;
}

std::wstring BiDiEngine::IsolateInlineBiDi(const std::wstring& text, bool isParentRTL) {
    if (!isParentRTL) return text;

    // In RTL parent, isolate embedded LTR inline runs with Left-To-Right Isolate (U+2066) and Pop Directional Isolate (U+2069)
    // or Left-to-Right Mark (U+200E) at the boundary
    std::wstring result;
    result.reserve(text.size() + 8);
    result.push_back(0x2066); // LRI
    result.append(text);
    result.push_back(0x2069); // PDI
    return result;
}

std::wstring BiDiEngine::FixMixedPunctuation(const std::wstring& text) {
    if (text.empty()) return text;
    bool isRTL = IsParagraphRTL(text);
    if (!isRTL) return text;

    std::wstring result = text;
    // Check if ends with trailing punctuation that needs Right-to-Left Mark (U+200F)
    wchar_t last = result.back();
    if (last == L'.' || last == L'!' || last == L'?' || last == L':' || last == L';' || last == L')' || last == L']') {
        result.push_back(0x200F); // RLM ensures proper end-of-sentence position
    }
    return result;
}

std::wstring BiDiEngine::NormalizePersianGlyphs(const std::wstring& input) {
    std::wstring result;
    result.reserve(input.size());

    for (wchar_t ch : input) {
        switch (ch) {
            case 0x064A: // Arabic Yeh (ي)
            case 0x0649: // Alef Maksura (ى)
                result.push_back(0x06CC); // Persian Yeh (ی)
                break;
            case 0x0643: // Arabic Kaf (ك)
                result.push_back(0x06A9); // Persian Kaf (ک)
                break;
            case 0x06C0: // Heh with Yeh above (ۀ) -> ه + ZWNJ + ی
                result.push_back(0x0647);
                result.push_back(0x200C);
                result.push_back(0x06CC);
                break;
            default:
                result.push_back(ch);
                break;
        }
    }
    return result;
}

std::wstring BiDiEngine::Utf8ToWide(std::string_view utf8Str) {
    if (utf8Str.empty()) return {};
    int count = MultiByteToWideChar(CP_UTF8, 0, utf8Str.data(), static_cast<int>(utf8Str.size()), nullptr, 0);
    if (count <= 0) return {};
    std::wstring result(count, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.data(), static_cast<int>(utf8Str.size()), &result[0], count);
    return result;
}

std::string BiDiEngine::WideToUtf8(std::wstring_view wideStr) {
    if (wideStr.empty()) return {};
    int count = WideCharToMultiByte(CP_UTF8, 0, wideStr.data(), static_cast<int>(wideStr.size()), nullptr, 0, nullptr, nullptr);
    if (count <= 0) return {};
    std::string result(count, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.data(), static_cast<int>(wideStr.size()), &result[0], count, nullptr, nullptr);
    return result;
}
