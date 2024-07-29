#include <stdio.h>
#define N 5

void PrintArray(int  arr[]);

int main (void) 
{
    // Задание 1
    printf("Square matrix:\n");
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            printf("%3d", i * N + j + 1);
            if ((j + 1) % N == 0) 
            {
                printf("\n");
            }
        }
    }

    // Задание 2
    int arr[N] = {0};
    for (int i = 0; i < N; i++) // Заполняем массив от 1 до N
    {
        arr[i] = i + 1;
    }

    printf("\nOriginal array: ");
    PrintArray(arr);
    for (int i = 0, k = N - 1; i < k; i++, k--) 
    {
        // Меняем местами члены массива
        int temp = arr[i];
        arr[i] = arr[k];
        arr[k] = temp;
    }
    printf("Reversed array: ");
    PrintArray(arr);

    // Задание 3
    printf("\nQuad matrix of 0 and 1:\n");
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            if (i + j >= N - 1) 
            {
                printf("%3d", 1); 
            }
            else 
            {
                printf("%3d", 0);
            }
            
            if ((j + 1) % N == 0) 
            {
                printf("\n");
            }
        }
    }

    // Задание 4
    int snail[N][N] = {0};
    int top = 0; // Верхняя горизонталь матрицы
    int bottom = N - 1; // Нижняя горизонталь матрицы
    int left = 0; // Крайняя левая вертикаль матрицы
    int right = N - 1; // Крайняя правая вертикаль матрицы
    int value = 1; // Значение, записываемое в элемент матрицы

    while (top <= bottom && left <= right) 
    {
        for (int i = left; i <= right; i++) // Присваиваем числа по текущей верхней горизонтали слева направо
        {
            snail[top][i] = value++;
        }
        top++; // Сдвигаем верхнюю границу вниз

        for (int i = top; i <= bottom; i++) // Присваиваем числа по текущей крайней правой вертикали сверху вниз
        {
            snail[i][right] = value++;
        }
        right--; // Сдвигаем правую границу левее

        if (top <= bottom) // Если верхняя граница не зашла за нижнюю
        {
            for (int i = right; i >= left; i--) // Присваиваем числа по текущей нижней горизонтали справа налево
            {
                snail[bottom][i] = value++;
            }
            bottom--; // Сдвигаем нижнюю границу вверх
        }
        else 
        {
            break; // иначе завершаем цикл
        }

        if (left <= right) // Если левая граница не зашла за правую
        {
            for (int i = bottom; i >= top; i--) // Присваиваем числа по текущей крайней левой вертикали снизу вверх
            {
                snail[i][left] = value++;
            }
            left++; // Сдвигаем левую границу правее
        }
        else 
        {
            break; // иначе завершаем цикл
        }
        
    }

    printf("\nSnail matrix:\n");
    for (int i = 0; i < N; i++) // Выводим матрицу на экран
    {
        for (int j = 0; j < N; j++) 
        {
            printf("%3d", snail[i][j]);
            if ((j + 1) % N == 0) 
            {
                printf("\n");
            }
        }
    }

    return 0;
}

void PrintArray(int arr[])
{
    for (int i = 0; i < N; i++) 
    {
        printf("%3d", arr[i]);
    }
    printf("\n");


}