#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "headers.h"

void echo(char** command);
void chd(char** command, char* home_dir, char* prev_dir);
void pwd();
void repeat(char **arr, char *home_dir, char *prev_dir);

#endif