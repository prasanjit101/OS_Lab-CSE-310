#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *user_function()
{
    //user function to
    printf("\nHELLO WORLD\n");
    //The pthread_exit() function terminates the calling thread and returns a value (in our case junk value)
    pthread_exit(NULL);
}

int main()
{
    pthread_t new;
    /* Create independent threads each of which will execute function */
    pthread_create(&new, NULL, &user_function, NULL);
    /* Wait till threads are complete before main continues.  */
    pthread_join(new, NULL);
    return 0;
}

//CSB19057