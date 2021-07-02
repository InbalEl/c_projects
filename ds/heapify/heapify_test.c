/********************************
* Task name: heap   			*
* Develoer: Inbal Elmalech		*
* Reviewer: ????        		*
* Date: Aug 13, 2020			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include "heapify.h" /* heap */

/****************************   DECLARATIONS   *******************************/
int CmpInt(const void *data1, const void *data2, void *param);
void TestUp(int *arr, size_t size);
void TestDown(int *arr, size_t size);
void PrintArr(int *arr, size_t size);

/********************************  MAIN  *************************************/

int main(void)
{
	int arr1[] = {627, 545, 120, 97, 2, 541, 78, 134, 92, 32, 188, 124, 342, 11};
	int arr2[] = {627, 545, 120, 97, 2, 541, 78, 134, 92, 32, 188, 124, 342, 11};
	size_t size = sizeof(arr1) / sizeof(int);

	puts("---------------TestUp------------------");
	PrintArr(arr1, size);
	TestUp(arr1, size);
	PrintArr(arr1, size);
	puts("---------------TestDown------------------");
	PrintArr(arr2, size);
	TestDown(arr2, size);
	PrintArr(arr2, size);

	return (0);
}

/*****************************************************************************/

void TestUp(int *arr, size_t size)
{
	HeapifyUp((void *)arr, size, (size - 1), sizeof(int), CmpInt, NULL);
}

/*---------------------------------------------------------------------------*/

void TestDown(int *arr, size_t size)
{
	HeapifyDown((void *)arr, size, 0, sizeof(int), CmpInt, NULL);

}

/***************************** Utilty Functions ******************************/

void PrintArr(int *arr, size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; ++i)
	{
		printf("[%d] ", arr[i]);
	}
	
	puts(" ");
}

/*---------------------------------------------------------------------------*/

int CmpInt(const void *data1, const void *data2, void *param)
{
	(void)param;

	return (*(int *)data1 - *(int *)data2);
}

/*****************************************************************************/

