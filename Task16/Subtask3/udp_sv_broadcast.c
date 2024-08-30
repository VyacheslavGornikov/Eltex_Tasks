#include "common.h"
#include "inet_socks.h"

int main (void) 
{
    struct sockaddr_in broadcast_addr;
    socklen_t length;    
    int sfd;
    char broadcast_message[BUF_SIZE] = "This is broadcast message!";

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1) 
    {
        err_exit("socket server");
    }   

    int flag = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1) 
    {
        err_exit("setsockopt");
    }

    memset(&broadcast_addr, 0, sizeof(struct sockaddr_in));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    broadcast_addr.sin_port = htons(PORT_NUM);

    length = sizeof(struct sockaddr_in);

    printf("Сервер отправил в широковещательный канал(%s) сообщение: %s\n", BROADCAST_IP, broadcast_message);
    if (sendto(sfd, broadcast_message, BUF_SIZE, 0, (struct sockaddr*) &broadcast_addr, length) != BUF_SIZE) 
    {
        err_exit("sendto client");
    }

    exit(EXIT_SUCCESS);
}