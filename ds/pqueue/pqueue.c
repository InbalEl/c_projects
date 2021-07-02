/********************************
* File name: pqueue				*
* Develoer: Inbal Elmalech		*
* Reviewer:  Noa Pilosof		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free. size_t */
#include <assert.h> /* assert */
#include <stdbool.h> /* true, false */
#include "sorted_list.h" /* slist */
#include "pqueue.h" /* pqueue */
/***************************   ENUMS & STRUCTURES  ***************************/

struct pqueue
{
	sorted_list_t *sorted_list;
	pqueue_cmp_func_t cmpare;
	void *param;
};

/********************    Utility Functions Declaration   *********************/

static sorted_list_iter_t SortedListInitIter(void);
static int PQcmpToIsBefore(const void *data1, const void *data2, void *param);

/*************************    General Functions    ***************************/

pqueue_t *PQCreate(pqueue_cmp_func_t cmpare, void *param)
{
	pqueue_t *new_pq = (pqueue_t *) malloc(sizeof(pqueue_t));

	if (!new_pq)
	{
		return (NULL);
	}

	new_pq->sorted_list = SortedListCreate(&PQcmpToIsBefore, new_pq);

	if (!new_pq->sorted_list)
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

	SortedListDestroy(pqueue->sorted_list);

	free(pqueue);
	pqueue = NULL;
}

/*---------------------------------------------------------------------------*/

size_t PQSize(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (SortedListSize(pqueue->sorted_list));
}

/*---------------------------------------------------------------------------*/

int PQIsEmpty(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (SortedListIsEmpty(pqueue->sorted_list));
}

/*---------------------------------------------------------------------------*/

int PQEnqueue(pqueue_t *pqueue, void *data)
{
	assert(pqueue);

	return (SortedListIterIsEqual(SortedListInsert(pqueue->sorted_list, data),
								  SortedListEnd(pqueue->sorted_list)));
}

/*---------------------------------------------------------------------------*/

void *PQDequeue(pqueue_t *pqueue)
{
	assert(pqueue);

	return (SortedListPopFront(pqueue->sorted_list));
}

/*---------------------------------------------------------------------------*/

void *PQPeek(const pqueue_t *pqueue)
{
	assert(pqueue);

	return (SortedListGetData(SortedListBegin(pqueue->sorted_list)));
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

void *PQErase(pqueue_t *pqueue, pqueue_is_match_func_t is_match, void *param)
{
	void *data = NULL;
	sorted_list_iter_t match_iter = SortedListInitIter();

	assert(pqueue);
	assert(is_match);

	match_iter = SortedListFindIf(SortedListBegin(pqueue->sorted_list),
								  SortedListEnd(pqueue->sorted_list),
								  is_match,
								  param);

	data = SortedListGetData(match_iter);
	SortedListRemove(match_iter);

	return (data);
}

/*************************    Utility Functions    ***************************/

static sorted_list_iter_t SortedListInitIter(void)
{
	sorted_list_iter_t iter = {NULL};

	return (iter);
}

/*---------------------------------------------------------------------------*/

static int PQcmpToIsBefore(const void *data1, const void *data2, void *param)
{
	pqueue_t *pq = (pqueue_t *)param;

	if ((-1) == pq->cmpare(data1, data2, pq->param))
	{
		return (true);
	}

	return (false);
}

/*****************************************************************************/




