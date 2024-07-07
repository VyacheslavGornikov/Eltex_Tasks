#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "phonebook.h"

/*
 * Функция main реализует работу телефонного справочника
 *
 * abonents - указатель на начало массива структур типа Abonent
 * index - вводимое пользоателем значение номера абонента в справочнике
 * isRunning - переменная отвечает за работу цикла, при завершении программы равна 0
 * choice - выбор пункта меню, вводится пользоателем
 */
int main (void) 
{
    Abonent* abonents = NULL;
    int size = 0;
    int index;
    int isRunning = 1;

    while (isRunning)
    {
        system("clear");
        PrintMenu();
        printf("Введите пункт меню: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                if (size < MAX_LIST_SIZE) 
                {
                    if (AddAbonent(&abonents, &size)) 
                    {
                        printf("Абонент был успешно добавлен.");
                    }
                    else 
                    {
                        printf("Абонент не будет добавлен!");
                    }
                }
                else 
                {
                    printf("Справочник переполнен!!!");
                }
                break;
            case 2:
                if (size > 0) 
                {
                    printf("Введите индекс абонента для удаления (1 - %d): ", size);
                    scanf ("%d", &index);
                    while (index < 1 || index > size) 
                    {
                        printf("Индекс должен принимать значения от 1 до %d! Повторите ввод: ", size);
                        scanf ("%d", &index);
                    }
                    //getchar();
                    if (DeleteAbonent(&abonents, &size, index - 1)) 
                    {
                        printf("Абонент был успешно удален.");
                    }
                    else 
                    {
                        printf ("Абонент не будет удален!");
                    }
                    getchar();
                }
                else 
                {
                    printf("Справочник пуст! Удаление невозможно!");
                }
                break;
            case 3:
                if (size > 0) 
                {
                    FindAbonentsByName(abonents, size);
                }
                else 
                {
                    printf("Справочник пуст!");
                }
                break;
            case 4:
                if (size > 0) 
                {
                    PrintAllAbonents(abonents, size);
                }
                else 
                {
                    printf("Справочник пуст!");
                }
                break;
            case 5:
                isRunning = 0;
                printf("Завершение работы программы...");
                if (abonents != NULL) 
                {
                    free (abonents);
                }                
                break;
            
            default:
                printf("Неверный пункт меню! Повторите ввод!\n");
                break;
        }

        printf("\nНажмите любую клавишу для продолжения (или завершения) работы программы...\n");
        getchar();
    }
    
    return 0;

}

