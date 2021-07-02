/********************************
* Task name: bst				*
* Develoer: Inbal Elmalech		*
* Reviewer: Iman Nasreldeen		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free, size_t, qsort */
#include <stdio.h> /* printf, puts */
#include "bst.h" /* bst_t */

/****************************   DECLARATIONS   *******************************/
void TestBSTCreateAndDestroy(void);
void TestBSTizeIsEmpty(void);
void TestBSTInsertSizeIsEmptyEndBeginGetDataPrevIsEqual(void);
void TestBSTBeginEndNextPrevRemove(void);
void TestBSTFindForEach(void);

int CmpInts(const void *data1, const void *data2, void *param);
int AddNum(void *data, void *param);
/********************************  MAIN  *************************************/

int main(void)
{
	TestBSTCreateAndDestroy();
	puts("---------------------------------");
	TestBSTizeIsEmpty();
	puts("---------------------------------");
	TestBSTInsertSizeIsEmptyEndBeginGetDataPrevIsEqual();
	puts("---------------------------------");
	TestBSTBeginEndNextPrevRemove();
	puts("---------------------------------");
	TestBSTFindForEach();

	return (0);
}

/*****************************************************************************/

void TestBSTCreateAndDestroy(void)
{
	/* TODO: add timing! */

	bst_t *new_tree = BSTCreate(CmpInts, NULL);
	printf("New tree at: %p\n", (void *)new_tree);

	printf("destroying tree\n");
	BSTDestroy(new_tree);
}

/*---------------------------------------------------------------------------*/

void TestBSTizeIsEmpty(void)
{
	/* TODO: add timing! */

	bst_t *new_tree = BSTCreate(CmpInts, NULL);
	printf("New tree at: %p\n", (void *)new_tree);
	printf("size: %lu\n", BSTSize(new_tree));
	printf("is empty? %d\n", BSTIsEmpty(new_tree));
	BSTDestroy(new_tree);
}

/*---------------------------------------------------------------------------*/

void TestBSTInsertSizeIsEmptyEndBeginGetDataPrevIsEqual(void)
{
	/* TODO: add timing! */
	bst_iter_t begin = {NULL};
	bst_iter_t end = {NULL};

	int x1 = 13;
	bst_iter_t x1_iter = {NULL};
	int x2 = 7;
	bst_iter_t x2_iter = {NULL};
	int x3 = 8;
	bst_iter_t x3_iter = {NULL};
	int x4 = 5;
	bst_iter_t x4_iter = {NULL};
	int x5 = 21;
	bst_iter_t x5_iter = {NULL};
	int x6 = 15;
	bst_iter_t x6_iter = {NULL};
	int x7 = 23;
	bst_iter_t x7_iter = {NULL};
	int x8 = 22;
	bst_iter_t x8_iter = {NULL};
	int x9 = 2;
	bst_iter_t x9_iter = {NULL};
	int x10 = 3;
	bst_iter_t x10_iter = {NULL};

	bst_t *new_tree = BSTCreate(CmpInts, NULL);

	begin = BSTBegin(new_tree);
	end = BSTEnd(new_tree);

	printf("is begin equal to end? %d\n", BSTIterIsEqual(begin, end));

	x1_iter = BSTInsert(new_tree, (void *)&x1);
	x2_iter = BSTInsert(new_tree, (void *)&x2);
	x3_iter = BSTInsert(new_tree, (void *)&x3);
	x4_iter = BSTInsert(new_tree, (void *)&x4);
	x5_iter = BSTInsert(new_tree, (void *)&x5);
	x6_iter = BSTInsert(new_tree, (void *)&x6);
	x7_iter = BSTInsert(new_tree, (void *)&x7);
	x8_iter = BSTInsert(new_tree, (void *)&x8);
	x9_iter = BSTInsert(new_tree, (void *)&x9);
	x10_iter = BSTInsert(new_tree, (void *)&x10);

	begin = BSTBegin(new_tree);
	end = BSTEnd(new_tree);

	printf("Begin data: %d\n", *(int *)BSTGetData(begin));
	printf("End data: %d\n", *(int *)BSTGetData(BSTPrev(end)));
	printf("is begin equal to end? %d\n", BSTIterIsEqual(begin, end));

	printf("size: %lu\n", BSTSize(new_tree));
	printf("is empty? %d\n", BSTIsEmpty(new_tree));
	BSTDestroy(new_tree);
}

/*---------------------------------------------------------------------------*/

