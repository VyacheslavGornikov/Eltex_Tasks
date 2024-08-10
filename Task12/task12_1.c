#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 10

int main (void) 
{
    int pfd[2];
    pid_t child_pid;
    const char* message = "Hi!";    
    
    if (pipe(pfd) < 0) 
    {
        perror("pipe error!\n");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();
    if (child_pid < 0) 
    {
        perror("fork error!\n");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0) 
    {
        if (close(pfd[1]) < 0) 
        {
            perror("pfd[1] close error!\n");
            exit(EXIT_FAILURE);
        }
        char buffer[BUF_SIZE];
        
        if (read(pfd[0], buffer, BUF_SIZE) < 0) 
        {
            perror("pfd[0] read error!\n");
            exit(EXIT_FAILURE);
        }

        printf("Дочерний процесс получил сообщение: %s\n", buffer);
        
        if (close(pfd[0]) < 0) 
        {
            perror("pfd[0] close error!\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else 
    {
        if (close(pfd[0]) < 0) 
        {
            perror("pfd[0] close error!\n");
            exit(EXIT_FAILURE);
        }

        if (write(pfd[1], message, strlen(message) + 1) < 0) 
        {
            perror("pfd[1] write error!\n");
            exit(EXIT_FAILURE);
        }

        if (close(pfd[1]) < 0) 
        {
            perror("pfd[1] close error!\n");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}