
#include "common.h"
#include "inet_socks.h"


int main (void) 
{
    struct sockaddr_storage claddr;
    int sfd, cfd, optval;
    socklen_t addr_length;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char message[BUF_SIZE];

    /* Заполняем структуру для запроса свободных "пассивных" адресов, подходящих для вызова accept */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if (getaddrinfo(NULL, PORT_NUM_STR, &hints, &result) != 0) 
    {
        err_exit("getaddrinfo server");
    }

    /* Пытаемся привязать сокет к одному из полученных через getaddrinfo адресов */
    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) 
        {
            continue; /* Если не удалось создать сокет, пробуем следующий адрес */
        }

        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) 
        {
            err_exit("sersockopt");
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) 
        {
            break; /* Если bind завершился успешно, выходим из цикла */
        }

        /* Если нет, закрываем сокет и пробуем следующий адрес */
        close(sfd);
    }

    if (rp == NULL) 
    {
        err_exit("Couldn't bind socket to any address");
    }

    if (listen(sfd, BACKLOG) == -1) 
    {
        err_exit("listen");
    } 

    freeaddrinfo(result);

    addr_length = sizeof(struct sockaddr_storage);

    cfd = accept(sfd, (struct sockaddr*) &claddr, &addr_length);
    if (cfd == -1) 
    {
        err_exit("cfd accept");
    }

    if (getnameinfo((struct sockaddr*) &claddr, addr_length, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST) == 0) 
    {
        snprintf(addrStr, ADDRSTRLEN, "%s:%s", host, service);
    }
    else 
    {
        snprintf(addrStr, ADDRSTRLEN, "?UNKNOWN?");
    }

    if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("read from client");
    }

    printf("Сервер получил от клиента(%s) сообщение: %s\n", addrStr, message);

    for (int i = 0; i < strlen(message) + 1; i++) 
    {
        message[i] = toupper(message[i]);
    }

    printf("Сервер отправил клиенту(%s) сообщение: %s\n", addrStr, message);

    if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("write to client");
    }

    exit(EXIT_SUCCESS);
}
