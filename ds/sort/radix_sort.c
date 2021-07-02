/********************************
* Task name: counting sort		*
* Develoer: Inbal Elmalech		*
* Reviewer: Ori Komemi			*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* allocs, frees, size_t, exit status */
#include <assert.h> /* assert */
#include <math.h>     /* pow */
#include <string.h> /* memset, memcpy */
#include "radix_sort.h" /* sort */
/*************************    General Functions    ***************************/

void CountingSortHelper(unsigned int *arr, unsigned int *dest, size_t nmemb,
				   unsigned int chunck_size, unsigned int chunk_number,
				   unsigned int *histogram, size_t histo_size);

/*---------------------------------------------------------------------------*/

int RadixSort(unsigned int *arr, size_t nmemb, unsigned int chunk_size)
{
	size_t nof_iterations = (sizeof(unsigned int) % chunk_size == 0) ?
							(sizeof(unsigned int) / chunk_size) :
							(sizeof(unsigned int) / chunk_size + 1);

	size_t i = 0;
	size_t starting_bit = 0;
	unsigned int *dest = NULL;
	unsigned int *histogram = NULL;
	size_t histogram_size = ~(~0 << chunk_size);

	assert(arr);

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

	for (i = 0 ; i < nof_iterations; ++i, starting_bit += chunk_size)
	{
		if (0 == (nof_iterations % 2))
		{
			CountingSortHelper(arr, dest, nmemb, chunk_size, i, histogram,
							   histogram_size);
		}

		else
		{
			CountingSortHelper(dest, arr, nmemb, chunk_size, i, histogram,
							   histogram_size);
		}

		histogram = memset(histogram, 0, histogram_size * sizeof(unsigned int));
		--nof_iterations;
	}

	if(0 != (i % 2))
	{
		memcpy(arr, dest, nmemb * sizeof(unsigned int));
	}

	free(dest);
	dest = NULL;

	free(histogram);
	histogram = NULL;

	return (EXIT_SUCCESS);
}

/*****************************************************************************/




