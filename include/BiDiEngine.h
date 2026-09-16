#pragma once
#include <string>
#include <vector>

enum class TextDirection {
    LTR,
    RTL,
    Neutral
};

class BiDiEngine {
public:
    static bool IsRTLChar(wchar_t ch) noexcept;
    static bool IsLTRChar(wchar_t ch) noexcept;
    static bool IsStrongChar(wchar_t ch) noexcept;
    static bool IsNeutralChar(wchar_t ch) noexcept;
    static bool IsPersianDigit(wchar_t ch) noexcept;
    static bool IsArabicDigit(wchar_t ch) noexcept;
    static int DigitToInt(wchar_t ch) noexcept;
    static std::wstring ToPersianDigits(int number);
    static std::wstring ToPersianDigits(const std::wstring& input);
    static bool ParseNumber(const std::wstring& text, size_t startPos, int& outNumber, size_t& outCharsConsumed);

    static TextDirection DetectFirstStrongDirection(const std::wstring& text) noexcept;
    static TextDirection DetectDominantDirection(const std::wstring& text) noexcept;
    static bool IsParagraphRTL(const std::wstring& paragraph) noexcept;

    // Fix punctuation flip and wrap inline code snippets with BiDi isolation marks
    static std::wstring IsolateInlineBiDi(const std::wstring& text, bool isParentRTL);
    static std::wstring FixMixedPunctuation(const std::wstring& text);
    static std::wstring NormalizePersianGlyphs(const std::wstring& input);

    // UTF-8 <-> UTF-16 conversions
    static std::wstring Utf8ToWide(std::string_view utf8Str);
    static std::string WideToUtf8(std::wstring_view wideStr);
};
