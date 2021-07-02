/********************************
* Task name: hash table			*
* Develoer: Inbal Elmalech		*
* Reviewer: Janna Torbilo		*
* Date: Aug ?, 2020				*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h> /* printf, puts */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp */
#include "hash_table.h" /* hash_table_t */
#define TABLE_SIZE 10
/****************************   DECLARATIONS   *******************************/

void TestHashCreate(void);
void TestHashSizeTableSizeBucketSizeIsEmpty(void);
void TestHashInsertRemove(void);
void TestHashFindForEach(void);
int HashFunc(const void *data1, void *param);
int MatchFunc(const void *data, void *param);
int DoAction(const void *data, void *param);
void TestLinuxDictionary(void);

/********************************  MAIN  *************************************/

int main(void)
{
	puts("---------------------------------");
	TestHashCreate();
	puts("---------------------------------");
	TestHashSizeTableSizeBucketSizeIsEmpty();
	puts("---------------------------------");
	TestHashInsertRemove();
	puts("---------------------------------");
	TestHashFindForEach();
	puts("\n--------------- Linux Dictionary ------------------");
	TestLinuxDictionary();

	return (0);
}

/*****************************************************************************/
/* 
*	- hash_func - pointer to hash function.
*   - mach_func - pointer to compare function.
*	- param - parameters given by user to be sent into hash function.
*   - table_size - size of hash table requesed by the user.
*/
void TestHashCreate(void)
{
	hash_t * hashtable = NULL;
	printf("Creating a new hash table!\n");
	
	hashtable = HashCreate(HashFunc, MatchFunc, NULL ,TABLE_SIZE);

	if (NULL == hashtable)
	{
		printf("oh no, failed to create table!\n");
		return;
	}

	printf("new hashtable at %p\n", (void *)hashtable);

	printf("Destroying hashtable\n");
	HashDestroy(hashtable);
}

/*---------------------------------------------------------------------------*/

void TestHashSizeTableSizeBucketSizeIsEmpty(void)
{
	hash_t * hashtable = NULL;
	
	hashtable = HashCreate(HashFunc, MatchFunc, NULL ,TABLE_SIZE);

	if (NULL == hashtable)
	{
		printf("oh no, failed to create table!\n");
		return;
	}

	printf("new hashtable at %p\n", (void *)hashtable);

	printf("size: %lu\n", HashSize(hashtable));
	printf("table size: %lu\n", HashTableSize(hashtable));
	printf("bucket size: %lu\n", HashBucketSize(4, hashtable));
	printf("Is empty? %d\n", HashIsEmpty(hashtable));
}

/*---------------------------------------------------------------------------*/

void TestHashInsertRemove(void)
{
	char *str1 = "So long and thanks for all the fish";
	char *str2 = "So sad that it should come to this";
	char *str3 = "We tried to warn you all that your dead";

	char *str4 = "You may not share our intellect";
	char *str5 = "Which might explain your disrespect";
	char *str6 = "For all the natural wonders that grow around you";

	hash_t * hashtable = NULL;
	
	hashtable = HashCreate(HashFunc, MatchFunc , NULL ,TABLE_SIZE);

	if (NULL == hashtable)
	{
		printf("oh no, failed to create table!\n");
		return;
	}

	printf("new hashtable at %p\n", (void *)hashtable);

	printf("Inserting data into hashtable:\n");

	HashInsert(hashtable, (void *)str1);
	printf("size: %lu\n", HashSize(hashtable));
	HashInsert(hashtable, (void *)str2);
	printf("size: %lu\n", HashSize(hashtable));
	HashInsert(hashtable, (void *)str3);
	printf("size: %lu\n", HashSize(hashtable));
	HashInsert(hashtable, (void *)str4);
	printf("size: %lu\n", HashSize(hashtable));
	HashInsert(hashtable, (void *)str5);
	printf("size: %lu\n", HashSize(hashtable));
	HashInsert(hashtable, (void *)str6);

	printf("size: %lu\n", HashSize(hashtable));
	printf("table size: %lu\n", HashTableSize(hashtable));
	printf("bucket size at index 2: %lu\n", HashBucketSize(2, hashtable));
	printf("Is empty? %d\n", HashIsEmpty(hashtable));

	HashRemove(hashtable, (void *)str1);
	HashRemove(hashtable, (void *)str2);
	HashRemove(hashtable, (void *)str3);
	HashRemove(hashtable, (void *)str4);
	HashRemove(hashtable, (void *)str5);
	HashRemove(hashtable, (void *)str6);

	printf("size: %lu\n", HashSize(hashtable));
	printf("table size: %lu\n", HashTableSize(hashtable));
	printf("bucket size at index 3: %lu\n", HashBucketSize(3, hashtable));
	printf("Is empty? %d\n", HashIsEmpty(hashtable));
}

