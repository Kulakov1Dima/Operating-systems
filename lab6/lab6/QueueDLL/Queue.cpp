#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include "pch.h"
#include "Queue.h"
#include <condition_variable>


SharedBuffer::SharedBuffer(int bufferSize) : capacity(bufferSize), front(0), rear(-1), count(0) {
    arr = new int[capacity];
}

SharedBuffer::~SharedBuffer() {
    delete[] arr;
}

void SharedBuffer::add(int item) {
    std::unique_lock<std::mutex> lock(mtx);
    notFull.wait(lock, [this]() { return count < capacity; });

    rear = (rear + 1) % capacity;
    arr[rear] = item;
    count++;

    lock.unlock();
    notEmpty.notify_one();
}

void SharedBuffer::remove() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [this]() { return count > 0; });

    int item = arr[front];
    front = (front + 1) % capacity;
    count--;

    lock.unlock();
    notFull.notify_one();

    std::cout << "Потребитель получил: " << item << std::endl;
}

void SharedBuffer::writeToFile(const char* fileName) {
    std::unique_lock<std::mutex> lock(mtx);

    HANDLE masFile = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (masFile == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка открытия файла для записи!" << std::endl;
        return;
    }

    DWORD bytesWritten;
    WriteFile(masFile, &capacity, sizeof(capacity), &bytesWritten, NULL);
    WriteFile(masFile, &front, sizeof(front), &bytesWritten, NULL);
    WriteFile(masFile, &rear, sizeof(rear), &bytesWritten, NULL);
    WriteFile(masFile, &count, sizeof(count), &bytesWritten, NULL);

    WriteFile(masFile, arr, capacity * sizeof(int), &bytesWritten, NULL);

    CloseHandle(masFile);
}

void SharedBuffer::readFromFile(const char* fileName) {
    unique_lock<std::mutex> lock(mtx);

    HANDLE hFile = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка открытия файла для чтения!" << std::endl;
        return;
    }

    DWORD bytesRead;
    ReadFile(hFile, &capacity, sizeof(capacity), &bytesRead, NULL);
    ReadFile(hFile, &front, sizeof(front), &bytesRead, NULL);
    ReadFile(hFile, &rear, sizeof(rear), &bytesRead, NULL);
    ReadFile(hFile, &count, sizeof(count), &bytesRead, NULL);

    delete[] arr;
    arr = new int[capacity];
    ReadFile(hFile, arr, capacity * sizeof(int), &bytesRead, NULL);

    CloseHandle(hFile);
}

void SharedBuffer::waitForEmpty() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [this]() { return count > 0; });
}

void SharedBuffer::waitForNotFull() {
    std::unique_lock<std::mutex> lock(mtx);
    notFull.wait(lock, [this]() { return count < capacity; });
}

class ProducerConsumer {

public:
    SharedBuffer* sharedBuffer;
    ProducerConsumer(int bufferSize) {
        sharedBuffer = new SharedBuffer(bufferSize);
    }

    ~ProducerConsumer() {
        delete sharedBuffer;
    }

    void produce(int item) {
        sharedBuffer->add(item);
    }

    void consume() {
        sharedBuffer->remove();
    }
};

extern "C" __declspec(dllexport) void testQueue() {
    ProducerConsumer producerConsumer(5);

    thread producer2(&ProducerConsumer::produce, &producerConsumer, 32);
    thread producer3(&ProducerConsumer::produce, &producerConsumer, 9);
    thread producer4(&ProducerConsumer::produce, &producerConsumer, 94);
    thread consumer1(&ProducerConsumer::consume, &producerConsumer);
    thread consumer2(&ProducerConsumer::consume, &producerConsumer);
    thread consumer3(&ProducerConsumer::consume, &producerConsumer);

    producer2.join();
    producer3.join();
    producer4.join();

    // Добавим небольшую задержку перед завершением потоков
    this_thread::sleep_for(std::chrono::milliseconds(500));

    consumer1.join();
    consumer2.join();
    consumer3.join();

    producerConsumer.sharedBuffer->writeToFile("test.txt");
    SharedBuffer newBuffer(5);
    newBuffer.readFromFile("test.txt");
}