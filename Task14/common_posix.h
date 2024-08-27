#ifndef _COMMON_POSIX_H_
#define _COMMON_POSIX_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_MAME "/shm_sem"
#define SHM_SIZE 10
#define err_exit(message) \
do { \
    printf("%s -> ", message); \
    printf("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
} while (0)

#endif