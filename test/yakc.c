//Kernel routines written in C. Global variables
//used by kernel code or shared by kernel and application
//code should also be defined in this file
#include "clib.h"
#include "yakk.h"

#define FLAGS_INTERRUPT_ONLY 0x200

#define IDLESTACKSIZE 256
int idleStack[IDLESTACKSIZE];           /* Space for each task's stack */

unsigned int YKCtxSwCount;	// incremented every context switch
unsigned int YKIdleCount;	// incremented by idle task in while(1) loop
unsigned int YKTickNum;		// incremented by tick handler
unsigned int YKISRCallDepth;

TCBptr YKRdyList;// a list of TCBs of all ready tasks in order of decreasing priority
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	// array to allocate all needed TCBs extra for idle task

// When the scheduler dispatches a task, it sets this to that task.
TCBptr YKCurrentlyExecuting; // Starts at 0 because nothing is executing at start

// Gets set by YKRun.
// Scheduler does not call dispatcher unless it is set
char started_running = 0;

void YKInitialize(void){
	int i;

	// Initialize them in initialize. That makes sense.
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKCurrentlyExecuting = 0; // Starts at 0 because nothing is executing at start
	YKISRCallDepth = 0;
//	printString("Initializing!\n");

	/* Allocate stack space */
	//*******************************************use the #DEFINE IN HEADER FILE
    /* code to construct singly linked available TCB list from initial
       array */ 
	YKEnterMutex();
    YKAvailTCBList = &(YKTCBArray[0]);
    for (i = 0; i < MAXTASKS; i++)
	YKTCBArray[i].next = &(YKTCBArray[i+1]);
    YKTCBArray[MAXTASKS].next = NULL;

 	/* Create idle task by calling YKIdleTask() */
	// I think that's dumb.
	// Let's create the idle task by calling YKNewTask on it
//	printString("calling newtask for idle task(");
//	printInt((int) YKIdleTask);
//	printString(") and giving it its stack(");
//	printInt((int) &idleStack[IDLESTACKSIZE]);
//	printString(")\n");
	YKNewTask(YKIdleTask, (void *)&idleStack[IDLESTACKSIZE], 100);	// Give it a priority of 100 for some reason
}

void YKIdleTask(void) {
	//Kernel's idle task
	while(1){
	//	printString("IDLE TASK EXECUTING\n");
		YKIdleCount = YKIdleCount+1; 
		YKExitMutex();
	}
	//after disassembly, ensure while(1) loop is at least 4 instructions per iteration (including jmp instruction)  
}

/*
   !! make new TCB entry for task					DONE
   !! store name and priority in TCB				DONE
   !! store proper stack pointer in TCB for task	DONE
   !! store PC in TCB								DONE
   !! 0 ticks to delay stored in TCB				DONE
   */
