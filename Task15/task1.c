//#define  _POSIX_C_SOURCE  200809L
#define _GNU_SOURCE

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

void sig_handler (int sig_num, siginfo_t * info, void * args) 
{
    int* param = (int*) args;
    printf("This is handler of SIGUSR! with sig_num = %d, param = %d, si_signo = %d\n", sig_num, *param, info->si_signo);
}

int main (void) 
{
    struct sigaction sa;    
    int ret;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sa.sa_sigaction = sig_handler;
    sa.sa_mask = mask;
        
    ret = sigaction(SIGUSR1, &sa, NULL);
    if (ret < 0) 
    {
        err_exit("sigaction");
    }

    while (1) 
    {
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}