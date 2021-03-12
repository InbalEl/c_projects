/**********************   PREPROCESSOR DIRECTIVES   **************************/
#define _DEFAULT_SOURCE
#include <stdio.h>          /* printf */
#include <string.h>         /* strstr */
#include <stdlib.h>         /* malloc, free, size_t */
#include <errno.h>          /* perror */
#include <signal.h>
#include <pthread.h>        /* pthread_t */
#include <sys/types.h>
#include <sys/wait.h>       /* waitpid */
#include <semaphore.h>      /* sem_t */
#include <fcntl.h>          /* oflag arg in sem_open */
#include <sys/stat.h>       /* sem_t permmission symbols */
#include <sys/ipc.h>

#include "uid.h"            /* uid_t */
#include "scheduler.h"      /* sch_t */
#include "wdlib.h"          /* MMI, DNR */

#define SEND_PULSE_EVERY 1
#define CHECK_PULSE_EVERY 2
#define NOF_HANDLERS 3
/* #define DEBUG_PRINT */

/*****************************   STRUCTURES   ********************************/

static sig_atomic_t sigusr1_received = 0;
static sig_atomic_t is_mmi_active = 0;
struct sigaction sa_arr[NOF_HANDLERS];
char *sem_name_g = "/watchdog_sem";
char *wd_name_g = "./wd.Debug.out";
char *env_var_g = "WATCHDOG";

typedef enum who_am_i
{/* DO NOT CHANGE */
    USER_APP = 0,
    WD
}who_am_i_t;

typedef struct thread_data
{
    who_am_i_t who_am_i;
    pid_t pid_to_signal;
    int is_ancestor;
    int is_waiting;
    char **argv;
    sch_t *sched;
    sem_t *sem;
}th_data_t;

th_data_t th_data_g = {0};

/****************************   DECLARATIONS   *******************************/
/*********************   Sigaction and Signal Handling   *********************/
static int InitSigactions(void);
static void RestoreDefSigactions(void);
static void sigusr1_action(int signal_num, siginfo_t *info, void *context);
static void sigusr2_action(int signal_num, siginfo_t *info, void *context);
static void sigchld_action(int signal_num, siginfo_t *info, void *context);

/**************************   Schedule Managment   ***************************/
static sch_t *CreateAndLaunchSched(void);
static int LoadSched(sch_t *sched);
static int SendPulse(sch_t *sch, unique_id_t uid, void *null);
static int CheckPulse(sch_t *sch, unique_id_t uid, void *null);

/**********************  Process & Thread Managment   ************************/
static pid_t Resuscitate(void);
static void SelfDestruct(void);
static void *LaunchKeepAliveThread(void *thrd_data);

/*******************************   Utils   ***********************************/
static void CopyArgs(char **arg_list, char **argv, int nof_copies);

/*****************************   FUNCTIONS   *********************************/