void YKNewTask(void (*task)(void), void *taskStack, unsigned char priority){
	// -----------------------------------------------
	// Make a new TCB entry for task (on ready list)
	//
	TCBptr tmp, tmp2;

	//	printString("new task executing for function: ");
	//	printInt((int)task);
	//	printString("\n");

	// grabs an unused TCB from the available list
	tmp = YKAvailTCBList;
	YKAvailTCBList = tmp->next;


	// 0 ticks to delay in TCB
	tmp->delay = 0;


	// Store PC in TCB
	//	tmp->address = task;

	// Store priority in TCB
	tmp->priority = priority;

YKEnterMutex();

	// code to insert an entry in doubly linked ready list sorted by
	// priority numbers (lowest number first).  tmp points to TCB
	// to be inserted
	if (YKRdyList == NULL) {	// is this first insertion?
		YKRdyList = tmp;
		tmp->next = NULL;
		tmp->prev = NULL;
	} else {			// not first insertion 
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
YKExitMutex();
	// End making a new TCB entry for task
	// ----------------------------------------------

	//	printString("we have decided our new task's TCB is to be ");
	//	printInt((int)tmp);
	//	printString("\nand our new task's STACK is");
	//	printInt((int)taskStack);
	//	printString("\n");
	// Save the stack pointer
	tmp->stackptr = taskStack;
	printString("Address for new task's SP is ");
	printInt((int) &(tmp->stackptr));
	printString("\n");
	tmp->ss = 0;				// just always make ss 0. is good idea?

	// Now we need to store in this stack an entire "context" to restore from
	tmp->stackptr		= tmp->stackptr - 11;
	*(tmp->stackptr+11)	= FLAGS_INTERRUPT_ONLY;		// flags
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


	// Now we need to call the scheduler. Which will decide what to call next.
	//	printString("calling the scheduler\n");
	YKScheduler(1);	// We DO need to save context.
}

/*
 * Starts the kernel
 * !!run()
 */
void YKRun(void) { /* starts the kernel */
	printString("run called. Calling the scheduler\n");
	started_running = 1;
	YKScheduler(0);		//	Because no tasks running, do NOT save context.
}


/*
 * !!For each in TCB
 * !! find highest-priority ready task
 * !! call dispatcher
 */
void YKScheduler_old(void) {
	// highest-ready task to be called from TCB
	TCBptr highest_priority_task = YKRdyList;

	printString("THIS SHOULD NEVER HAPPEN\nTHIS SHOULD NEVER HAPPEN\n");
	printString("scheduler here. dispatcher will load ");
	printInt((int)highest_priority_task);
	printString(" which has stack ");
	printInt((int)highest_priority_task->stackptr);
	printString("\n");
/*
 *	Potentially, the address does not need to be stored in TCB.
 *	If it's the instruction pointer, then it gets taken care of by iret???
 *	That's the presumed word on the street based off of what others have
 *	interpreted their help from the TA Taylor
 *
 *
 *	other: decrement SP in Dispatcher...
 * */
	
	// which one is the current task??
	// See note in description...might need to pass in a parameter...

	//if current task is diff from highest_priority_task
	//then call dispatcher
	
	
	// I don't think we need to worry about ordering things just now
	// because we are only calling NewTask. Which puts itself in the proper order.
	if(started_running){	// Only call the dispatcher if we are running.
		if(YKCurrentlyExecuting == highest_priority_task){
			return;
		}
		
		YKCtxSwCount = YKCtxSwCount + 1;
		YKCurrentlyExecuting = highest_priority_task;
//		printString("calling the dispatcher\n");
		YKDispatcher();
	}
}

void YKScheduler(int need_to_save_context){
	TCBptr highest_priority_task = YKRdyList;
	TCBptr currentlyExecuting = YKCurrentlyExecuting;

//	printString("\tSCHEDULER :)");
//	printString("YKRdyList: ");
//	printInt((int)YKRdyList);
//	printString("\n");

	// Only do things if we are running. Otherwise return
	if(!started_running){
//		printString("scheduler called, but not yet running\n");
		return;
	}
	if(YKCurrentlyExecuting == highest_priority_task){
//		printString("scheduler called; returning to task\n");
		return;	// We do not need to dispatcher if go back to same task!
	}

	YKCtxSwCount = YKCtxSwCount + 1;	// Switching context one more time
	YKCurrentlyExecuting = highest_priority_task;

	// If we do not need to save context, it doesn't get an address to save it
	if(!need_to_save_context){
//		printString("scheduler called, no need to save context\n");
//		printString("giving the dispatcher stackptr");
//		printInt((int)highest_priority_task->stackptr);
//		printString(" and SS ");
//		printInt((int)highest_priority_task->ss);
//		printString("highest_priority_task ");
//		printInt((int)highest_priority_task);
//		printNewLine();
		YKDispatcher_save_context(0,(int **) 1, (int **)1,
				highest_priority_task->stackptr, highest_priority_task->ss);
	} else {
		// We DO need to save context
		// SP and SS of what we need to save
		// SP and SS that we need to restore
//		printString("scheduler called, need to save context\n");

//		printString("currently running task: ");
//		printInt((int)currentlyExecuting);
//		printNewLine();
//		printString("Handing dispatcher SP: ");
//		printInt((int)&(currentlyExecuting->stackptr));
//		printString("Handing dispatcher SS: ");
//		printInt((int)&(currentlyExecuting->ss));
//		printNewLine();

		YKDispatcher_save_context(need_to_save_context, 
				&(currentlyExecuting->stackptr), &(currentlyExecuting->ss),
				highest_priority_task->stackptr, highest_priority_task->ss);
	}
}



/*	!! restore SP
 *	!! pop the context into every register
 *	!! restore PC with iret
 */
// Commented because we are coding this up in assembly
//void YKDispatcher(void) {
//    YKCtxSwCount ++;
//}

// Delays a task for a specified number of clock ticks
void YKDelayTask(unsigned count)
{
  TCBptr tmp;
  YKEnterMutex();
  //After taking care of all required bookkepping to mark change of
  //state for currently running task, call scheduler.
	//BOOKKEEPING TIME!!!!!!  
  //if count is zero, then don't delay. Just return
  if(count != 0)
  {
   /* code to remove an entry from the ready list and put in
       suspended list, which is not sorted.  (This only works for the
       current task, so the TCB of the task to be suspended is assumed
       to be the first entry in the ready list.)   */
    tmp = YKRdyList;		/* get ptr to TCB to change */
    YKRdyList = tmp->next;	/* remove from ready list */
    tmp->next->prev = NULL;	/* ready list is never empty */
    tmp->next = YKSuspList;	/* put at head of delayed list */
    YKSuspList = tmp;
    tmp->prev = NULL;
    if (tmp->next != NULL)	/* susp list may be empty */
	tmp->next->prev = tmp;
    tmp->delay = count;
  }
  else
  {
    YKExitMutex();
    return;
  }
  //at the very end, this function calls the scheduler
  YKScheduler(1);  // we DO need to save context
  YKExitMutex();
}
/*
// Called at the beginning of an ISR. Increments ISR call depth
void YKEnterISR(void)
{
  //these might actually be better off in assembly...
}

// Called at the end of an ISR. Decrements ISR call depth
void YKExitISR(void)
{
  //maybe better to be in assembly...
  //call scheduler
}
*/
// Called from the Tick ISR each time it runs. Responsible for waking delayed tasks
void YKTickHandler(void)
{
  TCBptr tmp, tmp2,tmp_next;
//  printString("called YKTickHandler() currently within it\n");  
//bookkeeping required to support timely reawakening of delayed tasks.
  
  //may also call user tick handler if user code requires actions to be taken on each clock tick...what's that even mean?!?
  tmp = YKSuspList;
  YKEnterMutex();

  while(tmp != NULL)
  {
//    printString("Tick Handler Loop\n");
    tmp_next = tmp->next;
    tmp->delay = tmp->delay - 1; //decrement the delay of this one in the list 
    if(tmp->delay == 0)//if specified number of clock ticks has ocurred, a delayed task is made ready.
    {
//	printString("A DELAYED TASK IS NOW READY :)\n");
     /* code to remove an entry from the suspended list and insert it
       in the (sorted) ready list.  tmp points to the TCB that is to
       be moved. */
      if (tmp->prev == NULL)	/* fix up suspended list */
	YKSuspList = tmp->next;
      else
	tmp->prev->next = tmp->next;
      if (tmp->next != NULL)
	tmp->next->prev = tmp->prev;

      tmp2 = YKRdyList;		/* put in ready list (idle task always
				 at end) */
      while (tmp2->priority < tmp->priority)
	tmp2 = tmp2->next;
      if (tmp2->prev == NULL)	/* insert before TCB pointed to by tmp2 */
	YKRdyList = tmp;
      else
  	tmp2->prev->next = tmp;
      tmp->prev = tmp2->prev;
      tmp->next = tmp2;
      tmp2->prev = tmp;
    }
    tmp = tmp_next;
  }
  YKExitMutex();
}

void YKEnterISR(void)
{
  YKISRCallDepth = YKISRCallDepth+1;
}

void YKExitISR(void)
{
	YKISRCallDepth = YKISRCallDepth-1;

  //decrements the counter representing ISR call depth
  if(YKISRCallDepth == 0)
  {
	  // DO NOT SAVE CONTEXT :)
    YKScheduler(0);	//calls scheduler if counteris zero
  }
    //I'm pretty sure YKScheduler() should handle the following situations:
	//in case of nested interrupts, counter is zero only when exiting last ISR, so it indicates
	//when control will return to task code rather than another ISR.
	//If it is last ISR then control should return to highest priority ready task.
	//This may not be the task that was interrupted by this ISR if the actions of interrrupt
	//handler made a higher priority task ready
}
