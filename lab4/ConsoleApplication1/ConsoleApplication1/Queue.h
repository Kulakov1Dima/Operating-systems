#pragma once

using namespace std;

class Queue {
private:
    int* arr; // Динамический массив
    int capacity; // Емкость массива
    int front; // Индекс первого элемента в очереди
    int rear; // Индекс последнего элемента в очереди
    int count; // Количество элементов в очереди
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