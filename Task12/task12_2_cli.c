#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO_NAME "my_fifo"
#define BUF_SIZE 10

int main (void) 
{
    char buffer[BUF_SIZE];
    int fd;

    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) 
    {
        perror("Error opening FIFO bu client!\n");
        exit(EXIT_FAILURE);
    }

    if (read(fd, buffer, BUF_SIZE) == -1) 
    {
        perror("Error reading message by client!\n");
        exit(EXIT_FAILURE);
    }

    printf("Клиент принял сообщение: %s\n", buffer);

    if (close(fd) == -1) 
    {
        perror("Error closing file descriptor by client!\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}