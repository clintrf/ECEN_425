#include "clib.h"
#include "yakk.h"





/******************** Global Variables ********************/
unsigned int YKCtxSwCount;            // must be incremented each time a context switch occurs, defined as - 
                                      //  - the dispatching of a task other than the task that ran most recently.
unsigned int YKIdleCount;             // Must be incremented by the idle task in its while(1) loop.
//unsigned int YKTickNum;             // Must be incremented each time the kernel's tick handler runs. For dif lab

unsigned int run_flag;

TCBptr TKCurrentlyRunning;
int idleStack[IDLE_STACK_SIZE];

TCBptr YKRdyList;
TCBptr YKSuspList;
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs*/

void YKInitialize(void){    // Initializes all required kernel data structures
  int i;
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  //YKTickNum = 0;            // Set to 0
  run_flag = 0;           // No proccesses are running at initialization
  TKCurrentlyRunning = 0;
  
  	YKEnterMutex();           // Turn on interupts at initialization
  
  /* code to construct singly linked available TCB list from initial array */ 
  YKAvailTCBList = &(YKTCBArray[0]);
  
  for (i = 0; i < MAXTASKS; i++)
	  YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[MAXTASKS].next = NULL;
	
  printString("before new task...\n");
	
  YKNewTask(YKIdleTask, (void*)&idleStack[IDLE_STACK_SIZE], LOWEST_PRIORITY);
  //call YKIdleTask         // From YAK Kernel instruction book
  //^ could call YKIdleTask as YKNewTask()
  
  
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKIdleCount=YKIdleCount+1;          // 
    YKExitMutex();
  }                        
  /*Therefore, to prevent overflow, your while(1) loop in YKIdleTask should -
  - take at least 4 instructions per iteration to prevent overflow of YKIdlecount -
  - at the default tick rate. Ideally, you want YKIdleTask to take exactly 4 -
  - instructions per iteration. After writing your idle task, disassemble it -
  - to make sure its while(1) loop is at least 4 instructions per iteration -
  - (including the jmp instruction). */
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
    while (tmp2->priority < tmp->priority){
      tmp2 = tmp2->next;	/* assumes idle task is at end */
      printString("looking for Idle \n"); 
    }
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
	
  printString("Address for new task's SP is "); 
  printInt((int) &(tmp->stackptr));
  printString("\n");
	
  tmp->ss = 0;
  tmp->stackptr		= tmp->stackptr - 11;
  *(tmp->stackptr+11)	= 0x200; //flag interupt
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
	
  printString("before Scheduler...\n");
	
  YKScheduler(SAVE);          // Save current block of mem
  
}

void YKRun(void){                 // Starts actual execution of user code
  run_flag = HIGH;                // Start the Scheduler for the very first time
  YKScheduler(NSAVE);             // run the top proccess 
}

void YKScheduler(unsigned int save_flag){     // Determines the highest priority ready task
  TCBptr highest_priority_task = YKRdyList;
  TCBptr currentlyRunning = TKCurrentlyRunning;  
  if(!run_flag){                               // NOT redundant! Tell the kernel to begin for first time
    return;	  
  }
  if (currentlyRunning == highest_priority_task){
    return;
  }
  YKCtxSwCount = YKCtxSwCount + 1;	// Switching context one more time
  TKCurrentlyRunning = highest_priority_task;   
	
  printString("before dipatcher...\n");
 
  if(save_flag){
    YKDispatcherSave(1,&(currentlyRunning->stackptr),&(currentlyRunning->ss), highest_priority_task->stackptr, highest_priority_task->ss);
  }
  else{
    YKDispatcherNSave(0,(int **) 1,(int ** ) 1, highest_priority_task->stackptr, highest_priority_task->ss);
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
