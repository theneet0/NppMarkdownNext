#include "../include/HtmlExporter.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#ifndef NPP_READER_TESTING
#error The I/O harness must use the test-only Win32 shim.
#endif

namespace {
enum class Fault { None, ShortWrite, ZeroWrite, Write, Flush, Close, Replace1175,
    Replace1176, Replace1177, Restore, Race, Busy, Register, Allocate, AllocateSecond, Lock, LockSecond, Empty, Html, Text, Both, ClipboardClose };
Fault fault = Fault::None;
HANDLE temporaryHandle = INVALID_HANDLE_VALUE;
std::filesystem::path directory;
unsigned fileNumber = 0, writeAttempts = 0, allocationAttempts = 0, lockAttempts = 0;
bool emptied = false;
std::set<HGLOBAL> allocations;
std::map<UINT, HGLOBAL> transferred;
constexpr UINT htmlFormat = 0xc001;

void WriteBytes(const std::filesystem::path& path, const std::string& bytes) {
    std::ofstream file(path, std::ios::binary); file.write(bytes.data(), bytes.size());
    file.close(); assert(file.good());
}
std::string Bytes(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), {}};
}
std::filesystem::path NextPath() { return directory / (L"экспорт-" + std::to_wstring(++fileNumber) + L".html"); }

HANDLE WINAPI Create(LPCWSTR path, DWORD access, DWORD sharing, LPSECURITY_ATTRIBUTES security,
    DWORD disposition, DWORD attributes, HANDLE templateFile) {
    HANDLE value = ::CreateFileW(path, access, sharing, security, disposition, attributes, templateFile);
    if (disposition == CREATE_NEW) temporaryHandle = value;
    return value;
}
BOOL WINAPI Write(HANDLE file, LPCVOID bytes, DWORD count, LPDWORD written, LPOVERLAPPED overlap) {
    ++writeAttempts;
    if (fault == Fault::Write) { SetLastError(ERROR_DISK_FULL); return FALSE; }
    if (fault == Fault::ZeroWrite) { *written = 0; return TRUE; }
    if (fault == Fault::ShortWrite && writeAttempts == 1) count = 3;
    return ::WriteFile(file, bytes, count, written, overlap);
}
BOOL WINAPI Flush(HANDLE file) {
    if (fault == Fault::Flush) { SetLastError(ERROR_WRITE_FAULT); return FALSE; }
    return ::FlushFileBuffers(file);
}
BOOL WINAPI Close(HANDLE file) {
    BOOL result = ::CloseHandle(file);
    if (fault == Fault::Close && file == temporaryHandle) { SetLastError(ERROR_INVALID_HANDLE); return FALSE; }
    return result;
}
BOOL WINAPI Replace(LPCWSTR target, LPCWSTR replacement, LPCWSTR backup, DWORD flags, LPVOID a, LPVOID b) {
    if (fault == Fault::Replace1175 || fault == Fault::Replace1176) {
        SetLastError(fault == Fault::Replace1175 ? 1175 : 1176); return FALSE;
    }
    if (fault == Fault::Replace1177 || fault == Fault::Restore) {
        assert(::MoveFileExW(target, backup, 0));
        SetLastError(1177); return FALSE;
    }
    return ::ReplaceFileW(target, replacement, backup, flags, a, b);
}
BOOL WINAPI Move(LPCWSTR source, LPCWSTR destination, DWORD flags) {
    if (fault == Fault::Restore && std::wstring(source).ends_with(L".bak")) {
        SetLastError(ERROR_ACCESS_DENIED); return FALSE;
    }
    if (fault == Fault::Race) WriteBytes(destination, "concurrent file");
    return ::MoveFileExW(source, destination, flags);
}
BOOL WINAPI Open(HWND) { if (fault == Fault::Busy) { SetLastError(ERROR_ACCESS_DENIED); return FALSE; } return TRUE; }
BOOL WINAPI Empty() { if (fault == Fault::Empty) { SetLastError(ERROR_ACCESS_DENIED); return FALSE; } emptied = true; return TRUE; }
BOOL WINAPI FakeCloseClipboard() { if (fault == Fault::ClipboardClose) { SetLastError(ERROR_ACCESS_DENIED); return FALSE; } return TRUE; }
UINT WINAPI Register(LPCSTR name) {
    assert(std::string(name) == "HTML Format");
    if (fault == Fault::Register) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); return 0; }
    return htmlFormat;
}
HGLOBAL WINAPI Allocate(UINT flags, SIZE_T size) {
    ++allocationAttempts;
    if (fault == Fault::Allocate || (fault == Fault::AllocateSecond && allocationAttempts == 2)) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); return nullptr; }
    HGLOBAL value = ::GlobalAlloc(flags, size); assert(value); allocations.insert(value); return value;
}
LPVOID WINAPI Lock(HGLOBAL value) {
    ++lockAttempts;
    if (fault == Fault::Lock || (fault == Fault::LockSecond && lockAttempts == 2)) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); return nullptr; }
    return ::GlobalLock(value);
}
HGLOBAL WINAPI Free(HGLOBAL value) {
    assert(allocations.erase(value) == 1);
    for (const auto& entry : transferred) assert(entry.second != value);
    return ::GlobalFree(value);
}
HANDLE WINAPI Set(UINT format, HANDLE value) {
    assert(emptied);
    if (fault == Fault::Both || (format == htmlFormat && fault == Fault::Html) ||
        (format == CF_UNICODETEXT && fault == Fault::Text)) {
        SetLastError(ERROR_ACCESS_DENIED); return nullptr;
    }
    assert(allocations.contains(static_cast<HGLOBAL>(value)));
    transferred[format] = static_cast<HGLOBAL>(value); return value;
}
void Reset(Fault next) {
    for (const auto& entry : transferred) { assert(allocations.erase(entry.second) == 1); ::GlobalFree(entry.second); }
    transferred.clear(); assert(allocations.empty());
    fault = next; emptied = false; writeAttempts = 0; allocationAttempts = 0; lockAttempts = 0;
}
MarkdownDocument Document() { return MarkdownParser::Parse(L"# Title\n\n**Привет** سلام 😀\n\n```text\ncode line\n```\n\n| A | B |\n|---|---|\n| 1 | 2 |\n"); }
OperationResult Save(const std::filesystem::path& path) { return HtmlExporter::SaveToFile(path.wstring(), Document(), L"Title", PreviewTheme::Light); }
OperationResult Copy() { return HtmlExporter::CopyToClipboard(reinterpret_cast<HWND>(1), Document(), L"Title", PreviewTheme::Light); }

