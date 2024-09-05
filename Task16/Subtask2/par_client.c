#include "common.h"
#include "inet_sockets.h"

int main (int argc, char** argv) 
{
    struct sockaddr svaddr;
    socklen_t len = sizeof(struct sockaddr);    
    int cfd;
    int children_num = N_CHILDS;
    int request_num = N_REQS;
    pid_t child_pid;
    char* host = NULL;
    char service[NI_MAXSERV] = PORT_NUM_STR;
    char svaddrStr[INET_ADDRSTRLEN];
    char message[BUF_SIZE];

    if (argc == 3) 
    {        
        children_num = atoi(argv[1]);
        request_num = atoi(argv[2]);
    }

    for (int i = 0; i < children_num; i++) 
    {
        child_pid = fork();
        if (child_pid == 0) 
        {
            for (int j = 0; j < request_num; j++) 
            {
                cfd = inet_connect(host, service, SOCK_STREAM);
                if (cfd == -1) 
                {
                    err_exit("inet_connect");
                }

                if (getpeername(cfd, (struct sockaddr*) &svaddr, &len) == -1) 
                {
                    err_exit("getpeername client");
                }

                if (inet_ntop(AF_INET, svaddr.sa_data, svaddrStr, INET_ADDRSTRLEN) == NULL) 
                {
                    err_exit("inet_ntop");
                }
                snprintf(message, BUF_SIZE, "Client pid = %d, request number = %d", getpid(), j + 1);
                printf("Клиент отправляет сообщение серверу(%s:%s): %s\n", svaddrStr, service, message);

                if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
                {
                    err_exit("write to server");
                }

                if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
                {
                    err_exit("read from server");
                }
                
                printf("Клиент получил от сервера(%s:%u) сообщение: %s\n", svaddrStr, PORT_NUM, message);

                if (close(cfd) == -1) 
                {
                    err_exit("close cfd");
                }
            }

            printf("Клиент %d обслужен!\n", getpid());
            exit(EXIT_SUCCESS);
        }
        else if (child_pid < 0) 
        {
            err_exit("fork");
        }
    }

    while (wait(NULL) > 0); 

    exit(EXIT_SUCCESS);
}