#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    //ftok to generate unique key
    key_t key = ftok("Shared_", 98);

    //shmget returns an identifier or else -1 if error
    int shm_id = shmget(key, 1024, 0666 | IPC_CREAT);

    //shmat to attach pointer to a shared memory
    char *s = shmat(shm_id, (void *)0, 0);

    //getting pid
    __pid_t pid = getpid();

    printf("Data written in shared memory :%d \n", pid);
    sprintf(s, "%d", pid);

    //detach from shared memory
    shmdt(s);
    return 0;
}