#pragma once
class Queue {
private:
    int* arr; 
    int capacity; 
    int front; 
    int rear; 
    int count;
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