#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define MAX_RETRIES 10

union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int initsem(key_t key, int nsems) /* key from ftok() */
{
	int i, semid;
	union semun arg;
	struct semid_ds buf;
	struct sembuf sb;

	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

	if (semid >= 0)
	{ /* we got it first */
		sb.sem_op = 1;
		sb.sem_flg = 0;
		arg.val = 1;
		printf("press return\n");
		getchar();
		for (sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++)
		{
			/* do a semop() to "free" the semaphores. */
			/* this sets the sem_otime field, as needed below. */
			if (semop(semid, &sb, 1) == -1)
			{
				int e = errno;
				semctl(semid, 0, IPC_RMID); /* clean up */
				errno = e;
				return -1; /* error, check errno */
			}
		}
	}
	else if (errno == EEXIST)
	{ /* someone else got it first */
		printf("server\n");
		int ready = 0;

		semid = semget(key, nsems, 0); /* get the id */
		if (semid < 0)
			return semid; /* error, check errno */
		/* wait for other process to initialize the semaphore: */
		arg.buf = &buf;

		for (i = 0; i < MAX_RETRIES && !ready; i++)
		{
			semctl(semid, nsems - 1, IPC_STAT, arg);
			if (arg.buf->sem_otime != 0)
			{
				ready = 1;
			}
			else
			{
				sleep(1);
			}
		}
		if (!ready)
		{
			errno = ETIME;
			return -1;
		}
	}
	else
	{
		return semid; /* error, check errno */
	}
	return semid;
}

int main()
{
	char ch, c;
	key_t key1, key2;
	int semid, shmid;

	key1 = ftok("Shared_", 'b');

    //shmget returns an identifier or else -1 if error
    int shm_id = shmget(key1, 1024, 0666 | IPC_CREAT);
	char *str;
	str = (char *)shmat(shmid, (void *)0, 0);
	printf("Enter hello : ");
	fgets(str, 6, stdin);
	printf("%s\n", str);
	printf("the id of the shared memory segment is : %d\n\n", shmid);

	union semun arg;
	struct semid_ds buf;
	arg.buf = &buf;

	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1; /* set to allocate resource */
	sb.sem_flg = SEM_UNDO;
	key2 = ftok("Shared_", 'B');

	/* grab the semaphore set created by seminit.c: */
	if ((semid = initsem(key2, 2)) == -1)
	{
		perror("initsem");
		exit(1);
	}

	if (semop(semid, &sb, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
	printf("first semaphore is locked.\n");
	printf("The content of the shared memory segment is : %s\n", str);
	sb.sem_op = 1;
	if (semop(semid, &sb, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
	printf("first semaphore is unlocked.\n\n");

	sb.sem_num = 1;

	while (1)
	{
		if (semctl(semid, 0, GETVAL) > 0)
		{
			printf("Press return to lock: ");
			getchar();
			printf("Trying to lock...\n");
			sb.sem_op = -1;

			if (semop(semid, &sb, 1) == -1)
			{
				perror("semop");
				exit(1);
			}
			printf("second semaphore is locked.\n\n");

			printf("The content of the shared memory segment is : %s\n\n", str);

			printf("Press return to unlock: ");
			getchar();
			sb.sem_op = 1; /* free resource */
			if (semop(semid, &sb, 1) == -1)
			{
				perror("semop");
				exit(1);
			}
			printf("second semaphore is unlocked\n\n");

			printf("Enter q to quit : ");
			scanf("%c", &ch);
			printf("\n");
			if (ch == 'q' || ch == 'Q')
				break;
		}
		else
		{
			continue;
		}
	}
	shmdt(str);
	shmctl(shmid, IPC_RMID, NULL);
	if (semctl(semid, 0, IPC_RMID, arg) == -1)
	{
		perror("semctl");
		exit(1);
	}
	return 0;
}
