#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "threadfunc.h"
#include "threaddefines.h"

Store stores[STORES_NUM]; /* Массив структур магазинов */

/*
 * Процедура инициализирует глобальный массив stores значениями от 1 до 10000
 * Примечание: функция использует rand() для генерации случайных чисел
 */
void init_stores(void) 
{
    for(int i = 0; i < STORES_NUM; i++) 
    {
        stores[i].capacity = rand() % MAX_STORE_CAPACITY + 1;
        stores[i].is_busy = 0;
    }
}

/*
 * Функция для запуска потока покупателя (customer)
 * Примечание: функция использует rand() для генерации случайных чисел
 *
 * customer_num - указатель на адрес переменной, содержащей порядковый номер потока покупателя
 */
void* customer_func (void* customer_number) 
{
    int* customer_num = (int*) customer_number;
    int need = rand() % MAX_CUSTOMER_NEED + 1;    

    while (need > 0) 
    {        
        for (int i = 0; i < STORES_NUM; i++) 
        {
            if (stores[i].is_busy == 0  && stores[i].capacity != 0) 
            {                
                stores[i].is_busy = 1; 
                int taken = (stores[i].capacity < need) ? stores[i].capacity : need;
                printf("Я покупатель №%d, моя потребность равна %d, я зашел в магазин №%d, там лежит %d товаров,"
                "\nя взял %d и теперь моя потребность %d, в магазине осталось %d товаров\n\n", 
                *customer_num + 1, need, i + 1, stores[i].capacity, taken, need - taken, stores[i].capacity - taken);
                need -= taken;
                stores[i].capacity -= taken;
                stores[i].is_busy = 0;
                break;
            }
        }       
        
        sleep(2);
    }

    printf("***Я покупатель №%d и я насытил свою потребность!!!***\n\n", *customer_num + 1);
    return NULL;
}

/*
 * Функция для запуска потока погрузчика (loader)
 * Примечание: функция использует rand() для генерации случайных чисел
 */
void* loader_func (void*) 
{    
    while (1) 
    {
        int i = rand() % STORES_NUM;
        while (stores[i].is_busy != 0) 
        {
            i = rand() % STORES_NUM;
        }
        printf("Я погрузчик и я зашел в магазин %d, там было %d товаров, я погрузил %d, теперь там %d товаров\n\n",
        i + 1, stores[i].capacity, LOADER_CAPACITY, stores[i].capacity + LOADER_CAPACITY);
        stores[i].capacity += LOADER_CAPACITY;
        sleep(1);
    }
    
    return NULL;
}