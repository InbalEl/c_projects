/********************************
* Task name: hash table			*
* Develoer: Inbal Elmalech		*
* Reviewer: Janna Torbilo		*
* Date: Aug ?, 2020				*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "dlist.h" /* dlist */
#include "hash_table.h" /* hash_table */
/******************************   STRUCTURES   *******************************/

typedef struct bucket bucket_t;

struct hash
{
    hash_table_func_t hash_func;
    hash_is_match_func_t match_func;
    size_t table_size;
    bucket_t *bucket_arr; /* unecessary pointer */
    void *param;
};

struct bucket
{
    dlist_t *data_chain;
};

/****************************   DECLARATIONS   *******************************/

/*************************    General Functions    ***************************/
hash_t *HashCreate(hash_table_func_t hash_func,hash_is_match_func_t match_func,
                   void *param, size_t table_size)
{
    size_t i = 0;
    hash_t *hash = NULL;
    bucket_t *bucket_arr = NULL;
    
    assert(hash_func);
    assert(match_func);

    hash = (hash_t *) malloc(sizeof(hash_t));

    if (NULL == hash)
    {
        return (NULL);
    }
    
    bucket_arr = (bucket_t *)malloc(sizeof(bucket_t) * table_size);

    hash->hash_func = hash_func;
    hash->match_func = match_func;
    hash->table_size = table_size;
    hash->param = param;
    hash->bucket_arr = bucket_arr;

    /* TODO: change lazy list creation */
    for (i = 0; i < table_size; ++i)
    {
        (bucket_arr + i)->data_chain = DListCreate();

        if (NULL == (bucket_arr + i)->data_chain)
        {
            return (NULL);
        }/* possible memory leak - not freeing everything before the fail! */
    }

    return (hash);
}

/*---------------------------------------------------------------------------*/

void HashDestroy(hash_t *hash)
{
    size_t i = 0;
    assert(hash);

    for (i = 0; i < hash->table_size; ++i)
    {
        DListDestroy((hash->bucket_arr + i)->data_chain);
    }

    free(hash->bucket_arr);
    hash->bucket_arr = NULL;

    free(hash);
    hash = NULL;
}

/*---------------------------------------------------------------------------*/

size_t HashSize(const hash_t *hash)
{
    size_t i = 0;
    size_t nof_elements = 0;
    bucket_t *bucket_arr = NULL;

    assert(hash);

    bucket_arr = hash->bucket_arr;

    for (i = 0; i < hash->table_size; ++i)
    {
        nof_elements += DListSize((bucket_arr + i)->data_chain);
    }

    return (nof_elements);
}

/*---------------------------------------------------------------------------*/

size_t HashTableSize(const hash_t *hash)
{
    assert(hash);

    return (hash->table_size);
}

/*---------------------------------------------------------------------------*/

size_t HashBucketSize(const size_t index, const hash_t *hash)
{
    assert(hash);

    return (DListSize((hash->bucket_arr + index)->data_chain));
}

/*---------------------------------------------------------------------------*/

int HashIsEmpty(const hash_t *hash)
{
    assert(hash);

    return (0 == HashSize(hash));
}

/*---------------------------------------------------------------------------*/

int HashInsert(hash_t *hash, void *data)
{
    size_t insert_index = 0;
    dlist_iter_t from = NULL; 
    dlist_iter_t to = NULL;
    bucket_t * insertion_bucket = NULL;

    assert(hash);

    /* prepare args for DListFind Call */
    insert_index = (hash->hash_func(data, hash->param)) % hash->table_size;

    insertion_bucket = (hash->bucket_arr + insert_index);

    if (DListIsEmpty(insertion_bucket->data_chain))
    {
        DListPushBack(insertion_bucket->data_chain, data);
        
        return (0);
    }

    /* TODO: is this the right param to use? */
    /* if we got back the end of the list - data wasn't found */

    from = DListBegin(insertion_bucket->data_chain);
    to = DListEnd(insertion_bucket->data_chain);

    if (to != DListFind(from, to, hash->match_func, data))
    {
        return (1);
    }

    return (to ==
            DListPushBack(insertion_bucket->data_chain, data));
}

/*---------------------------------------------------------------------------*/

void HashRemove(hash_t *hash, void *data)
{
    size_t remove_index = 0;
    dlist_iter_t from = NULL; 
    dlist_iter_t to = NULL;
    dlist_iter_t iter_to_remove = NULL;
    bucket_t *remove_bucket = NULL;

    assert(hash);

    /* prepare args for DListFind Call */
    remove_index = (hash->hash_func(data, hash->param)) % hash->table_size;

    remove_bucket = (hash->bucket_arr + remove_index);

    from = DListBegin(remove_bucket->data_chain);
    to = DListEnd(remove_bucket->data_chain);

    /* TODO: is this the right param to use? */
    iter_to_remove = DListFind(from, to, hash->match_func, data);

    /* if we got back the end of the list - data wasn't found */
    if (to == iter_to_remove)
    {
        return;
    }

    DListRemove(iter_to_remove);
}

/*---------------------------------------------------------------------------*/

void *HashFind(const hash_t *hash, const void *data)
{
    size_t find_index = 0;
    dlist_iter_t from = NULL; 
    dlist_iter_t to = NULL;
    dlist_iter_t target_iter = NULL;
    bucket_t *find_bucket = NULL;

    assert(hash);

    /* prepare args for DListFind Call */
    find_index = (hash->hash_func(data, hash->param)) % hash->table_size;

    find_bucket = (hash->bucket_arr + find_index);

    from = DListBegin(find_bucket->data_chain);
    to = DListEnd(find_bucket->data_chain);

    /* call DListFind */
    /* TODO: is this the right param to use? */
    target_iter = DListFind(from, to, hash->match_func, (void *)data);

    /* if we got back the end of the list - data wasn't found */
    if (target_iter == to)
    {
        return (NULL);
    }

    return (DListGetData(target_iter));
}

/*---------------------------------------------------------------------------*/

int HashForEach(hash_t *hash, hash_do_action_func_t do_action ,void *param)
{
    size_t i = 0;
    int running_res = 0;
    dlist_iter_t from = NULL; 
    dlist_iter_t to = NULL;
    bucket_t *curr_bucket = NULL;

    assert(hash);
 
    curr_bucket = hash->bucket_arr;

    /* DListForEach call loop */
    for (i = 0;
        (i < hash->table_size) && (running_res == 0);
        ++i, ++curr_bucket)
    {
        if (DListIsEmpty(curr_bucket->data_chain))
        {
            continue;
        }

        from = DListBegin(curr_bucket->data_chain);
        to = DListEnd(curr_bucket->data_chain);
        
        running_res = DListForEach(from, to, do_action, param);
    }

    return (running_res);
}

/***************************** Utilty Functions ******************************/

/*---------------------------------------------------------------------------*/

/*****************************************************************************/