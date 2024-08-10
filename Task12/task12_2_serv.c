#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "my_fifo"

int main (void) 
{
    const char* message = "Hi!";
    int fd;

    if (mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) 
    {
        perror("Error creating FIFO!\n");
        exit(EXIT_FAILURE);
    }

    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) 
    {
        perror("Error opening FIFO by server!\n");
        exit(EXIT_FAILURE);
    }

    if (write(fd, message, strlen(message) + 1) == -1) 
    {
        perror("Error writing message to FIFO by server!\n");
        exit(EXIT_FAILURE);
    }

    if(close(fd) == -1) 
    {
        perror("Error closing file descriptor by server!\n");
        exit(EXIT_FAILURE);
    }

    if (unlink(FIFO_NAME) == -1) 
    {
        perror("Error deleting FIFO!\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}