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
# 3 "myinth.c" 2
# 1 "lab7defs.h" 1
# 15 "lab7defs.h"
extern YKEVENT *charEvent;
extern YKEVENT *numEvent;
# 4 "myinth.c" 2


extern int KeyBuffer;

void c_reset_handler(){
 exit(0);
}

void c_tick_handler(){
    YKTickHandler();
}

void c_key_handler(){
    char c;
    c = KeyBuffer;

    if(c == 'a') YKEventSet(charEvent, 0x1);
    else if(c == 'b') YKEventSet(charEvent, 0x2);
    else if(c == 'c') YKEventSet(charEvent, 0x4);
    else if(c == 'd') YKEventSet(charEvent, 0x1 | 0x2 | 0x4);
    else if(c == '1') YKEventSet(numEvent, 0x1);
    else if(c == '2') YKEventSet(numEvent, 0x2);
    else if(c == '3') YKEventSet(numEvent, 0x4);
    else {
        print("\nKEYPRESS (", 11);
        printChar(c);
        print(") IGNORED\n", 10);
    }
}
