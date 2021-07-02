/********************************
* File name: scheduler			*
* Develoer: Inbal Elmalech		*
* Reviewer:  Guy Chen			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h> /* printf, puts */
#include <time.h> /* time */
#include "scheduler.h" /* sched_t, timer_t */
/*****************************   DECLARATIONS   ******************************/

void Test_CreateAndDestroy(void);
void Test_StartTimerCancelTimer(void);
/* void Test_SchedRunSchedStop(void); */

/********************    Utility Function Declarations    ********************/

unique_id_t UidInit(void);
int Every2Sec(sch_t *sch, unique_id_t uid, void *param);
int Every5Sec(sch_t *sch, unique_id_t uid, void *param);
/* void UtilPrintUid(unique_id_t uid); */

/********************************  MAIN  *************************************/

int main(void)
{
	Test_CreateAndDestroy();
	/*Test_StartTimerCancelTimer();*/
	/* Test_SchedRunSchedStop(); */

	return (0);
}

/***********************    General Functions    *****************************/

void Test_CreateAndDestroy(void)
{
	sch_t *new_sched = SchCreate();

	printf("-->> Testing sched_t Create and Destroy <<--\n");

	if (NULL == new_sched)
	{
	printf("SchedCreate failed!\n");
	}

	printf("Created new_sched at %p\n", (void *)new_sched);

	SchDestroy(new_sched);
	printf("new_sched destroyed\n");
}

/*---------------------------------------------------------------------------*/

void Test_StartTimerCancelTimer(void)
{
	
	sch_t *new_sched = SchCreate();
	unique_id_t bad_id = UIDGetBadUID();
	unique_id_t res = UidInit();

	printf("\n-->> Testing start timer and cancel timer <<--\n");

	if (NULL == new_sched)
	{
	printf("SchedCreate failed!\n");
	}

	res = SchTimerStart(new_sched, 2, &Every2Sec, NULL);
	if (UIDIsSame(bad_id, res))
	{
		printf("task1 id is bad id!\n");
		return;
	}
	
	res = SchTimerStart(new_sched, 5, &Every5Sec, NULL);
	if (UIDIsSame(bad_id, res))
	{
		printf("task2 id is bad id!\n");
		return;
	}

	SchRun(new_sched);
	/*sleep(10);
	SchStop(new_sched);
	printf("sched stopped\n");
	SchDestroy(new_sched);
	printf("new_sched destroyed\n");*/
}

/*---------------------------------------------------------------------------*/
#if(0)
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
#endif

/*---------------------------------------------------------------------------*/

int Every2Sec(sch_t *sch, unique_id_t uid, void *param)
{
	(void)sch;
	(void)uid;
	(void)param;
	printf("EVERY 2: %lu\n", time(NULL));

	return (1);
}

/*---------------------------------------------------------------------------*/

int Every5Sec(sch_t *sch, unique_id_t uid, void *param)
{
	(void)sch;
	(void)uid;
	(void)param;
	printf("EVERY 5: %lu\n", time(NULL));

	return (1);
}

/*---------------------------------------------------------------------------*/

unique_id_t UidInit(void)
{
	unique_id_t uid = {0, 0, 0};

	return (uid);
}

/*****************************************************************************/
