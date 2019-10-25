# 1 "lab4c_app.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "lab4c_app.c"






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
# 8 "lab4c_app.c" 2
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
# 9 "lab4c_app.c" 2



int TaskStack[256];

void Task(void);

void main(void)
{
    YKInitialize();

    printString("Creating task...\n");
    YKNewTask(Task, (void *) &TaskStack[256], 0);

    printString("Starting kernel...\n");
    YKRun();
}

void Task(void)
{
    unsigned idleCount;
    unsigned numCtxSwitches;

    printString("Task started.\n");
    while (1)
    {
        printString("Delaying task...\n");

        YKDelayTask(2);

        YKEnterMutex();
        numCtxSwitches = YKCtxSwCount;
        idleCount = YKIdleCount;
        YKIdleCount = 0;
        YKExitMutex();

        printString("Task running after ");
        printUInt(numCtxSwitches);
        printString(" context switches! YKIdleCount is ");
        printUInt(idleCount);
        printString(".\n");
    }
}
