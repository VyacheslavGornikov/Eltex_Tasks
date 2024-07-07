#ifndef _PHONEBOOK_H_
#define _PHONEBOOK_H_

#define MAX_LIST_SIZE 100
#define NAME_SIZE 40
#define TEL_SIZE 15

typedef struct abonent
{
    char name[NAME_SIZE];
    char secondName[NAME_SIZE];
    char telephone[TEL_SIZE];
} Abonent;

void PrintMenu (void);
int AddAbonent (Abonent** abonents, int* size);
int DeleteAbonent (Abonent** abonents, int* size, int index);
void PrintAbonentByIndex (Abonent* abonents, int size, int index);
void PrintAllAbonents (Abonent* abonents, int size);
void FindAbonentsByName (Abonent* abonents, int size);

#endif