#include "headers.h"
#include "ctrl.h"

void ctrlc(int signal)
{
    if (fgproc.pid > 0)
    {
        kill(fgproc.pid, SIGINT);
        deletefgproc();
    }
    printf("\r");
}

void ctrlz(int signal)
{
    printf("\r");
    if (fgproc.pid != -1)
    {
        fprintf(stderr, "%d", fgproc.pid);
        kill(fgproc.pid, SIGTSTP);
        newbgproc(fgproc.command,fgproc.pid, fgproc.pname);
        deletefgproc();
    }
}

