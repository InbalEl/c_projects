/********************************
* Task name: heap   			*
* Develoer: Inbal Elmalech		*
* Reviewer: ????        		*
* Date: Aug 13, 2020			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include "heap.h" /* heap */
#include "heapify.h" /* heap */

/****************************   DECLARATIONS   *******************************/

void HeapCreateDestroy(void);
void HeapPushSizeIsEmpty(void);
void HeapPeekPopRemove(void);
int CmpInt(const void *data1, const void *data2, void *param);
int is_match(const void *data1, const void *data2, void *param);

/********************************  MAIN  *************************************/

int main(void)
{
	puts("----------------HeapCreateDestroy-----------------");
	HeapCreateDestroy();

	puts("----------------HeapPushPeek-----------------");
	HeapPushSizeIsEmpty();

	puts("----------------HeapPushPeek-----------------");
	HeapPeekPopRemove();

	puts("--------------------THE END----------------------");

	return (0);
}

/*****************************************************************************/

void HeapCreateDestroy(void)
{
	heap_t *new_heap = HeapCreate(CmpInt, NULL);
	printf("New heap created at: %p\n", (void *)new_heap);
	
	printf("Destroying heap\n");
	HeapDestroy(new_heap);
}

/*---------------------------------------------------------------------------*/

void HeapPushSizeIsEmpty(void)
{
	heap_t *new_heap = HeapCreate(CmpInt, NULL);
	size_t i = 0;

	int arr[] = {627, 545, 120, 97, 2, 541, 78, 134, 92, 32, 188, 124, 342, 11};
	size_t size = sizeof(arr) / sizeof(int);

	printf("Is heap empty? %d\n", HeapIsEmpty(new_heap));

	for (i = 0; i < size; ++i)
	{
		printf("inserting [%d]\t\t", arr[i]);
		HeapPush(new_heap, (void *)(arr + i));
		printf("Heap size is now %lu\n", HeapSize(new_heap));
	}	

	printf("Is heap empty? %d\n", HeapIsEmpty(new_heap));

	HeapDestroy(new_heap);
}

/*---------------------------------------------------------------------------*/

void HeapPeekPopRemove(void)
{
	heap_t *new_heap = HeapCreate(CmpInt, NULL);
	size_t i = 0;

	int arr[] = {627, 545, 120, 97, 2, 541, 78, 134, 92, 32, 188, 124, 342, 11};
	size_t size = sizeof(arr) / sizeof(int);

	printf("inserting elements\n");
	
	for (i = 0; i < size; ++i)
	{
		HeapPush(new_heap, (void *)(arr + i));
		printf("heap peek value after inserion is: [%d]\n",
		*(int *)HeapPeek(new_heap));
	}	

	HeapPop(new_heap);
	printf("After popping heap, peek value is [%d]\n",
		   *(int *)HeapPeek(new_heap));

	printf("Value of removed value [in heap]: %d\n",
	*(int*)HeapRemove(new_heap, (void *)(arr + 10), is_match, NULL));
	
	printf("Address of removed value [not in heap]: %p\n",
	(void *)HeapRemove(new_heap, (void *)(arr + 10), is_match, NULL));

	HeapDestroy(new_heap);
}

/***************************** Utilty Functions ******************************/

int CmpInt(const void *data1, const void *data2, void *param)
{
	(void)param;

	return (*(int *)data1 - *(int *)data2);
}

/*---------------------------------------------------------------------------*/

int is_match(const void *data1, const void *data2, void *param)
{
	(void)param;

	return (*(int *)data1 == *(int *)data2);
}


/*****************************************************************************/