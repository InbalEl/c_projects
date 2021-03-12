/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h>
#include <unistd.h>
#include "wdlib.h" /* MMI, DNR */

/**********************************  MAIN  ***********************************/

int main(int argc, char **argv)
{
    size_t counter = 10;

    if (0 != (MMI(argc, argv)))
    {
        printf("MMI 24/7 failed\n");
        return (1);
    }

    while (counter)
    {
        sleep(1);
        --counter;
        printf("User app counter is %lu!\n", counter);
    }

    if (0 != DNR())
    {
        printf("dnr userapp failed!\n");
        return (1);
    }

    printf("user app finished running\n");

    return (0);
}

/*****************************************************************************/