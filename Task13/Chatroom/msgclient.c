#include "common.h"
#include "msgclient.h"

extern int clientId;
extern int serverId;
extern char client_name[NAME_LENGTH];

/*
 * Функция отправляет сообщение серверу о появлении нового клиента
 * Примечание: serverId необходимо сделать глобальной переменной
 */
void register_client()
{
    RequestMsg req;
    req.client_id = clientId;
    req.request_type = REGISTRATION;
    req.mtype = SERVER;
    strncpy(req.mtext, client_name, NAME_LENGTH);

    if (msgsnd(serverId, &req, MESSAGE_LENGTH, 0) < 0)
    {
        perror("msgsnd client to server error");
        exit(EXIT_FAILURE);
    }
}

/* Функция отправляет сообщение серверу об отключении клиента */
void disconnect_client (void) 
{
    RequestMsg disc_req;
    disc_req.client_id = clientId;
    disc_req.mtype = SERVER;
    disc_req.request_type = DISCONNECTION;

    if (msgsnd(serverId, &disc_req, MESSAGE_LENGTH, 0) < 0) 
    {
        err_exit("msgsnd client to server error");
    }    
}

/*
 * Функция получает список клиентов от сервера
 * Примечание: clientId необходимо сделать глобальной переменной   
 * 
 * resp - структура сообщения, полученного от сервера
 * client_list - список клиентов
 * cli_list_size - количество клиентов
 */
void receive_clients (ResponseMsg* resp, char*** client_list, int* cli_list_size) 
{    
    *cli_list_size = resp->list_size;            
    *client_list = realloc(*client_list, (*cli_list_size) * sizeof(char*));
    if (*client_list == NULL && *cli_list_size != 0) 
    {
        err_exit("realloc");
    }
    for (int i = 0; i < *cli_list_size; i++) 
    {
        (*client_list)[i] = malloc(NAME_LENGTH);
        if ((*client_list)[i] == NULL) 
        {
            err_exit("malloc");
        }
    }
                    
    strncpy((*client_list)[0], resp->msg.client_name, NAME_LENGTH);           
    
    for (int i = 1; i < *cli_list_size; i++) 
    {
        if (msgrcv(clientId, resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        {
            err_exit("msgrcv response from server to client error");
        }
        strncpy((*client_list)[i], resp->msg.client_name, NAME_LENGTH);
    }
}

/*
 * Функция получает список клиентов от сервера
 * Примечание: переменная clientId необходимо сделать глобальной   
 * 
 * resp - структура сообщения, полученного от сервера
 * client_list - список клиентов
 * cli_list_size - количество клиентов
 */
void receive_messages (ResponseMsg* resp, Message** message_list, int* mes_list_size) 
{
    *mes_list_size = resp->list_size;
    *message_list = realloc(*message_list, (*mes_list_size) * sizeof(Message));
    if (*message_list == NULL) 
    {
        err_exit("realloc");
    }
    
    strncpy((*message_list)[0].client_name, resp->msg.client_name, NAME_LENGTH);
    strncpy((*message_list)[0].message, resp->msg.message, MESSAGE_LENGTH);
    strncpy((*message_list)[0].datetime, resp->msg.datetime, DATE_LENGTH);    

    for (int i = 1; i < *mes_list_size; i++) 
    {
        if (msgrcv(clientId, resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        {
            err_exit("msgrcv response from server to client error");
        }
        strncpy((*message_list)[i].client_name, resp->msg.client_name, NAME_LENGTH);
        strncpy((*message_list)[i].message, resp->msg.message, MESSAGE_LENGTH);
        strncpy((*message_list)[i].datetime, resp->msg.datetime, DATE_LENGTH);
    }
}

/* Функция удаляет очередь клиента */
void remove_queues (void) 
{
    msgctl(clientId, IPC_RMID, NULL);
}