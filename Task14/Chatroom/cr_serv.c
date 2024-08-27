#include "common.h"
#include "chatroom.h"
#include "semV.h"

static int shmid;
static int semid;
static SharedData* chat;

void handle_sigint(int sig);
void sem_init(int _semid, int _sem_num);
void sem_reserve (int _semid, int _sem_num);
void sem_release (int _semid, int _sem_num);

int main() 
{    
    signal(SIGINT, handle_sigint);     
    
    shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | IPC_EXCL | 0644);
    if (shmid == -1) 
    {
        err_exit("shmget server");
    }

    chat = (SharedData*) shmat(shmid, NULL, 0);
    if (chat == (void*) -1) 
    {
        shmctl(shmid, IPC_RMID, NULL);
        err_exit("shmat server");
    }

    semid = semget(SEM_KEY, MAX_CLIENTS, IPC_CREAT | 0644);
    if (semid == -1) 
    {
        shmdt(chat);
        shmctl(shmid, IPC_RMID, NULL);
        err_exit("semget");
    }    

    chat->clients_num = 0;
    chat->messages_num = 0;

    printf("Сервер запущен и ожидает клиентов...\n");
    
    while (1)
    {
                          

    }       

    exit(EXIT_SUCCESS);
}

/* Обработчик сигнала прерывания программы SIGINT */
void handle_sigint(int sig) 
{
    printf("\nСервер завершает свою работу...\n");    
    shmdt(chat);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    exit(EXIT_SUCCESS);
}

void sem_init(int _semid, int _sem_num)
{
    if (semctl(_semid, _sem_num, SETVAL, 1) == -1)
    {
        err_exit("semctl");
    }
}

void sem_reserve (int _semid, int _sem_num) 
{
    //struct sembuf lock[2] = {{_sem_num, 0, 0}, {_sem_num, 1, 0}};
    struct sembuf sop = {_sem_num, -1, 0};
    if (semop(_semid, &sop, 1) == -1) 
    {
        err_exit("reserve semop");
    }
}

void sem_release (int _semid, int _sem_num) 
{
    struct sembuf sop = {_sem_num, 1, 0};
    if (semop(_semid, &sop, 1) == -1) 
    {
        err_exit("release semop");
    }
}
