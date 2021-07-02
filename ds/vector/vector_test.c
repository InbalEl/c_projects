/********************************
* File name: ws5.c				*
* Develoer: Inbal Elmalech		*
* Reviewer: 					*
* Review date: May, 2020		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts*/
#include "vector.h" /* stack_t */

struct vector
{
	size_t initial_capacity;
	size_t current_capacity;
	size_t size;
	void **data;
};

/*****************************   DECLARATIONS   ******************************/

static void PrintVector(vector_t *vector);

vector_t *Test_VectorCreate(size_t capacity);
void Test_VectorDestroy(vector_t *vector);
void Test_VectorGetElement(const vector_t *vector, size_t index);
void Test_VectorSetElement(vector_t *vector, size_t index, void *element);
void Test_VectorPushBack(vector_t *vector, void *element);
void Test_VectorPopBack(vector_t *vector);
void Test_VectorSize(vector_t *vector);
void Test_VectorCapacity(vector_t *vector);
void Test_VectorReserve(vector_t *vector, size_t new_capacity);
void Test_VectorShrinkToFit(vector_t *vector);


/* TODO: ADD STATIC FUNCTION TO PRINT THE VECTOR AT ANY GIVEN MOMENT */

/********************************  MAIN  *************************************/

int main(void)
{
	size_t i = 0;
	int meaning = 42;
	float phi = 1.618;
	int year = 2020;
	vector_t *vector1 = NULL;
	char str[] = "don't panic";
	char str2[] = "I'm a doctor";

	printf("\n--------------Test_StackCreate------------------\n");

	printf("\n-------vector1-------\n");

	vector1 = Test_VectorCreate(4);

	printf("Address of vector1 is: %p\n", (void *)vector1);
	printf("initial_capacity is: %lu\n", vector1->initial_capacity);
	printf("size is: %lu\n", vector1->size);
	printf("current_capacity; is: %lu\n", vector1->current_capacity);
	printf("Address of data is: %p\n", (void *)vector1->data);

	PrintVector(vector1);

	printf("\n--------------Test_VectorPushBack------------------\n");

	Test_VectorPushBack(vector1, (void *) &meaning);
	Test_VectorPushBack(vector1, (void *) &phi);
	Test_VectorPushBack(vector1, (void *) &phi);
	Test_VectorPushBack(vector1, (void *) &year);
	Test_VectorPushBack(vector1, (void *) str);
	Test_VectorPushBack(vector1, (void *) str2);

	PrintVector(vector1);

	printf("\n--------------Test_VectorGetElement------------------\n");

	Test_VectorGetElement(vector1, 5);


	printf("\n--------------Test_VectorSetElement------------------\n");

	Test_VectorSetElement(vector1, 3, &meaning);

	PrintVector(vector1);

	printf("\n--------------Test_VectorPopBack------------------\n");

	Test_VectorPopBack(vector1);
	Test_VectorPopBack(vector1);
	Test_VectorPopBack(vector1);

	PrintVector(vector1);

	printf("\n--------------Test_VectorReserve------------------\n");

	Test_VectorReserve(vector1, 20);

	for(i = 0; i < 12; ++i)
	{
		Test_VectorPushBack(vector1, (void *) &meaning);
	}

	printf("\n--------------Test_VectorShrinkToFit------------------\n");

	Test_VectorShrinkToFit(vector1);

	printf("\n--------------Test_VectorDestroy------------------\n");

	Test_VectorDestroy(vector1);

	return (0);
}

/***************************** END OF MAIN ***********************************/

/***********************    General Functions    *****************************/

static void PrintVector(vector_t *vector)
{
	size_t i = 0;
	printf("\nvector is:\n");

	for (i = 0; i <(vector->size); ++i)
	{
		printf("[%lu]: %p\n", i, *((vector->data) + i));
	}
}

/*---------------------------------------------------------------------------*/

vector_t *Test_VectorCreate(size_t capacity)
{
	printf("Creating new vector!\n");
	return (VectorCreate(capacity));
}

/*---------------------------------------------------------------------------*/

void Test_VectorDestroy(vector_t *vector)
{
	printf("Vector at %p will be exterminated\n", (void *) vector);
	VectorDestroy(vector);
}

/*---------------------------------------------------------------------------*/

void Test_VectorGetElement(const vector_t *vector, size_t index)
{
	printf("Element at %lu is %p\n", index, VectorGetElement(vector, index));
}

/*---------------------------------------------------------------------------*/

void Test_VectorSetElement(vector_t *vector, size_t index, void *element)
{
	Test_VectorGetElement(vector, index);
	puts("Updating element\n");
	VectorSetElement(vector, index, element);
	Test_VectorGetElement(vector, index);
}

/*---------------------------------------------------------------------------*/

void Test_VectorPushBack(vector_t *vector, void *element)
{
	VectorPushBack(vector, element);
}

/*---------------------------------------------------------------------------*/

void Test_VectorPopBack(vector_t *vector)
{
	VectorPopBack(vector);
}

/*---------------------------------------------------------------------------*/

void Test_VectorSize(vector_t *vector)
{
	printf("Vector size is: %lu\n", VectorSize(vector));
}

/*---------------------------------------------------------------------------*/

void Test_VectorCapacity(vector_t *vector)
{
	printf("Vector current capacity is: %lu\n", VectorCapacity(vector));
}

/*---------------------------------------------------------------------------*/

void Test_VectorReserve(vector_t *vector, size_t new_capacity)
{
	printf("Increasing vector capacity to %lu\nstatus: %d\n",
		   new_capacity, VectorReserve(vector, new_capacity));

	Test_VectorCapacity(vector);
}

/*---------------------------------------------------------------------------*/

void Test_VectorShrinkToFit(vector_t *vector)
{
	printf("Shrinking vector capacity\n");
	VectorShrinkToFit(vector);

	Test_VectorCapacity(vector);
}

/*****************************************************************************/


