#include "common.h"
#include "chatroom.h"
#include "msgclient.h"
#include "ncursescr.h"
#include <ncurses.h>
#include <sys/ioctl.h>

int serverId;
int clientId;
static pthread_t receiving_thread, sending_thread;
char client_name[NAME_LENGTH];

WINDOW* mes_border = NULL;
WINDOW* cli_border = NULL;
WINDOW* input_border = NULL;
WINDOW* mes_wnd = NULL;
WINDOW* cli_wnd = NULL;
WINDOW* input_wnd = NULL;
int mes_wnd_height, mes_wnd_width;
int cli_wnd_height, cli_wnd_width;
int input_wnd_height, input_wnd_width;

void handle_sigint(int sig);
void sig_winch(int signo);
void* handle_response(void*);
void* handle_message (void*);
void cancel_pthreads (void);




int main(int argc, char *argv[]) 
{
    signal(SIGINT, handle_sigint);
    signal(SIGWINCH, sig_winch); // обработка изменения размеров окна

    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }          
    
    strncpy(client_name, argv[1], NAME_LENGTH);       
    
    /* Создаем очередь для клиента и подключаемся к очереди сервера */
    serverId =  msgget(SERVER_KEY, S_IWUSR);
    if (serverId < 0) 
    {        
        err_exit("msgget server connect error");
    }

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId < 0) 
    {        
        err_exit("msgget client create error");
    }

    /* Регистрируем клиента на сервере */
    register_client();

    /* Подключаем интерфейс ncurses */
    init_ncurses();
    create_windows();

    /* Создаем потоки для обмена сообщениями между сервером и клиентом */
    if (pthread_create(&sending_thread, NULL, handle_message, NULL) != 0) 
    {
        err_exit("pthread_create sending_thread");
    }
    if (pthread_create(&receiving_thread, NULL, handle_response, NULL) != 0)
    {
        err_exit("pthread_create receiving_thread");
    }    
    
    /* ВОжидаем завершения потоков потоков */
    pthread_join(sending_thread, NULL);
    pthread_join(receiving_thread, NULL);  
        
    exit(EXIT_SUCCESS);
}

/* Обработчик сигнала прерывания программы SIGINT */
void handle_sigint(int sig) 
{
    clear();
    endwin();
    cancel_pthreads();
    disconnect_client();
    printf("Клиент вышел из чата...\n");    
    remove_queues();
    exit(EXIT_SUCCESS);
}

/* Обработчик сигнала изменения размеров окна SIG_WINCH */
void sig_winch(int signo) 
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*) &size);
    resizeterm(size.ws_row, size.ws_col);
}

/* Функция для запуска потока-получателя (receiving_thread) сообщений сервера */
void* handle_response (void*) 
{
    Message *message_list = NULL;
    int mes_list_size = 0;
    char **client_list = NULL;
    int cli_list_size = 0; 
    ResponseMsg resp;

    while (1)
    {        
        if (msgrcv(clientId, &resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        {
            err_exit("msgrcv response from server to client error");
        }
        
        if (resp.response_type == CLIENT_INFO) 
        {
            receive_clients(&resp, &client_list, &cli_list_size);
            print_clients(client_list, cli_list_size);
        }
        else if (resp.response_type == MESSAGE_INFO) 
        {
            
            receive_messages(&resp, &message_list, &mes_list_size);
            print_messages(message_list, mes_list_size);
        }
    }
}



/* Функция для запуска потока-отправителя (sending_thread) сообщений серверу */
void* handle_message (void*) 
{
    RequestMsg req;
    char client_message[MESSAGE_LENGTH];
    while (1)
    {        
        werase(input_wnd);        
        if (mvwgetnstr(input_wnd, 0, 0, client_message, MESSAGE_LENGTH) == ERR) 
        {
            ERROR_MESSAGE_NCURS("error reading from keyboard");        
        }
        
        req.client_id = clientId;
        req.mtype = SERVER;
        req.request_type = MESSAGE_INFO;
        strncpy(req.mtext, client_message, MESSAGE_LENGTH);
            

        if (msgsnd(serverId, &req, MESSAGE_LENGTH, 0) < 0) 
        {            
            err_exit("msgsnd client to server error");
        }       
     
    }       
}

/* Функция завершает работу потоков */
void cancel_pthreads (void) 
{
    pthread_cancel(sending_thread);
    pthread_cancel(receiving_thread);    
}





