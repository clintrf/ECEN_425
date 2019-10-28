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
extern unsigned int YKTickNum;
extern unsigned int YKISRDepth;

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
extern TCBptr YKDelayList;
extern TCBptr YKAvailTCBList;		/* a list of available TCBs */
extern TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs

/******************** Functions in yakc.c ********************/
void YKInitialize(void);                    // Initializes all required kernel data structures
void YKIdleTask(void);                      // Kernel's idle task
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);                // Creates a new task
void YKRun(void);                           // Starts actual execution of user code
//void YKScheduler(void);                   // original Scheduler
void YKScheduler(int save_flag);   // Pass Scheduler a flag to know if it should save
void YKDelayTask(unsigned count);  // Function call to delay the current task... count is for how long
void YKEnterISR(void);             // Enters the ISR and increments the counter for how deap it is
void YKExitISR(void);              // Exits the ISR and decrements the counter for how deap it is
void YKTickHandler(void);is.       // handles the ticks


/******************** Functions in yaks.s ********************/
void YKEnterMutex(void);              // Disables interrupts
void YKExitMutex(void);               // Enables interrupts
void YKDispatcherNSave(int *restore_sp, int * restore_ss); 
void YKDispatcherSave(int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss); 



/******************** Functions not in this lab ********************/
//YKSEM* YKSemCreate(int initialValue)
//void YKSemPend(YKSEM *semaphore)
//void YKSemPost(YKSEM *semaphore)
//YKQ *YKQCreate(void **start, unsigned size)
//void *YKQPend(YKQ *queue)
//int YKQPost(YKQ *queue, void *msg)
//YKEVENT *YKEventCreate(unsigned initialValue)
//unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
//void YKEventSet(YKEVENT *event, unsigned eventMask)


#endif

