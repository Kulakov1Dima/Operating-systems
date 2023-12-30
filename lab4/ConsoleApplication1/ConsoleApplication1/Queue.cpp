#include "Queue.h"
#include <iostream>
#include <windows.h>

using namespace std;


Queue::Queue(int size)
{
    arr = new int[size];
    capacity = size;
    front = 0;
    rear = -1;
    count = 0;
}

Queue::~Queue() {
    delete[] arr;
}

void Queue::add(int item) {
    if (isFull()) {
        cout << "Очередь полна!" << std::endl;
        return;
    }

    rear = (rear + 1) % capacity;
    arr[rear] = item;
    count++;
}

bool Queue::isFull() {
    return count == capacity;
}

bool Queue::isEmpty() {
    return count == 0;
}

void Queue::del() {
    if (isEmpty()) {
        cout << "Очередь пуста!" << endl;
        return;
    }

    front = (front + 1) % capacity;
    count--;
}

int Queue::getFront() {
    if (isEmpty()) {
        cout << "Очередь пуста!" << endl;
        return -1;
    }
    return arr[front];
}

void Queue::writeToFile(const char* fileName) {
    HANDLE masFile = CreateFile((LPCWSTR)fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (masFile == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    WriteFile(masFile, &capacity, sizeof(capacity), NULL, NULL);
    WriteFile(masFile, &front, sizeof(front), NULL, NULL);
    WriteFile(masFile, &rear, sizeof(rear), NULL, NULL);
    WriteFile(masFile, &count, sizeof(count), NULL, NULL);

    WriteFile(masFile, arr, capacity * sizeof(int), NULL, NULL);

    CloseHandle(masFile);
}

void Queue::readFromFile(const char* fileName) {
    HANDLE hFile = CreateFile((LPCWSTR)fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка открытия файла для чтения!" << std::endl;
        return;
    }

    ReadFile(hFile, &capacity, sizeof(capacity), NULL, NULL);
    ReadFile(hFile, &front, sizeof(front), NULL, NULL);
    ReadFile(hFile, &rear, sizeof(rear), NULL, NULL);
    ReadFile(hFile, &count, sizeof(count), NULL, NULL);

    delete[] arr;
    arr = new int[capacity];
    ReadFile(hFile, arr, capacity * sizeof(int), NULL, NULL);

    CloseHandle(hFile);
}