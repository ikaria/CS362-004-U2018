#include "assess.h"


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
        printf("%s | Result: FAILED\n\n", condition);
    }
    else
    {
        printf("%s | Result: PASSED\n\n", condition);
    }

    return pass;
}