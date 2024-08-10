#include "common.h"
#include "ncursescmd.h"

/*
 * Функция создает окно для ввода команды с клавиатуры и сохраняет ее в переменной cmd
 *
 * cmd - переменная для хранения введенной с клавиатуры команды
 * active_window - переменная для отслеживания активного в текущий момент окна
 */
void input_command (char* cmd, int active_window) 
{   
    /* Включаем видимость курсора и отображение нажатых на клавиатуре клавиш, получаем размер окна */
    curs_set(TRUE);
    echo();
    int x_max, y_max;
    getmaxyx(stdscr, y_max, x_max);

    WINDOW* cmd_wnd;
    if (active_window == 1) 
    {
        cmd_wnd = newwin(3, x_max / 2 - 5, y_max - 5, 2);
    }
    else if (active_window == 2) 
    {
        cmd_wnd = newwin(3, x_max / 2 - 5, y_max - 5, x_max / 2 + 2);
    }
    
    if (cmd_wnd == NULL) 
    {
        ERROR_MESSAGE("Error creating cmd_wnd");        
    }

    box(cmd_wnd, '$', '$');
    mvwprintw(cmd_wnd, 1, 1, "Enter command: ");
    wrefresh(cmd_wnd);

    if (wgetnstr(cmd_wnd, cmd, COMMAND_LENGTH - 1) == ERR) 
    {
        ERROR_MESSAGE("error reading from keyboard");        
    }
    /* Отключаем видимость курсора и отображение нажатых на клавиатуре клавиш, удаляем окно */    
    curs_set(FALSE);
    noecho();
    delwin(cmd_wnd);
}

/*
 * Функция выполняет введенную с клавиатуры команду и сохраняет вывод в массив files
 *
 * cmd - переменная для хранения выполняемой команды
 * files[][256] - массив, хранящий результат выполнения команды
 * num_files - количество записей в массиве files 
 * path - путь до директории, где будет выполняться команда
 */
void execute_command(char* cmd, char files[][256], int *num_files, char* path) 
{    
    int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        ERROR_MESSAGE("pipe error");         
    }

    pid_t pid = fork();
    if (pid == 0) 
    {   // Дочерний процесс
        if(close(pipefd[0] == -1)) // Закрываем дескриптор чтения
        {
            ERROR_MESSAGE("error closing pipefd[0]");            
        } 
        if(dup2(pipefd[1], STDOUT_FILENO) == -1) // Перенаправляем вывод в канал
        {
            ERROR_MESSAGE("error duplicate fd");           
        }
        if(close(pipefd[1]) == -1) 
        {
            ERROR_MESSAGE("error closing pipefd[1]");            
        }
        if (chdir(path) == -1) 
        {
            ERROR_MESSAGE("error chdir");             
        }
        execlp("sh", "sh", "-c", cmd, NULL); // Выполняем команду
        perror("execlp");
        exit(EXIT_FAILURE);
    } 
    else if (pid > 0) 
    {   // Родительский процесс
        close(pipefd[1]); // Закрываем дескриптор записи
        int file_count = 0;
        char buffer[4096];
        ssize_t bytes_read;       
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) 
        {
            char *line = strtok(buffer, "\n");
            while (line != NULL) 
            {
                strncpy(files[file_count++], line, strlen(line) + 1);
                line = strtok(NULL, "\n");
            }
        }
        
        *num_files = file_count - 1;
        close(pipefd[0]);
        wait(NULL); // Ждем завершения дочернего процесса
    } 
    else 
    {
        perror("fork");
        return;
    }
}