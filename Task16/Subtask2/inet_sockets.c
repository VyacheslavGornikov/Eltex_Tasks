#define _GNU_SOURCE
#include "inet_sockets.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"

/*
 * Возвращает файловый дескриптор сокета, соединенный с хостом
 *
 * host - адрес хоста
 * service - порт хоста
 * sock_type - тип сокета
 */
int inet_connect (const char* host, const char* service, int sock_type) 
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int cfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = sock_type;
    hints.ai_family = AF_UNSPEC;
    
    s = getaddrinfo(host, service, &hints, &result);
    if (s != 0) 
    {
        errno = ENOSYS;
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1) 
        {
            continue;
        }

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) == 0) 
        {
            break;
        }

        close(cfd);
    }

    freeaddrinfo(result);
    return (rp == NULL) ? -1 : cfd;
}

/*
 * Вспомогательная функция для inet_bind и inet_listen
 * 
 * service - порт сокета
 * sock_type - тип сокета
 * addrlen - указатель на переменную, хранящую размер структуры sockaddr
 * do_listen - слушать ли сокет?
 * backlog - допустимый размер очереди ожидающих соединений 
 */
static int inet_passive_socket(char* service, int sock_type, socklen_t* addrlen, int do_listen, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = sock_type;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0) 
    {
        return -1;
    }

    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) 
        {
            continue;
        }

        if (do_listen) 
        {
            if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) 
            {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) 
        {
            break;
        }

        close(sfd);
    }

    if (rp != NULL && do_listen) 
    {
        if (listen(sfd, backlog) == -1) 
        {
            freeaddrinfo(result);
            return -1;
        }
    }

    if (rp != NULL && addrlen != NULL) 
    {
        *addrlen = rp->ai_addrlen;
    }

    freeaddrinfo(result);
    return (rp == NULL) ? -1 : sfd;
}

/*
 * Возвращает файловый дескриптор слушающего потокового сокета(SOCK_STREAM)
 * 
 * service - порт сокета
 * backlog - допустимый размер очереди ожидающих соединений
 * addrlen - указатель на переменную, хранящую размер структуры sockaddr
 */
int inet_listen(char* service, int backlog, socklen_t* addrlen) 
{
    return inet_passive_socket(service, SOCK_STREAM, addrlen, INET_LISTEN, BACKLOG);
}

/*
 * Возвращает файловый дескриптор указанного типа сокета, привязанного к указанному порту
 * 
 * service - порт сокета
 * sock_type - тип сокета
 * addrlen - указатель на переменную, хранящую размер структуры sockaddr
 */
int inet_bind(char* service, int sock_type, socklen_t* addrlen) 
{
    return inet_passive_socket(service, sock_type, addrlen, 0, 0);
}



