/********************************
* Task name: Heap sort   		*
* Develoer: Inbal Elmalech		*
* Reviewer: Tali Raphael    	*
* Review Date: 15/08/2020		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include "sort.h" /* sort */
#include "sort.h" /* sort */
#include "utils.h" /* swap elements */
#include "heapify.h" /* heapify down */

/*************************    General Functions    ***************************/

void HeapSort(void *base, size_t nmemb, size_t size,
              sort_cmp_func_t cmp,
              void *arg)
{
    int i = 0;
    assert(base);
    assert(cmp);
    
    for (i = 0; i < nmemb; ++i)
    {
        HeapifyUp(base, nmemb - 1, i, size, cmp, arg);
    }

    for (i = nmemb - 1; i > 0; --i)
    {
        SwapElements(base, (void *)((char *)base + (size * i)), size);
        HeapifyDown(base, i, 0, size, cmp, arg);
    }
}

/*****************************************************************************/



