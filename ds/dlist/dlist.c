/********************************
* File name: ws5.c				*
* Develoer: Inbal Elmalech		*
* Reviewer:  Uriel Naiman		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, puts */
#include <assert.h> /* assert */
#include "dlist.h" /* slist */

#define DEADBEEF (void *)0xDEADBEEF
#define SUCCESS 0
#define FAIL 1

/***************************   ENUMS & STRUCTURES  ***************************/

struct dlist
{
	dlist_iter_t head;
	dlist_iter_t tail;
};

struct dlist_node
{
	void *data;
	dlist_iter_t next;
	dlist_iter_t prev;
};

/* TODO; CODE REUSE!!! */

/********************    Utility Functions Declaration   *********************/

static dlist_iter_t DListCreateNode(void *data,
									dlist_iter_t prev,
									dlist_iter_t next);

/*************************    General Functions    ***************************/

dlist_t *DListCreate(void)
{
	dlist_t *new_list_ptr = NULL;
	dlist_iter_t head_dummy = NULL;
	dlist_iter_t tail_dummy = NULL;

	/* allocating memory for the new dlist */
	new_list_ptr = (dlist_t *) malloc(sizeof(dlist_t));

	if (NULL == new_list_ptr)
	{
		return (NULL);
	}

	/* Creating dummy head node in new list */
	head_dummy = DListCreateNode(NULL, NULL, NULL);

	if (NULL == head_dummy)
	{
		/* freeing new list alloc if dummy node alloc failed */
		free(new_list_ptr);
		new_list_ptr = DEADBEEF;

		return (NULL);
	}

	/* Creating dummy tail node in new list */
	tail_dummy = DListCreateNode(NULL, head_dummy, NULL);

	if (NULL == tail_dummy)
	{
		/* freeing new list and head dummy if alloc for tail failed */
		free(new_list_ptr);
		new_list_ptr = DEADBEEF;
		free(tail_dummy);
		tail_dummy = DEADBEEF;

		return (NULL);
	}

	/* assigning initial values to list and dummies */
	new_list_ptr->head = head_dummy;
	new_list_ptr->tail = tail_dummy;

	head_dummy->next = tail_dummy;

	return (new_list_ptr);
}

/*---------------------------------------------------------------------------*/

