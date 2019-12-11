#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"


extern int KeyBuffer;
extern unsigned NewPieceID;
extern unsigned NewPieceType;
extern unsigned NewPieceOrient;
extern unsigned NewPieceCol;

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

void game_over_handler(void){
    printString("\nGAME OVER\n");
    exit(0);
}

void new_piece_handler(void){
    static int i = 0;
    newPieceList[i].id = NewPieceID;
    newPieceList[i].type = NewPieceType;
    newPieceList[i].orient = NewPieceOrient;
    newPieceList[i].col = NewPieceCol;
    
    YKQPost(newPieceQueue, (void *) &(newPieceList[i]));
    i++;
    if(i == PIECE_QUEUE_SIZE){
        i = 0;
    }
}

void received_handler(void){
    YKSemPost(movePieceSem);
}


