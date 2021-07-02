/********************************
* Task name: fsa				*
* Develoer: Inbal Elmalech		*
* Reviewer: Gilad Barak			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdbool.h> /* true, false */
#include <stdio.h> /* TODO: REMOVE! printf, puts */
#include "fsa.h" /* fsa_t */

#define WORD_SIZE sizeof(size_t)

/********************   STRUCTURES AND GLOBAL VARIABLES   ********************/
typedef struct block block_t;

struct fixed_sized_alloc
{
	block_t *first_block;
	size_t block_size;
};

struct block
{
	block_t *next_block;
	char data[1]; /* flexible array TODO: calc as part of data*/
};

/********************    Utility Functions Declaration   *********************/

static size_t FSACalcTtlBlockSize(size_t block_size);
size_t PtrAlign(size_t num, unsigned int align /* power of 2 */);

/*************************    General Functions    ***************************/

fsa_t *FSAInit(void *mem_pool, size_t pool_size, size_t block_size)
{
	fsa_t *fsa = NULL;
	size_t excess = 0;
	size_t ttl_block_size = 0;
	block_t *end_of_pool = NULL;
	block_t *curr_block = NULL;
	block_t *next_block = NULL;

	assert(mem_pool);

	/* Positioning fsa on the first word aligned address */
	excess = (size_t)mem_pool % WORD_SIZE;

	if (0 != excess)
	{
		fsa = (fsa_t *)(((char *)mem_pool) + (WORD_SIZE - excess));
	}

	else
	{
		fsa = (fsa_t *)mem_pool;
	}

	printf("fsa is: %p\n", (void *)fsa);
	/* Initializing fsa struct members in the beginning of the memory pool */
	fsa->block_size = block_size;
	fsa->first_block = (block_t *)(((size_t)fsa) + sizeof(fsa_t));

	/* Calculating ttl block size */
	ttl_block_size = FSACalcTtlBlockSize(block_size);

	/* Initializing curr_block, next_block, and end_of_pool */
	curr_block = fsa->first_block;
	next_block = (block_t *)((size_t)curr_block + ttl_block_size);
	end_of_pool = (block_t *)((size_t)mem_pool + pool_size);

	while (((size_t)end_of_pool - (size_t)curr_block) <= (2 * ttl_block_size))
	{
		curr_block->next_block = next_block;
		curr_block = next_block;
		next_block = (block_t *)((size_t)next_block + ttl_block_size);
	}

	curr_block->next_block = NULL;

	return (fsa);
}

/*---------------------------------------------------------------------------*/

void FSAFree(void *block)
{
	fsa_t *fsa = NULL;
	block_t *first_block = NULL;

	assert(block);

	fsa = (fsa_t *)(((fsa_t *)block)->first_block);
	first_block = fsa->first_block;
	fsa->first_block = block;
	((block_t *)block)->next_block = first_block;
}

/*---------------------------------------------------------------------------*/

void *FSAAlloc(fsa_t *fsa)
{
	block_t *alloc_block = NULL;

	assert(fsa);

	alloc_block = fsa->first_block;

	if (!alloc_block)
	{
		return (alloc_block);
	}

	fsa->first_block = alloc_block->next_block;
	alloc_block->next_block = (block_t *)fsa;

	return ((void *)alloc_block->data);
}

/*---------------------------------------------------------------------------*/

size_t FSARequiredPoolSize(size_t num_of_blocks, size_t block_size)
{
	return ((sizeof(fsa_t)) + ((num_of_blocks + 1) * 
							 (FSACalcTtlBlockSize(block_size))));
}

/*---------------------------------------------------------------------------*/

size_t FSAFreeBlocks(fsa_t *fsa)
{
	size_t counter = 0;
	block_t *tmp = NULL;

	assert(fsa);

	tmp = fsa->first_block;

	while (NULL != tmp)
	{
		tmp = tmp->next_block;
		++counter;
	}

	return (counter);
}

/*************************    Utility Functions    ***************************/

static size_t FSACalcTtlBlockSize(size_t block_size)
{
	size_t block_padding = WORD_SIZE * !!(block_size % WORD_SIZE) -
						   (block_size % WORD_SIZE);

	size_t ttl_block_size = block_size + sizeof(block_t) + block_padding;

	#if 0
	/* adding padding after user data */
	if (0 != (block_size % WORD_SIZE))
	{
		ttl_block_size += WORD_SIZE - (block_size % WORD_SIZE);
	}
	#endif

	printf("req size of pool is: %lu\n", ttl_block_size);
	return (ttl_block_size);
}

/*---------------------------------------------------------------------------*/

size_t PtrAlign(size_t num, unsigned int align /* power of 2 */)
{
	return ((num - 1 + align) & -align);
}

/*****************************************************************************/




