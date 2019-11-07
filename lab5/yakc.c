#include "clib.h"
#include "yakk.h"

int idleStack[256];

/******************** Global Variables ********************/
unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as -
                                      //  - the dispatching of a task other than the task that ran most recently.
unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
unsigned int YKTickNum;
unsigned int YKISRDepth;
//unsigned int YKSemCount;

TCBptr YKRdyList;
TCBptr YKDelayList;
//TCBptr YKSuspList;
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs*/

TCBptr TKCurrentlyRunning;

YKSEM YKSemArray[SEM_COUNT]; // Not sure how large this array should be, change it if needed
TCBptr YKSemWaitList;        // List of the semaphores currently waiting

char run_flag = 0;

void YKInitialize(void){    // Initializes all required kernel data structures
  int i;
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  TKCurrentlyRunning = 0;   // Set to 0
  YKISRDepth = 0;
  //YKSemCount = 0;

  
  YKEnterMutex();

  /* code to construct singly linked available TCB list from initial array */
  YKAvailTCBList = &(YKTCBArray[0]);

  for (i = 0; i < MAXTASKS; i++)
	  YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[MAXTASKS].next = NULL;
  
  for (i = 0; i < SEM_COUNT; i++){ 
    YKSemArray[i].val = -10;           // init the value of the semaphore
    YKSemArray[i].active = 0;        // init if the semaphore has been activated (has not)
    YKSemArray[i].id = i;            // init the Id of the semaphore so we can track it / debug
  }

  YKNewTask(YKIdleTask, (void*)&idleStack[256], 100);
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKIdleCount=YKIdleCount+1;          //
    YKExitMutex();
  }
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){    // Creates a new task
  TCBptr tmp, tmp2;
  int i;

  YKEnterMutex();             //Disable interupts
	
  taskStack = ((int *)taskStack) - 1;
	
  /* code to grab an unused TCB from the available list */
  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;

  // Set the struct var definitions
  tmp->priority = priority;
  tmp->delay = 0;

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

  // Saving the stack pointer
  tmp->stackptr = taskStack; 		// from function call

  // storeing the context into stack
  tmp->stackptr		= tmp->stackptr - 11;
  for(i=11; i>=0;i--){
    if(i == 11){
      *(tmp->stackptr+i)	= 0x200; 	//flag interupt
    }
    else if(i == 10){
      *(tmp->stackptr+i)	= 0;	// CS
    }
    else if(i == 9){
      *(tmp->stackptr+i)	= (int)task;	// IP
    }
    else{
      *(tmp->stackptr+i)	= 0;		// AX,BX,CX,DX,BP,SI,DI,DS,ES
    }

  }

  YKScheduler(1);          // Save current block of mem
  YKExitMutex();              		// starts interrupts
}

void YKRun(void){                 // Starts actual execution of user code
	printString("Start Run and call scheduler\n");
  run_flag = 1;                // Start the Scheduler for the very first time
  YKScheduler(0);             // run the top proccess
}

void YKScheduler(int save_flag){     // Determines the highest priority ready task
  int testVar;
  int* testPt;
  TCBptr highest_priority_task;
  TCBptr currentlyRunning;

  YKEnterMutex();
  //printString("Entering Scheduler\n\r");
  highest_priority_task = YKRdyList;
  currentlyRunning = TKCurrentlyRunning;

  if(!run_flag || (TKCurrentlyRunning == highest_priority_task)){                               // NOT redundant! Tell the kernel to begin for first time
    return;
  }
  // update YKCtxSwCount
  YKCtxSwCount = YKCtxSwCount + 1;
  TKCurrentlyRunning = highest_priority_task;
  if(!save_flag){
    //printString("NONSAVE\n\r");
    YKDispatcherNSave(highest_priority_task->stackptr);
    //printString("EXIT NONSAVE DISPATCHER\n\r");
  }
  else{
    //printString("SAVE\n\r");
    YKDispatcherSave(&(currentlyRunning->stackptr), highest_priority_task->stackptr);
    //printString("EXIT SAVE DISPATCHER\n\r");
  }
  YKExitMutex();
}

