/********************************
* Task name: avl				*
* Develoer: Inbal Elmalech		*
* Reviewer: Ido Finkelstein		*
* Date: Aug 4, 2020				*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free, size_t, qsort */
#include <stdio.h> /* printf, puts */
#include "avl.h" /* avl_t */

/****************************   DECLARATIONS   *******************************/
void TestAVLCreateAndDestroy(void);
void TestAVLInsertRemove(void);
void TestAVLForEach(void);
void TestAVLSizeFind(void);

int CmpInts(const void *data1, const void *data2, void *param);
int AddNum(void *data, void *param);
int ActionPrint(void *data, void *param);

/********************************  MAIN  *************************************/

int main(void)
{
	TestAVLCreateAndDestroy();
	puts("---------------------------------");
	TestAVLInsertRemove();
	puts("---------------------------------");
	TestAVLForEach();
	puts("---------------------------------");
	TestAVLSizeFind();

	return (0);
}

/*****************************************************************************/

void TestAVLCreateAndDestroy(void)
{
	avl_t *new_avl = AVLCreate(CmpInts, NULL);
	printf("New tree at: %p\n", (void *)new_avl);

	printf("destroying tree\n");
	AVLDestroy(new_avl);
}

/*---------------------------------------------------------------------------*/

void TestAVLInsertRemove(void)
{
	avl_t *new_avl = AVLCreate(CmpInts, NULL);
	int x1 = 42;
	int x2 = 23;
	int x3 = 15;
	int x4 = 67;
	int x5 = 4;

	printf("New tree at: %p\n", (void *)new_avl);

	printf("inserting data to tree\n");
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x1);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x2);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x3);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x4);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x5);
	AVLPrint2D(new_avl);
	puts("---------------------------------");


	printf("size of tree is: %lu\n", AVLSize(new_avl));

	printf("removing elements from tree\n");
	AVLRemove(new_avl, (void *)&x4);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLRemove(new_avl, (void *)&x1);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLRemove(new_avl, (void *)&x2);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLRemove(new_avl, (void *)&x3);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLRemove(new_avl, (void *)&x5);
	AVLPrint2D(new_avl);
	puts("---------------------------------");	
	
	printf("destroying tree\n");
	AVLDestroy(new_avl);
}

/*---------------------------------------------------------------------------*/

void TestAVLForEach(void)
{
	avl_t *new_avl = AVLCreate(CmpInts, NULL);
	int x1 = 42;
	int x2 = 23;
	int x3 = 15;
	int x4 = 67;
	int x5 = 4;

	printf("New tree at: %p\n", (void *)new_avl);

	printf("inserting data to tree\n");
	AVLInsert(new_avl, (void *)&x1);
	AVLInsert(new_avl, (void *)&x2);
	AVLInsert(new_avl, (void *)&x3);
	AVLInsert(new_avl, (void *)&x4);
	AVLInsert(new_avl, (void *)&x5);

	printf("printing in-order using for each:\n");
	AVLForEach(new_avl, ActionPrint, NULL);
	puts(" ");

	printf("destroying tree\n");
	AVLDestroy(new_avl);
}

/*---------------------------------------------------------------------------*/

void TestAVLSizeFind(void)
{
	avl_t *new_avl = AVLCreate(CmpInts, NULL);
	int x1 = 42;
	int x2 = 23;
	int x3 = 15;
	int x4 = 67;
	int x5 = 4;

	printf("New tree at: %p\n", (void *)new_avl);

	printf("inserting data to tree\n");
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x1);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x2);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x3);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x4);
	AVLPrint2D(new_avl);
	puts("---------------------------------");
	AVLInsert(new_avl, (void *)&x5);
	AVLPrint2D(new_avl);
	puts("---------------------------------");

	printf("finding [%d] %p:\n", x3, (void *)&x3);
	printf("data found: %d\n", *(int *)AVLFind(new_avl, (void *)&x3));
	printf("size of tree is: %lu\n", AVLSize(new_avl));

	printf("destroying tree\n");
	AVLDestroy(new_avl);
}

/***************************** Utilty Functions ******************************/

int AddNum(void *data, void *param)
{
	*(int *)data += *(int *)param;

	return (0);
}

/*---------------------------------------------------------------------------*/

int CmpInts(const void *data1, const void *data2, void *param)
{
	(void)param;

	if (*(int *)data1 < *(int *)data2)
	{
		return (-1);
	}

	else if (*(int *)data1 > *(int *)data2)
	{
		return (1);
	}

	return (0);
}

/*---------------------------------------------------------------------------*/

int ActionPrint(void *data, void *param)
{
	(void)param;

	printf("[%d] ", *(int *)data);

	return (0);
}

/*****************************************************************************/

