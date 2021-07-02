/******************************************************************************
* File name: utils
* Develoer: Inbal Elmalech
******************************************************************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stdio.h> /* printf, puts */
#include "utils.h" /* utils */

/*****************************   DECLARATIONS   ******************************/

void TestSwapElements(void);

/********************************  MAIN  *************************************/

int main(void)
{
	TestSwapElements();

	return (0);
}

/***********************    General Functions    *****************************/

void TestSwapElements(void)
{
	int a = 42;
	int b = 15;
	printf("a = %d\tb = %d\n", a, b);

	SwapElements((void *)&a, (void *)&b, sizeof(int));

	printf("a = %d\tb = %d\n", a, b);
}

/*****************************************************************************/