int MMI(int argc, char **argv)
{
    char **arg_list = NULL;
    pthread_t keepalive_th = 0;

    /* Signal action registration */
    if (0 != InitSigactions())
    {
        return (1);
    }

    is_mmi_active = 1;
    th_data_g.who_am_i = (NULL != strstr(argv[0], wd_name_g))? WD: USER_APP;

    /* Initializing / allocating appropriate arg_list */
    if (th_data_g.who_am_i == USER_APP)
    {
        if (NULL == (arg_list = (char **)malloc((argc + 2) * sizeof(char *))))
        {
            printf("arg_list malloc failed\n");
            return (1);
        }

        arg_list[0] = wd_name_g;
        CopyArgs(arg_list + 1, argv, argc);
        th_data_g.argv = arg_list;
    }
    else
    {
        th_data_g.argv = argv + 1;
    }

    /* Checking if we are in a normal launch
    (if WATCHDOG env var doesn't exist yet) */
    if (NULL == getenv(env_var_g))
    {
        th_data_g.is_ancestor = 1;
        th_data_g.is_waiting = 1;

        if (0 != putenv("WATCHDOG=running"))
        {
            free(th_data_g.argv);
            return (1);
        }

        if (0 > (th_data_g.pid_to_signal = Resuscitate()))
        {
            unsetenv(env_var_g);
            free(th_data_g.argv);
            return (1);
        }
    }

    /* In case WATCHDOG env var already exists */
    else
    {
        th_data_g.is_ancestor = 0;
        th_data_g.is_waiting = 0;
        th_data_g.pid_to_signal = getppid();
    }
    
    /* Creating and loading the tasks schedule */
    if (NULL == (th_data_g.sched = CreateAndLaunchSched()))
    {
        unsetenv(env_var_g);
        if (th_data_g.who_am_i == USER_APP)
        {
            free(th_data_g.argv);
        }
        
        return (1);
    }

    /* Creating / opening the semaphore */
    if (SEM_FAILED ==
        (th_data_g.sem = sem_open(sem_name_g, O_CREAT, 0664, 0)))
    {
        perror("sem_open");

        SchDestroy(th_data_g.sched);
        unsetenv(env_var_g);

        if (th_data_g.who_am_i == USER_APP)
        {
            free(th_data_g.argv);
        }
        return (1);
    }

    pthread_create(&keepalive_th, NULL, &LaunchKeepAliveThread, NULL);

    return (0);
}

/*---------------------------------------------------------------------------*/

int DNR(void)
{
    if (0 != kill(th_data_g.pid_to_signal ,SIGUSR2))
    {
        perror("kill");
        return (1);
    }
    is_mmi_active = 0;

    return (0);
}

/*********************   Sigaction and Signal Handling   *********************/

static int InitSigactions(void)
{
    struct sigaction sa;
    sa.sa_sigaction = sigusr1_action;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (0 != sigaction(SIGUSR1, &sa, sa_arr))
    {
        return (1);
    }
 
    sa.sa_sigaction = sigusr2_action;
    if (0 != sigaction(SIGUSR2, &sa, sa_arr + 1))
    {
        sigaction(SIGUSR1, sa_arr, NULL);
        return (1);
    }

    sa.sa_sigaction = sigchld_action;
    if (0 != sigaction(SIGCHLD, &sa, sa_arr + 2))
    {
        sigaction(SIGUSR1, sa_arr, NULL);
        sigaction(SIGUSR2, sa_arr + 1, NULL);
        return (1);
    }
    return (0);
}

/*---------------------------------------------------------------------------*/

static void RestoreDefSigactions(void)
{
    sigaction(SIGUSR1, sa_arr, NULL);
    sigaction(SIGUSR2, sa_arr + 1, NULL);
    sigaction(SIGCHLD, sa_arr + 2, NULL);
}

/*---------------------------------------------------------------------------*/

static void sigusr1_action(int signal_num, siginfo_t *info, void *context)
{
    (void)signal_num;
    (void)context;
    (void)info;

    sigusr1_received = 1;


    #ifdef DEBUG_PRINT
    printf("%d received a pulse!\n", (int)getpid());
    #endif
}

/*---------------------------------------------------------------------------*/

static void sigusr2_action(int signal_num, siginfo_t *info, void *context)
{
    (void)signal_num;
    (void)context;
    (void)info;

    is_mmi_active = 0;
}

/*---------------------------------------------------------------------------*/

static void sigchld_action(int signal_num, siginfo_t *info, void *context)
{
    (void)signal_num;
    (void)context;
    (void)info;

    waitpid(info->si_pid, NULL, WNOHANG);
}

/*************************   Schedule Management   ***************************/

static sch_t *CreateAndLaunchSched(void)
{
    sch_t *task_sched = SchCreate();

    if (NULL == task_sched)
    {
        return (NULL);
    }

    if (0 != LoadSched(task_sched))
    {
        SchDestroy(task_sched);
        return (NULL);
    }
    return (task_sched);
}

