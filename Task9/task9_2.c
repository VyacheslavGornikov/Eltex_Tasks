
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <ncurses.h>
#include <string.h>
#include <errno.h>
#include "ncursesdirmanager.h"


#define MAX_FILES 100

void sig_winch(int signo);
void print_box(WINDOW *wnd);
void init_settings();


int main(void) 
{
    WINDOW* wnd1 = NULL;
    WINDOW* wnd2 = NULL;

    // Инициализация окна и получение его размеров
    initscr();
    int x_max, y_max;
    getmaxyx(stdscr, y_max, x_max);

    signal(SIGWINCH, sig_winch); // обработка изменения размеров окна

    init_settings();
    // включает считывание символов с клавиатуры
    keypad(stdscr, TRUE);
    keypad(wnd1, TRUE);
    keypad(wnd2, TRUE);
    refresh();

    // Отдельные переменные для выделенного файла в каждом окне
    int selected1 = 0; // номер выбранной директории для окна 1
    int selected2 = 0; // номер выбранной директории для окна 2

    int num_files = 0; // количество объектов в директории
    char files[MAX_FILES][256]; // массив файлов директории, где 256 - максимальная длина имени файла 

    // Храним пути для каждого окна
    char path1[256] = "/home"; // путь для окна 1
    char path2[256] = "/home"; // путь для окна 2

    // инициализация цветовой пары текст-фон
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 

    // Инициализация окон
    wnd1 = newwin(y_max - 1, x_max / 2, 0, 0);
    if (wnd1 == NULL) 
    {
        perror("Error creating window!");
        getch();
        endwin();
        exit(EXIT_FAILURE);
    }
    wbkgd(wnd1, COLOR_PAIR(1) | A_BOLD);
    print_box(wnd1);

    wnd2 = newwin(y_max - 1, x_max / 2, 0, x_max / 2);
    if (wnd2 == NULL) 
    {
        perror("Error creating window!");
        getch();
        endwin();
        exit(EXIT_FAILURE);
    }
    wbkgd(wnd2, COLOR_PAIR(1) | A_BOLD);
    print_box(wnd2);

    // Первоначальная загрузка и отображение содержимого директорий
    read_directory(files, &num_files, path1);
    show_directory(wnd1, files, num_files, selected1);
    read_directory(files, &num_files, path2);
    show_directory(wnd2, files, num_files, selected2);

    // Переменная для отслеживания активного окна
    int active_window = 1; 

    while (1) 
    {
        mvprintw(y_max - 1, 0, "Press q to quit, W - up, S - down, Tab - switch window");
        refresh();

        // Отображение файлов в активном окне
        if (active_window == 1) 
        {
            // Отображаем файлы для окна 1
            show_directory(wnd1, files, num_files, selected1);
        } 
        else if (active_window == 2) 
        {
            // Отображаем файлы для окна 2
            show_directory(wnd2, files, num_files, selected2);
        }

        int ch = getch();

        // Обработка навигации для активного окна
        if (active_window == 1) 
        {
            if (ch == 'w' || ch == KEY_UP) 
            {
                selected1 = (selected1 - 1 + num_files) % num_files;
            } 
            else if (ch == 's' || ch == KEY_DOWN) 
            {
                selected1 = (selected1 + 1) % num_files;
            }
        } 
        else if (active_window == 2) 
        {
            if (ch == 'w' || ch == KEY_UP) 
            {
                selected2 = (selected2 - 1 + num_files) % num_files;
            } 
            else if (ch == 's' || ch == KEY_DOWN) 
            {
                selected2 = (selected2 + 1) % num_files;
            }
        }

        if (ch == 10) // Enter
        { 
            // Обработка Enter для текущего активного окна
            if (active_window == 1) 
            {
                strcat(path1, "/");
                strcat(path1, files[selected1]);
                read_directory(files, &num_files, path1);
                wclear(wnd1);                
                selected1 = 0; 
            } 
            else if (active_window == 2) 
            {
                strcat(path2, "/");
                strcat(path2, files[selected2]);
                read_directory(files, &num_files, path2);
                wclear(wnd2);                
                selected2 = 0; 
            }

            print_box(wnd1);
            print_box(wnd2);

        }         
        else if (ch == 9) // Tab
        { 
            // Переключение окон
            active_window = (active_window % 2) + 1;

            // Обновляем путь для активного окна
            if (active_window == 1) 
            {
                read_directory(files, &num_files, path1); 
                wclear(wnd1);
                print_box(wnd1);
            } 
            else if (active_window == 2) 
            {
                read_directory(files, &num_files, path2); 
                wclear(wnd2);
                print_box(wnd2);
            }
        }
        else if (ch == 'q') // выход из программы при нажатии q
        {
            break;
        } 
    }

    // Высвобождает память, выделенную под окна
    delwin(wnd1);
    delwin(wnd2);
    // Восстанавливает состояние терминала до ncurses
    endwin();
    return 0;
}

/*
 * Начальные настройки работы терминала
 */
void init_settings()
{
    cbreak();
    noecho();
    curs_set(FALSE);
    start_color();    
}

/*
 * Функция печатает рамку для окна
 *
 * wnd - указатель на структуру WINDOW
 */
void print_box(WINDOW *wnd)
{
    box(wnd, '|', '-');
    wrefresh(wnd);
}

/*
 * Функция обрабатывает изменения размера окна терминала
 */
void sig_winch(int signo) 
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*) &size);
    resizeterm(size.ws_row, size.ws_col);
}


