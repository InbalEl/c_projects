/********************************
* Task name: counting sort		*
* Develoer: Inbal Elmalech		*
* Reviewer: Ori Komemi			*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* size_t, exit status */
#include <assert.h> /* assert */
#include <stdio.h> /* printf, puts */
#include <string.h> /* memcpy */
#include "utils.h" /* utils */

#define CHAR_BIT 8
/*************************    General Functions    ***************************/

void CountingSortHelper(unsigned int *arr, unsigned int *dest, size_t nmemb,
				   unsigned int chunck_size, unsigned int chunk_number,
				   unsigned int *histogram, size_t histo_size);

/*---------------------------------------------------------------------------*/

int CountingSort2(unsigned int *arr, size_t nmemb,
				 unsigned int min, unsigned int max)
{
	unsigned int *dest = NULL;
	unsigned int *histogram = NULL;

	size_t histogram_size = max - min + 1;

	dest = (unsigned int *) malloc(sizeof(unsigned int) * nmemb);

	if (NULL == dest)
	{
		return (EXIT_FAILURE);
	}

	histogram = (unsigned int *) calloc(histogram_size, sizeof(unsigned int));

	if (NULL == histogram)
	{
		free(dest);
		dest = NULL;

		return (EXIT_FAILURE);
	}

	CountingSortHelper(arr, dest, nmemb, (sizeof(unsigned int) * CHAR_BIT),
					   1, histogram, histogram_size);

	arr = memcpy(arr, dest, nmemb * sizeof(unsigned int));

	free(dest);
	dest = NULL;

	free(histogram);
	histogram = NULL;

	return (EXIT_SUCCESS);
}

/*---------------------------------------------------------------------------*/

void CountingSortHelper(unsigned int *arr, unsigned int *dest, size_t nmemb,
				   unsigned int chunk_size, unsigned int chunk_number,
				   unsigned int *histogram, size_t histo_size)
{
	size_t i = 0;
	unsigned int min = 0;
	unsigned int rotate = 0;
	unsigned int mask = ~(~0 << chunk_size);

	assert(arr);
	assert(dest);
	assert(histogram);

	min = FindMinUint(arr, nmemb);
	rotate = chunk_number * chunk_size;
	mask <<= rotate;

	/* Filling up histogram with count of value instances
	(normalized to 0 using min) */
	for (i = 0; i < nmemb; ++i)
	{
		++histogram[(((arr[i] - min) & mask) >> rotate)];
	}

	/* Adding up each 2 elements in count_arr */
	for (i = 1; i < histo_size; ++i)
	{
		histogram[i] += histogram[i - 1];
	}

	/* Filling up dest with values */
	for (i = (nmemb - 1); i > 0 ; --i)
	{
		--histogram[(((arr[i] - min) & mask) >> rotate)];
		dest[histogram[(((arr[i] - min) & mask) >> rotate)]] = arr[i];
	}

	--histogram[(((arr[i] - min) & mask) >> rotate)];
	dest[histogram[(((arr[i] - min) & mask) >> rotate)]] = arr[i];

	free(histogram);
	histogram = NULL;

	return;
}

/*****************************************************************************/




