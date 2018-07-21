#include "utils.h"

typedef struct
{
    int *check;
    void (*functionToTest)();

}args_struct;


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
        pthread_cancel(thread_check);
    }

    return check;
}

int AssertCondition(char* failMsg, int pass)
{
    if(!pass)
    {
        printf("%s\n",failMsg);
    }
    return pass;
}


int AssertTest(char* condition, int pass)
{
    if(!pass)
    {
        printf("%s | Result: FAILED\n", condition);
    }
    else
    {
        printf("%s | Result: PASSED\n", condition);
    }

    return pass;
}

/******** EXAMPLE USAGE **************

int main()
{
    TestRace(&testF);
    exit(0);
}

void testF()
{
    printf("function pointer");
}
**************************************/