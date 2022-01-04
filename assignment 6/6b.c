#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>

int main()
{

    pid_t child;
    struct timeval A,B;
    struct tms a,b;
    child = fork();
    if (child < 0)
        perror("Fork error\n");
    else if (child == 0)
    {
        printf("Under the child process :\n");
        int i, j, k=0;
        times(&b);
        gettimeofday(&B,NULL);
        for (i = 0; i < 1000; i++)
            for (j = 0; j < 1000; j++)k++;
        times(&a);
        gettimeofday(&A, NULL);
        FILE *fp;
        fp = fopen("tmp.txt", "w");
        fprintf(fp, "Submission time of child process from times: %ld seconds\n", b.tms_utime);
        fprintf(fp, "Submission time of child process from gettimeofday(): %ld seconds\n", B.tv_usec);
        fprintf(fp, "Termination time of child process from times: %ld seconds\n", a.tms_utime);
        fprintf(fp, "Termination time of child process from gettimeofday(): %ld seconds\n", A.tv_sec);
        printf("Child Exiting...\n");
        exit(0);
    }
    else
    {
        printf("This is under the parent process\n");
        wait(NULL);
        printf("Parent Exiting...\n");
    }
    exit(0);
}