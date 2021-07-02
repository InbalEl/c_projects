/********************************
* Task name: sort				*
* Develoer: Inbal Elmalech		*
* Reviewer: Guy Barazany		*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stdlib.h> /* malloc, free, size_t, qsort */
#include <stdio.h> /* printf, puts */
#include <time.h>
#include "sort.h" /* sort */
#include "radix_sort.h" /* radix */
#include "utils.h" /* swap */

/****************************   DECLARATIONS   *******************************/

static void TestBubbleSort(int *arr, size_t size);
static void TestInsertionSort(int *arr, size_t size);
static void TestSelectionSort(int *arr, size_t size);
static void TestcCountingSort(int *arr, size_t size);
static void TestRadixSort(unsigned int *uint_arr, size_t size);
/*static void TestMergeSort(unsigned int *uint_arr, size_t size);*/
static void TestQuickSort(unsigned int *uint_arr, size_t size);
static void TestHeapSort(unsigned int *uint_arr, size_t size);
static void TestQSortR(int *arr, size_t size);

int Cmp(const void *data1, const void *data2);
int CmpInts(const void *data1, const void *data2, void *param);
void PrintUintArr(unsigned int *arr, size_t size);
void InitArr(int *arr, size_t size);
void InitUintArr(unsigned int *arr, size_t num_of_elements);
void Test_Sorts(size_t size);

/********************************  MAIN  *************************************/

int main(void)
{
	size_t size = 20;
	puts("----------------------------------------");
	printf("int array size: %ld\n", size);
	Test_Sorts(size);
	puts("----------------------------------------");
	size = 100;
	printf("int array size: %ld\n", size);
	Test_Sorts(size);
	puts("----------------------------------------");
	size = 1000;
	printf("int array size: %ld\n", size);
	Test_Sorts(size);
	puts("----------------------------------------");
	/*size = 10000;
	printf("int array size: %ld\n", size);
	Test_Sorts(size);
	puts("----------------------------------------");
	size = 100000;
	printf("int array size: %ld\n", size);
	Test_Sorts(size);*/

	return (0);
}

/*****************************************************************************/

static void TestBubbleSort(int *arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	void *arg = NULL;

	printf("Bubble\t\t");
	InitArr(arr, size);

	start = clock();
	BubbleSort(arr, size, sizeof(int), CmpInts, arg);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);
}

/*----------------------------------------------------------------------------*/

static void TestInsertionSort(int *arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	void *arg = NULL;

	printf("Insertion\t");
	InitArr(arr, size);

	start = clock();
	InsertionSort(arr, size, sizeof(int), CmpInts, arg);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);
}

/*----------------------------------------------------------------------------*/

static void TestSelectionSort(int *arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	void *arg = NULL;

	printf("Selection\t");
	InitArr(arr, size);

	start = clock();
	SelectionSort(arr, size, sizeof(int), CmpInts, arg);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);
}

/*----------------------------------------------------------------------------*/

static void TestcCountingSort(int *arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;
	int min = 0;
	int max = 0;
	int *dest = (int *) malloc(sizeof(int) * size);

	printf("Counting\t");

	if (NULL == dest)
	{
		printf("Malloc failed... peace out\n");
	}

	InitArr(arr, size);

	min = FindMinInt(arr, size);
	max = FindMaxInt(arr, size);

	start = clock();
	CountingSort(arr, dest, size, min, max);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);

	free(dest);
	dest = NULL;
}

/*----------------------------------------------------------------------------*/

static void TestQSortR(int *arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	printf("qsort\t\t");
	InitArr(arr, size);

	start = clock();
	qsort(arr, size, sizeof(int), Cmp);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);
}

/*----------------------------------------------------------------------------*/

static void TestRadixSort(unsigned int *uint_arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	unsigned int chunk_size = 4;

	printf("Radix\t\t");
	InitUintArr(uint_arr, size);

	start = clock();
	RadixSort(uint_arr, size, chunk_size);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);

	/*PrintUintArr(uint_arr, size);*/
}

/*----------------------------------------------------------------------------*/

/*static void TestMergeSort(unsigned int *uint_arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	void *arg = NULL;

	unsigned int chunk_size = 4;

	printf("Radix\t\t");
	InitUintArr(uint_arr, size);

	start = clock();
	MergeSort(uint_arr, size, chunk_size, CmpInts, arg);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);

	/PrintUintArr(uint_arr, size);/
}*/

/*----------------------------------------------------------------------------*/

static void TestQuickSort(unsigned int *uint_arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	unsigned int chunk_size = 4;

	printf("Quick\t\t");
	InitUintArr(uint_arr, size);

	start = clock();
	QuickSort(uint_arr, size, chunk_size, CmpInts);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);

	/*PrintUintArr(uint_arr, size);*/
}

/*----------------------------------------------------------------------------*/

static void TestHeapSort(unsigned int *uint_arr, size_t size)
{
	clock_t start, end;
	double cpu_time = 0;

	unsigned int chunk_size = 4;

	printf("Heap\t\t");
	InitUintArr(uint_arr, size);


	start = clock();
	HeapSort(uint_arr, size, chunk_size, CmpInts, NULL);
	end = clock();

	cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run: %f seconds\n", cpu_time);

	/*PrintUintArr(uint_arr, size);*/
}

/***************************** Utilty Functions ******************************/

void Test_Sorts(size_t size_of_arr)
{
	int *arr = (int *)(malloc(sizeof(int) * size_of_arr));
	unsigned int *uint_arr = (unsigned int *)
							 (malloc(sizeof(unsigned int) * size_of_arr));

	if (NULL == arr)
	{
		printf("Malloc failed... peace out\n");
	}

	/*TestBubbleSort(arr, size_of_arr);
	TestInsertionSort(arr, size_of_arr);
	TestSelectionSort(arr, size_of_arr);
	TestcCountingSort(arr, size_of_arr);
	TestQSortR(arr, size_of_arr);
	TestRadixSort(uint_arr, size_of_arr);
	TestMergeSort(uint_arr, size_of_arr);*/
	TestQuickSort(uint_arr, size_of_arr);
	/*TestHeapSort(uint_arr, size_of_arr);*/

	free(arr);
	arr = NULL;

	free(uint_arr);
	uint_arr = NULL;
}

/*----------------------------------------------------------------------------*/

int Cmp(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

/*----------------------------------------------------------------------------*/

int CmpInts(const void *data1, const void *data2, void *param)
{
	(void)param;

	return (*(int *)data1 - *(int *)data2);
}

/*----------------------------------------------------------------------------*/

void PrintUintArr(unsigned int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		printf("%u ", arr[i]);
	}
	puts(" ");
}

/*----------------------------------------------------------------------------*/

void InitArr(int *arr, size_t num_of_elements)
{
	size_t i = 0;

	srand(time(NULL));

	for (i = 0; i < num_of_elements; ++i)
	{
		arr[i] = rand() % 10;
	}
}

/*----------------------------------------------------------------------------*/

void InitUintArr(unsigned int *arr, size_t num_of_elements)
{
	size_t i = 0;
	int tmp = 0;

	srand(time(NULL));

	for (i = 0; i < num_of_elements; ++i)
	{
		tmp = rand() % 10;
		arr[i] = (tmp < 0) ? (tmp * -1) : tmp;
	}
}

/*****************************************************************************/
