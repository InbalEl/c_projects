/********************************
* File name: pqueue				*
* Develoer: Inbal Elmalech		*
* Reviewer:  Noa Pilosof		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h> /* printf */
#include "pqueue.h" /* pqueue */
/***************************   ENUMS & STRUCTURES  ***************************/

/*****************************   DECLARATIONS   ******************************/

void Test_CreateAndDestroy(void);
void Test_SizeIsEmpty(void);
void Test_EnqueueDequeueClearErase(void);

/********************    Utility Function Declarations    ********************/


int PQCompare(const void *data1, const void *data2, void *param);
int PQIsMatch (const void *data, void *param);

/********************************  MAIN  *************************************/

int main(void)
{
	Test_CreateAndDestroy();
	Test_SizeIsEmpty();
	Test_EnqueueDequeueClearErase();

	return (0);
}

/***************************** END OF MAIN ***********************************/

/***********************    General Functions    *****************************/

void Test_CreateAndDestroy(void)
{
	pqueue_t *new_pq = PQCreate(&PQCompare, NULL);

	printf("\n-->> Testing pqueue Create and Destroy <<--\n");

	if (NULL == new_pq)
	{
	printf("PQCreate failed!\n");
	}

	printf("Created pqueue at %p\n", (void *)new_pq);

	PQDestroy(new_pq);
	printf("pqueue destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_SizeIsEmpty(void)
{
	pqueue_t *new_pq = PQCreate(&PQCompare, NULL);

	printf("\n-->> Testing pqueue size, is empty <<--\n");

	if (NULL == new_pq)
	{
	printf("PQCreate failed!\n");
	}

	printf("Created pqueue at %p\n", (void *)new_pq);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	PQDestroy(new_pq);
	printf("pqueue destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_EnqueueDequeueClearErase(void)
{
	int x1 = 1;
	int x2 = 2;
	int x3 = 3;
	int x4 = 4;
	int x5 = 5;
	int x6 = 6;
	int x7 = 7;
	int x8 = 8;
	int x9 = 9;

	pqueue_t *new_pq = PQCreate(&PQCompare, NULL);

	printf("\n-->> Testing pqueue enqueue, dequeue <<--\n");

	if (NULL == new_pq)
	{
	printf("PQCreate failed!\n");
	}

	printf("Created pqueue at %p\n", (void *)new_pq);

	printf("Enqueueing 9 items\n");
	PQEnqueue(new_pq, (void *)&x1);
	PQEnqueue(new_pq, (void *)&x2);
	PQEnqueue(new_pq, (void *)&x3);
	PQEnqueue(new_pq, (void *)&x4);
	PQEnqueue(new_pq, (void *)&x5);
	PQEnqueue(new_pq, (void *)&x6);
	PQEnqueue(new_pq, (void *)&x7);
	PQEnqueue(new_pq, (void *)&x8);
	PQEnqueue(new_pq, (void *)&x9);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	printf("Dequeueing 2 items\n");
	PQDequeue(new_pq);
	PQDequeue(new_pq);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	printf("Erasing element\n");
	PQErase(new_pq, &PQIsMatch, NULL);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	printf("Erasing element\n");
	PQErase(new_pq, &PQIsMatch, NULL);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	printf("clearing pq\n");
	PQClear(new_pq);

	printf("Size is: %ld\n", PQSize(new_pq));
	printf("Is empty?%d\n", PQIsEmpty(new_pq));

	PQDestroy(new_pq);
	printf("pqueue destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_Peek(void)
{
	int x1 = 1;
	int x2 = 2;
	int x3 = 3;
	int x4 = 4;
	int x5 = 5;
	int x6 = 6;
	int x7 = 7;
	int x8 = 8;
	int x9 = 9;

	pqueue_t *new_pq = PQCreate(&PQCompare, NULL);

	printf("\n-->> Testing pqueue enqueue, dequeue <<--\n");

	if (NULL == new_pq)
	{
	printf("PQCreate failed!\n");
	}

	printf("Created pqueue at %p\n", (void *)new_pq);

	PQEnqueue(new_pq, (void *)&x1);
	PQEnqueue(new_pq, (void *)&x2);
	PQEnqueue(new_pq, (void *)&x3);
	PQEnqueue(new_pq, (void *)&x4);
	PQEnqueue(new_pq, (void *)&x5);
	PQEnqueue(new_pq, (void *)&x6);
	PQEnqueue(new_pq, (void *)&x7);
	PQEnqueue(new_pq, (void *)&x8);
	PQEnqueue(new_pq, (void *)&x9);

	printf("Peeking into pq: %d\n", *(int *)PQPeek(new_pq));

	printf("Dequeueing 2 items\n");
	PQDequeue(new_pq);
	PQDequeue(new_pq);

	printf("Peeking into pq: %d\n", *(int *)PQPeek(new_pq));

	PQDestroy(new_pq);
	printf("pqueue destroyed\n");
}

/***********************    Utility Functions    *****************************/

int PQCompare(const void *data1, const void *data2, void *param)
{
	(void)param;

	if (*(int *)data1 > *(int *)data2)
	{
		return (1);
	}

	else if (*(int *)data1 < *(int *)data2)
	{
		return (-1);
	}

	return (0);
}

/*---------------------------------------------------------------------------*/

int PQIsMatch (const void *data, void *param)
{
	int num = *(int *)data;
	(void)param;

	return (num && !(num & (num - 1)));
}

/*****************************************************************************/


