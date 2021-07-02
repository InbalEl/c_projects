/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free, EXIT_SUCCESS, EXIT_FAILURE */
#include <assert.h> /* assert */

#include "slist.h" /* slist */

/***************************   ENUMS & STRUCTURES  ***************************/
struct slist
{
	slist_iter_t head;
	slist_iter_t tail;
};

struct slist_node
{
	void *data;
	slist_iter_t next;
};

/********************    Utility Functions Declaration   *********************/
static slist_iter_t SListCreateNode(void *data, slist_iter_t next);

/*************************    General Functions    ***************************/
slist_t *SListCreate(void)
{
	slist_t *new_list_ptr = NULL;
	slist_iter_t dummy_ptr = {NULL};

	new_list_ptr = (slist_t *) malloc(sizeof(slist_t));
	if (!new_list_ptr)
	{
		return (new_list_ptr);
	}

	dummy_ptr = SListCreateNode(NULL, NULL);
	if (!dummy_ptr)
	{
		/* cleanups */
		free(new_list_ptr);
		new_list_ptr = NULL;

		return (dummy_ptr);
	}

	new_list_ptr->head = dummy_ptr;
	new_list_ptr->tail = dummy_ptr;
	dummy_ptr->data = NULL;
	dummy_ptr->next = NULL;

	return (new_list_ptr);
}

/*---------------------------------------------------------------------------*/
void SListDestroy(slist_t *slist)
{
	free(slist->tail); /* dummy */
	free(slist);
}

/*---------------------------------------------------------------------------*/
void SlistEmptyList(slist_t *slist)
{
	slist_iter_t curr = {NULL};
	slist_iter_t next = {NULL};

	assert(slist);

	curr = slist->head;
	next = curr->next;

	while (next)
	{
		free(curr);
		curr = next;
		next = curr->next;
	}

	slist->head = curr;
	slist->tail = curr;
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListBegin(const slist_t *slist)
{
	assert(slist);

	return (slist->head);
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListEnd(const slist_t *slist)
{
	assert(slist);

	return (slist->tail);
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListNext(const slist_iter_t iter)
{
	assert(iter);

	return (iter->next);
}

/*---------------------------------------------------------------------------*/
void *SListGetData(const slist_iter_t iter)
{
	assert(iter);

	return (iter->data);
}

/*---------------------------------------------------------------------------*/
void SListSetData(slist_iter_t iter, void *data)
{
	assert(iter);

	iter->data = data;
}

/*---------------------------------------------------------------------------*/
int SListIterIsEqual(const slist_iter_t iter1, const slist_iter_t iter2)
{
	assert(iter1);
	assert(iter2);

	return (iter1 == iter2);
}

/*---------------------------------------------------------------------------*/
int SListIsEmpty(const slist_t *slist)
{
	assert(slist);

	return (slist->head == slist->tail);
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListInsert(slist_t *slist, slist_iter_t where, void *data)
{
	slist_iter_t tmp = {NULL};
	assert(slist);
	assert(where);

	tmp = SListCreateNode(where->data, where->next);

	if (!tmp)
	{
		return (SListEnd(slist));
	}

	where->data = data;
	where->next = tmp;

	if (where == SListEnd(slist))
	{
		slist->tail = tmp;
	}

	return (tmp);
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListInsertAfter(slist_t *slist, slist_iter_t where, void *data)
{
	slist_iter_t new_node = {NULL};
	assert(slist);
	assert(where);

	if (where == SListEnd(slist))
	{
		return (where);
	}

	new_node = SListCreateNode(data, SListNext(where));
	if (!new_node)
	{
		return (SListEnd(slist));
	}

	where->next = new_node;

	return (new_node);
}

/*---------------------------------------------------------------------------*/

slist_iter_t SListRemove(slist_t *slist, slist_iter_t iter)
{
	slist_iter_t tmp = {NULL};
	assert(slist);
	assert(iter);

	if (iter == SListEnd(slist))
	{
		return (iter);
	}

	/* copying the next node into iter and freeing iterr->next */
	tmp = SListNext(iter);
	*iter = *(tmp);

	if (NULL == SListNext(iter))
	{
		slist->tail = iter;
	}

	free(tmp);
	tmp = NULL;

	return (SListNext(iter));
}

/*---------------------------------------------------------------------------*/
size_t SListCount(const slist_t *slist)
{
	size_t nof_elements = 0;
	slist_iter_t curr = NULL;
	slist_iter_t end = NULL;

	assert(slist);
	curr = SListBegin(slist);
	end = SListEnd(slist);

	while (curr != end)
	{
		curr = SListNext(curr);
		++nof_elements;
	}

	return (nof_elements);
}

/*---------------------------------------------------------------------------*/
slist_iter_t SListFind(const slist_iter_t from, const slist_iter_t to,
					   slist_is_match_func_t is_match, void *param)
{
	slist_iter_t key = NULL;
	assert(from);
	assert(to);
	key = from;

	while (key != to)
	{
		if (is_match(param, key->data))
		{
			return (key);
		}

		key = SListNext(key);
	}

	return (to);
}

/*---------------------------------------------------------------------------*/
int SListForEach(const slist_iter_t from, const slist_iter_t to, 
				 slist_do_action_func_t do_action, void *param)
{
	slist_iter_t key = NULL;
	assert(from);
	assert(to);
	key = from;

	while (key != to)
	{
		if (do_action(param, key->data))
		{
			return (EXIT_FAILURE);
		}

		key = SListNext(key);
	}

	return (EXIT_SUCCESS);
}

/*---------------------------------------------------------------------------*/
void SListAppend(slist_t *dest, slist_t *src)
{
	assert(dest);
	assert(src);

	*(dest->tail) = *(src->head);

	dest->tail = src->tail;
	src->tail = src->head;

	src->head->data = NULL;
	src->head->next = NULL;
}

/*************************    Utility Functions    ***************************/

static slist_iter_t SListCreateNode(void *data, slist_iter_t next)
{
	slist_iter_t new_node = (slist_iter_t) malloc(sizeof(struct slist_node));
	if (!new_node)
	{
		return (NULL);
	}

	new_node->data = data;
	new_node->next = next;

	return (new_node);
}

/*****************************************************************************/