/*---------------------------------------------------------------------------*/

static int LoadSched(sch_t *sched)
{
    unique_id_t bad_id = UIDGetBadUID();

    if (UIDIsSame(bad_id,
                  SchTimerStart(sched, SEND_PULSE_EVERY,
                                &SendPulse, NULL)))
    {
        return (1);
    }

    if (UIDIsSame(bad_id,
                  SchTimerStart(sched, CHECK_PULSE_EVERY,
                                &CheckPulse, NULL)))
    {
        return (1);
    }

    return (0);
}

/*---------------------------------------------------------------------------*/

static int SendPulse(sch_t *sch, unique_id_t uid, void *null)
{
    (void)sch;
    (void)uid;
    (void)null;

    /* if dnr has been called */
    if (!is_mmi_active)
    {
        if (th_data_g.who_am_i == USER_APP)
        {
            sem_wait(th_data_g.sem);
            /* missing a wait() if there's no sigchld handler */
            SelfDestruct();
        }
        else
        {
            sem_post(th_data_g.sem);
            SelfDestruct();
        }
    }
    
    /* if mmi still active */
    if (0 != kill(th_data_g.pid_to_signal ,SIGUSR1))
    {
        perror("kill");
    }

    #ifdef DEBUG_PRINT
    if (th_data_g.who_am_i == USER_APP)
    {
        printf("User app [%d] Sent pulse to [%d]\n", getpid(),
               th_data_g.pid_to_signal);
    }
    else
    {
        printf("WD [%d] Sent pulse to [%d]\n", getpid(),
               th_data_g.pid_to_signal);
    }
    
    #endif
    return (1);
}

/*---------------------------------------------------------------------------*/

static int CheckPulse(sch_t *sch, unique_id_t uid, void *null)
{
    (void)sch;
    (void)uid;
    (void)null;

    
    /* if flag is on turn off */
    if (sigusr1_received)
    {
        sigusr1_received = 0;
    }

    /* if it's off - resuscitate */
    else if (is_mmi_active)
    {
        th_data_g.pid_to_signal = Resuscitate();
        sem_wait(th_data_g.sem);
    }
    
    return (1);
}

/**********************  Process & Thread Managment   ************************/

static pid_t Resuscitate(void)
{
    int child_pid = fork();

    /* fork error */
    if (0 > child_pid)
    {
        perror("Error: fork");
    }
    
    /* inside child */
    else if (0 == child_pid)
    {
        execv(th_data_g.argv[0], th_data_g.argv);
        perror("Error: evecv");
        abort();
    }

    return (child_pid);
}

/*---------------------------------------------------------------------------*/

static void SelfDestruct(void)
{
    SchStop(th_data_g.sched);
    SchDestroy(th_data_g.sched);

    if (th_data_g.who_am_i == USER_APP)
    {
        /* free signal handlers */
        unsetenv(env_var_g);
        free(th_data_g.argv);
        th_data_g.argv = NULL;

        sem_close(th_data_g.sem);
        sem_unlink(sem_name_g);
        RestoreDefSigactions();
        pthread_exit(NULL);
    }

    else
    {
        RestoreDefSigactions();
        exit(0);
    }
    
}

/*---------------------------------------------------------------------------*/

static void *LaunchKeepAliveThread(void *null)
{
    if (th_data_g.is_waiting)
    {
        sem_wait(th_data_g.sem);
    }

    else
    {
        sem_post(th_data_g.sem);
    }

    SchRun(th_data_g.sched);

    return (null);
}

/*******************************   Utils   ***********************************/

static void CopyArgs(char **arg_list, char **argv, int nof_copies)
{
    int i = 0;

    while (i < nof_copies)
    {
        arg_list[i] = argv[i];
        ++i;
    }
    
    arg_list[i] = NULL;
}

/*****************************************************************************/
