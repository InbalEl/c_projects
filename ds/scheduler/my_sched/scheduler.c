/********************************
* File name: scheduler			*
* Develoer: Inbal Elmalech		*
* Reviewer:  Guy Chen			*
********************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h> /* malloc, free. size_t */
#include <assert.h> /* assert */
#include <stdbool.h> /* true, false */
#include <time.h> /* time_t */
#include "scheduler.h" /* sch_t */
#include "pqueue.h" /* pqueue */
#include "uid.h" /* unique_id_t */
/***************************   ENUMS & STRUCTURES  ***************************/
struct scheduler
{
	pqueue_t *sch_pq;
	int is_running;
};

/*---------------------------------------------------------------------------*/
typedef struct sch_timer
{
	unique_id_t task_uid;
	sch_task_func_t task_func;
	void *task_param;
	size_t interval;
	size_t next_run;
}
task_timer_t;

/*---------------------------------------------------------------------------*/
enum task_stat
{
	TASK_FAIL = -1,
	TASK_SUCCESS,
	RERUN_TASK
};

/********************    Utility Functions Declaration   *********************/
static int compare_time(const void *data1,const void *data2, void *param);
int cmp_next_run(size_t time1, size_t time2);
static int SchAddTimer(sch_t *sch, task_timer_t *curr_task);
static void EmptySch(sch_t *sch);
int IsUidMatch(const void *data1, const void *data2, void *param);
#ifdef PRINT
static void PrintUid(unique_id_t uid);
#endif
/*************************    General Functions    ***************************/

sch_t *SchCreate(void)
{
	void *pq_param = NULL;
	sch_t *new_sched =  (sch_t *) malloc(sizeof(sch_t));

	if (!new_sched)
	{
		return (NULL);
	}

	new_sched->sch_pq =	PQCreate(compare_time, pq_param);

	if (!new_sched->sch_pq)
	{
		free(new_sched);
		new_sched = NULL;

		return (NULL);
	}

	return (new_sched);
}

/*---------------------------------------------------------------------------*/

void SchDestroy(sch_t *sch)
{
	assert(sch);

	EmptySch(sch);
	PQDestroy(sch->sch_pq);

	free(sch);
	sch = NULL;
}

/*---------------------------------------------------------------------------*/

int SchRun(sch_t *sch)
{
	task_timer_t *curr_task = NULL;
	int curr_task_stat = 0;
	size_t nap_time = 0;

	assert(sch);

	sch->is_running = 1;

	while (((-1) != curr_task_stat) &&
		   (sch->is_running) && (!PQIsEmpty(sch->sch_pq)))
	{
		curr_task = (task_timer_t *)PQDequeue(sch->sch_pq);

		#ifdef PRINT
		PrintUid(curr_task->task_uid);
		printf(" next_run before sleep: %lu\n", curr_task->next_run);
		#endif

		nap_time = curr_task->next_run - time(NULL);

		#ifdef PRINT
		PrintUid(curr_task->task_uid);
		printf(" nap time: %lu\n", nap_time);
		#endif

		/* if sleep is needed, sleep until next task's next run */
		while (0 < nap_time)
		{
			nap_time = sleep(nap_time);
		}

		if (RERUN_TASK ==
		(curr_task_stat = curr_task->task_func(sch, curr_task->task_uid,
											   curr_task->task_param)))
		{
			curr_task->next_run += curr_task->interval;

			#ifdef PRINT
			PrintUid(curr_task->task_uid);
			printf(" new run time = %lu\n", curr_task->next_run);
			#endif

			if (0 != SchAddTimer(sch, curr_task))
			{
				free(curr_task);
				curr_task = NULL;
				curr_task_stat = (-1);
				sch->is_running = TASK_FAIL;
			}
		}

		else
		{
			free(curr_task);
			curr_task = NULL;
		}
	}

	sch->is_running = 0;

	return (curr_task_stat);
}

/*---------------------------------------------------------------------------*/

int SchStop(sch_t *sch)
{
	assert(sch);
	sch->is_running = 0;
	EmptySch(sch);

	return (0);
}

/*---------------------------------------------------------------------------*/

unique_id_t SchTimerStart(sch_t *sch, size_t interval_in_sec,
						  sch_task_func_t task, void* param)
{
	unique_id_t bad_uid = UIDGetBadUID();
	task_timer_t *new_timer = NULL;

	assert(sch);
	assert(task);

	if (NULL == (new_timer = (task_timer_t *) malloc(sizeof(task_timer_t))))
	{
		return (bad_uid);
	}

	if (UIDIsSame(bad_uid, (new_timer->task_uid = UIDCreate())))
	{
		free(new_timer);
		return (bad_uid);
	}

	new_timer->task_func = task;
	new_timer->task_param = param;
	new_timer->interval = interval_in_sec;
	new_timer->next_run = time(NULL) + new_timer->interval;

	if (SchAddTimer(sch, (void *)new_timer))
	{
		free(new_timer);
		return (bad_uid);
	}

	return (new_timer->task_uid);
}

/*---------------------------------------------------------------------------*/

void SchTimerCancel(sch_t *sch, unique_id_t uid)
{
	task_timer_t *to_remove = NULL;

	assert(sch);

	to_remove = PQErase(sch->sch_pq, &uid,
						(pqueue_is_match_func_t)&IsUidMatch, NULL);

	free(to_remove);
	to_remove = NULL;
}

/*************************    Utility Functions    ***************************/

int IsUidMatch(const void *data1, const void *data2, void *param)
{
	task_timer_t *timer_to_check = NULL;
	unique_id_t *uid_to_cmp = NULL;

	/*(void)param;*/

	timer_to_check = (task_timer_t *)data1;
	uid_to_cmp = (unique_id_t *)data2;

	return(UIDIsSame(timer_to_check->task_uid, *uid_to_cmp));
}

/*---------------------------------------------------------------------------*/

static int compare_time(const void *data1,const void *data2, void *param)
{
	task_timer_t *task1 = *(task_timer_t **)data1;
	task_timer_t *task2 = *(task_timer_t **)data2;

	assert(data1);
	assert(data2);

	(void)param;

    if(task1->next_run < task2->next_run)
    {
        return (1);
    }

    else if(task1->next_run > task2->next_run)
    {
        return (-1);
    }

    return (0);
}

/*---------------------------------------------------------------------------*/
#if(0)
int cmp_next_run(size_t time1, size_t time2)
{
	return((1 * (time1 > time2)) + ((-1) * (time1 < time2)));
}
#endif
/*---------------------------------------------------------------------------*/

static int SchAddTimer(sch_t *sch, task_timer_t *curr_task)
{
	return (PQEnqueue(sch->sch_pq, (void *)curr_task));
}

/*---------------------------------------------------------------------------*/

static void EmptySch(sch_t *sch)
{
	void *data = NULL;

	while (!PQIsEmpty(sch->sch_pq))
	{
		data = PQDequeue(sch->sch_pq);
		free(data);
		data = NULL;
	}
}

/*---------------------------------------------------------------------------*/
#ifdef PRINT
static void PrintUid(unique_id_t uid)
{
	printf("{%lu}{%d}{%lu}", uid.counter, uid.pid, uid.time);
}
#endif
/*****************************************************************************/
