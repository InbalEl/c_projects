#ifndef UTIS_H
#define UTILS_H

#include <stddef.h>	/* size_t */

#endif /* UTILS_H */ 

void SwapElements(void *data1,void *data2, size_t nof_bytes);

unsigned int FindMinUint(unsigned int *arr, size_t arr_size);

int FindMinInt(int *arr, size_t arr_size);

int FindMaxInt(int *arr, size_t arr_size);
