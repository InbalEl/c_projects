/********************************
* Task name: queue				*
* Develoer: Inbal Elmalech		*
* Reviewer: 					*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h>	/* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "slist.h" /* slist */
#include "queue.h" /* queue */

/***************************   ENUMS & STRUCTURES  ***************************/

struct queue
{
    slist_t *slist;
};

/*************************    General Functions    ***************************/

/* TODO: I'M MISSING CHECKS FOR RETURN VALUES OF SLIST FUNCTIONS */

queue_t *QCreate(void)
{
	queue_t *new_q = (queue_t *) malloc (sizeof(queue_t));

	if (NULL == new_q)
	{
		return (NULL);
	}

	new_q->slist = SListCreate();

	if (NULL == new_q->slist)
	{
		free(new_q);
		new_q = new_q;
		return (NULL);
	}

	return (new_q);
}

/*---------------------------------------------------------------------------*/

void QDestroy(queue_t *queue)
{
	assert(queue);

	SListDestroy(queue->slist);

	free(queue);
	queue = NULL;
}

/*---------------------------------------------------------------------------*/

size_t QSize(const queue_t *queue)
{
	assert(queue);

	return (SListCount(queue->slist));
}

/*---------------------------------------------------------------------------*/

int QIsEmpty(const queue_t *queue)
{
	assert(queue);

	return (SListIsEmpty(queue->slist));
}

/*---------------------------------------------------------------------------*/

int QEnqueue(queue_t *queue, void *data)
{
	slist_iter_t rear = NULL;
	slist_iter_t result = NULL;

	assert(queue);

	rear = SListEnd(queue->slist);

	result = SListInsert(queue->slist, rear, data);

	return (rear != result);
}

/*---------------------------------------------------------------------------*/

void QDequeue(queue_t *queue)
{
	assert(queue);

	SListRemove(queue->slist, SListBegin(queue->slist));
}

/*---------------------------------------------------------------------------*/

void *QPeek(const queue_t *queue)
{
	assert(queue);

	return (SListGetData(SListBegin(queue->slist)));
}

/*---------------------------------------------------------------------------*/

void QAppend(queue_t *dest, queue_t *src)
{
	SListAppend(dest->slist, src->slist);
}

/*****************************************************************************/




