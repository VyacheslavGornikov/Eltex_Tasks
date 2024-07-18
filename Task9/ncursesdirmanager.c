#include "ncursesdirmanager.h"
#include <string.h>
#include <dirent.h>
#include <errno.h>

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

    *num_files = 0;
    if ((dir = opendir(dir_path)) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) 
        {
            if (strcmp(ent->d_name, ".") != 0) 
            {
                strcpy(files[*num_files], ent->d_name);
                (*num_files)++;
            }
        }
        closedir(dir);
    } 
    else 
    {
        clear();
        mvprintw(0, 0, "Error opening directory: %s\n", dir_path);
        printw("%s\n", strerror(errno));
        strcpy(dir_path, "/home");
        read_directory(files, num_files, dir_path);
        printw("You will be returned to path /home. Press any key to continue...\n");
        getch();
    }
}
