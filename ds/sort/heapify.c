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
void Swap(void *dest, void *src, size_t size);
/*************************    General Functions    ***************************/

void HeapifyUp(void *base, size_t nmemb, size_t index,
               size_t elem_size, heapify_cmp_func_t cmp,
               void *param)
{
    char *base_ch = (char *)base;
    size_t parent_i = 0;

    assert(base);

    parent_i = (index - 1) / 2;

    if ((0 == index) ||
        (!(0 > cmp((void *)(base_ch + (index * elem_size)),
                   (void *)(base_ch + (parent_i * elem_size)),
                   param))))
    {
        return;
    }

    else
    {
        SwapElements((void *)(base_ch + (index * elem_size)),
                     (void *)(base_ch + (parent_i * elem_size)), elem_size);

        HeapifyUp(base, nmemb, parent_i, elem_size, cmp, param);
    }    
}

/*---------------------------------------------------------------------------*/

void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param)
/*{
    char *base_ch = (char *)base;
    size_t left_i = (index * 2) + 1;
    size_t right_i = (index * 2) + 2;
    int cmp_to_l = 0;
    int cmp_to_r = 0;
    int cmp_l_to_r = 0;

    / out of the array /
    if (right_i >= nmemb)
    {
        return;
    }

    / saving comparison of index with left child /
    cmp_to_l = cmp((void *)(base_ch + (index * elem_size)),
                   (void *)(base_ch + (left_i * elem_size)), param);

    /
    6 if there is only left child, and if index is greater than left child /
    if ((left_i >= nmemb) && (0 < cmp_to_l))
    {
        SwapElements((void *)(base_ch + (index * elem_size)),
                     (void *)(base_ch + (left_i * elem_size)), elem_size);

        HeapifyDown(base, nmemb, left_i, elem_size, cmp, param);
    }


    else
    {
        cmp_to_r = cmp((void *)(base_ch + (index * elem_size)),
                       (void *)(base_ch + (right_i * elem_size)), param);

        cmp_l_to_r = cmp((void *)(base_ch + (left_i * elem_size)),
                         (void *)(base_ch + (right_i * elem_size)), param);


        if ((0 > cmp_to_l) && (0 > cmp_to_r))
        {
            return;
        }

        else if ((0 > cmp_to_l) && (0 < cmp_to_r))
        {
            SwapElements((void *)(base_ch + (index * elem_size)),
                         (void *)(base_ch + (right_i * elem_size)), elem_size);

            HeapifyDown(base, nmemb, right_i, elem_size, cmp, param);
        }

        else if ((0 < cmp_to_l) && (0 > cmp_to_r))
        {
            SwapElements((void *)(base_ch + (index * elem_size)),
                         (void *)(base_ch + (left_i * elem_size)), elem_size);

            HeapifyDown(base, nmemb, left_i, elem_size, cmp, param);
        }

        else
        {
            if (0 < cmp_l_to_r)
            {
                SwapElements((void *)(base_ch + (index * elem_size)),
                        (void *)(base_ch + (left_i * elem_size)), elem_size);

                HeapifyDown(base, nmemb, left_i, elem_size, cmp, param);
            }

            else
            {
                SwapElements((void *)(base_ch + (index * elem_size)),
                        (void *)(base_ch + (right_i * elem_size)), elem_size);

                HeapifyDown(base, nmemb, right_i, elem_size, cmp, param);
            }
        }
    }
}*/
{
    char *array = NULL;
    size_t min_index = 0;

    assert(base);

    array = (char *)base;


    if(HeapGetRightChild(index) < nmemb)
    {
        if(cmp(array + HeapGetRightChild(index) * elem_size,
        array + HeapGetLeftChild(index) * elem_size, param) > 0)
        {
            min_index = HeapGetLeftChild(index);
        }
        else
        {
            min_index = HeapGetRightChild(index);
        }
    }
    else if(HeapGetLeftChild(index) < nmemb)
    {
        min_index = HeapGetLeftChild(index);
    }
    
    if (HeapGetLeftChild(index) >= nmemb || (cmp(array + index * elem_size,
        array + min_index * elem_size, param) < 0))
    {
        return;
    }

    Swap(array + index * elem_size, array + min_index * elem_size, elem_size);

    HeapifyDown(base, nmemb, min_index, elem_size, cmp, param);
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

/*---------------------------------------------------------------------------*/

void Swap(void *dest, void *src, size_t size)
{
    char *temp_dest = dest;
    char *temp_src = src;
    char temp = '\0';
    size_t i = 0;

    assert(dest);
    assert(src);

    for(i = 0; i < size; ++i)
    {
        temp = *temp_dest;
        *temp_dest = *temp_src;
        *temp_src = temp;

        ++temp_dest;
        ++temp_src;
    }
}

/*****************************************************************************/