#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

int newPieceTaskStk[TASK_STACK_SIZE];
int movePieceTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];

int moveQueueIndex;
int corner_flag =  0;

struct movePiece movePieceList[MOVE_QUEUE_SIZE];
struct newPiece newPieceList[PIECE_QUEUE_SIZE];

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
           colPiece = message->col;
          if(colPiece == 5){
              YKSemPend(movePieceSem);
              i = getIndex();
              movePieceList[i].id = message->id;
              movePieceList[i].movement = MOVE_LEFT;
              movePieceList[i].function = SlidePiece;
              YKQPost(movePieceQueue, &movePieceList[i]);
          }
          else{
              while(colPiece <4){
                  YKSemPend(movePieceSem);
               i = getIndex();
              movePieceList[i].id = message->id;
              movePieceList[i].movement = MOVE_RIGHT;
              movePieceList[i].function = SlidePiece;
              colPiece++;
              YKQPost(movePieceQueue, &movePieceList[i]);              
              }
          }
          if (message->orient){
              YKSemPend(movePieceSem);
              i = getIndex();
              movePieceList[i].id = message->id;
              movePieceList[i].movement = TURN_RIGHT;
              movePieceList[i].function = RotatePiece;
              YKQPost(movePieceQueue, &movePieceList[i]);
         }
      }
      else{
          colPiece = message->col;
          if(colPiece == 5){
              YKSemPend(movePieceSem);
              i = getIndex();
              movePieceList[i].id = message->id;
              movePieceList[i].movement = MOVE_LEFT;
              movePieceList[i].function = SlidePiece;
              colPiece--;
              YKQPost(movePieceQueue, &movePieceList[i]);
          }
          else if(colPiece == 0){
              YKSemPend(movePieceSem);
              i = getIndex();
              movePieceList[i].id = message->id;
              movePieceList[i].movement = MOVE_RIGHT;
              movePieceList[i].function = SlidePiece;
              colPiece++;
              YKQPost(movePieceQueue, &movePieceList[i]);              
          }
          if (!corner_orient){
               corner_orient = 1;
              switch(message->orient){
                  case 1:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_RIGHT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
                  case 2:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_LEFT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
                  case 3:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_LEFT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
              }
              while(colPiece > 0){
                  YKSemPend(movePieceSem);
                  i = getIndex();
                  movePieceList[i].id = message->id;
                  movePieceList[i].movement = MOVE_LEFT;
                  movePieceList[i].function = SlidePiece;
                  colPiece--;
                  YKQPost(movePieceQueue, &movePieceList[i]);
             }
          }
          else{
              corner_orient = 0;
              switch(message->orient){
                  case 0:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_LEFT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
                  case 1:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_LEFT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
                  case 3:
                      YKSemPend(movePieceSem);
                      i = getIndex();
                      movePieceList[i].id = message->id;
                      movePieceList[i].movement = TURN_RIGHT;
                      movePieceList[i].function = RotatePiece;
                      YKQPost(movePieceQueue, &movePieceList[i]);
                      break;
              }
              while(colPiece > 2){
                  YKSemPend(movePieceSem);
                  i = getIndex();
                  movePieceList[i].id = message->id;
                  movePieceList[i].movement = TURN_LEFT;
                  movePieceList[i].function = RotatePiece;
                  colPiece--;
                  YKQPost(movePieceQueue, &movePieceList[i]);
              }
              while(colPiece < 2){
                  YKSemPend(movePieceSem);
                  i = getIndex();
                  movePieceList[i].id = message->id;
                  movePieceList[i].movement = TURN_LEFT;
                  movePieceList[i].function = RotatePiece;
                  colPiece++;
                  YKQPost(movePieceQueue, &movePieceList[i]);
              }
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

    SeedSimptris(87245);
    
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
    //movePieceEvent = YKEventCreate(READY);
    
    printString(" ");

    YKNewTask(STask, (void *) &STaskStk[TASK_STACK_SIZE], 0);
    YKRun();

}		
