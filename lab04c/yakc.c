#include "clib.h"
#include "yakk.h"


/******************** Global Variables ********************/
unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as - 
                                      //  - the dispatching of a task other than the task that ran most recently.
unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
unsigned int YKISRDepth;
unsigned int YKTickNum;
char run_flag = 0;

int idleStack[IDLE_STACK_SIZE];

TCBptr YKRdyList;
TCBptr YKDelayList;
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs*/

TCBptr TKCurrentlyRunning;


/******************** Function Def ********************/

void YKInitialize(void){    // Initializes all required kernel data structures
  int i;
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  TKCurrentlyRunning = 0;   // Set to 0
  
  YKEnterMutex();           // Turn on interupts at initialization
  
  /* code to construct singly linked available TCB list from initial array */ 
  YKAvailTCBList = &(YKTCBArray[0]);
  
  for (i = 0; i < MAXTASKS; i++)
	  YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[MAXTASKS].next = NULL;
	
  YKNewTask(YKIdleTask, (void*)&idleStack[IDLE_STACK_SIZE], LOWEST_PRIORITY);
  //call YKIdleTask         // From YAK Kernel instruction book
  //^ could call YKIdleTask as YKNewTask()
  
  
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKIdleCount=YKIdleCount+1;          // 
    YKExitMutex();
  }                        
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){    // Creates a new task
      
  TCBptr tmp, tmp2;
  
  /* code to grab an unused TCB from the available list */
  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;
  
  // Set the struct var definitions
  tmp->priority = priority;
  tmp->delay = DEFAULT_DELAY;
  
  
  YKEnterMutex();             //Disable interupts

  // Code taken from the example code
  if (YKRdyList == NULL){	/* is this first insertion? */
    YKRdyList = tmp;
    tmp->next = NULL;
    tmp->prev = NULL;
  }
  else{			/* not first insertion */
    tmp2 = YKRdyList;	/* insert in sorted ready list */
    while (tmp2->priority < tmp->priority){
      tmp2 = tmp2->next;	/* assumes idle task is at end */
    }
    if (tmp2->prev == NULL)	/* insert in list before tmp2 */
      YKRdyList = tmp;
    else
      tmp2->prev->next = tmp;
    tmp->prev = tmp2->prev;
    tmp->next = tmp2;
    tmp2->prev = tmp;
  }

  YKExitMutex();              		// starts interrupts
	
  // Saving the stack pointer
  tmp->stackptr = taskStack; 		// from function call
  tmp->ss = 0;
	
  // storeing the context into stack
  tmp->stackptr		= tmp->stackptr - 11;
  *(tmp->stackptr+11)	= 0x200; 	//flag interupt
  *(tmp->stackptr+10)	= 0;		// CS
  *(tmp->stackptr+9)	= (int)task;	// IP
  *(tmp->stackptr+8)	= 0;		// AX
  *(tmp->stackptr+7)	= 0;		// BX
  *(tmp->stackptr+6)	= 0;		// CX
  *(tmp->stackptr+5)	= 0;		// DX
  *(tmp->stackptr+4)	= 0;		// BP
  *(tmp->stackptr+3)	= 0;		// SI
  *(tmp->stackptr+2)	= 0;		// DI
  *(tmp->stackptr+1)	= 0;		// DS
  *(tmp->stackptr+0)	= 0;		// ES	
	
  YKScheduler(SAVE);          // Save current block of mem
  
}

void YKRun(void){                 // Starts actual execution of user code
  run_flag = HIGH;                // Start the Scheduler for the very first time
  YKScheduler(NSAVE);             // run the top proccess 
}

