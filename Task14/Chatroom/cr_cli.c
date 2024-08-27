#include "common.h"
#include "chatroom.h"
#include "ncursescr.h"
#include "semV.h"
#include <sys/ioctl.h>

static int shmid;
static int semid;
int semaphore_number;
static pthread_t receiving_thread, sending_thread;
char client_name[NAME_LENGTH];
SharedData* chat;

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

    /* Открываем участок разделяемой памяти */    
    shmid = shmget(SHM_KEY, sizeof(SharedData), 0644);
    if (shmid == -1) 
    {
        err_exit("shmget");
    }

    chat = (SharedData*) shmat(shmid, NULL, 0);
    if (chat == (void*) -1) 
    {
        err_exit("shmat");
    }
    
    /* Открываем семафор */  
    semid = semget(SEM_KEY, MAX_CLIENTS, 0644);
    if (semid == -1) 
    {
        shmdt(chat);
        err_exit("semget");
    }

    /* Инициализируем семафор и добавляем клиента */  
    semaphore_number = chat->clients_num;    
    
    strncpy(chat->client_list[chat->clients_num], client_name, NAME_LENGTH);
    chat->clients_num++;
    
    semaphore_number = -1;
    for (int i = 0; i < chat->clients_num; i++) 
    {
        if (strcmp(chat->client_list[i], client_name) == 0) 
        {
            semaphore_number = i;
        }
    }

    sem_init(semid, semaphore_number);     
   
    /* Подключаем интерфейс ncurses */
    init_ncurses();
    create_windows();

    /* Создаем потоки для обмена сообщениями между сервером и клиентом */
    if (pthread_create(&sending_thread, NULL, handle_message, chat) != 0) 
    {
        err_exit("pthread_create sending_thread");
    }
    if (pthread_create(&receiving_thread, NULL, handle_response, chat) != 0)
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
    for (int i = 0; i < chat->clients_num; i++) 
    {
        if (strcmp(chat->client_list[i], client_name) == 0) 
        {
            for (int j = i; j < chat->clients_num - 1; j++) 
            {
                strncpy(chat->client_list[j], chat->client_list[j + 1], NAME_LENGTH);
            }
            chat->clients_num--;
            break;
        }
    }   
    printf("Клиент вышел из чата...\n");     
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
void* handle_response (void* arg) 
{
    SharedData* chat = (SharedData*) arg;
    
    while (1)
    {   
        sem_reserve(semid, semaphore_number);                        
        print_clients(chat->client_list, chat->clients_num);                       
        print_messages(chat->message_list, chat->messages_num);
        sem_release(semid, semaphore_number);        
    }
}

/* Функция для запуска потока-отправителя (sending_thread) сообщений серверу */
void* handle_message (void* arg) 
{
    SharedData* chat = (SharedData*) arg;
    char client_message[MESSAGE_LENGTH];
    
    while (1)
    {   
        sem_reserve(semid, semaphore_number);     
        werase(input_wnd);
                
        if (mvwgetnstr(input_wnd, 0, 0, client_message, MESSAGE_LENGTH) == ERR) 
        {
            ERROR_MESSAGE_NCURS("error reading from keyboard");        
        }        
        
        
        strncpy(chat->message_list[chat->messages_num].client_name, client_name, NAME_LENGTH);
        strncpy(chat->message_list[chat->messages_num].message, client_message, MESSAGE_LENGTH);
        time_t current_time = time(NULL);
        struct tm *local_time = localtime(&current_time);
        char formatted_time[DATE_LENGTH];
        strftime(formatted_time, DATE_LENGTH, "%Y-%m-%d %H:%M:%S", local_time);
        strncpy(chat->message_list[chat->messages_num].datetime, formatted_time, DATE_LENGTH);
        chat->messages_num++;
        sem_release(semid, semaphore_number);             
    }       
}

/* Функция завершает работу потоков */
void cancel_pthreads (void) 
{
    pthread_cancel(sending_thread);
    pthread_cancel(receiving_thread);    
}








