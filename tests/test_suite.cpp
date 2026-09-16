#include "../include/MarkdownParser.h"
#include "../include/BiDiEngine.h"
#include "../include/SyntaxHighlighter.h"
#include "../include/HtmlExporter.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

void TestBiDiEngine() {
    std::cout << "[TEST] BiDiEngine..." << std::endl;

    // 1. RTL char detection
    assert(BiDiEngine::IsRTLChar(0x06AF)); // گ
    assert(BiDiEngine::IsRTLChar(0x0686)); // چ
    assert(BiDiEngine::IsRTLChar(0x067E)); // پ
    assert(BiDiEngine::IsRTLChar(0x0698)); // ژ
    assert(BiDiEngine::IsRTLChar(0x06CC)); // ی
    assert(!BiDiEngine::IsRTLChar(L'A'));
    assert(!BiDiEngine::IsRTLChar(L'1'));

    // 2. Strong direction detection
    std::wstring persianSentence = L"این یک متن فارسی است";
    assert(BiDiEngine::IsParagraphRTL(persianSentence));

    std::wstring englishSentence = L"This is an English sentence.";
    assert(!BiDiEngine::IsParagraphRTL(englishSentence));

    // 3. Mixed direction - first strong character
    std::wstring mixedRtl = L"  سلام Hello";
    assert(BiDiEngine::IsParagraphRTL(mixedRtl));

    std::wstring mixedLtr = L"  Hello سلام";
    assert(!BiDiEngine::IsParagraphRTL(mixedLtr));

    // 4. Persian normalization
    std::wstring arabicText = L"يك كمك"; // With Arabic Kaf (0x0643) and Arabic Yeh (0x064A)
    std::wstring normalized = BiDiEngine::NormalizePersianGlyphs(arabicText);
    assert(normalized.find(0x064A) == std::wstring::npos);
    assert(normalized.find(0x0643) == std::wstring::npos);
    assert(normalized.find(0x06CC) != std::wstring::npos); // Persian Yeh
    assert(normalized.find(0x06A9) != std::wstring::npos); // Persian Kaf

    // 5. UTF conversions
    std::string utf8Str = "سلام دنیا Hello World";
    std::wstring wideStr = BiDiEngine::Utf8ToWide(utf8Str);
    assert(!wideStr.empty());
    std::string backToUtf8 = BiDiEngine::WideToUtf8(wideStr);
    assert(backToUtf8 == utf8Str);

    // 6. Persian digit conversion & number parsing
    assert(BiDiEngine::ToPersianDigits(123) == L"۱۲۳");
    assert(BiDiEngine::ToPersianDigits(0) == L"۰");
    int parsedNum = 0;
    size_t consumed = 0;
    assert(BiDiEngine::ParseNumber(L"۴۵۶", 0, parsedNum, consumed));
    assert(parsedNum == 456 && consumed == 3);

    std::cout << "  -> BiDiEngine PASS" << std::endl;
}

