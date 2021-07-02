/********************************
* Task name: bst				*
* Develoer: Inbal Elmalech		*
* Reviewer: Iman Nasreldeen		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* allocs, frees, size_t, exit status */
#include <assert.h> /* assert */
#include <stdio.h> /* printf, puts */
#include <string.h> /* memset, memcpy */
#include "bst.h" /* bst_t */

#define DEADBEEF ((void *)0xDEADBEEF)
/******************************   STRUCTURES   *******************************/
struct bst_node;

struct bst
{
	struct bst_node *end;
	void *param;
	bst_cmp_func_t cmp;
};

struct bst_node
{
	void *data;
	struct bst_node *parent;
	struct bst_node *left;
	struct bst_node *right;
};

/****************************   DECLARATIONS   *******************************/

bst_iter_t BSTInitLeft(struct bst_node *node, void *data,
					   struct bst_node *parent, bst_t *bst);
bst_iter_t BSTInitRight(struct bst_node *node, void *data,
					   struct bst_node *parent, bst_t *bst);
bst_iter_t BSTFreeNodeAndReturnEndIter(struct bst_node *to_free, bst_t *bst);
void FreeLeaf(bst_iter_t parent, bst_iter_t node);
void FreeNodeWithOneChild(bst_iter_t parent, bst_iter_t node, bst_iter_t child);
void FreeNodeWithTwoChlidren(bst_iter_t parent, bst_iter_t node,
							 bst_iter_t successor);
int ChildrenCount(bst_iter_t iter);

/*************************    General Functions    ***************************/

bst_t *BSTCreate(bst_cmp_func_t cmp, void *param)
{
	bst_t *new_bst = NULL;
	struct bst_node *end_dummy = NULL;

	assert(cmp);

	new_bst = (bst_t *) malloc(sizeof(bst_t));

	if (NULL == new_bst)
	{
		return (NULL);
	}

	end_dummy = (struct bst_node *) malloc(sizeof(struct bst_node));

	if (NULL == end_dummy)
	{
		free(new_bst);
		new_bst = NULL;

		return (NULL);
	}

	new_bst->end = end_dummy;
	new_bst->param = param;
	new_bst->cmp = cmp;

	end_dummy->data = DEADBEEF;
	end_dummy->parent = NULL;
	end_dummy->left = NULL;
	end_dummy->right =(struct bst_node *)new_bst;

	return (new_bst);
}

/*---------------------------------------------------------------------------*/

void BSTDestroy(bst_t *bst)
{
	struct bst_node *curr_node = NULL;
	struct bst_node *curr_parent = NULL;

	assert(bst);

	curr_node = bst->end;

	if (NULL != curr_node->parent)
	{
		curr_node->parent->right = NULL;
		curr_node->parent = NULL;
	}

	while (curr_node != (struct bst_node *)bst)
	{
		if (curr_node->left)
		{
			curr_node = curr_node->left;
		}
		
		else if (curr_node->right)
		{
			curr_node = curr_node->right;
		}

		else
		{
			curr_parent = curr_node->parent;

			if (curr_parent)
			{
				(curr_node = curr_parent->left) ?
				(curr_parent->left = NULL) :
				(curr_parent->right = NULL) ;
			}

			free(curr_node);
			curr_node = curr_parent;
		}
	}

	free(bst);
	bst = NULL;
}

/*---------------------------------------------------------------------------*/

size_t BSTSize(const bst_t *bst)
{
	bst_iter_t curr_iter = {NULL};
	bst_iter_t end_iter = {NULL};
	size_t size_of_tree = 0;

	assert(bst);

	curr_iter = BSTBegin(bst);
	end_iter = BSTEnd(bst);

	while (!BSTIterIsEqual(curr_iter, end_iter))
	{
		++size_of_tree;
		curr_iter = BSTNext(curr_iter);
	}

	return (size_of_tree);
}

/*---------------------------------------------------------------------------*/

