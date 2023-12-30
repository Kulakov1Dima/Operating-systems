#include <iostream>
#include <Windows.h>

int main() {

    setlocale(LC_ALL, "ru");

    int M, N;
    std::cout << "Введите размерность M: ";
    std::cin >> M;
    std::cout << "Введите размерность N: ";
    std::cin >> N;

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

    int** minValues = (int**)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, M * sizeof(int*));
    for (int i = 0; i < M; i++) {
        minValues[i] = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 3 * sizeof(int));
    }

    for (int i = 0; i < M; i++) {
        minValues[i][0] = array[i][0];
        for (int j = 1; j < N; j++) {
            if (array[i][j] < minValues[i][0])
                minValues[i][0] = array[i][j];
                minValues[i][1] = i;
                minValues[i][2] = j;
        }
    }

    // Поиск наибольшего значения среди минимальных
    int maxIndex = 0;
    for (int i = 1; i < M; i++) {
        if (minValues[i][0] > minValues[maxIndex][0])
            maxIndex = i;
    }

    std::cout << "Элемент с максимальным значением из минимальных: " << minValues[maxIndex][0] << std::endl;
    std::cout << "Строка: " << minValues[maxIndex][1] << std::endl;
    std::cout << "Столбец: " << minValues[maxIndex][2] << std::endl;

    // Освобождение памяти
    for (int i = 0; i < M; i++) {
        HeapFree(GetProcessHeap(), 0, minValues[i]);
        HeapFree(GetProcessHeap(), 0, array[i]);
    }
    HeapFree(GetProcessHeap(), 0, minValues);
    HeapFree(GetProcessHeap(), 0, array);

    return 0;
}
