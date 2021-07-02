/*
*	File:			heap.c
*	Description:	Implementing API functions for Heap
*	Developer:		Noa Pilosof
*	Reviewer:	 	
*	Date:			TBD
*/

#include <stdlib.h>		/* malloc, free, success, fail */
#include <assert.h>		/* assert */

#include "heap.h"
#include "heapify.h"
#include "vector.h"

#define VECTOR_CAPACITY 1
#define FIRST_IDX 0

/************************    Custom Structures    ****************************/
struct heap
{
    vector_t *heap_arr;
    heap_cmp_func_t cmp;
    void *param;
};

#ifdef MY_DEBUG

#include <stdio.h>		/* printf */

typedef enum print_data
{
    HEAP = 0,
    PQ = 1
}print_data_t;

typedef struct to_do
{
    char *item;
    int priority;
}todo_t;

typedef struct pet_queue
{
	char name[20];
	size_t priority;
}pet_t;


/*******************    Utiliy functions declarations    *********************/

/* print functions */
void HeapPrintData(heap_t *heap, print_data_t data_type);
void HeapPrintPriority(heap_t *heap, print_data_t data_type);

#endif  /* MY_DEBUG */

/***************************   API Functions    ******************************/

heap_t* HeapCreate(heap_cmp_func_t cmp, void *param)
{
    heap_t *new_heap = NULL;
    vector_t *arr = NULL;
    
    /* create heap */
    new_heap = (heap_t *)malloc (sizeof(heap_t));
    if (!new_heap)
    {
        return (NULL);
    }

    /* create vector */
    arr = VectorCreate(VECTOR_CAPACITY);
    if (!arr)
    {
        free(new_heap);
        new_heap = NULL;

        return (NULL);
    }

    /* initialize heap */
    new_heap->heap_arr = arr;
    new_heap->cmp = cmp;
    new_heap->param = param;

    return (new_heap);

}

/*---------------------------------------------------------------------------*/

void HeapDestroy(heap_t *heap)
{
    /* destroy vector */
    VectorDestroy(heap->heap_arr);
    heap->heap_arr = NULL;

    /* free heap */
    free(heap);
    heap = NULL;
}

/*---------------------------------------------------------------------------*/

int HeapPush(heap_t *heap, void *data)
{
    int push_status = EXIT_SUCCESS;
    void **heap_arr = NULL;
    size_t heap_size = 0;

    assert(heap);

    /* insert element to back of heap */
    push_status = VectorPushBack(heap->heap_arr, data);
    if (push_status)
    {
        return (EXIT_FAILURE);
    }

    heap_arr = VectorGetArr(heap->heap_arr);
    heap_size = HeapSize(heap);

    /* heapify */
    HeapifyUp(heap_arr, heap_size, heap_size-1, sizeof(void *), heap->cmp, 
              heap->param);
    

    return (EXIT_SUCCESS);
}

/*---------------------------------------------------------------------------*/

void *HeapPeek(heap_t *heap)
{
    assert(heap);

    return (VectorGetElement(heap->heap_arr, FIRST_IDX));
}

/*---------------------------------------------------------------------------*/

void *HeapPop(heap_t *heap)
{
    void *data_popped = NULL;
    void *last_elem = NULL;
    size_t last_index = 0;
    void **heap_array = NULL;

    assert(heap);

    last_index = HeapSize(heap) - 1;

    /* set last element in the beginnig of heap */
    data_popped = VectorGetElement(heap->heap_arr, FIRST_IDX);
    last_elem = VectorGetElement(heap->heap_arr, last_index);
    VectorSetElement(heap->heap_arr, FIRST_IDX, last_elem);

    /* popback from vector */
    VectorPopBack(heap->heap_arr);
    --last_index;

    /* heapify */
    heap_array = VectorGetArr(heap->heap_arr);
    
    HeapifyDown(heap_array, HeapSize(heap), FIRST_IDX, sizeof(void *), 
                heap->cmp, heap->param);

    return(data_popped);
}

/*---------------------------------------------------------------------------*/

void *HeapRemove(heap_t *heap, void *data, heap_is_match_func_t is_match, void *param)
{
    void *data_removed = NULL;
    void *last_data = NULL;
    void **heap_array = NULL;
    size_t heap_size = 0;
    size_t last_index = 0;
    size_t i = 0;
    
    heap_size = HeapSize(heap);
    
	/*printf("heap size %lu\n", heap_size);*/
    while (i < heap_size && !is_match(VectorGetElement(heap->heap_arr, i), 
                             data, param))
    {
        ++i;
    }

    if (i < heap_size)
    {
        data_removed = VectorGetElement(heap->heap_arr, i);
        last_index = heap_size - 1;
        last_data = VectorGetElement(heap->heap_arr, last_index);

        /* put last index instead of index removed */
        VectorSetElement(heap->heap_arr, i, last_data);
        VectorPopBack(heap->heap_arr);
        --last_index;

        /* heapify */
        heap_array = VectorGetArr(heap->heap_arr);
        HeapifyDown(heap_array, heap_size, i, sizeof(void *), heap->cmp,
                    heap->param);
    }

    return (data_removed);
}

/*---------------------------------------------------------------------------*/

size_t HeapSize(heap_t *heap)
{
    assert(heap);

    return(VectorSize(heap->heap_arr));
}

/*---------------------------------------------------------------------------*/

int HeapIsEmpty(heap_t *heap)
{
    assert(heap);

    return(0 == HeapSize(heap));
}

/*************************    Utility Functions    ***************************/


/**************************    Print Functions    ****************************/
#ifdef MY_DEBUG
void HeapPrintData(heap_t *heap, print_data_t data_type)
{
    todo_t item = {NULL, 0};
    pet_t pet = {{0}, 0};
    size_t i = 0;
    size_t heap_size = 0;

    heap_size = HeapSize(heap);

    switch (data_type)
    {
        case (HEAP):
        {
            for (i = 0; i < heap_size; ++i)
            {
                item = *(todo_t *)VectorGetElement(heap->heap_arr, i);
                printf("%d  %s\n", item.priority, item.item);
            }
            
            break;
        }

        case (PQ):
        {
            for (i = 0; i < heap_size; ++i)
            {
                pet = *(pet_t *)VectorGetElement(heap->heap_arr, i);
                printf("%d  %s\n", pet.priority, pet.name);
            }
            
            break;
        }
        
    }
}

/*---------------------------------------------------------------------------*/

void HeapPrintPriority(heap_t *heap, print_data_t data_type)
{
    todo_t item = {NULL, 0};
    pet_t pet = {{0}, 0};
    size_t i = 0;
    size_t heap_size = 0;

    heap_size = HeapSize(heap);

    switch (data_type)
    {
        case (HEAP):
        {
            for (i = 0; i < heap_size; ++i)
            {
                item = *(todo_t *)VectorGetElement(heap->heap_arr, i);
                printf("%d  ", item.priority);
            }
            
            break;
        }

        case (PQ):
        {
            for (i = 0; i < heap_size; ++i)
            {
                pet = *(pet_t *)VectorGetElement(heap->heap_arr, i);
                printf("%d  ", pet.priority);
            }
            
            break;
        }
        
    }

    printf("\n");
}
#endif  /* MY_DEBUG */