void TestBSTBeginEndNextPrevRemove(void)
{
	/* TODO: add timing! */
	bst_iter_t curr = {NULL};
	bst_iter_t end = {NULL};

	int x1 = 13;
	bst_iter_t x1_iter = {NULL};
	int x2 = 7;
	bst_iter_t x2_iter = {NULL};
	int x3 = 8;
	bst_iter_t x3_iter = {NULL};
	int x4 = 5;
	bst_iter_t x4_iter = {NULL};
	int x5 = 21;
	bst_iter_t x5_iter = {NULL};
	int x6 = 15;
	bst_iter_t x6_iter = {NULL};
	int x7 = 23;
	bst_iter_t x7_iter = {NULL};
	int x8 = 22;
	bst_iter_t x8_iter = {NULL};
	int x9 = 2;
	bst_iter_t x9_iter = {NULL};
	int x10 = 3;
	bst_iter_t x10_iter = {NULL};

	bst_t *new_tree = BSTCreate(CmpInts, NULL);

	x1_iter = BSTInsert(new_tree, (void *)&x1);
	x2_iter = BSTInsert(new_tree, (void *)&x2);
	x3_iter = BSTInsert(new_tree, (void *)&x3);
	x4_iter = BSTInsert(new_tree, (void *)&x4);
	x5_iter = BSTInsert(new_tree, (void *)&x5);
	x6_iter = BSTInsert(new_tree, (void *)&x6);
	x7_iter = BSTInsert(new_tree, (void *)&x7);
	x8_iter = BSTInsert(new_tree, (void *)&x8);
	x9_iter = BSTInsert(new_tree, (void *)&x9);
	x10_iter = BSTInsert(new_tree, (void *)&x10);
	
	printf("size: %lu\n", BSTSize(new_tree));
	printf("is empty? %d\n", BSTIsEmpty(new_tree));
	
	curr = BSTBegin(new_tree);
	end = BSTEnd(new_tree);

	printf("printing using next:\n");
	while(!BSTIterIsEqual(curr, end))
	{
		printf("[%d] ", *(int *)BSTGetData(curr));
		curr = BSTNext(curr);
	}
	puts(" ");

	curr = BSTPrev(end);

	printf("printing using prev:\n");
	while(!BSTIterIsEqual(curr, end))
	{
		printf("[%d] ", *(int *)BSTGetData(curr));
		curr = BSTPrev(curr);
	}
	puts(" ");

	puts("removing 4 elements\n");
	BSTRemove(x6_iter);
	BSTRemove(x3_iter);
	BSTRemove(x10_iter);
	BSTRemove(x8_iter);

	curr = BSTBegin(new_tree);

	printf("printing using next:\n");
	while(!BSTIterIsEqual(curr, end))
	{
		printf("[%d] ", *(int *)BSTGetData(curr));
		curr = BSTNext(curr);
	}
	puts(" ");

	BSTDestroy(new_tree);
}

/*---------------------------------------------------------------------------*/

void TestBSTFindForEach(void)
{
	int for_each_res = 0;
	int num_to_add = 3;
	bst_iter_t curr = {NULL};
	bst_iter_t end = {NULL};
	bst_iter_t find_res = {NULL};

	int x1 = 13;
	bst_iter_t x1_iter = {NULL};
	int x2 = 7;
	bst_iter_t x2_iter = {NULL};
	int x3 = 8;
	bst_iter_t x3_iter = {NULL};
	int x4 = 5;
	bst_iter_t x4_iter = {NULL};
	int x5 = 21;
	bst_iter_t x5_iter = {NULL};
	int x6 = 15;
	bst_iter_t x6_iter = {NULL};
	int x7 = 23;
	bst_iter_t x7_iter = {NULL};
	int x8 = 22;
	bst_iter_t x8_iter = {NULL};
	int x9 = 2;
	bst_iter_t x9_iter = {NULL};
	int x10 = 3;
	bst_iter_t x10_iter = {NULL};

	bst_t *new_tree = BSTCreate(CmpInts, NULL);

	x1_iter = BSTInsert(new_tree, (void *)&x1);
	x2_iter = BSTInsert(new_tree, (void *)&x2);
	x3_iter = BSTInsert(new_tree, (void *)&x3);
	x4_iter = BSTInsert(new_tree, (void *)&x4);
	x5_iter = BSTInsert(new_tree, (void *)&x5);
	x6_iter = BSTInsert(new_tree, (void *)&x6);
	x7_iter = BSTInsert(new_tree, (void *)&x7);
	x8_iter = BSTInsert(new_tree, (void *)&x8);
	x9_iter = BSTInsert(new_tree, (void *)&x9);
	x10_iter = BSTInsert(new_tree, (void *)&x10);
	
	curr = BSTBegin(new_tree);
	end = BSTEnd(new_tree);

	find_res = BSTFind(new_tree, (void *)&x3);
	printf("finding data of x3 (should be 8): [%d]\n",
		   *(int *)BSTGetData(find_res));

	printf("tree values:\n");
	while(!BSTIterIsEqual(curr, end))
	{
		printf("[%d] ", *(int *)BSTGetData(curr));
		curr = BSTNext(curr);
	}
	puts(" ");

	curr = BSTBegin(new_tree);

	for_each_res = BSTForEach(curr, end, AddNum, (void *)&num_to_add);

	curr = BSTBegin(new_tree);

	printf("tree values after for each of adding %d to elems:\n", num_to_add);
	while(!BSTIterIsEqual(curr, end))
	{
		printf("[%d] ", *(int *)BSTGetData(curr));
		curr = BSTNext(curr);
	}
	puts(" ");
	
	BSTDestroy(new_tree);
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

/*****************************************************************************/

