/********************************
* File name: dlist				*
* Develoer: Inbal Elmalech		*
* Reviewer:  Uriel Naiman		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include "dlist.h" /* */

#define SUCCESS 0
#define FAIL 1

/***************************   ENUMS & STRUCTURES  ***************************/

struct dlist
{
	dlist_iter_t head;
	dlist_iter_t tail;
};

struct dlist_node
{
	void *data;
	dlist_iter_t next;
	dlist_iter_t prev;
};

/*****************************   DECLARATIONS   ******************************/

void DListPrint(dlist_t *dlist);
void Test_BasicFunctions(void);
void Test_PushPopNextPrev(void);
void Test_GetSetInsertRemoveSize(void);
void Test_DListSplice(void);
void Test_FindForEach(void);

/********************************  MAIN  *************************************/

int main(void)
{
	Test_BasicFunctions();
	Test_PushPopNextPrev();
	Test_GetSetInsertRemoveSize();
	Test_DListSplice();
	Test_FindForEach();


	return (0);
}

/***************************** END OF MAIN ***********************************/

/* TODO; NO USING INNER STRUCTS UNLESS FOR PRINTING!!! */

/********************    Utility Function Declarations    ********************/

void DListPrint(dlist_t *dlist);
int AddOne(void *data, void *param);
int IsMatch(const void *data, void *param);

/***********************    General Functions    *****************************/

void Test_BasicFunctions(void)
{
	/* Testing for create, IsEmpty, Begin, End, IsIterEqual, Destroy. */

	dlist_t *new_dlist = NULL;
	dlist_iter_t head = NULL;
	dlist_iter_t tail = NULL;

	printf("\n------------------Test_BasicFunctions------------------\n");

	new_dlist = DListCreate();

	if (NULL == new_dlist)
	{
		printf("Exiting due to memory allocation failure.\n");
		return;
	}

	printf("New dlist at %p\n", (void *)new_dlist);

	printf("Is list empty? %d\n", DListIsEmpty(new_dlist));

	head = DListBegin(new_dlist);

	printf("Dlist begins at %p\n", (void *)head);

	tail = DListEnd(new_dlist);

	printf("Dlist end at %p\n", (void *)tail);

	printf("Is head == tail? %d\n", DListIterIsEqual(head, tail));

	printf("Destroying dlist at %p\n", (void *)new_dlist);

	DListDestroy(new_dlist);
}

/*---------------------------------------------------------------------------*/

void Test_PushPopNextPrev(void)
{
/* Testing for:
* pushfront, pushbacks,
* IsEmpty, DListIterIsEqual - testing again with different input
* popfront, popback,
* next, prev
*/

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	dlist_t *new_dlist = NULL;
	dlist_iter_t curr = NULL;

	printf("\n-----------------Test_PushPopNextPrev------------------\n\n");

	new_dlist = DListCreate();

	if (NULL == new_dlist)
	{
		printf("Exiting due to memory allocation failure.\n");
		return;
	}

	printf("New dlist at %p\n", (void *)new_dlist);

	printf("pushing b to front.\n");
	DListPushFront(new_dlist, (void *)&b);
	printf("pushing a to front.\n");
	DListPushFront(new_dlist, (void *)&a);

	DListPrint(new_dlist);

	printf("\n--------------DListPushBack----------------\n\n");

	DListPushBack(new_dlist, (void *)&c);
	DListPushBack(new_dlist, (void *)&d);

	DListPrint(new_dlist);

	curr = DListNext(new_dlist->head);

	printf("DListNext(new_dlist->head) data is %d\n", *((int *)curr->data));

	curr = DListNext(curr);

	printf("DListNext(current) data is %d\n", *((int *)curr->data));

	curr = DListPrev(curr);

	printf("DListPrev(current) data is %d\n", *((int *)curr->data));

	printf("Is list empty? %d\n", DListIsEmpty(new_dlist));

	printf("Is head == tail? %d\n",
		    DListIterIsEqual(new_dlist->head, new_dlist->tail));

	printf("\n***DListPopFront***\n\n");

	DListPopFront(new_dlist);

	DListPrint(new_dlist);

	printf("\n***DListPopBack***\n\n");

	DListPopBack(new_dlist);

	DListPrint(new_dlist);

	printf("Destroying dlist at %p\n", (void *)new_dlist);

	DListDestroy(new_dlist);
}


