#include "../include/MarkdownParser.h"
#include "../include/BiDiEngine.h"
#include <sstream>
#include <cwctype>
#include <algorithm>

bool MarkdownParser::IsHorizontalRule(const std::wstring& line) {
    size_t count = 0;
    wchar_t hrChar = 0;
    for (wchar_t ch : line) {
        if (ch == L' ' || ch == L'\t') continue;
        if (ch == L'-' || ch == L'*' || ch == L'_') {
            if (hrChar == 0) hrChar = ch;
            else if (hrChar != ch) return false;
            count++;
        } else {
            return false;
        }
    }
    return count >= 3;
}

bool MarkdownParser::ParseHeading(const std::wstring& line, MarkdownBlockType& type, std::wstring& content, int& level) {
    size_t i = 0;
    while (i < line.size() && (line[i] == L' ' || line[i] == L'\t')) i++;

    size_t hashCount = 0;
    while (i + hashCount < line.size() && line[i + hashCount] == L'#') hashCount++;

    if (hashCount >= 1 && hashCount <= 6) {
        size_t afterHash = i + hashCount;
        if (afterHash < line.size() && (line[afterHash] == L' ' || line[afterHash] == L'\t')) {
            while (afterHash < line.size() && (line[afterHash] == L' ' || line[afterHash] == L'\t')) afterHash++;
            content = line.substr(afterHash);
            level = static_cast<int>(hashCount);
            switch (hashCount) {
                case 1: type = MarkdownBlockType::Header1; break;
                case 2: type = MarkdownBlockType::Header2; break;
                case 3: type = MarkdownBlockType::Header3; break;
                case 4: type = MarkdownBlockType::Header4; break;
                case 5: type = MarkdownBlockType::Header5; break;
                default: type = MarkdownBlockType::Header6; break;
            }
            return true;
        }
    }
    return false;
}

bool MarkdownParser::ParseListItem(const std::wstring& line, MarkdownBlockType& type, std::wstring& content, int& index, int& nestLevel, bool& isChecked) {
    size_t i = 0;
    size_t indentSpaces = 0;
    while (i < line.size() && (line[i] == L' ' || line[i] == L'\t')) {
        indentSpaces += (line[i] == L'\t') ? 4 : 1;
        i++;
    }
    if (i >= line.size()) return false;

    nestLevel = static_cast<int>(indentSpaces / 2);

    // Unordered or Task List: -, *, +
    if ((line[i] == L'-' || line[i] == L'*' || line[i] == L'+') && (i + 1 < line.size()) && (line[i + 1] == L' ' || line[i + 1] == L'\t')) {
        size_t start = i + 2;
        while (start < line.size() && (line[start] == L' ' || line[start] == L'\t')) start++;

        // Check for task list: [ ] or [x] or [X]
        if (start + 3 <= line.size() && line[start] == L'[' && (line[start + 1] == L' ' || line[start + 1] == L'x' || line[start + 1] == L'X') && line[start + 2] == L']') {
            type = MarkdownBlockType::TaskListItem;
            isChecked = (line[start + 1] == L'x' || line[start + 1] == L'X');
            start += 3;
            while (start < line.size() && (line[start] == L' ' || line[start] == L'\t')) start++;
            content = line.substr(start);
            return true;
        }

        type = MarkdownBlockType::UnorderedListItem;
        content = line.substr(start);
        index = 0;
        return true;
    }

    // Ordered list: 1. or 1) or Persian ۱. or ۱)
    int parsedNum = 0;
    size_t numLen = 0;
    if (BiDiEngine::ParseNumber(line, i, parsedNum, numLen)) {
        size_t afterNum = i + numLen;
        if (afterNum < line.size() && (line[afterNum] == L'.' || line[afterNum] == L')' || line[afterNum] == L'-') && (afterNum + 1 < line.size()) && (line[afterNum + 1] == L' ' || line[afterNum + 1] == L'\t')) {
            type = MarkdownBlockType::OrderedListItem;
            index = parsedNum;
            size_t start = afterNum + 2;
            while (start < line.size() && (line[start] == L' ' || line[start] == L'\t')) start++;

            // Task list can also be ordered e.g. 1. [ ] Task
            if (start + 3 <= line.size() && line[start] == L'[' && (line[start + 1] == L' ' || line[start + 1] == L'x' || line[start + 1] == L'X') && line[start + 2] == L']') {
                type = MarkdownBlockType::TaskListItem;
                isChecked = (line[start + 1] == L'x' || line[start + 1] == L'X');
                start += 3;
                while (start < line.size() && (line[start] == L' ' || line[start] == L'\t')) start++;
            }

            content = line.substr(start);
            return true;
        }
    }

    return false;
}

