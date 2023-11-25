﻿#include <iostream>
#include <Windows.h>
#include "..\QueueDLL\Queue.h"

int main() {
    setlocale(LC_ALL, "Russian");

    HINSTANCE hDLL = LoadLibrary(L"QueueDLL.dll");
    if (hDLL != NULL) {
        typedef void (*testQueue)();
        testQueue testFunction = (testQueue)GetProcAddress(hDLL, "testQueue");

        if (testFunction != NULL) {
            testFunction();
        }
        else {
            std::cout << "Функция не найдена";
        }
    }
    else {
        std::cout << "Библиотека не найдена";
    }

    /*
        Queue queue(5);

        queue.add(89);
        queue.add(32);
        queue.add(9);
        queue.add(4);
        queue.add(107);

        std::cout << "Первый элемент в очереди: " << queue.getFront() << std::endl;

        queue.writeToFile("queue.txt");

        Queue queue1(5);
        queue1.readFromFile("queue.txt");
        queue1.del();
        queue1.del();
        std::cout << "Первый элемент в очереди после удаления двух элементов: " << queue1.getFront() << std::endl;
        */
    return 0;
}
