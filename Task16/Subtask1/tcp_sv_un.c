#include "common.h"
#include "local_socks.h"

int main (void) 
{
    struct sockaddr_un svaddr, claddr;
    socklen_t length;
    int sfd, cfd;
    char message[BUF_SIZE];

    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd == -1) 
    {
        err_exit("socket server");
    }

    if (remove(SV_TCP_PATH) == -1 && errno != ENOENT) 
    {
        err_exit("remove server");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_LOCAL;
    strncpy(svaddr.sun_path, SV_TCP_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_un)) == -1) 
    {
        err_exit("bind server");
    }

    if (listen(sfd, BACKLOG) == -1) 
    {
        err_exit("listen");
    }

    length = sizeof(struct sockaddr_un);

    cfd = accept(sfd, (struct sockaddr*) &claddr, &length);
    if (cfd == -1) 
    {
        err_exit("cfd client");
    }

    if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("read from client");
    }

    printf("Сервер получил от клиента(%s) сообщение: %s\n", claddr.sun_path, message);

    for (int i = 0; i < strlen(message) + 1; i++) 
    {
        message[i] = toupper(message[i]);
    }

    printf("Сервер отправил клиенту(%s) сообщение: %s\n", claddr.sun_path, message); 

    if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("write to client");
    }

    exit(EXIT_SUCCESS);
}