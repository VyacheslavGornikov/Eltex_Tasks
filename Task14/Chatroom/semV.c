#include "semV.h"

/*
 * Инициализирует семафор значением 1
 *
 * _semid - идентификатор семафора
 * _sem_num - номер семафора
 */
void sem_init(int _semid, int _sem_num)
{
    if (semctl(_semid, _sem_num, SETVAL, 1) == -1)
    {
        err_exit("semctl");
    }
}

/*
 * Резервирует семафор, обозначает начало критической секции
 *
 * _semid - идентификатор семафора
 * _sem_num - номер семафора
 */
void sem_reserve (int _semid, int _sem_num) 
{    
    struct sembuf sop = {_sem_num, -1, 0};
    if (semop(_semid, &sop, 1) == -1) 
    {
        err_exit("reserve semop");
    }
}

/*
 * Высвобождает семафор, обозначает конец критической секции
 *
 * _semid - идентификатор семафора
 * _sem_num - номер семафора
 */
void sem_release (int _semid, int _sem_num) 
{
    struct sembuf sop = {_sem_num, 1, 0};
    if (semop(_semid, &sop, 1) == -1) 
    {
        err_exit("release semop");
    }
}