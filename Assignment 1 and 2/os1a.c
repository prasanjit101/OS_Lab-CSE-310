#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <utmp.h>

int c1 = 0, c2 = 0;
//counter variables
int main()
{
	pid_t id;
	id = fork();

	if (id == 0)
	{
		//child process
		c1++;
		pid_t id2;
		id2 = fork();
		if (id2 == 0)
		{
			//grand child process
			c2++;
			c1++;
			printf("The grandchild has printed roll number as CSB19057\n");
			printf("No of Child = %d ,No of GrandChild = %d \n", c1, c2);
		}
		else if (id2 > 0)
		{
			//child process
			printf("Child: PID = %d , PPID = %d \n", getpid(), getppid());
		}
		else
		{
			//error
			exit(1);
		}
	}
	else if (id > 0)
	{
		//parent process
		printf("Number of Child Processes : %d", c1);
		printf("\nNumber of Grandchild Processes : %d", c2);
		exit(0);
	}
	else
	{
		//if -1 returned by fork
		exit(1);
	}
	return 0;
}
