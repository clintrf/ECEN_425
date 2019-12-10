#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

#define TASK_STACK_SIZE 512			
#define PIECE_QUEUE_SIZE 40		
#define MOVE_QUEUE_SIZE 40




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
