#ifndef _COMMON_SYSV_H_
#define _COMMON_SYSV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>

#define F_PATH "./Makefile"
#define SHM_SIZE 10

#define err_exit(message) \
do { \
    printf("%s -> ", message); \
    printf("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
} while (0)

#endif