//The server file will send a message to the client file using message queue
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
//message queue node structure .The field type is used later when retrie
//positive number. message[200] is the data this will be added to the queue
//more members can be added as long as first member is of long type
struct message_queue
{
    long type;
    char message[200];
} a;

int main(int argc, const char *argv[])
{
    key_t key = ftok("IPCMQ", 9);//generate a probably-unique key which two process can use to access the message queue.
    //here the server and reciever should use the same key
    if (argc < 2)
    {
        printf("\nInsufficient arguments!\n");
        exit(0);
    }
    else
    {
        strcpy(a.message, argv[1]);
    }
    a.type = 1;
    int queue_id = msgget(key, 0666 | IPC_CREAT);//message queue created using msgget() ,it returns the message queue ID on success, or -1 on failure. here IPC_CREAT is bit-wise OR'd with the permissions for this queue
    //666-> rw-rw-rw
    if (msgsnd(queue_id, &a, sizeof(a), 0/*ignoring optional flag parameters*/) == 0)//write to the message queue
    {
        printf("\nMessage successfully written\n");
    }
    return 0;
}