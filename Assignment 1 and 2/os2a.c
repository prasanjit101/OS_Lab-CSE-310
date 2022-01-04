#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
        void handler(int s); /* prototype */
        //SIGINT - Interactive attention signal. Expands to:  2
        signal(SIGINT, handler);
        printf("Running : \n");
        while (1)
        {
                sleep(5);
        }
        return 0;
}

void handler(int s)
{
        printf("\nUser function called: \n");
        pid_t id;
        printf("\ns : %d",s);
        id = fork();
        if (id == 0)
        {
                //child process
                printf("Child: PID = %d , PPID = %d \n", getpid(), getppid());
        }
        else if (id > 0)
        {
                //parent process
                wait(NULL);
                printf("Parent: PID = %d , PPID = %d  \n", getpid(), getppid());
        }
        else
        {
                exit(1);
        }
        exit(0);
}

//CSB19057