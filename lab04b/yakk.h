// yak.h file for the yak.c file

#ifndef YAKK_H
#define YAKK_H


#define HIGH 1
#define LOW 0

#define SAVE 1
#define NSAVE 0 

#define NULL 0
#define MAXTASKS 3		/* count of user tasks */


#define TASK_RUNNING 1
#define TASK_READY 2
#define TASK_BLOCKED 3
#define IDLE_STACK_SIZE 256

#define LOWEST_PRIORITY 100
#define HIGHEST_PRIORITY 0

#define DEFAULT_DELAY 0


/******************** Global Variables ********************/
extern unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as - 
                                      //  - the dispatching of a task other than the task that ran most recently.
extern unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
//unsigned int YKTickNum;             // Must be incremented each time the kernel's tick handler runs. For dif lab

extern unsigned int run_flag;

extern unsigned int current_priority;

extern int idleStack[IDLE_STACK_SIZE];

/******************** Global Structs ********************/

// TCB Struct
typedef struct taskblock *TCBptr;
typedef struct taskblock
{				        /* the TCB struct definition */
    //void *stackptr;		/* pointer to current top of stack */
    int *stackptr;
    int *ss;
    int state;			/* current state */
    int priority;		/* current priority */
    int delay;			/* #ticks yet to wait */
    TCBptr next;		/* forward ptr for dbl linked list */
    TCBptr prev;		/* backward ptr for dbl linked list */
}  TCB;

extern TCBptr YKRdyList;
extern TCBptr YKSuspList;
extern TCBptr YKAvailTCBList;		/* a list of available TCBs */
extern TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs

/******************** Functions in yakc.c ********************/
void YKInitialize(void);                    // Initializes all required kernel data structures
void YKEnterMutex(void);              // Disables interrupts
void YKExitMutex(void);               // Enables interrupts

void YKIdleTask(void);                      // Kernel's idle task
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);                // Creates a new task
void YKRun(void);                           // Starts actual execution of user code
//void YKScheduler(void);                   // original Scheduler
void YKScheduler(unsigned int save_flag);   // Pass Scheduler a flag to know if it should save



/******************** Functions in yaks.s ********************/
//void YKDispatcher(void);              // Begins or resumes execution of the next task -
                                        // - Split our dispature into a save dispatcher -
                                        // - and a non save dispatcher
void YKDispatcherSave(int saveFlag, int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss);                // Dispatcher that saves to mem/stack?
void YKDispatcherNSave(int saveFlag,int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss);             // Dispatcher that  doesn't saves to mem/stack?




/******************** Functions not in this lab ********************/
//void YKDelayTask(unsigned count){}
//void YKEnterISR(void){}
//void YKExitISR(void){}
//void YKTickHandler(void){}
//YKSEM* YKSemCreate(int initialValue)
//void YKSemPend(YKSEM *semaphore)
//void YKSemPost(YKSEM *semaphore)
//YKQ *YKQCreate(void **start, unsigned size)
//void *YKQPend(YKQ *queue)
//int YKQPost(YKQ *queue, void *msg)
//YKEVENT *YKEventCreate(unsigned initialValue)
//unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
//void YKEventSet(YKEVENT *event, unsigned eventMask)


#endif // YAKK_H
//void YKEventReset(YKEVENT *event, unsigned eventMask)
