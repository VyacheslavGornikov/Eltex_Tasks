#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define MQUEUE_NAME "/my_mqueue"
#define MAX_MSG 10
#define MSG_SIZE 10

int main (void) 
{
    mqd_t mqd;
    pid_t child_pid;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = MSG_SIZE;
    ssize_t num_read;
    unsigned int prio = 1;

    char buffer[MSG_SIZE] = "Hi!";

    mqd = mq_open(MQUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mqd == (mqd_t)-1) 
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }   

    if (mq_send(mqd, buffer, MSG_SIZE, prio) == -1) 
    {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Отправлено сообщение клиенту: %s с приоритетом %u\n", buffer, prio);
    
    child_pid = fork();
    if (child_pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0) 
    {
        execl("posix_cli", "posix_cli", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else if (child_pid > 0) 
    {
        wait(NULL);
        num_read = mq_receive(mqd, buffer, MSG_SIZE, &prio);
        if (num_read == -1) 
        {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
    }   

    printf("Полученное от клиента сообщение: %s с приоритетом %u\n", buffer, prio); 

    if (mq_close(mqd) == -1) 
    {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    if (mq_unlink(MQUEUE_NAME) == -1) 
    {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }
     
    exit(EXIT_SUCCESS);
}