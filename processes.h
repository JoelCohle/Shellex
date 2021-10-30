#ifndef __JOBS_H
#define __JOBS_H

int findproc(char **arr);
void jobs(char** arr);
void sig(char** arr);
void bg(char** arr);
void fg(char **arr);
void addFgproc(int pid, char **command);
void deletefgproc();
void deletebgproc(int procindex);
void newbgproc(char *command, int pid, char* cname);
void addbgproc(char **command, int pid);

#endif