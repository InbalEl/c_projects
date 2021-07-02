/********************************
* Task name: vsa				*
* Develoer: Inbal Elmalech		*
* Reviewer: Reut Ben Harush		*
********************************/


/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free, size_t */
#include <stdio.h> /* printf, puts */
#include "vsa.h" /* vsa_t */

/****************************   DECLARATIONS   *******************************/

void test_case(void);

/********************************  MAIN  *************************************/

int main(void)
{
	test_case();

	return (0);
}

/***********************    General Functions    *****************************/
void test_case(void)
{
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	void *block5 = NULL;
	void *block6 = NULL;
	void *block7 = NULL;
	void *block8 = NULL;
	void *block9 = NULL;
	void *block10 = NULL;

	vsa_t *vsa_pool = (vsa_t *)malloc(200);

	vsa_pool = VSAInit(vsa_pool, 200);

	printf("Biggest free block: %ld\n", VSABiggestFreeBlock(vsa_pool));

	block1 = VSAAlloc(vsa_pool, 20);
	block2 = VSAAlloc(vsa_pool, 22);
	block3 = VSAAlloc(vsa_pool, 14);
	block4 = VSAAlloc(vsa_pool, 10);
	block5 = VSAAlloc(vsa_pool, 15);

	printf("Biggest free block: %ld\n", VSABiggestFreeBlock(vsa_pool));

	VSAFree(block1);
	VSAFree(block3);
	VSAFree(block4);

	printf("Biggest free block: %ld\n", VSABiggestFreeBlock(vsa_pool));

	block6 = VSAAlloc(vsa_pool, 8);
	block7 = VSAAlloc(vsa_pool, 3);
	block8 = VSAAlloc(vsa_pool, 42);

	printf("Biggest free block: %ld\n", VSABiggestFreeBlock(vsa_pool));

	block8 = VSAAlloc(vsa_pool, 213); /* TODO: check return val */

	free(vsa_pool);
	vsa_pool = NULL;
}

/**************************    Utility Functions    **************************/

/*****************************************************************************/
