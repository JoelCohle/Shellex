#include "headers.h"
#include "replay.h"
#include "execute.h"

void replay(char** arr, char *home_dir, char *prev_dir)
{
    int period = 0, interval = 0, flags[3] = {0};
    char *command = (char *)malloc(2*SIZE * sizeof(char));
    memset(command, 0, 2*SIZE);
    
    for (int i = 1; arr[i] ; i++)
    {
        if(arr[i+1] && !strcmp(arr[i], "-command")){
            int j = 0;
            flags[0] = 1;
            while(arr[i+1] && strcmp(arr[i+1], "-interval") && strcmp(arr[i+1], "-period")){
                strcat(command, arr[i+1]), strcat(command, " ");
                i++;
            }
        }
        else if (arr[i+1] && !strcmp(arr[i], "-interval")){
            interval = strtonum(arr[++i]);
            flags[1] = 1;
            if (interval == -1){
                fprintf(stderr, "Interval is not a valid number\n");
                return;
            }
        }
        else if (arr[i+1] && !strcmp(arr[i], "-period")){
            period = strtonum(arr[++i]);
            flags[2] = 1;
            if (interval == -1){
                fprintf(stderr, "Period is not a valid number\n");
                return;
            }
        }
        else{
            fprintf(stderr, "Invalid command");
            return;
        }
    }
    if (!flags[0] || !flags[1] || !flags[2]){
        fprintf(stderr, "Invalid command");
        return;
    }
    int num = period/interval;
    for (int i = 0; i < num; ++i)
    {
        sleep(interval);
        runcommand(command, home_dir, prev_dir);
    }
    
    free(command);
}