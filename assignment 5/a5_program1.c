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
    // mkfifo(<pathname>,<permission>)
    mknod(FIFO, S_IFIFO | 0666, 0);

    char str1[100], str2[100];
        // open in write mode and write
        // string taken from user.
        my_file = open(FIFO,O_WRONLY);
        fgets(str2, 100, stdin);
        write(my_file, str2, strlen(str2)+1);
        close(my_file);
        // First open in read only and read
        my_file = open(FIFO,O_RDONLY);
        read(my_file, str1, 100);

        // Print the read string and close
        printf("\nReceived acknowledgement message: %s\n", str1);
        close(my_file);
    return 0;
}