int BSTIsEmpty(const bst_t *bst)
{
	assert(bst);

	return (NULL == bst->end->parent);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
	bst_iter_t new_iter = {NULL};
	struct bst_node *curr_node = NULL;
	struct bst_node *new_node = NULL;

	bst_cmp_func_t cmp = NULL;
	void *param = NULL;
	int res = 0;

	assert(bst);

	curr_node = bst->end;
	new_iter.node = curr_node;
	new_node = (struct bst_node *) malloc(sizeof(struct bst_node));

	if (NULL == new_node)
	{
		return (new_iter);
	}

	/* if tree is empty */
	if (BSTIsEmpty(bst))
	{
		BSTInitLeft(new_node, data, curr_node, bst);
		curr_node->left = new_node;
		curr_node->parent = new_node;

		new_iter.node = new_node;
		return (new_iter);
	}

	/* if tree is not empty */
	else
	{
		curr_node = curr_node->left;
		cmp = bst->cmp;
		param = bst->param;

		while (curr_node->data != DEADBEEF)
		{
			res = cmp(data, curr_node->data, param);
			if ((-1) == res)
			{
				if (NULL == curr_node->left)
				{
					return (BSTInitLeft(new_node, data, curr_node, bst));
				}

				curr_node = curr_node->left;
			}

			else if (1 == res)
			{
				if (NULL == curr_node->right)
				{
					return (BSTInitRight(new_node, data, curr_node, bst));
				}

				curr_node = curr_node->right;
			}

			else
			{
				return (BSTFreeNodeAndReturnEndIter(new_node, bst));
			}			
		}
	}

	return (new_iter);
}

/*---------------------------------------------------------------------------*/

