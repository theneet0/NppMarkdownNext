// Generated from headings.json.
#include <initializer_list>
struct HeadingCase { const wchar_t* name; std::initializer_list<const wchar_t*> reserved, headings, expected; };
inline const HeadingCase kHeadingCases[] = {
    {L"duplicates", {}, {L"Same", L"Same", L"Same"}, {L"same", L"same-1", L"same-2"}},
    {L"legacy-alias-collision", {}, {L"Intro", L"Reader Heading 0"}, {L"intro", L"reader-heading-0"}},
    {L"unicode", {}, {L"Привет Мир!", L"Café", L"مقدمه", L"Á title"}, {L"привет-мир", L"café", L"مقدمه", L"á-title"}},
    {L"punctuation", {}, {L"!!!", L"", L"API v2.0", L"Name-With_Dash"}, {L"section", L"section-1", L"api-v20", L"namewithdash"}},
    {L"reserved", {L"content", L"same", L"reader-heading-0"}, {L"Content", L"Same", L"Same"}, {L"content-1", L"same-1", L"same-2"}},
    {L"whitespace", {}, {L"  Hello\t World  ", L"A\nB", L"😀 Title"}, {L"hello-world", L"a-b", L"title"}},
    {L"classification", {}, {L"^´ Test", L"ः Title", L"½ ①", L"𐐀", L"😀"}, {L"test", L"ः-title", L"½-①", L"𐐨", L"section"}},
    {L"simple-lowercase", {}, {L"İ", L"Σ", L"ΟΣ", L"𞤀"}, {L"i̇", L"σ", L"οσ", L"𞤢"}},
};
