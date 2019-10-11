#include "clib.h"
#include "yakh.h"

#define TASK_RUNNING 1
#define TASK_READY 2
#define TASK_BLOCKED 3
#define STACK_SIZE 128

#define LOWEST_PRIORITY 100
#define HIGHEST_PRIORITY 0

#define DEFAULT_DELAY 0

void YKInitialize(void){    // Initializes all required kernel data structures
  YKCtxSwCount = 0;         // Set to 0
  YKIdleCount = 0;          // Set to 0
  //YKTickNum = 0;            // Set to 0
  run_flag = 0;              // No proccesses are running at initialization
  
  YKEntermutex();           // Turn on interupts at initialization
  for(int i = 0; i < MAXTASKS; i++){
    YKTCBArray[i].next = &(YKTCBArray[i+1]);
  }
  YKTCBArray[MAXTASKS].next = NULL;
  // Do we need to allocate memory for the task stack to pass in?
  // What is that first parameter?
  void* stackptr = malloc(STACK_SIZE);
  YKNewTask(YKIdleTask, stackptr, LOWEST_PRIORITY);
  //call YKIdleTask         // From YAK Kernel instruction book
  YKIdleTask();
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
  // Gets next open spot in TCB
  int i = 0;
  while(YKTCBArray[i].next != NULL){
    i++;
  }
  TCB newTCB;
  TCBInit(&newTCB, taskStack, priority, DEFAULT_DELAY, NULL, YKTCBArray[i]); // Inits TCB
  // stops interrupts
  // makes the new entry
  // starts interrupts
  // calles YKScheduler(SAVE) to save it.
  
}

void YKRun(void){                 // Starts actual execution of user code
  run_flag = HIGH;                // Start the Scheduler for the very first time
  YKScheduler(NSAVE);             // run the top proccess 
}

void YKScheduler(unsigned int save_flag){     // Determines the highest priority ready task
  if(run_flag){                               // NOT redundant! Tell the kernel to begin for first time
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
