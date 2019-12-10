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
# 15 "yakk.h"
extern unsigned int YKCtxSwCount;

extern unsigned int YKIdleCount;
extern unsigned int YKTickNum;


typedef struct YKEVENT
{
  int active;
  unsigned flag;
} YKEVENT;
# 46 "yakk.h"
typedef struct taskblock *TCBptr;
typedef struct taskblock
{

    int *stackptr;
    int state;
    int priority;
    int delay;
    TCBptr next;
    TCBptr prev;



    YKEVENT *event;
    unsigned eventMask;
    int waitMode;
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

YKEVENT *YKEventCreate(unsigned initialValue);
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);
void YKEventSet(YKEVENT *event, unsigned eventMask);
void YKEventReset(YKEVENT *event, unsigned eventMask);
# 98 "yakk.h"
void YKEnterMutex(void);
void YKExitMutex(void);
void YKDispatcherNSave(int *restore_sp);
void YKDispatcherSave(int ** save_sp, int *restore_sp);
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


YKEVENT YKEVENTArray[2];

TCBptr YKSemWaitList;
TCBptr YKQWaitList;
TCBptr YKEventWaitList;

TCBptr TKCurrentlyRunning;

char run_flag = 0;

void YKInitialize(void){
  int i;
  YKCtxSwCount = 0;
  YKIdleCount = 0;
  TKCurrentlyRunning = 0;
  YKISRDepth = 0;
  YKTickNum = 0;

  YKEnterMutex();

  YKAvailTCBList = &(YKTCBArray[0]);

  for (i = 0; i < 9; i++){
    YKTCBArray[i].next = &(YKTCBArray[i+1]);
  }
  YKTCBArray[9].next = 0;
# 67 "yakc.c"
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
    if (tmp2->prev == 0){
      YKRdyList = tmp;
    }
    else{
      tmp2->prev->next = tmp;
    }
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

  YKEnterMutex();

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
    YKScheduler(0);
  }
}

void YKTickHandler(void){
  TCBptr tempDelay, tempReady, tempNext;
  tempDelay = YKDelayList;
  YKEnterMutex();

  YKTickNum = YKTickNum + 1;

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
# 484 "yakc.c"
YKEVENT *YKEventCreate(unsigned initialValue){
  int i;
  YKEnterMutex();
  for (i = 0; YKEVENTArray[i].active; i++){};

  YKEVENTArray[i].active = 1;
  YKEVENTArray[i].flag = initialValue;

  return &(YKEVENTArray[i]);
}

unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode){
  TCBptr readyTask;

  YKEnterMutex();

  if(((waitMode == 0) && ((eventMask & event->flag ) > 0 )) ||
     ((waitMode == 1) && ((eventMask & event->flag ) == eventMask)) ){
    YKExitMutex();
    return event->flag;
  }

  readyTask = YKRdyList;
  YKRdyList = readyTask->next;
  readyTask->next->prev = 0;
  readyTask->next = YKEventWaitList;
  YKEventWaitList = readyTask;
  readyTask->prev = 0;

  if(readyTask->next != 0){
    readyTask->next->prev = readyTask;
  }

  readyTask->event = event;
  readyTask->eventMask = eventMask;
  readyTask->waitMode = waitMode;

  YKScheduler(1);
  YKExitMutex();

  return event->flag;
}

void YKEventSet(YKEVENT *event, unsigned eventMask){
  TCBptr eventTask, unWaitTask, readyTask, taskNext;
  YKEnterMutex();
  event->flag |= eventMask;

  for(eventTask = YKEventWaitList; eventTask != 0; ){
    taskNext=eventTask->next;
    if(eventTask->event != event){
      eventTask = taskNext;
      continue;
    }
    else{
      if( ((eventTask->waitMode == 0) && ((eventTask->eventMask & event->flag ) > 0 )) ||
          ((eventTask->waitMode == 1) && ((eventTask->eventMask & event->flag) == eventTask->eventMask)) ){
        unWaitTask = eventTask;

        if(unWaitTask->prev == 0){
          YKEventWaitList = unWaitTask->next;
        }
        else{
          unWaitTask->prev->next = unWaitTask->next;
        }
        if (unWaitTask->next != 0){
          unWaitTask->next->prev = unWaitTask->prev;
        }
        readyTask = YKRdyList;
        while (readyTask->priority < unWaitTask->priority){
          readyTask = readyTask->next;
        }
        if(readyTask->prev == 0){
          YKRdyList = unWaitTask;
        }
        else{
          readyTask->prev->next = unWaitTask;
        }
        unWaitTask->prev = readyTask->prev;
        unWaitTask->next = readyTask;
        readyTask->prev = unWaitTask;

        unWaitTask->event = 0;
      }
      else{
        unWaitTask = 0;
      }
    }
    eventTask = taskNext;
  }
  if(YKISRDepth == 0){
    YKScheduler(1);
  }
  YKExitMutex();
}

void YKEventReset(YKEVENT *event, unsigned eventMask){
  YKEnterMutex();
  event->flag &= ~eventMask;
  YKExitMutex();
}
