#pragma once
#ifdef QUEUE_EXPORTS
#define QUEUE_API __declspec(dllexport)
#else
#define QUEUE_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <mutex>
#include "Queue.h"

using namespace std;

class QUEUE_API SharedBuffer {
private:
    int* arr; // Динамический массив
    int capacity; // Емкость массива
    int front; // Индекс первого элемента в очереди
    int rear; // Индекс последнего элемента в очереди
    int count; // Количество элементов в очереди
    mutex mtx;
    condition_variable notFull;
    condition_variable notEmpty;
public:
    SharedBuffer(int bufferSize);
    ~SharedBuffer();
    void add(int item);
    void remove();
    void writeToFile(const char* fileName);
    void readFromFile(const char* fileName);
    void waitForEmpty();
    void waitForNotFull();
};
extern "C" __declspec(dllexport) void testQueue();

