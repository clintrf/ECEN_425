#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

int newPieceTaskStk[TASK_STACK_SIZE];
int movePieceTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];

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

    SeedSimptris(836);
    
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
    newPieceQueue = YKQCreate(newPieceQue, PIECE_QUEUE_SIZE);
    movePieceQueue = YKQCreate(movePieceQue, MOVE_QUEUE_SIZE);
    movePieceSem = YKSemCreate(READY);
    
    printString(" ");

    YKNewTask(STask, (void *) &STaskStk[STACK_SIZE], 0);
    YKRun();

}		
