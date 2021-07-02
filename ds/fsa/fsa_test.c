/********************************
* Task name: fsa				*
* Develoer: Inbal Elmalech		*
* Reviewer: Gilad Barak			*
********************************/


/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free, size_t */
#include <stdio.h> /* printf, puts */
#include "fsa.h" /* fsa_t */

/***************************   ENUMS & STRUCTURES  ***************************/
typedef struct student
{
	char *name;
	char *city;
	int age;
}
student_t;

/*****************************   DECLARATIONS   ******************************/
void test_case(void);
/********************    Utility Function Declarations    ********************/

/********************************  MAIN  *************************************/

int main(void)
{
	test_case();

	return (0);
}

/***********************    General Functions    *****************************/
void test_case(void)
{
	fsa_t *student_pool = NULL;
	void *inbal_elmalech = NULL;
	void *itay_adi_yosef = NULL;
	void *noa_pilosof = NULL;
	void *gilad_barak = NULL;
	void *rotem_amir = NULL;
	void *janna_torbilo = NULL;
	void *chen_hasid = NULL;
	void *reut_ben_harush = NULL;
	void *ido_finkelstein = NULL;

	size_t student_pool_size = FSARequiredPoolSize(6, 
												   sizeof(student_t));

	printf("size of student_t: %lu\n", sizeof(student_t));

	student_pool = (fsa_t *)malloc(student_pool_size);
	printf("student_pool is: %p\n", (void *)student_pool);
	student_pool = FSAInit(student_pool, student_pool_size, sizeof(student_t));

	printf("Free blocks: %ld\n", FSAFreeBlocks(student_pool));


	inbal_elmalech = FSAAlloc(student_pool);
	printf("blabla\n");
	itay_adi_yosef = FSAAlloc(student_pool);
	noa_pilosof = FSAAlloc(student_pool);
	gilad_barak = FSAAlloc(student_pool);
	rotem_amir = FSAAlloc(student_pool);
	janna_torbilo = FSAAlloc(student_pool);

	printf("Free blocks: %ld\n", FSAFreeBlocks(student_pool));

	FSAFree(rotem_amir);
	FSAFree(gilad_barak);
	FSAFree(itay_adi_yosef);

	printf("Free blocks: %ld\n", FSAFreeBlocks(student_pool));

	chen_hasid = FSAAlloc(student_pool);;
	reut_ben_harush = FSAAlloc(student_pool);;
	ido_finkelstein = FSAAlloc(student_pool);;

	printf("Free blocks: %ld\n", FSAFreeBlocks(student_pool));

	free(student_pool);
	student_pool = NULL;
}

/**************************    Utility Functions    **************************/

void StudentPrint(student_t *student)
{
	printf("Name: %s\n", student->name);
	printf("City: %s\n", student->city);
	printf("Age: %d\n", student->age);
}

/*****************************************************************************/
