#include "phonebook.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


/*
 * Функция выводит меню справочника
 */
void PrintMenu (void) 
{
    printf("Меню справочника:\n");
    printf("1) Добавить абонента\n");
    printf("2) Удалить абонента\n");
    printf("3) Найти абонента по имени\n");
    printf("4) Вывести справочник\n");
    printf("5) Выход\n");    
}

/* 
 * int AddAbonent (Abonent** abonents, int* size)

 * Функция AddAbonent добавляет абонента в конец телефонного справочника

 * Возвращает 1, если абонент успешно добавлен, в противном случае возвращает 0 

 * abonents - адрес указателя на начало массива структур типа Abonent
 * size - адрес переменной, хранящей размер массива структур типа Abonent 
 */
int AddAbonent (Abonent** abonents, int* size) 
{
    *abonents = realloc(*abonents, (*size + 1) * sizeof(Abonent));
    if (*abonents == NULL) 
    {
        printf("Ошибка выделения памяти! ");
        return 0;
    }    

    printf("Введите имя абонента: ");
    fgets((*abonents)[*size].name, NAME_SIZE, stdin);
    printf("Введите фамилию абонента: ");
    fgets((*abonents)[*size].secondName, NAME_SIZE, stdin);
    printf("Введите номер телефона: ");
    fgets((*abonents)[*size].telephone, TEL_SIZE, stdin);

    (*size)++;

    return 1;
}

/* 
 * int DeleteAbonent (Abonent** abonents, int* size, int index)
 *
 * Функция DeleteAbonent удаляет выбранного абонента из телефонного справочника
 *
 * Возвращает 1, если абонент успешно удален, в противном случае возвращает 0 
 *
 * abonents - адрес указателя на начало массива структур типа Abonent
 * size - адрес переменной, хранящей размер массива структур типа Abonent
 * index - номер по порядку удаляемого абонента, вводится пользователем 
 */
int DeleteAbonent (Abonent** abonents, int* size, int index) 
{
    char ch;
    PrintAbonentByIndex(*abonents, *size, index);
    printf("Вы действительно хотите удалить данного абонента (y/n)? ");
    getchar();
    ch = getchar();
    while (ch != 'y' && ch != 'n') 
    {
        printf("Неверный символ! Введите y или n: ");
        getchar();
        ch = getchar();        
    }

    if (ch == 'y') 
    {
        for (int i = index; i < *size - 1; i++) 
        {
            memcpy(&(*abonents)[i], &(*abonents)[i + 1], sizeof(Abonent)); 
        }        
        
        *abonents = realloc(*abonents, (*size - 1) * sizeof(Abonent));
        (*size)--;

        if (*abonents == NULL && *size != 0) 
        {
            printf("Ошибка выделения памяти! ");
            return 0;
        }               

        return 1;
    }
    else 
    {
        printf("Пользователь отменил операцию удаления! ");
        return 0;
    }
}

/* 
 * void PrintAbonentByIndex (Abonent* abonents, int size, int index)
 *
 * Функция PrintAbonentByIndex выводит на экран по индексу абонента из телефонного справочника
 *
 * abonents - указатель на начало массива структур типа Abonent
 * size - переменная, хранящей размер массива структур типа Abonent
 * index - индекс абонента 
 */
void PrintAbonentByIndex (Abonent* abonents, int size, int index) 
{
    if (index < 0 || index > size - 1) 
    {
        printf("Абонента с таким индексом не существует!\n");
    }
    else 
    {
        printf("\nАбонент №%d.\n", index + 1);
        printf("Имя: %s", abonents[index].name);
        printf("Фамилия: %s", abonents[index].secondName);
        printf("Телефон: %s", abonents[index].telephone);        
    }
}

/* 
 * void PrintAllAbonents (Abonent* abonents, int size)
 *
 * Функция PrintAllAbonents выводит на экран всех абонентов из телефонного справочника
 *
 * abonents - указатель на начало массива структур типа Abonent
 * size - переменная, хранящей размер массива структур типа Abonent
 */
void PrintAllAbonents (Abonent* abonents, int size) 
{
    printf("\n***Список абонентов***\n");
    for (int i = 0; i < size; i++) 
    {
        PrintAbonentByIndex(abonents, size, i);
    }
}

/* 
 * void FindAbonentsByName (Abonent* abonents, int size)
 *
 * Функция FindAbonentsByName выводит на экран всех абонентов с заданным именем
 *
 * abonents - указатель на начало массива структур типа Abonent
 * size - переменная, хранящей размер массива структур типа Abonent
 */
void FindAbonentsByName (Abonent* abonents, int size) 
{
    printf("Введите имя для поиска абонентов: ");
    char name[NAME_SIZE];
    fgets(name, NAME_SIZE, stdin);

    int counter = 0;
    printf("\nАбоненты с именем %s", name);

    for (int i = 0; i < size; i++) 
    {
        if (strcmp(abonents[i].name, name) == 0) 
        {
            PrintAbonentByIndex(abonents, size, i);
            counter++;
        }
    }

    if (counter == 0) 
    {
        printf("Совпадений не найдено!");
    }
}

