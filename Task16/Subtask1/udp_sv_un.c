#include "common.h"
#include "local_socks.h"

int main (void) 
{
    struct sockaddr_un svaddr, claddr;
    int sfd;
    ssize_t num_bytes;
    socklen_t length;
    char message[BUF_SIZE];

    sfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sfd == -1) 
    {
        err_exit("socket server");
    }

    if (remove(SV_UDP_PATH) == -1 && errno != ENOENT) 
    {
        err_exit("remove SV_SOCK_PATH");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_LOCAL;
    strncpy(svaddr.sun_path, SV_UDP_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_un)) == -1) 
    {
        err_exit("bind server");
    }

    length = sizeof(struct sockaddr_un);

    num_bytes = recvfrom(sfd, message, BUF_SIZE, 0, (struct sockaddr*) &claddr, &length);
    if (num_bytes == -1) 
    {
        err_exit("recvfrom client");
    }

    printf("Сервер получил от клиента(%s) сообщение: %s\n", claddr.sun_path, message);

    for (int i = 0; i < strlen(message) + 1; i++) 
    {
        message[i] = toupper(message[i]);
    }

    printf("Сервер отправил клиенту(%s) сообщение: %s\n", claddr.sun_path, message);    

    if (sendto(sfd, message, BUF_SIZE, 0, (struct sockaddr*) &claddr, length) != num_bytes) 
    {
        err_exit("sendto client");
    }

    exit(EXIT_SUCCESS);
    
}