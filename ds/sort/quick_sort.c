/********************************
* Task name: Merge sort 		*
* Develoer: Inbal Elmalech		*
* Reviewer: Reut Ben Harush		*
* Review Date: 08/08/2020		*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* size_t, exit status */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include "sort.h" /* sort */
#include "utils.h" /* swap */
/*************************    General Functions    ***************************/

static void *QSPartition(void *base, void *last, size_t size,
                         sort_cmp_func_t cmp);


void QuickSort(void *base, size_t nmemb, size_t size, sort_cmp_func_t cmp)
{
    void *last = NULL;
    void *pivot = NULL;

    assert(base);
    assert(cmp);

    last = (void *)((char *)base + ((nmemb - 1) * size));
    pivot = QSPartition(base, last, size, cmp);

    /* sort subarray left of pivot's new location */
    QuickSort(base,
              (((size_t)pivot - (size_t)base) / size),
              size,
              cmp);
    /* sort subarray right of pivot's new location */
    QuickSort((void *)((char *)pivot + size),
              (((size_t)last - (size_t)pivot) / size),
              size,
              cmp);
}

/*---------------------------------------------------------------------------*/

void *QSPartition(void *base, void *last, size_t size, sort_cmp_func_t cmp)
{
    void *pivot = last;
    void *left_scanner = base;
    void *right_scanner = (void *)((char *)last - size);

    while ((size_t)left_scanner < (size_t)right_scanner)
    {
        /* swapping values bwteen right and left */
        SwapElements(left_scanner, right_scanner, size);

        /* keep moving right and left scanners */
        while (1 > cmp(left_scanner, pivot, NULL))
        {
            left_scanner = (void *)(((char *)left_scanner + size));
        }

        while ((-1) < cmp(right_scanner, pivot, NULL))
        {
            right_scanner = (void *)(((char *)left_scanner - size));
        }
    }

    /* swapping values bwteen pivot and left */
    SwapElements(left_scanner, pivot, size);

    return (left_scanner);
}

/*****************************************************************************/




