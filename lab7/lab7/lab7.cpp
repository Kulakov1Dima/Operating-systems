#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

using namespace std;

int sizeBuffer = 10;
int delete_size = 4;

struct SharedData {
    HANDLE dataReady;
    int sizeBuffer;
};

SharedData* pData;
HANDLE hMapFile;

void create_init_display_object() {    
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(SharedData),
        L"MemoryFile");

    if (hMapFile == NULL) {
        std::cerr << "Ошибка при создании файловой проекции: " << GetLastError() << std::endl;
        exit(1);
    }
}

PROCESS_INFORMATION start_proc() {
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    std::wstring currentDirectory = szPath;
    std::wstring::size_type pos = currentDirectory.find_last_of(L"\\");
    std::wstring childProcessPath = currentDirectory.substr(0, pos + 1) + L"Lab6.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    if (!CreateProcess(
        childProcessPath.c_str(),
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "Ошибка при создании дочернего процесса: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        exit(1);
    }

    return pi;
}

void add_buff() {
    pData = static_cast<SharedData*>(MapViewOfFile(
        hMapFile,
        FILE_MAP_WRITE,
        0,
        0,
        sizeof(SharedData)));

    if (pData == nullptr) {
        std::cerr << "Ошибка при отображении файловой проекции: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        exit(1);
    }

    pData->dataReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    pData->sizeBuffer = sizeBuffer;

    PROCESS_INFORMATION pi = start_proc();
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hMapFile);
    
}

void read_init_display_object() {
    hMapFile = OpenFileMapping(
        FILE_MAP_READ,
        FALSE,
        L"MemoryFile");

    if (hMapFile == NULL) {
        cerr << "Ошибка при открытии файловой проекции: " << GetLastError() << std::endl;
        exit(1);
    }

    pData = static_cast<SharedData*>(MapViewOfFile(
        hMapFile,
        FILE_MAP_READ,
        0,
        0,
        sizeof(SharedData)));
}

DWORD WINAPI Readdispl(LPVOID lpParam) {
    while (true) {
        read_init_display_object();
        Sleep(5000);
    }

    return 0;
}

int main() {
    create_init_display_object();
    setlocale(LC_ALL, "Russian");
    HANDLE addToBufferThread = CreateThread(NULL, 0, Readdispl, NULL, 0, NULL);
    add_buff();
    WaitForSingleObject(pData->dataReady, INFINITE);
    CloseHandle(addToBufferThread);
    CloseHandle(pData->dataReady);
    return 0;
}