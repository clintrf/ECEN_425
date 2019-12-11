/*
File: lab8defs.h
Revision date: 
Description: Required definitions for EE 425 lab 8
*/

#include "yakk.h"

extern YKQ *movePieceQueue;
extern YKQ *newPieceQueue;

extern YKSEM *movePieceSem;

#define TASK_STACK_SIZE 512			
#define PIECE_QUEUE_SIZE 50		
#define MOVE_QUEUE_SIZE 50

#define READY 1
#define NOT_READY 0

#define STRAIGHT 1
#define CONER 0

#define MOVE_RIGHT 1
#define MOVE_LEFT 0

#define TURN_RIGHT 1
#define TURN_LEFT 0

struct newPiece{

    unsigned id;
    unsigned type;
    unsigned orient;
    unsigned col;
    /*
 *  0   1   2   3
 *
 *  o    o  oo  oo
 *  oo  oo   o  o
 *      
 *  ooo o
 *      o
 *      o 
    */
};


struct movePiece{
    unsigned id;
    int movement;
    void (*function)(int, int); //ID , movement
};

