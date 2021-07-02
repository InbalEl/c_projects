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

void SelectionSort(void *base, size_t nmemb, size_t size,
				   int (*compar)(const void *, const void *, void *),
				   void *arg)
{
	char *insert_here = (char *)base;
	char *lowest_val = insert_here;
	char *elem = lowest_val;

	assert(base);
	assert(compar);

	while (insert_here != (((char *)base) + ((nmemb - 1) * size)))
	{
		while (elem != (((char *)base) + (nmemb * size)))
		{
			if (0 > compar((void *)elem, (void *)lowest_val, arg))
			{
				lowest_val = elem;
			}

			elem += size;
		}

		if (insert_here != lowest_val)
		{
			SwapElements((void *)insert_here, (void *)lowest_val, size);
		}

		insert_here += size;
		lowest_val = insert_here;
		elem = lowest_val;
	}
}

/*****************************************************************************/




