/********************************
* File name: Sorted list		*
* Develoer: Inbal Elmalech		*
* Reviewer:  Gal Shvinkelstein	*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts*/
#include "sorted_list.h" /* sorted_list*/

#define FALSE 0
#define TRUE 1
/***************************   ENUMS & STRUCTURES  ***************************/

/*****************************   DECLARATIONS   ******************************/
void Test_CreateAndDestroy(void);
void Test_BeginEndIsEqual(void);
void Test_SizeIsEmpty(void);
void Test_InsertRemove(void);
void Test_Pops(void);
void Test_ForEachFind(void);
void Test_FindIf(void);
void Test_Merge(void);

static int SLIsBefore(const void *data1,const void *data2, void *param);
void SLPrintList(sorted_list_t *sl);
int SLAdd3(void *data, void *param);
/********************************  MAIN  *************************************/

int main(void)
{
	Test_CreateAndDestroy();
	Test_BeginEndIsEqual();
	Test_SizeIsEmpty();
	Test_InsertRemove();
	Test_Pops();
	Test_ForEachFind();
	Test_FindIf();
	Test_Merge();

	return (0);
}

/***************************** END OF MAIN ***********************************/

/********************    Utility Function Declarations    ********************/

static int SLIsBefore(const void *data1,const void *data2, void *param);
static sorted_list_iter_t SortedListInitIter(void);
int SLAdd3(void *data, void *param);
int SLIsMatch(void *data, void *param);
void SLPrintList(sorted_list_t *sl);

/***********************    General Functions    *****************************/