void AtomicSaveFailures() {
    for (Fault next : {Fault::None, Fault::ShortWrite}) {
        Reset(next); auto path = NextPath(); auto result = Save(path);
        assert(result.status == OperationStatus::Succeeded);
        assert(Bytes(path) == HtmlExporter::GeneratePreviewHtml(Document(), L"Title", PreviewTheme::Light, 1, false));
        if (next == Fault::ShortWrite) assert(writeAttempts > 1);
    }
    for (Fault next : {Fault::Write, Fault::Flush, Fault::Close}) {
        Reset(next); auto path = NextPath(); WriteBytes(path, "original bytes");
        assert(Save(path).status == OperationStatus::Failed); assert(Bytes(path) == "original bytes");
    }
    Reset(Fault::None); auto path = NextPath(); WriteBytes(path, "old");
    assert(Save(path).status == OperationStatus::Succeeded); assert(Bytes(path).starts_with("<!DOCTYPE html>"));
}
void AtomicSaveCommitStates() {
    for (Fault next : {Fault::Replace1175, Fault::Replace1176, Fault::Replace1177, Fault::Restore}) {
        Reset(next); auto path = NextPath(); WriteBytes(path, "original bytes");
        auto result = Save(path);
        assert(result.status == OperationStatus::Failed);
        assert(!result.recoveryPath.empty()); assert(Bytes(result.recoveryPath) == "original bytes");
        if (next == Fault::Restore) { assert(!std::filesystem::exists(path)); assert(std::filesystem::exists(result.recoveryPath)); }
        else assert(Bytes(path) == "original bytes");
    }
}
void AtomicSaveNoProgressAndRace() {
    Reset(Fault::ZeroWrite); auto path = NextPath(); WriteBytes(path, "old");
    assert(Save(path).status == OperationStatus::Failed); assert(writeAttempts == 1); assert(Bytes(path) == "old");
    Reset(Fault::Race); path = NextPath(); assert(Save(path).status == OperationStatus::Failed);
    assert(Bytes(path) == "concurrent file");
    Reset(Fault::Close); path = NextPath(); assert(Save(path).status == OperationStatus::Failed);
    assert(!std::filesystem::exists(path));
}
void ClipboardFormatsAndFailures() {
    Reset(Fault::None); assert(Copy().status == OperationStatus::Succeeded);
    auto* html = static_cast<const char*>(::GlobalLock(transferred.at(htmlFormat)));
    std::string value(html); ::GlobalUnlock(transferred.at(htmlFormat));
    auto offset = [&](const std::string& key) { return std::stoull(value.substr(value.find(key) + key.size(), 8)); };
    assert(value.substr(offset("StartFragment:"), 3) == "<h1");
    assert(value.substr(offset("EndFragment:"), 18) == "<!--EndFragment-->");
    assert(offset("EndHTML:") == value.size());
    auto* wide = static_cast<const wchar_t*>(::GlobalLock(transferred.at(CF_UNICODETEXT)));
    std::wstring plain(wide); ::GlobalUnlock(transferred.at(CF_UNICODETEXT));
    assert(plain.find(L"Привет سلام 😀") != std::wstring::npos);
    assert(plain.find(L"**") == std::wstring::npos);
    assert(plain.find(L"code line") != std::wstring::npos && plain.find(L"1\t2") != std::wstring::npos);
    for (Fault next : {Fault::Busy, Fault::Register, Fault::Allocate, Fault::AllocateSecond, Fault::Lock, Fault::LockSecond, Fault::Empty}) {
        Reset(next); assert(Copy().status == OperationStatus::Failed); assert(!emptied);
        assert(allocations.empty() && transferred.empty());
    }
    for (Fault next : {Fault::Html, Fault::Text}) {
        Reset(next); assert(Copy().status == OperationStatus::Partial);
        assert(transferred.size() == 1 && allocations.size() == 1);
    }
    Reset(Fault::Both); assert(Copy().status == OperationStatus::Failed); assert(allocations.empty());
    Reset(Fault::ClipboardClose); auto result = Copy();
    assert(result.status == OperationStatus::Failed && result.detail.find(L"transferred") != std::wstring::npos);
    assert(transferred.size() == 2); Reset(Fault::None);
}
}

