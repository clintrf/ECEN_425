#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

int newPieceTaskStk[TASK_STACK_SIZE];
int movePieceTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];


int newPieceTask(void){
    int i = 0;
    struct newPiece *message;
    while(1){
        message = (struct newPiece *) YKQPend(newPieceQueue);

        if(message->type == PIECE_TYPE_STRAIGHT){
             pieceCol = message->col;
            if(pieceCol == 5){
                i = getMovePieceQueueListIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = LEFT;
                movePieceList[i].function = SlidePiece;
                YKQPost(movePieceQueue, &movePieceList[i]);
            }
            else{
                while(pieceCol <4){
                 i = getMovePieceQueueListIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = RIGHT;
                movePieceList[i].function = SlidePiece;
                YKQPost(movePieceQueue, &movePieceList[i]);               
                }
            }
            if (message->orient){
                i = getMovePieceQueueListIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = TURN_RIGHT;
                movePieceList[i].function = RotatePiece;
                YKQPost(movePieceQueue, &movePieceList[i]);
           }
        }
        else{
            pieceCol = message->col;
            if(pieceCol == 5){
                i = getMovePieceQueueListIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = LEFT;
                movePieceList[i].function = SlidePiece;
                pieceCol--;
                YKQPost(movePieceQueue, &movePieceList[i]);
            }
            else if(pieceCol == 0){
                i = getMovePieceQueueListIndex();
                movePieceList[i].id = message->id;
                movePieceList[i].movement = RIGHT;
                movePieceList[i].function = SlidePiece;
                pieceCol++;
                YKQPost(movePieceQueue, &movePieceList[i]);              
            }
            if (!corner_orient){
                 corner_orient = 1;
                switch(message->orient){
                    case 1:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_RIGHT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                    case 2:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_LEFT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                    case 3:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_LEFT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                }
                while(pieceCol > 0){
                    i = getMovePieceQueueListIndex();
                    movePieceList[i].id = message->id;
                    movePieceList[i].movement = TURN_LEFT;
                    movePieceList[i].function = RotatePiece;
                    pieceCol--;
                    YKQPost(movePieceQueue, &movePieceList[i]);
               }
            }
            else{
                corner_orient = 0;
                switch(message->orient){
                    case 1:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_LEFT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                    case 2:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_LEFT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                    case 3:
                        i = getMovePieceQueueListIndex();
                        movePieceList[i].id = message->id;
                        movePieceList[i].movement = TURN_RIGHT;
                        movePieceList[i].function = RotatePiece;
                        YKQPost(movePieceQueue, &movePieceList[i]);
                        break;
                }
                while(pieceCol > 2){
                    i = getMovePieceQueueListIndex();
                    movePieceList[i].id = message->id;
                    movePieceList[i].movement = TURN_LEFT;
                    movePieceList[i].function = RotatePiece;
                    pieceCol--;
                    YKQPost(movePieceQueue, &movePieceList[i]);
                }}
                while(pieceCol < 2){
                    i = getMovePieceQueueListIndex();
                    movePieceList[i].id = message->id;
                    movePieceList[i].movement = TURN_LEFT;
                    movePieceList[i].function = RotatePiece;
                    pieceCol++;
                    YKQPost(movePieceQueue, &movePieceList[i]);
                }
            }
        }
    }
}

int movePieceTask(void){
    struct movePiece *message;
    while(1){
        YKSemPend();
        message = (struct movePiece *) YKQPend(movePieceQueue);
        message->function(message->id, message->movement);
    }
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
