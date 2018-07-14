#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
void *Timeout()
{
    //timeout after 1 sec
    sleep(1);
    pthread_exit(NULL);
}

void *CheckRace(void *check)
{
    //function to test goes here
    int i;
    for (i=1; i > 0; i++)
    {
        //nothing
    }

    //never runs if stuck in infinite loop
    int *ptr;
    ptr = (int*)check;
    *ptr = 1;
    pthread_exit(NULL);
}
  
int main()
{
    pthread_t thread_timeout, thread_check;
    int check = 0;

    pthread_create(&thread_check, NULL, CheckRace, (void *)&check);
    pthread_create(&thread_timeout, NULL, Timeout, NULL);
    pthread_join(thread_timeout, NULL);
    if(check == 0)
    {
        printf("Race Condition: FAIL\n");
        pthread_cancel(thread_check);
    }
    else
    {
        printf("Race Condition: PASS\n");
    }
    exit(0);
}