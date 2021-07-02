/************************************
* Project name: calculator	    	*
* Develoer: Inbal Elmalech	    	*
* Reviewer:	Tali Raphael	    	*
* Submitted on: Aug 1, 2020         *
************************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free, size_t, qsort */
#include <stdio.h> /* printf, puts */
#include "stack.h"
#include "calculator.h"

/****************************   DECLARATIONS   *******************************/

/********************************  MAIN  *************************************/

int main(void)
{
	char *exp1 = "1+4*5";
	printf("%s = ", exp1);
	printf("%f\n", Calc(exp1));

	free(exp1);
	exp1 = NULL;
	return (0);
}

/*****************************************************************************/

/***************************** Utilty Functions ******************************/

/*****************************************************************************/

