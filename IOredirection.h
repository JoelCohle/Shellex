#ifndef __REDIR_H
#define __REDIR_H

#include "headers.h"

void redir_check(char** arr, int* flags, char* outputfile, char* inputfile);
void redirectIO(int *flags, char *outputfile, char *inputfile);
int changeIO(int oldfd, int newfd);

#endif