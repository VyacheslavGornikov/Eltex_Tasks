#ifndef _NCURSESCR_H_
#define _NCURSESCR_H_

#include "chatroom.h"

void init_ncurses(void);
void create_windows(void);
void print_clients(char **client_list, int cli_list_size);
void print_messages(Message *message_list, int mes_list_size);

#endif