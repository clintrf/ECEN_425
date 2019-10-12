#include "clib.h"
#include "yakh.h"



void YKInitialize(void){    // Initializes all required kernel data structures
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  //YKTickNum = 0;            // Set to 0
  run_flag = 0;           // No proccesses are running at initialization
  current_priority = 0;
  
  YKEntermutex();           // Turn on interupts at initialization
  
  /* code to construct singly linked available TCB list from initial array */ 
  YKAvailTCBList = &(YKTCBArray[0]);
  for (int i = 0; i < MAXTASKS; i++)
	  YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[MAXTASKS].next = NULL;
  
  
  // Do we need to allocate memory for the task stack to pass in?
  // What is that first parameter?
  void* stackptr = malloc(STACK_SIZE);
	
  YKNewTask(YKIdleTask, (void*)&idleStack, LOWEST_PRIORITY);
  //call YKIdleTask         // From YAK Kernel instruction book
  //^ could call YKIdleTask as YKNewTask()
  
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKIdleCount++;          // 
    YKExitMutex();
  }                        
  /*Therefore, to prevent overflow, your while(1) loop in YKIdleTask should -
  - take at least 4 instructions per iteration to prevent overflow of YKIdlecount -
  - at the default tick rate. Ideally, you want YKIdleTask to take exactly 4 -
  - instructions per iteration. After writing your idle task, disassemble it -
  - to make sure its while(1) loop is at least 4 instructions per iteration -
  - (including the jmp instruction). */
}

void TCBInit(TCB* newTCB, void* stackptr, int state, int priority, int delay, TCBptr next, TCBptr prev){
  // Initialize all the new TCB values
  newTCB->stackptr = stackptr;
  newTCB->state = state;
  newTCB->priority = priority;
  newTCB->delay = delay;
  newTCB->next = next;
  newTCB->prev = prev;
  // Set the previous TCB's next to be the new TCB
  prev->next = newTCB;
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){    // Creates a new task
      
  TCBptr tmp, tmp2;
  
  /* code to grab an unused TCB from the available list */
  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;
  
  //TCB newTCB;
  //TCBInit(&newTCB, taskStack, priority, DEFAULT_DELAY, NULL, YKTCBArray[i]); // Inits TCB
	
  tmp->delay = DEFAULT_DELAY;
  tmp->priority = priority;
  //tmp2 = &newTCB;
  
  YKEnterMutex();             //Disable interupts
  /* code to insert an entry in doubly linked ready list sorted by
   priority numbers (lowest number first).  tmp points to TCB
   to be inserted */ 
  if (YKRdyList == NULL){	/* is this first insertion? */
    YKRdyList = tmp;
    tmp->next = NULL;
    tmp->prev = NULL;
  }
  else{			/* not first insertion */
    tmp2 = YKRdyList;	/* insert in sorted ready list */
    while (tmp2->priority < tmp->priority)
      tmp2 = tmp2->next;	/* assumes idle task is at end */
    if (tmp2->prev == NULL)	/* insert in list before tmp2 */
      YKRdyList = tmp;
    else
      tmp2->prev->next = tmp;
    tmp->prev = tmp2->prev;
    tmp->next = tmp2;
    tmp2->prev = tmp;
  }

  YKExitMutex();              // starts interrupts
  
  //YKAvailTCBList = tmp2->next;  // sets YKAvailTCBList to next open spot
  
  //current_priority = tmp2->priority;
	
  tmp->stackptr = taskStack; // from function call
  tmp->ss = 0;
  tmp->stackptr		= tmp->stackptr - 11;
  *(tmp->stackptr+11)	= 0x200 ;flag interupt
  *(tmp->stackptr+10)	= 0;		// CS
  *(tmp->stackptr+9)	= (int)task;		// IP
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

void YKScheduler(unsigned int save_flag){     // Determines the highest priority ready task
  TCBptr highest_priority_task = YKRdyList;
  TCBptr currentlyExecuting = YKCurrentlyExecuting;  
  if(run_flag){                               // NOT redundant! Tell the kernel to begin for first time
    if (current_priority == highest_priority_task){
      return;
    }
    else{
      YKCtxSwCount = YKCtxSwCount + 1;	// Switching context one more time
      YKCurrentlyExecuting = highest_priority_task;      
      if(save_flag){
        YKDispatcherSave(&(current_priority->stackptr),&(current_priority->ss, highest_priotity_task->stackptr, highest_priority_task->ss);
      }
      else{
        YKDispatcherNSave((int **) 1,(int ** ) 1, highest_priotity_task->stackptr, highest_priority_task->ss);
      }
    }
  }
}

/******************** Functions in yaks.s ********************/
// Functions are made inside of yaks.s because they are coded in assembly
//void YKDispatcher(void);    // Begins or resumes execution of the next task
//void YKEnterMutex(void);    // Disables interrupts
//void YKExitMutex(void);     // Enables interrupts
  

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
//void YKEventReset(YKEVENT *event, unsigned eventMask)
