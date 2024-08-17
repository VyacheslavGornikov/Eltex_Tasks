#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MQUEUE_NAME "/my_mqueue"

int main (void) 
{
    mqd_t mqd;
    ssize_t num_read;
    struct mq_attr attr;
    unsigned int prio;
    char* buffer = NULL;

    mqd = mq_open(MQUEUE_NAME, O_RDWR);
    if (mqd == (mqd_t)-1) 
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mqd, &attr) == -1) 
    {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL) 
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    num_read = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
    if (num_read == -1) 
    {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }

    printf("Полученное от сервера сообщение: %s с приоритетом %u\n", buffer, prio);

    strncpy(buffer, "Hello!", attr.mq_msgsize);

    if (mq_send(mqd, buffer, attr.mq_msgsize, 0) == -1) 
    {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Серверу отправлено сообщение: %s с приоритетом %u\n", buffer, 0);

    if (mq_close(mqd) == -1) 
    {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}