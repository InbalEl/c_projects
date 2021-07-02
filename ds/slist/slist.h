#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>	/* size_t */

/* slist and slist iterator  */
typedef struct slist_node* slist_iter_t;  
typedef struct slist slist_t; 

/* User's callback functions */

/* return value is boolean - 1 for match, 0 if not matched */
typedef int (*slist_is_match_func_t)(void *data, void *param);
/* return value is status - 0 for success, 1 for fail */
typedef int (*slist_do_action_func_t)(void *data, void *param);

/*****************************************************************************/

slist_t *SListCreate(void);
void SListDestroy(slist_t *slist);
void SlistEmptyList(slist_t *slist);
slist_iter_t SListBegin(const slist_t *slist);
slist_iter_t SListEnd(const slist_t *slist);
slist_iter_t SListNext(const slist_iter_t iter);
void *SListGetData(const slist_iter_t iter);
void SListSetData(slist_iter_t iter, void *data);
int SListIterIsEqual(const slist_iter_t iter1, const slist_iter_t iter2);
int SListIsEmpty(const slist_t *slist);
slist_iter_t SListInsert(slist_t *slist, slist_iter_t where, void *data);
slist_iter_t SListInsertAfter(slist_t *slist, slist_iter_t where, void *data);
slist_iter_t SListRemove(slist_t *slist, slist_iter_t iter);
size_t SListCount(const slist_t *slist);
slist_iter_t SListFind(const slist_iter_t from, 
                       const slist_iter_t to,
					   slist_is_match_func_t is_match, 
                       void *param);
int SListForEach(const slist_iter_t from, const slist_iter_t to, 
				 slist_do_action_func_t do_action, void *param);
void SListAppend(slist_t *dest, slist_t *src);

/*---------------------------------------------------------------------------*/
#endif /* SLIST_H */
/*****************************************************************************/