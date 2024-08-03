#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "threadfunc.h"
#include "threaddefines.h"

int main (void) 
{
    int s; /* Переменная-обработчик вызова поточных функций */        
    
    pthread_t customer_threads[CUSTOMERS_NUM]; /* Массив tid потоков покупателей */
    int customer_num[CUSTOMERS_NUM]; /* Массив порядковых номеров потоков покупателей */
    pthread_t loader_thread; /* tid потока загрузчика */

    srand(time(NULL));
    init_stores();

    s = pthread_create(&loader_thread, NULL, loader_func, NULL);

    for (int i = 0; i < CUSTOMERS_NUM; i++) 
    {
        customer_num[i] = i;        
        s = pthread_create(&customer_threads[i], NULL, customer_func, (void*) &customer_num[i]);
        if (s != 0) 
        {
            perror("Error creating thread!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CUSTOMERS_NUM; i++) 
    {
        s = pthread_join(customer_threads[i], NULL);
        if (s != 0) 
        {
            perror("Error customer_thread join!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Все покупатели закрыли свои потребности!!!\n");

    /* Отмена работы потока погрузчика и его присоединение */
    s = pthread_cancel(loader_thread);
    if (s != 0) 
    {
        perror("Error canceling thread!\n");
        exit(EXIT_FAILURE);
    }
    s = pthread_join(loader_thread, NULL);
    if (s != 0) 
    {
        perror("Error loader_thread join!\n");
        exit(EXIT_FAILURE);
    }   
    printf("Я погрузчик и я завершил свою работу!!!\n");

    exit(EXIT_SUCCESS);
}


