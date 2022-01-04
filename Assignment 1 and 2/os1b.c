#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>
#include <sys/wait.h>

int who()
{
    struct utmp *u;
    // setutent() rewinds the file pointer to the beginning of the utmp  file.
    setutent();
    // getutent()  reads  a  line  from  the current file position in the utmp file.
    while ((u = getutent()) != NULL)
    {
        if (u->ut_type != USER_PROCESS)
            continue;
        printf("\nLogged in User:\t%s\n", u->ut_user);
    }
    endutent();
    // endutent() closes the utmp file.
    exit(0);
}

int main()
{
    pid_t id, gid;
    id = fork();
    if (id == 0)
    {
        //child process
        gid = fork();
        if (gid == 0)
        {
            //grandchild process
            who();
            exit(0);
        }
        else if (gid > 0)
        {
            //child process
            wait(NULL);
            exit(0);
        }
    }
    else if (id > 0)
    {
        wait(NULL);
        //parent process
    }
    else
    {
        // when fork returns -1 i.e error in fork
        exit(1);
    }
    return 0;
}