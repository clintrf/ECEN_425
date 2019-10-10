#include "clib.h"
#include "yakh.h"


void YKInitialize(void){    // Initializes all required kernel data structures
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  //YKTickNum = 0;            // Set to 0
  run_flag = 0              // No proccesses are running at initialization
  
  YKEntermutex();           // Turn on interupts at initialization
  //make list of TCB based of the max number of tasks
  //call YKIdleTask         // From YAK Kernel instruction book
  //^ could call YKIdleTask as YKNewTask()
  
}

void YKIdleTask(void){      // Kernel's idle task
  while(1){                 // From YAK Kernal instuction book
    YKIdleCount++;          // ^
  }                         // ^
  /*Therefore, to prevent overflow, your while(1) loop in YKIdleTask should -
  - take at least 4 instructions per iteration to prevent overflow of YKIdlecount -
  - at the default tick rate. Ideally, you want YKIdleTask to take exactly 4 -
  - instructions per iteration. After writing your idle task, disassemble it -
  - to make sure its while(1) loop is at least 4 instructions per iteration -
  - (including the jmp instruction). */
}

void YKNewTask(void (* task)(void), \         // Creates a new task
void *taskStack, \
unsigned char priority){    

  //
  
  
  YKScheduler(SAVE);            // run the top proccess 
}

void YKRun(void){                 // Starts actual execution of user code
  run_flag = HIGH;                // Start the Scheduler for the very first time
  YKScheduler(NSAVE);             // run the top proccess 
}

void YKScheduler(unsigned int save_flag){     // Determines the highest priority ready task
  if(run_flag){                               // NOT redundent! Tell the kernel to begin for first time
    if(save_flag){
      YKDispatcherSave();
    }
    else{
      YKDispatcherNSave();
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
