#include "common.h"
#include "local_socks.h"

int main (void) 
{
    struct sockaddr_un svaddr, claddr;
    int cfd;
    char message[BUF_SIZE] = "hello world!";

    cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (cfd == -1) 
    {
        err_exit("socket client");
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_LOCAL;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/tcp_cl.%ld", (long) getgid());

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
    strncpy(svaddr.sun_path, SV_TCP_PATH, sizeof(svaddr.sun_path) - 1);

    if (connect(cfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_un)) == -1) 
    {
        err_exit("connect");
    }

    printf("Клиент отправляет сообщение серверу(%s): %s\n", svaddr.sun_path, message);
    if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("write to server");
    }

    if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("read from server");
    }

    printf("Клиент получил от сервера(%s) сообщение: %s\n", svaddr.sun_path, message);

    exit(EXIT_SUCCESS);
}