void YKDelayTask(unsigned count){
  TCBptr ready;
  YKEnterMutex();

  if(count == 0){
    YKExitMutex();
    return;
  }

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
    ready->next->prev=ready;
  }
  ready->delay = count;
  YKScheduler(1);
  YKExitMutex();
}


void YKEnterISR(void){
  YKISRDepth = YKISRDepth + 1;
  // printNewLine();
}

void YKExitISR(void){
  YKISRDepth = YKISRDepth - 1;
  // printString("Depth ");
  //printInt(YKISRDepth);
  // printNewLine();
	if(YKISRDepth == 0) {
    YKScheduler(1);
  }
}

void YKTickHandler(void){
  TCBptr tempDelay, tempReady, tempNext;

  YKEnterMutex();
  YKTickNum = YKTickNum + 1;
  tempDelay = YKDelayList;
	//While the delay is not finished, counter--;
  while(tempDelay != NULL){
    tempNext = tempDelay->next;
    tempDelay->delay = tempDelay->delay - 1;
    if(tempDelay->delay <= 0){
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
      // insert delayed task in ready list
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
    tempDelay = tempNext;
  }
  YKExitMutex();
}

YKSEM* YKSemCreate(int initialValue){
  int i;
  //YKSEM* newSem;
  //newSem->val = initialValue;
  //newSem->id = YKSemCount;
  //printInt(YKSemCount);
  //if(initialValue >= 0 && YKSemCount < SEM_COUNT){
  //  YKSemArray[YKSemCount] = newSem;
  //  printString("Create");
  //  YKSemCount++;
  //  for(i = 0; i < YKSemCount; i++){
  //    printInt(YKSemArray[i]->val);
  //  }
  //  printNewLine();
  //
  //  return newSem;
  //}
  //else{
  //  return NULL;
  //}
  
	
  //YKEnterMutex();
  
  for (i = 0; YKSemArray[i].active; i++){}; // find next open semaphore
  
  YKSemArray[i].active = 1; //make active
  YKSemArray[i].val = initialValue;
  YKSemArray[i].id = i;
	
  return &(YKSemArray[i]);
}

/* Take semaphore
This function tests the value of the indicated semaphore then decrements it. 
If the value before decrementing was greater than zero, the code returns to the caller. 
If the value before decrementing was less than or equal to zero, the calling task is 
suspended by the kernel until the semaphore is available, and the scheduler is called. 
This function is called only by tasks, and never by ISRs or interrupt handlers.
*/
void YKSemPend(YKSEM *semaphore){
//   TCBptr readyTask;
//   TCBptr suspTask;
//   int i;
//   printString("Pend ");
//   printInt(semaphore->id);
//   for(i = 0; i < YKSemCount; i++){
//     printInt(YKSemArray[i]->id);
//   }
//   printNewLine();

//   YKEnterMutex();
//   if(semaphore->val > 0){
//     semaphore->val--;
//   }
//   else{
//     semaphore->val--;
//     // Move task from ready list to susp list
//     readyTask = YKRdyList;
//     YKRdyList = readyTask->next;
//     YKRdyList->prev = NULL;

//     suspTask = YKSuspList;
//     // Find where the the task to suspend should go
//     while(readyTask->priority < suspTask->priority && suspTask->next != NULL){
//       suspTask = suspTask->next;
//     }

//     if(suspTask->prev == NULL){ // insert readyTask at beginning of suspList
//       readyTask->next = suspTask;
//       suspTask->prev = readyTask;
//       readyTask->prev = NULL;
//       YKSuspList = readyTask;
//     }
//     else{ // otherwise insert normally
//       readyTask->next = suspTask;
//       readyTask->prev = suspTask->prev;
//       suspTask->prev->next = readyTask;
//       suspTask->prev = readyTask;
//       YKSuspList = readyTask;
//     }

//     YKScheduler(1);
//   }
//   YKExitMutex();
	
  TCBptr readyTask;
  YKEnterMutex();
  // make semaphore unavalible to take
  YKExitMutex();
  
  if (semaphore->val >= 0){ return; } // break if it gets above zero because that means its available
	  
  YKEnterMutex();
  readyTask = YKRdyList;
  YKRdyList = readyTask->next;
  readyTask->next->prev = NULL;
  readyTask->prev = YKSemWaitList; // store on the top of the sem wait list
  YKSemWaitList = readyTask;
  readyTask->prev = NULL;
	
  if(readyTask->next != NULL){
    readyTask->next->prev = readyTask;
  }
	
  readyTask->semWait = semaphore;
	
  YKScheduler(1);
  YKExitMutex();
  
	
	
}

/* Release semaphore
This function increments the value of the indicated semaphore. 
If any suspended tasks are waiting for this semaphore, the waiting task with the 
highest priority is made ready. Unlike YKSemPend, this function may be called from 
both task code and interrupt handlers. If called from task code (easily determined 
by the value of the ISR call depth counter) then the function should call the 
scheduler so that newly awakened high-priority tasks can resume right away. 
If the function is called from an interrupt handler, the scheduler should not be 
called within the function. It will be called shortly in YKExitISR after all ISR actions are completed.
*/
void YKSemPost(YKSEM *semaphore){
//   TCBptr suspTask;
//   printString("Post ");
//   printInt(semaphore->id);
//   printNewLine();

//   YKEnterMutex();
//   semaphore->val++;
//   suspTask = YKSuspList;
//   // Loop through suspended tasks
//   while(suspTask->next != NULL){
//       //  if task is highest priority and is waiting for sem, make ready
//       if(&(suspTask->semWait) == semaphore){

//         //  if called from task code (ISR depth == 0), call the scheduler
//         if(YKISRDepth == 0){
//           YKScheduler(1);
//         }
//         YKExitMutex();
//         return;
//       }
//       suspTask = suspTask->next;
//     }
//   YKExitMutex();
	
  TCBptr semWaiting, unSuspTask, readyTask;
  unSuspTask = NULL;
  semWaiting = YKSemWaitList;

  YKEnterMutex();
  semaphore->val++;
	
  // Loop through suspended tasks
  while(semWaiting != NULL){
      //  if task is highest priority and is waiting for sem, make ready
      if(semWaiting->semWait == semaphore){
        if((unSuspTask == NULL) || (semWaiting->priority < unSuspTask->priority)){
          unSuspTask = semWaiting;
	}
      }
      semWaiting = semWaiting->next;
  }
	
  if(unSuspTask == NULL){ // if it never assigns unSuspTask in while loop then kill and return
    YKExitMutex();
    return;
  }
	
  if (unSuspTask->next != NULL){
    YKSemWaitList = unSuspTask->next;
  }
  else{
    unSuspTask->next->prev = unSuspTask->prev;
  }
  
  // now deal with the ready list
  readyTask = YKRdyList;
  while (readyTask->priority < unSuspTask->priority){
    readyTask = readyTask->next;
  }
  if(readyTask->prev == NULL){ // AKA its at the front
    YKRdyList = unSuspTask;
  }
  else{                        // insert it
    readyTask->prev->next = unSuspTask;
  }
  unSuspTask->prev = readyTask->prev;
  unSuspTask->next = readyTask;
  readyTask->prev = unSuspTask;
	
  unSuspTask->semWait = NULL;
	
  if( YKISRDepth == 0){
  YKScheduler(1);
  }
  YKExitMutex();
  return;
}


/******************** Functions in yaks.s ********************/
// Functions are made inside of yaks.s because they are coded in assembly
//void YKDispatcher(void);    // Begins or resumes execution of the next task
//void YKEnterMutex(void);    // Disables interrupts
//void YKExitMutex(void);     // Enables interrupts


/******************** Functions not in this lab ********************/
//YKQ *YKQCreate(void **start, unsigned size)
//void *YKQPend(YKQ *queue)
//int YKQPost(YKQ *queue, void *msg)
//YKEVENT *YKEventCreate(unsigned initialValue)
//unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
//void YKEventSet(YKEVENT *event, unsigned eventMask)
//void YKEventReset(YKEVENT *event, unsigned eventMask)
