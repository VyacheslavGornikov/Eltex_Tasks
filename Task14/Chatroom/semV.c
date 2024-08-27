#include "semV.h"

void sem_init(int _semid, int _sem_num)
{
    if (semctl(_semid, _sem_num, SETVAL, 0) == -1)
    {
        err_exit("semctl");
    }
}

void sem_reserve (int _semid, int _sem_num) 
{
    struct sembuf lock[2] = {{_sem_num, 0, 0}, {_sem_num, 1, 0}};
    if (semop(_semid, lock, 2) == -1) 
    {
        err_exit("reserve semop");
    }
}

void sem_release (int _semid, int _sem_num) 
{
     struct sembuf unlock = {_sem_num, -1, 0};
    if (semop(_semid, &unlock, 1) == -1) 
    {
        err_exit("release semop");
    }
}