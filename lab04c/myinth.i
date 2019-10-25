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
# 3 "myinth.c" 2

void delay();

extern int KeyBuffer;




void c_reset_handler(){
 printString("\nRESET PROGRAM\n");
 exit(0);
}


void c_tick_handler(){
 static unsigned int tick = 1;
 printString("\nTICK ");
 printInt(tick++);
 printNewLine();

 YKTickHandler();
}

void c_key_handler(){
 int i=0;
 char c = (char) KeyBuffer;
 if (c == 'd'){
  printString("\r\nDELAY KEY PRESSED\r\n");

  for(i = 0; i < 5000; i=i){
   i++;
  }
  printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
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
