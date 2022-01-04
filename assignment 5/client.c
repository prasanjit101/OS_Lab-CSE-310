#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

//message queue node structure
struct message_queue
{
    long type;
    char message[200];
} a;

int main()
{
    key_t key = ftok("IPCMQ", 9);//connect to the message queue
    int queue_id = msgget(key, 0666 | IPC_CREAT);//generate quieue id same as the server side
    msgrcv(queue_id, &a, sizeof(a), 1, 0);//receive the message and store and display it
    printf("\nMessage successfully received: %s\n", a.message);//read from the message queue
    msgctl(queue_id, IPC_RMID, NULL);//destroy the message queue
    return 0;
}