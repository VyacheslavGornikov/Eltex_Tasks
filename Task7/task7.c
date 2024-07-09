#include <stdio.h>
#include <stdlib.h>

void PrintCalcMenu (void);

int main (void) 
{
    int isRunning = 1;

    while (isRunning)
    {
        system("clear");
        PrintCalcMenu();
        printf("Выберите операцию: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        int num1, num2;
        int result;

        switch (choice)
        {
            case 1:
                printf("Введите первое число: ");
                scanf("%d", &num1);
                printf("Введите второе число: ");
                scanf("%d", &num2);
                getchar();
                result = Add(num1, num2);
                printf("%d + %d = %d\n", num1, num2, result);
                break;
            case 2:
                printf("Введите первое число: ");
                scanf("%d", &num1);
                printf("Введите второе число: ");
                scanf("%d", &num2);
                getchar();
                result = Subtract(num1, num2);
                printf("%d - %d = %d\n", num1, num2, result);
                break;
            case 3:
                printf("Введите первое число: ");
                scanf("%d", &num1);
                printf("Введите второе число: ");
                scanf("%d", &num2);
                getchar();
                result = Multiply(num1, num2);
                printf("%d * %d = %d\n", num1, num2, result);
                break;
            case 4:
                printf("Введите первое число: ");
                scanf("%d", &num1);
                printf("Введите второе число: ");
                scanf("%d", &num2);
                getchar();
                result = Divide(num1, num2);
                printf("%d / %d = %d\n", num1, num2, result);
                break;
            case 5:
                isRunning = 0;
                printf("Завершение работы калькулятора...\n");                             
                break;
            
            default:
                printf("Неверный пункт меню! Повторите ввод!\n");
                break;
        }
        
        printf("Нажмите любую клавишу для продолжения (или завершения) работы калькулятора...\n");
        getchar();
    }
}

void PrintCalcMenu (void) 
{
    printf("Меню операций калькулятора:\n");
    printf("1) Сложение\n");
    printf("2) Вычитание\n");
    printf("3) Умножение\n");
    printf("4) Деление\n");
    printf("5) Выход\n");
}