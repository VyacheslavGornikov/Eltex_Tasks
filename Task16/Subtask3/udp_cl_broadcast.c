#include "common.h"
#include "inet_socks.h"

int main (void) 
{
    struct sockaddr_in svaddr;
    socklen_t length;
    struct in_addr inaddr_any;
    inaddr_any.s_addr = INADDR_ANY;
    int cfd;
    char broadcast_message[BUF_SIZE] ;
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

    if (bind(cfd, (struct sockaddr*) &svaddr, sizeof (struct sockaddr_in)) == -1) 
    {
        err_exit("bind client");
    }

    length = sizeof(struct sockaddr_in);

    if (recvfrom(cfd, broadcast_message, BUF_SIZE, 0, (struct sockaddr*) &svaddr, &length) != BUF_SIZE) 
    {
        err_exit("recvfrom server");
    }

    if (inet_ntop(AF_INET, &svaddr.sin_addr, svaddrStr, INET_ADDRSTRLEN) == NULL) 
    {
        err_exit("inet_ntop");
    }    

    printf("Клиент получил от сервера(%s:%u) сообщение: %s\n", svaddrStr, PORT_NUM, broadcast_message);

    exit(EXIT_SUCCESS);
}