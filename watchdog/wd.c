/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h>
#include <unistd.h>
#include "wdlib.h" /* MMI, DNR */

/**********************************  MAIN  ***********************************/

int main(int argc, char **argv)
{
    if (0 != (MMI(argc, argv)))
    {
        printf("MMI wd failed\n");
        return (1);
    }

    while (1)
    {
        sleep(1);
        printf("hi I'm wd app\n");
    }

    return (0);
}

/*****************************************************************************/