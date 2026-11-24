// Generated from themes/callouts.json.
struct CalloutRegistryEntry { const wchar_t* alias; AlertType type; const wchar_t* title; };
inline constexpr CalloutRegistryEntry kCalloutRegistry[] = {
    {L"NOTE", AlertType::Note, L"Note"},
    {L"نکته", AlertType::Note, L"نکته"},
    {L"یادداشت", AlertType::Note, L"نکته"},
    {L"TIP", AlertType::Tip, L"Tip"},
    {L"راهنما", AlertType::Tip, L"ترفند"},
    {L"ترفند", AlertType::Tip, L"ترفند"},
    {L"IMPORTANT", AlertType::Important, L"Important"},
    {L"مهم", AlertType::Important, L"مهم"},
    {L"WARNING", AlertType::Warning, L"Warning"},
    {L"هشدار", AlertType::Warning, L"هشدار"},
    {L"CAUTION", AlertType::Caution, L"Caution"},
    {L"احتیاط", AlertType::Caution, L"احتیاط"},
    {L"خطر", AlertType::Caution, L"احتیاط"},
};
