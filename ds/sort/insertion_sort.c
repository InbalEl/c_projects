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

void InsertionSort(void *base, size_t nmemb, size_t size,
				   int (*compar)(const void *, const void *, void *),
				   void *arg)
{
	char *insert_here = (char *)base;
	char *unsorted = (char *)base + size;
	char *elem = unsorted;

	assert(base);
	assert(compar);

	while (unsorted != (((char *)base) + (nmemb * size)))
	{
		while (0 > compar((void *)elem, (void *)insert_here, arg) &&
			   !((size_t)insert_here < (size_t)base))
		{
			SwapElements((void *)elem, (void *)insert_here, size);
			elem = elem - size;
			insert_here = insert_here - size;
		}

		insert_here = unsorted;
		unsorted = unsorted + size;
		elem = unsorted;
	}
}

/*****************************************************************************/




