/********************************
* Task name: sort				*
* Develoer: Inbal Elmalech		*
* Reviewer: Guy Barazany		*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include "sort.h" /* sort */
#include "utils.h" /* swap */
/*************************    General Functions    ***************************/

void BubbleSort(void *base, size_t nmemb, size_t size,
				int (*compar)(const void *, const void *, void *),
				void *arg)
{
	char *curr_elem = (char *)base;

	assert(base);
	assert(compar);

	for (; nmemb; --nmemb, curr_elem = (char *)base)
	{
		for (;
			 ((curr_elem - ((char *)base)) / size) < nmemb - 1;
			 curr_elem = curr_elem + size)
		{
			if (0 < (compar((void *)curr_elem,
							(void *)(curr_elem + size),
							arg)))
			{
				SwapElements((void *)curr_elem,
							 (void *)(curr_elem + size),
							 size);
			}
		}
	}
}

/*****************************************************************************/




