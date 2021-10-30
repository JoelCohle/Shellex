#include "headers.h"

void pinfo(char **command, char *home_dir)
{

    char pid[40] = {'\0'};
    if(!command[1]){
        int curr_pid = getppid();
        sprintf(pid, "%d", curr_pid);
    }
    else
        strcpy(pid,command[1]);

    char cur[SIZE] = "/proc/";
    char proc_stat[SIZE] = {'\0'};

    strcat(cur, pid);
    strcpy(proc_stat, cur);
    strcat(proc_stat, "/stat");

    FILE *file = fopen(proc_stat, "r");

    if(!file){
        fprintf(stderr, "No process with pid: %s\n", pid);
        return;    
    }
    else{
        char f[SIZE] = {'0'}, s[SIZE] = {'\0'}, t[SIZE] = {'\0'};
        fscanf(file, "%s %s %s", f, s, t);

        printf("pid -- %s\n", pid);
        printf("Process Status -- %s\n", t);
    }

    char virtualMem[SIZE] = {'\0'};

    strcpy(virtualMem, cur);
    strcat(virtualMem, "/statm");

    file = fopen(virtualMem, "r");

    char f[SIZE] = {'\0'};
    fscanf(file, "%s", f);
    printf("memory -- %s  {Virtual Memory}\n", f);

    char sendPath[SIZE] = {'\0'};
    strcpy(sendPath, cur);
    strcat(sendPath, "/exe");

    char exec_path[SIZE];

    readlink(sendPath, exec_path, SIZE);
    exec_path[strlen(exec_path)] = '\0';

    int n1 = strlen(home_dir);
    int n2 = strlen(exec_path);

    int i = 0;
    while( i < n1 && i < n2)
    {
        if(exec_path[i] != home_dir[i])
            break;
        i++;
    }

    if(i == n1)
    {
        if(n1 == n2 || (n2 > n1 && exec_path[n1] == '/'))
        {
            exec_path[0] = '~';
            while(i<n2){
                exec_path[i-n1+1] = exec_path[i];
                i++;
            }
            exec_path[n2-n1+1] = '\0';
        }
    }

    printf("Executable Path -- %s\n", exec_path);
}