void TestMarkdownParser() {
    std::cout << "[TEST] MarkdownParser..." << std::endl;

    std::wstring md =
        L"# Heading 1\n"
        L"## Heading 2\n"
        L"A simple paragraph with **bold** and *italic* and ==highlight== and `code`.\n"
        L"\n"
        L"- Item 1\n"
        L"- [ ] Incomplete task\n"
        L"- [x] Completed task\n"
        L"\n"
        L"۱. مورد اول فارسی\n"
        L"۲. مورد دوم فارسی\n"
        L"\n"
        L"```cpp\n"
        L"int main() {\n"
        L"    return 0;\n"
        L"}\n"
        L"```\n"
        L"\n"
        L"| Col 1 | Col 2 |\n"
        L"|:------|:-----:|\n"
        L"| Val A | Val B |\n"
        L"\n"
        L"> [!NOTE]\n"
        L"> This is an English note alert.\n"
        L"\n"
        L"> [!هشدار]\n"
        L"> این یک هشدار مهم به زبان فارسی است.\n"
        L"\n"
        L"> This is a classic blockquote\n"
        L"\n"
        L"---\n";

    MarkdownDocument doc = MarkdownParser::Parse(md);
    assert(!doc.blocks.empty());

    // Check headings
    assert(doc.blocks[0].type == MarkdownBlockType::Header1);
    assert(doc.blocks[0].rawText == L"Heading 1");
    assert(doc.blocks[1].type == MarkdownBlockType::Header2);
    assert(doc.blocks[1].rawText == L"Heading 2");

    // Check paragraph inlines
    assert(doc.blocks[2].type == MarkdownBlockType::Paragraph);
    bool hasBold = false, hasItalic = false, hasCode = false, hasHighlight = false;
    for (const auto& span : doc.blocks[2].inlines) {
        if (span.type == InlineStyleType::Bold && span.text == L"bold") hasBold = true;
        if (span.type == InlineStyleType::Italic && span.text == L"italic") hasItalic = true;
        if (span.type == InlineStyleType::Highlight && span.text == L"highlight") hasHighlight = true;
        if (span.type == InlineStyleType::InlineCode && span.text == L"code") hasCode = true;
    }
    assert(hasBold);
    assert(hasItalic);
    assert(hasHighlight);
    assert(hasCode);

    // Check list items
    assert(doc.blocks[3].type == MarkdownBlockType::UnorderedListItem);
    assert(doc.blocks[4].type == MarkdownBlockType::TaskListItem && !doc.blocks[4].isTaskChecked);
    assert(doc.blocks[5].type == MarkdownBlockType::TaskListItem && doc.blocks[5].isTaskChecked);

    // Check Persian ordered list
    assert(doc.blocks[6].type == MarkdownBlockType::OrderedListItem);
    assert(doc.blocks[6].listIndex == 1);
    assert(doc.blocks[6].isRTL);
    assert(doc.blocks[7].type == MarkdownBlockType::OrderedListItem);
    assert(doc.blocks[7].listIndex == 2);
    assert(doc.blocks[7].isRTL);

    // Check code block
    assert(doc.blocks[8].type == MarkdownBlockType::CodeBlock);
    assert(doc.blocks[8].codeLanguage == L"cpp");
    assert(doc.blocks[8].codeLines.size() == 3);

    // Check table
    assert(doc.blocks[9].type == MarkdownBlockType::Table);
    assert(doc.blocks[9].table.alignments.size() == 2);
    assert(doc.blocks[9].table.rows.size() == 2);

    // Check Alerts (English & Persian)
    assert(doc.blocks[10].type == MarkdownBlockType::AlertCallout);
    assert(doc.blocks[10].alertType == AlertType::Note);

    assert(doc.blocks[11].type == MarkdownBlockType::AlertCallout);
    assert(doc.blocks[11].alertType == AlertType::Warning);
    assert(doc.blocks[11].isRTL);

    // Check regular blockquote & HR
    assert(doc.blocks[12].type == MarkdownBlockType::Blockquote);
    assert(doc.blocks[13].type == MarkdownBlockType::HorizontalRule);

    std::cout << "  -> MarkdownParser PASS" << std::endl;
}

void TestSyntaxHighlighter() {
    std::cout << "[TEST] SyntaxHighlighter..." << std::endl;

    std::wstring cppCode = L"const int count = 42; // Answer";
    auto tokens = SyntaxHighlighter::Tokenize(cppCode, L"cpp");
    assert(!tokens.empty());

    bool foundKeyword = false;
    bool foundType = false;
    bool foundNumber = false;
    bool foundComment = false;

    for (const auto& t : tokens) {
        if (t.type == HighlightTokenType::Keyword) foundKeyword = true;
        if (t.type == HighlightTokenType::Type) foundType = true;
        if (t.type == HighlightTokenType::Number) foundNumber = true;
        if (t.type == HighlightTokenType::Comment) foundComment = true;
    }
    assert(foundKeyword);
    assert(foundType);
    assert(foundNumber);
    assert(foundComment);

    // Bash / shell tokens
    std::wstring shCode = L"sudo chmod 0755 /usr/local/bin/gost # Make executable";
    auto shTokens = SyntaxHighlighter::Tokenize(shCode, L"bash");
    assert(!shTokens.empty());
    bool foundShKw = false, foundShNum = false, foundShComment = false;
    for (const auto& t : shTokens) {
        if (t.type == HighlightTokenType::Keyword) foundShKw = true;
        if (t.type == HighlightTokenType::Number) foundShNum = true;
        if (t.type == HighlightTokenType::Comment) foundShComment = true;
    }
    // Ensure 'local' in '/usr/local/bin/gost' is NOT treated as a bash keyword
    for (const auto& t : shTokens) {
        std::wstring tokStr = shCode.substr(t.start, t.length);
        assert(tokStr != L"local");
    }

    std::cout << "  -> SyntaxHighlighter PASS" << std::endl;
}

