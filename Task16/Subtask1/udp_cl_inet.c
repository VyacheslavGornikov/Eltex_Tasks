#include "common.h"
#include "inet_socks.h"

int main (void) 
{
    struct sockaddr_in svaddr;
    struct in_addr inaddr_any;
    inaddr_any.s_addr = INADDR_ANY;
    int cfd;
    char message[BUF_SIZE] = "hello world!";
    char svaddrStr[INET_ADDRSTRLEN];

    cfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (cfd == -1) 
    {
        err_exit("socket client");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr = inaddr_any;
    svaddr.sin_port = htons(PORT_NUM);

    if (inet_ntop(AF_INET, &svaddr.sin_addr, svaddrStr, INET_ADDRSTRLEN) == NULL) 
    {
        err_exit("inet_ntop");
    }

    printf("Клиент отправляет сообщение серверу(%s:%u): %s\n", svaddrStr, PORT_NUM, message);
    if (sendto(cfd, message, BUF_SIZE, 0, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_in)) != BUF_SIZE) 
    {
        err_exit("sendto server");
    }

    if (recvfrom(cfd, message, BUF_SIZE, 0, NULL, NULL) != BUF_SIZE) 
    {
        err_exit("recvfrom server");
    }

    printf("Клиент получил от сервера(%s:%u) сообщение: %s\n", svaddrStr, PORT_NUM, message);

    exit(EXIT_SUCCESS);
}