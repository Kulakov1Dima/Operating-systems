#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

class __declspec(dllexport) Queue {
private:
    
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

#endif