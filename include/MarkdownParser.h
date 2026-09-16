#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <cstdint>

enum class MarkdownBlockType {
    Paragraph,
    Header1,
    Header2,
    Header3,
    Header4,
    Header5,
    Header6,
    UnorderedListItem,
    OrderedListItem,
    TaskListItem,
    Blockquote,
    AlertCallout,
    CodeBlock,
    Table,
    HorizontalRule
};

enum class AlertType {
    None,
    Note,       // [!NOTE], [!نکته], [!یادداشت]
    Tip,        // [!TIP], [!راهنما], [!ترفند]
    Important,  // [!IMPORTANT], [!مهم]
    Warning,    // [!WARNING], [!هشدار]
    Caution     // [!CAUTION], [!احتیاط], [!خطر]
};

enum class InlineStyleType {
    Normal,
    Bold,
    Italic,
    BoldItalic,
    Strikethrough,
    Underline,
    Highlight,
    InlineCode,
    Link,
    Image,
    InlineMath
};

struct MarkdownSpan {
    InlineStyleType type = InlineStyleType::Normal;
    std::wstring text;
    std::wstring extra; // URL for Link/Image
};

enum class TableColumnAlign {
    Left,
    Center,
    Right
};

struct TableCell {
    std::wstring text;
    std::vector<MarkdownSpan> spans;
};

struct TableRow {
    std::vector<TableCell> cells;
    bool isHeader = false;
};

struct MarkdownTable {
    std::vector<TableColumnAlign> alignments;
    std::vector<TableRow> rows;
};

struct MarkdownBlock {
    MarkdownBlockType type = MarkdownBlockType::Paragraph;
    std::wstring rawText;
    std::vector<MarkdownSpan> inlines;
    int sourceLineStart = 0;
    int sourceLineEnd = 0;
    int level = 0;              // List nesting level or Blockquote depth
    int listIndex = 1;          // For ordered list numbering
    bool isTaskChecked = false; // For task list items
    std::wstring codeLanguage;  // For fenced code blocks
    std::vector<std::wstring> codeLines;
    MarkdownTable table;
    bool isRTL = false;
    AlertType alertType = AlertType::None;
    std::wstring alertTitle;
};

struct MarkdownDocument {
    std::vector<MarkdownBlock> blocks;
    size_t headingCount = 0;
};

class MarkdownParser {
public:
    static MarkdownDocument Parse(const std::wstring& markdownText);
    static std::vector<MarkdownSpan> ParseInlines(const std::wstring& text);

private:
    static bool IsHorizontalRule(const std::wstring& line);
    static bool ParseHeading(const std::wstring& line, MarkdownBlockType& type, std::wstring& content, int& level);
    static bool ParseListItem(const std::wstring& line, MarkdownBlockType& type, std::wstring& content, int& index, int& nestLevel, bool& isChecked);
    static bool ParseBlockquote(const std::wstring& line, std::wstring& content, int& nestLevel);
    static bool IsTableDividerLine(const std::wstring& line, std::vector<TableColumnAlign>& aligns);
    static std::vector<std::wstring> SplitTableRow(const std::wstring& line);
};
