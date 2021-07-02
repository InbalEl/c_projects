/********************************
* File name: Sorted list		*
* Develoer: Inbal Elmalech		*
* Reviewer:  Gal Shvinkelstein	*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, puts */
#include <assert.h> /* assert */
#include "dlist.h" /* slist */
#include "sorted_list.h" /* slist */

/***************************   ENUMS & STRUCTURES  ***************************/

struct sorted_list
{
	dlist_t *dlist;
	sorted_list_is_before_func_t is_before;
	void *param;
};

/********************    Utility Functions Declaration   *********************/

/*************************    General Functions    ***************************/

sorted_list_t *SortedListCreate(sorted_list_is_before_func_t is_before,
								void * param)
{
	sorted_list_t *new_sorted_l = NULL;
	dlist_t *new_dl = NULL;

	assert(is_before);

	new_sorted_l = (sorted_list_t *) malloc(sizeof(sorted_list_t));

	if (NULL == new_sorted_l)
	{
		return (NULL);
	}

	new_dl = DListCreate();

	/* Freeing allocated memory of new_sorted_l in case DListCreate failed */
	if (NULL == new_dl)
	{
		free(new_sorted_l);
		new_sorted_l = NULL;

		return (NULL);
	}

	new_sorted_l->dlist = new_dl;
	new_sorted_l->is_before = is_before;
	new_sorted_l->param = param;

	return (new_sorted_l);
}

/*---------------------------------------------------------------------------*/

void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(sorted_list);

	DListDestroy(sorted_list->dlist);

	free(sorted_list);
	sorted_list = NULL;
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListBegin(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t begin = {NULL};

	assert(sorted_list);

	begin.internal_iter = DListBegin(sorted_list->dlist);

	return (begin);
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListEnd(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t end = {NULL};

	assert(sorted_list);

	end.internal_iter = DListEnd(sorted_list->dlist);

	return (end);
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
	sorted_list_iter_t next = {NULL};

	assert(iter.internal_iter);

	next.internal_iter = DListNext(iter.internal_iter);

	return (next);
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{
	sorted_list_iter_t prev = {NULL};

	assert(iter.internal_iter);

	prev.internal_iter = DListPrev(iter.internal_iter);

	return (prev);
}

/*---------------------------------------------------------------------------*/

void *SortedListGetData(sorted_list_iter_t iter)
{
	assert(iter.internal_iter);

	return (DListGetData(iter.internal_iter));
}

/*---------------------------------------------------------------------------*/

int SortedListIterIsEqual(sorted_list_iter_t iter1,
						  sorted_list_iter_t iter2)
{
	assert(iter1.internal_iter);
	assert(iter2.internal_iter);

	return (DListIterIsEqual(iter1.internal_iter, iter2.internal_iter));
}

/*---------------------------------------------------------------------------*/

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	return (DListIsEmpty(sorted_list->dlist));
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t where = {NULL};

	assert(sorted_list);

	where = SortedListBegin(sorted_list);

	/* While next of where is not null, and data is not before data of where */
	while ((SortedListNext(where).internal_iter) &&
		   (!sorted_list->is_before(data,
									SortedListGetData(where),
									sorted_list->param)))
	{
		where = SortedListNext(where);
	}

	/* Inserting before relevant element according to is_before function */
	where.internal_iter = DListInsert(sorted_list->dlist,
									  where.internal_iter, 
									  data);

	return (where);
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter)
{
	sorted_list_iter_t next = {NULL};

	assert(iter.internal_iter);

	next = SortedListNext(iter);

	next.internal_iter = DListRemove(iter.internal_iter);

	return (next);
}

/*---------------------------------------------------------------------------*/

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(sorted_list);

	return (DListPopFront(sorted_list->dlist));
}

/*---------------------------------------------------------------------------*/

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(sorted_list);

	return (DListPopBack(sorted_list->dlist));
}

/*---------------------------------------------------------------------------*/

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	return (DListSize(sorted_list->dlist));
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list,
								  sorted_list_iter_t from,
								  sorted_list_iter_t to,
								  void *to_find)
{
	assert(sorted_list);
	assert(from.internal_iter);
	assert(to.internal_iter);

	/* While from is not equal to to,
	and while from is before to_find,
	and while data of from is not equal to to_find (in order to stop
	at first match) - promote from.*/
	while (!(SortedListIterIsEqual(from, to))
			&&
		   (sorted_list->is_before(SortedListGetData(from),
								   to_find,
								   sorted_list->param))
			&&
		   (SortedListGetData(from) != to_find))
	{
		from = SortedListNext(from);
	}

	return (from);
}

/*---------------------------------------------------------------------------*/

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, 
									sorted_list_iter_t to,
									sorted_list_is_match_func_t is_match, 
									void *param)
{
	assert(from.internal_iter);
	assert(to.internal_iter);

	from.internal_iter = DListFind(from.internal_iter,
								   to.internal_iter,
								   is_match,
								   param);

	return (from);
}

/*---------------------------------------------------------------------------*/

int SortedListForEach(sorted_list_iter_t from, 
					  sorted_list_iter_t to, 
					  sorted_list_do_action_func_t do_action, 
					  void *param)
{

	assert(from.internal_iter);
	assert(to.internal_iter);

	return(DListForEach(from.internal_iter, to.internal_iter, 
						do_action, param));
}

/*---------------------------------------------------------------------------*/

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	sorted_list_iter_t where = {NULL};
	sorted_list_iter_t src_from = {NULL};
	sorted_list_iter_t src_to = {NULL};
	sorted_list_iter_t src_end = {NULL};
	sorted_list_is_before_func_t is_before = NULL;
	void *param = NULL;

	assert(dest);
	assert(src);

	where = SortedListBegin(dest);
	src_from = SortedListBegin(src);
	src_to = src_from;
	src_end = SortedListEnd(src);
	is_before = dest->is_before;
	param = dest->param;

	/* Looping while src_from is not equal to src_end, and while
	where is not equal to dest_end. Promoting where in each iteration. */
	while (!(SortedListIterIsEqual(src_from, src_end)) &&
		   (!(SortedListIterIsEqual(where,
									SortedListEnd(dest)))))
	{
		/* Looping while src_to is not equal to src_end, and while
		 src_to is before where. Promoting src_to in each iteration. */
		while ((!(SortedListIterIsEqual(src_to,
										src_end)) && 
			   (is_before(SortedListGetData(src_to),
						  SortedListGetData(where),
						  param))))
		{
			src_to = SortedListNext(src_to);
		}

		/* Once src_to is promoted to the element next to the last
		one to be splices before the current where, the section of src
		is splices and src_from is promoted to src_to. */
		if (!SortedListIterIsEqual(src_from, src_to))
		{
			DListSplice(src_from.internal_iter,
						src_to.internal_iter,
						where.internal_iter);

			src_from = src_to;
		}

		where = SortedListNext(where);
	}

	/* If where is at the end of dest but src still has elements,
	src_to is moved to the end and all remaining elements are spliced. */
	if (!(SortedListIterIsEqual(src_from, src_end)))
	{
		src_to = src_end;
		DListSplice(src_from.internal_iter,
					src_to.internal_iter,
					where.internal_iter);
	}
}

/*****************************************************************************/

/*************************    Utility Functions    ***************************/

/*****************************************************************************/



