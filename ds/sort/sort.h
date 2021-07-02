#ifndef ILRD_SORT_H
#define ILRD_SORT_H

#include <stddef.h>	/* size_t */

/*int Cmp(const void *data1, const void *data2, void *param);*/

typedef int (*sort_cmp_func_t)(const void *data1,
							   const void *data2,
							   void *param);

/* stable sort */
void BubbleSort(void *base, size_t nmemb, size_t size,
                  sort_cmp_func_t cmp,
                  void *arg);

void SelectionSort(void *base, size_t nmemb, size_t size,
                  sort_cmp_func_t cmp,
                  void *arg);

/* stable sort */
void InsertionSort(void *base, size_t nmemb, size_t size,
                  sort_cmp_func_t cmp,
                  void *arg);

/* stable sort */
int CountingSort(const int *arr, int *dest, size_t nmemb, int min, int max);

/* iterative binary search */
/* recursive binary search */

/* recursive merge */
void MergeSort(void *base, size_t nmemb, size_t size,
                  sort_cmp_func_t cmp,
                  void *arg);

/* recursive quick */
void QuickSort(void *base, size_t nmemb, size_t size,sort_cmp_func_t cmp);

void HeapSort(void *base, size_t nmemb, size_t size,
              sort_cmp_func_t cmp,
              void *arg);

#endif /* ILRD_SORT_H */ 

