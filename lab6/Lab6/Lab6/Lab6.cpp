#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

int sizeBuffer = 10;

Queue myQueue(sizeBuffer);
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


DWORD WINAPI SaveToFileThread(LPVOID lpParam) {
    while (true) {
        WaitForSingleObject(dataReady, INFINITE);

        while (!buffer.isEmpty()) {
            int value = buffer.getFront();
            buffer.del();
            myQueue.add(value);

            myQueue.writeToFile("queue.txt");

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
    HANDLE saveToFileThread = CreateThread(NULL, 0, SaveToFileThread, NULL, 0, NULL);

    WaitForSingleObject(allFileReady, INFINITE);


    std::cout << "Первый элемент в очереди: " << myQueue.getFront() << std::endl;

    myQueue.writeToFile("queue.txt");

    Queue myQueue1(5);
    myQueue1.readFromFile("queue.txt");
    myQueue1.del();
    myQueue1.del();
    myQueue1.del();
    std::cout << "Первый элемент в очереди после удаления трёх элементов: " << myQueue1.getFront() << std::endl;


    CloseHandle(dataReady);
    CloseHandle(fileReady);
    CloseHandle(allFileReady);
    CloseHandle(addToBufferThread);
    CloseHandle(saveToFileThread);

    return 0;

}
