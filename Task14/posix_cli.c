#include "common_posix.h"

int main (void) 
{
    int fd;
    char* addr;
    char* message = "Hello!";
    sem_t* sem;

    fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) 
    {
        err_exit("shm_open");
    }
    
    addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) 
    {
        err_exit("mmap");
    }

    sem = sem_open(SEM_MAME, 0);
    if (sem == SEM_FAILED) 
    {
        err_exit("sem_open");
    }    

    printf("Клиент получил от сервера сообщение: %s\n", addr);
    
    memcpy(addr, message, strlen(message) + 1);

    if (sem_post(sem) == -1) 
    {
        err_exit("sem_post");
    }

    if (sem_close(sem) == -1) 
    {
        err_exit("sem_close");
    }

    exit(EXIT_SUCCESS);
}