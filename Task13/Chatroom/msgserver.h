#ifndef _MSGSERVER_H_
#define _MSGSERVER_H_
#include "chatroom.h"

void serve_request (const RequestMsg* req, Client** client_list, int* cli_list_size, Message** message_list, int* mes_list_size);
void send_clients (Client* client_list, int cli_list_size);
void send_messages (Client* client_list, int cli_list_size, Message* message_list, int mes_list_size);
int get_client_index (const Client* list, int size, int client_id);
void add_client (Client** list, const Client* const client, int* size);
void remove_client (Client** list, int* size, int index);
void add_message (Message** list, const Message* const message, int* size);

#endif