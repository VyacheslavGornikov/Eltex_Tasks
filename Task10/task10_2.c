#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>

/* Дерево процессов:
 *
 *              Root
 *          /           \
 *       Child1        Child2
 *     /         \          \
 * Subchild1  Subchild2   Subchild3
 */

int main (void) 
{
    pid_t child1_pid; // pid процесса 1
    pid_t subchild1_pid; // pid процесса 3
    pid_t subchild2_pid; // pid процесса 4   
    pid_t child2_pid; // pid процесса 2
    pid_t subchild3_pid; // pid процесса 5    

    child1_pid = fork();
    if (child1_pid == 0) 
    {
        printf("Child1 pid = %d, Parent pid = %d\n", getpid(), getppid());
        
        subchild1_pid = fork();
        if (subchild1_pid == 0) 
        {
            printf("Subchild1 pid = %d, Parent pid = %d\n", getpid(), getppid());            
            exit(3); // завершение процесса 3
        }
        else if (subchild1_pid > 0)
        {
            subchild2_pid = fork();
            if (subchild2_pid == 0) 
            {
                printf("Subchild2 pid = %d, Parent pid = %d\n", getpid(), getppid());                
                exit(4); // завершение процесса 4
            }
            else if (subchild2_pid < 0) 
            {
                perror("Fork error in process 4\n");
                exit(EXIT_FAILURE);
            }
            // Процесс 1 ждет завершение дочерних процессов 3 и 4
            waitpid(subchild1_pid, NULL, 0);
            waitpid(subchild2_pid, NULL, 0);
        }
        else 
        {
            perror("Fork error in process 3\n");
            exit(EXIT_FAILURE);
        }

        exit(1); // завершение процесса 1
    }
    else if (child1_pid > 0)
    {
        printf("Root pid = %d\n", getpid());
        
        child2_pid = fork();
        if (child2_pid == 0) 
        {
            printf("Child2 pid = %d, Parent pid = %d\n", getpid(), getppid());
            
            subchild3_pid = fork();
            if (subchild3_pid == 0)
            {
                printf("Subchild3 pid = %d, Parent pid = %d\n", getpid(), getppid());                
                exit(5); // завершение процесса 5
            }
            else if (subchild3_pid < 0) 
            {
                perror("Fork error in process 5\n");
                exit(EXIT_FAILURE);
            }
            // Процесс 2 ждет завершения дочернего процесса 5
            waitpid(subchild3_pid, NULL, 0);
            exit(2); // завершение процесса 2
        }
        else if (child2_pid < 0) 
        {
            perror("Fork error in process 2\n");
            exit(EXIT_FAILURE);
        }
        // Корневой процесс ждет завершения дочерних процессов 1 и 2
        waitpid(child1_pid, NULL, 0);
        waitpid(child2_pid, NULL, 0);        
    }
    else 
    {
        perror("Fork error in process 1\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}