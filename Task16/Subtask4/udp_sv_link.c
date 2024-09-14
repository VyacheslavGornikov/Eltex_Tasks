#include "common.h"
#include "inet_sockets.h"

void sig_int(int signum);

int main (void) 
{
    struct sockaddr_in svaddr, claddr;     
    int sfd;
    socklen_t length;
    char message[BUF_SIZE];
    char claddrStr[INET_ADDRSTRLEN];

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1) 
    {
        err_exit("socket server");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    svaddr.sin_port = htons(PORT_NUM);

    if (bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_in)) == -1) 
    {
        err_exit("bind server");
    }

    length = sizeof(struct sockaddr_in);

    if (recvfrom(sfd, message, BUF_SIZE, 0, (struct sockaddr*) &claddr, &length) != BUF_SIZE) 
    {
        err_exit("recvfrom client");
    }

    if (inet_ntop(AF_INET, &claddr.sin_addr, claddrStr, INET_ADDRSTRLEN) == NULL) 
    {
        err_exit("inet_ntop");
    }

    printf("Сервер получил от клиента(%s:%u) сообщение: %s\n", claddrStr, ntohs(claddr.sin_port), message);

    for (int i = 0; i < strlen(message) + 1; i++) 
    {
        message[i] = toupper(message[i]);
    }

    printf("Сервер отправил клиенту(%s:%u) сообщение: %s\n", claddrStr, ntohs(claddr.sin_port), message);

    if (sendto(sfd, message, BUF_SIZE, 0, (struct sockaddr*) &claddr, length) != BUF_SIZE) 
    {
        err_exit("sendto client");
    }

    exit(EXIT_SUCCESS);
}

void sig_int(int signum) 
{
    printf("Прерывание программы по сигналу signum = %d\n", signum);
    exit(EXIT_SUCCESS);
}