void BSTRemove(bst_iter_t iter)
{
	bst_iter_t curr_iter = iter;
	bst_iter_t successor = {NULL};
	bst_iter_t curr_child = {NULL};
	bst_iter_t curr_parent = {NULL};
	int nof_children = ChildrenCount(iter);


	curr_parent.node = iter.node->parent;
	/* if iter node has no children */
	if (0 == nof_children)
	{
		FreeLeaf(curr_parent, curr_iter);
	}

	/* if iter has one child */
	else if (1 == nof_children)
	{
		curr_child.node = curr_iter.node->right ?
						  curr_iter.node->right :
						  curr_iter.node->left;
		FreeNodeWithOneChild(curr_parent, curr_iter, curr_child);
	}

	/* if iter has two children */
	else
	{
		successor = BSTPrev(curr_iter);
		FreeNodeWithTwoChlidren(curr_parent, curr_iter, successor);
	}

	return;
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTBegin(const bst_t *bst)
{
	bst_iter_t begin_iter = {NULL};
	struct bst_node *begin_node = NULL;
	assert(bst);

	begin_node = bst->end;

	while (NULL != begin_node->left)
	{
		begin_node = begin_node->left;
	}

	begin_iter.node = begin_node;
	return (begin_iter);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTEnd(const bst_t *bst)
{
	bst_iter_t end_iter = {NULL};

	assert(bst);

	end_iter.node = bst->end;
	return (end_iter);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTPrev(bst_iter_t iter)
{
	struct bst_node *curr_node = iter.node;
	bst_iter_t end_iter = {NULL};
	bst_iter_t next_iter = {NULL};
	bst_t *bst = NULL;
	bst_cmp_func_t cmp = NULL;
	void *param = NULL;

	/* looping to get to end, where the ptr to bst is set as the right child */
	while (DEADBEEF != curr_node->data)
	{
		printf("%p\n", curr_node);
		curr_node = curr_node->parent;
	}

	/* retrieving cmp and param from end via bst ptr */
	bst = (bst_t *)(curr_node->right);
	cmp = bst->cmp;
	param = bst->param;

	end_iter = BSTEnd(bst);

	/* if iter is smallest or list is empty*/
	if (BSTIterIsEqual(iter, BSTBegin(bst)) || BSTIsEmpty(bst))
	{
		return (end_iter);
	}

	/* if iter is end */
	if (BSTIterIsEqual(iter, end_iter))
	{
		next_iter.node = end_iter.node->parent;
		return (next_iter);
	}

	/* returning curr to iter given by user */
	curr_node = iter.node;

	/* if curr has no left child, going up the tree until a parent's data
	is smaller than the given iter's data */
	if (NULL == curr_node->left)
	{
		while ((1) != cmp(iter.node->data, curr_node->data, param))
		{
			curr_node = curr_node->parent;
		}

		next_iter.node = curr_node;

		return (next_iter);
	}

	/* if it has left child, going to it and then right all the way */
	curr_node = curr_node->left;

	while (curr_node->right)
	{
		curr_node = curr_node->right;
	}
	
	next_iter.node = curr_node;

	return (next_iter);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTNext(bst_iter_t iter)
{
	struct bst_node *curr_node = iter.node;
	bst_iter_t next_iter = {NULL};
	bst_cmp_func_t cmp = NULL;
	void *param = NULL;

	/* if iter is the largest element */
	if ((NULL != curr_node->right) && (curr_node->right->data == DEADBEEF))
	{
		next_iter.node = curr_node->right;
		return (next_iter);
	}

	/* looping to get to end */
	while (DEADBEEF != curr_node->data)
	{
		curr_node = curr_node->parent;
	}

	/* retrieving cmp and param from end via bst ptr */
	cmp = ((bst_t *)(curr_node->right))->cmp;
	param = ((bst_t *)(curr_node->right))->param;

	/* returning curr to iter given by user */
	curr_node = iter.node;

	/* if curr has no right child, going up the tree until a parent's data
	is larger than the given iter's data */
	if (NULL == curr_node->right)
	{
		while ((-1) != cmp(iter.node->data, curr_node->data, param))
		{
			curr_node = curr_node->parent;
		}

		next_iter.node = curr_node;

		return (next_iter);
	}

	/* if it has a right child, going to it and then left all the way */
	curr_node = curr_node->right;

	while (curr_node->left)
	{
		curr_node = curr_node->left;
	}
	
	next_iter.node = curr_node;

	return (next_iter);
}

/*---------------------------------------------------------------------------*/

int BSTIterIsEqual(bst_iter_t iter1, bst_iter_t iter2)
{
	assert(iter1.node);
	assert(iter2.node);

	return (iter1.node == iter2.node);
}

/*---------------------------------------------------------------------------*/

void *BSTGetData(bst_iter_t iter)
{
	assert(iter.node);

	return (iter.node->data);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
	bst_iter_t curr = {NULL};

	assert(bst);

	curr = BSTBegin(bst);

	while (!BSTIterIsEqual(curr, BSTEnd(bst)))
	{
		if (data == BSTGetData(curr))
		{
			return (curr);
		}

		curr = BSTNext(curr);
	}

	return (curr);
}

/*---------------------------------------------------------------------------*/

int BSTForEach(bst_iter_t from, bst_iter_t to, 
				bst_do_action_func_t do_action ,void *param)
{
	assert(do_action);

	while (!BSTIterIsEqual(from, to))
	{
		if (0 != do_action(BSTGetData(from), param))
		{
			return (1);
		}

		from = BSTNext(from);
	}

	return (0);
}

/***************************** Utilty Functions ******************************/

bst_iter_t BSTInitLeft(struct bst_node *node, void *data,
					   struct bst_node *parent, bst_t *bst)
{
	bst_iter_t node_iter = {NULL};
	node->data = data;
	node->parent = parent;
	node->left = NULL;
	node->right = NULL;

	if (parent == bst->end)
	{
		node->right = parent;
		node->right->parent = node;
	}
	
	parent->left = node;
	node_iter.node = node;

	return (node_iter);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTInitRight(struct bst_node *node, void *data,
					   struct bst_node *parent, bst_t *bst)
{
	bst_iter_t node_iter = {NULL};
	node->data = data;
	node->parent = parent;
	node->left = NULL;
	node->right = NULL;

	if (parent->right == bst->end)
	{
		node->right = bst->end;
		node->right->parent = node;
	}
	
	parent->right = node;
	node_iter.node = node;

	return (node_iter);
}

/*---------------------------------------------------------------------------*/

bst_iter_t BSTFreeNodeAndReturnEndIter(struct bst_node *to_free, bst_t *bst)
{
	bst_iter_t end_iter = {NULL};

	free(to_free);
	to_free = NULL;
	end_iter.node = bst->end;

	return (end_iter);
}

/*---------------------------------------------------------------------------*/

void FreeLeaf(bst_iter_t parent, bst_iter_t node)
{
	/* set ref from parent */
	if (node.node == parent.node->left)
	{
		parent.node->left = NULL;
	}

	parent.node->right = NULL;

	free(node.node);
	node.node = NULL;

	return;
}

/*---------------------------------------------------------------------------*/

void FreeNodeWithOneChild(bst_iter_t parent, bst_iter_t node, bst_iter_t child)
{
	/* set ref from parent */
	if (node.node == parent.node->left)
	{
		parent.node->left = child.node;
	}

	parent.node->right = child.node;
	child.node->parent = parent.node;

	/* if node is the last element, find prev and set right child to end */
	if (DEADBEEF == node.node->right->data)
	{
		BSTPrev(node).node->right = node.node->right;
	}

	free(node.node);
	node.node = NULL;
}

/*---------------------------------------------------------------------------*/

void FreeNodeWithTwoChlidren(bst_iter_t parent, bst_iter_t node,
							 bst_iter_t successor)
{
	/* connecting node's right child to successor's right child */
	successor.node->right = node.node->right;
	node.node->right->parent = successor.node;

	/* connecting node's parent with node's left child in its stead */
	if (node.node == parent.node->left)
	{
		parent.node->left = node.node->left;
	}

	parent.node->right = node.node->left;
	node.node->left->parent = parent.node;

	free(node.node);
	node.node = NULL;
}

/*---------------------------------------------------------------------------*/

int ChildrenCount(bst_iter_t iter)
{
	return (!!(iter.node->left) + !!(iter.node->left));
}

/*****************************************************************************/



