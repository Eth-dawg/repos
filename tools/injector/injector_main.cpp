#include <Windows.h>
#include <TlHelp32.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace {

DWORD FindProcessIdByName(const std::wstring& processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W entry {};
    entry.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0) {
                processId = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return processId;
}

bool InjectDll(DWORD pid, const std::wstring& dllPath) {
    HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (!process) {
        return false;
    }

    const SIZE_T bytes = (dllPath.size() + 1) * sizeof(wchar_t);
    void* remoteMem = VirtualAllocEx(process, nullptr, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        CloseHandle(process);
        return false;
    }

    if (!WriteProcessMemory(process, remoteMem, dllPath.c_str(), bytes, nullptr)) {
        VirtualFreeEx(process, remoteMem, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }

    HMODULE kernel = GetModuleHandleW(L"kernel32.dll");
    auto loadLibrary = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(kernel, "LoadLibraryW"));
    HANDLE thread = CreateRemoteThread(process, nullptr, 0, loadLibrary, remoteMem, 0, nullptr);
    if (!thread) {
        VirtualFreeEx(process, remoteMem, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(process, remoteMem, 0, MEM_RELEASE);
    CloseHandle(process);
    return true;
}

} // namespace

int wmain(int argc, wchar_t** argv) {
    if (argc < 3) {
        std::wcout << L"Usage: camelotpad_injector <process_name.exe> <path_to_dll>\n";
        return 1;
    }

    const std::wstring processName = argv[1];
    const std::filesystem::path dllPath = std::filesystem::absolute(argv[2]);
    if (!std::filesystem::exists(dllPath)) {
        std::wcout << L"DLL not found: " << dllPath.c_str() << L"\n";
        return 1;
    }

    const DWORD pid = FindProcessIdByName(processName);
    if (pid == 0) {
        std::wcout << L"Process not found: " << processName << L"\n";
        return 1;
    }

    if (!InjectDll(pid, dllPath.wstring())) {
        std::wcout << L"Injection failed.\n";
        return 1;
    }

    std::wcout << L"Injection succeeded into PID " << pid << L".\n";
    return 0;
}
