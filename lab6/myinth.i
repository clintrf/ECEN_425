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





typedef struct YKQ
{
    int size;
    int length;
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

YKQ *YKQCreate(void **start, unsigned size);
void *YKQPend(YKQ *queue);
int YKQPost(YKQ *queue, void *msg);


void YKEnterMutex(void);
void YKExitMutex(void);
void YKDispatcherNSave(int *restore_sp);
void YKDispatcherSave(int ** save_sp, int *restore_sp);
YKSEM* YKSemCreate(int initialValue);
void YKSemPend(YKSEM *semaphore);
void YKSemPost(YKSEM *semaphore);
# 3 "myinth.c" 2
# 1 "lab6defs.h" 1
# 11 "lab6defs.h"
struct msg
{
    int tick;
    int data;
};
# 4 "myinth.c" 2
# 12 "myinth.c"
extern struct msg MsgArray[20];
extern YKQ *MsgQPtr;
extern int GlobalFlag;

void c_reset_handler(){
 exit(0);
}

void c_tick_handler(){
    static int next = 0;
    static int data = 0;
    YKTickHandler();


    MsgArray[next].tick = YKTickNum;
    data = (data + 89) % 100;
    MsgArray[next].data = data;

    if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0){
        printString("  TickISR: queue overflow! \n");
    }
    else if (++next >= 20){
        next = 0;
    }






}

void c_key_handler(){
 GlobalFlag = 1;
# 61 "myinth.c"
}
