#include "processes.h"
#include "headers.h"

Process fgproc;
int jobnum = 1;

void jobs(char **arr)
{
    if (arr[1] && arr[2])
    {
        printf("Too many arguments\n");
        return;
    }
    qsort(processes, pcount, sizeof(Process), cmpfunc);

    for (int i = 0; i < pcount; i++)
    {
        char path[SIZE] = "/proc/", status[SIZE] = {0}, pid[SIZE] = {0};
        sprintf(pid, "%d", processes[i].pid);
        strcat(path, pid);
        strcat(path, "/stat");
        FILE *file = fopen(path, "r");
        char f[SIZE] = {'0'}, s[SIZE] = {'\0'}, t[SIZE] = {'\0'};
        fscanf(file, "%s %s %s", f, s, t);
        if (!strcmp(t, "T"))
            strcpy(status, "Stopped");
        else
            strcpy(status, "Running");

        if (arr[1])
        {
            if (!strcmp(arr[1], "-s") && !strcmp(t, "T"))
                printf("[%d] Stopped %s [%d]\n", processes[i].jobnum, processes[i].command, processes[i].pid);
            else if (!strcmp(arr[1], "-r") && strcmp(t, "T"))
                printf("[%d] Running %s [%d]\n", processes[i].jobnum, processes[i].command, processes[i].pid);
            else if (strcmp(arr[1], "-r") && strcmp(arr[1], "-s"))
            {
                printf("Invalid flag\n");
                return;
            }
        }
        else if (arr[1] == 0)
            printf("[%d] %s %s [%d]\n", processes[i].jobnum, status, processes[i].command, processes[i].pid);
    }
}

void sig(char **arr)
{
    qsort(processes, pcount, sizeof(Process), cmpfunc);
    int cnt = 0;
    while (arr[cnt])
        cnt++;
    if (cnt != 3)
    {
        fprintf(stderr, "Incorrect number of arguments!\n");
        return;
    }
    int procindx = findproc(arr);
    if (procindx == -1)
    {
        fprintf(stderr, "Job doesn't exist\n");
        return;
    } 
    
    int signum = strtonum(arr[2]);
    if (kill(processes[procindx].pid, signum) < 0)
        perror("Kill error");
    return;
}

int findproc(char **arr)
{
    int jobnum = strtonum(arr[1]);
    if (jobnum < 1)
    {
        perror("Jobnumber doesn't exist");
        return -1;
    }
    for (int i = 0; i < pcount; i++)
        if (processes[i].jobnum == jobnum)
            return i;
    return -1;
}

void bg(char **arr)
{
    qsort(processes, pcount, sizeof(Process), cmpfunc);
    int cnt = 0;
    while (arr[cnt])
        cnt++;
    if (cnt != 2)
    {
        fprintf(stderr, "Incorrect number of arguments!\n");
        return;
    }
    int procindx = findproc(arr);
    if (procindx == -1)
    {
        fprintf(stderr, "Job doesn't exist\n");
        return;
    }

    if (kill(processes[procindx].pid, SIGCONT))
        perror("Process couldn't be continued");
    return;
}

void fg(char **arr)
{
    qsort(processes, pcount, sizeof(Process), cmpfunc);
    int cnt = 0;
    while (arr[cnt])
        cnt++;
    if (cnt != 2)
    {
        fprintf(stderr, "Incorrect number of arguments!\n");
        return;
    }
    int procindx = findproc(arr);

    if (procindx == -1)
    {
        fprintf(stderr, "Job doesn't exist\n");
        return;
    }

    char temp[SIZE];
    fgproc.jobnum = processes[procindx].jobnum;
    fgproc.pid = processes[procindx].pid;
    strcpy(fgproc.command, processes[procindx].command);
    strcpy(fgproc.pname, processes[procindx].pname);

    if (kill(processes[procindx].pid, SIGCONT))
        perror("Process couldn't be continued");
    int status;
    if (waitpid(processes[procindx].pid, &status, WUNTRACED) == -1)
        perror("waitpid error");

    deletebgproc(procindx);
    return;
}

void addFgproc(int pid, char **command)
{
    char str[SIZE] = {0};
    for (int i = 0; command[i]; ++i)
        strcat(str, command[i]), strcat(str, " ");

    str[strlen(str) - 1] = 0;

    fgproc.jobnum = jobnum;
    strcpy(fgproc.command, str);
    fgproc.pid = pid;
    strcpy(fgproc.pname, command[0]);
}

void deletefgproc()
{
    fgproc.pid = -1;
    memset(fgproc.pname, 0, SIZE);
    memset(fgproc.command, 0, SIZE);
}

void deletebgproc(int procindex)
{
    while (procindex < pcount - 1)
    {
        processes[procindex].jobnum = processes[procindex + 1].jobnum;
        processes[procindex].pid = processes[procindex + 1].pid;
        strcpy(processes[procindex].pname, processes[procindex + 1].pname);
        strcpy(processes[procindex].command, processes[procindex + 1].command);
        procindex++;
    }
    pcount--;
}

void newbgproc(char *command, int pid, char *cname)
{
    processes[pcount].jobnum = jobnum;
    processes[pcount].pid = pid;
    strcpy(processes[pcount].pname, cname);
    strcpy(processes[pcount].command, command);
    pcount++, jobnum++;
}

void addbgproc(char **command, int pid)
{
    char str[SIZE] = {0};
    for (int j = 0; command[j]; j++)
    {
        strcat(str, command[j]);
        strcat(str, " ");
    }
    newbgproc(str, pid, command[0]);
}
