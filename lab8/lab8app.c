#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

int newPieceTaskStk[TASK_STACK_SIZE];
int movePieceTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];

int moveQueueIndex;

struct movePiece movePieceList[MOVE_QUEUE_SIZE];

void * newPieceQue[PIECE_QUEUE_SIZE];
void * movePieceQue[MOVE_QUEUE_SIZE];

YKQ *movePieceQueue;
YKQ *newPieceQueue;

YKSEM *movePieceSem;


int getIndex(void){
	if(moveQueueIndex == MOVE_QUEUE_SIZE ){
		moveQueueIndex = 0;
	}

	return moveQueueIndex ++;
}


int newPieceTask(void){
    static int corner_orient = 0;
    int i = 0;
    int colPiece;
    struct newPiece *message;
    
    while(1){
        message = (struct newPiece *) YKQPend(newPieceQueue);
        if(message->type == STRAIGHT){
            printString("straight \r\n");
            colPiece = message->col;
            if(colPiece == 5){
                i = getIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = MOVE_LEFT;
                movePieceList[i].function = SlidePiece;
                YKQPost(movePieceQueue, &movePieceList[i]);
            }
            else{
                while(colPiece <4){
                 i = getIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = MOVE_RIGHT;
                movePieceList[i].function = SlidePiece;
                colPiece++;
                YKQPost(movePieceQueue, &movePieceList[i]);               
                }
            }
            if (message->orient){
                i = getIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = TURN_RIGHT;
                movePieceList[i].function = RotatePiece;
                YKQPost(movePieceQueue, &movePieceList[i]);
           }
        }
        else{
            colPiece = message->col;
            while(colPiece > 1){
                 i = getIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = MOVE_LEFT;
                movePieceList[i].function = SlidePiece;
                colPiece--;
                YKQPost(movePieceQueue, &movePieceList[i]);               
            }
        }
    }              
}

int movePieceTask(void){
    struct movePiece *message;
    while(1){
        YKSemPend(movePieceSem);
        message = (struct movePiece *) YKQPend(movePieceQueue);
        message->function(message->id, message->movement);
    }
}

void STask(void){
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
    
    YKNewTask(newPieceTask, (void *) &newPieceTaskStk[TASK_STACK_SIZE], 3);
    YKNewTask(movePieceTask, (void *) &movePieceTaskStk[TASK_STACK_SIZE], 5);
    
    
	
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

    YKNewTask(STask, (void *) &STaskStk[TASK_STACK_SIZE], 0);
    YKRun();

}		