void Test_CreateAndDestroy(void)
{
	sorted_list_t *new_sl =
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	printf("\n-->> Testing Sorted List Create and Destroy <<--\n");

	if (NULL == new_sl)
	{
	printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	SortedListDestroy(new_sl);

	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_BeginEndIsEqual(void)
{
	sorted_list_t *new_sl =
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	sorted_list_iter_t begin = SortedListInitIter();
	sorted_list_iter_t end = SortedListInitIter();

	printf("\n-->> Testing Begin, End, Is equal <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	begin = SortedListBegin(new_sl);
	end = SortedListEnd(new_sl);

	printf("Is begin == end? %d\n", SortedListIterIsEqual(begin, end));

	SortedListDestroy(new_sl);

	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_SizeIsEmpty(void)
{
	sorted_list_t *new_sl =
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	printf("\n-->> Testing Size, Is empty <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	printf("Size of list is %lu\n", SortedListSize(new_sl));
	printf("Is list empty? %d\n", SortedListIsEmpty(new_sl));

	SortedListDestroy(new_sl);
	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_InsertRemove(void)
{
	int a = 18;
	int b = 7;
	int c = 42;
	int d = 15;
	int e = 5;

	sorted_list_t *new_sl =
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	sorted_list_iter_t iter_c = SortedListInitIter();
	sorted_list_iter_t iter_e = SortedListInitIter();

	printf("\n-->> Testing insert, remove <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);


	SortedListInsert(new_sl, (void *)&a);
	SortedListInsert(new_sl, (void *)&b);
	iter_c = SortedListInsert(new_sl, (void *)&c);
	SortedListInsert(new_sl, (void *)&d);
	iter_e = SortedListInsert(new_sl, (void *)&e);

	SLPrintList(new_sl);

	printf("Size of list is %lu\n", SortedListSize(new_sl));
	printf("Is list empty? %d\n", SortedListIsEmpty(new_sl));

	printf("Removing 5, 42\n");

	SortedListRemove(iter_e);
	SortedListRemove(iter_c);

	SLPrintList(new_sl);
	printf("Size of list is %lu\n", SortedListSize(new_sl));

	SortedListDestroy(new_sl);

	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_Pops(void)
{
	int a = 18;
	int b = 7;
	int c = 42;
	int d = 15;
	int e = 5;

	sorted_list_t *new_sl =
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	printf("\n-->> Testing pops <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	SortedListInsert(new_sl, (void *)&a);
	SortedListInsert(new_sl, (void *)&b);
	SortedListInsert(new_sl, (void *)&c);
	SortedListInsert(new_sl, (void *)&d);
	SortedListInsert(new_sl, (void *)&e);

	SLPrintList(new_sl);

	printf("Begin is %d\n",
			*(int *)SortedListGetData(SortedListBegin(new_sl)));
	printf("End is %d\n",
			*(int *)SortedListGetData(SortedListPrev(SortedListEnd(new_sl))));

	printf("%d popped from front of list\n",
		   *(int *)SortedListPopFront(new_sl));

	printf("%d popped from back of list\n",
		   *(int *)SortedListPopBack(new_sl));

	SLPrintList(new_sl);

	printf("Begin is %d\n",
			*(int *)SortedListGetData(SortedListBegin(new_sl)));
	printf("End is %d\n",
			*(int *)SortedListGetData(SortedListPrev(SortedListEnd(new_sl))));

	SortedListDestroy(new_sl);

	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_ForEachFind(void)
{
	int a = 18;
	int b = 7;
	int c = 42;
	int d = 15;
	int e = 5;

	int status = 0;

	sorted_list_t *new_sl = 
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	sorted_list_iter_t sl_end = SortedListInitIter();
	sorted_list_iter_t sl_iter = SortedListInitIter();

	printf("\n-->> Testing for each, find <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	SortedListInsert(new_sl, (void *)&a);
	SortedListInsert(new_sl, (void *)&b);
	SortedListInsert(new_sl, (void *)&c);
	SortedListInsert(new_sl, (void *)&d);
	SortedListInsert(new_sl, (void *)&e);

	SLPrintList(new_sl);

	puts("Adding 3 to each element");

	status = SortedListForEach(SortedListBegin(new_sl),
							   SortedListEnd(new_sl),
							   &SLAdd3,
							   NULL);

	if (1 == status)
	{
		puts("ForEach status: failed");
	}

	else
	{
		puts("ForEach status: success");
	}

	SLPrintList(new_sl);

	sl_end = SortedListEnd(new_sl);

	printf("Finding number 21 (was 18, saved at &a - %p)\n", (void *)&a);
	sl_iter = SortedListFind(new_sl, SortedListBegin(new_sl),
							 SortedListEnd(new_sl),
							 (void *)&a);

	if (sl_end.internal_iter == sl_iter.internal_iter)
	{
		printf("Got end of list :(\n");
	}

	else
	{
		printf("data of iter given by find: %d\n",
			   *(int *)SortedListGetData(sl_iter));
	}

	SortedListDestroy(new_sl);
	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_FindIf(void)
{
	int a = 18;
	int b = 7;
	int c = 42;
	int d = 15;
	int e = 5;

	sorted_list_t *new_sl = 
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	sorted_list_iter_t sl_to = SortedListInitIter();
	sorted_list_iter_t sl_iter = SortedListInitIter();

	printf("\n-->> Testing findif <<--\n");

	if (NULL == new_sl)
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)new_sl);

	SortedListInsert(new_sl, (void *)&a);
	SortedListInsert(new_sl, (void *)&b);
	SortedListInsert(new_sl, (void *)&c);
	SortedListInsert(new_sl, (void *)&d);
	SortedListInsert(new_sl, (void *)&e);

	SLPrintList(new_sl);
	sl_to = SortedListEnd(new_sl);

	sl_iter = SortedListFindIf(SortedListBegin(new_sl),
							   sl_to,
							   (sorted_list_is_match_func_t)&SLIsMatch,
							   NULL);

	if (SortedListIterIsEqual(sl_iter, sl_to))
	{
		printf("Got end of list :(\n");
	}

	else
	{
	printf("Finding first power of 2 - %d)\n",
			   *(int *)SortedListGetData(sl_iter));
	}

	++d;

	SLPrintList(new_sl);

	sl_iter = SortedListFindIf(SortedListBegin(new_sl),
							   sl_to,
							   (sorted_list_is_match_func_t)&SLIsMatch,
							   NULL);

	if (SortedListIterIsEqual(sl_iter, sl_to))
	{
		printf("Got end of list :(\n");
	}

	else
	{
	printf("Finding first power of 2 - %d)\n",
			   *(int *)SortedListGetData(sl_iter));
	}

	SortedListDestroy(new_sl);
	printf("sorted list destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_Merge(void)
{
	int d1 = 4;
	int d2 = 3;
	int d3 = 12;

	int s1 = 7;
	int s2 = 1;
	int s3 = 4;
	int s4 = 2;

	int s5 = 8;
	int s6 = 13;
	int s7 = 5;
	int s8 = 6;
	int s9 = 14;
	int s10 = 9;

	sorted_list_t *dest = 
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);
	sorted_list_t *src = 
	SortedListCreate((sorted_list_is_before_func_t) &SLIsBefore, NULL);

	printf("\n-->> Testing merge <<--\n");
	if ((NULL == dest) || (NULL == src))
	{
		printf("SortedListCreate failed!\n");
	}

	printf("Created sorted list at %p\n", (void *)dest);
	printf("Created sorted list at %p\n", (void *)src);

	printf("-Trying to merge empty dest with empty src-\n");
	SortedListMerge(dest, src);

	printf("-filling up dest-\n");
	SortedListInsert(dest, (void *)&d1);
	SortedListInsert(dest, (void *)&d2);
	SortedListInsert(dest, (void *)&d3);
	SLPrintList(dest);

	printf("-Merging dest with empty src, dest is now-\n");
	SortedListMerge(dest, src);
	SLPrintList(dest);

	printf("-filling up src. It is now-\n");
	SortedListInsert(src, (void *)&s1);
	SortedListInsert(src, (void *)&s2);
	SortedListInsert(src, (void *)&s3);
	SortedListInsert(src, (void *)&s4);
	SLPrintList(src);


	SortedListMerge(dest, src);
	printf("-Merging dest with src, des is now-\n");
	SLPrintList(dest);

	printf("-src is now-\n");
	SLPrintList(src);

	printf("-filling up src again, src is now-\n");
	SortedListInsert(src, (void *)&s5);
	SortedListInsert(src, (void *)&s6);
	SortedListInsert(src, (void *)&s7);
	SortedListInsert(src, (void *)&s8);
	SortedListInsert(src, (void *)&s9);
	SortedListInsert(src, (void *)&s10);
	SLPrintList(src);

	SortedListMerge(dest, src);
	printf("-Merging dest with src, des is now-\n");
	SLPrintList(dest);

	SortedListDestroy(dest);
	SortedListDestroy(src);
	printf("sorted lists destroyed\n");
}

/***********************    Utility Functions    *****************************/

static int SLIsBefore(const void *data1,const void *data2, void *param)
{
	(void)param;

	if ((*(int *)data1) < (*(int *)data2))
	{
		return (TRUE);
	}

	return (FALSE);
}

static sorted_list_iter_t SortedListInitIter(void)
{
	sorted_list_iter_t iter = {NULL};

	return (iter);

}

/*---------------------------------------------------------------------------*/

int SLAdd3(void *data, void *param)
{
	*(int *)data = *(int *)data + 3;
	(void)param;

	return (0);
}

/*---------------------------------------------------------------------------*/

void SLPrintList(sorted_list_t *sl)
{
	sorted_list_iter_t curr_elem = SortedListBegin(sl);

	while (curr_elem.internal_iter != SortedListEnd(sl).internal_iter)
	{
		printf("[%d]\t", *(int *)SortedListGetData(curr_elem));
		curr_elem = SortedListNext(curr_elem);
	}
	puts(" "); 
}

/*---------------------------------------------------------------------------*/

int SLIsMatch(void *data, void *param)
{
	int num = *(int *)data;
	(void)param;

	return (num && !(num & (num - 1)));
}

/*****************************************************************************/


