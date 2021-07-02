/********************************
* File name: ws5.c				*
* Develoer: Inbal Elmalech		*
* Reviewer: Noa Pilosof			*
* Review date: 12 June, 2020	*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include "slist.h" /* */

/***************************   ENUMS & STRUCTURES  ***************************/

struct slist
{
	slist_iter_t head;
	slist_iter_t tail;
};

struct slist_node
{
	void *data;
	slist_iter_t next;
};

/*****************************   DECLARATIONS   ******************************/

void SListPrint(slist_t *slist);

slist_t *Test_SListCreate();
void Test_SListDestroy(slist_t *slist);
slist_iter_t Test_SListBegin(slist_t *slist);
slist_iter_t Test_SListEnd(slist_t *slist);
slist_iter_t Test_SListNext(slist_iter_t iter);
void *Test_SListGetData(const slist_iter_t iter);
void Test_SlistSetData(slist_iter_t iter, void *data);
void Test_SListIterIsEqual(slist_iter_t iter1, slist_iter_t iter2);
void Test_SListIsEmpty(slist_t *slist);
slist_iter_t Test_SListInsert(slist_t *slist, slist_iter_t where, void *data);
slist_iter_t Test_SListInsertAfter(slist_t *slist,
								   slist_iter_t where,
								   void *data);
void Test_SListRemove(slist_t *slist, slist_iter_t iter);
void Test_SListCount(slist_t *slist);
void Test_SListAppend(slist_t *dest, slist_t *src);
/*slist_iter_t Test_SListFind(slist_iter_t from, 
							slist_iter_t to,
							slist_is_match_func_t is_match, 
							void *param);
int Test_SListForEach(slist_iter_t from, 
					  slist_iter_t to, 
					  slist_do_action_func_t do_action, 
					  void *param);*/

/********************************  MAIN  *************************************/

int main(void)
{
	slist_t *new_list = NULL;
	slist_t *src_list = NULL;

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;

	slist_iter_t head = NULL;
	slist_iter_t second = NULL;
	slist_iter_t tail = NULL;

	printf("\n---------------Test_SListCreate----------------\n\n");

	new_list = Test_SListCreate();

	printf("\n---------------Test_SListIsEmpty----------------\n\n");

	Test_SListIsEmpty(new_list);

	printf("\n---------------Test_SListBegin----------------\n\n");

	head = Test_SListBegin(new_list);

	printf("\n---------------Test_SListEnd----------------\n\n");

	tail = Test_SListEnd(new_list);

	printf("\n----Test_SListInsert and Test_SListInsertAfter----\n\n");

	Test_SListInsert(new_list, tail, (void *)&a);
	Test_SListInsertAfter(new_list, head, (void *)&b);

	head = Test_SListBegin(new_list);
	tail = Test_SListEnd(new_list);

	SListPrint(new_list);

	Test_SListInsert(new_list, tail, (void *)&d);
	Test_SListInsert(new_list, tail, (void *)&c);

	SListPrint(new_list);

	printf("\n---------------Test_SListIsEmpty----------------\n\n");

	Test_SListIsEmpty(new_list);

	printf("\n---------------Test_SlistNext----------------\n\n");

	second = Test_SListNext(head);

	printf("\n---------------Test_SListGetData----------------\n\n");

	Test_SListGetData(second);

	printf("\n---------------Test_SlistSetData----------------\n\n");

	Test_SlistSetData(second, (void *)&a);

	SListPrint(new_list);

	printf("\n---------------Test_SListIterIsEqual----------------\n\n");

	Test_SListIterIsEqual(head, tail);
	Test_SListIterIsEqual(tail, tail);

	printf("\n---------------Test_SListCount----------------\n\n");

	Test_SListCount(new_list);
	SListPrint(new_list);

	printf("\n---------------Test_SListRemove----------------\n\n");

	SListPrint(new_list);
	Test_SListRemove(new_list, second);
	SListPrint(new_list);

	printf("\n---------------Test_SListAppend----------------\n\n");


	src_list = Test_SListCreate();
	Test_SListInsert(src_list, src_list->tail, (void *)&e);
	Test_SListInsert(src_list, src_list->tail, (void *)&f);
	Test_SListInsert(src_list, src_list->tail, (void *)&g);

	printf("Dest list:\n");
	SListPrint(new_list);
	printf("Src list:\n");
	SListPrint(src_list);

	Test_SListAppend(new_list, src_list);
	SListPrint(new_list);
	printf("\n---------------Test_SListDestroy----------------\n\n");

	Test_SListDestroy(new_list);

	return (0);
}

