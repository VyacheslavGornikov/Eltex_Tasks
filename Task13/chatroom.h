#ifndef _CHATROOM_H_
#define _CHATROOM_H_

#define BUF_SIZE 4096
#define MESSAGE_LENGTH 255
#define NAME_LENGTH 30
#define DATE_LENGTH 30
#define SERVER_KEY 0x1aaaaaa1

enum RECEIVER 
{
    SERVER = 1,
    CLIENT = 2
};

enum REQUEST_TYPE 
{
    REGISTRATION = 1,
    DISCONNECTION = 2,
    MESSAGE_INFO = 3,
    CLIENT_INFO = 4
};

typedef struct message 
{
    char client_name[NAME_LENGTH];
    char message[MESSAGE_LENGTH];
    char datetime[DATE_LENGTH];
} Message;

typedef struct client 
{
    int client_id;
    char client_name[NAME_LENGTH];    
} Client;

typedef struct requestMsg 
{
    long mtype;
    int client_id;
    int request_type;
    char mtext[MESSAGE_LENGTH];
} RequestMsg;

typedef struct responseMsg 
{    
    long mtype;
    int response_type;
    int list_size;
    Message msg;        
} ResponseMsg;



#endif