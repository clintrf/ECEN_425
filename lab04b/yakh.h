// yak.h file for the yak.c file

#ifndef YAKH_H
#define YAKH_H


/******************** Global Variables ********************/
unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as - 
                                      //  - the dispatching of a task other than the task that ran most recently.
unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
//unsigned int YKTickNum;             // Must be incremented each time the kernel's tick handler runs. For dif lab


/******************** Functions in yakc.c ********************/
void YKInitialize(void);              // Initializes all required kernel data structures
void YKIdleTask(void);                // Kernel's idle task
void YKNewTask(void (* task)(void),\  // Creates a new task
void *taskStack, \
unsigned char priority);       
void YKRun(void);                     // Starts actual execution of user code
void YKScheduler(void);               // Determines the highest priority ready task


/******************** Functions in yaks.s ********************/
void YKDispatcher(void);              // Begins or resumes execution of the next task
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
