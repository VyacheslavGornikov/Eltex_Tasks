#include "common.h"
#include "local_socks.h"

int main (void) 
{
    struct sockaddr_un svaddr, claddr;
    int cfd;    
    char message[BUF_SIZE] = "hello world!";

    cfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (cfd == -1) 
    {
        err_exit("socket client");
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_LOCAL;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/udp_cl.%ld", (long) getgid());

    if (remove(claddr.sun_path) == -1 && errno != ENOENT) 
    {
        err_exit("remove client");
    }
    
    if (bind(cfd, (struct sockaddr*) &claddr, sizeof(struct sockaddr_un)) == -1) 
    {        
        err_exit("bind client");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_LOCAL;
    strncpy(svaddr.sun_path, SV_UDP_PATH, sizeof(svaddr.sun_path) - 1);

    printf("Клиент отправляет сообщение серверу(%s): %s\n", svaddr.sun_path, message);
    if (sendto(cfd, message, BUF_SIZE, 0, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_un)) != BUF_SIZE) 
    {
        err_exit("sendto server");
    }

    if (recvfrom(cfd, message, BUF_SIZE, 0, NULL, NULL) == -1) 
    {
        err_exit("recvfrom server");
    }

    printf("Клиент получил от сервера(%s) сообщение: %s\n", svaddr.sun_path, message);

    if (remove(claddr.sun_path) == -1) 
    {
        err_exit("remove client");
    }
    exit(EXIT_SUCCESS);
}