#ifndef _NCURSESCR_H_
#define _NCURSESCR_H_

#include "chatroom.h"
#include <ncurses.h>

void init_ncurses(void);
void create_windows(void);
void print_clients(const char client_list[][NAME_LENGTH], int cli_list_size);
void print_messages(const Message *message_list, int mes_list_size);

#endif