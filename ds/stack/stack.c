/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */

#include "stack.h" /* stack_t */

#define ELEM_SIZE sizeof(size_t *)

/************************   ENUMS & STRUCTURES  ******************************/
struct Stack
{
	size_t capacity;
	void **top;
	void **head;
};

/*************************    General Functions    ***************************/

static void* Alloc(size_t alloc_size)
{
	return (malloc(alloc_size));
}

/*---------------------------------------------------------------------------*/

Stack_t* StackCreate(size_t capacity)
{
	Stack_t *new_stack = ((Stack_t *) Alloc(sizeof(Stack_t)));
	if (!new_stack)
	{
		return (new_stack);
	}

	new_stack->head = Alloc(sizeof(ELEM_SIZE) * (capacity));
	if (!new_stack->head)
	{
		free(new_stack);
		new_stack = NULL;

		return (NULL);
	}

	new_stack->top = new_stack->head;
	new_stack->capacity = capacity;

	return (new_stack);
}

/*---------------------------------------------------------------------------*/

void StackDestroy(Stack_t *stack)
{
	free(stack->head);
	stack->head = NULL;
	free(stack);
	stack = NULL;
}

/*---------------------------------------------------------------------------*/

int StackPush(Stack_t *stack, void *element)
{
	if (StackSize(stack) < stack->capacity)
	{
		*(stack->top) = element;
		stack->top = (void *) (((char *)stack->top) + ELEM_SIZE);

		return (0);
	}

	return (1);
}

/*---------------------------------------------------------------------------*/

void StackPop(Stack_t *stack)
{
	if (!StackIsEmpty(stack))
	{
		stack->top = (void *) (((char *)stack->top) - ELEM_SIZE);
	}
}

/*---------------------------------------------------------------------------*/

void *StackPeek(Stack_t *stack)
{
	if (!StackIsEmpty(stack))
	{
		return ((void *) *(((char *)stack->top) - ELEM_SIZE));
	}

	return (NULL);
}

/*---------------------------------------------------------------------------*/

size_t StackSize(Stack_t *stack)
{
	return (((char *)(stack->top) - (char *)(stack->head)) / ELEM_SIZE);
}

/*---------------------------------------------------------------------------*/

int StackIsEmpty(Stack_t *stack)
{
	return (StackSize(stack) == 0);
}

/*****************************************************************************/