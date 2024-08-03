#define CORES 4
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void * thread_calc (void * num) 
{
    long* number = (long *) num;    

    for (int i = 0; i < 25000000; i++) 
    {
        pthread_mutex_lock(&m1);
        (*number)++;
        pthread_mutex_unlock(&m1);
    }

    return NULL;
}

int main (void) 
{
    int s;
    int* res;
    long var = 0;
    //int thread_num[CORES] = {0};
    pthread_t thread[CORES];

    for (int i = 0; i < CORES; i++) 
    {        
        s = pthread_create(&thread[i], NULL, thread_calc, (void*) &var);
        if (s != 0) 
        {
            perror("Error creating thread!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CORES; i++) 
    {
        s = pthread_join(thread[i], (void**) &res);
        if (s != 0) 
        {
            perror("Error thread join!!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("var = %ld\n", var);
    exit(EXIT_SUCCESS);
}