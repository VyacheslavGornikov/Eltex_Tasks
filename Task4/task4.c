#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

static int abonentsCounter = 0;

struct abonent 
{
    char name[20];
    char secondName[30];
    char telephone[12];
};

typedef struct abonent Abonent;
void PrintMenu (void);
void AddAbonent (Abonent* aPtr);
void PrintAbonents (const Abonent* aPtr);
void DeleteAbonent (Abonent* aPtr);
void SearchAbonent (Abonent* aPtr);

int main(void) 
{
    Abonent abonents[SIZE];

    for (int i = 0; i < SIZE; i++) 
    {
        strcpy(abonents[i].name, "0");
        strcpy(abonents[i].secondName, "0");
        strcpy(abonents[i].telephone, "0");
    }

    int isRunning = 1;

    while (isRunning) 
    {
        system("clear");
        PrintMenu();
        int choice;
        scanf("%d", &choice);
		getchar();

        switch (choice) 
        {
            case 1:
                if (abonentsCounter < 100) 
                {
                    AddAbonent(abonents);
                }
                else 
                {
                    printf("Справочник переполнен! Добавление невозможно!\n");
                }
                break;
            case 2:
                if (abonentsCounter != 0) 
                {
                    DeleteAbonent(abonents);
                }
                else 
                {
                    printf("Список абонентов пуст!\n"); 
                }                
                break;
            case 3:
                if (abonentsCounter != 0) 
                {
                    SearchAbonent(abonents);
                }
                else 
                {
                    printf("Список абонентов пуст!\n"); 
                }                  
                break;
            case 4:
                if (abonentsCounter != 0) 
                {
                    PrintAbonents(abonents);
                }
                else 
                {
                    printf("Список абонентов пуст!\n");
                }
                break;
            case 5:
                isRunning = 0;
                printf("Завершение работы программы...\n");
                break;
            default:
                printf("Неверный пункт меню! Попробуйте еще раз!\n");
                break;
        }
        printf("\nНажмите клавишу для продолжения(завершения),,,\n");
        getchar();        
    } 

    return 0;
}

void PrintMenu (void) 
{
    printf("Меню телефонного справочника:\n");
    printf("1) Добавить абонента\n");
    printf("2) Удалить абонента\n");
    printf("3) Поиск абонента по имени\n");
    printf("4) Вывод всех записей\n");
    printf("5) Выход\n");
    printf("Введите пункт меню: ");
}

void AddAbonent (Abonent* aPtr) 
{
    for (int i = 0; i < SIZE; i++) 
    {
        if (strcmp(aPtr[i].name, "0") == 0) 
        {
            printf("Абонент %d.\n", i + 1);
            printf("Введите имя абонента: ");
            gets(aPtr[i].name);
            printf("Введите фамилию абонента: ");
            gets(aPtr[i].secondName);
            printf("Введите номер телефона: ");
            gets(aPtr[i].telephone);
            abonentsCounter++;
            printf("Абонент успешно добавлен!\n");
            break;
        }
    }
}

void PrintAbonents (const Abonent* aPtr) 
{
    printf("\n***Список абонентов***\n");
    
    for (int i = 0; i < SIZE; i++) 
    {
        if (strcmp(aPtr[i].name, "0") != 0) 
        {
            printf("\nАбонент %d.\n", i + 1);
            printf("Имя: %s\n", aPtr[i].name);
            printf("Фамилия: %s\n", aPtr[i].secondName);
            printf("Телефон: %s\n", aPtr[i].telephone);                      
        }
    }    
}

void DeleteAbonent (Abonent* aPtr) 
{
    printf("Введите фамилию и имя абонента для удаления\n");
    char name[20] = "";
    printf("Введите имя: ");
    gets(name);
    char secondName[30];
    printf("Введите фамилия: ");
    gets(secondName);
    int isFound = 0;

    for (int i = 0; i < SIZE; i++) 
    {        
        if (strcmp(aPtr[i].name, name) == 0 && strcmp(aPtr[i].secondName, secondName) == 0) 
        {
            isFound = 1;
            printf("\nАбонент %d.\n", i + 1);
            printf("Имя: %s\n", aPtr[i].name);
            printf("Фамилия: %s\n", aPtr[i].secondName);
            printf("Телефон: %s\n", aPtr[i].telephone);

            strcpy(aPtr[i].name, "0");
            strcpy(aPtr[i].secondName, "0");
            strcpy(aPtr[i].telephone, "0");
            abonentsCounter--;
            printf("Успешно удален!\n");   
        }
    }
    if (!isFound) 
    {
        printf("Абонента с такими данными не существует!\n");
    }
}

void SearchAbonent (Abonent* aPtr) 
{
    char name[20] = "";
    printf("Введите имя для поиска абонентов: ");
    gets(name);
    printf("***Абоненты с именем <%s> ***\n", name);
    int counter = 0;

    for (int i = 0; i < SIZE; i++) 
    {
        if (strcmp(aPtr[i].name, name) == 0) 
        {
            printf("\nАбонент %d.\n", i + 1);
            printf("Имя: %s\n", aPtr[i].name);
            printf("Фамилия: %s\n", aPtr[i].secondName);
            printf("Телефон: %s\n", aPtr[i].telephone);
            counter++;
        }
    }

    if (counter == 0) 
    {
        printf("Абоненты с именем <%s> не найдены!\n", name);
    }
    
}