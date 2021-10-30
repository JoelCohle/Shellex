#include "headers.h"
#include "utils.h"

void tokenise(char* string, char** arr, char* delimiter)
{
    char *tempstr = strdup(string);
    char *token, *randstr;
    int i = 0;
    token = (char *)malloc(1024 * sizeof(char));
    if (token == NULL)
    {
        perror("Malloc failed");
        return;
    }
    for (token = strtok_r(tempstr, delimiter, &randstr); token; token = strtok_r(NULL, delimiter, &randstr)){
        strcpy(arr[i], token);
        i++;
    }
    arr[i] = 0;
}

int cmpfunc (const void * a, const void * b) 
{
    Process* A = ((Process*) a);
    Process* B = ((Process*) b);
    int val = strcmp(A->command,B->command);
    if (val != 0)
        return val;
    else
        return (A->pid - B->pid);
}

int strtonum(char* str)
{
    int num = 0;
    for (int i = 0; i < strlen(str); ++i)
    {
        num *= 10;
        int val = str[i] - '0';
        if (val > 9 || val < 0)
        {
            fprintf(stderr, "Number not provided\n");
            return -1;
        }
        num += val;
    }
    return num;
}