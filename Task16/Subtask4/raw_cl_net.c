#include "common.h"
#include "inet_sockets.h"
#include <netinet/udp.h>
#include <netinet/ip.h>

#define IP_HEADER_LEN 20
#define UDP_HEADER_LEN 8
#define RAW_PORT 50000

void sig_int(int signum); 

int main (void) 
{
    int cfd;
    struct sockaddr_in svaddr;
    // struct in_addr inaddr_any;
    // inaddr_any.s_addr = INADDR_ANY;
    socklen_t addr_len;    
    char svaddrStr[INET_ADDRSTRLEN];
    char message[BUF_SIZE] = "hello world!";

    cfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (cfd == -1) 
    {
        err_exit("socket client");
    }

    int optval = 1;
    if (setsockopt(cfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) == -1) 
    {
        err_exit("setsockopt");
    }

    /* Структура, хранящая endpoint сервера */
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    svaddr.sin_port = htons(PORT_NUM);

    /* Заполняем заголовок IP для отправки сообщения */
    struct iphdr ip_hdr;
    ip_hdr.version = 4; /* Версия протокола IP */
    ip_hdr.ihl = 5; /* Размер IP заголовка 5*4 = 20 байт */
    ip_hdr.ttl = 255; /* Время жизни(кол-во маршрутизаторов) */ 
    ip_hdr.tot_len = 0; /* Длина автоматически заполняется IP_HDRINCL */
    ip_hdr.id = 0; /* Идентификатор пакета автоматически заполняется IP_HDRINCL*/
    ip_hdr.protocol = IPPROTO_UDP; /* Транспортный протокол */
    ip_hdr.check = 0; /* checksum автоматически заполняется IP_HDRINCL */
    ip_hdr.saddr = 0; /* Адрес источника автоматически заполняется IP_HDRINCL */
    ip_hdr.daddr = svaddr.sin_addr.s_addr; /* Адрес получателя */

    /* Заполняем заголовок UDP для отправки сообщения(все поля в big endian) */
    struct udphdr udp_hdr;
    udp_hdr.source = htons(RAW_PORT); /* порт клиента */
    udp_hdr.dest = htons(PORT_NUM); /* порт сервера */
    udp_hdr.len = htons(UDP_HEADER_LEN + BUF_SIZE); /* длина сообщения */
    udp_hdr.check = 0; /* контрольная сумма */

    

    signal(SIGINT, sig_int);

    /* Выделяем память для отправки сообщения серверу: buf = ip_hdr + udp_hdr + message */
    char* buf = malloc(IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE);
    if (buf == NULL) 
    {
        err_exit("malloc");
    }
    memmove(buf, &ip_hdr, IP_HEADER_LEN); /* Перемещаем в buf структуру ip_hdr */
    memmove(buf + IP_HEADER_LEN, &udp_hdr, UDP_HEADER_LEN); /* Перемещаем в buf структуру udp_hdr */
    memmove(buf + IP_HEADER_LEN + UDP_HEADER_LEN, message, BUF_SIZE);  /* Перемещаем в buf сообщение message */

    if (inet_ntop(AF_INET, &svaddr.sin_addr, svaddrStr, INET_ADDRSTRLEN) == NULL) 
    {
        err_exit("inet_ntop");
    }

    printf("Клиент отправляет сообщение серверу(%s:%u): %s\n", svaddrStr, PORT_NUM, message);

    if (sendto(cfd, buf, IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE, 0, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_in)) == -1) 
    {
        err_exit("sendto server");
    }
    

    while (1)
    {
        char recv_buf[512];
        addr_len = sizeof(struct sockaddr_in);
        /* Получаем сообщение вида ip_header(20 байт) + udp_header(8 байт) + message(BUF_SIZE) */
        int recv_bytes =  recvfrom(cfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*) &svaddr, &addr_len);
        if (recv_bytes == -1) 
        {
            err_exit("recvfrom server");
        }

        /* Извлекаем из полученного сообщения udp_header */
        struct udphdr* recv_udp_hdr = (struct udphdr*) (recv_buf + IP_HEADER_LEN);        

        /* Если порт назначения в udp_header совпал с портом нашего клиента, то печатаем полученное сообщение */
        if (ntohs(recv_udp_hdr->dest) == RAW_PORT)  
        {
            if (inet_ntop(AF_INET, &svaddr, svaddrStr, INET_ADDRSTRLEN) == NULL) 
            {
                err_exit("inet_ntop");
            }

            /* Из сообщения вида ip_header(20 байт) + udp_header(8 байт) + message(BUF_SIZE) нам нужно достать message(BUF_SIZE) */
            char* data = recv_buf + IP_HEADER_LEN + UDP_HEADER_LEN;

            printf("raw client(port = %u) получил от сервера(%s:%u) сообщение: %s\n", ntohs(recv_udp_hdr->dest), svaddrStr, ntohs(recv_udp_hdr->source), data);
        }        
    }    

    exit(EXIT_SUCCESS);
}

void sig_int(int signum) 
{
    printf("Прерывание программы по сигналу signum = %d\n", signum);
    exit(EXIT_SUCCESS);
}