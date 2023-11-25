#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

int sizeBuffer = 10;
int delete_size = 4;

Queue queue0(sizeBuffer);
Queue buffer(sizeBuffer);

HANDLE dataReady;
HANDLE fileReady;
HANDLE allFileReady;


DWORD WINAPI AddToBuffer(LPVOID lpParam) {
    for (int i = 0; i < sizeBuffer; i++) {
        int start = 10;
        int end = 200;
        int x = rand() % (end - start + 1) + start;

        buffer.add(x);
        std::cout << "Добавлено в буфер: " << x << std::endl;
    }
    SetEvent(dataReady);

    return 0;
}


DWORD WINAPI Save(LPVOID lpParam) {
    while (true) {
        WaitForSingleObject(dataReady, INFINITE);

        while (!buffer.isEmpty()) {
            int value = buffer.getFront();
            buffer.del();
            queue0.add(value);

            queue0.writeToFile("queue.txt");

            SetEvent(fileReady);
        }

        if (buffer.isEmpty()) {
            SetEvent(allFileReady);
            break;
        }
    }

    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    dataReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    fileReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    allFileReady = CreateEvent(NULL, TRUE, FALSE, NULL);

    HANDLE addToBufferThread = CreateThread(NULL, 0, AddToBuffer, NULL, 0, NULL);
    HANDLE saveToFileThread = CreateThread(NULL, 0, Save, NULL, 0, NULL);

    WaitForSingleObject(allFileReady, INFINITE);


    std::cout << "Первый элемент в очереди: " << queue0.getFront() << std::endl;

    queue0.writeToFile("queue.txt");

    Queue queue1(sizeBuffer);
    queue1.readFromFile("queue.txt");
    for (int i = 0; i < delete_size; i++) {
        queue1.del();
    }
    std::cout << "Первый элемент в очереди после удаления "<< delete_size<< " элементов : " << queue1.getFront() << std::endl;


    CloseHandle(dataReady);
    CloseHandle(fileReady);
    CloseHandle(allFileReady);
    CloseHandle(addToBufferThread);
    CloseHandle(saveToFileThread);

    return 0;

}
