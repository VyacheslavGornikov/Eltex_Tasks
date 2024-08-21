#include "common.h"
#include "chatroom.h"
#include "msgserver.h"

int serverId;

void handle_sigint(int sig);

int main() 
{    
    signal(SIGINT, handle_sigint);   

    RequestMsg req;    
    Message *message_list = NULL;
    int mes_list_size = 0;
    Client *client_list = NULL;
    int cli_list_size = 0;      
    
    /* Создаем очередь для обработки сообщений клиентов */
    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId < 0) 
    {
        err_exit("msgget server create error");        
    }

    printf("Сервер запущен и ожидает клиентов...\n");
    
    while (1)
    {
        if(msgrcv(serverId, &req, MESSAGE_LENGTH, SERVER, 0) < 0) 
        {            
            err_exit("msgrcv server error");
        }        

        serve_request(&req, &client_list, &cli_list_size, &message_list, &mes_list_size);                    

    }       

    exit(EXIT_SUCCESS);
}

/* Обработчик сигнала прерывания программы SIGINT */
void handle_sigint(int sig) 
{
    printf("\nСервер завершает свою работу...\n");    
    remove_queues();
    exit(EXIT_SUCCESS);
}


