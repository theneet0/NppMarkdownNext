#pragma once
#include <windows.h>
#include <string>

enum class OperationStatus { Succeeded, Cancelled, Failed, Partial };

struct OperationResult {
    OperationStatus status = OperationStatus::Failed;
    DWORD error = ERROR_SUCCESS;
    std::wstring detail;
    std::wstring recoveryPath;
};
