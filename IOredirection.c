#include "IOredirection.h"
#include "headers.h"

void redir_check(char **arr, int *flags, char *outputfile, char *inputfile)
{
    int count = 0;
    for (int i = 0; arr[i]; i++)
    {
        if (strcmp(arr[i], "<") == 0)
        {
            if (!arr[i + 1])
            {
                fprintf(stderr, "Invalid input\n");
                return;
            }
            flags[0] = 1;
            strcpy(inputfile, arr[i + 1]);
            i++;
        }
        else if (strcmp(arr[i], ">") == 0)
        {
            if (!arr[i + 1])
            {
                fprintf(stderr, "Invalid input\n");
                return;
            }
            flags[1] = 1;
            strcpy(outputfile, arr[i + 1]);
            i++;
        }
        else if (strcmp(arr[i], ">>") == 0)
        {
            if (!arr[i + 1])
            {
                fprintf(stderr, "Invalid input\n");
                return;
            }
            flags[2] = 1;
            strcpy(outputfile, arr[i + 1]);
            i++;
        }
        else
        {
            strcpy(arr[count], arr[i]);
            count++;
        }
    }
    for (int i = count; arr[i]; i++)
    {
        arr[i] = 0;
    }
}

void redirectIO(int *flags, char *outputfile, char *inputfile)
{
    if (flags[0] == 1)
    {
        int inp_fd = open(inputfile, O_RDONLY);
        changeIO(inp_fd, 0);
    }
    if (flags[1] == 1)
    {
        int out_fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        changeIO(out_fd, 1);
    }
    if (flags[2] == 1)
    {
        int out_fd = open(outputfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
        changeIO(out_fd, 1);
    }
}

int changeIO(int oldfd, int newfd)
{
    if (oldfd < 0)
    {
        printf("Bad file descriptor");
        return 0;
    }

    if (dup2(oldfd, newfd) < 0)
    {
        perror("Unable to duplicate file descriptor");
        return 0;
    }

    if (oldfd != newfd)
        if (close(oldfd) == -1)
        {
            perror("close error");
            return 0;
        }
    return 1;
}