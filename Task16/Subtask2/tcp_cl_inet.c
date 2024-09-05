#include "common.h"
#include "inet_sockets.h"

int main (void) 
{
    int cfd;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char svaddrStr[INET_ADDRSTRLEN];
    char message[BUF_SIZE] = "hello world!";

    /* Заполняем структуру для запроса свободных адресов, подходящих для вызова connect */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(NULL, PORT_NUM_STR, &hints, &result) != 0) 
    {
        err_exit("getaddrinfo client");
    }

    /* Пытаемся привязать сокет к одному из полученных через getaddrinfo адресов */
    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1) 
        {
            continue; /* Если не удалось создать сокет, пробуем следующий адрес */
        }

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1) 
        {
            break; /* Если вызов connect завершился успешно, выходим из цикла */
        }

        /* Если нет, закрываем сокет и пробуем следующий адрес */
        close(cfd); 
    }

    if (rp == NULL) 
    {
        err_exit("Couldn't connect socket to any address");
    }

    freeaddrinfo(result);

    if (inet_ntop(AF_INET, rp->ai_addr->sa_data, svaddrStr, INET_ADDRSTRLEN) == NULL) 
    {
        err_exit("inet_ntop");
    }

    printf("Клиент отправляет сообщение серверу(%s:%u): %s\n", svaddrStr, PORT_NUM, message);
    if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("write to server");
    }

    if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
    {
        err_exit("read from server");
    }
    
    printf("Клиент получил от сервера(%s:%u) сообщение: %s\n", svaddrStr, PORT_NUM, message);

    exit(EXIT_SUCCESS);
}