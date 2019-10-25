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
# 29 "yakk.h"
extern unsigned int YKCtxSwCount;

extern unsigned int YKIdleCount;
extern unsigned int YKTickNum;
extern unsigned int YKISRDepth;




typedef struct taskblock *TCBptr;
typedef struct taskblock
{

    int *stackptr;
    int *ss;
    int state;
    int priority;
    int delay;
    TCBptr next;
    TCBptr prev;
} TCB;

extern TCBptr YKRdyList;
extern TCBptr YKDelayList;
extern TCBptr YKAvailTCBList;
extern TCB YKTCBArray[3 +1];


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
void YKDispatcherNSave(int *restore_sp, int * restore_ss);
void YKDispatcherSave(int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss);
# 3 "yakc.c" 2



unsigned int YKCtxSwCount;

unsigned int YKIdleCount;
unsigned int YKISRDepth;
unsigned int YKTickNum;
char run_flag = 0;

int idleStack[256];

TCBptr YKRdyList;
TCBptr YKDelayList;
TCBptr YKAvailTCBList;
TCB YKTCBArray[3 +1];

TCBptr TKCurrentlyRunning;




void YKInitialize(void){
  int i;
  YKCtxSwCount = 0;
  YKIdleCount = 0;
  TKCurrentlyRunning = 0;




  YKAvailTCBList = &(YKTCBArray[0]);

  for (i = 0; i < 3; i++)
   YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[3].next = 0;

  YKNewTask(YKIdleTask, (void*)&idleStack[256], 100);




}

void YKIdleTask(void){
  while(1){
    YKIdleCount=YKIdleCount+1;
    YKExitMutex();
  }
}

void YKNewTask( void (*task)(void), void *taskStack, unsigned char priority){

  TCBptr tmp, tmp2;

  YKEnterMutex();


  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;


  tmp->priority = priority;
  tmp->delay = 0;


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
  tmp->ss = 0;


  tmp->stackptr = tmp->stackptr - 11;
  *(tmp->stackptr+11) = 0x200;
  *(tmp->stackptr+10) = 0;
  *(tmp->stackptr+9) = (int)task;
  *(tmp->stackptr+8) = 0;
  *(tmp->stackptr+7) = 0;
  *(tmp->stackptr+6) = 0;
  *(tmp->stackptr+5) = 0;
  *(tmp->stackptr+4) = 0;
  *(tmp->stackptr+3) = 0;
  *(tmp->stackptr+2) = 0;
  *(tmp->stackptr+1) = 0;
  *(tmp->stackptr+0) = 0;

  YKScheduler(1);
  YKExitMutex();
}

void YKRun(void){
  run_flag = 1;
  YKScheduler(0);
}

void YKScheduler(int save_flag){
  int testVar;
  int* testPt;
  TCBptr highest_priority_task;
  TCBptr currentlyRunning;

  YKEnterMutex();
  printString("Entering Scheduler\n\r");
  highest_priority_task = YKRdyList;
  currentlyRunning = TKCurrentlyRunning;

  printString("Comp ");
  printInt(TKCurrentlyRunning->priority);
  printString(" ");
  printInt(highest_priority_task->priority);
  printNewLine();

  if(!run_flag || (TKCurrentlyRunning == highest_priority_task)){
    return;
  }

  YKCtxSwCount = YKCtxSwCount + 1;
  TKCurrentlyRunning = highest_priority_task;
  if(!save_flag){
    printString("NONSAVE\n\r");
    YKDispatcherNSave(highest_priority_task->stackptr, highest_priority_task->ss);
    printString("EXIT NONSAVE DISPATCHER\n\r");
  }
  else{
    printString("SAVE\n\r");
    YKDispatcherSave(&(currentlyRunning->stackptr),&(currentlyRunning->ss),
       highest_priority_task->stackptr, highest_priority_task->ss);
    printString("EXIT SAVE DISPATCHER\n\r");
  }
  YKExitMutex();
}
# 161 "yakc.c"
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

  printInt(YKISRDepth);

 if(YKISRDepth == 0) {
    YKScheduler(1);
  }
}
# 209 "yakc.c"
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
