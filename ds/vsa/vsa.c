/********************************
* Task name: vsa				*
* Develoer: Inbal Elmalech		*
* Reviewer: Reut Ben Harush		*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdbool.h> /* true, false */
#include <stdio.h> /* TODO: REMOVE! printf, puts */
#include "vsa.h" /* fsa_t */

#define WORD_SIZE sizeof(size_t)
#define MAX(x, y) ((x) > (y) ? (x) : (y))
/********************   STRUCTURES AND GLOBAL VARIABLES   ********************/

struct variable_sized_alloc
{
	void *first_block;
	void *end_of_pool;
};

typedef struct block block_t;

struct block
{
	size_t capacity;
	size_t curr_size;
	block_t *next_block;
	char data[1];
};

/********************    Utility Functions Declaration   *********************/

/*************************    General Functions    ***************************/

vsa_t *VSAInit(void *mem_pool, size_t pool_size)
{
	size_t excess = 0;
	vsa_t *vsa = NULL;
	block_t *first_block = NULL;

	assert(mem_pool);

	if (pool_size  < sizeof(vsa_t))
	{
		return (NULL);
	}

	excess = (size_t)mem_pool % WORD_SIZE;

	/* Positioning vsa on the first word aligned address */
	if (0 != excess)
	{
		vsa = (vsa_t *)((char *)mem_pool + (WORD_SIZE - excess));
	}

	else
	{
		vsa = (vsa_t *)mem_pool;
	}

	/* Initializing first block */
	first_block = (block_t *)((char *)vsa + sizeof(vsa_t));

	first_block->capacity = pool_size - sizeof(vsa_t);
	first_block->curr_size = 0;
	first_block->next_block = NULL;

	/* Initializing vsa members */
	vsa->end_of_pool = (void *)(((char *)mem_pool) + pool_size);
	vsa->first_block = first_block;

	return (vsa);
}

/*---------------------------------------------------------------------------*/

void VSAFree(void *block)
{
	assert(block);

	((block_t *)block)->curr_size = 0;
}

/*---------------------------------------------------------------------------*/

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	block_t *curr_block = NULL;
	block_t *new_block = NULL;
	size_t block_padding = 0;
	size_t ttl_block_size = block_size;

	assert(vsa);

	block_padding = WORD_SIZE * (!!(block_size % WORD_SIZE)) -
					(block_size % WORD_SIZE);

	ttl_block_size += sizeof(block_t) + block_padding;
	curr_block = vsa->first_block;

	while (NULL != curr_block)
	{
		/* is there free space? */
		if (curr_block->capacity > curr_block->curr_size)
		{
			/* is there enough free space for the requested block? */
			if ((curr_block->capacity - curr_block->curr_size) >=
				ttl_block_size)
			{
				/* is the block empty? */
				if (0 == curr_block->curr_size)
				{
					curr_block->curr_size = ttl_block_size;

					return ((void *)curr_block->data);
				}
				/* block has enough free space but isn't empty => splitting */
				else
				{
					new_block = (block_t *)
					(((char *)curr_block) + curr_block->curr_size);
					new_block->capacity = curr_block->capacity -
										  curr_block->curr_size;
					new_block->curr_size = ttl_block_size;
					new_block->next_block = curr_block->next_block;

					curr_block->capacity = curr_block->curr_size;
					curr_block->next_block = new_block;

					return ((void *)new_block->data);
				}
			}

			/* Not enough free space, next block has no data, and we're not
			in the last block ==> merging with next block */
			else if ((NULL != curr_block->next_block) &&
					 (0 == curr_block->next_block->curr_size))
			{
				curr_block->next_block = curr_block->next_block->next_block;
				curr_block->capacity += curr_block->next_block->capacity;
			}

			/* Next block has data - can't nerge, promoting curr_block */
			else
			{
				curr_block = curr_block->next_block;
			}
		}

		/* No free space in block, promoting curr_block */
		else
		{
			curr_block = curr_block->next_block;
		}
	}

	/* If you made it to here without hitting another return - no space */
	return (NULL);
}

/*---------------------------------------------------------------------------*/

size_t VSABiggestFreeBlock(vsa_t *vsa)
{
	size_t biggest_free = 0;
	block_t *curr_block = NULL;
	block_t *new_block = NULL;

	assert(vsa);

	curr_block = vsa->first_block;

	while (NULL != curr_block)
	{
		/* Is there any free space? */
		if ((curr_block->capacity) > (curr_block->curr_size))
		{
			/* Is there a next block? */
			if (NULL != curr_block->next_block)
			{
				/* Is next block empty? */
				if (0 == (curr_block->next_block->curr_size))
				{
					biggest_free = MAX((curr_block->capacity -
									   curr_block->curr_size),
									   biggest_free);
					curr_block = curr_block->next_block;
				}

				/* Next block is not empty */
				else
				{
					curr_block->next_block = curr_block->next_block->next_block;
					curr_block->capacity += curr_block->next_block->capacity;
				}
			}

			else
			{
				biggest_free = MAX((curr_block->capacity -
								   curr_block->curr_size),
								   biggest_free);
				curr_block = curr_block->next_block;
			}
		}

		/* No free space in block - promote curr_block */
		else
		{
			curr_block = curr_block->next_block;
		}
	}

	return (biggest_free);
}

/*************************    Utility Functions    ***************************/


/*****************************************************************************/




