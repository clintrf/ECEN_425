# 1 "lab8app.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "lab8app.c"
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
# 2 "lab8app.c" 2
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


typedef struct YKQ
{
    int size;
    int cur_length;
    void** base_addr;
    int head;
    int tail;
} YKQ;

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
    YKQ *queueWait;

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

YKQ *YKQCreate(void **start, unsigned size);
void *YKQPend(YKQ *queue);
int YKQPost(YKQ *queue, void *msg);

YKSEM *YKSemCreate(int initialValue);
void YKSemPend(YKSEM *semaphore);
void YKSemPost(YKSEM *semaphore);



void YKEnterMutex(void);
void YKExitMutex(void);
void YKDispatcherNSave(int *restore_sp);
void YKDispatcherSave(int ** save_sp, int *restore_sp);
# 3 "lab8app.c" 2
# 1 "lab8defs.h" 1
# 9 "lab8defs.h"
extern YKQ *movePieceQueue;
extern YKQ *newPieceQueue;

extern YKEVENT *movePieceEvent;
# 30 "lab8defs.h"
struct newPiece{

    unsigned id;
    unsigned type;
    unsigned orient;
    unsigned col;
# 46 "lab8defs.h"
};


struct movePiece{
    unsigned id;
    int movement;
    void (*function)(int, int);
};
# 4 "lab8app.c" 2
# 1 "simptris.h" 1
void SlidePiece(int ID, int direction);
void RotatePiece(int ID, int direction);
void SeedSimptris(long seed);
void StartSimptris(void);
# 5 "lab8app.c" 2



void clearLine(){

}

void printScreen(void){

}

int newPieceTask(void){

}

int movePieceTask(void){

}

void STack(void){
    unsigned max, switchCount, idleCount;
    int tmp;

    YKDelayTask(1);
    printString("Welcome to the YAK kernel\r\n");
    printString("Determining CPU capacity\r\n");
    YKDelayTask(1);
    YKIdleCount = 0;
    YKDelayTask(5);
    max = YKIdleCount / 25;
    YKIdleCount = 0;

    SeedSimptris(10);

    YKNewTask(newPieceTask);
    YKNewTask(movePieceTask);

    StartSimptris();

    while(1){
        YKDelayTask(20);
        YKEnterMutex();
        switchCount = YKCtxSwCount;
        idleCount = YKIdleCount;
        YKExitMutex();

        printString("<<<<< Context switches: ");
        printInt((int)switchCount);
        printString(", CPU usage: ");
        tmp = (int) (idleCount/max);
        printInt(100-tmp);
        printString("% >>>>>\r\n");

        YKEnterMutex();
        YKCtxSwCount = 0;
        YKIdleCount = 0;
        YKExitMutex();

    }
}

void main(void){

 YKInitialize();

}
