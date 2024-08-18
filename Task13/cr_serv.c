#include "common.h"
#include "chatroom.h"

static int serverId;

void handle_sigint(int sig);
void remove_queues (void);
void grimReaper(int sig);
int get_client_index (const Client* list, int size, int client_id);
void add_client (Client** list, const Client* const client, int* size);
void remove_client (Client** list, int* size, int index);
void add_message (Message** list, const Message* const message, int* size);

int main() 
{
    //key_t key;
    signal(SIGINT, handle_sigint);
    atexit(remove_queues);

    RequestMsg req;
    ResponseMsg resp;
    Message *message_list = NULL;
    int mes_list_size = 0;
    Client *client_list = NULL;
    int cli_list_size = 0;

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
    {
        err_exit("sigaction");        
    }    
    
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

        if (req.request_type == REGISTRATION) 
        {
            printf("Клиент %s (id = %d) присоединился к чату!\n", req.mtext, req.client_id);

            Client new_client;
            new_client.client_id = req.client_id;
            strncpy(new_client.client_name, req.mtext, NAME_LENGTH);

            add_client(&client_list, &new_client, &cli_list_size);

            printf("size = %d\n", cli_list_size);
            resp.mtype = CLIENT;
            resp.response_type = CLIENT_INFO;
            resp.list_size = cli_list_size;
            printf("resp size = %d\n", resp.list_size);

            printf("Клиенты:\n");
            for (int i = 0; i < cli_list_size; i++) 
            {
                printf("Cname: %s, cid %d\n\n", client_list[i].client_name, client_list[i].client_id);
            }
            for (int i = 0; i < cli_list_size; i++) 
            {
                for (int j = 0; j < cli_list_size; j++) 
                {                
                    strncpy(resp.msg.client_name, client_list[j].client_name, NAME_LENGTH);
                    printf("i = %d, resp client name = %s\n", i,resp.msg.client_name);
                    if (msgsnd(client_list[i].client_id, &resp, sizeof(ResponseMsg), 0) < 0) 
                    {
                        err_exit("msgsnd server to client error(client_list)");
                    }
                }
            }

            if (message_list != NULL) 
            {
                printf("Сообщения:\n");
                for (int i = 0; i < mes_list_size; i++) 
                {
                    printf("%s\n", message_list[i].datetime);
                    printf("%s: %s\n", message_list[i].client_name, message_list[i].message);
                }
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
            
        }
        else if (req.request_type == DISCONNECTION) 
        {
            Client disc_client;
            disc_client.client_id = req.client_id;
            int client_index = -1;
            client_index = get_client_index(client_list, cli_list_size, disc_client.client_id);
            if (client_index == -1) 
            {
                errno = ENOKEY;
                err_exit("get_client_index error");
            }
            strncpy(disc_client.client_name, client_list[client_index].client_name, NAME_LENGTH);

            remove_client(&client_list, &cli_list_size, client_index);

            for (int i = 0; i < cli_list_size; i++) 
            {
                for (int j = 0; j < cli_list_size; j++) 
                {                
                    strncpy(resp.msg.client_name, client_list[j].client_name, NAME_LENGTH);
                    printf("i = %d, resp client name = %s\n", i,resp.msg.client_name);
                    if (msgsnd(client_list[i].client_id, &resp, sizeof(ResponseMsg), 0) < 0) 
                    {
                        err_exit("msgsnd server to client error(client_list)");
                    }
                }
            }

            printf("Клиент %s (id = %d) покинул чат!\n", disc_client.client_name, disc_client.client_id);

        }
        else if (req.request_type == MESSAGE_INFO) 
        {
            Message new_message;
            Client sender;

            sender.client_id = req.client_id;
            int client_index = -1;
            client_index = get_client_index(client_list, cli_list_size, sender.client_id);
            if (client_index == -1) 
            {
                errno = ENOKEY;
                err_exit("get_client_index error");
            } 
            strncpy(sender.client_name, client_list[client_index].client_name, NAME_LENGTH);
            time_t current_time = time(NULL);
            struct tm *local_time = localtime(&current_time);
            char formatted_time[DATE_LENGTH];
            strftime(formatted_time, DATE_LENGTH, "%Y-%m-%d %H:%M:%S", local_time);       
           
            strncpy(new_message.client_name, sender.client_name, NAME_LENGTH);
            strncpy(new_message.message, req.mtext, MESSAGE_LENGTH);
            strncpy(new_message.datetime, formatted_time, DATE_LENGTH);

            add_message(&message_list, &new_message, &mes_list_size);
            
            printf("Сообщения:\n");
            for (int i = 0; i < mes_list_size; i++) 
            {
                printf("%s\n", message_list[i].datetime);
                printf("%s: %s\n", message_list[i].client_name, message_list[i].message);
            }
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

    }   

    free(client_list);
    //free(message_list);
    remove_queues();

    exit(EXIT_SUCCESS);
}

void handle_sigint(int sig) 
{
    printf("\nСервер завершает свою работу...\n");
    remove_queues();
    exit(EXIT_SUCCESS);
}

void remove_queues (void) 
{
    msgctl(serverId, IPC_RMID, NULL);
}

void /* SIGCHLD handler */
grimReaper(int sig)
{
    int savedErrno;
    savedErrno = errno; /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0) 
    {
        continue;
    }
    errno = savedErrno;
}

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

void add_client (Client** list, const Client* const client, int* size) 
{
    *list = realloc(*list, (++(*size)) * sizeof(Client));
    if (*list == NULL)
        err_exit("add_client realloc");
    
    memmove(&(*list)[*size - 1], client, sizeof(Client));    
}

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

void add_message (Message** list, const Message* const message, int* size) 
{
    *list = realloc(*list, (++(*size)) * sizeof(Message));
    if (*list == NULL)
        err_exit("add_message realloc");
    
    memmove(&(*list)[*size - 1], message, sizeof(Message));
}