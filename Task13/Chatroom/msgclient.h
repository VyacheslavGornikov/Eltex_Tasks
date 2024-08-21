#ifndef _MSGCLIENT_H_
#define _MSGCLIENT_H_
#include "chatroom.h"

void register_client(void);
void disconnect_client (void);
void receive_clients (ResponseMsg* resp, char*** client_list, int* cli_list_size);
void receive_messages (ResponseMsg* resp, Message** message_list, int* mes_list_size);

#endif