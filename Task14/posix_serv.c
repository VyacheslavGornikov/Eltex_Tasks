#include "common_posix.h"

int main (void) 
{
    int fd;
    char* addr;
    char* message = "Hi!";
    sem_t* sem; 

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0664);
    if (fd == -1) 
    {
        err_exit("shm_open");
    }

    if(ftruncate(fd, SHM_SIZE) == -1) 
    {
        err_exit("ftruncate");
    }

    addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) 
    {
        err_exit("mmap");
    }

    sem = sem_open(SEM_MAME, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) 
    {
        err_exit("sem_open");
    }

    memcpy(addr, message, strlen(message) + 1);    

    if (sem_wait(sem) == -1) 
    {
        err_exit("sem_wait");
    }

    printf("Сервер получил сообщение от клиента: %s\n", addr);    

    if (shm_unlink(SHM_NAME) == -1) 
    {
        err_exit("shm_unlink shm");
    }

    if (sem_close(sem) == -1) 
    {
        err_exit("sem_close");
    }

    if (sem_unlink(SEM_MAME) == -1) 
    {
        err_exit("sem_unlink sem");
    }

    exit(EXIT_SUCCESS);
}