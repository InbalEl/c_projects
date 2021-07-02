/********************************
* Task name: avl				*
* Develoer: Inbal Elmalech		*
* Reviewer: Ido Finkelstein		*
* Date: Aug 4, 2020				*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* allocs, frees, size_t, exit status */
#include <assert.h> /* assert */
#ifndef DNDEBUG
#include <stdio.h>	/* printf */
#endif
#include "avl.h"	/* bst_t */

#define DEADBEEF ((void *)0xDEADBEEF)

enum traverse_order
{
	pre_order,
	in_order,
	post_order
};

typedef enum
{
	LEFT = 0, /* DO NOT CHANGE, THESE ARE INDICES */
	RIGHT
}child_dir;

/******************************   STRUCTURES   *******************************/
typedef struct avl_node node_t;

struct avl
{
	node_t *root;
	avl_cmp_func_t cmp;
	void *param;
};

struct avl_node
{
	void *data;
	node_t *children[2];
};

/****************************   DECLARATIONS   *******************************/
/***********************    Recursive Functions    ***************************/

static node_t *R_Insert(node_t *curr, avl_t *avl, void *data);
static node_t *R_Remove(node_t *curr, avl_t *avl, void *data);
static int R_ForEach(node_t *curr,
					 avl_do_action_func_t do_action, void *param);
static void *R_Find(node_t *curr, void *data, avl_t *avl,
					avl_cmp_func_t cmp, void *param);
static size_t R_Size(node_t *curr_root);
static void R_Destroy(node_t *curr);
static size_t R_GetHeight(node_t *node);

/************************    Rotation Functions    ***************************/
static node_t *RebalanceTree(avl_t *avl, node_t *node, int balance_factor);
static node_t *BalanceLeftBranch(avl_t *avl, node_t *curr);
static node_t *LLRotation(avl_t *avl, node_t *gen_I);
static node_t *LRRotation(avl_t *avl, node_t *curr);
static node_t *BalanceRightBranch(avl_t *avl, node_t *curr);
static node_t *RRRotation(avl_t *avl, node_t *curr);
static node_t *RLRotation(avl_t *avl, node_t *curr);

/*************************    Utility Functions    ***************************/

static node_t *RemoveNode(node_t *node, avl_t *avl);
static node_t *InitNewNode(void *data);
/*static void TraverseTree(avl_t *avl, enum traverse_order);*/
void Print2D(node_t *root, int space);
void AVLPrint2D(const avl_t *avl);
static int GetBalanceFactor(node_t *node);
static size_t max(size_t a, size_t b);

/* 
CODE REVIEW:
-	instead of defining left and right, you can put an array of
	node_t in the struct of node.
	THen define:

	typedef enum
	{
		LEFT = 0, / DO NOT CHANGE, THESE ARE INDICES /
		RIGHT
	}child_dir;

	and use the enum to determine, like so:
	int child_index = 0;

	child_index = (avl->cmp(node->data, data, avl->param) > 0)


-	In size func, no need to address the empty tree case separately -
	the recursive function should takecare of if (see fixed func)

	this is relevant for find as well, and any other wrapper function
	that checks for empty tree case.

-	Put stdio in ifdef
*/

/*************************    General Functions    ***************************/

avl_t *AVLCreate(avl_cmp_func_t cmp, void *param)
{
	avl_t *new_avl = NULL;

	assert(cmp);

	new_avl = (avl_t *)malloc(sizeof(avl_t));

	if (NULL == new_avl)
	{
		return (NULL);
	}

	new_avl->root = NULL;
	new_avl->param = param;
	new_avl->cmp = cmp;

	return (new_avl);
}

/*---------------------------------------------------------------------------*/

void AVLDestroy(avl_t *avl)
{
	assert(avl);

	R_Destroy(avl->root);

	free(avl);
	avl = NULL;
}

/*---------------------------------------------------------------------------*/

size_t AVLSize(const avl_t *avl)
{
	assert(avl);

	return (R_Size(avl->root));
}

/*---------------------------------------------------------------------------*/

int AVLIsEmpty(const avl_t *avl)
{
	assert(avl);

	return (NULL == avl->root);
}

/*---------------------------------------------------------------------------*/

int AVLInsert(avl_t *avl, void *data)
{
	assert(avl);

	return (NULL == R_Insert(avl->root, avl, data));
}

/*---------------------------------------------------------------------------*/

void AVLRemove(avl_t *avl, void *data)
{
	assert(avl);

	R_Remove(avl->root, avl, data);
}

/*---------------------------------------------------------------------------*/

void *AVLFind(const avl_t *avl, const void *data)
{
	assert(avl);

	return (R_Find(avl->root, (void *)data, (avl_t *)avl, 
				   avl->cmp, avl->param));
}

/*---------------------------------------------------------------------------*/

