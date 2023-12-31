﻿#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

using namespace std;

struct SharedData {
    HANDLE allFileReady;
    int sizeBuffer;
};
SharedData* pData;
HANDLE hMapFile;

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

void AddToBuffer() {
    Queue buffer(pData->sizeBuffer);

    for (int i = 0; i < pData->sizeBuffer; i++) {
        int start = 10;
        int end = 200;
        int x = rand() % (end - start + 1) + start;

        buffer.add(x);

        std::cout << "Добавлено в буфер: " << x << std::endl;

    }
    
    buffer.writeToFile("queue.txt");
    SetEvent(pData->allFileReady);
}

DWORD WINAPI check_init(LPVOID lpParam) {
    if (pData == nullptr) {
        std::cerr << "Ошибка при отображении файловой проекции: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        exit(1);
    }
    else {
        std::cout << "Размер буфера: " << pData->sizeBuffer << std::endl << std::endl;
        AddToBuffer();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    read_init_display_object();

    HANDLE addToBufferThread = CreateThread(NULL, 0, check_init, NULL, 0, NULL);

    Sleep(10000);
    std::cout << std::endl;

    WaitForSingleObject(pData->allFileReady, INFINITE);

    CloseHandle(addToBufferThread);
    CloseHandle(hMapFile);
    CloseHandle(pData->allFileReady);
    return 0;
}