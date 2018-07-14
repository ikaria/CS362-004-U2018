#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int *check;
    void (*functionToTest)();

}args_struct;

void testF()
{
    printf("function pointer");
}

void *Timeout()
{
    //timeout after 1 sec
    sleep(1);
    pthread_exit(NULL);
}

void *CheckRace(void *args)
{
    args_struct *ptr;
    ptr = (args_struct *)args;
    //function to test goes here
    ptr->functionToTest();

    //never runs if stuck in infinite loop
    *(ptr->check) = 1;
    pthread_exit(NULL);
}
  
int TestRace(void (*userFunction)())
{
    pthread_t thread_timeout, thread_check;

    args_struct *args = malloc(sizeof *args);
    int check = 0;
    args->check = &check;
    args->functionToTest = userFunction;

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
    //exit(0);

}

int main()
{
    TestRace(&testF);
    exit(0);
}