#include "common.h"
#include "inet_sockets.h"
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>

#define ETH_HEADER_LEN 14
#define IP_HEADER_LEN 20
#define UDP_HEADER_LEN 8
#define RAW_PORT 50000

void sig_int(int signum);
unsigned char* mac_to_uchar_array (const char* mac);
unsigned short ip_checksum(const void *iphdr, int len);

int main (void) 
{
    int cfd;
    struct sockaddr_ll svaddr;    
    socklen_t addr_len;    
    char svaddrStr[INET_ADDRSTRLEN];
    char message[BUF_SIZE] = "hello world!";

    cfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (cfd == -1) 
    {
        err_exit("socket client");
    }    

    /* Структура, хранящая endpoint сервера */
    memset(&svaddr, 0, sizeof(struct sockaddr_ll));
    svaddr.sll_family = AF_PACKET;
    svaddr.sll_protocol = htons(ETH_P_IP);
    svaddr.sll_ifindex = if_nametoindex(SERVER_INTERFACE);
    svaddr.sll_halen = 6;        
    memmove(svaddr.sll_addr, mac_to_uchar_array(SERVER_MAC), 6);
    
    /* Заполняем заголовок ETHERNET */
    struct ethhdr eth_hdr;
    memmove(eth_hdr.h_dest, mac_to_uchar_array(SERVER_MAC), 6);       
    memmove(eth_hdr.h_source, mac_to_uchar_array(CLIENT_MAC), 6);   
    eth_hdr.h_proto = htons(ETH_P_IP);

    /* Заполняем заголовок IP для отправки сообщения */
    struct iphdr ip_hdr;
    ip_hdr.version = 4; /* Версия протокола IP */
    ip_hdr.ihl = 5; /* Размер IP заголовка 5*4 = 20 байт */
    ip_hdr.ttl = 255; /* Время жизни(кол-во маршрутизаторов) */ 
    ip_hdr.tot_len = htons(IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE); /* Длина автоматически заполняется IP_HDRINCL */
    ip_hdr.id = htons(1234); /* Идентификатор пакета автоматически заполняется IP_HDRINCL*/
    ip_hdr.protocol = IPPROTO_UDP; /* Транспортный протокол */
    ip_hdr.check = 0; /* checksum автоматически заполняется IP_HDRINCL */
    ip_hdr.saddr = inet_addr(CLIENT_IP); /* Адрес источника автоматически заполняется IP_HDRINCL */
    ip_hdr.daddr = inet_addr(SERVER_IP); /* Адрес получателя */    

    //ip_hdr.check = ip_checksum(&ip_hdr, IP_HEADER_LEN);
    /* Считаем checksum для заголовка IP */  
    ip_hdr.check = ip_checksum(&ip_hdr, IP_HEADER_LEN);  

    /* Заполняем заголовок UDP для отправки сообщения(все поля в big endian) */
    struct udphdr udp_hdr;
    udp_hdr.source = htons(RAW_PORT); /* порт клиента */
    udp_hdr.dest = htons(PORT_NUM); /* порт сервера */
    udp_hdr.len = htons(UDP_HEADER_LEN + BUF_SIZE); /* длина сообщения */
    udp_hdr.check = 0; /* контрольная сумма */    

    signal(SIGINT, sig_int);

    /* Выделяем память для отправки сообщения серверу: buf = ip_hdr + udp_hdr + message */
    char* buf = malloc(ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE);
    if (buf == NULL) 
    {
        err_exit("malloc");
    }

    memmove(buf, &eth_hdr, ETH_HEADER_LEN);
    memmove(buf + ETH_HEADER_LEN, &ip_hdr, IP_HEADER_LEN); /* Перемещаем в buf структуру ip_hdr */
    memmove(buf + ETH_HEADER_LEN + IP_HEADER_LEN, &udp_hdr, UDP_HEADER_LEN); /* Перемещаем в buf структуру udp_hdr */
    memmove(buf + ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN, message, BUF_SIZE);  /* Перемещаем в buf сообщение message */
    
    printf("Клиент отправляет сообщение серверу(%s:%u): %s\n", SERVER_IP, PORT_NUM, message);

    if (sendto(cfd, buf, ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE, 0, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_ll)) == -1) 
    {
        err_exit("sendto server");
    }
    

    while (1)
    {
        char recv_buf[512];
        addr_len = sizeof(struct sockaddr_in);
        struct sockaddr_in svaddr_recv;
        /* Получаем сообщение вида ip_header(20 байт) + udp_header(8 байт) + message(BUF_SIZE) */
        int recv_bytes =  recvfrom(cfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*) &svaddr_recv, &addr_len);
        if (recv_bytes == -1) 
        {
            err_exit("recvfrom server");
        }

        /* Извлекаем из полученного сообщения udp_header */
        struct udphdr* recv_udp_hdr = (struct udphdr*) (recv_buf + ETH_HEADER_LEN + IP_HEADER_LEN);        

        /* Если порт назначения в udp_header совпал с портом нашего клиента, то печатаем полученное сообщение */
        if (ntohs(recv_udp_hdr->dest) == RAW_PORT)  
        {
            if (inet_ntop(AF_INET, &svaddr_recv.sin_addr, svaddrStr, INET_ADDRSTRLEN) == NULL) 
            {
                err_exit("inet_ntop");
            }

            /* Из сообщения вида ip_header(20 байт) + udp_header(8 байт) + message(BUF_SIZE) нам нужно достать message(BUF_SIZE) */
            char* data = recv_buf + ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN;

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

unsigned char* mac_to_uchar_array (const char* mac) 
{    
    unsigned char* sll_addr = malloc(6 * sizeof(unsigned char));
    if (sll_addr == NULL) 
    {
        err_exit("malloc");
    }
    sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &sll_addr[0], &sll_addr[1], &sll_addr[2], &sll_addr[3], &sll_addr[4], &sll_addr[5]);
    return sll_addr;
}

unsigned short ip_checksum(const void *iphdr, int len)
{
    unsigned short* csbuf = (unsigned short*) iphdr;
    int csum = 0;    

    for (int i = 0; i < 10; i++) 
    {
        csum += *csbuf;
        csbuf++;
    }
    
    int tmp = csum >> 16;
    csum = (csum & 0xFFFF) + tmp; 
    csum = ~csum;

    return csum;
}