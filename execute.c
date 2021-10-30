#include "headers.h"
#include "execute.h"

void runcommand(char *command, char *home_dir, char *prev_dir)
{
    int pipefd[2];
    size_t size = 1024;
    int stdin_fd = dup(0);
    int stdout_fd = dup(1);
    char **pipecmd;
    pipecmd = (char **)malloc(50 * sizeof(char *));
    for (int k = 0; k < 50; k++)
    {
        pipecmd[k] = (char *)malloc(size * sizeof(char));
        memset(pipecmd[k], 0, size);
    }
    tokenise(command, pipecmd, "|");
    for (int j = 0; pipecmd[j]; j++)
    {
        if (pipecmd[j + 1])
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe error");
                break;
            }
            changeIO(pipefd[1], STDOUT_FILENO);
        }
        else
            changeIO(stdout_fd, STDOUT_FILENO);

        char **arr;
        arr = (char **)malloc(50 * sizeof(char *));
        for (int k = 0; k < 50; k++)
        {
            arr[k] = (char *)malloc(size * sizeof(char));
            memset(arr[k], 0, size);
        }
        tokenise(pipecmd[j], arr, " \r\t");

        int *flags = (int *)malloc(3 * sizeof(int));
        flags[0] = 0, flags[1] = 0, flags[2] = 0;
        char *outputfile = (char *)malloc(size * sizeof(char));
        char *inputfile = (char *)malloc(size * sizeof(char));

        int newfd = dup(STDOUT_FILENO);
        redir_check(arr, flags, outputfile, inputfile);
        redirectIO(flags, outputfile, inputfile);

        if (strcmp(arr[0], "repeat") == 0)
            repeat(arr, home_dir, prev_dir);
        else if (strcmp(arr[0], "echo") == 0)
            echo(arr);
        else if (strcmp(arr[0], "pwd") == 0)
            pwd();
        else if (strcmp(arr[0], "cd") == 0)
            chd(arr, home_dir, prev_dir);
        else if (strcmp(arr[0], "ls") == 0)
            ls(arr, home_dir);
        else if (strcmp(arr[0], "pinfo") == 0)
            pinfo(arr, home_dir);
        else if (strcmp(arr[0], "jobs") == 0)
            jobs(arr);
        else if (strcmp(arr[0], "sig") == 0)
            sig(arr);
        else if (strcmp(arr[0], "bg") == 0)
            bg(arr);
        else if (strcmp(arr[0], "fg") == 0)
            fg(arr);
        else if (strcmp(arr[0], "replay") == 0)
            replay(arr, home_dir, prev_dir);
        else
            bg_fg(arr);

        changeIO(newfd, STDOUT_FILENO);
        if (pipecmd[j + 1])
            changeIO(pipefd[0], STDIN_FILENO);
    }
    changeIO(stdin_fd, STDIN_FILENO);

}