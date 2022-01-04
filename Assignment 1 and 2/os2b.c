#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    int i;
    if (argc != 2)
    {
        printf("Invalid , Enter a single process id\n");
        exit(1);
    }
    //Convert a string to an integer.
    i = atoi(argv[1]);
    //The command kill sends the specified signal to the specified processes or process groups
    kill(argv[1], SIGINT);
    return 0;
}

//CSB19057