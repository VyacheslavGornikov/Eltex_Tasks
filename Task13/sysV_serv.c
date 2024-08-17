#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define F_PATH "./Makefile"
#define BUF_SIZE 10
#define MSGS_TYPE 1
#define MSGR_TYPE 2

struct mbuf 
{
    long mtype;
    char mtext[BUF_SIZE];
};

int main(void) 
{
    key_t key;
    int msqid;
    struct mbuf message;
    message.mtype = MSGS_TYPE;
    strncpy(message.mtext, "Hi!", BUF_SIZE);

    key = ftok(F_PATH, 'A');
    if (key < 0) 
    {
        perror("Key error");
        exit(EXIT_FAILURE);
    }

    msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid < 0) 
    {
        perror("msgget error");
        exit(EXIT_FAILURE);
    }

    printf("Отправлено сообщение клиенту: %s с приоритетом %ld\n", message.mtext, message.mtype);
    if(msgsnd(msqid, &message, BUF_SIZE, 0) < 0) 
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    if(msgrcv(msqid, &message, BUF_SIZE, MSGR_TYPE, 0) < 0) 
    {
        perror("msgrcv error");
        exit(EXIT_FAILURE);
    }

    printf("Полученное от клиента сообщение: %s с приоритетом %ld\n", message.mtext, message.mtype);

    if (msgctl(msqid, IPC_RMID, NULL) < 0) 
    {
        perror("msgctl error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
