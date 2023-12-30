#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

using namespace std;

int sizeBuffer = 10;
int delete_size = 4;

struct SharedData {
    HANDLE  allFileReady;
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
        CREATE_NEW_CONSOLE,
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

    pData->allFileReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    pData->sizeBuffer = sizeBuffer;
}

int main() {
    setlocale(LC_ALL, "Russian");
    create_init_display_object();
    add_buff();
    
    PROCESS_INFORMATION pi = start_proc();
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hMapFile);
    CloseHandle(pData->allFileReady);


    Queue queue(sizeBuffer);
    queue.readFromFile("queue.txt");
    for (int i = 0; i < delete_size; i++) {
        queue.del();
    }
    std::cout << "Первый элемент в очереди после удаления " << delete_size << " элементов : " << queue.getFront() << std::endl;

    return 0;
}