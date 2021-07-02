/*
*	File:			scheduler.c
*	Description:	Implementing API functions for scheduler
*	Developer:		Noa Pilosof
*	Reviewer:	 	Rotem Amir
*	Date:			TBD
*/

#include <stdlib.h>	/* malloc, free */
#include <assert.h>	/* assert */
#include <unistd.h>	/* sleep */
#include <stdio.h>
#include "scheduler.h"
#include "pqueue.h"


#define SUCCESS 0
#define FAIL 1
#define TRUE 1
#define FALSE 0

/* custom structures */
struct scheduler
{
	pqueue_t *sch_pqueue;
	int sch_run;
};


typedef struct task_timer
{
	unique_id_t timer_uid;
	size_t interval;
	size_t next_run;
	sch_task_func_t task_func;
	void *task_param;
}task_timer_t;

/*---------------------------------------------------------------------------*/

/* utility functions declarations */
static int SchCmp(const void *data1, const void *data2, void *param);
static task_timer_t *SchCreateTimer(size_t interval_in_sec,
                             sch_task_func_t task,
                             void* param);
static int SchIsMatch(const void *data1, const void *data2, 
                                      void *param);                             
unique_id_t SchTimerInit(void);
static void SchFreeTimer(task_timer_t *timer);
static int SchAddTimer(sch_t *sch, task_timer_t *timer);

/*---------------------------------------------------------------------------*/

/* API functions */

sch_t *SchCreate(void)
{
	sch_t *new_schedule = NULL;
	pqueue_cmp_func_t cmp_func = NULL;
	pqueue_t *sch_pq = NULL;
	void *param_to_pq = NULL;
	
	cmp_func = &SchCmp;
	
	/* allocating memory for scheduler */
	new_schedule = (sch_t *)malloc (sizeof(sch_t));
	if (NULL == new_schedule)
	{
		return (NULL);
	}
	
	/* creating pqueue for the scheduler */
	sch_pq = PQCreate(cmp_func, param_to_pq);
	if (NULL == sch_pq)
	{
		free(new_schedule);
		new_schedule = NULL;
		
		return (NULL);
	}
	
	/* initializing scheduler */
	new_schedule->sch_pqueue = sch_pq;
	
	return (new_schedule); 
}

/*---------------------------------------------------------------------------*/

void SchDestroy(sch_t *sch)
{
	task_timer_t *data_to_free = NULL;
	
	assert(sch);
	
	/* destroying pqueue */
	while (!PQIsEmpty(sch->sch_pqueue))
	{
		data_to_free = (task_timer_t *)PQDequeue(sch->sch_pqueue);
		SchFreeTimer(data_to_free);
	}
	
	PQDestroy(sch->sch_pqueue);
	
	/* freeing scheduler */
	free(sch);
	sch = NULL;
}

/*---------------------------------------------------------------------------*/

unique_id_t SchTimerStart(sch_t *sch, 
                                size_t interval_in_sec,
                                sch_task_func_t task,
                                void* param)
{
	task_timer_t *new_timer = NULL;
	
	assert(sch);
	assert(task);
	
	/* allocating memory and initializing new timer */
	new_timer = SchCreateTimer(interval_in_sec, task, param);
	if (!new_timer)
	{
		return (UIDGetBadUID());
	}

	/* Enqueing new timer */
	if(SchAddTimer(sch, new_timer))
	{
		return (UIDGetBadUID());
	}
	
	return (new_timer->timer_uid);
}

/*---------------------------------------------------------------------------*/

int SchRun(sch_t *sch)
{
	task_timer_t *timer_run = NULL;
	int task_status = 0;
	unsigned int sleep_interval = 0;
	static unsigned int last_run_sec = 0;
	
	assert(sch);
	
	sch->sch_run = TRUE;
	
	while(!PQIsEmpty(sch->sch_pqueue) && TRUE == sch->sch_run)
	{
		/*printf("pq size %lu\n", PQSize(sch->sch_pqueue)); */

		/*timer_run = (task_timer_t *)PQPeek(sch->sch_pqueue);*/
		/* dequeue task in line */
		timer_run = (task_timer_t *)PQDequeue(sch->sch_pqueue);
		
		sleep_interval = (unsigned int)timer_run->next_run - last_run_sec;
		/*printf("timer run next run: %lu\n", timer_run->next_run);
		printf("last run sec: %lu\n", last_run_sec);
		printf("sleeping for: %lu\n", sleep_interval);
		printf("\n");*/
		
		sleep(sleep_interval);
		
		last_run_sec = (unsigned int)timer_run->next_run;
		
		task_status = timer_run->task_func(sch, timer_run->timer_uid, 
										   timer_run->task_param);
		
		/* in case task fails, free timer and return FAIL */
		if (-1 == task_status)
		{
			last_run_sec = 0;
			SchFreeTimer(timer_run);
			
			return (FAIL);
		}
		
		/* in case task repeats, enqueue it again with new time to run */
		else if (1 == task_status)
		{
			/*printf("rescheduling\n");*/
			
			timer_run->next_run += timer_run->interval;
			
			if(SchAddTimer(sch, timer_run))
			{
				return (FAIL);
			}
		}	
		
		/* in case task finishes and ends, free timer */
		else 
		{
			SchFreeTimer(timer_run);
		}
	}
	
	last_run_sec = 0;
	
	return (SUCCESS);
}

