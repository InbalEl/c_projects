/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include "stack.h" /* Stack_t */

/*****************************   DECLARATIONS   ******************************/
Stack_t *Test_StackCreate(size_t capacity);
void Test_StackDestroy(Stack_t *stack);
void Test_StackPush(Stack_t *stack, void *element);
void Test_StackPop(Stack_t *stack);
void *Test_StackPeek(Stack_t *stack);
void Test_StackSize(Stack_t *stack);
void Test_StackIsEmpty(Stack_t *stack);

/********************************  MAIN  *************************************/

int main(void)
{
	int meaning = 42;
	float phi = 1.618;
	Stack_t *new_stack = NULL;
	char str[] = "don't panic";

	printf("\n--------------Test_StackCreate------------------\n");

	new_stack = Test_StackCreate(6);
	printf("Address of new stack is: %p\n", (void *)new_stack);

	printf("\n--------------Test_StackIsEmpty------------------\n");

	Test_StackIsEmpty(new_stack);

	printf("\n--------------Test_StackPush------------------\n");

	printf("Element is: %d\n", (meaning));
	printf("Element address is: %p\n", (void *) &meaning);
	Test_StackPush(new_stack, (void *) &meaning);

	printf("\n--------------Test_StackPeek------------------\n");

	printf("value is %p\n", Test_StackPeek(new_stack));

	printf("\n--------------Test_StackPush------------------\n");

	printf("Element is: %f\n", phi);
	printf("Element address is: %p\n", (void *) &phi);
	Test_StackPush(new_stack, (void *) &phi);

	printf("\n--------------Test_StackPeek------------------\n");

	printf("value is %p\n", Test_StackPeek(new_stack));

	printf("\n--------------Test_StackPush------------------\n");

	printf("Element address is: %p\n", (void *) str);
	Test_StackPush(new_stack, (void *) str);

	printf("\n--------------Test_StackPeek------------------\n");

	printf("value is %p\n", Test_StackPeek(new_stack));

	printf("\n--------------Test_StackPop------------------\n");

	Test_StackPop(new_stack);

	printf("\n--------------Test_StackPeek------------------\n");

	printf("value is %p\n", Test_StackPeek(new_stack));

	printf("\n--------------Test_StackSize------------------\n");

	Test_StackSize(new_stack);

	printf("\n--------------Test_StackIsEmpty------------------\n");

	Test_StackIsEmpty(new_stack);

	printf("\n--------------Test_StackDestroy------------------\n");

	Test_StackDestroy(new_stack);

	return (0);
}

/***********************    General Functions    *****************************/

Stack_t* Test_StackCreate(size_t capacity)
{
	printf("Creating new stack!\n");
	return (StackCreate(capacity));
}

/*---------------------------------------------------------------------------*/

void Test_StackDestroy(Stack_t *stack)
{
	printf("Stack at %p will be exterminated\n\n", (void *) stack);
	StackDestroy(stack);
}

/*---------------------------------------------------------------------------*/

void Test_StackPush(Stack_t *stack, void *element)
{
	printf("Pushing element %p into stack\n", element);
	StackPush(stack, element);
}

/*---------------------------------------------------------------------------*/

void Test_StackPop(Stack_t *stack)
{
	StackPop(stack);
}

/*---------------------------------------------------------------------------*/

void *Test_StackPeek(Stack_t *stack)
{
	return (StackPeek(stack));
}

/*---------------------------------------------------------------------------*/

void Test_StackSize(Stack_t *stack)
{
	printf("The stack size is: %lu\n", StackSize(stack));
}

/*---------------------------------------------------------------------------*/

void Test_StackIsEmpty(Stack_t *stack)
{
	printf("IsEmpty: %d\n", StackIsEmpty(stack));
}

/*****************************************************************************/