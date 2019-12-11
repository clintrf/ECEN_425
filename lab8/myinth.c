#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"


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
    printString("\nNEW PIECE\n");
    newPieceList[i].id = NewPieceID;
    newPieceList[i].type = NewPieceType;
    newPieceList[i].orient = NewPieceOrientation;
    newPieceList[i].col = NewPieceColumn;
    
    YKQPost(newPieceQueue, (void *) &(newPieceList[i]));
    i++;
    if(i == PIECE_QUEUE_SIZE){
        i = 0;
    }
}

void c_received_handler(void){
    //YKSemPost(movePieceSem);
    YKEventSet(movePieceEvent, 1);
}


