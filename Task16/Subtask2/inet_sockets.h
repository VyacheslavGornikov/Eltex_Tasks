#ifndef _INET_SOCKETS_H_
#define _INET_SOCKETS_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define BUF_SIZE 255
#define BACKLOG 5
#define N_CHILDS 5
#define N_REQS 3
#define N_THREADS 5
#define INET_LISTEN 1
#define PORT_NUM 50002
#define PORT_NUM_STR "50002"
#define BROADCAST_IP "255.255.255.255"
#define MULTICAST_GROUP "224.0.0.1"
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

#define UDP_PORT 50000
#define TCP_PORT 50001

int inet_connect (const char* host, const char* service, int sock_type);
int inet_listen(char* service, int backlog, socklen_t* addrlen);
int inet_bind(char* service, int sock_type, socklen_t* addrlen);
char* inet_addr_str(const struct sockaddr* addr, socklen_t addrlen, char* addr_str, int addr_strlen);

#endif