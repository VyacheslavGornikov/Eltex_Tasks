#include "common.h"
#include "chatroom.h"
#include <ncurses.h>

int clientId;

void handle_sigint(int sig);
void remove_queues (void);

int main(int argc, char *argv[]) 
{
    signal(SIGINT, handle_sigint);
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    RequestMsg req;
    ResponseMsg resp;
    Message *message_list = NULL;
    int mes_list_size = 0;
    // Client *client_list = NULL;
    // int cli_list_size = 0;
    char **client_list = NULL;
    int cli_list_size = 0;


    char client_name[NAME_LENGTH];
    char client_message[MESSAGE_LENGTH];
    strncpy(client_name, argv[1], NAME_LENGTH);
    int serverId;
    

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

    while (1)
    {
        if (msgrcv(clientId, &resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        {
            err_exit("msgrcv response from server to client error");
        }
        
        if (resp.response_type == CLIENT_INFO) 
        {
            cli_list_size = resp.list_size;
            printf("im here! size = %d\n", cli_list_size);
            client_list = realloc(client_list, cli_list_size * sizeof(char*));
            if (client_list == NULL) 
            {
                err_exit("realloc");
            }
            for (int i = 0; i < cli_list_size; i++) 
            {
                client_list[i] = malloc(NAME_LENGTH);
                if (client_list[i] == NULL) 
                {
                    err_exit("malloc");
                }
            }
            
            int i = 0;
            printf("im here 2! resp = %s\n", resp.msg.client_name);
            strncpy(client_list[i], resp.msg.client_name, NAME_LENGTH);
            printf("im here 2/5! i = %d\n", i);
            i++;
            printf("im here 3! i = %d\n", i);
            for (i; i < cli_list_size; i++) 
            {
                if (msgrcv(clientId, &resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
                {
                    err_exit("msgrcv response from server to client error");
                }
                strncpy(client_list[i], resp.msg.client_name, NAME_LENGTH);
            }
            
            printf("Клиенты:\n");
            for (int i = 0; i < cli_list_size; i++) 
            {
                printf("Cname: %s\n", client_list[i]);
            }
        }
        // else if (resp.response_type == MESSAGE_INFO) 
        // {
        //     mes_list_size = resp.list_size;
        //     message_list = realloc(message_list, mes_list_size * sizeof(Message));

        //     int i = 0;
        //     strncpy(message_list[i].client_name, resp.msg.client_name, NAME_LENGTH);
        //     strncpy(message_list[i].message, resp.msg.message, MESSAGE_LENGTH);
        //     strncpy(message_list[i].datetime, resp.msg.datetime, DATE_LENGTH);
        //     i++;

        //     for (i; i < mes_list_size; i++) 
        //     {
        //         if (msgrcv(clientId, &resp, sizeof(ResponseMsg), CLIENT, 0) < 0) 
        //         {
        //             err_exit("msgrcv response from server to client error");
        //         }
        //         strncpy(message_list[i].client_name, resp.msg.client_name, NAME_LENGTH);
        //         strncpy(message_list[i].message, resp.msg.message, MESSAGE_LENGTH);
        //         strncpy(message_list[i].datetime, resp.msg.datetime, DATE_LENGTH);
        //     }

        //     printf("Сообщения:\n");
        //     for (int i = 0; i < mes_list_size; i++) 
        //     {
        //         printf("%s\n", message_list[i].datetime);
        //         printf("%s: %s\n", message_list[i].client_name, message_list[i].message);
        //     }
        // } 
        
        // printf("Введите сообщение: ");
        // fgets(client_message, MESSAGE_LENGTH, stdin);
        // req.client_id = clientId;
        // req.mtype = SERVER;
        // req.request_type = MESSAGE_INFO;
        // strncpy(req.mtext, client_message, MESSAGE_LENGTH);

        // if (msgsnd(serverId, &req, MESSAGE_LENGTH, 0) < 0) 
        // {            
        //     err_exit("msgsnd client to server error");
        // }


    }
    
    

    getchar();

    msgctl(clientId, IPC_RMID, NULL);

    
    //run_messanger(argv[1]);
    return 0;
}

void handle_sigint(int sig) 
{
    printf("\nКлиент вышел из чата...\n");
    remove_queues();
    exit(EXIT_SUCCESS);
}

void remove_queues (void) 
{
    msgctl(clientId, IPC_RMID, NULL);
}