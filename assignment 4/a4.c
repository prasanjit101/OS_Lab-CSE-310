#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int my_file;
    //declaring the structure of the flock and setting up its members:
    struct flock f;
    f.l_type = F_WRLCK; //Type of lock
    f.l_whence = SEEK_SET;
    f.l_start = 0;      //Offset where the lock begins.
    f.l_len = 0;        //Size of the locked area; zero means until EOF.
    f.l_pid = getpid(); //Process holding the lock
    if ((my_file = open("text.txt", O_RDWR)) == -1)
    {
        perror("\nFile opening error!");
        exit(1);
    }
    //check and set the lock
    if (fcntl(my_file, F_SETLK, &f) == -1)
    {
        perror("\nerror in fcntl-lock");
        exit(1);
    }
    //read file
    char read_str[250];
    int last_index = read(my_file, read_str, sizeof(read_str));
    read_str[last_index] = '\0';
    puts(read_str);
    f.l_type = F_UNLCK;//remove the lock
    printf("\nPress <RETURN> to unlock the file!");
    getchar();
    if (fcntl(my_file, F_SETLK, &f) == -1)
    {
        perror("\nerror while releasing!");
        exit(1);
    }
    close(my_file);
    printf("\nUnlocked...\n");
    return 0;
}