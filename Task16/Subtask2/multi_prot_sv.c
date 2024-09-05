#include "common.h"
#include "inet_sockets.h"

void sig_int(int signum);

int main (void) 
{
    int tcp_fd, udp_fd;
    struct sockaddr_in tcp_addr, udp_addr;
    socklen_t addr_length;
    fd_set read_fds;
    char message[BUF_SIZE];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char tcpAddrStr[ADDRSTRLEN];
    char udpAddrStr[INET_ADDRSTRLEN];

    tcp_fd =  inet_listen(PORT_NUM_STR, BACKLOG, NULL);
    if (tcp_fd == -1) 
    {
        err_exit("inet_listen");
    }

    udp_fd = inet_bind(PORT_NUM_STR, SOCK_DGRAM, NULL);
    if (udp_fd == -1) 
    {
        err_exit("inet_bind");
    }
    
    signal(SIGINT, sig_int);
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(tcp_fd, &read_fds);
        FD_SET(udp_fd, &read_fds);

        int max_fd = (tcp_fd > udp_fd) ? tcp_fd : udp_fd;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) 
        {
            err_exit("select");
        }

        if (FD_ISSET(tcp_fd, &read_fds)) 
        {
            addr_length = sizeof(struct sockaddr_in);
            int cfd = accept(tcp_fd, (struct sockaddr*) &tcp_addr, &addr_length);
            if (cfd == -1) 
            {
                err_exit("tcp cfd");
            }

            if (getnameinfo((struct sockaddr*) &tcp_addr, addr_length, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST) == 0) 
            {
                snprintf(tcpAddrStr, ADDRSTRLEN, "%s:%s", host, service);
            }
            else 
            {
                snprintf(tcpAddrStr, ADDRSTRLEN, "?UNKNOWN?");
            }

            if (read(cfd, message, BUF_SIZE) != BUF_SIZE) 
            {
                err_exit("read from tcp client");
            }

            printf("Сервер получил от tcp клиента(%s) сообщение: %s\n", tcpAddrStr, message);

            for (int i = 0; i < strlen(message) + 1; i++) 
            {
                message[i] = toupper(message[i]);
            }

            printf("Сервер отправил tcp клиенту(%s) сообщение: %s\n", tcpAddrStr, message);

            if (write(cfd, message, BUF_SIZE) != BUF_SIZE) 
            {
                err_exit("write to tcp client");
            }

            if (close(cfd) == -1) 
            {
                err_exit("close tcp");
            }
        }

        if (FD_ISSET(udp_fd, &read_fds)) 
        {
            addr_length = sizeof(struct sockaddr_in);
            if (recvfrom(udp_fd, message, BUF_SIZE, 0, (struct sockaddr*) &udp_addr, &addr_length) != BUF_SIZE) 
            {
                err_exit("recvfrom client");
            }

            if (inet_ntop(AF_INET, &udp_addr.sin_addr, udpAddrStr, INET_ADDRSTRLEN) == NULL) 
            {
                err_exit("inet_ntop");
            }

            printf("Сервер получил от udp клиента(%s:%u) сообщение: %s\n", udpAddrStr, ntohs(udp_addr.sin_port), message);

            for (int i = 0; i < strlen(message) + 1; i++) 
            {
                message[i] = toupper(message[i]);
            }

            printf("Сервер отправил udp клиенту(%s:%u) сообщение: %s\n", udpAddrStr, ntohs(udp_addr.sin_port), message);

            if (sendto(udp_fd, message, BUF_SIZE, 0, (struct sockaddr*) &udp_addr, addr_length) != BUF_SIZE) 
            {
                err_exit("sendto client");
            }
        }

    }    

    exit(EXIT_SUCCESS);
}

void sig_int(int signum) 
{
    printf("Прерывание программы по сигналу signum = %d\n", signum);
    exit(EXIT_SUCCESS);
}