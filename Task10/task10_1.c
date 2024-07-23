#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>

int main (void) 
{
    pid_t child_pid;
    int status;

    child_pid = fork();
    if (child_pid == 0) 
    {
        printf("Child pid = %d\n", getpid());
        exit(10);
    }
    else if (child_pid > 0) 
    {
        printf("Parent pid = %d\n", getpid());
        wait(&status);
        printf("Child status = %d\n", WEXITSTATUS(status));
    }
    else 
    {
        perror("Fork error\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}