/********************************
* Task name: heap   			*
* Develoer: Inbal Elmalech		*
* Reviewer: ????        		*
* Date: Aug 13, 2020			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <assert.h> /* assert */
#include <stdio.h> /* printf, puts */
#include "utils.h" /* SwapElements */
#include "heapify.h" /* heapify */

/****************************   DECLARATIONS   *******************************/

size_t HeapGetParent(size_t index);
size_t HeapGetLeftChild(size_t index);
size_t HeapGetRightChild(size_t index);

/*************************    General Functions    ***************************/

void HeapifyUp(void *base, size_t nmemb, size_t index,
               size_t elem_size, heapify_cmp_func_t cmp,
               void *param)
{
    char *base_ch = (char *)base;
    char *elem = (void *)(base_ch + (index * elem_size));
    char *parent = (void *)(base_ch + (HeapGetParent(index) * elem_size));

    assert(base);
    assert(cmp);

    /* if index is 0 OR
    index is not 0 AND index does not need to be bubbled up */
    if ((0 == index) || (!(0 > cmp(elem, parent, param))))
    {
        return;
    }

    SwapElements(elem, parent, elem_size);
    
    HeapifyUp(base, nmemb, HeapGetParent(index), elem_size, cmp, param);
}

/*---------------------------------------------------------------------------*/

void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param)
{
    char *element = NULL;
    char *smallest = NULL;
    char *l_child = NULL;
    char *r_child = NULL;
    size_t l_child_idx = 0;
    size_t r_child_idx = 0;
    size_t next_index = 0;

    assert(base);
    assert(cmp);

    /* get indices of children */
    l_child_idx = (2 * index) + 1;
    r_child_idx = (2 * index) + 2;
    
    element = (char *)base + (index * elem_size);
    l_child = (char *)base + (l_child_idx * elem_size);
    r_child = (char *)base + (r_child_idx * elem_size);
    smallest = element;

    /* compares left child against element */
    if (l_child_idx < nmemb && 0 < cmp(l_child, smallest, param))
    {
        smallest = l_child;
        next_index = l_child_idx;
    }

    /* compares right child agains element / left child */
    if (r_child_idx < nmemb && 0 < cmp(r_child, smallest, param))
    {
        smallest = r_child;
        next_index = r_child_idx;
    }

    /* if smallest has changed - swaps and calls function again */
    if (smallest != element)
    {
        SwapElements(smallest, element, elem_size);
        HeapifyDown(base, nmemb, next_index, elem_size, cmp, param);
    }

    #if(0)
    char *base_ch = (char *)base;
    size_t left_i = (index * 2) + 1;
    size_t right_i = (index * 2) + 2;
    size_t swap_index = left_i;
    int cmp_to_l = 0;
    int cmp_to_r = 0;
    int cmp_l_to_r = 0;

    /* out of the array */
    if (left_i >= nmemb)
    {
        return;
    }

    cmp_to_l = cmp((void *)(base_ch + (index * elem_size)),
                   (void *)(base_ch + (left_i * elem_size)), param);

    if ((right_i >= nmemb) && !(0 < cmp_to_l))
    {
        return;
    }
    
    else
    {
        swap_index = left_i;
    }
    
    if (right_i < nmemb)
    {
        cmp_to_r = cmp((void *)(base_ch + (index * elem_size)),
                   (void *)(base_ch + (right_i * elem_size)), param);

        cmp_l_to_r = cmp((void *)(base_ch + (left_i * elem_size)),
                   (void *)(base_ch + (right_i * elem_size)), param);

        if (!(cmp_to_l > 0) && !(cmp_to_r > 0))
        {
            return;
        }

        else if (((cmp_to_l > 0) && (cmp_to_r > 0)) ||
                 (!(cmp_to_l > 0) && (cmp_to_r > 0)))
        {
            swap_index = right_i;
        }
    }

    SwapElements((void *)(base_ch + (index * elem_size)),
                (void *)(base_ch + (swap_index * elem_size)), elem_size);

    HeapifyDown(base, nmemb, swap_index, elem_size, cmp, param);
    #endif
}

/***************************** Utilty Functions ******************************/

size_t HeapGetParent(size_t index)
{
    return((index - 1) / 2);
}

/*---------------------------------------------------------------------------*/

size_t HeapGetLeftChild(size_t index)
{
    return((index * 2) + 1);
}

/*---------------------------------------------------------------------------*/

size_t HeapGetRightChild(size_t index)
{
    return((index * 2) + 2);
}

/*****************************************************************************/