#ifndef _COMMON_H_
#define _COMMON_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <malloc.h>
//#include <time.h>

#define err_exit(message) \
do { \
    printf("%s -> ", message); \
    printf("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
} while (0)

#define ERROR_MESSAGE_NCURS(message) \
do { \
    clear(); \
    printw("%s ", message); \
    printw("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    getch(); \
    endwin(); \
    exit(EXIT_FAILURE); \
} while (0)
#endif