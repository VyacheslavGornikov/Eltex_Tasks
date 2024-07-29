#include <stdio.h>
#include <string.h>
#define SIZE 50

void DisplayBits (unsigned value);
const char* FindSubString (const char* str, const char* substr);

int main(void) 
{
    // Задание 1
    unsigned number = 0;
    unsigned thirdByte = 0;

    printf("Enter positive integer number: ");
    scanf("%d", &number);
    while (number < 0)
    {
        printf("Number must be positive! Try again: ");
        scanf("%d", &number);
    }
    unsigned char *ptr = (unsigned char*)&number; // Указываем на первый байт числа number
    DisplayBits(number); // Выводим число по битам от старшего к младшему (слева направо)

    printf("Enter third byte value from 0 to 255: ");
    scanf("%d", &thirdByte);
    while (thirdByte < 0 || thirdByte > 255) 
    {
        printf("Incorrect value! Value must be from 0 to 255. Try again: ");
        scanf("%d", &thirdByte);
    }
    DisplayBits(thirdByte);
    ptr += 2; // Перемещаем указатель на третий байт числа
    *ptr = (unsigned char)thirdByte; // Записываем в 3-ий байт введенное с клавиатуры число
    printf("Number with replaced third byte:\n");
    DisplayBits(number);
    printf("\n");

    // Задание 2
    float x = 5.0;
    printf("x = %f, ", x);
    float y = 6.0;
    printf("y = %f\n", y);
    float *xp = &y; // TODO: отредактируйте эту строку, и только правую часть уравнения
    float *yp = &y;
    printf("Результат: %f\n", *xp + *yp);

    // Задание 3
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *aPtr = arr;
    printf("\nPrinted array: ");
    for (int i = 0; i < 10; i++) 
    {
        printf("%d ", *(aPtr + i));
    }
    printf("\n\n");
    getchar(); // чтобы поток ввода не принимал \n при вводе строки

    // Задание 4
    char inputStr[SIZE] = "";
    char subStr[SIZE] = "";

    printf("Enter string: ");
    fgets(inputStr, SIZE, stdin); // строка заканчивается символом \n при вводе
    
    printf("Enter substring: ");
    fgets(subStr, SIZE, stdin);
    const char* subStrPtr = NULL;
    subStrPtr = FindSubString(inputStr, subStr); // Аналог функции strstr
    if (subStrPtr != NULL) 
    {
        printf ("Substring: %s", subStrPtr);
    }
    else 
    {
        puts("Substring was not found");
    }

    return 0;
}

void DisplayBits (unsigned value) 
{
    printf("%d = ", value);
    for (int i = 32; i >= 1; i--) 
    {
        if (i % 8 == 0) 
        {
            putchar(' ');
        }
        printf("%d", (value >> (i - 1)) & 1);
        
    }
    printf("\n");
}

const char* FindSubString (const char* str, const char* substr) 
{
    const char* subStrPtr = NULL; // Указатель на начало подстроки
    for (str; *str != '\n'; str++) 
    {
        if (*str == *substr) // Если символ строки совпадает с символом подстроки, начинаем посимвольное сравнение
        {
            subStrPtr = str;
            int isFound = 1; // Найдена ли подстрока? - 1 -да, 0 - нет
            for (const char* startStr = str, *startSubStr = substr; *startStr != '\n' && *startSubStr != '\n'; startStr++, startSubStr++) 
            {
                if (*startStr != *startSubStr) // если символы не совпали
                {
                    subStrPtr = NULL;
                    isFound = 0;
                    break;
                }
            }
            if (isFound) // если все символы совпали, если нет - идем по циклу дальше
            {
                return subStrPtr;
            }            
        }
    }
    return NULL;
}