/*---------------------------------------------------------------------------*/

void Test_GetSetInsertRemoveSize(void)
{
/* Testing for:
* get data, set data
* Insert, remove
* size
*/

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;

	dlist_t *new_dlist = NULL;
	dlist_t *new_dlist2 = NULL;
	dlist_iter_t first_elem = NULL;

	printf("\n--------------Test_GetSetInsertRemoveSize--------------\n\n");

	new_dlist = DListCreate();

	if (NULL == new_dlist)
	{
		printf("Exiting due to memory allocation failure.\n");
		return;
	}

	printf("New dlist at %p\n", (void *)new_dlist);

	printf("Adding elements with pushback\n");

	DListPushBack(new_dlist, (void *)&a);
	DListPushBack(new_dlist, (void *)&b);
	DListPushBack(new_dlist, (void *)&c);
	DListPushBack(new_dlist, (void *)&d);

	DListPrint(new_dlist);

	first_elem = new_dlist->head->next;
	printf("GetData of element 1 is %d\n", *(int *)DListGetData(first_elem));

	printf("Switching data of element 1 to 5\n");
	DListSetData(first_elem, (void *)&e);

	printf("Data of element 1 is %d\n", *(int *)DListGetData(first_elem));

	printf("\n----------------DListInsert----------------\n\n");

	DListInsert(new_dlist, new_dlist->tail->prev, (void *)&f);
	DListPrint(new_dlist);

	printf("\n-----------------DListSize-----------------\n\n");

	printf("Size of list is %lu\n", DListSize(new_dlist));

	new_dlist2 = DListCreate();
	printf("New dlist at %p\n", (void *)new_dlist2);
	printf("Size of list is %lu\n", DListSize(new_dlist2));
	printf("Inserting 2 elements to new list\n");

	DListInsert(new_dlist2, new_dlist2->tail, (void *)&e);
	DListInsert(new_dlist2, new_dlist2->tail, (void *)&g);

	DListPrint(new_dlist2);

	printf("Removing the first element in new list\n");

	DListRemove(new_dlist2->head->next);

	DListPrint(new_dlist2);

	printf("Destroying dlist at %p\n", (void *)new_dlist2);

	printf("\n-------------------------------------------\n\n");

	printf("Destroying dlist at %p\n", (void *)new_dlist);
	DListDestroy(new_dlist);
}

/*---------------------------------------------------------------------------*/

