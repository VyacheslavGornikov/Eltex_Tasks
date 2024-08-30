#include "common.h"
#include "inet_socks.h"

int main (void) 
{
    struct sockaddr_in multicast_addr;
    socklen_t length;    
    int sfd;
    char multicast_message[BUF_SIZE] = "This is multicast message!";

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1) 
    {
        err_exit("socket server");
    }

    memset(&multicast_addr, 0, sizeof(struct sockaddr_in));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
    multicast_addr.sin_port = htons(PORT_NUM);

    length = sizeof(struct sockaddr_in);

    printf("Сервер отправил в multicast канал(%s) сообщение: %s\n", MULTICAST_GROUP, multicast_message);
    if (sendto(sfd, multicast_message, BUF_SIZE, 0, (struct sockaddr*) &multicast_addr, length) != BUF_SIZE) 
    {
        err_exit("sendto client");
    }

    exit(EXIT_SUCCESS);
}