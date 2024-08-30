#ifndef _INET_SOCKS_H_
#define _INET_SOCKS_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <ctype.h>

#define BUF_SIZE 100
#define BACKLOG 5
#define PORT_NUM 50002
#define PORT_NUM_STR "50002"
#define BROADCAST_IP "255.255.255.255"
#define MULTICAST_GROUP "224.0.0.1"
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

#endif