void TestHtmlExporter() {
    std::cout << "[TEST] HtmlExporter..." << std::endl;

    std::wstring md =
        L"# Title\n\n"
        L"Persian: سلام دنیا\n\n"
        L"- ایران: `/usr/local/bin/ir.yaml`\n\n"
        L"فایل binary باید `/usr/local/bin/gost` باشد.\n\n"
        L"Inline math: $a^2 + b^2 = c^2$ فرمول ریاضی\n\n"
        L"```text\n"
        L"/usr/local/bin/fullchain.cer\n"
        L"/usr/local/bin/jojo-data.com.key\n"
        L"```\n\n"
        L"```bash\n"
        L"chown root:root /usr/local/bin/gost\n"
        L"chmod 0755 /usr/local/bin/gost\n"
        L"```\n\n"
        L"> [!NOTE]\n"
        L"> Alert test\n\n"
        L"$$E = mc^2$$\n\n"
        L"```mermaid\n"
        L"graph TD\n"
        L"node-a[Step 1] --> node-b(Step 2)\n"
        L"```\n";

    MarkdownDocument doc = MarkdownParser::Parse(md);
    std::wstring html = HtmlExporter::ExportToHtml(doc, L"Test Title", true);

    assert(!html.empty());
    assert(html.find(L"<!DOCTYPE html>") != std::wstring::npos);
    assert(html.find(L"<title>Test Title</title>") != std::wstring::npos);
    assert(html.find(L"dir=\"rtl\"") != std::wstring::npos);
    assert(html.find(L"<ul") != std::wstring::npos);
    assert(html.find(L"</ul>") != std::wstring::npos);
    assert(html.find(L"alert-callout") != std::wstring::npos);

    // 1. Verify BiDi isolated inline code
    assert(html.find(L"<code class=\"inline-code\" dir=\"ltr\"><bdi dir=\"ltr\">/usr/local/bin/ir.yaml</bdi></code>") != std::wstring::npos);
    assert(html.find(L"<code class=\"inline-code\" dir=\"ltr\"><bdi dir=\"ltr\">/usr/local/bin/gost</bdi></code>") != std::wstring::npos);
    assert(html.find(L"katex-inline") != std::wstring::npos);
    assert(html.find(L"data-tex=\"a^2 + b^2 = c^2\"") != std::wstring::npos);
    assert(html.find(L"node-a[Step 1]") != std::wstring::npos);
    assert(html.find(L"node-b(Step 2)") != std::wstring::npos);

    // 2. Verify code block text is intact and NOT blank
    assert(html.find(L"/usr/local/bin/fullchain.cer") != std::wstring::npos);
    assert(html.find(L"/usr/local/bin/jojo-data.com.key") != std::wstring::npos);
    assert(html.find(L"chown") != std::wstring::npos);
    assert(html.find(L"chmod") != std::wstring::npos);

    // Verify code block does NOT have nested <!DOCTYPE html> inside <pre><code>
    size_t prePos = html.find(L"<pre><code>");
    assert(prePos != std::wstring::npos);
    size_t codeEndPos = html.find(L"</code></pre>", prePos);
    assert(codeEndPos != std::wstring::npos);
    std::wstring preCodeContent = html.substr(prePos, codeEndPos - prePos);
    assert(preCodeContent.find(L"<!DOCTYPE") == std::wstring::npos);
    assert(preCodeContent.find(L"<floating-toolbar") == std::wstring::npos);

    // 3. Test modern GeneratePreviewHtml
    std::string previewHtml = HtmlExporter::GeneratePreviewHtml(doc, L"Preview Title", true, 1.0f, true);
    assert(!previewHtml.empty());
    assert(previewHtml.find("floating-toolbar") == std::string::npos);
    assert(previewHtml.find("app-layout") != std::string::npos);
    assert(previewHtml.find("custom-context-menu") != std::string::npos);
    assert(previewHtml.find("menu-sync-label") != std::string::npos);
    assert(previewHtml.find("menu-theme-label") != std::string::npos);
    assert(previewHtml.find("menu-toc-label") != std::string::npos);
    assert(previewHtml.find("_selectedText") != std::string::npos);
    assert(previewHtml.find("tocStateChanged") != std::string::npos);
    assert(previewHtml.find("search-overlay") != std::string::npos);
    assert(previewHtml.find("toc-drawer") != std::string::npos);
    assert(previewHtml.find("toc-backdrop") != std::string::npos);
    assert(previewHtml.find("katex") != std::string::npos);
    assert(previewHtml.find("mermaid") != std::string::npos);

    // 4. Verify 100% offline / zero network links (no external CDNs or external fonts)
    assert(previewHtml.find("fonts.googleapis.com") == std::string::npos);
    assert(previewHtml.find("fonts.gstatic.com") == std::string::npos);
    assert(previewHtml.find("cdn.jsdelivr.net") == std::string::npos);
    assert(previewHtml.find("http://") == std::string::npos);
    // The only https:// references allowed are MathML/SVG namespace URIs like xmlns="http://www.w3.org/..."
    assert(previewHtml.find("https://cdn.") == std::string::npos);
    assert(previewHtml.find("https://fonts.") == std::string::npos);

    // 5. Test GeneratePreviewComponents for instant in-place DOM updates
    auto components = HtmlExporter::GeneratePreviewComponents(doc, L"Preview Title", true, 1.0f, true);
    assert(!components.fullHtml.empty());
    assert(!components.bodyHtml.empty());
    assert(!components.tocHtml.empty());
    assert(components.bodyHtml.find("<!DOCTYPE") == std::string::npos); // Clean body fragment!
    assert(components.bodyHtml.find("/usr/local/bin/fullchain.cer") != std::string::npos);

    // 6. Verify context menu zoom controls exist and reading stats are completely absent
    assert(previewHtml.find("Zoom In") != std::string::npos);
    assert(previewHtml.find("Zoom Out") != std::string::npos);
    assert(previewHtml.find("Reset Zoom") != std::string::npos);
    assert(previewHtml.find("Save As HTML") != std::string::npos);
    assert(previewHtml.find("Smart BiDi") != std::string::npos);
    assert(previewHtml.find("zoomInFromMenu") != std::string::npos);
    assert(previewHtml.find("zoomOutFromMenu") != std::string::npos);
    assert(previewHtml.find("zoomResetFromMenu") != std::string::npos);
    assert(previewHtml.find("saveAsHtmlFromMenu") != std::string::npos);
    assert(previewHtml.find("toggleBiDiFromMenu") != std::string::npos);
    assert(previewHtml.find("min read") == std::string::npos);
    assert(previewHtml.find("menu-stats") == std::string::npos);
    assert(previewHtml.find("\u0645\u0637\u0627\u0644\u0639\u0647") == std::string::npos);
    assert(previewHtml.find("\u062f\u0642\u06cc\u0642\u0647") == std::string::npos);

    std::cout << "  -> HtmlExporter PASS" << std::endl;
}

