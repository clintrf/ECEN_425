# 1 "yakc.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "yakc.c"
# 1 "clib.h" 1



void print(char *string, int length);
void printNewLine(void);
void printChar(char c);
void printString(char *string);


void printInt(int val);
void printLong(long val);
void printUInt(unsigned val);
void printULong(unsigned long val);


void printByte(char val);
void printWord(int val);
void printDWord(long val);


void exit(unsigned char code);


void signalEOI(void);
# 2 "yakc.c" 2
# 1 "yakk.h" 1
# 12 "yakk.h"
extern unsigned int YKCtxSwCount;

extern unsigned int YKIdleCount;
extern unsigned int YKTickNum;


typedef struct YKSEM
{
    int val;
    int active;
    int id;
} YKSEM;


typedef struct taskblock *TCBptr;
typedef struct taskblock
{

    int *stackptr;
    int state;
    int priority;
    int delay;
    TCBptr next;
    TCBptr prev;
    YKSEM *semWait;
} TCB;


extern TCBptr YKRdyList;
extern TCBptr YKDelayList;
extern TCBptr YKSemWaitList;
extern TCBptr YKAvailTCBList;
extern TCB YKTCBArray[9 +1];


void YKInitialize(void);
void YKIdleTask(void);
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);
void YKRun(void);

void YKScheduler(int save_flag);
void YKDelayTask(unsigned count);
void YKEnterISR(void);
void YKExitISR(void);
void YKTickHandler(void);



void YKEnterMutex(void);
void YKExitMutex(void);
void YKDispatcherNSave(int *restore_sp);
void YKDispatcherSave(int ** save_sp, int *restore_sp);
YKSEM* YKSemCreate(int initialValue);
void YKSemPend(YKSEM *semaphore);
void YKSemPost(YKSEM *semaphore);
# 3 "yakc.c" 2

int idleStack[256];


unsigned int YKCtxSwCount;

unsigned int YKIdleCount;
unsigned int YKTickNum;
unsigned int YKISRDepth;


TCBptr YKRdyList;
TCBptr YKDelayList;

TCBptr YKAvailTCBList;
TCB YKTCBArray[9 +1];

YKSEM YKSemArray[19];
TCBptr YKSemWaitList;

TCBptr TKCurrentlyRunning;

char run_flag = 0;

void YKInitialize(void){
  int i;
  YKCtxSwCount = 0;
  YKIdleCount = 0;
  TKCurrentlyRunning = 0;
  YKISRDepth = 0;



  YKEnterMutex();


  YKAvailTCBList = &(YKTCBArray[0]);

  for (i = 0; i < 9; i++)
   YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[9].next = 0;

  for (i = 0; i < 19; i++){
    YKSemArray[i].val = -10;
    YKSemArray[i].active = 0;
    YKSemArray[i].id = i;
  }

  YKNewTask(YKIdleTask, (void*)&idleStack[256], 100);
}

void YKIdleTask(void){
  while(1){
    YKEnterMutex();
    YKIdleCount=YKIdleCount+1;
    YKExitMutex();
  }
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){
  TCBptr tmp, tmp2;
  int i;

  taskStack = ((int *)taskStack) - 1;


  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;


  tmp->priority = priority;
  tmp->delay = 0;

  YKEnterMutex();


  if (YKRdyList == 0){
    YKRdyList = tmp;
    tmp->next = 0;
    tmp->prev = 0;
  }
  else{
    tmp2 = YKRdyList;
    while (tmp2->priority < tmp->priority){
      tmp2 = tmp2->next;
    }
    if (tmp2->prev == 0)
      YKRdyList = tmp;
    else
      tmp2->prev->next = tmp;
    tmp->prev = tmp2->prev;
    tmp->next = tmp2;
    tmp2->prev = tmp;
  }


  tmp->stackptr = taskStack;


  tmp->stackptr = tmp->stackptr - 11;
  for(i=11; i>=0;i--){
    if(i == 11){
      *(tmp->stackptr+i) = 0x200;
    }
    else if(i == 10){
      *(tmp->stackptr+i) = 0;
    }
    else if(i == 9){
      *(tmp->stackptr+i) = (int)task;
    }
    else{
      *(tmp->stackptr+i) = 0;
    }

  }

  YKScheduler(1);

}

void YKRun(void){
 printString("Start Run and call scheduler\n");
  run_flag = 1;
  YKScheduler(0);
}

