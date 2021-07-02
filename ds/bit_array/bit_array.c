/*
File name:
Develoer: Inbal Elmalech
Reviewer:
Review date:
*/
#include <stdio.h> /* TODO: REMOVE */
#include <stdlib.h> /* size_t */
#include <limits.h> /* CHAR_BIT */

# define BIT_INDEX_MIN 1 
# define MAX_NOF_BITS (sizeof(size_t) * CHAR_BIT)
# define INT_ASCII 48

/* typedef unsigned char byte_t; */
typedef size_t bit_array_t;


/******************    STATIC FUNCTIONS DECLARATIONS    **********************/

/* static byte_t BitCountSetBitsInByte(byte_t n)
{
	n = (n & 0x55) + ((n & 0xAA) >> 1);
	n = (n & 0x33) + ((n & 0xCC) >> 2);
	n = (n & 0x0F) + ((n & 0xF0) >> 4);

	return (n);
} */

/*---------------------------------------------------------------------------*/

static int IsIndexInRange(size_t index)
{
	return ((index >= BIT_INDEX_MIN) && (index <= MAX_NOF_BITS));
}

/***************************      FUNCTIONS     ******************************/

bit_array_t BitArrSetAll(bit_array_t arr)
{
	return (arr | (bit_array_t)~0);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrResetAll(bit_array_t arr)
{
	return (arr & (bit_array_t)0);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrSetOn(bit_array_t arr, size_t index)
{
	if (IsIndexInRange(index))
	{
		return (arr | ((bit_array_t)1 << (index - 1)));
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrSetOff(bit_array_t arr, size_t index)
{
	if (IsIndexInRange(index))
	{
		return (arr & ((bit_array_t)1 << ~(index - 1)));
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrSetBit(bit_array_t arr, size_t index, int boolean_value)
{
	if (IsIndexInRange(index))
	{
		if (boolean_value)
		{
			return (BitArrSetOn(arr, index));
		}

		else
		{
			return (BitArrSetOff(arr, index));
		}
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrFlip(bit_array_t arr, size_t index)
{
	if (IsIndexInRange(index))
	{
		return (arr ^ ((bit_array_t)1 << (index - 1)));
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

int BitArrIsOn(bit_array_t arr, size_t index)
{
	if (IsIndexInRange(index))
	{
		return ((arr >> (index - 1)) & (bit_array_t)1);
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

int BitArrIsOff(bit_array_t arr, size_t index)
{
	if (IsIndexInRange(index))
	{
		return ((arr >> (index - 1)) & (bit_array_t)1);;
	}

	return (arr);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrRotR(bit_array_t arr, size_t num_to_shift)
{
	num_to_shift %= MAX_NOF_BITS;
	return ((arr >> num_to_shift) | (arr << (MAX_NOF_BITS - num_to_shift)));
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrRotL(bit_array_t arr, size_t num_to_shift)
{
	num_to_shift %= MAX_NOF_BITS;
	return ((arr << num_to_shift) | (arr >> (MAX_NOF_BITS - num_to_shift)));
}

/*---------------------------------------------------------------------------*/

size_t BitArrCountOn(bit_array_t arr)
{
	arr = (arr & 0x55) + ((arr & 0xAA) >> 1);
	arr = (arr & 0x33) + ((arr & 0xCC) >> 2);
	arr = (arr & 0x0F) + ((arr & 0xF0) >> 4);

	return (arr);
}

/*---------------------------------------------------------------------------*/

char *BitArrToString(bit_array_t arr, char *dest)
{
	size_t i = 0;

	dest[CHAR_BIT - 1] = '\0';

	for(i = (CHAR_BIT - 1); i >= (size_t)0 ; --i)
	{
		dest[i] = (arr % 2 + INT_ASCII);
		arr /= 2;
	}

	return(dest);
}

/*---------------------------------------------------------------------------*/

size_t BitArrCountOnLUT(bit_array_t arr)
{
	printf("hi I'm a stub\n");
	/* TODO: stub */
	return (arr);
}

/*---------------------------------------------------------------------------*/

size_t BitArrCountOff(bit_array_t arr)
{
	return (BitArrCountOn(~arr));
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrMirror(bit_array_t arr)
{
	printf("hi I'm a stub\n");
	/* TODO: stub */
	return (arr);
}

/*---------------------------------------------------------------------------*/

bit_array_t BitArrMirrorLUT(bit_array_t arr)
{
	printf("hi I'm a stub\n");
	/* TODO: stub */
	return (arr);
}

/*****************************************************************************/
/*
static size_t CountOnBits(size_t bitarray, size_t nof_bytes)
{
	unsigned int i = 0;
	unsigned int counter = 0;
	size_t tmp = 0;
	size_t mask = 0xFF;

	for (i = 0; i < nof_bytes; ++i)
	{
		tmp = ((bitarray & (mask << (i * CHAR_BIT))) >> (i * CHAR_BIT));

		tmp = ((tmp & 0x55) + ((tmp & 0xAA) >> 1));
		tmp = ((tmp & 0x33) + ((tmp & 0xCC) >> 2));
		tmp = ((tmp & 0xF) + ((tmp & 0xF0) >> 4));

		counter += tmp;
	}

	return (counter);
}*/