#include "common.h"
#include "ncursesdirmanager.h"
#include <dirent.h>



/*
 * Функция отображает в окне список файлов директории
 * 
 * wnd - указатель на структуру WINDOW
 * files[][256] - массив файлов директории
 * num_files - количество файлов в директории
 * selected - номер файла, выделенного курсором
 */
void show_directory(WINDOW *wnd, char files[][256], int num_files, int selected)
{
    for (int i = 0; i < num_files; i++)
    {
        if (i == selected)
        {
            wattron(wnd, A_REVERSE);
        }
        mvwprintw(wnd, i + 1, 2, "%s", files[i]);
        wattroff(wnd, A_REVERSE);
    }
    wrefresh(wnd);
}

/*
 * Функция читает директорию по переданному пути и записывает список файлов директории в массив
 * 
 * files[][256] - массив файлов директории
 * num_files - количество файлов в директории
 * dir_path - путь до директории  
 */
void read_directory(char files[][256], int* num_files, char* dir_path) 
{
    DIR* dir = NULL;
    struct dirent* ent;
    struct stat st;

    *num_files = 0;
    if ((dir = opendir(dir_path)) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            if (strcmp(ent->d_name, ".") != 0) 
            {
                char full_path[256];
                snprintf(full_path, sizeof(full_path) * 2, "%s/%s", dir_path, ent->d_name);
                
                // Получаем информацию о файле с помощью stat
                if (stat(full_path, &st) == 0) 
                {
                    // Проверяем тип объекта
                    if (S_ISDIR(st.st_mode)) 
                    { 
                        // Это директория
                        strncpy(files[*num_files], ent->d_name, 256 * sizeof(char));
                        // Добавляем "/" в начало имени директории
                        memmove(files[*num_files] + 1, files[*num_files], strlen(files[*num_files]) + 1); 
                        files[*num_files][0] = '/';
                    } 
                    else  
                    {
                        // Это файл
                        strncpy(files[*num_files], ent->d_name, 256 * sizeof(char)); 
                    }
                    (*num_files)++;
                } 
                else 
                {
                    clear();
                    mvprintw(0, 0, "Error reading info from stat\n");
                    printw("%s\n", strerror(errno));
                    printw("Program finished. Press any key to exit...\n");                    
                    getch();
                    endwin();
                    exit(EXIT_FAILURE);
                }
            }           

        }
        closedir(dir);

        // Сортировка
        for (int i = 0; i < *num_files - 1; i++) 
        {
            for (int j = i + 1; j < *num_files; j++) 
            {
                if (strcmp(files[i], files[j]) > 0) 
                {
                    char temp[256];
                    strcpy(temp, files[i]);
                    strcpy(files[i], files[j]);
                    strcpy(files[j], temp);
                }
            }
        }
    } 
    else 
    {
        clear();
        mvprintw(0, 0, "Error opening directory: %s\n", dir_path);
        printw("%s\n", strerror(errno));
        strncpy(dir_path, "/home", 256 * sizeof(char));
        read_directory(files, num_files, dir_path);
        printw("You will be returned to path /home. Press any key to continue...\n");
        getch();
    }
}

