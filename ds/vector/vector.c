/********************************
* File name: ws5.c				*
* Develoer: Inbal Elmalech		*
* Reviewer: Rita Lampart		*
* Review date: May, 2020		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* malloc, free */
#include "vector.h" /* vector_t */

#define INDEX_IN_RANGE(index) ((index) < (vector->size))
#define ELEM_SIZE (sizeof(void *))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define GROWTH_F 2
#define SHRINK_F 4

/***************************   ENUMS & STRUCTURES  ***************************/

struct vector
{
	size_t initial_capacity;
	size_t current_capacity;
	size_t size;
	void **data;
};

/********************    Utility Functions Declaration   *********************/

static void **VectorReallocateData(vector_t *vector, size_t size);

/*************************    General Functions    ***************************/

vector_t *VectorCreate(size_t capacity)
{
	vector_t *vector = (vector_t *) malloc((sizeof(vector_t)));

	if (NULL == vector)
	{
		return (NULL);
	}

	vector->data = (void **) malloc((capacity) * (ELEM_SIZE));

	if (NULL == vector->data)
	{
		free(vector);
		vector = NULL;

		return (NULL);
	}

	vector->initial_capacity = capacity;
	vector->current_capacity = capacity;
	vector->size = 0;

	return (vector);
}

/*---------------------------------------------------------------------------*/

void VectorDestroy(vector_t *vector)
{
	free(vector->data);
	vector->data = NULL;

	free(vector);
	vector = NULL;
}

/*---------------------------------------------------------------------------*/

void *VectorGetElement(const vector_t *vector, size_t index)
{
	assert (INDEX_IN_RANGE(index));

	return (*((vector->data) + index));
}

/*---------------------------------------------------------------------------*/

void VectorSetElement(vector_t *vector, size_t index, void *element)
{
	assert (INDEX_IN_RANGE(index));

	*(vector->data + index) = element;
}

/*---------------------------------------------------------------------------*/

int VectorPushBack(vector_t *vector, void *element)
{
	void **tmp_data = NULL;

	if (vector->size == vector->current_capacity)
	{

		tmp_data = VectorReallocateData(vector,
				   (ELEM_SIZE * (vector->current_capacity * GROWTH_F)));

		if (NULL == tmp_data)
		{
			return (1);
		}
	
		vector->data = tmp_data;
	}
	
	*((vector->data) + (vector->size)) = element;
	++(vector->size);

	return (0);
}

/*---------------------------------------------------------------------------*/

void VectorPopBack(vector_t *vector)
{
	void **tmp_data = NULL;

	--(vector->size);

	if (vector->size <= (vector->current_capacity / SHRINK_F))
	{
		tmp_data = VectorReallocateData(vector, ELEM_SIZE *
				   MAX((vector->current_capacity / GROWTH_F),
						vector->initial_capacity));

		if (NULL == tmp_data)
		{
			return;
		}

		vector->data = tmp_data;
	}
}

/*---------------------------------------------------------------------------*/

size_t VectorSize(const vector_t *vector)
{
	return (vector->size);
}

/*---------------------------------------------------------------------------*/

size_t VectorCapacity(const vector_t *vector)
{
	return (vector->current_capacity);
}

/*---------------------------------------------------------------------------*/

int VectorReserve(vector_t *vector, size_t new_capacity)
{
	void **tmp_data = NULL;
	
	if (!(vector->current_capacity < new_capacity))
	{
		return (1);
	}

	tmp_data = VectorReallocateData(vector, (ELEM_SIZE * new_capacity));

	if (NULL == tmp_data)
	{
		return (1);
	}
	
	vector->data = tmp_data;

	return (0);
}

/*---------------------------------------------------------------------------*/

void VectorShrinkToFit(vector_t *vector)
{
	void **tmp_data = VectorReallocateData(vector,
										  (ELEM_SIZE *
					  MAX((vector->size), vector->initial_capacity)));
	
	if (NULL == tmp_data)
	{
		return;
	}

	vector->data = tmp_data;
}

/*---------------------------------------------------------------------------*/

void *VectorGetArr(vector_t *vector)
{
	assert(vector);

	return ((void *)vector->data);
}

/*************************    Utility Functions    ***************************/

static void **VectorReallocateData(vector_t *vector, size_t new_cap_size)
{
	void **tmp_data = (void **) realloc(vector->data, new_cap_size);

	if (NULL == tmp_data)
	{
		return (NULL);
	}

	vector->current_capacity = new_cap_size / ELEM_SIZE;

	return (tmp_data);
}

/*****************************************************************************/