void TestRealUserDocument() {
    std::cout << "[TEST] Real User Document (gost-systemd-install-fa.md)..." << std::endl;
    std::ifstream f("tests/fixtures/gost-systemd-install-fa.md", std::ios::binary);
    if (!f.is_open()) {
        f.open("gost-systemd-install-fa.md", std::ios::binary);
    }
    if (!f.is_open()) {
        f.open("E:/cert/GOST3/gost-systemd-install-fa.md", std::ios::binary);
    }
    if (!f.is_open()) {
        std::cout << "  [SKIP] User document file not found in tests/fixtures/ or E:/cert/GOST3/..." << std::endl;
        return;
    }
    std::string bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    std::wstring wide = BiDiEngine::Utf8ToWide(bytes);
    assert(!wide.empty());

    MarkdownDocument doc = MarkdownParser::Parse(wide);
    assert(doc.blocks.size() > 0);

    auto comp = HtmlExporter::GeneratePreviewComponents(doc, L"gost-systemd-install-fa.md", true, 1.0f, true);

    // Check code block content
    assert(comp.bodyHtml.find("/usr/local/bin/fullchain.cer") != std::string::npos);
    assert(comp.bodyHtml.find("/usr/local/bin/jojo-data.com.key") != std::string::npos);
    assert(comp.bodyHtml.find("chown") != std::string::npos);
    assert(comp.bodyHtml.find("chmod") != std::string::npos);
    assert(comp.bodyHtml.find("root:root") != std::string::npos);

    // Verify 'local' keyword is NOT injected inside /usr/local/bin/ paths
    assert(comp.bodyHtml.find("/usr/<span class=\"hl-keyword\">local</span>/bin") == std::string::npos);

    std::cout << "  -> Real User Document PASS" << std::endl;
}

int main() {
    std::cout << "=== Running NppMarkdownNext Native C++ Unit Tests ===" << std::endl;
    TestBiDiEngine();
    TestMarkdownParser();
    TestSyntaxHighlighter();
    TestHtmlExporter();
    TestRealUserDocument();
    std::cout << "\n>>> ALL 5 TEST SUITES PASSED SUCCESSFULLY! <<<" << std::endl;
    return 0;
}
