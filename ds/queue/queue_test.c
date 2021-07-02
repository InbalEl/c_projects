/********************************
* Task name: queue				*
* Develoer: Inbal Elmalech		*
* Reviewer: 					*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include "queue.h" /* queue */

/*****************************   DECLARATIONS   ******************************/

void Test_QCreateAndDestroy();
void Test_QIsEmptyAndSize();
void Test_QEnqueueAndQDequeueAndPeek();
void Test_QAppend();

/********************************  MAIN  *************************************/

int main(void)
{
	Test_QCreateAndDestroy();
	Test_QIsEmptyAndSize();
	Test_QEnqueueAndQDequeueAndPeek();
	Test_QAppend();

	return (0);
}

/***************************** END OF MAIN ***********************************/

/***********************    General Functions    *****************************/

void Test_QCreateAndDestroy()
{
	queue_t *new_queue = QCreate();

	printf("\n-------------------QCreate--------------------\n\n");

	if (NULL != new_queue)
	{
		printf("new queue at %p\n", (void *)new_queue);
	}

	printf("\n-------------------QDestroy-------------------\n\n");

	printf("Destroying queue at %p\n", (void *)new_queue);

	QDestroy(new_queue);
}

/*---------------------------------------------------------------------------*/

void Test_QIsEmptyAndSize()
{
	queue_t *new_queue = QCreate();

	printf("\n-------------------QIsEmpty-------------------\n\n");

	printf("QIsEmpty? %d\n", QIsEmpty(new_queue));

	printf("\n---------------------QSize--------------------\n\n");

	printf("Size of queue: %lu\n", QSize(new_queue));

	QDestroy(new_queue);

}

/*---------------------------------------------------------------------------*/

void Test_QEnqueueAndQDequeueAndPeek()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	queue_t *new_queue = QCreate();

	printf("\n-------------------QEnqueue-------------------\n\n");

	printf("element added to queue: %d\t%p\n", a, (void *)&a);
	QEnqueue(new_queue, (void *)&a);

	printf("element added to queue: %d\t%p\n", b, (void *)&b);
	QEnqueue(new_queue, (void *)&b);

	printf("element added to queue: %d\t%p\n", c, (void *)&c);
	QEnqueue(new_queue, (void *)&c);

	printf("element added to queue: %d\t%p\n", d, (void *)&d);
	QEnqueue(new_queue, (void *)&d);

	printf("\n---------------------QPeek--------------------\n\n");

	printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));

	printf("\n-------------------QIsEmpty-------------------\n\n");

	printf("QIsEmpty? %d\n", QIsEmpty(new_queue));

	printf("\n---------------------QSize--------------------\n\n");

	printf("Size of queue: %lu\n", QSize(new_queue));

	printf("\n-------------------QDequeue-------------------\n\n");

	printf("element removed from queue\n");

	QDequeue(new_queue);

	printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));

	printf("element removed from queue\n");

	QDequeue(new_queue);

	printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));

	printf("element removed from queue\n");

	QDequeue(new_queue);

	printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));

	printf("element removed from queue\n");

	QDequeue(new_queue);/***********************    General Functions    *****************************/

	/*printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));

	printf("element removed from queue\n");

	QDequeue(new_queue);

	printf("element at front of queue: %d\t%p\n",
			*(int *)(QPeek(new_queue)),
			QPeek(new_queue));*/

	printf("\n-------------------QIsEmpty-------------------\n\n");

	printf("QIsEmpty? %d\n", QIsEmpty(new_queue));

	printf("\n---------------------QSize--------------------\n\n");

	printf("Size of queue: %lu\n", QSize(new_queue));

	QDestroy(new_queue);
}

/*---------------------------------------------------------------------------*/

void Test_QAppend()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;

	queue_t *queue1 = QCreate();
	queue_t *queue2 = QCreate();

	printf("\n-------------------QEnqueue-------------------\n\n");

	printf("\n---queue1---\n\n");

	printf("element added to queue1: %d\t%p\n", a, (void *)&a);
	QEnqueue(queue1, (void *)&a);

	printf("element added to queue1: %d\t%p\n", b, (void *)&b);
	QEnqueue(queue1, (void *)&b);

	printf("element added to queue1: %d\t%p\n", c, (void *)&c);
	QEnqueue(queue1, (void *)&c);

	printf("element added to queue1: %d\t%p\n", d, (void *)&d);
	QEnqueue(queue1, (void *)&d);

	printf("QPeek: element at front of queue1: %d\t%p\n",
			*(int *)(QPeek(queue1)),
			QPeek(queue1));

	printf("\n---queue2---\n\n");

	printf("element added to queue1: %d\t%p\n", e, (void *)&e);
	QEnqueue(queue2, (void *)&e);

	printf("element added to queue1: %d\t%p\n", f, (void *)&f);
	QEnqueue(queue2, (void *)&f);

	printf("element added to queue1: %d\t%p\n", g, (void *)&g);
	QEnqueue(queue2, (void *)&g);

	printf("QPeek: element at front of queue2: %d\t%p\n",
			*(int *)(QPeek(queue2)),
			QPeek(queue2));

	printf("\n--------------------QAppend-------------------\n\n");

	printf("\n---Appending queue2 to queue1---\n");

	QAppend(queue1, queue2);

	printf("Size of queue1: %lu\n", QSize(queue1));
	printf("Size of queue2: %lu\n", QSize(queue2));

	QDestroy(queue1);
	QDestroy(queue2);
}

/*****************************************************************************/


