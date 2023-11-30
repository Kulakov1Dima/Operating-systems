#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

using namespace std;

struct SharedData {
    HANDLE dataReady;
    int sizeBuffer;
};
SharedData* pData;
HANDLE hMapFile;
HANDLE fileReady;

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
    CloseHandle(hMapFile);
}

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

DWORD WINAPI AddToBuffer(LPVOID lpParam) {
    Queue buffer(pData->sizeBuffer);
    for (int i = 0; i < pData->sizeBuffer; i++) {
        int start = 10;
        int end = 200;
        int x = rand() % (end - start + 1) + start;

        buffer.add(x);

        std::cout << "Добавлено в буфер: " << x << std::endl;

    }
    
    buffer.writeToFile("queue.txt");
    pData->sizeBuffer = pData->sizeBuffer;
    SetEvent(pData->dataReady);
    create_init_display_object();
    return 0;
}

void check_init() {
    if (pData == nullptr) {
        std::cerr << "Ошибка при отображении файловой проекции: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        exit(1);
    }
    else {
        std::cout << "Размер буфера: " << pData->sizeBuffer << "\n";
        AddToBuffer(NULL);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    read_init_display_object();
    check_init();

    UnmapViewOfFile(pData);
    CloseHandle(hMapFile);
    return 0;
}