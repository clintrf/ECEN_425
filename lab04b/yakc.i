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
extern TCBptr YKSuspList;
extern TCBptr YKAvailTCBList;
extern TCB YKTCBArray[4 +1];


void YKInitialize(void);
void YKEnterMutex(void);
void YKExitMutex(void);

void YKIdleTask(void);
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);
void YKRun(void);

void YKScheduler( int save_flag);







void YKDispatcherSave(int saveFlag, int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss);
void YKDispatcherNSave(int saveFlag,int ** save_sp, int ** save_ss, int *restore_sp, int * restore_ss);
# 3 "yakc.c" 2






unsigned int YKCtxSwCount;

unsigned int YKIdleCount;


unsigned int YKISRCallDepth;

char run_flag = 0;

TCBptr TKCurrentlyRunning;
int idleStack[256];

TCBptr YKRdyList;
TCBptr YKSuspList;
TCBptr YKAvailTCBList;
TCB YKTCBArray[4 +1];

void YKInitialize(void){
  int i;
  YKCtxSwCount = 0;
  YKIdleCount = 0;


  YKISRCallDepth = 0;
  TKCurrentlyRunning = 0;

  YKEnterMutex();


  YKAvailTCBList = &(YKTCBArray[0]);

  for (i = 0; i < 4; i++)
   YKTCBArray[i].next = &(YKTCBArray[i+1]);
  YKTCBArray[4].next = 0;

  printString("before new task...\n");

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


  tmp = YKAvailTCBList;
  YKAvailTCBList = tmp->next;




  tmp->delay = 0;
  tmp->priority = priority;


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

  YKExitMutex();





  tmp->stackptr = taskStack;

  printString("Address for new task's SP is ");
  printInt((int) &(tmp->stackptr));
  printString("\n");

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

  printString("before Scheduler...\n");

  YKScheduler(1);

}

void YKRun(void){
  run_flag = 1;
  YKScheduler(0);
}

void YKScheduler(int save_flag){
  TCBptr highest_priority_task = YKRdyList;
  TCBptr currentlyRunning = TKCurrentlyRunning;
  if(!run_flag){
    return;
  }
  if (TKCurrentlyRunning == highest_priority_task){
    return;
  }
  YKCtxSwCount = YKCtxSwCount + 1;
  TKCurrentlyRunning = highest_priority_task;

  printString("before dipatcher...\n");

  if(!save_flag){
    YKDispatcherSave(0,(int **) 1,(int ** ) 1, highest_priority_task->stackptr, highest_priority_task->ss);
  }
  else{
    YKDispatcherSave(save_flag,&(currentlyRunning->stackptr),&(currentlyRunning->ss), highest_priority_task->stackptr, highest_priority_task->ss);

  }
}
