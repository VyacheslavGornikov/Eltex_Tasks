#include "common.h"
#include "msgserver.h"

extern int serverId;

/* 
 * Функция обслуживает запрос клиента
 *
 * req - структура запроса клиента
 * client_list - список клиентов
 * cli_list_size - количество клиентов
 * message_list - список сообщений
 * mes_list_size - количество сообщений
 */
void serve_request (const RequestMsg* req, Client** client_list, int* cli_list_size, Message** message_list, int* mes_list_size) 
{
    
    if (req->request_type == REGISTRATION) /* Регистрация клиента */
        {
            printf("Клиент %s (id = %d) присоединился к чату!\n", req->mtext, req->client_id);

            Client new_client;
            new_client.client_id = req->client_id;
            strncpy(new_client.client_name, req->mtext, NAME_LENGTH);

            add_client(client_list, &new_client, cli_list_size);            
            
            send_clients(*client_list, *cli_list_size);

            if (*message_list != NULL) 
            {   
                send_messages(*client_list, *cli_list_size, *message_list, *mes_list_size);
            }            
            
        }
        else if (req->request_type == DISCONNECTION) /* ОТключение клиента */
        {
            Client disc_client;
            disc_client.client_id = req->client_id;
            int client_index = -1;
            client_index = get_client_index(*client_list, *cli_list_size, disc_client.client_id);
            if (client_index == -1) 
            {
                errno = ENOKEY;
                err_exit("get_client_index error");
            }
            strncpy(disc_client.client_name, (*client_list)[client_index].client_name, NAME_LENGTH);

            remove_client(client_list, cli_list_size, client_index);

            send_clients(*client_list, *cli_list_size);

            printf("Клиент %s (id = %d) покинул чат!\n", disc_client.client_name, disc_client.client_id);

        }
        else if (req->request_type == MESSAGE_INFO) /* Отправка информации о сообщениях */
        {
            Message new_message;
            Client sender;

            sender.client_id = req->client_id;
            int client_index = -1;
            client_index = get_client_index(*client_list, *cli_list_size, sender.client_id);
            if (client_index == -1) 
            {
                errno = ENOKEY;
                err_exit("get_client_index error");
            } 
            strncpy(sender.client_name, (*client_list)[client_index].client_name, NAME_LENGTH);
            time_t current_time = time(NULL);
            struct tm *local_time = localtime(&current_time);
            char formatted_time[DATE_LENGTH];
            strftime(formatted_time, DATE_LENGTH, "%Y-%m-%d %H:%M:%S", local_time);       
           
            strncpy(new_message.client_name, sender.client_name, NAME_LENGTH);
            strncpy(new_message.message, req->mtext, MESSAGE_LENGTH);
            strncpy(new_message.datetime, formatted_time, DATE_LENGTH);

            add_message(message_list, &new_message, mes_list_size);
            
            send_messages(*client_list, *cli_list_size, *message_list, *mes_list_size);
        }  
}

/* 
 * Функция передает список клиентов клиентам
 * 
 * client_list - список клиентов
 * cli_list_size - количество клиентов 
 */
void send_clients (Client* client_list, int cli_list_size) 
{
    ResponseMsg resp;
    resp.mtype = CLIENT;
    resp.response_type = CLIENT_INFO;
    resp.list_size = cli_list_size;         
    
    for (int i = 0; i < cli_list_size; i++) 
    {
        for (int j = 0; j < cli_list_size; j++) 
        {                
            strncpy(resp.msg.client_name, client_list[j].client_name, NAME_LENGTH);
            
            if (msgsnd(client_list[i].client_id, &resp, sizeof(ResponseMsg), 0) < 0) 
            {
                err_exit("msgsnd server to client error(client_list)");
            }
        }
    }
}

/* 
 * Функция передает список сообщений клиентам
 * 
 * client_list - список клиентов
 * cli_list_size - количество клиентов 
 * message_list - список сообщений
 * mes_list_size - количество сообщений
 */
void send_messages (Client* client_list, int cli_list_size, Message* message_list, int mes_list_size) 
{
    ResponseMsg resp;           
    resp.mtype = CLIENT;
    resp.response_type = MESSAGE_INFO;
    resp.list_size = mes_list_size;

    for (int i = 0; i < cli_list_size; i++) 
    {
        for (int j = 0; j < mes_list_size; j++) 
        {                
            strncpy(resp.msg.client_name, message_list[j].client_name, NAME_LENGTH);
            strncpy(resp.msg.message, message_list[j].message, MESSAGE_LENGTH);
            strncpy(resp.msg.datetime, message_list[j].datetime, DATE_LENGTH);
            
            if (msgsnd(client_list[i].client_id, &resp, sizeof(ResponseMsg), 0) < 0) 
            {
                err_exit("msgsnd server to client error(client_list)");
            }
        }
    }  
}

/* 
 * Функция выполняет поиск индекса клиента в списке клиентов
 * Возвращает индекс, если клиент найден и -1, если клиент не найден
 * 
 * list - список клиентов
 * size - количество клиентов
 * client_id - идентификатор клиента(его очереди)
 */
int get_client_index (const Client* list, int size, int client_id) 
{
    int index = -1;
    for (int i = 0; i < size; i++) 
    {
        if (list[i].client_id == client_id) 
        {
            index = i;
            break;
        }
    }

    return index;
}

/* 
 * Функция добавляет нового клиента в список клиентов 
 * 
 * list - список клиентов
 * client - структура клиента для добавления
 * size - количество клиентов 
 */
void add_client (Client** list, const Client* const client, int* size) 
{
    *list = realloc(*list, (++(*size)) * sizeof(Client));
    if (*list == NULL)
        err_exit("add_client realloc");
    
    memmove(&(*list)[*size - 1], client, sizeof(Client));    
}

/* 
 * Функция удаляет клиента из списка клиентов при отключении 
 * 
 * list - список клиентов 
 * size - количество клиентов
 * index - индекс клиента в списке клиентов
 */
void remove_client (Client** list, int* size, int index) 
{    
    for (int i = index; i < *size - 1; i++) 
    {
        memcpy(&(*list)[i], &(*list)[i + 1], sizeof(Client)); 
    }        
    
    *list = realloc(*list, (--(*size)) * sizeof(Client));    

    if (*list == NULL && *size != 0) 
    {
        err_exit("remove_client realloc");
    }            
}

/* 
 * Функция добавляет сообщение в список сообщений 
 * 
 * list - список сообщений
 * client - структура сообщения для добавления
 * size - количество сообщений 
 */
void add_message (Message** list, const Message* const message, int* size) 
{
    *list = realloc(*list, (++(*size)) * sizeof(Message));
    if (*list == NULL)
        err_exit("add_message realloc");
    
    memmove(&(*list)[*size - 1], message, sizeof(Message));
}

/* Удаляет очередь сервера */
void remove_queues (void) 
{
    msgctl(serverId, IPC_RMID, NULL);
}
