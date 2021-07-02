/********************************
* File name: scheduler			*
* Develoer: Inbal Elmalech		*
* Reviewer:  Guy Chen			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h> /* printf, puts */
#include "scheduler.h" /* sched_t, timer_t */
/*****************************   DECLARATIONS   ******************************/

void Test_CreateAndDestroy(void);
void Test_StartTimerCancelTimer(void);
void Test_SchedRunSchedStop(void);
/* TODO: THERE SHOULD BE NO REFERENCE TO UID HERE! */
/********************    Utility Function Declarations    ********************/

/* TODO: to be removed 
typedef int (*sch_task_func_t)(sch_t *sch, unique_id_t uid, void *param);
*/


unique_id_t UidInit(void);
void UtilPrintUid(unique_id_t uid);

/********************************  MAIN  *************************************/

int main(void)
{
	Test_CreateAndDestroy();
	Test_StartTimerCancelTimer();
	Test_SchedRunSchedStop();

	return (0);
}

/***********************    General Functions    *****************************/

void Test_CreateAndDestroy(void)
{
	sch_t *new_sched = SchCreate();

	printf("\n-->> Testing sched_t Create and Destroy <<--\n");

	if (NULL == new_sched)
	{
	printf("SchedCreate failed!\n");
	}

	printf("Created new_sched at %p\n", (void *)new_sched);

	SchDestory(new_sched);
	printf("new_sched destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_StartTimerCancelTimer(void)
{
	sch_t *new_sched = SchCreate();
	unique_id_t timer1 = UidInit();
	unique_id_t bad_id = UIDGetBadUID();

	printf("\n-->> Testing start timer and cancel timer <<--\n");

	if (NULL == new_sched)
	{
	printf("SchedCreate failed!\n");
	}

	printf("Created new_sched at %p\n", (void *)new_sched);

	timer1 = SchStartTimer(new_sched, 30, task1, param);

	if (UIDIsSame(bad_id, timer1))
	{
		printf("Timer id is bad id!\n");
		return;
	}

	printf("uid of new task is:\n");
	UtilPrintTimer(timer1);

	SchCancelTimer(new_sched, timer1);
	printf("timer has been canceled\n");

	SchDestory(new_sched);
	printf("new_sched destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_SchedRunSchedStop(void)
{
	sch_t *parklife_sched = SchCreate();
	unique_id_t timer1 = TimerInit();
	unique_id_t timer2 = TimerInit();
	unique_id_t timer3 = TimerInit();
	unique_id_t timer4 = TimerInit();

	printf("\n-->> Testing sched run and sched stop <<--\n");

	if (NULL == parklife_sched)
	{
	printf("SchedCreate failed!\n");
	}

	printf("Created parklife_sched at %p\n", (void *)parklife_sched);

	timer1 = SchStartTimer(parklife_sched, 1, task1, param1);
	timer2 = SchStartTimer(parklife_sched, 2, task2, param2);
	timer3 = SchStartTimer(parklife_sched, 5, task3, param3);
	timer4 = SchStartTimer(parklife_sched, 10, task4, param4);

	if (UIDIsSame(bad_id, timer1) ||
		UIDIsSame(bad_id, timer2) ||
		UIDIsSame(bad_id, timer3) ||
		UIDIsSame(bad_id, timer4))
	{
		printf("Timer id is bad id!\n");
		return;
	}

	SchRun(parklife_sched);

	SchStop(parklife_sched);

	SchDestory(parklife_sched);
	printf("parklife_sched destroyed\n");
}

/*****************************************************************************/

void UtilPrintUid(unique_id_t uid)
{
	/* TODO: STUB */
	printf("stub print uid func - to be written\n");
}

/*---------------------------------------------------------------------------*/

unique_id_t UidInit(void)
{
	unique_id_t uid = {0, 0, 0};

	return (uid);
}

/*****************************************************************************/
