#ifndef _COMMON_H_
#define _COMMON_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define err_exit(message) \
do { \
    printf("%s -> ", message); \
    printf("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
} while (0)

#endif