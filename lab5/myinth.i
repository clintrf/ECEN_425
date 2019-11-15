# 1 "myinth.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "myinth.c"
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
# 2 "myinth.c" 2
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
# 3 "myinth.c" 2



extern int KeyBuffer;
extern YKSEM *NSemPtr;

void delay();
extern void YKTickHandler(void);


void c_reset_handler(){
 printString("\nRESET PROGRAM\n");
 exit(0);
}

void c_tick_handler(){
 static int tick = 0;
 printString("\nTICK ");
 printInt(tick++);
 printNewLine();

 YKTickHandler();
}

void c_key_handler(){
 char c = (char) KeyBuffer;
 if (c == 'd'){
  printString("\r\nDELAY KEY PRESSED\r\n");
  delay();
  printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
 }
 else if(c == 'p'){
  printString("\r\n P KEY PRESSED\r\n");
  YKSemPost(NSemPtr);
 }
 else{
  printString("\r\nKEYPRESS (");
  printChar(c);
  printString(") IGNORED*********************************************\r\n");
 }
}

void delay(){
 int i=0;
 for(i = 0; i < 5000; i++){}
}

void print_debug(){
 printString("\r\nDEBUGGER\r\n");
}
