#include "prompt.h"
#include "headers.h"
#include "commands.h"

void dir_name(char* curr_dir, char* home_dir, char rel_dir[1024])
{
    if (!(strcmp(curr_dir,home_dir))){
        sprintf(rel_dir, "~");
    }
    else if(!(strstr(curr_dir, home_dir))){
        sprintf(rel_dir, "%s", curr_dir);
    }
    else{
        sprintf(rel_dir, "~%s", curr_dir + strlen(home_dir));
    }
}

void prompt(char* home_dir) {

    int size = 2048;
    char prompt[size], sys_name[200], username[200];

    int dir_size = 1024;
    char* curr_dir = (char *)malloc(dir_size * sizeof(char));
    if (curr_dir == NULL)
    {
        perror("malloc failed");
        return;
    }
    char rel_dir[1024] = {'\0'};

    getcwd(curr_dir, dir_size);
    gethostname(sys_name, 200);
    getlogin_r(username, 200);

    dir_name(curr_dir, home_dir, rel_dir);

    printf("\033[0;32m");
    printf("<%s@%s:%s>", username, sys_name, rel_dir);
    printf("\033[0m");
}
