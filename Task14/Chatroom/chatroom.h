#ifndef _CHATROOM_H_
#define _CHATROOM_H_

#define MAX_MESSAGES 100
#define MESSAGE_LENGTH 255
#define NAME_LENGTH 30
#define DATE_LENGTH 30
#define MAX_CLIENTS 10
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

typedef struct message 
{
    char client_name[NAME_LENGTH];
    char message[MESSAGE_LENGTH];
    char datetime[DATE_LENGTH];
} Message;

typedef struct sharedData 
{
    Message message_list[MAX_MESSAGES];
    char client_list[MAX_CLIENTS][NAME_LENGTH];
    int messages_num;
    int clients_num;
} SharedData;

#endif