int main(int argc, char** argv) {
    wchar_t executable[MAX_PATH]{}; GetModuleFileNameW(nullptr, executable, MAX_PATH);
    auto parent = std::filesystem::path(executable).parent_path();
    directory = parent / (L"export-io-" + std::to_wstring(GetCurrentProcessId()) + L"-" + std::to_wstring(GetTickCount64()));
    assert(directory.parent_path() == parent && std::filesystem::create_directory(directory));
    ExportIoTest::Hooks hooks;
    hooks.createFile = Create; hooks.writeFile = Write; hooks.flushFile = Flush;
    hooks.closeHandle = Close; hooks.replaceFile = Replace; hooks.moveFile = Move;
    hooks.openClipboard = Open; hooks.emptyClipboard = Empty; hooks.closeClipboard = FakeCloseClipboard;
    hooks.registerFormat = Register; hooks.globalAlloc = Allocate; hooks.globalLock = Lock;
    hooks.globalFree = Free; hooks.setClipboardData = Set; ExportIoTest::SetHooks(hooks);
    std::string selected = argc == 3 && std::string(argv[1]) == "--case" ? argv[2] : "";
    bool ran = false;
    for (const auto& test : std::map<std::string, void(*)()>{{"AtomicSaveFailures", AtomicSaveFailures},
        {"AtomicSaveCommitStates", AtomicSaveCommitStates}, {"AtomicSaveNoProgressAndRace", AtomicSaveNoProgressAndRace},
        {"ClipboardFormatsAndFailures", ClipboardFormatsAndFailures}}) {
        if (!selected.empty() && selected != test.first) continue;
        std::cout << test.first << std::endl; test.second(); std::cout << "PASS\n"; ran = true;
    }
    assert(ran); Reset(Fault::None);
    assert(directory.parent_path() == parent); std::filesystem::remove_all(directory);
}