void YKScheduler(int save_flag){     // Determines the highest priority ready task
  TCBptr highest_priority_task = YKRdyList;
  TCBptr currentlyRunning = TKCurrentlyRunning;  
	
  if(!run_flag || (TKCurrentlyRunning == highest_priority_task)){                               // NOT redundant! Tell the kernel to begin for first time
    return;	  
  }
  
  // update YKCtxSwCount
  YKCtxSwCount = YKCtxSwCount + 1;
  TKCurrentlyRunning = highest_priority_task;   
 
  if(!save_flag){
    YKDispatcherNSave(highest_priority_task->stackptr, highest_priority_task->ss);
  }
  else{
    YKDispatcherSave(&(currentlyRunning->stackptr),&(currentlyRunning->ss), 
		     highest_priority_task->stackptr, highest_priority_task->ss);

  }
}
/*
YKDelayTask. Prototype: void YKDelayTask(unsigned count)
- This function delays a task for the specified number of clock ticks. 
- After taking care of all required bookkeeping to mark the change of state for the currently running task, 
this function calls the scheduler. 
- After the specified number of ticks, the kernel will mark the task ready. 
- If the function is called with a count argument of 0 then it should not delay and should simply return. 
This function is called only by tasks, and never by interrupt handlers or ISRs.
*/
void YKDelayTask(unsigned count){
  TCBptr ready;
  YKEnterMutex();

  if(count == 0)
    YKExitMutex();
    return;
  
	
	//Get next TCB from readylist
  ready = YKRdyList;
	//Remove from readylist
  YKRdyList = ready->next;
  ready->next->prev = NULL;
	//Put at top of delay list (which is a doubly-linked list)
  ready->next = YKDelayList;
  YKDelayList = ready;
  ready->prev = NULL;
  if(ready->next != NULL){ // if not empty
    ready-next->prev=ready
  }
  ready->delay = count;
	
  YKScheduler(SAVE);
  YKExitMutex();
}

void YKEnterISR(void){
  YKISRDepth++;
}

void YKExitISR(void){
  YKISRDepth--;
	if(YKISRDepth == 0) {
    YKScheduler(NSAVE);
  }
}

/*
- This function must be called from the tick ISR each time it runs. 
- YKTickHandler is responsible for the bookkeeping required to support the timely reawakening of delayed tasks. 
(If the specified number of clock ticks has occurred, a delayed task is made ready.) 
- The tick ISR may also call a user tick handler if the user code requires actions to be taken on each clock 
tick.
*/
void YKTickHandler(void){
  TCBptr tempDelay, tempReady, tempNext;

  YKEnterMutex();
  YKTickNum++; 
  tempDelay = YKDelayList;
	//While the delay is not finished, counter--; 
  while(tempDelay != NULL){
    tempNext = tempDelay->next
    tempDelay->delay--;
    if(tempDelay->delay == 0){
      // Find ready task in delay list
      if(tempDelay->prev == NULL){
        YKDelayList = tempDelay->next;
      }
      else{
        tempDelay->prev->next = tempDelay->next;
      }
      if(tempDelay->next != NULL){
        tempDelay->next->prev = tempDelay->prev;
      }
      // incert delayed task in ready list 
      tempReady = YKRdyList;
      while(tempReady->priority < tempDelay->priority){ // Find the next lower priority tempReady
        tempReady = tempReady->next;
      }
      if(tempReady->prev == NULL){ // insert tempDelay before the lower priority tempReady
        YKRdyList = tempDelay;
      }
      else{ // otherwise insert normally
        tempReady->prev->next = tempDelay;
      }
      // Clean up ready list
      tempDelay->prev = tempReady->prev;
      tempDelay->next = tempReady;
      tempReady->prev = tempDelay;
    }
    tempDelayed = tempNext;
  }
  YKExitMutex();
}

/******************** Functions in yaks.s ********************/
// Functions are made inside of yaks.s because they are coded in assembly
//void YKDispatcher(void);    // Begins or resumes execution of the next task
//void YKEnterMutex(void);    // Disables interrupts
//void YKExitMutex(void);     // Enables interrupts
  

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
//void YKEventReset(YKEVENT *event, unsigned eventMask)
