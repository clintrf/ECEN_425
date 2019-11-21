// yak.h file for the yak.c file
#ifndef YAKK_H
#define YAKK_H

#define NULL 0
#define MAXTASKS 10
#define SEM_COUNT 10
#define QUE_COUNT 2
#define EVENT_COUNT 2


/******************** Global Variables ********************/
extern unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as -
                                             //  - the dispatching of a task other than the task that ran most recently.
extern unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
extern unsigned int YKTickNum;

/******************** Global Structs ********************/
typedef struct YKEVENT
{				 
  int active;
  int flag;
} YKEVENT;

typedef struct YKQ
{				        
    int size;           // max number of entries in the queue
    int cur_length;     // number of entries currently in the queue
    void** base_addr;   // base address of queue
    int head;    // next location to remove from
    int tail;    // Next location to insert at
}  YKQ;

typedef struct YKSEM
{				       
    int val;       // indicates if semaphore has is ready
    int active;    // indicates if semaphore has been initialezed
    int id;
}  YKSEM;

// TCB Struct
typedef struct taskblock *TCBptr;
typedef struct taskblock
{				        /* the TCB struct definition */
    //void *stackptr;		/* pointer to current top of stack */
    int *stackptr;
    int state;			/* current state */
    int priority;		/* current priority */
    int delay;			/* #ticks yet to wait */
    TCBptr next;		/* forward ptr for dbl linked list */
    TCBptr prev;		/* backward ptr for dbl linked list */
    YKSEM *semWait;      /* semaphore the task is waiting for. NULL if not waiting, lets make it a pointer*/
    YKQ *queueWait;
  
    YKEVENT *event;
    unsigned eventMask;
    int waitMode;
}  TCB;


extern TCBptr YKRdyList;
extern TCBptr YKDelayList;
extern TCBptr YKSemWaitList;
extern TCBptr YKAvailTCBList;		/* a list of available TCBs */
extern TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs*/

/******************** Functions in yakc.c ********************/
void YKInitialize(void);                    // Initializes all required kernel data structures
void YKIdleTask(void);                      // Kernel's idle task
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);                // Creates a new task
void YKRun(void);                           // Starts actual execution of user code

void YKScheduler(int save_flag);   // Pass Scheduler a flag to know if it should save
void YKDelayTask(unsigned count);  // Function call to delay the current task... count is for how long
void YKEnterISR(void);             // Enters the ISR and increments the counter for how deap it is
void YKExitISR(void);              // Exits the ISR and decrements the counter for how deap it is
void YKTickHandler(void);       // handles the ticks

YKEVENT *YKEventCreate(unsigned initialValue);
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);
void YKEventSet(YKEVENT *event, unsigned eventMask);
YKEventReset. Prototype: void YKEventReset(YKEVENT *event, unsigned eventMask);

YKQ *YKQCreate(void **start, unsigned size);
void *YKQPend(YKQ *queue);
int YKQPost(YKQ *queue, void *msg);

YKSEM* YKSemCreate(int initialValue);
void YKSemPend(YKSEM *semaphore);
void YKSemPost(YKSEM *semaphore);

/******************** Functions in yaks.s ********************/
void YKEnterMutex(void);              // Disables interrupts
void YKExitMutex(void);               // Enables interrupts
void YKDispatcherNSave(int *restore_sp);
void YKDispatcherSave(int ** save_sp, int *restore_sp);



/******************** Functions not in this lab ********************/

#endif
