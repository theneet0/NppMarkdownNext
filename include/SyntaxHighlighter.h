#pragma once
#include <string>
#include <vector>
#include <cstdint>

enum class HighlightTokenType {
    Default,
    Keyword,
    Type,
    String,
    Comment,
    Number,
    Preprocessor,
    Operator
};

struct HighlightToken {
    HighlightTokenType type = HighlightTokenType::Default;
    size_t start = 0;
    size_t length = 0;
};

class SyntaxHighlighter {
public:
    static std::vector<HighlightToken> Tokenize(const std::wstring& line, const std::wstring& language);
};