bool MarkdownParser::ParseBlockquote(const std::wstring& line, std::wstring& content, int& nestLevel) {
    size_t i = 0;
    nestLevel = 0;
    while (i < line.size()) {
        while (i < line.size() && (line[i] == L' ' || line[i] == L'\t')) i++;
        if (i < line.size() && line[i] == L'>') {
            nestLevel++;
            i++;
        } else {
            break;
        }
    }

    if (nestLevel > 0) {
        if (i < line.size() && (line[i] == L' ' || line[i] == L'\t')) i++;
        content = (i < line.size()) ? line.substr(i) : L"";
        return true;
    }
    return false;
}

std::vector<std::wstring> MarkdownParser::SplitTableRow(const std::wstring& line) {
    std::vector<std::wstring> cells;
    std::wstring current;
    bool inPipe = false;
    size_t start = 0;
    while (start < line.size() && (line[start] == L' ' || line[start] == L'\t')) start++;
    if (start < line.size() && line[start] == L'|') start++;

    size_t end = line.size();
    while (end > start && (line[end - 1] == L' ' || line[end - 1] == L'\t')) end--;
    if (end > start && line[end - 1] == L'|') end--;

    for (size_t i = start; i < end; ++i) {
        if (line[i] == L'\\') {
            if (i + 1 < end) {
                current.push_back(line[i + 1]);
                i++;
                continue;
            }
        }
        if (line[i] == L'|') {
            // Trim current
            while (!current.empty() && (current.front() == L' ' || current.front() == L'\t')) current.erase(0, 1);
            while (!current.empty() && (current.back() == L' ' || current.back() == L'\t')) current.pop_back();
            cells.push_back(current);
            current.clear();
        } else {
            current.push_back(line[i]);
        }
    }
    while (!current.empty() && (current.front() == L' ' || current.front() == L'\t')) current.erase(0, 1);
    while (!current.empty() && (current.back() == L' ' || current.back() == L'\t')) current.pop_back();
    cells.push_back(current);

    return cells;
}

bool MarkdownParser::IsTableDividerLine(const std::wstring& line, std::vector<TableColumnAlign>& aligns) {
    auto parts = SplitTableRow(line);
    if (parts.empty()) return false;

    std::vector<TableColumnAlign> parsedAligns;
    for (const auto& part : parts) {
        if (part.empty()) return false;
        bool leftColon = (part.front() == L':');
        bool rightColon = (part.back() == L':');

        size_t dashCount = 0;
        for (wchar_t ch : part) {
            if (ch == L'-') dashCount++;
            else if (ch == L':') continue;
            else return false; // Invalid char in divider
        }
        if (dashCount < 1) return false;

        if (leftColon && rightColon) {
            parsedAligns.push_back(TableColumnAlign::Center);
        } else if (rightColon) {
            parsedAligns.push_back(TableColumnAlign::Right);
        } else {
            parsedAligns.push_back(TableColumnAlign::Left);
        }
    }

    aligns = std::move(parsedAligns);
    return true;
}

