#ifndef _NCURSESDIRMANAGER_H_
#define _NCURSESDIRMANAGER_H_
#include <ncurses.h>
void show_directory(WINDOW *wnd, char files[][256], int num_files, int selected);
void read_directory(char files[][256], int* num_files, char* dir_path);
#endif