/*---------------------------------------------------------------------------*/

void SchTimerCancel(sch_t *sch, unique_id_t uid)
{
	pqueue_is_match_func_t is_match = NULL;
	unique_id_t *uid_p = NULL;
	unique_id_t uid_to_check = uid;
	task_timer_t *data_erased = NULL;
	
	assert(sch);
	
	uid_p = &uid_to_check;
	is_match = (pqueue_is_match_func_t)&SchIsMatch;
	
	data_erased = (task_timer_t *)PQErase(sch->sch_pqueue, uid_p, is_match, 
										  NULL);
	
	SchFreeTimer(data_erased);
}

/*---------------------------------------------------------------------------*/

int SchStop(sch_t *sch)
{
	assert(sch);
	sch->sch_run = FALSE;
	
	return (SUCCESS);
}

/*---------------------------------------------------------------------------*/

/* utility functions */

static int SchCmp(const void *data1, const void *data2, void *param)
{
	const task_timer_t *timer_in_node = *(task_timer_t **)data1;
    const task_timer_t *timer_to_add = *(task_timer_t **)data2;

    (void)param;

    if(timer_in_node->next_run < timer_to_add->next_run)
    {
        return (1);
    }
    else if(timer_in_node->next_run > timer_to_add->next_run)
    {
        return (-1);
    }

    return (0);

	#if 0

	ssize_t time_in_node = 0;
	ssize_t time_to_add = 0;
	task_timer_t *timer_in_node = NULL;
	task_timer_t *timer_to_add = NULL;

	task_timer_t **timer_in_node_p = NULL;
	task_timer_t **timer_to_add_p = NULL;

	
	(void)param;
	
	timer_to_add_p = (task_timer_t **)data1;
	timer_in_node_p = (task_timer_t **)data2; 

	timer_to_add = *timer_to_add_p;
	timer_in_node = *timer_in_node_p; 
	
	time_to_add = timer_to_add->next_run;
	time_in_node = timer_in_node->next_run;
	
	printf("timer to add %lu\n", time_to_add);
	printf("timer in node %lu\n", timer_in_node);

	if (time_to_add < time_in_node)
	{
		return (-1);
	}
	
	else if (time_to_add == time_in_node)
	{
		return (0);
	}
	
	else
	{
		return (1);
	}
	#endif
}

/*---------------------------------------------------------------------------*/

static int SchIsMatch(const void *data1, const void *data2,
                                      void *param)
{
	task_timer_t *timer_to_check = NULL;
	unique_id_t *uid_to_cmp = NULL;

	(void)param;

	timer_to_check = (task_timer_t *)data1;
	uid_to_cmp = (unique_id_t *)data2;

	return(UIDIsSame(timer_to_check->timer_uid, *uid_to_cmp));
}

/*---------------------------------------------------------------------------*/

static task_timer_t *SchCreateTimer(size_t interval_in_sec,
                             sch_task_func_t task,
                             void* param)
{
	task_timer_t *timer = NULL;
	timer = (task_timer_t *)malloc (sizeof(task_timer_t));
	if (!timer)
	{
		return (NULL);
	}
	
	/* initializing timer */
	timer->timer_uid = UIDCreate();
	if(UIDIsSame(timer->timer_uid, UIDGetBadUID()))
	{
		return (NULL);
	}
	
	timer->interval = interval_in_sec;
	timer->next_run = interval_in_sec;
	timer->task_func = task;
	timer->task_param = param;
	
	return (timer);
}

/*---------------------------------------------------------------------------*/

unique_id_t SchTimerInit(void)
{
	unique_id_t timer;
	
	timer.counter = 0;
	timer.pid = 0;
	timer.time = 0;
	
	return (timer);
}

/*---------------------------------------------------------------------------*/

static void SchFreeTimer(task_timer_t *timer)
{
	free(timer);
	timer = NULL;
}

/*---------------------------------------------------------------------------*/

static int SchAddTimer(sch_t *sch, task_timer_t *timer)
{
	return (PQEnqueue(sch->sch_pqueue, timer));
}
/*---------------------------------------------------------------------------*/
