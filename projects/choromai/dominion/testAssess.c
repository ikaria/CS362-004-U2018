#include "assess.h"

int main()
{
    int res1 = AssertCondition("test fail msg", 0 > 2);
    int res2 = AssertCondition("this should not print", 1 == 1);
    printf("Res1: %d\n", res1);
    printf("Res2: %d\n", res2);

    AssertTest("true condition ", 1 == 1);
    AssertTest("false condition ", 1 == 0);

    int one = 1;
    int zero = 0;
    int anotherOne = 1;

    anotherOne &= 1;
    one &= 0;

    printf("ONE should be 0: %d\n", one);
    printf("ANOTHER ONE should be 1: %d\n", anotherOne);

    return 0;
}