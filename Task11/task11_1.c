#define N 5
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void * thread_call (void * num) 
{
    int* n = (int *) num;
    pthread_t tid = pthread_self();

    printf("Thread number %d with tid = %ld\n", *n, tid);

    return NULL;
}

int main (void) 
{
    int s;
    int* res;
    int thread_num[N] = {0};
    pthread_t thread[N];

    for (int i = 0; i < N; i++) 
    {
        thread_num[i] = i;
        s = pthread_create(&thread[i], NULL, thread_call, (void*) &thread_num[i]);
        if (s != 0) 
        {
            perror("Error creating thread!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; i++) 
    {
        s = pthread_join(thread[i], (void**) &res);
        if (s != 0) 
        {
            perror("Error thread join!!\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}