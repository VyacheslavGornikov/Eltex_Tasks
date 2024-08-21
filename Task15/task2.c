#define _GNU_SOURCE
//#define  _POSIX_C_SOURCE  200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define err_exit(message) \
do { \
    printf("%s -> ", message); \
    printf("%s, %s : %d\n", strerror(errno), __FILE__,__LINE__); \
    exit(EXIT_FAILURE); \
} while (0)

int main (void) 
{       
    int ret;
    int sig_num;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);    
    ret = sigprocmask(SIG_BLOCK, &mask, NULL);       
    
    if (ret < 0) 
    {
        err_exit("sigprocmask");
    }

    int var = 0;

    while (1) 
    {
        sigwait(&mask, &sig_num);
        var++;
        printf("Signal with sig_num = %d, now var = %d\n", sig_num, var);
    }

    exit(EXIT_SUCCESS);
}