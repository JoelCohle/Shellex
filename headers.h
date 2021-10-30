#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>    
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include "prompt.h"
#include "commands.h"
#include "ls.h"
#include "bg_fg.h"
#include "pinfo.h"
#include "utils.h"
#include "IOredirection.h"
#include "processes.h"
#include "ctrl.h"
#include "replay.h"
#include "execute.h"

#define SIZE 1024

typedef struct process{
    int jobnum;
    pid_t pid;
    char pname[40];
    char command[SIZE];
} Process;

extern Process processes[50];
extern Process fgproc;
extern int pcount;
extern char* home_dir;
extern int jobnum;

#endif