#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handlerfunc(int sig)
{
    int pfds[2];
    int i, N, pi[1], ppi[1];
    printf("\nEnter number of terms to display: ");
    scanf("%d", &N);
    int ca[N], pa[N];
    pipe(pfds);
    //pipe is used for interprocess communication
    //here pfds[0] is used to read and pfds[1] is used to write
    if (!fork())
    {
        pi[0] = getpid();
        printf("CHILD PID: %d \n", pi[0]);
        write(pfds[1], pi, sizeof(int));
        //fibonnaci is stored in array ca[n]
        ca[0] = 0;
        ca[1] = 1;
        for (i = 2; i < N; i++)
        {
            ca[i] = ca[i-1]+ca[i-2];
        }
        write(pfds[1], ca, N * sizeof(int));
        printf("CHILD- exiting\n");
        pi[0] = sig;
        write(pfds[1], pi, sizeof(int));
        exit(0);
    }
    else
    {
        wait(NULL);
        printf("PARENT: reading from pipe\n");
        read(pfds[0], ppi, sizeof(int));
        printf("CHILD(PID: %d) :writing from parent\n", ppi[0]);
        read(pfds[0], pa, N * sizeof(int));
        for (i = 0; i < N; i++)
            printf("%d\n", pa[i]);
        read(pfds[0], ppi, sizeof(int));
        printf("signal id for which handler is generated = %d\n", ppi[0]);
        exit(0);
    }
}

int main()
{
    signal(SIGINT, handlerfunc);
    printf("Press Ctrl+C\n");
    while (1)
    {
        sleep(1);
    }
    return 0;
}
