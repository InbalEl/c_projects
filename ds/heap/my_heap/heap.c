/********************************
* Task name: heap   			*
* Develoer: Inbal Elmalech		*
* Reviewer: ????        		*
* Date: Aug 13, 2020			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "vector.h" /* vector */
#include "heap.h" /* heap */
#include "heapify.h" /* heap */
#define INIT_SIZE 100
/******************************   STRUCTURES   *******************************/

struct heap
{
    vector_t *vector;
    void *param;
    heap_cmp_func_t cmp;
};

/****************************   DECLARATIONS   *******************************/

static size_t HeapFind(heap_t *heap, void *data,
                       heap_is_match_func_t is_match, void *param);

/*************************    General Functions    ***************************/

heap_t* HeapCreate(heap_cmp_func_t cmp, void *param)
{
    heap_t *heap = NULL;
    vector_t *vector = NULL;

    assert(cmp);

    if (NULL == (heap = (heap_t *)malloc(sizeof(heap_t))))
    {
        return (NULL);
    }

    if (NULL == (vector = VectorCreate(INIT_SIZE)))
    {
        free(heap);
        heap = NULL;
        return (NULL);
    }

    heap->vector = vector;
    heap->cmp = cmp;
    heap->param = param;

    return (heap);
}

/*---------------------------------------------------------------------------*/

void HeapDestroy(heap_t *heap)
{
    assert(heap);
    VectorDestroy(heap->vector);
    heap->vector = NULL;
    free(heap);
    heap = NULL;
}

/*---------------------------------------------------------------------------*/

int HeapPush(heap_t *heap, void *data)
{
    size_t heap_size = 0;
    int push_back_res = 0;

    assert(heap);

    push_back_res = VectorPushBack(heap->vector, data);
    heap_size = HeapSize(heap);
    
    if (1 == push_back_res)
    {
        return push_back_res;
    }

    /* TODO: hard-coded element size */
    HeapifyUp(VectorGetArr(heap->vector),heap_size,
              heap_size - 1, sizeof(void *), heap->cmp, heap->param);

    return (push_back_res);
}

/*---------------------------------------------------------------------------*/

void *HeapPeek(heap_t *heap)
{
    assert(heap);

    return (VectorGetElement(heap->vector, 0));
}

/*---------------------------------------------------------------------------*/

void *HeapPop(heap_t *heap)
{
    void *popped_val = NULL;
    void *last_elem = NULL;

    assert(heap);

    popped_val = HeapPeek(heap);

    last_elem = VectorGetElement(heap->vector, HeapSize(heap) - 1);
    VectorSetElement(heap->vector, 0, last_elem);
    VectorPopBack(heap->vector);

    /* TODO: hard-coded element size */
    HeapifyDown(VectorGetArr(heap->vector), HeapSize(heap),
                0, sizeof(void *), heap->cmp, heap->param);

    return (popped_val);
}

/*---------------------------------------------------------------------------*/

void *HeapRemove(heap_t *heap, void *data,
                 heap_is_match_func_t is_match, void *param)
{
    size_t removal_i = 0;
    void *removed_val = NULL;
    void *last_elem = NULL;

    assert(heap);

    if (HeapSize(heap) == (removal_i = HeapFind(heap, data, is_match, param)))
    {
        return (NULL);
    }
   
    removed_val = VectorGetElement(heap->vector, removal_i); 
    last_elem = VectorGetElement(heap->vector, VectorSize(heap->vector) - 1);
    VectorSetElement(heap->vector, removal_i, last_elem);
    VectorPopBack(heap->vector);

    /* TODO: hard-coded element size */
    HeapifyDown(VectorGetArr(heap->vector), HeapSize(heap), removal_i,
                             sizeof(void *), heap->cmp, heap->param);

    return (removed_val);
}

/*---------------------------------------------------------------------------*/

size_t HeapSize(heap_t *heap)
{
    assert(heap);

    return (VectorSize(heap->vector));
}

/*---------------------------------------------------------------------------*/

int HeapIsEmpty(heap_t *heap)
{
    assert(heap);

    return (0 == HeapSize(heap));
}

/***************************** Utilty Functions ******************************/

static size_t HeapFind(heap_t *heap, void *data,
                       heap_is_match_func_t is_match, void *param)
{
    size_t curr_index = 0;
    size_t vector_size = VectorSize(heap->vector);

    while ((curr_index < vector_size) &&
           (!(is_match(data,
                       VectorGetElement(heap->vector, curr_index),
                       param))))
    {
        ++curr_index;
    }

    return (curr_index);
}
/*---------------------------------------------------------------------------*/

/*****************************************************************************/