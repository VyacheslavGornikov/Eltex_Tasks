#include "common.h"
#include "ncursescr.h"
#include <ncurses.h>
#include <sys/ioctl.h>

extern WINDOW* mes_border;
extern WINDOW* cli_border;
extern WINDOW* input_border;
extern WINDOW* mes_wnd;
extern WINDOW* cli_wnd;
extern WINDOW* input_wnd;
extern int mes_wnd_height, mes_wnd_width;
extern int cli_wnd_height, cli_wnd_width;
extern int input_wnd_height, input_wnd_width;
extern char client_name[NAME_LENGTH];


/* Начальные настройки ncurses */
void init_ncurses(void) 
{
    initscr();
    curs_set(FALSE);
    cbreak();
    echo();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Datetime
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Client
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Message   
}

/*
 * Создание окон ncurses для чат-комнаты
 * Примечание: указатели окон являются глобальными переменными
 */
void create_windows(void)
{
    mes_wnd_height = LINES - 5;
    mes_wnd_width = COLS - COLS / 5;
    cli_wnd_height = LINES - 5;
    cli_wnd_width = COLS - mes_wnd_width - 1;
    input_wnd_height = LINES - mes_wnd_height;
    input_wnd_width = COLS;

    mes_border = newwin(mes_wnd_height, mes_wnd_width, 0, 0);
    cli_border = newwin(cli_wnd_height, cli_wnd_width, 0, mes_wnd_width + 1);
    input_border = newwin(input_wnd_height, input_wnd_width, mes_wnd_height, 0);

    mes_wnd = newwin(mes_wnd_height - 2, mes_wnd_width - 2, 1, 1);
    cli_wnd = newwin(cli_wnd_height - 2, cli_wnd_width - 2, 1, mes_wnd_width + 2);
    input_wnd = newwin(input_wnd_height - 2, input_wnd_width - 2, mes_wnd_height + 1, 1);

    box(mes_border, 0, 0);
    box(cli_border, 0, 0);
    box(input_border, 0, 0);

    wrefresh(mes_border);
    wrefresh(cli_border);
    wrefresh(input_border);
}

/* 
 * Печатает в окне список клиентов
 * Примечание: указатель на окно cli_wnd необходимо сделать глобальной переменной
 * 
 * client_list - список клиентов
 * cli_list_size - количество клиентов 
 */
void print_clients(const char client_list[][NAME_LENGTH], int cli_list_size)
{    
    werase(cli_wnd);    
    for (int i = 0; i < cli_list_size; i++) 
    {
        if (strcmp(client_name, client_list[i]) == 0)
        {
            wattron(cli_wnd, A_REVERSE);
        }
        wprintw(cli_wnd, "%s\n", client_list[i]);
        wattroff(cli_wnd, A_REVERSE);
    }      
    wrefresh(cli_wnd);    
}

/* 
 * Печатает в окне список сообщений
 * Примечание: указатель на окно mes_wnd необходимо сделать глобальной переменной
 * 
 * message_list - список сообщений
 * mes_list_size - количество сообщений 
 */
void print_messages(const Message *message_list, int mes_list_size)
{    
    werase(mes_wnd);    
    for (int i = 0; i < mes_list_size; i++)
    {
        wattron(mes_wnd, COLOR_PAIR(1));
        wprintw(mes_wnd, "%s\n", message_list[i].datetime);
        wattroff(mes_wnd, COLOR_PAIR(1));
        wattron(mes_wnd, COLOR_PAIR(2));
        wprintw(mes_wnd, "%s: ", message_list[i].client_name);
        wattroff(mes_wnd, COLOR_PAIR(2));
        wattron(mes_wnd, COLOR_PAIR(3));
        wprintw(mes_wnd, "%s\n", message_list[i].message);
        wattroff(mes_wnd, COLOR_PAIR(3));
    }    
    wrefresh(mes_wnd);    
}

