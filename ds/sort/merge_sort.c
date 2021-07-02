#include <stdlib.h> /*malloc, free*/
#include <string.h> /*memcpy*/

#include "sort.h"


static void Merge(void *base, void *left, void *right, sort_cmp_func_t cmp,
           size_t size, size_t nmemb_left, size_t nmemb_right, void *param);

void MergeSort(void *base, size_t nmemb, size_t size,
                sort_cmp_func_t cmp,
                void *param)
{
    size_t mid = 0;
    void *left = NULL;
    void *right = NULL;

    if(nmemb < 2)
    {
        return;
    }
    
    mid  = nmemb / 2;

    left = malloc(size * mid);
    if(NULL == left)
    {
        return;
    }
    right = malloc(size * (nmemb - mid));
    if (NULL == right)
    {
        free (left);
        left = NULL;
        return;
    }

    memcpy(left, base, size * mid);
    memcpy(right, ((char *)base + mid * size), (nmemb - mid) * size);

    MergeSort(left, mid, size, cmp, param);
    MergeSort(right, nmemb - mid, size, cmp, param);

    Merge(base, left, right, cmp, size, mid, (nmemb - mid), param);   

    free(left);
    left = NULL;

    free(right);
    right = NULL;
}

static void Merge(void *base, void *left, void *right, sort_cmp_func_t cmp, 
           size_t size, size_t nmemb_left, size_t nmemb_right, void *param)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    while (i < (nmemb_left * size) && j < (nmemb_right * size))
    {
        if(cmp((char *)left + i,(char *)right + j,param) > 0)
        {
            memcpy((char *)base + k, (char *)right + j, size);
            j += size;
        }
        else
        {
            memcpy((char *)base + k, (char *)left + i, size);
            i += size;    
        }
        
        k += size;
    }

    while (i < nmemb_left * size)
    {
        memcpy((char *)base + k, (char *)left + i, size);
        i += size; 
        k += size;
    }

    while (j < nmemb_right * size)
    {
        memcpy((char *)base + k, (char *)right + j, size);

        j += size;
        k += size;
    }  

}