std::vector<MarkdownSpan> MarkdownParser::ParseInlines(const std::wstring& text) {
    std::vector<MarkdownSpan> spans;
    if (text.empty()) return spans;

    size_t i = 0;
    std::wstring currentText;

    auto flushText = [&]() {
        if (!currentText.empty()) {
            MarkdownSpan span;
            span.type = InlineStyleType::Normal;
            span.text = currentText;
            spans.push_back(span);
            currentText.clear();
        }
    };

    while (i < text.size()) {
        // 1. Escaped character
        if (text[i] == L'\\' && i + 1 < text.size()) {
            currentText.push_back(text[i + 1]);
            i += 2;
            continue;
        }

        // 2. Inline code: `code`
        if (text[i] == L'`') {
            size_t end = text.find(L'`', i + 1);
            if (end != std::wstring::npos) {
                flushText();
                MarkdownSpan span;
                span.type = InlineStyleType::InlineCode;
                span.text = text.substr(i + 1, end - i - 1);
                spans.push_back(span);
                i = end + 1;
                continue;
            }
        }

        // 3. Bold / Italic: ***, **, *, ___, __, _
        if (text[i] == L'*' || text[i] == L'_') {
            wchar_t marker = text[i];
            size_t count = 0;
            while (i + count < text.size() && text[i + count] == marker) count++;

            if (count == 3) {
                std::wstring closer(3, marker);
                size_t end = text.find(closer, i + 3);
                if (end != std::wstring::npos) {
                    flushText();
                    MarkdownSpan span;
                    span.type = InlineStyleType::BoldItalic;
                    span.text = text.substr(i + 3, end - i - 3);
                    spans.push_back(span);
                    i = end + 3;
                    continue;
                }
            } else if (count == 2) {
                std::wstring closer(2, marker);
                size_t end = text.find(closer, i + 2);
                if (end != std::wstring::npos) {
                    flushText();
                    MarkdownSpan span;
                    span.type = InlineStyleType::Bold;
                    span.text = text.substr(i + 2, end - i - 2);
                    spans.push_back(span);
                    i = end + 2;
                    continue;
                }
            } else if (count == 1) {
                size_t end = text.find(marker, i + 1);
                if (end != std::wstring::npos) {
                    flushText();
                    MarkdownSpan span;
                    span.type = InlineStyleType::Italic;
                    span.text = text.substr(i + 1, end - i - 1);
                    spans.push_back(span);
                    i = end + 1;
                    continue;
                }
            }
        }

        // 4. Strikethrough: ~~text~~
        if (text[i] == L'~' && i + 1 < text.size() && text[i + 1] == L'~') {
            size_t end = text.find(L"~~", i + 2);
            if (end != std::wstring::npos) {
                flushText();
                MarkdownSpan span;
                span.type = InlineStyleType::Strikethrough;
                span.text = text.substr(i + 2, end - i - 2);
                spans.push_back(span);
                i = end + 2;
                continue;
            }
        }

        // 4b. Highlight: ==text==
        if (text[i] == L'=' && i + 1 < text.size() && text[i + 1] == L'=') {
            size_t end = text.find(L"==", i + 2);
            if (end != std::wstring::npos) {
                flushText();
                MarkdownSpan span;
                span.type = InlineStyleType::Highlight;
                span.text = text.substr(i + 2, end - i - 2);
                spans.push_back(span);
                i = end + 2;
                continue;
            }
        }

        // 4c. Inline Math: $math$
        if (text[i] == L'$' && i + 1 < text.size() && text[i + 1] != L'$' && (i == 0 || text[i - 1] != L'\\')) {
            size_t end = text.find(L'$', i + 1);
            if (end != std::wstring::npos && end > i + 1) {
                flushText();
                MarkdownSpan span;
                span.type = InlineStyleType::InlineMath;
                span.text = text.substr(i + 1, end - i - 1);
                spans.push_back(span);
                i = end + 1;
                continue;
            }
        }

        // 5. Image: ![alt](url)
        if (text[i] == L'!' && i + 1 < text.size() && text[i + 1] == L'[') {
            size_t closeBracket = text.find(L']', i + 2);
            if (closeBracket != std::wstring::npos && closeBracket + 1 < text.size() && text[closeBracket + 1] == L'(') {
                size_t closeParen = text.find(L')', closeBracket + 2);
                if (closeParen != std::wstring::npos) {
                    flushText();
                    MarkdownSpan span;
                    span.type = InlineStyleType::Image;
                    span.text = text.substr(i + 2, closeBracket - i - 2);
                    span.extra = text.substr(closeBracket + 2, closeParen - closeBracket - 2);
                    spans.push_back(span);
                    i = closeParen + 1;
                    continue;
                }
            }
        }

        // 6. Link: [text](url)
        if (text[i] == L'[') {
            size_t closeBracket = text.find(L']', i + 1);
            if (closeBracket != std::wstring::npos && closeBracket + 1 < text.size() && text[closeBracket + 1] == L'(') {
                size_t closeParen = text.find(L')', closeBracket + 2);
                if (closeParen != std::wstring::npos) {
                    flushText();
                    MarkdownSpan span;
                    span.type = InlineStyleType::Link;
                    span.text = text.substr(i + 1, closeBracket - i - 1);
                    span.extra = text.substr(closeBracket + 2, closeParen - closeBracket - 2);
                    spans.push_back(span);
                    i = closeParen + 1;
                    continue;
                }
            }
        }

        // Default character
        currentText.push_back(text[i]);
        i++;
    }

    flushText();
    return spans;
}

