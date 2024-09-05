#include "common.h"
#include "inet_sockets.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sig_int(int signum);
void* handle_request (void* arg);

int main (int argc, char** argv) 
{
    int lfd;
    pthread_t* tid_list;
    int thread_num = N_THREADS;

    if (argc == 2) 
    {
        thread_num = atoi(argv[1]);
    }

    tid_list = malloc(thread_num * sizeof(pthread_t));
    if (tid_list == NULL) 
    {
        err_exit("malloc");
    }    

    lfd = inet_listen(PORT_NUM_STR, BACKLOG, NULL);
    if (lfd == -1) 
    {
        err_exit("inet_listen");
    }    

    signal(SIGINT, sig_int);
    for (int i = 0; i < thread_num; i++) 
    {  
        if (pthread_create(&tid_list[i], NULL, handle_request, (void*) &lfd) != 0) 
        {
            err_exit("pthread_create");
        }
    }

    while(1);
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
    int cfd;
    int lfd = *((int*) arg);   

    if (pthread_detach(pthread_self()) != 0) 
    {
        err_exit("pthread_detach");
    }

    printf("Поток сервера (tid = %ld) начал свою работу!\n", pthread_self());

    while(1) 
    {
        pthread_mutex_lock(&mutex);
        cfd = accept(lfd, NULL, NULL);
        if (cfd == -1) 
        {
            err_exit("accept");
        }
        pthread_mutex_unlock(&mutex);

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
    }
    
    return NULL;
}