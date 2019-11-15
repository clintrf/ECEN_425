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

YKSEM YKSemArray[SEM_COUNT]; // Not sure how large this array should be, change it if needed
YKQ YKQueueArray[QUE_COUNT]; // List of queues
TCBptr YKSemWaitList;        // List of the semaphores currently waiting
TCBptr YKQWaitList;

TCBptr TKCurrentlyRunning;

int tickcount = 0;
char run_flag = 0;

void YKInitialize(void){    // Initializes all required kernel data structures
  int i;
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  TKCurrentlyRunning = 0;   // Set to 0
  YKISRDepth = 0;
  //YKSemCount = 0;
  YKTickNum = 0;
  
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
 
  for (i = 0; i < QUE_COUNT; i++){ 
    YKQueueArray[i].size = 0;
    YKQueueArray[i].length = 0;
    YKQueueArray[i].base_addr = 0; 
    YKQueueArray[i].head = 0;
    YKQueueArray[i].tail = 0;

  }

  YKNewTask(YKIdleTask, (void*)&idleStack[256], 100);
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKEnterMutex();
    YKIdleCount=YKIdleCount+1;          
    YKExitMutex();
  }
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){    // Creates a new task
  TCBptr tmp, tmp2;
  int i;

  taskStack = ((int *)taskStack) - 1;	
	
  /* code to grab an unused TCB from the available list */
  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;

  // Set the struct var definitions
  tmp->priority = priority;
  tmp->delay = 0;
	
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
  //YKExitMutex();              		// starts interrupts
}

void YKRun(void){                 // Starts actual execution of user code
  printString("Start Run and call scheduler\n");
  run_flag = 1;                // Start the Scheduler for the very first time
  YKScheduler(0);             // run the top proccess
}

void YKScheduler(int save_flag){     // Determines the highest priority ready task
  //int testVar;
  //int* testPt;
  TCBptr highest_priority_task;
  TCBptr currentlyRunning;

  //YKEnterMutex();
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
  //YKExitMutex();
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
	
  if(YKISRDepth == 0) {
    YKScheduler(1);
  }
}

