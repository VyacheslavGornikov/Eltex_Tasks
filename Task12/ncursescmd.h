#ifndef _NCURSESCMD_H_
#define _NCURSESCMD_H_
#define MAX_COMMANDS 10
#define COMMAND_LENGTH 256
void input_command (char* cmd, int active_window);
void execute_command(char* cmd, char files[][256], int *num_files, char* path);
#endif