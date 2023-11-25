#include <stdio.h>
#include <corecrt_malloc.h>


//Вариант № 6

void sort_zerro(int mas[], int size) {
    int* p = mas;

    for (int i = 0; i < size; i++) {
        if (mas[i] != 0) {
            *p = mas[i];
            p++;
        }
    }

    for (int i = p - mas; i < size; i++) {
        *p = 0;
        p++;
    }
}

int main()
{
	int mas[10] = { 0, 40, 34, 0, 78, 9, 0, 9, 5, 0 };
    
    sort_zerro(mas, sizeof(mas) / sizeof(mas[0]));

    for (int i = 0; i < sizeof(mas) / sizeof(mas[0]); i++) {
        printf("%d ", mas[i]);
    }

}

