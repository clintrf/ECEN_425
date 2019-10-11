// yak.h file for the yak.c file

#ifndef YAKH_H
#define YAKH_H


#define HIGH 1
#define LOW 0

#define SAVE 1
#define NSAVE 0 
#define NULL 0

#define MAXTASKS 5		/* count of user tasks */

// TCB Struct
typedef struct taskblock *TCBptr;
typedef struct taskblock
{				/* the TCB struct definition */
    void *stackptr;		/* pointer to current top of stack */
    int state;			/* current state */
    int priority;		/* current priority */
    int delay;			/* #ticks yet to wait */
    TCBptr next;		/* forward ptr for dbl linked list */
    TCBptr prev;		/* backward ptr for dbl linked list */
}  TCB;

TCBptr YKRdyList;
TCBptr YKSuspList;
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs

/******************** Global Variables ********************/
unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as - 
                                      //  - the dispatching of a task other than the task that ran most recently.
unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
//unsigned int YKTickNum;             // Must be incremented each time the kernel's tick handler runs. For dif lab

unsigned int run_flag;


/******************** Functions in yakc.c ********************/
void YKInitialize(void);                    // Initializes all required kernel data structures
void YKIdleTask(void);                      // Kernel's idle task
void YKNewTask(void (* task)(void),\  
    void *taskStack, \
    unsigned char priority);                // Creates a new task
void YKRun(void);                           // Starts actual execution of user code
//void YKScheduler(void);                   // original Scheduler
void YKScheduler(unsigned int save_flag);   // Pass Scheduler a flag to know if it should save



/******************** Functions in yaks.s ********************/
//void YKDispatcher(void);              // Begins or resumes execution of the next task -
                                        // - Split our dispature into a save dispatcher -
                                        // - and a non save dispatcher
void YKDispatcherSave();                // Dispatcher that saves to mem/stack?
void YKDispatcherNSave();             // Dispatcher that  doesn't saves to mem/stack?


void YKEnterMutex(void);              // Disables interrupts
void YKExitMutex(void);               // Enables interrupts



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


#endif // YAKH_H
//void YKEventReset(YKEVENT *event, unsigned eventMask)
