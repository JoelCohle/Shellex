#include "prompt.h"
#include "headers.h"
#include "commands.h"
#include "ls.h"
#include "bg_fg.h"
#include "pinfo.h"
#include "utils.h"
#include "processes.h"
#include "ctrl.h"
#include "replay.h"
#include "execute.h"

char *home_dir;

int main()
{
    signal(SIGCHLD, handler);

    signal(SIGINT, ctrlc);
	signal(SIGTSTP, ctrlz);
    
    char *prev_dir;
    size_t size = 1024;
    home_dir = (char *)malloc(size * sizeof(char));
    if (home_dir == NULL)
    {
        perror("malloc failed");
        exit(1);
    }
    getcwd(home_dir, size);

    prev_dir = (char *)malloc(size * sizeof(char));
    if (prev_dir == NULL)
    {
        perror("malloc failed");
        exit(1);
    }
    strcpy(prev_dir, home_dir);

    while (1)
    {
        prompt(home_dir);

        size_t inp_size = 2024;
        char *input = (char *)malloc(inp_size * sizeof(char));
        if (input == NULL)
        {
            perror("Malloc failed");
            continue;
        }
        size_t str_len = getline(&input, &inp_size, stdin);
        if (str_len == -1)
            printf("\n"), exit(EXIT_SUCCESS);
        input[str_len - 1] = '\0';

        char **commands;
        commands = (char **)malloc(30 * sizeof(char *));
        for (int i = 0; i < 30; i++)
        {
            commands[i] = (char *)malloc(size * sizeof(char));
            memset(commands[i], 0, size);
        }
        tokenise(input, commands, ";");
        for (int i = 0; commands[i]; i++)
            runcommand(commands[i], home_dir, prev_dir);  

        
    }
}