MarkdownDocument MarkdownParser::Parse(const std::wstring& markdownText) {
    MarkdownDocument doc;
    if (markdownText.empty()) return doc;

    std::vector<std::wstring> lines;
    {
        std::wstringstream ss(markdownText);
        std::wstring line;
        while (std::getline(ss, line)) {
            if (!line.empty() && line.back() == L'\r') line.pop_back();
            lines.push_back(line);
        }
    }

    size_t lineIdx = 0;
    while (lineIdx < lines.size()) {
        const std::wstring& line = lines[lineIdx];

        // Empty line
        bool isEmpty = true;
        for (wchar_t ch : line) {
            if (ch != L' ' && ch != L'\t') { isEmpty = false; break; }
        }
        if (isEmpty) {
            lineIdx++;
            continue;
        }

        // 1. Fenced Code Block: ```lang
        size_t trimmedStart = 0;
        while (trimmedStart < line.size() && (line[trimmedStart] == L' ' || line[trimmedStart] == L'\t')) trimmedStart++;

        if (trimmedStart + 3 <= line.size() && line.substr(trimmedStart, 3) == L"```") {
            std::wstring lang = line.substr(trimmedStart + 3);
            while (!lang.empty() && (lang.front() == L' ' || lang.front() == L'\t')) lang.erase(0, 1);
            while (!lang.empty() && (lang.back() == L' ' || lang.back() == L'\t')) lang.pop_back();

            MarkdownBlock block;
            block.type = MarkdownBlockType::CodeBlock;
            block.codeLanguage = lang;
            block.sourceLineStart = static_cast<int>(lineIdx);
            block.isRTL = false; // Code blocks are always LTR

            lineIdx++;
            while (lineIdx < lines.size()) {
                const std::wstring& codeLine = lines[lineIdx];
                size_t cTrimmed = 0;
                while (cTrimmed < codeLine.size() && (codeLine[cTrimmed] == L' ' || codeLine[cTrimmed] == L'\t')) cTrimmed++;
                if (cTrimmed + 3 <= codeLine.size() && codeLine.substr(cTrimmed, 3) == L"```") {
                    break;
                }
                block.codeLines.push_back(codeLine);
                lineIdx++;
            }
            block.sourceLineEnd = static_cast<int>(lineIdx);
            doc.blocks.push_back(block);
            lineIdx++;
            continue;
        }

        // 2. Table: check if current line has pipes and next line is divider
        if (lineIdx + 1 < lines.size() && line.find(L'|') != std::wstring::npos) {
            std::vector<TableColumnAlign> aligns;
            if (IsTableDividerLine(lines[lineIdx + 1], aligns)) {
                MarkdownBlock tableBlock;
                tableBlock.type = MarkdownBlockType::Table;
                tableBlock.sourceLineStart = static_cast<int>(lineIdx);
                tableBlock.table.alignments = std::move(aligns);

                // Header row
                TableRow headerRow;
                headerRow.isHeader = true;
                auto headerParts = SplitTableRow(line);
                for (const auto& hPart : headerParts) {
                    TableCell cell;
                    cell.text = hPart;
                    cell.spans = ParseInlines(hPart);
                    headerRow.cells.push_back(cell);
                }
                tableBlock.table.rows.push_back(headerRow);

                lineIdx += 2; // Skip header and divider

                // Data rows
                while (lineIdx < lines.size()) {
                    const std::wstring& rowLine = lines[lineIdx];
                    if (rowLine.empty() || rowLine.find(L'|') == std::wstring::npos) break;
                    auto rowParts = SplitTableRow(rowLine);
                    if (rowParts.empty()) break;

                    TableRow row;
                    row.isHeader = false;
                    for (const auto& rPart : rowParts) {
                        TableCell cell;
                        cell.text = rPart;
                        cell.spans = ParseInlines(rPart);
                        row.cells.push_back(cell);
                    }
                    tableBlock.table.rows.push_back(row);
                    lineIdx++;
                }
                tableBlock.sourceLineEnd = static_cast<int>(lineIdx - 1);
                tableBlock.isRTL = BiDiEngine::IsParagraphRTL(line);
                doc.blocks.push_back(tableBlock);
                continue;
            }
        }

        // 3. Heading: # to ######
        MarkdownBlockType hType;
        std::wstring hContent;
        int hLevel = 1;
        if (ParseHeading(line, hType, hContent, hLevel)) {
            MarkdownBlock block;
            block.type = hType;
            block.level = hLevel;
            block.rawText = hContent;
            block.inlines = ParseInlines(hContent);
            block.sourceLineStart = static_cast<int>(lineIdx);
            block.sourceLineEnd = static_cast<int>(lineIdx);
            block.isRTL = BiDiEngine::IsParagraphRTL(hContent);
            doc.blocks.push_back(block);
            doc.headingCount++;
            lineIdx++;
            continue;
        }

        // 4. Horizontal Rule: ---, ***, ___
        if (IsHorizontalRule(line)) {
            MarkdownBlock block;
            block.type = MarkdownBlockType::HorizontalRule;
            block.sourceLineStart = static_cast<int>(lineIdx);
            block.sourceLineEnd = static_cast<int>(lineIdx);
            doc.blocks.push_back(block);
            lineIdx++;
            continue;
        }

        // 5. Blockquote: >
        std::wstring bqContent;
        int bqNest = 0;
        if (ParseBlockquote(line, bqContent, bqNest)) {
            MarkdownBlock block;
            block.type = MarkdownBlockType::Blockquote;
            block.level = bqNest;
            block.rawText = bqContent;
            block.sourceLineStart = static_cast<int>(lineIdx);

            lineIdx++;
            while (lineIdx < lines.size()) {
                std::wstring nextBq;
                int nextNest = 0;
                if (ParseBlockquote(lines[lineIdx], nextBq, nextNest) && nextNest == bqNest) {
                    block.rawText += L" " + nextBq;
                    lineIdx++;
                } else {
                    break;
                }
            }
            block.sourceLineEnd = static_cast<int>(lineIdx - 1);

            // Detect GitHub / Persian Alert Callouts: [!NOTE], [!TIP], [!نکته], etc.
            std::wstring trimmedBq = block.rawText;
            while (!trimmedBq.empty() && (trimmedBq.front() == L' ' || trimmedBq.front() == L'\t')) trimmedBq.erase(0, 1);

            auto startsWithNoCase = [](const std::wstring& str, const std::wstring& prefix) {
                if (str.size() < prefix.size()) return false;
                for (size_t k = 0; k < prefix.size(); ++k) {
                    if (::towlower(str[k]) != ::towlower(prefix[k])) return false;
                }
                return true;
            };

            if (startsWithNoCase(trimmedBq, L"[!NOTE]") || startsWithNoCase(trimmedBq, L"[!نکته]") || startsWithNoCase(trimmedBq, L"[!یادداشت]")) {
                block.type = MarkdownBlockType::AlertCallout;
                block.alertType = AlertType::Note;
                block.alertTitle = startsWithNoCase(trimmedBq, L"[!NOTE]") ? L"Note" : L"نکته";
                size_t close = trimmedBq.find(L']');
                block.rawText = (close != std::wstring::npos && close + 1 < trimmedBq.size()) ? trimmedBq.substr(close + 1) : L"";
            } else if (startsWithNoCase(trimmedBq, L"[!TIP]") || startsWithNoCase(trimmedBq, L"[!راهنما]") || startsWithNoCase(trimmedBq, L"[!ترفند]")) {
                block.type = MarkdownBlockType::AlertCallout;
                block.alertType = AlertType::Tip;
                block.alertTitle = startsWithNoCase(trimmedBq, L"[!TIP]") ? L"Tip" : L"ترفند";
                size_t close = trimmedBq.find(L']');
                block.rawText = (close != std::wstring::npos && close + 1 < trimmedBq.size()) ? trimmedBq.substr(close + 1) : L"";
            } else if (startsWithNoCase(trimmedBq, L"[!IMPORTANT]") || startsWithNoCase(trimmedBq, L"[!مهم]")) {
                block.type = MarkdownBlockType::AlertCallout;
                block.alertType = AlertType::Important;
                block.alertTitle = startsWithNoCase(trimmedBq, L"[!IMPORTANT]") ? L"Important" : L"مهم";
                size_t close = trimmedBq.find(L']');
                block.rawText = (close != std::wstring::npos && close + 1 < trimmedBq.size()) ? trimmedBq.substr(close + 1) : L"";
            } else if (startsWithNoCase(trimmedBq, L"[!WARNING]") || startsWithNoCase(trimmedBq, L"[!هشدار]")) {
                block.type = MarkdownBlockType::AlertCallout;
                block.alertType = AlertType::Warning;
                block.alertTitle = startsWithNoCase(trimmedBq, L"[!WARNING]") ? L"Warning" : L"هشدار";
                size_t close = trimmedBq.find(L']');
                block.rawText = (close != std::wstring::npos && close + 1 < trimmedBq.size()) ? trimmedBq.substr(close + 1) : L"";
            } else if (startsWithNoCase(trimmedBq, L"[!CAUTION]") || startsWithNoCase(trimmedBq, L"[!احتیاط]") || startsWithNoCase(trimmedBq, L"[!خطر]")) {
                block.type = MarkdownBlockType::AlertCallout;
                block.alertType = AlertType::Caution;
                block.alertTitle = startsWithNoCase(trimmedBq, L"[!CAUTION]") ? L"Caution" : L"احتیاط";
                size_t close = trimmedBq.find(L']');
                block.rawText = (close != std::wstring::npos && close + 1 < trimmedBq.size()) ? trimmedBq.substr(close + 1) : L"";
            }

            while (!block.rawText.empty() && (block.rawText.front() == L' ' || block.rawText.front() == L'\t')) block.rawText.erase(0, 1);

            block.inlines = ParseInlines(block.rawText);
            block.isRTL = BiDiEngine::IsParagraphRTL(block.rawText);
            doc.blocks.push_back(block);
            continue;
        }

        // 6. List Item: Ordered, Unordered, Task
        MarkdownBlockType listType;
        std::wstring listContent;
        int listIndex = 1;
        int listNest = 0;
        bool isTaskChecked = false;
        if (ParseListItem(line, listType, listContent, listIndex, listNest, isTaskChecked)) {
            MarkdownBlock block;
            block.type = listType;
            block.level = listNest;
            block.listIndex = listIndex;
            block.isTaskChecked = isTaskChecked;
            block.rawText = listContent;
            block.inlines = ParseInlines(listContent);
            block.sourceLineStart = static_cast<int>(lineIdx);
            block.sourceLineEnd = static_cast<int>(lineIdx);
            block.isRTL = BiDiEngine::IsParagraphRTL(listContent);
            doc.blocks.push_back(block);
            lineIdx++;
            continue;
        }

        // 7. Regular Paragraph (can span multiple lines until empty line or block)
        MarkdownBlock pBlock;
        pBlock.type = MarkdownBlockType::Paragraph;
        pBlock.sourceLineStart = static_cast<int>(lineIdx);
        pBlock.rawText = line;

        lineIdx++;
        while (lineIdx < lines.size()) {
            const std::wstring& nextLine = lines[lineIdx];
            bool nextEmpty = true;
            for (wchar_t ch : nextLine) {
                if (ch != L' ' && ch != L'\t') { nextEmpty = false; break; }
            }
            if (nextEmpty) break;

            // Check if next line is a new block boundary
            MarkdownBlockType dummyType;
            std::wstring dummyContent;
            int dummyInt = 0;
            bool dummyBool = false;
            if (IsHorizontalRule(nextLine) ||
                ParseHeading(nextLine, dummyType, dummyContent, dummyInt) ||
                (nextLine.size() >= 3 && nextLine.substr(0, 3) == L"```") ||
                ParseListItem(nextLine, dummyType, dummyContent, dummyInt, dummyInt, dummyBool) ||
                ParseBlockquote(nextLine, dummyContent, dummyInt)) {
                break;
            }

            pBlock.rawText += L" " + nextLine;
            lineIdx++;
        }
        pBlock.sourceLineEnd = static_cast<int>(lineIdx - 1);
        pBlock.inlines = ParseInlines(pBlock.rawText);
        pBlock.isRTL = BiDiEngine::IsParagraphRTL(pBlock.rawText);
        doc.blocks.push_back(pBlock);
    }

    return doc;
}