void YKScheduler(int save_flag){


  TCBptr highest_priority_task;
  TCBptr currentlyRunning;



  highest_priority_task = YKRdyList;
  currentlyRunning = TKCurrentlyRunning;

  if(!run_flag || (TKCurrentlyRunning == highest_priority_task)){
    return;
  }

  YKCtxSwCount = YKCtxSwCount + 1;
  TKCurrentlyRunning = highest_priority_task;
  if(!save_flag){

    YKDispatcherNSave(highest_priority_task->stackptr);

  }
  else{

    YKDispatcherSave(&(currentlyRunning->stackptr), highest_priority_task->stackptr);

  }

}

void YKDelayTask(unsigned count){
  TCBptr ready;
  YKEnterMutex();

  if(count == 0){
    YKExitMutex();
    return;
  }


  ready = YKRdyList;

  YKRdyList = ready->next;
  ready->next->prev = 0;

  ready->next = YKDelayList;
  YKDelayList = ready;
  ready->prev = 0;
  if(ready->next != 0){
    ready->next->prev=ready;
  }
  ready->delay = count;
  YKScheduler(1);
  YKExitMutex();
}


void YKEnterISR(void){
  YKISRDepth = YKISRDepth + 1;

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
  tempDelay = YKDelayList;

  while(tempDelay != 0){
    tempNext = tempDelay->next;
    tempDelay->delay = tempDelay->delay - 1;
    if(tempDelay->delay <= 0){

      if(tempDelay->prev == 0){
        YKDelayList = tempDelay->next;
      }
      else{
        tempDelay->prev->next = tempDelay->next;
      }
      if(tempDelay->next != 0){
        tempDelay->next->prev = tempDelay->prev;
      }

      tempReady = YKRdyList;
      while(tempReady->priority < tempDelay->priority){
        tempReady = tempReady->next;
      }
      if(tempReady->prev == 0){
        YKRdyList = tempDelay;
      }
      else{
        tempReady->prev->next = tempDelay;
      }

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
# 263 "yakc.c"
  YKEnterMutex();
  i = 0;
  while(YKSemArray[i].active){
    i = i+1;
  }


  YKSemArray[i].active = 1;
  YKSemArray[i].val = initialValue;
  YKSemArray[i].id = i;

  return &(YKSemArray[i]);
}
# 284 "yakc.c"
void YKSemPend(YKSEM *semaphore){
# 330 "yakc.c"
  TCBptr readyTask;
  YKEnterMutex();
  semaphore->val = semaphore->val - 1;
  YKExitMutex();

  if (semaphore->val >= 0){ return; }

  YKEnterMutex();
  readyTask = YKRdyList;
  YKRdyList = readyTask->next;
  readyTask->next->prev = 0;
  readyTask->next = YKSemWaitList;
  YKSemWaitList = readyTask;
  readyTask->prev = 0;

  if(readyTask->next != 0){
    readyTask->next->prev = readyTask;
  }

  readyTask->semWait = semaphore;

  YKScheduler(1);
  YKExitMutex();

}
# 366 "yakc.c"
void YKSemPost(YKSEM *semaphore){
# 391 "yakc.c"
  TCBptr semWaiting, unSuspTask, readyTask;
  unSuspTask = 0;
  semWaiting = YKSemWaitList;

  YKEnterMutex();
  semaphore->val = semaphore->val + 1;


  while(semWaiting != 0){

      if(semWaiting->semWait == semaphore){
        if((unSuspTask == 0) || (semWaiting->priority < unSuspTask->priority)){
          unSuspTask = semWaiting;
 }
      }
      semWaiting = semWaiting->next;
  }

  if(unSuspTask == 0){
    YKExitMutex();
    return;
  }

  if(unSuspTask->prev == 0){
      YKSemWaitList = unSuspTask->next;
  }
  else{
    unSuspTask->prev->next = unSuspTask->next;
  }

  if (unSuspTask->next != 0){
    unSuspTask->next->prev = unSuspTask->prev;
  }


  readyTask = YKRdyList;
  while (readyTask->priority < unSuspTask->priority){
    readyTask = readyTask->next;
  }
  if(readyTask->prev == 0){
    YKRdyList = unSuspTask;
  }
  else{
    readyTask->prev->next = unSuspTask;
  }
  unSuspTask->prev = readyTask->prev;
  unSuspTask->next = readyTask;
  readyTask->prev = unSuspTask;

  unSuspTask->semWait = 0;

  if( YKISRDepth == 0){
    YKScheduler(1);
  }
  YKExitMutex();
  return;
}
