/********************************
* File name: pqueue				*
* Develoer: Inbal Elmalech		*
* Reviewer:  Noa Pilosof		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free. size_t */
#include <assert.h> /* assert */
#include <stdbool.h> /* true, false */
#include "heap.h" /* slist */
#include "pqueue.h" /* pqueue */
/***************************   ENUMS & STRUCTURES  ***************************/

struct pqueue
{
	heap_t *heap;
	pqueue_cmp_func_t cmpare;
	void *param;
};

/********************    Utility Functions Declaration   *********************/

/*************************    General Functions    ***************************/

pqueue_t *PQCreate(pqueue_cmp_func_t cmpare, void *param)
{
	pqueue_t *new_pq = (pqueue_t *) malloc(sizeof(pqueue_t));

	if (!new_pq)
	{
		return (NULL);
	}

	new_pq->heap = HeapCreate(cmpare, param);

	if (!new_pq->heap)
	{
		free(new_pq);
		new_pq = NULL;

		return (NULL);
	}

	new_pq->cmpare = cmpare;
	new_pq->param = param;

	return (new_pq);
}

/*---------------------------------------------------------------------------*/

void PQDestroy(pqueue_t *pqueue)
{
	assert(pqueue);

	HeapDestroy(pqueue->heap);

	free(pqueue);
	pqueue = NULL;
}

/*---------------------------------------------------------------------------*/

size_t PQSize(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (HeapSize(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

int PQIsEmpty(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (HeapIsEmpty(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

int PQEnqueue(pqueue_t *pqueue, void *data)
{
	assert(pqueue);

	return (HeapPush(pqueue->heap, data));
}

/*---------------------------------------------------------------------------*/

void *PQDequeue(pqueue_t *pqueue)
{
	assert(pqueue);

	return (HeapPop(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

void *PQPeek(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (HeapPeek(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

void PQClear(pqueue_t *pqueue)
{
	assert(pqueue);

	while (!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}

/*---------------------------------------------------------------------------*/

void *PQErase(pqueue_t *pqueue, void *data,
			  pqueue_is_match_func_t is_match, void *param)
{
	assert(pqueue);
	assert(is_match);

	return(HeapRemove(pqueue->heap, data, (heap_is_match_func_t)is_match, param));
}

/*************************    Utility Functions    ***************************/

/*****************************************************************************/




