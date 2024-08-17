#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define F_PATH "./Makefile"
#define BUF_SIZE 10
#define MSGS_TYPE 2
#define MSGR_TYPE 1

struct mbuf 
{
    long mtype;
    char mtext[BUF_SIZE];
};

int main (void) 
{
    key_t key;
    int msqid;
    struct mbuf message;    

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

    if (msgrcv(msqid, &message, BUF_SIZE, MSGR_TYPE, 0) < 0) 
    {
        perror("msgrcv error");
        exit(EXIT_FAILURE);
    }

    printf("Полученное от сервера сообщение: %s с приоритетом %ld\n", message.mtext, message.mtype);

    message.mtype = MSGS_TYPE;
    strncpy(message.mtext, "Hello!", BUF_SIZE);

    printf("Отправлено сообщение серверу: %s с приоритетом %ld\n", message.mtext, message.mtype);
    if (msgsnd(msqid, &message, BUF_SIZE, 0) < 0) 
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }   

    exit(EXIT_SUCCESS);
}