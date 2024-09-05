#include "common.h"
#include "inet_sockets.h"

void sig_int(int signum);
void* handle_request (void* arg);

int main (void) 
{
    int lfd, *cfd_ptr;
    pthread_t tid;    

    lfd = inet_listen(PORT_NUM_STR, BACKLOG, NULL);
    if (lfd == -1) 
    {
        err_exit("inet_listen");
    }    

    signal(SIGINT, sig_int);
    while (1) 
    {
        cfd_ptr = malloc(sizeof(int));
        if (cfd_ptr == NULL) 
        {
            err_exit("malloc");
        }

        *cfd_ptr = accept(lfd, NULL, NULL);
        if (*cfd_ptr == -1) 
        {
            err_exit("accept");
        }

        if (pthread_create(&tid, NULL, handle_request, (void*) cfd_ptr) != 0) 
        {
            err_exit("pthread_create");
        }
    }

    exit(EXIT_SUCCESS);
}

void sig_int(int signum) 
{
    printf("Прерывание программы по сигналу signum = %d\n", signum);
    exit(EXIT_SUCCESS);
}

void* handle_request (void* arg) 
{    
    struct sockaddr claddr;
    socklen_t claddr_len = sizeof(struct sockaddr);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char claddrStr[ADDRSTRLEN];
    char message[BUF_SIZE];

    int cfd = *((int*) arg);
    free(arg);

    if (pthread_detach(pthread_self()) != 0) 
    {
        err_exit("pthread_detach");
    }

    if (getsockname(cfd, (struct sockaddr*) &claddr, &claddr_len) == -1) 
    {
        err_exit("getsockname");
    }

    if (getnameinfo((struct sockaddr*) &claddr, claddr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST) == 0) 
    {
        snprintf(claddrStr, ADDRSTRLEN, "%s:%s", host, service);
    }
    else 
    {
        snprintf(claddrStr, ADDRSTRLEN, "?UNKNOWN?");
    }

    if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("read from client");
    }

    printf("Сервер получил от клиента(%s) сообщение: %s\n", claddrStr, message);

    for (int i = 0; i < strlen(message) + 1; i++) 
    {
        message[i] = toupper(message[i]);
    }

    printf("Сервер отправил клиенту(%s) сообщение: %s\n", claddrStr, message);

    if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("write to client");
    }

    if (close(cfd) == -1) 
    {
        err_exit("close");
    }
    return NULL;
}