/*
*	File:			heap_util.c
*	Description:	Implementing util functions for Heap
*	Developer:		Noa Pilosof
*	Reviewer:	 	
*	Date:			TBD
*/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* malloc, free, success, fail */
#include <assert.h>		/* assert */
#include <stdbool.h>	/* true, false */

#include "heapify.h"

#define FIRST_IDX 0
/************************    Custom Structures    ****************************/

/*******************    Utiliy functions declarations    *********************/

static void HeapifySwap(void *element1, void *element2, size_t elem_size);
size_t HeapFloor(size_t n);

/***************************   API Functions    ******************************/


void HeapifyUp(void *base, size_t nmemb, size_t index, size_t elem_size,     
               heapify_cmp_func_t cmp, void *param)
    
{
    char *element = NULL;
    char *parent = NULL;
    size_t parent_index = 0;

    assert(base);
    assert(cmp);

    parent_index = !index ? index : /*HeapFloor(index - 1)*/ (index-1) / 2;
    /*printf("idx:    %d\n", index);
    printf("parent: %d\n", parent_index);*/

    element = (char *)base + (index * elem_size);
    parent = (char *)base + (parent_index * elem_size);
       
    #if 0
    if (0 == index || 0 < cmp(parent, element, param)) 
    {
        return;
    }

    HeapifySwap(element, parent, elem_size);
    HeapifyUp(base, nmemb, parent_index, elem_size, cmp, param);
    #endif
    /*printf("cmp: %d\n", cmp(parent, element, param));*/
    if (index && cmp(parent, element, param) < 0)
    {
        /*printf("swap\n");*/
        HeapifySwap(element, parent, elem_size);
        
        /* to keep the heap stable-sorted after swap, we need to check if the previous parent is not the same as the element before it. if it is, we need to swap them to keep it stable */
        if (0 <= cmp(element, element - elem_size, param))
        {
            HeapifySwap(element, element - elem_size, elem_size);
        }
        
        HeapifyUp(base, nmemb, parent_index, elem_size, cmp, param);
    }
   
}



#if 0
void HeapifyUp(void *base, size_t nmemb, size_t index, size_t elem_size,     
               heapify_cmp_func_t cmp, void *param)
{
    /*size_t cmp_i = (index % 2) == 0 ? index + 1 : index - 1;*/
    int parent = !index ? index : ((index - 1) / 2);
    char *arr = (char *)base;

    (void)nmemb;

    while(parent >= 0 && 0 < cmp(arr + index * elem_size, arr + parent * elem_size, 
    param))
    {
        HeapifySwap(arr + index * elem_size, arr + parent * elem_size, elem_size);
        index = parent;
        parent = (index - 1) / 2;
    }
}
#endif

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
        HeapifySwap(smallest, element, elem_size);
        HeapifyDown(base, nmemb, next_index, elem_size, cmp, param);
    }

}

#if 0
void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param)
{
    size_t son_1 = index * 2 + 1;
    size_t son_2 = index * 2 + 2;
    size_t cmp_i = 0;
    char *arr = (char *)base;

    while(son_1 < nmemb)
    {
        if(son_2 < nmemb)
        {
           cmp_i = (0 < cmp(arr + son_1 * elem_size, arr + son_2 * elem_size, 
           param)) ? son_1 : son_2;
        }
        else
        {
            cmp_i = son_1;
        }
        
        if(0 < cmp(arr + cmp_i * elem_size, arr + index * elem_size, param))
        {
            HeapifySwap(arr + cmp_i * elem_size, arr + index * elem_size, elem_size);
        }

        index = cmp_i;
        son_1 = index * 2 + 1;
        son_2 = index * 2 + 2;
    } 
}
#endif
/*---------------------------------------------------------------------------*/


/*************************    Utility Functions    ***************************/

static void HeapifySwap(void *element1, void *element2, size_t elem_size)
{
    char temp = 0;

    while (elem_size)
    {
        temp = *(char *)element1;
        *(char *)element1 = *(char *)element2;
        *(char *)element2 = temp;

        element1 = (char *)element1 + 1;
        element2 = (char *)element2 + 1;
        --elem_size;
    }
}
/*---------------------------------------------------------------------------*/

size_t HeapFloor(size_t n)
{
    return ((n & 0x1) ? ((n / 2) - 1) : (n / 2));
}