/***************************** END OF MAIN ***********************************/

/***********************    Utility Functions    *****************************/

void SListPrint(slist_t *slist)
{
	slist_iter_t curr = slist->head;
	size_t count = 0;

	puts("\nList is now:\n");

	while (slist->tail != curr)
	{
		printf("Item %lu is %d\n", count, *(int *)(curr->data));
		curr = curr->next;
		++count;
	}
}

/***********************    General Functions    *****************************/

slist_t *Test_SListCreate()
{
	slist_t *new_list = SListCreate();
	printf("pointer to new list is: %p\n", (void *)new_list);

	return (new_list);
}

/*---------------------------------------------------------------------------*/

void Test_SListDestroy(slist_t *slist)
{
	printf("Destroying list in %p\n", (void *)slist);
	SListDestroy(slist);
}

/*---------------------------------------------------------------------------*/

slist_iter_t Test_SListBegin(slist_t *slist)
{
	slist_iter_t head = SListBegin(slist);
	printf("List begins in %p\n", (void *)head);

	return (head);
}

/*---------------------------------------------------------------------------*/

slist_iter_t Test_SListEnd(slist_t *slist)
{
	slist_iter_t tail = SListEnd(slist);
	printf("List ends in %p\n", (void *)tail);

	return (tail);
}

/*---------------------------------------------------------------------------*/

slist_iter_t Test_SListNext(slist_iter_t iter)
{
	slist_iter_t next = SListNext(iter);
	printf("Next item in list after %p is:%p\n", (void *)iter,
	(void *)next);

	return (next);
}

/*---------------------------------------------------------------------------*/

void *Test_SListGetData(const slist_iter_t iter)
{
	void *data = SListGetData(iter);
	printf("Data in %p is %d\n",  (void *)iter, *(int *)data);

	return (data);
}

/*---------------------------------------------------------------------------*/

void Test_SlistSetData(slist_iter_t iter, void *data)
{
	Test_SListGetData(iter);
	SListSetData(iter, data);
	Test_SListGetData(iter);
}

/*---------------------------------------------------------------------------*/

void Test_SListIterIsEqual(slist_iter_t iter1, slist_iter_t iter2)
{
	printf("Iter 1 is %p\n", (void *)iter1);
	printf("Iter 2 is %p\n", (void *)iter2);
	printf("Is equal?  %d\n", SListIterIsEqual(iter1, iter2));
}

/*---------------------------------------------------------------------------*/

void Test_SListIsEmpty(slist_t *slist)
{
	printf("Is list empty? %d\n", SListIsEmpty(slist));
}

/*---------------------------------------------------------------------------*/

slist_iter_t Test_SListInsert(slist_t *slist, slist_iter_t where, void *data)
{
	printf("Insering %p to list before %p\n", data, (void *)where);
	return (SListInsert(slist, where, data));
}

/*---------------------------------------------------------------------------*/

slist_iter_t Test_SListInsertAfter(slist_t *slist,
								   slist_iter_t where,
								   void *data)
{
	printf("Insering %p to list after %p\n", data, (void *)where);
	return (SListInsertAfter(slist, where, data));
}

/*---------------------------------------------------------------------------*/

void Test_SListRemove(slist_t *slist, slist_iter_t iter)
{
	printf("Removing %p from list\n", (void *)iter);
	printf("Element removed, next iter on list is %p\n",
										(void *)SListRemove(slist, iter));
}

/*---------------------------------------------------------------------------*/

void Test_SListCount(slist_t *slist)
{
	printf("List has %lu elements\n", SListCount(slist));
}

/*---------------------------------------------------------------------------*/

void Test_SListAppend(slist_t *dest, slist_t *src)
{
	printf("Appending lists\n");
	SListAppend(dest, src);
}


/*---------------------------------------------------------------------------*/
/*
slist_iter_t Test_SListFind(slist_iter_t from, 
							slist_iter_t to,
							slist_is_match_func_t is_match, 
							void *param)
{

}
*/
/*---------------------------------------------------------------------------*/
/*
int Test_SListForEach(slist_iter_t from, 
					  slist_iter_t to, 
					  slist_do_action_func_t do_action, 
					  void *param)
{

}
*/
/*****************************************************************************/


