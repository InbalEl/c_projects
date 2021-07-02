/********************************
* Task name: circular buffer	*
* Develoer: Inbal Elmalech		*
* Reviewer: Itay Kapara			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <errno.h>	/* errno */
#include <unistd.h> /* ssize_t */
#include <stddef.h> /* size_t */
#include <string.h>	/* strerror */
#include <stdio.h> /* printf, puts */
#include <stdlib.h> /* malloc, free */
#include "c_buffer.h" /* circular_buffer */

/*****************************   DECLARATIONS   ******************************/

void Test_CBufferCreateDestroyAndCapacity(void);
void Test_CBufferWriteAndRead(void);
void Test_CBufferIsEmptyAndFreeSpace(void);
void Test_Case(void);

/********************************  MAIN  *************************************/

int main(void)
{
	Test_CBufferCreateDestroyAndCapacity();
	Test_CBufferWriteAndRead();
	Test_CBufferIsEmptyAndFreeSpace();
	Test_Case();

	return (0);
}

/***************************** END OF MAIN ***********************************/

/***********************    General Functions    *****************************/

void Test_CBufferCreateDestroyAndCapacity(void)
{
	c_buffer_t *new_buffer = NULL;

	printf("\n-----Test_CBufferCreateDestroyAndCapacity-----\n");

	printf("***CBufferCreate***\n");

	new_buffer = CBufferCreate(6);

	if (NULL == new_buffer)
	{
		printf("New buffer is null!\n");
		return;
	}

	printf("new buffer at %p\n", (void *)new_buffer);

	printf("***CBufferCapacity***\n");

	printf("Capacity of buffer is %lu\n",CBufferCapacity(new_buffer));

	printf("***CBufferIsEmpty***\n");

	printf("Is buffer empty? %d\n",CBufferIsEmpty(new_buffer));

	printf("***CBufferDestroy***\n");

	printf("Destroying buffer at %p\n", (void *)new_buffer);

	CBufferDestroy(new_buffer);
}

/*---------------------------------------------------------------------------*/

void Test_CBufferWriteAndRead(void)
{
	c_buffer_t *new_buffer = CBufferCreate(20);

	const char *src = "don't panic";

	char *dest = (char *)malloc((sizeof(char) * 11) + 1);

	printf("\n----------Test_CBufferWriteAndRead------------\n");

	if ((NULL == new_buffer) | (NULL == dest))
	{
		printf("Memory allocation failed!\n");

		return;
	}

	printf("new buffer at %p\n", (void *)new_buffer);

	printf("Capacity of buffer is %lu\n",CBufferCapacity(new_buffer));

	printf("***CBufferWrite***\n");

	printf("src is: %s\n", src);

	printf("%ld bytes from src were written into buffer\n",
	CBufferWrite(new_buffer, (void *)src, (size_t)11));

	printf("***CBufferIsEmpty***\n");

	printf("Is buffer empty? %d\n",CBufferIsEmpty(new_buffer));

	printf("***Test_CBufferFreeSpace***\n");

	printf("Free space in buffer is %lu\n",CBufferFreeSpace(new_buffer));

	printf("***CBufferRead***\n");

	printf("%ld bytes were read from buffer\n",
	CBufferRead(new_buffer, (void *)dest, 11));
	dest[11] = '\0';

	printf("dest is: %s\n", dest);

	printf("Destroying buffer at %p\n", (void *)new_buffer);
	CBufferDestroy(new_buffer);

	free(dest);
	dest = NULL;
}

/*---------------------------------------------------------------------------*/

void Test_CBufferIsEmptyAndFreeSpace(void)
{
	c_buffer_t *new_buffer = CBufferCreate(25);

	const char *src = "don't panic";

	char *dest = (char *)malloc(sizeof(char) * 100);

	printf("\n-------Test_CBufferIsEmptyAndFreeSpace--------\n");

	if ((NULL == new_buffer) | (NULL == dest))
	{
		printf("Memory allocation failed!\n");

		return;
	}

	printf("new buffer at %p\n", (void *)new_buffer);
	printf("Capacity of buffer is %lu\n",CBufferCapacity(new_buffer));

	printf("***CBufferIsEmpty***\n");

	printf("Is buffer empty? %d\n",CBufferIsEmpty(new_buffer));

	printf("***Test_CBufferFreeSpace***\n");

	printf("Free space in buffer is %lu\n",CBufferFreeSpace(new_buffer));

	printf("%ld bytes from src were written into buffer\n",
	CBufferWrite(new_buffer, src, 11));

	printf("***CBufferIsEmpty***\n");

	printf("Is buffer empty? %d\n",CBufferIsEmpty(new_buffer));

	printf("***Test_CBufferFreeSpace***\n");

	printf("Free space in buffer is %lu\n",CBufferFreeSpace(new_buffer));

	printf("%ld bytes were read from buffer\n",
	CBufferRead(new_buffer, (void *)dest, 11));

	dest[11] = '\0';

	printf("dest is: %s\n", dest);

	printf("***CBufferIsEmpty***\n");

	printf("Is buffer empty? %d\n",CBufferIsEmpty(new_buffer));

	printf("***Test_CBufferFreeSpace***\n");

	printf("Free space in buffer is %lu\n",CBufferFreeSpace(new_buffer));

	printf("Destroying buffer at %p\n", (void *)new_buffer);
	CBufferDestroy(new_buffer);

	free(dest);
	dest = NULL;
}

/*---------------------------------------------------------------------------*/

void Test_Case(void)
{
	c_buffer_t *new_buffer = CBufferCreate(35);

	char *dest = (char *)malloc(sizeof(char) * 100);

	const char *src = "so long and thanks for all the fish";

	if (NULL == new_buffer)
	{
		printf("Memory allocation failed!\n");

		return;
	}

	printf("\n-------Test_Case--------\n");

	printf("Capacity of new buffer is %lu\n",CBufferCapacity(new_buffer));

	printf("Free space is %lu\n",CBufferFreeSpace(new_buffer));

	printf("Trying to read from an empty buffer:\n");

	if ((-1) == CBufferRead(new_buffer, (void *)dest, 3))
	{
		printf("The error message is : %s\n", strerror(errno));
	}

	printf("Free space is %lu\n",CBufferFreeSpace(new_buffer));

	printf("Writing 3 bytes into buffer\n");

	printf("%ld bytes from src were written into buffer\n",
	CBufferWrite(new_buffer, (void *)src, 3));

	printf("Free space is %lu\n",CBufferFreeSpace(new_buffer));

	printf("Trying to read more than was written in (5):\n");

	printf("%ld bytes were read from buffer\n",
	CBufferRead(new_buffer, (void *)dest, 5));

	printf("Free space is %lu\n",CBufferFreeSpace(new_buffer));

	printf("Trying to write more than capacity in (40):\n");

	printf("%ld bytes from src were written into buffer\n",
	CBufferWrite(new_buffer, (void *)src, 40));

	printf("Trying to write to a full buffer:\n");

	if ((-1) == CBufferWrite(new_buffer, (void *)dest, 3))
	{
		printf("The error message is : %s\n", strerror(errno));
	}

	free(dest);
	dest = NULL;

	printf("Destroying buffer at %p\n", (void *)new_buffer);
	CBufferDestroy(new_buffer);
}

/*****************************************************************************/
