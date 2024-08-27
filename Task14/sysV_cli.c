#include "common_sysV.h"

void init_sem(int semid, int _sem_num);
void reserve_sem (int semid, int _sem_num);
void release_sem (int semid, int _sem_num);

int main (void) 
{
    key_t key_shm, key_sem;
    int shmid;
    int semid;
    char* addr;
    char* message = "Hello!";

    key_shm = ftok(F_PATH, 'A');
    if (key_shm == -1) 
    {
        err_exit("ftok shm");
    }

    key_sem = ftok(F_PATH, 'B');
    if (key_sem == -1) 
    {
        err_exit("ftok sem");
    }

    shmid = shmget(key_shm, SHM_SIZE, 0644);
    if (shmid == -1) 
    {
        err_exit("shmget");
    }

    addr = shmat(shmid, NULL, 0);
    if (addr == (void*) -1) 
    {
        err_exit("shmat");
    }

    semid = semget(key_sem, 1, 0644);
    if (semid == -1) 
    {
        err_exit("semget");
    } 

    printf("Клиент получил от сервера сообщение: %s\n", addr);

    memcpy(addr, message, strlen(message) + 1);
    release_sem(semid, 0);

    if (shmdt(addr) == -1) 
    {
        err_exit("shmdt");
    }   

    exit(EXIT_SUCCESS);
}

void init_sem(int semid, int _sem_num)
{
    if (semctl(semid, _sem_num, SETVAL, 0) == -1)
    {
        err_exit("semctl");
    }
}

void reserve_sem (int semid, int _sem_num) 
{
    struct sembuf sops = {0, -1, 0};
    if (semop(semid, &sops, 1) == -1) 
    {
        err_exit("reserve semop");
    }
}

void release_sem (int semid, int _sem_num) 
{
    struct sembuf sops = {0, 1, 0};
    if (semop(semid, &sops, 1) == -1) 
    {
        err_exit("release semop");
    }
}