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
# 3 "myinth.c" 2
# 1 "lab8defs.h" 1
# 9 "lab8defs.h"
extern YKQ *movePieceQueue;
extern YKQ *newPieceQueue;

extern YKSEM *movePieceSem;
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
# 4 "myinth.c" 2


extern int KeyBuffer;
extern unsigned NewPieceID;
extern unsigned NewPieceType;
extern unsigned NewPieceOrientation;
extern unsigned NewPieceColumn;

extern YKQ *newPieceQueue;
extern struct newPiece newPieceList[];

void c_reset_handler(){
    exit(0);
}

void c_tick_handler(){
    YKTickHandler();
}

void c_key_handler(){
    char c;
    c = KeyBuffer;

    print("\nKEYPRESS (", 11);
    printChar(c);
    print(") IGNORED\n", 10);
}

void c_game_over_handler(void){
    printString("\nGAME OVER\n");
    exit(0);
}

void c_new_piece_handler(void){
    static int i = 0;
    newPieceList[i].id = NewPieceID;
    newPieceList[i].type = NewPieceType;
    newPieceList[i].orient = NewPieceOrientation;
    newPieceList[i].col = NewPieceColumn;

    YKQPost(newPieceQueue, (void *) &(newPieceList[i]));
    i++;
    if(i == 50){
        i = 0;
    }
}

void c_received_handler(void){
    YKSemPost(movePieceSem);
}