int AVLForEach(avl_t *avl, avl_do_action_func_t do_action, void *param)
{
	assert(avl);
	assert(do_action);

	return (R_ForEach(avl->root, do_action, param));
}

/***************************** Utilty Functions ******************************/

static node_t *R_Insert(node_t *curr, avl_t *avl, void *data)
{
	node_t *new_node = NULL;
	int balance_factor = 0;
	int cmp_res = 0;

	if (curr == NULL)
	{
		new_node = InitNewNode(data);
		if (NULL == avl->root)
		{
			avl->root = new_node;
		}
		
		return (new_node);
	}

	cmp_res = avl->cmp(data, curr->data, avl->param);

	if (0 == cmp_res)
	{
		return (NULL);
	}
	
	new_node = R_Insert(curr->children[cmp_res > 0], avl, data);

	balance_factor = GetBalanceFactor(curr);

	new_node = RebalanceTree(avl, curr, balance_factor);

	return (new_node);
}

/*---------------------------------------------------------------------------*/

static void R_Destroy(node_t *curr)
{
	if (NULL == curr)
	{
		return;
	}

	R_Destroy(curr->children[LEFT]);
	R_Destroy(curr->children[RIGHT]);
	free(curr);
	curr = NULL;
}

/*---------------------------------------------------------------------------*/

static node_t *InitNewNode(void *data)
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));

	if (NULL == new_node)
	{
		return (new_node);
	}

	new_node->data = data;
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return (new_node);
}

/*---------------------------------------------------------------------------*/

static node_t *R_Remove(node_t *curr, avl_t *avl, void *data)
{
	int balance_factor = 0;
	int cmp_res = 0;

	if (NULL == curr)
	{
		return (NULL);
	}

	cmp_res = avl->cmp(data, curr->data, avl->param);

	if (0 == cmp_res)
	{
		curr = RemoveNode(curr, avl);
		return (curr);
	}

	curr = R_Remove(curr->children[cmp_res > 0], avl, data);

	balance_factor = GetBalanceFactor(curr);

	curr = RebalanceTree(avl, curr, balance_factor);

	return (curr);
}

/*---------------------------------------------------------------------------*/

static node_t *RemoveNode(node_t *node, avl_t *avl)
{
	/* TODO: THINK ABOUT ROOT REMOVALS!!! */
	node_t *successor = NULL;

	/* if node_to_remove is a leaf */
	 if ((NULL == node->children[LEFT]) &&
	 	 (NULL == node->children[RIGHT]))
	{
		free(node);
		node = NULL;
		return (NULL);
	}

	/* if node_to_remove has one child */
	else if ((NULL == node->children[LEFT]) ||
			 (NULL == node->children[RIGHT]))
	{
		successor = node->children[NULL == node->children[LEFT]];
		free(node);
		node = NULL;
		
		return (successor);
	}

	/* if node_to_remove has two children */
	else
	{
		/* finding successor (prev) */
		successor = node->children[LEFT];

		while (successor && successor->children[RIGHT])
		{
			successor = successor->children[RIGHT];
		}

		/* copying data from successor to node_to_remove */
		node->data = successor->data;

		/* calling R_Remove with successor's data within
		node_to_remove's left sub-tree */
		node->children[LEFT] = R_Remove(node->children[LEFT], avl, node->data);

		return (node);
	}
}

/*---------------------------------------------------------------------------*/

static int R_ForEach(node_t *curr, avl_do_action_func_t do_action, void *param)
{
	int status = 0;

	if (NULL == curr)
	{
		return (0);
	}

	status = R_ForEach(curr->children[LEFT], do_action, param);

	if (1 == status)
	{
		return (status);
	}

	status = do_action(curr->data, param);

	if (1 == status)
	{
		return (status);
	}

	status = R_ForEach(curr->children[RIGHT], do_action, param);

	if (1 == status)
	{
		return (status);
	}

	return (status);
}

/*---------------------------------------------------------------------------*/

static void *R_Find(node_t *curr, void *data, avl_t *avl,
					avl_cmp_func_t cmp, void *param)
{
	int child_index = 0;

	if (NULL == curr)
	{
		return (NULL);
	}

	if (0 == cmp(data, curr->data, param))
	{
		return (curr->data);
	}

	child_index = (avl->cmp(curr->data, data, avl->param) > 0);

	return (R_Find(curr->children[child_index], avl, data, cmp, param));
}

/*---------------------------------------------------------------------------*/

static size_t R_Size(node_t *curr_root)
{
	if (NULL == curr_root)
	{
		return (0);
	}

	return (1 + R_Size(curr_root->children[LEFT]) +
				R_Size(curr_root->children[RIGHT]));

	/*size_t nof_nodes = 0;
	
	if (NULL != curr_root)
	{
		nof_nodes += R_Size(curr_root->left);
		++nof_nodes;
		nof_nodes += R_Size(curr_root->right);
	}

	return (nof_nodes);*/
}

