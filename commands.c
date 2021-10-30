#include "headers.h"
#include "commands.h"
#include "execute.h"

void echo(char **command)
{
    for (int i = 1; command[i] ; i++)
    {
        printf("%s ", command[i]);
    }
    printf("\n");
}

void pwd()
{
    size_t size = 1024;
    char *curr_dir = (char *)malloc(size * sizeof(char));
    if (curr_dir == NULL)
    {
        perror("malloc failed");
        return;
    }
    getcwd(curr_dir, size);
    printf("%s\n", curr_dir);
}

void chd(char **command, char *home_dir, char *prev_dir)
{
    size_t size = 1024;
    char *curr_dir = (char *)malloc(size * sizeof(char));
    if (curr_dir == NULL)
    {
        perror("malloc failed");
        return;
    }
    getcwd(curr_dir, size);

    if (command[1] && command[2])
    {
        fprintf(stderr, "Invalid number of arguments for cd\n");
        return;
    }
    else if (!command[1] || !(strcmp(command[1], "~")))
    {
        chdir(home_dir);
    }
    else if (!(strcmp(command[1], "-")))
    {
        chdir(prev_dir);
    }
    else
    {
        if (chdir(command[1]))
        {
            perror("cd error");
            return;
        }
    }
    strcpy(prev_dir, curr_dir);
    getcwd(curr_dir, 1024);
    //free(dir);
}

void repeat(char **arr, char *home_dir, char *prev_dir)
{
    int num = strtonum(arr[1]);
    char *command = (char *)malloc(2*SIZE * sizeof(char));
    memset(command, 0, 2*SIZE);
    for (int j = 0; arr[j + 2] ; j++)
    {
        strcat(command, arr[j+2]), strcat(command, " ");
    }
    for (int j = 0; j < num; j++)
    {
        runcommand(command, home_dir, prev_dir);
    }
}