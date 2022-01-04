#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO "/tmp/myfifo" // FIFO file path

int main()
{
    int my_file;

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mknod(FIFO, S_IFIFO | 0666, 0);

    char arr1[100], arr2[]="Message successfully received\n";
        // Open FIFO for Read only
        my_file = open(FIFO, O_RDONLY);
        // Read from FIFO
        read(my_file, arr1, sizeof(arr1));

        // Print the read message
        printf("\nMessage from Program 1: %s\n", arr1);
        close(my_file);
        // Open FIFO for write only
        my_file = open(FIFO, O_WRONLY);
        // Write the input arr2ing on FIFO
        // and close it
        write(my_file, arr2, strlen(arr2)+1);
        close(my_file);
    return 0;
}