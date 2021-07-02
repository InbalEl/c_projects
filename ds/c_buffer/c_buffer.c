/********************************
* Task name: circular buffer	*
* Develoer: Inbal Elmalech		*
* Reviewer: Itay Kapara			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h>	/* size_t, offsetof */
#include <errno.h>	/* errno */
#include <unistd.h> /* ssize_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "c_buffer.h" /* circular_buffer */

#define MIN(x, y) ((x < y) ? (x) : (y))

#define BUFFER_T_SIZE sizeof(struct c_buffer)
#define BUFFER_OFFSET offsetof(struct c_buffer, buffer)
#define OVERLAP (BUFFER_T_SIZE - BUFFER_OFFSET)

#define ERROR (-1)
#define EXTRA_BYTE 1

/***************************   ENUMS & STRUCTURES  ***************************/

struct c_buffer
{
	char *read;
	char *write;
	size_t capacity;
	char buffer[1];
};


/*******************    Utility Functions Declarations    ********************/

static char *CBufferPromotePtr(c_buffer_t *c_buffer, char *move);

/*************************    General Functions    ***************************/

c_buffer_t *CBufferCreate(size_t capacity)
{
	c_buffer_t *new_buffer = (c_buffer_t *) malloc(BUFFER_OFFSET + capacity);

	if (NULL == new_buffer)
	{
		return (NULL);
	}

	new_buffer->capacity = capacity;
	new_buffer->read = (char *)(new_buffer) + BUFFER_OFFSET;
	new_buffer->write = (char *)(new_buffer) + BUFFER_OFFSET;

	return (new_buffer);
}

/*---------------------------------------------------------------------------*/

void CBufferDestroy(c_buffer_t *c_buffer)
{
	assert(c_buffer);

	free(c_buffer);

	c_buffer = NULL;
}

/*---------------------------------------------------------------------------*/

ssize_t CBufferRead(c_buffer_t *c_buffer, void *dest, size_t count)
{
	size_t nof_bytes_read = 0;
	char *dest_write = NULL;

	assert(c_buffer);
	assert(dest);

	dest_write = (char *)dest;

	if (CBufferIsEmpty(c_buffer))
	{
		errno = ENODATA;

		return (ERROR);
	}

	count = MIN(count, CBufferCapacity(c_buffer) - CBufferFreeSpace(c_buffer));

	while (nof_bytes_read < count)
	{
		*dest_write = *(c_buffer->read);

		c_buffer->read = (CBufferPromotePtr(c_buffer, c_buffer->read));
		++dest_write;
		++nof_bytes_read;
	}

	return ((ssize_t) nof_bytes_read);
}

/*---------------------------------------------------------------------------*/

ssize_t CBufferWrite(c_buffer_t *c_buffer, const void *src, size_t count)
{
	size_t nof_bytes_written = 0;
	char *src_read = NULL;
	size_t free_spaces = 0;

	assert(c_buffer);
	assert(src);

	src_read = (char *)src;

	free_spaces = CBufferFreeSpace(c_buffer);

	if (0 == free_spaces)
	{
		errno = ENOBUFS;

		return (ERROR);
	}

	count = MIN(count, free_spaces);

	while (nof_bytes_written < count)
	{
		*(c_buffer->write) = *src_read;

		c_buffer->write = (CBufferPromotePtr(c_buffer, c_buffer->write));
		++src_read;
		++nof_bytes_written;
	}

	return ((ssize_t) nof_bytes_written);
}



/*---------------------------------------------------------------------------*/

int CBufferIsEmpty(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return (c_buffer->write == c_buffer->read);
}

/*---------------------------------------------------------------------------*/

size_t CBufferFreeSpace(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	/* read pointer is after write pointer */
	if (c_buffer->read > c_buffer->write)
	{
		return ((c_buffer->read - c_buffer->write) - EXTRA_BYTE);
	}
	/* empty buffer or write pointer is after read pointer */
	return (c_buffer->capacity - (c_buffer->write - c_buffer->read));

}

/*---------------------------------------------------------------------------*/

size_t CBufferCapacity(const c_buffer_t *c_buffer)
{
	assert(c_buffer);

	return (c_buffer->capacity);
}

/*************************    Utility Functions    ***************************/

static char *CBufferPromotePtr(c_buffer_t *c_buffer, char *move)
{
	if (move == (c_buffer->buffer + c_buffer->capacity))
	{
		return (c_buffer->buffer);
	}

	return (move + 1);
}

/*****************************************************************************/

