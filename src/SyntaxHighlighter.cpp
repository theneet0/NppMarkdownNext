#include "../include/SyntaxHighlighter.h"
#include <unordered_set>
#include <cwctype>
#include <algorithm>

namespace {

const std::unordered_set<std::wstring> s_cppKeywords = {
    L"alignas", L"alignof", L"and", L"and_eq", L"asm", L"atomic_cancel", L"atomic_commit",
    L"atomic_noexcept", L"auto", L"bitand", L"bitor", L"bool", L"break", L"case", L"catch",
    L"class", L"compl", L"concept", L"const", L"consteval", L"constexpr", L"constinit",
    L"const_cast", L"continue", L"co_await", L"co_return", L"co_yield", L"decltype", L"default",
    L"delete", L"do", L"dynamic_cast", L"else", L"enum", L"explicit", L"export", L"extern",
    L"false", L"for", L"friend", L"goto", L"if", L"inline", L"import", L"module", L"mutable",
    L"namespace", L"new", L"noexcept", L"not", L"not_eq", L"nullptr", L"operator", L"or",
    L"or_eq", L"private", L"protected", L"public", L"reflexpr", L"register", L"reinterpret_cast",
    L"requires", L"return", L"sizeof", L"static", L"static_assert", L"static_cast", L"struct",
    L"switch", L"synchronized", L"template", L"this", L"thread_local", L"throw", L"true",
    L"try", L"typedef", L"typeid", L"typename", L"union", L"using", L"virtual", L"volatile",
    L"while", L"xor", L"xor_eq"
};

const std::unordered_set<std::wstring> s_cppTypes = {
    L"char", L"char8_t", L"char16_t", L"char32_t", L"wchar_t", L"short", L"int", L"long",
    L"signed", L"unsigned", L"float", L"double", L"void", L"size_t", L"int8_t", L"int16_t",
    L"int32_t", L"int64_t", L"uint8_t", L"uint16_t", L"uint32_t", L"uint64_t", L"intptr_t",
    L"uintptr_t", L"string", L"wstring", L"string_view", L"vector", L"map", L"unordered_map",
    L"set", L"unordered_set", L"unique_ptr", L"shared_ptr", L"weak_ptr", L"span", L"optional",
    L"variant", L"any"
};

const std::unordered_set<std::wstring> s_pythonKeywords = {
    L"and", L"as", L"assert", L"async", L"await", L"break", L"class", L"continue", L"def",
    L"del", L"elif", L"else", L"except", L"False", L"finally", L"for", L"from", L"global",
    L"if", L"import", L"in", L"is", L"lambda", L"None", L"nonlocal", L"not", L"or", L"pass",
    L"raise", L"return", L"True", L"try", L"while", L"with", L"yield", L"self"
};

const std::unordered_set<std::wstring> s_jsKeywords = {
    L"async", L"await", L"break", L"case", L"catch", L"class", L"const", L"continue",
    L"debugger", L"default", L"delete", L"do", L"else", L"export", L"extends", L"false",
    L"finally", L"for", L"function", L"if", L"import", L"in", L"instanceof", L"let", L"new",
    L"null", L"return", L"super", L"switch", L"this", L"throw", L"true", L"try", L"typeof",
    L"undefined", L"var", L"void", L"while", L"with", L"yield"
};

const std::unordered_set<std::wstring> s_bashKeywords = {
    L"sudo", L"chmod", L"chown", L"chgrp", L"systemctl", L"journalctl", L"service",
    L"echo", L"cat", L"grep", L"egrep", L"fgrep", L"sed", L"awk", L"find", L"xargs",
    L"mkdir", L"rmdir", L"rm", L"cp", L"mv", L"ln", L"touch", L"ls", L"cd", L"pwd",
    L"curl", L"wget", L"tar", L"gzip", L"gunzip", L"zip", L"unzip", L"ssh", L"scp",
    L"apt", L"apt-get", L"yum", L"dnf", L"pacman", L"apk", L"docker", L"git",
    L"export", L"source", L"alias", L"unalias", L"set", L"unset", L"read", L"local",
    L"if", L"fi", L"then", L"else", L"elif", L"for", L"in", L"do", L"done",
    L"case", L"esac", L"while", L"until", L"break", L"continue", L"exit", L"return",
    L"function", L"select", L"time", L"exec", L"eval", L"trap", L"wait", L"kill"
};

const std::unordered_set<std::wstring> s_yamlKeywords = {
    L"true", L"false", L"null", L"yes", L"no", L"on", L"off"
};

const std::unordered_set<std::wstring> s_sqlKeywords = {
    L"select", L"from", L"where", L"insert", L"update", L"delete", L"into", L"values",
    L"join", L"inner", L"outer", L"left", L"right", L"on", L"group", L"by", L"order",
    L"having", L"limit", L"create", L"table", L"drop", L"alter", L"add", L"and", L"or",
    L"not", L"in", L"is", L"null", L"as", L"distinct", L"case", L"when", L"then", L"else",
    L"end", L"primary", L"key", L"foreign", L"references", L"index"
};

} // namespace

