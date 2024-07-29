#include <stdio.h>
#include <stdlib.h>

int* GetBinaryNumber (int value); // Возвращает число в двоичном представлении в виде массива
void DisplayBinaryNumber (const int* valuePtr); // Выводит число в двоичном представлении на экран
int GetUnitsCount (const int* valuePtr); // Возвращает число единиц в двоичном числе
void ReplaceThirdByte (int* number); // Заменяет третий байт числа на введенное число

int main(void) 
{
    int* binValuePtr = NULL; // указатель на 32х разрядное число(массив битов)
    int number = 0;

    printf("Enter integer value: "); // Вводим число с клавиатуры
    scanf("%d", &number);

    // Задание №1,2
    binValuePtr = GetBinaryNumber(number); // Получаем массив битов(число в двоичном представлении)
    if (binValuePtr != NULL) // Если указатель на массив не равен NULL
    {
        printf("%d = ", number);
        DisplayBinaryNumber(binValuePtr); // Выводит на экран число в двоичном виде
        
        // Задание №3
        if (number > 0) 
        {
            printf("Number of units = %d\n", GetUnitsCount(binValuePtr));
        }
        else 
        {
            printf("Number is not positive! Function GetUnitsCount will not be called!\n");
        }
        
    }

    // Задание №4
    if (number > 0) 
    {
        ReplaceThirdByte(&number);

        binValuePtr = GetBinaryNumber(number);
        if (binValuePtr != NULL) 
        {
            // Вывод число в двоичном виде после замены 3-его байта
            puts("Number with replaced third byte:");
            printf("%d = ", number);
            DisplayBinaryNumber(binValuePtr);
        }
    }
    else 
    {
        printf("Number is not positive! Function ReplaceThirdByte will not be called!\n");
    }
    

    return 0;
}

int* GetBinaryNumber (int value) 
{
    int* binValue = malloc(32 * sizeof(int)); // Массив с двоичным целым числом    
    
    for (int i = 31; i >= 0; i--) 
    {
        binValue[31 - i] = (value >> i) & 1;
    }                 

    return binValue;
}

void DisplayBinaryNumber (const int* valuePtr) 
{    
    for (int i = 1; i <= 32; i++) 
    {
        printf("%d", valuePtr[i - 1]);
        if (i % 8 == 0) 
        {
            putchar(' ');
        }
    }
    putchar('\n');
}

int GetUnitsCount (const int* valuePtr) 
{
    int counter = 0;

    for (int i = 0; i < 32; i++) 
    {
        if (valuePtr[i] == 1) 
        {
            counter++;
        }
    }

    return counter;
}

void ReplaceThirdByte (int* number) 
{
    int thirdByteNumber = 0;
    int tempNumber = 0;
    printf("Enter third byte value: ");
    scanf("%d", &thirdByteNumber);
    while (thirdByteNumber < 0 || thirdByteNumber > 255) 
    {
        printf("Incorrect value! Value must be from 0 to 255. Try again: ");
        scanf("%d", &thirdByteNumber);
    }
    tempNumber = (*number & ~(0xFF << 16)) | (thirdByteNumber << 16);
    *number = tempNumber;
}