void Test_DListSplice(void)
{
/* Testing for DListSplice */

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;
	int h = 8;
	int i = 9;
	int j = 10;

	dlist_t *dlist1 = NULL;
	dlist_t *dlist2 = NULL;
	dlist_t *dlist3 = NULL;

	dlist_iter_t from = NULL;
	dlist_iter_t to = NULL;
	dlist_iter_t where = NULL;

	printf("\n-------------------Test_DListSplice--------------------\n\n");

	dlist1 = DListCreate();
	dlist2 = DListCreate();
	dlist3 = DListCreate();

	if ((NULL == dlist1) ||
		(NULL == dlist2) ||
		(NULL == dlist3))
	{
		printf("Exiting due to memory allocation failure.\n");
		return;
	}

	printf("\n********dlist1*******\n");

	printf("dlist1 at %p\n", (void *)dlist1);
	printf("\nSize of dlist1 is %lu\n", DListSize(dlist1));
	DListPrint(dlist1);

	printf("\n*******dlist2********\n");

	DListPushBack(dlist2, (void *)&a);
	DListPushBack(dlist2, (void *)&b);
	DListPushBack(dlist2, (void *)&c);
	DListPushBack(dlist2, (void *)&d);

	printf("dlist2 at %p\n", (void *)dlist2);
	printf("\nSize of dlist2 is %lu\n", DListSize(dlist2));
	DListPrint(dlist2);

	printf("\nSplicing all of dlist2 to before end of dlist1\n");

	DListSplice(DListBegin(dlist2), DListEnd(dlist2), DListEnd(dlist1));

	printf("\nSize of dlist1 is %lu\n", DListSize(dlist1));
	DListPrint(dlist1);

	printf("\nSize of dlist2 is %lu\n", DListSize(dlist2));
	DListPrint(dlist2);

	printf("\n********dlist3*******\n");

	DListPushBack(dlist3, (void *)&e);
	DListPushBack(dlist3, (void *)&f);
	DListPushBack(dlist3, (void *)&g);
	DListPushBack(dlist3, (void *)&h);
	DListPushBack(dlist3, (void *)&i);
	DListPushBack(dlist3, (void *)&j);


	printf("\nSize of dlist1 is %lu\n", DListSize(dlist1));
	DListPrint(dlist1);

	printf("dlist3 at %p\n", (void *)dlist3);
	printf("\nSize of dlist3 is %lu\n", DListSize(dlist3));
	DListPrint(dlist3);

	printf("\nSplicing dlist3 from elem. 2 to elem. 5 into dlist1"
		   " before elem. 2\n");

	/* TODO: WRITE THIS BETTER */
	from = dlist3->head->next->next;
	to = from->next->next->next;
	where = dlist1->head->next->next;

	DListSplice(from, to, where);

	printf("\nSize of dlist1 is %lu\n", DListSize(dlist1));
	DListPrint(dlist1);

	printf("\nSize of dlist3 is %lu\n", DListSize(dlist3));
	DListPrint(dlist3);

	printf("\n-------------------------------------------\n\n");

	printf("Destroying all lists\n");

	DListDestroy(dlist1);
	DListDestroy(dlist2);
	DListDestroy(dlist3);
}

/*---------------------------------------------------------------------------*/

void Test_FindForEach(void)
{
/* Testing for: find, for each */

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;

	dlist_iter_t find_res = NULL;
	dlist_t *new_dlist = NULL;

	printf("\n--------------Test_FindForEach--------------\n\n");

	new_dlist = DListCreate();

	if (NULL == new_dlist)
	{
		printf("Exiting due to memory allocation failure.\n");
		return;
	}

	DListPushBack(new_dlist, (void *)&a);
	DListPushBack(new_dlist, (void *)&b);
	DListPushBack(new_dlist, (void *)&c);
	DListPushBack(new_dlist, (void *)&d);
	DListPushBack(new_dlist, (void *)&e);
	DListPushBack(new_dlist, (void *)&f);
	DListPushBack(new_dlist, (void *)&g);

	DListPrint(new_dlist);

	printf("Using find to locate 4 (&d) - %p\n", (void *)&d);

	find_res = DListFind(DListBegin(new_dlist),
						 DListEnd(new_dlist),
						 (dlist_is_match_func_t)&IsMatch,
						 (void *)&d);

	printf("GetData of find_res is %d\n", *(int *)DListGetData(find_res));

	printf("Using foreach to add 1 to data\n");

	DListForEach(DListBegin(new_dlist),
				 DListEnd(new_dlist),
				 (dlist_do_action_func_t)&AddOne,
				 NULL);

	DListPrint(new_dlist);

	printf("Using find to locate 4 (&d) - %p\n", (void *)&d);

	find_res = DListFind(DListBegin(new_dlist),
						 DListEnd(new_dlist),
						 (dlist_is_match_func_t)&IsMatch,
						 (void *)&d);

	printf("GetData of find_res is %d\n", *(int *)DListGetData(find_res));

	DListDestroy(new_dlist);
}

/***********************    Utility Functions    *****************************/

int AddOne(void *data, void *param)
{
	(void)param;
	
	*(int *)data = (*(int *)data + 1);

	return (SUCCESS);
}

/*---------------------------------------------------------------------------*/

int IsMatch(const void *data, void *param)
{
	return (data == param);
}

/*---------------------------------------------------------------------------*/

void DListPrint(dlist_t *dlist)
{
	dlist_iter_t curr = dlist->head->next;
	size_t count = 1;

	puts("List is now:");

	while (dlist->tail != curr)
	{
		printf("Item %lu is %d\n", count, *(int *)(curr->data));
		curr = curr->next;
		++count;
	}
}

/*****************************************************************************/


