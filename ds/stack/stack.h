#ifndef STACK_H
#define STACK_H

#include <stddef.h> /* size_t */

/*****************************************************************************/
typedef struct Stack Stack_t;
/*---------------------------------------------------------------------------*/

Stack_t* StackCreate(size_t capacity);
void StackDestroy(Stack_t *stack);
int StackPush(Stack_t *stack, void *element);
void StackPop(Stack_t *stack);
void *StackPeek(Stack_t *stack);
size_t StackSize(Stack_t *stack);
int StackIsEmpty(Stack_t *stack);

/*---------------------------------------------------------------------------*/
#endif /* STACK_H */
/*****************************************************************************/