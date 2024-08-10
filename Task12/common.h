#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#define ERROR_MESSAGE(message) \
do { \
    clear(); \
    printw("%s ", message); \
    printw("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    getch(); \
    endwin(); \
    exit(EXIT_FAILURE); \
} while (0)
#endif