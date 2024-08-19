#include "common.h"
#include "chatroom.h"
#include <ncurses.h>

static int serverId;
static int clientId;
static pthread_t receiving_thread, sending_thread;

void handle_sigint(int sig);
void* handle_response (void*);
void receive_clients (ResponseMsg* resp, char*** client_list, int* cli_list_size);
void* handle_message (void*);
void receive_messages (ResponseMsg* resp, Message** message_list, int* mes_list_size);
void cancel_pthreads (void);
void remove_queues (void);
void disconnect_client (void);

int main(int argc, char *argv[]) 
{
    signal(SIGINT, handle_sigint);
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    RequestMsg req;      
    static char client_name[NAME_LENGTH];
    strncpy(client_name, argv[1], NAME_LENGTH);       
    
    serverId =  msgget(SERVER_KEY, S_IWUSR);
    if (serverId < 0) 
    {
        perror("msgget server connect error");
        exit(EXIT_FAILURE);
    }

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId < 0) 
    {
        perror("msgget client create error");
        exit(EXIT_FAILURE);
    }

    req.client_id = clientId;
    req.request_type = REGISTRATION;
    req.mtype = SERVER;
    strncpy(req.mtext, client_name, NAME_LENGTH);

    //printf("Отправляю сообщение серверу: %s\n", req.mtext);
    if (msgsnd(serverId, &req, MESSAGE_LENGTH, 0) < 0) 
    {
        perror("msgsnd client to server error");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&sending_thread, NULL, handle_message, NULL) != 0) 
    {
        err_exit("pthread_create sending_thread");
    }
    if (pthread_create(&receiving_thread, NULL, handle_response, NULL) != 0)
    {
        err_exit("pthread_create receiving_thread");
    }
    
    
    pthread_join(sending_thread, NULL);
    pthread_join(receiving_thread, NULL);  
        
    exit(EXIT_SUCCESS);
}

void handle_sigint(int sig) 
{
    cancel_pthreads();
    disconnect_client();
    printf("\nКлиент вышел из чата...\n");    
    remove_queues();
    exit(EXIT_SUCCESS);
}

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
            
            printf("Клиенты:\n");
            for (int i = 0; i < cli_list_size; i++) 
            {
                printf("Cname: %s\n", client_list[i]);
            }
        }
        else if (resp.response_type == MESSAGE_INFO) 
        {
            receive_messages(&resp, &message_list, &mes_list_size);

            printf("Сообщения:\n");
            for (int i = 0; i < mes_list_size; i++) 
            {
                printf("%s\n", message_list[i].datetime);
                printf("%s: %s\n", message_list[i].client_name, message_list[i].message);
            }
        }
    }             
}

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
    
    int i = 0;            
    strncpy((*client_list)[i], resp->msg.client_name, NAME_LENGTH);            
    i++;
    
    for (i; i < *cli_list_size; i++) 
    {
        if (msgrcv(clientId, resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        {
            err_exit("msgrcv response from server to client error");
        }
        strncpy((*client_list)[i], resp->msg.client_name, NAME_LENGTH);
    }
}

void* handle_message (void*) 
{
    RequestMsg req;
    char client_message[MESSAGE_LENGTH];
    while (1)
    {
        printf("Введите сообщение: ");        
        fgets(client_message, MESSAGE_LENGTH, stdin);
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

void receive_messages (ResponseMsg* resp, Message** message_list, int* mes_list_size) 
{
    *mes_list_size = resp->list_size;
    *message_list = realloc(*message_list, (*mes_list_size) * sizeof(Message));
    if (*message_list == NULL) 
    {
        err_exit("realloc");
    }
    int i = 0;
    strncpy((*message_list)[i].client_name, resp->msg.client_name, NAME_LENGTH);
    strncpy((*message_list)[i].message, resp->msg.message, MESSAGE_LENGTH);
    strncpy((*message_list)[i].datetime, resp->msg.datetime, DATE_LENGTH);
    i++;

    for (i; i < *mes_list_size; i++) 
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

void cancel_pthreads (void) 
{
    pthread_cancel(sending_thread);
    pthread_cancel(receiving_thread);    
}

void remove_queues (void) 
{
    msgctl(clientId, IPC_RMID, NULL);
}

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