#include "common.h"
#include "chatroom.h"


static int shmid;
static int semid;
static SharedData* chat;

void handle_sigint(int sig);

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
    
    while (1);          

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


