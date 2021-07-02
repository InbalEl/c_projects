/*
File name:
Develoer: Inbal Elmalech
Reviewer:
Review date:
*/

# include <stdio.h> /* printf */
# include "bit_array.h"

/*************************    DECLARATIONS    ********************************/

static void PrintLine(char *str);
static void PrintBin(size_t number);
void Test_BitArrSetOn();
void Test_BitArrSetOff();
void Test_BitArrSetBit();

/********************************* MAIN **************************************/

int main(void)
{
	PrintLine("BitArrSetOn");

	Test_BitArrSetOn();

	PrintLine("BitArrSetOff");

	Test_BitArrSetOff();

	PrintLine("BitArrSetBit");

	Test_BitArrSetBit();

	return (0);
}

/****************************   FUNCTIONS   **********************************/

static void PrintLine(char *str)
{
	printf("\n----------------%s----------------\n\n", str);
}

/*---------------------------------------------------------------------------*/

static void PrintBin(size_t number)
{
	size_t i;

	printf("%lu = ",number);

	for (i = (size_t)1 << ((sizeof(size_t) * 8) - 1); i > 0; i /= 2)
	{
		number & i ? printf("1") : printf("0");
	}

	puts("");
}

/*---------------------------------------------------------------------------*/

void Test_BitArrSetOn()
{
	size_t bit_arr = 0xB275;
	PrintBin(bit_arr);

	printf("Turning on bit 8\n");
	bit_arr = BitArrSetOn(bit_arr, 8);

	PrintBin(bit_arr);

	printf("Turning on bit 7\n");
	bit_arr = BitArrSetOn(bit_arr, 7);

	PrintBin(bit_arr);

	printf("Turning on bit 0\n");
	bit_arr = BitArrSetOn(bit_arr, 0);

	PrintBin(bit_arr);

	printf("Turning on bit 76\n");
	bit_arr = BitArrSetOn(bit_arr, 76);

	PrintBin(bit_arr);
}

/*---------------------------------------------------------------------------*/

void Test_BitArrSetOff()
{
	size_t bit_arr = 0xB275;
	PrintBin(bit_arr);

	printf("Turning on bit 10\n");
	bit_arr = BitArrSetOff(bit_arr, 10);

	PrintBin(bit_arr);

	printf("Turning on bit 4\n");
	bit_arr = BitArrSetOn(bit_arr, 4);

	PrintBin(bit_arr);

	printf("Turning on bit 0\n");
	bit_arr = BitArrSetOn(bit_arr, 0);

	PrintBin(bit_arr);

	printf("Turning on bit 76\n");
	bit_arr = BitArrSetOn(bit_arr, 76);

	PrintBin(bit_arr);
}

/*---------------------------------------------------------------------------*/

void Test_BitArrSetBit()
{
	size_t bit_arr = 0xB275;
	PrintBin(bit_arr);

	printf("Turning on bit 45\n");
	bit_arr = BitArrSetBit(bit_arr, 45, 1);

	PrintBin(bit_arr);

	printf("Turning off bit 45\n");
	bit_arr = BitArrSetBit(bit_arr, 45, 0);

	PrintBin(bit_arr);

	printf("Turning on bit 0\n");
	bit_arr = BitArrSetBit(bit_arr, 0, 1);

	PrintBin(bit_arr);

	printf("Turning on bit 77\n");
	bit_arr = BitArrSetBit(bit_arr, 77, 1);

	PrintBin(bit_arr);

	printf("Turning 5 bit 36\n");
	bit_arr = BitArrSetBit(bit_arr, 36, 5);

	PrintBin(bit_arr);

	printf("Turning 5 bit 88\n");
	bit_arr = BitArrSetBit(bit_arr, 88, 5);

	PrintBin(bit_arr);

	printf("Turning on bit 45\n");
	bit_arr = BitArrSetBit(bit_arr, 45, 1);

	PrintBin(bit_arr);
}

/*****************************************************************************/



