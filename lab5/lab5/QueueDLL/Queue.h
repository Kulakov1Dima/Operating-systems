#pragma once
#ifdef QUEUE_EXPORTS
#define QUEUE_API __declspec(dllexport)
#else
#define QUEUE_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>

using namespace std;

class QUEUE_API Queue {
private:
    int* arr; // ������������ ������
    int capacity; // ������� �������
    int front; // ������ ������� �������� � �������
    int rear; // ������ ���������� �������� � �������
    int count; // ���������� ��������� � �������
public:
    Queue(int size);
    ~Queue();
    void add(int item);
    bool isFull();
    bool isEmpty();
    void del();
    int getFront();
    void writeToFile(const char* fileName);
    void readFromFile(const char* fileName);
};
extern "C" __declspec(dllexport) void testQueue();