std::vector<HighlightToken> SyntaxHighlighter::Tokenize(const std::wstring& line, const std::wstring& language) {
    std::vector<HighlightToken> tokens;
    if (line.empty()) return tokens;

    std::wstring lowerLang = language;
    std::transform(lowerLang.begin(), lowerLang.end(), lowerLang.begin(), ::towlower);

    bool isPy = (lowerLang == L"python" || lowerLang == L"py");
    bool isJs = (lowerLang == L"js" || lowerLang == L"javascript" || lowerLang == L"ts" || lowerLang == L"typescript");
    bool isSh = (lowerLang == L"bash" || lowerLang == L"sh" || lowerLang == L"shell" || lowerLang == L"powershell" || lowerLang == L"ps1" || lowerLang == L"zsh");
    bool isYaml = (lowerLang == L"yaml" || lowerLang == L"yml");
    bool isSql = (lowerLang == L"sql");

    size_t i = 0;
    while (i < line.size()) {
        wchar_t ch = line[i];

        // Whitespace
        if (std::iswspace(ch)) {
            i++;
            continue;
        }

        // Single line comments
        if ((ch == L'/' && i + 1 < line.size() && line[i + 1] == L'/') ||
            ((isPy || isSh || isYaml) && ch == L'#') ||
            (isSql && ch == L'-' && i + 1 < line.size() && line[i + 1] == L'-')) {
            HighlightToken token;
            token.type = HighlightTokenType::Comment;
            token.start = i;
            token.length = line.size() - i;
            tokens.push_back(token);
            break;
        }

        // Shell variables: $VAR or ${VAR}
        if (isSh && ch == L'$') {
            size_t start = i;
            i++;
            if (i < line.size() && line[i] == L'{') {
                while (i < line.size() && line[i] != L'}') i++;
                if (i < line.size() && line[i] == L'}') i++;
            } else {
                while (i < line.size() && (std::iswalnum(line[i]) || line[i] == L'_' || line[i] == L'?')) i++;
            }
            if (i > start + 1) {
                HighlightToken token;
                token.type = HighlightTokenType::Type;
                token.start = start;
                token.length = i - start;
                tokens.push_back(token);
                continue;
            }
        }

        // Preprocessor / directive (#include, etc.)
        if (!isPy && !isSh && !isYaml && ch == L'#' && i == 0) {
            HighlightToken token;
            token.type = HighlightTokenType::Preprocessor;
            token.start = i;
            size_t end = i + 1;
            while (end < line.size() && !std::iswspace(line[end])) end++;
            token.length = end - i;
            tokens.push_back(token);
            i = end;
            continue;
        }

        // Strings: "..." or '...' or `...`
        if (ch == L'\"' || ch == L'\'' || (isJs && ch == L'`')) {
            wchar_t quote = ch;
            size_t start = i;
            i++;
            while (i < line.size()) {
                if (line[i] == L'\\' && i + 1 < line.size()) {
                    i += 2;
                    continue;
                }
                if (line[i] == quote) {
                    i++;
                    break;
                }
                i++;
            }
            HighlightToken token;
            token.type = HighlightTokenType::String;
            token.start = start;
            token.length = i - start;
            tokens.push_back(token);
            continue;
        }

        // Numbers: 0x..., 123, 3.14
        if (std::iswdigit(ch)) {
            size_t start = i;
            while (i < line.size() && (std::iswxdigit(line[i]) || line[i] == L'.' || line[i] == L'x' || line[i] == L'X' || line[i] == L'f' || line[i] == L'u' || line[i] == L'l')) {
                i++;
            }
            HighlightToken token;
            token.type = HighlightTokenType::Number;
            token.start = start;
            token.length = i - start;
            tokens.push_back(token);
            continue;
        }

        // Identifiers: keywords, types, words
        if (std::iswalpha(ch) || ch == L'_') {
            size_t start = i;
            while (i < line.size() && (std::iswalnum(line[i]) || line[i] == L'_')) {
                i++;
            }
            std::wstring word = line.substr(start, i - start);

            HighlightToken token;
            token.start = start;
            token.length = i - start;

            bool isBoundedInPath = false;
            if (isSh || isYaml) {
                if (start > 0 && (line[start - 1] == L'/' || line[start - 1] == L'\\' || line[start - 1] == L'.' || line[start - 1] == L'-' || line[start - 1] == L':')) {
                    isBoundedInPath = true;
                }
                if (i < line.size() && (line[i] == L'/' || line[i] == L'\\' || line[i] == L'.' || line[i] == L'-' || line[i] == L':')) {
                    isBoundedInPath = true;
                }
            }

            if (isBoundedInPath) {
                token.type = HighlightTokenType::Default;
            } else if (isPy) {
                if (s_pythonKeywords.count(word)) token.type = HighlightTokenType::Keyword;
                else token.type = HighlightTokenType::Default;
            } else if (isJs) {
                if (s_jsKeywords.count(word)) token.type = HighlightTokenType::Keyword;
                else token.type = HighlightTokenType::Default;
            } else if (isSh) {
                std::wstring lowerWord = word;
                std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::towlower);
                if (s_bashKeywords.count(lowerWord)) token.type = HighlightTokenType::Keyword;
                else token.type = HighlightTokenType::Default;
            } else if (isYaml) {
                std::wstring lowerWord = word;
                std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::towlower);
                if (s_yamlKeywords.count(lowerWord)) token.type = HighlightTokenType::Keyword;
                else token.type = HighlightTokenType::Default;
            } else if (isSql) {
                std::wstring lowerWord = word;
                std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::towlower);
                if (s_sqlKeywords.count(lowerWord)) token.type = HighlightTokenType::Keyword;
                else token.type = HighlightTokenType::Default;
            } else {
                if (s_cppKeywords.count(word)) token.type = HighlightTokenType::Keyword;
                else if (s_cppTypes.count(word)) token.type = HighlightTokenType::Type;
                else token.type = HighlightTokenType::Default;
            }

            if (token.type != HighlightTokenType::Default) {
                tokens.push_back(token);
            }
            continue;
        }

        i++;
    }

    return tokens;
}
