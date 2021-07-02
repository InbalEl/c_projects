#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
/*----------------------------------------------------------------------------*/

void SwapElements(void *data1,void *data2, size_t nof_bytes)
{
	char tmp = 0;
	char *data1_byte = (char *)data1;
	char *data2_byte = (char *)data2;

	while (nof_bytes)
	{
		tmp = *data1_byte;
		*data1_byte = *data2_byte;
		*data2_byte = tmp;

		--nof_bytes;
		++data1_byte;
		++data2_byte;
	}
}

/*----------------------------------------------------------------------------*/

unsigned int FindMinUint(unsigned int *arr, size_t arr_size)
{
	unsigned int min = 0;
	size_t i = 0;

	assert(arr);
	assert(arr_size);

	min = arr[i];

	for (i = 0; i < arr_size; ++i)
	{
		if (min > arr[i])
		{
			min = arr[i];
		}
	}

	return (min);
}

/*----------------------------------------------------------------------------*/

int FindMinInt(int *arr, size_t arr_size)
{
	int min = 0;
	size_t i = 0;

	assert(arr);
	assert(arr_size);

	min = arr[i];

	for (i = 0; i < arr_size; ++i)
	{
		if (min > arr[i])
		{
			min = arr[i];
		}
	}

	return (min);
}

/*----------------------------------------------------------------------------*/

int FindMaxInt(int *arr, size_t arr_size)
{
	int max = 0;
	size_t i = 0;

	assert(arr);
	assert(arr_size);

	max = arr[i];

	for (i = 0; i < arr_size; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}

	return (max);
}

/*----------------------------------------------------------------------------*/
