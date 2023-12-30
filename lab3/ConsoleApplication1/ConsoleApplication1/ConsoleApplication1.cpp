#include <iostream>
#include <Windows.h>

int main() {

    setlocale(LC_ALL, "ru");

    int M, N;
    std::cout << "Введите размерность M: ";
    std::cin >> M;
    std::cout << "Введите размерность N: ";
    std::cin >> N;

    // Выделение памяти
    int** array = (int**)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, M * sizeof(int*));
    for (int i = 0; i < M; i++) {
        array[i] = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, N * sizeof(int));
    }
    srand(time(NULL));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            array[i][j] = rand() % 100;
            std::cout << array[i][j] << ", ";
        }
        std::cout << "\n";
    }

    int* minValues = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, M * sizeof(int));
    // Обработка содержимого
    for (int i = 0; i < M; i++) {
        minValues[i] = array[i][0];
        for (int j = 1; j < N; j++) {
            if (array[i][j] < minValues[i])
                minValues[i] = array[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 1; j < N; j++) {
            if (array[i][j] < minValues[i])
                minValues[i] = array[i][j];
        }
    }

    // Поиск наибольшего значения среди минимальных
    int maxIndex = 0;
    for (int i = 1; i < M; i++) {
        if (minValues[i] > minValues[maxIndex])
            maxIndex = i;
    }

    std::cout << "Индекс элемента с наибольшим значением: " << maxIndex << std::endl;

    // Освобождение памяти
    for (int i = 0; i < M; i++) {
        HeapFree(GetProcessHeap(), 0, array[i]);
    }
    HeapFree(GetProcessHeap(), 0, array);
    HeapFree(GetProcessHeap(), 0, minValues);

    return 0;
}
