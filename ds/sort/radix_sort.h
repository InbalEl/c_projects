

#ifndef ILRD_RADIX_SORT_H
#define ILRD_RADIX_SORT_H

#include <stddef.h>	/* size_t */
#include <stdint.h> /* uint */



int CountingSort2(unsigned int *arr, size_t nmemb, unsigned int min, 
														unsigned int max);

int RadixSort(unsigned int *arr, size_t nmemb, unsigned int chunk_size);



#endif /* ILRD_RADIX_SORT_H */ 
