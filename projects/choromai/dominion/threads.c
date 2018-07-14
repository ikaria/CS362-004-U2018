#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int *check;
    
}args_struct;

void *Timeout()
{
    //timeout after 1 sec
    sleep(1);
    pthread_exit(NULL);
}

void *CheckRace(void *args)
{
    //function to test goes here
    int i;
    for (i=1; i < 0; i++)
    {
        //nothing
    }

    //never runs if stuck in infinite loop
    args_struct *ptr;
    ptr = (args_struct *)args;
    printf("got here");
    *(ptr->check) = 1;
    pthread_exit(NULL);
}
  
int main()
{
    pthread_t thread_timeout, thread_check;

    args_struct *args = malloc(sizeof *args);
    int check = 0;
    args->check = &check;

    pthread_create(&thread_check, NULL, CheckRace, args);
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

/*
int main()
{

}
*/