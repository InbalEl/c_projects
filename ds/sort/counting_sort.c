/********************************
* Task name: counting sort		*
* Develoer: Inbal Elmalech		*
* Reviewer: Ori Komemi			*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* size_t, exit status */
#include <assert.h> /* assert */
#include <stdio.h> /* printf, puts */
#include "sort.h" /* sort */
/*************************    General Functions    ***************************/

int CountingSort(const int *arr, int *dest, size_t nmemb, int min, int max)
{
	size_t i = 0;
	size_t range = 0;
	int *histogram = NULL;

	assert(arr);
	assert(dest);

	range = max - min + 1;

	/* Allocating memory Jawaz Safarfor count_arr according to range */
	histogram = (int *) calloc(range, sizeof(int));

	if (NULL == histogram)
	{
		return (EXIT_FAILURE);
	}

	/* Filling up histogram with count of value instances
	(normalized to 0 using min) */
	for (i = 0; i < nmemb; ++i)
	{
		++histogram[arr[i] - min];
	}

	/* Adding up each 2 elements in count_arr */
	for (i = 1; i < range; ++i)
	{
		histogram[i] += histogram[i - 1];
	}

	/* Filling up dest with values */
	for (i = (nmemb - 1); i > 0 ; --i)
	{
		--histogram[arr[i] - min];
		dest[histogram[arr[i] - min]] = arr[i];
	}

	--histogram[arr[i] - min];
	dest[histogram[arr[i] - min]] = arr[i];

	free(histogram);
	histogram = NULL;

	return (EXIT_SUCCESS);
}

/*****************************************************************************/