void YKTickHandler(void){
  TCBptr tempDelay, tempReady, tempNext;

  YKEnterMutex();
  YKTickNum = YKTickNum + 1;
  if(tickcount< 7){
    tickcount++;
    YKTickNum = 0;
  }
  //YKTickNum = YKTickNum + 1;
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
  YKEnterMutex();
  i = 0;
  while(YKSemArray[i].active){
    i = i+1;
  }
  //for (i = 0; YKSemArray[i].active; i++){}; // find next open semaphore
  
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
	
  TCBptr readyTask;
  YKEnterMutex();
  semaphore->val = semaphore->val - 1;
  YKExitMutex();
  
  if (semaphore->val >= 0){ return; } // break if it gets above zero because that means its available
	  
  YKEnterMutex();
  readyTask = YKRdyList;
  YKRdyList = readyTask->next;
  readyTask->next->prev = NULL;
  readyTask->next = YKSemWaitList; // store on the top of the sem wait list
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
  TCBptr semWaiting, unSuspTask, readyTask;
  unSuspTask = NULL;
  semWaiting = YKSemWaitList;

  YKEnterMutex();
  semaphore->val = semaphore->val + 1;
	
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
	
  if(unSuspTask->prev == NULL){
      YKSemWaitList = unSuspTask->next;
  }
  else{
    unSuspTask->prev->next = unSuspTask->next;
  }
	
  if (unSuspTask->next != NULL){
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

void queueInsert(YKQ* queue, void* msg){
  queue->base_addr[queue->head] = msg;
  if(queue->head < queue->size){
    queue->head += 1;
  }
  else{
    queue->head = 0;
  }
}

void* queueRemove(YKQ* queue){
  void* msg;
  msg = *(queue->base_addr + queue->tail);
  if(queue->tail > 0){
    queue->head -= 1;
  }
  else{
    queue->head = queue->size - 1;
  }
  return msg;
}
//*****************************************************************************************
//Initializes the message queue, returns pointer to that queue
YKQ *YKQCreate(void **start, unsigned size){
//   YKQ* queue;
//   queue->base_addr = start;
//   queue->cur_length = 0;
//   queue->size = size;
//   queue->tail = 0;
//   queue->head = 0; 
//   return queue;
  int i;	
  YKEnterMutex();
  for (i = 0; YKQueueArray[i].base_addr; i++){}; // find next queue
  
  YKQueueArray[i].base_addr = start;
  YKQueueArray[i].length = 0;
  YKQueueArray[i].size = size;
  YKQueueArray[i].tail = 0;
  YKQueueArray[i].head = 0; 
	
  return &(YKQueueArray[i]);
  
}

/*
This function removes the oldest message from the indicated message queue if it is non-empty. 
If the message queue is empty, the calling task is suspended by the kernel until a message becomes available. 
The function returns the oldest message in the queue (cast to C's generic "void pointer" type). 
This function is called only by tasks and never by interrupt handlers or ISRs.
*/
void *YKQPend(YKQ *queue){
//   int i;
//   void* msg;
//   TCBptr readyTask;
//   YKEnterMutex();
//   if(queue->cur_length == 0){
//     // suspend calling task until there is something in the queue
//     readyTask = YKRdyList;
//     YKRdyList = readyTask->next;
//     readyTask->next->prev = NULL;
//     readyTask->next = YKQWaitList; // store on the top of the queue wait list
//     YKQWaitList = readyTask;
//     readyTask->prev = NULL;

//     if(readyTask->next != NULL){
//       readyTask->next->prev = readyTask;
//     }

//     readyTask->queueWait = queue;

//     YKExitMutex();
//     return NULL;
//   }
//   else{
//     msg = queueRemove(queue);
//     YKExitMutex();
//     return msg;
//   }
  TCBptr readyTask;
  void* msg;
  YKEnterMutex();
  if(queue->length == 0){
    readyTask = YKRdyList;
    YKRdyList = readyTask->next;
    readyTask->next->prev = NULL;
    readyTask->next = YKQWaitList; // store on the top of the queue wait list
    YKQWaitList = readyTask;
    readyTask->prev = NULL;
    
    if(readyTask->next != NULL){
      readyTask->next->prev = readyTask;
    }

    readyTask->queueWait = queue;
    YKScheduler(1);  
  }
  msg = *(queue->base_addr + queue->tail);
  queue->size = queue->size - 1;
	
  if((queue->tail + 1) < queue->length){
    queue->tail = queue->tail + 1;
  }
  else{
    queue->tail = 0; // wrap around
  }
	
  YKExitMutex();
  return msg;
}

/*
This function places a message in a message queue. 
- The first parameter is the queue in which the message is to be placed.
- The second parameter is a pointer to the message (cast as a void pointer). 
- If space was available in the queue and the message was successfully inserted, the function returns the 
value 1. If the queue is full, no message is inserted and the value 0 is returned. 
- If any suspended tasks are waiting for a message from this queue, the highest priority task is made ready 
to run. 
- If called from task code (Depth = 0), the function should call the scheduler so newly awakened 
high-priority tasks have an opportunity to run immediately. 
- Otherwise, the scheduler should not be called in YKQPost.
*/
int YKQPost(YKQ *queue, void *msg){
  TCBptr queueWait, unWaitTask, readyTask;
  YKEnterMutex();

  if((queue->length - 1) == queue->size){
    return 0; // is full
  }
  unWaitTask = NULL;	
  queueWait = YKQWaitList;	

  // insert
  *(queue->base_addr + queue->head) = msg;
  queue->size = queue->size + 1;	
  if((queue->head + 1) < queue->length){
    queue->head = queue->head + 1;
  }
  else{
    queue->tail = 0; // wrap around
  }
	
  while(queueWait != NULL){
    //  if task is highest priority and is waiting for sem, make ready
    if(queueWait->queueWait == queue){
      if((unWaitTask == NULL) || (queueWait->priority < unWaitTask->priority)){
        unWaitTask = queueWait;
      }
    }
    queueWait = queueWait->next;
  }
    
  // If suspended tasks are waiting for a msg from this queue
  if(unWaitTask == NULL){
    YKExitMutex();
    return 1;
  }

  //    make the highest priority task ready
  if(unWaitTask->prev == NULL){
    YKQWaitList = unWaitTask->next;
  }
  else{
    unWaitTask->prev->next = unWaitTask->next;
  }
    
  if (unWaitTask->next != NULL){
    unWaitTask->next->prev = unWaitTask->prev;
  }   
  // now deal with the ready list
  readyTask = YKRdyList;
  while (readyTask->priority < unWaitTask->priority){
    readyTask = readyTask->next;
  }
  if(readyTask->prev == NULL){ // AKA its at the front
    YKRdyList = unWaitTask;
  }
  else{                        // insert it
    readyTask->prev->next = unWaitTask;
  }
  unWaitTask->prev = readyTask->prev;
  unWaitTask->next = readyTask;
  readyTask->prev = unWaitTask;
   
  unWaitTask->queueWait = NULL;
  
  if(YKISRDepth == 0){
    YKScheduler(1);
  }
  YKExitMutex();
  return 1;
}

/******************** Functions in yaks.s ********************/
// Functions are made inside of yaks.s because they are coded in assembly
//void YKDispatcher(void);    // Begins or resumes execution of the next task
//void YKEnterMutex(void);    // Disables interrupts
//void YKExitMutex(void);     // Enables interrupts


/******************** Functions not in this lab ********************/
//YKEVENT *YKEventCreate(unsigned initialValue)
//unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
//void YKEventSet(YKEVENT *event, unsigned eventMask)
//void YKEventReset(YKEVENT *event, unsigned eventMask)