/*---------------------------------------------------------------------------*/

static int GetBalanceFactor(node_t *node)
{
	return (R_GetHeight(node->children[LEFT]) -
			R_GetHeight(node->children[RIGHT]));
}

/*---------------------------------------------------------------------------*/

static size_t R_GetHeight(node_t *node)
{
	if (NULL == node)
	{
		return (0);
	}

	return (1 + max(R_GetHeight(node->children[LEFT]),
					R_GetHeight(node->children[RIGHT])));
}

/*---------------------------------------------------------------------------*/

static node_t *RebalanceTree(avl_t *avl, node_t *node, int balance_factor)
{
	/* left-heavy tree imbalance */
	if (1 < balance_factor)
	
	{
		return (BalanceLeftBranch(avl, node));
	}

	/* right-heavy tree imbalance */
	else if ((-1) > balance_factor)
	{
		return (BalanceRightBranch(avl, node));
	}

	return (node);
}

/*---------------------------------------------------------------------------*/

static node_t *BalanceLeftBranch(avl_t *avl, node_t *curr)
{
	if (R_GetHeight(curr->children[LEFT]) > R_GetHeight(curr->children[RIGHT]))
	{
		return (LLRotation(avl, curr));
	}

	else
	{
		return (LRRotation(avl, curr));
	}
}

/*---------------------------------------------------------------------------*/

static node_t *LLRotation(avl_t *avl, node_t *gen_I)
{
	node_t *gen_II = gen_I->children[LEFT];
	gen_I->children[LEFT] = gen_II->children[RIGHT];
	gen_II->children[RIGHT] = gen_I;

	if (gen_I == avl->root)
	{
		avl->root = gen_II;
	}

	return (gen_II);
}

/*---------------------------------------------------------------------------*/

static node_t *LRRotation(avl_t *avl, node_t *gen_I)
{
	node_t *gen_II = gen_I->children[LEFT];
	node_t *gen_III = gen_II->children[RIGHT];

	gen_II->children[RIGHT] = gen_III->children[LEFT];
	gen_I->children[LEFT] = gen_III->children[RIGHT];

	gen_III->children[LEFT] = gen_II;
	gen_III->children[RIGHT] = gen_I;

	if (gen_I == avl->root)
	{
		avl->root = gen_III;
	}

	return (gen_III);
}

/*---------------------------------------------------------------------------*/

static node_t *BalanceRightBranch(avl_t *avl, node_t *curr)
{
	if (R_GetHeight(curr->children[RIGHT]) > R_GetHeight(curr->children[LEFT]))
	{
		return (RRRotation(avl, curr));
	}

	else
	{
		return (RLRotation(avl, curr));
	}
}

/*---------------------------------------------------------------------------*/

static node_t *RRRotation(avl_t *avl, node_t *gen_I)
{
	node_t *gen_II = gen_I->children[RIGHT];
	gen_I->children[RIGHT] = gen_II->children[LEFT];
	gen_II->children[LEFT] = gen_I;

	if (gen_I == avl->root)
	{
		avl->root = gen_II;
	}

	return (gen_II);
}

/*---------------------------------------------------------------------------*/

static node_t *RLRotation(avl_t *avl, node_t *gen_I)
{
	node_t *gen_II = gen_I->children[RIGHT];
	node_t *gen_III = gen_II->children[LEFT];

	gen_II->children[LEFT] = gen_III->children[RIGHT];
	gen_I->children[RIGHT] = gen_III->children[LEFT];

	gen_III->children[RIGHT] = gen_II;
	gen_III->children[LEFT] = gen_I;

	if (gen_I == avl->root)
	{
		avl->root = gen_III;
	}

	return (gen_III);
}

/*---------------------------------------------------------------------------*/

/*static void TraverseTree(avl_t *avl, enum traverse_order order)
{
	(void)avl;
	if (0 == order)
	{
		* traverse pre-order *
	}

	else if (1 == order)
	{
		* traverse in-order *
	}

	else
	{
		* traverse post-order *
	}
}*/

/*---------------------------------------------------------------------------*/

void Print2D(node_t *root, int space) 
{ 
    int i = 0;
    if (root == NULL) 
    {
        return; 
    }
    space += 10; 
  
    Print2D(root->children[RIGHT], space); 
  
    printf("\n"); 
    for(i = 10; i < space; ++i) 
    {
        printf(" ");
    } 
    printf("%d\n", *(int*)root->data); 
  
    Print2D(root->children[LEFT], space); 
}

/*---------------------------------------------------------------------------*/

void AVLPrint2D(const avl_t *avl)
{
    Print2D(avl->root, 0);
}

/*---------------------------------------------------------------------------*/

static size_t max(size_t a, size_t b)
{
	return (a > b ? a : b);
}
/*****************************************************************************/