/*---------------------------------------------------------------------------*/

void TestHashFindForEach(void)
{
	char *str1 = "So long and thanks for all the fish";
	char *str2 = "So sad that it should come to this";
	char *str3 = "We tried to warn you all that your dead";

	char *str4 = "You may not share our intellect";
	char *str5 = "Which might explain your disrespect";
	char *str6 = "For all the natural wonders that grow around you";

	char *str7 = "If I had just one last wish, I would like a tasty fish";

	hash_t * hashtable = NULL;
	
	hashtable = HashCreate(HashFunc, MatchFunc , NULL ,TABLE_SIZE);

	if (NULL == hashtable)
	{
		printf("oh no, failed to create table!\n");
		return;
	}

	printf("new hashtable at %p\n", (void *)hashtable);

	printf("Inserting data into hashtable:\n");

	HashInsert(hashtable, (void *)str1);
	HashInsert(hashtable, (void *)str2);
	HashInsert(hashtable, (void *)str3);
	HashInsert(hashtable, (void *)str4);
	HashInsert(hashtable, (void *)str5);
	HashInsert(hashtable, (void *)str6);

	printf("size: %lu\n", HashSize(hashtable));
	printf("table size: %lu\n", HashTableSize(hashtable));
	printf("bucket size: %lu\n", HashBucketSize(2, hashtable));
	printf("Is empty? %d\n", HashIsEmpty(hashtable));

	printf("trying to find nonexistent string:\n%s\n", str7);
	printf("Was string found? %d\n",
	(NULL == HashFind(hashtable, (void *)str7)));
	
	HashInsert(hashtable, (void *)str7);

	printf("After inserting sting, was it found?%d\n",
		   (NULL == HashFind(hashtable, (void *)str7)));

	printf("Printing all strings using for each:\n");

	HashForEach(hashtable, (hash_do_action_func_t)DoAction, NULL);
}

/*---------------------------------------------------------------------------*/

void TestLinuxDictionary(void)
{
	FILE *dictionary = NULL;
	size_t i = 0;
	hash_t *dict_hash = NULL;
	size_t nof_lines_in_dict = 102305;
	char **dict_arr = NULL;
	size_t table_size = 10000;

	printf("Initiating dictionary hash table\n");

	dict_hash = HashCreate(HashFunc, MatchFunc, NULL, table_size);

	if (NULL == dict_hash)
	{
		printf("No soup for you\n");
	}

	dictionary = fopen("/usr/share/dict/words", "r");

	if (NULL == dictionary)
	{
		printf("No soup for you\n");
	}

	dict_arr = (char **)malloc(nof_lines_in_dict * sizeof(char *));

	if (NULL == dict_arr)
	{
		printf("No soup for you\n");
	}

	for (i = 0; i < nof_lines_in_dict; ++i)
	{
		dict_arr[i] = (char *)malloc(50 * sizeof(char));

		if (NULL == dict_arr[i])
		{
			printf("No soup for you\n");
			return;
		}

		fscanf(dictionary, "%s", dict_arr[i]);
		HashInsert(dict_hash, dict_arr[i]);
	}

	printf("size: %lu\n", HashSize(dict_hash));
	printf("table size: %lu\n", HashTableSize(dict_hash));
	printf("bucket size at index 212: %lu\n", HashBucketSize(212, dict_hash));
	printf("Is empty? %d\n", HashIsEmpty(dict_hash));

	fclose(dictionary);
}

/***************************** Utilty Functions ******************************/

int HashFunc(const void *data1, void *param)
{
    const int p = 31;
    const int m = 100;
    int hash_value = 0;
    int p_pow = 1;
	char *data_str = (char *)data1;

	(void)param;

	while (*data_str)
	{
        hash_value += ((*data_str - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;

		++data_str;
	}

    return hash_value;

}

/*---------------------------------------------------------------------------*/

int MatchFunc(const void *data, void *param)
{
	if (0 == strcmp((char *)data, (char *)param))
	{
		return (1);
	}

	return (0);
}

/*---------------------------------------------------------------------------*/

int DoAction(const void *data, void *param)
{
	(void)param;

	printf("%s\n", (char *)data);

	return (0);
}

/*****************************************************************************/

