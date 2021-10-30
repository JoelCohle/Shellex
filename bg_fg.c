#include "headers.h"
#include "bg_fg.h"
#include "prompt.h"
#include "processes.h"

int pcount = 0;
Process processes[50];

void handler(int sig)
{
    int stat;
    pid_t pid = waitpid(-1, &stat, WNOHANG);

    if (pid > 0)
    {
        char name[40];
        int procindex = -1;
        for (int i = 0; i < pcount; i++)
            if (processes[i].pid == pid)
            {
                strcpy(name, processes[i].pname);
                procindex = i;
                break;
            }
        deletebgproc(procindex);

        fprintf(stderr, "%s with pid %d exited with exit status %d\n", name, pid, WEXITSTATUS(stat));
        prompt(home_dir);
        fflush(stdout);
    }
    return;
}

void bg_fg(char **command)
{
    int i = 0;
    while (command[i])
        i++;
    int bg_flag = !strcmp(command[i - 1], "&");

    if (bg_flag)
    {
        command[i - 1] = 0;
        int pid = fork();

        if (pid < 0)
            perror(command[0]);

        if (pid == 0)
        {
            setpgid(0,0);
            if (execvp(command[0], command) == -1)
            {
                printf("%s: Command Not Found\n", command[0]);
                return;
            }
        }

        if (pid > 0)
        {
            printf("%d\n", pid);
            setpgid(pid,0);
            addbgproc(command, pid);
        }
    }
    else
    {
        int pid = fork();
        int status;

        if (pid < 0)
            perror(command[0]);

        if (pid == 0)
        {
            if (execvp(command[0], command) == -1)
            {
                printf("%s: Command Not Found\n", command[0]);
                return;
            }
        }

        if (pid > 0)
        {
            addFgproc(pid, command);
            if (waitpid(pid, &status, WUNTRACED) == -1)
                perror("wait() error");
            else
                deletefgproc();
        }
    }
}