void DListDestroy(dlist_t *dlist)
{
	dlist_iter_t prev = NULL;

	assert(dlist);

	prev = DListPrev(DListEnd(dlist));

	/* Starting from tail's prev, stepping back in the list and freeing next */
	while (prev)
	{
		free(DListNext(prev));
		prev->next = DEADBEEF;

		prev = DListPrev(prev);
	}

	/* Freeing head dummy and dlist*/
	free(prev);
	prev = DEADBEEF;

	free(dlist);
	dlist = DEADBEEF;
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListBegin(const dlist_t *dlist)
{
	assert(dlist);

	return (DListNext(dlist->head));
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListEnd(const dlist_t *dlist)
{
	assert(dlist);

	return (dlist->tail);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListNext(const dlist_iter_t iter)
{
	assert(iter);

	return (iter->next);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListPrev(const dlist_iter_t iter)
{
	assert(iter);

	return (iter->prev);
}

/*---------------------------------------------------------------------------*/

void *DListGetData(const dlist_iter_t iter)
{
	assert(iter);

	return (iter->data);
}

/*---------------------------------------------------------------------------*/

void DListSetData(dlist_iter_t iter, void *data)
{
	assert(iter);

	iter->data = data;
}

/*---------------------------------------------------------------------------*/

int DListIterIsEqual(const dlist_iter_t iter1, const dlist_iter_t iter2)
{
	assert(iter1);
	assert(iter2);

	return (iter1 == iter2);
}

/*---------------------------------------------------------------------------*/

int DListIsEmpty(const dlist_t *dlist)
{
	assert(dlist);

	return (DListBegin(dlist) == DListEnd(dlist));
}


/*---------------------------------------------------------------------------*/

dlist_iter_t DListInsert(dlist_t *dlist, dlist_iter_t where, void *data)
{
	dlist_iter_t new_node = NULL;

	assert(dlist);
	assert(where);

	new_node = DListCreateNode(data, DListPrev(where), where);

	if (NULL == new_node)
	{
		return (DListEnd(dlist));
	}

	where->prev = new_node;
	DListPrev(new_node)->next = new_node;

	return (new_node);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListRemove(dlist_iter_t iter)
{
	dlist_iter_t next_elem = NULL;

	assert(iter);

	next_elem = DListNext(iter);

	DListPrev(iter)->next = next_elem;
	DListNext(iter)->prev = DListPrev(iter);

	free(iter);
	iter = DEADBEEF;

	return (next_elem);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListPushFront(dlist_t *dlist, void *data)
{
	assert(dlist);

	return (DListInsert(dlist, DListBegin(dlist), data));
}

/*
	dlist_iter_t new_beginning = NULL;

	assert(dlist);

	new_beginning = DListCreateNode(data, dlist->head, DListNext(dlist->head));

	if (NULL == new_beginning)
	{
		return (DListEnd(dlist));
	}

	dlist->head->next = new_beginning;

	if (NULL == DListNext(new_beginning))
	{
		new_beginning->next = DListEnd(dlist);
	}

	DListNext(new_beginning)->prev = new_beginning;

	return (new_beginning);
}*/

/*---------------------------------------------------------------------------*/

dlist_iter_t DListPushBack(dlist_t *dlist, void *data)
{
	dlist_iter_t new_last_elem = NULL;

	assert(dlist);

	new_last_elem = DListCreateNode(data,
									DListPrev(DListEnd(dlist)),
									DListEnd(dlist));

	if (NULL == new_last_elem)
	{
		return (DListEnd(dlist));
	}

	DListEnd(dlist)->prev = new_last_elem;
	DListPrev(new_last_elem)->next = new_last_elem;

	return (new_last_elem);
}

/*---------------------------------------------------------------------------*/

void *DListPopFront(dlist_t *dlist)
{
	void *data = NULL;
	dlist_iter_t second_elem = NULL;

	assert(dlist);

	second_elem = DListNext(DListBegin(dlist));
	data = DListGetData(DListBegin(dlist));

	free(DListBegin(dlist));
	dlist->head->next = DEADBEEF;

	dlist->head->next = second_elem;
	second_elem->prev = dlist->head; 

	return (data);
}

/*---------------------------------------------------------------------------*/

void *DListPopBack(dlist_t *dlist)
{
	void *data = NULL;
	dlist_iter_t second_last_elem = NULL;

	assert(dlist);

	second_last_elem = DListPrev(DListPrev(DListEnd(dlist)));
	data = DListGetData(DListPrev(DListEnd(dlist)));

	free(DListPrev(DListEnd(dlist)));
	DListEnd(dlist)->prev = DEADBEEF;

	DListEnd(dlist)->prev = second_last_elem;
	second_last_elem->next = DListEnd(dlist); 

	return (data);
}

/*---------------------------------------------------------------------------*/

size_t DListSize(const dlist_t *dlist)
{
	size_t nof_elements = 0;
	dlist_iter_t curr_element = NULL;

	assert(dlist);

	curr_element = dlist->head;

	while (DListNext(curr_element) != DListEnd(dlist))
	{
		curr_element = DListNext(curr_element);
		++nof_elements;
	}

	return (nof_elements);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListSplice(dlist_iter_t from,
						 dlist_iter_t to,
						 dlist_iter_t where)
{
	dlist_iter_t tmp = NULL;

	assert(from);
	assert(to);
	assert(where);

	DListPrev(where)->next = from;
	DListPrev(from)->next = to;
	DListPrev(to)->next = where;

	tmp = DListPrev(to);
	to->prev = DListPrev(from);
	from->prev = DListPrev(where);
	where->prev = tmp;

	return (tmp);
}

/*---------------------------------------------------------------------------*/

dlist_iter_t DListFind(dlist_iter_t from, 
					   dlist_iter_t to,
					   dlist_is_match_func_t is_match, 
					   void *param)
{
	dlist_iter_t curr_node = from;

	assert(from);
	assert(to);
	assert(is_match);

	while (!(DListIterIsEqual(curr_node, to)) &&
		   !(is_match(DListGetData(curr_node), param)))
	{
		curr_node = DListNext(curr_node);
	}

	return (curr_node);
}

/*---------------------------------------------------------------------------*/

int DListForEach(dlist_iter_t from, 
				 dlist_iter_t to, 
				 dlist_do_action_func_t do_action, 
				 void *param)
{
	dlist_iter_t curr_node = from;
	int curr_status = 0;

	assert(from);
	assert(to);
	assert(do_action);

	while (!(DListIterIsEqual(curr_node, to) && !curr_status))
	{
		curr_status = do_action(DListGetData(curr_node), param);
		curr_node = DListNext(curr_node);
	}

	return (curr_status);
}

/*************************    Utility Functions    ***************************/

static dlist_iter_t DListCreateNode(void *data,
									dlist_iter_t prev,
									dlist_iter_t next)
{
	dlist_iter_t new_node = (dlist_iter_t) malloc(sizeof(struct dlist_node));

	if (NULL == new_node)
	{
		return (NULL);
	}

	new_node->data = data;
	new_node->prev = prev;
	new_node->next = next;

	return (new_node);
}

/*****************************************************************************/




