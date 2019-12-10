#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"


extern int KeyBuffer;



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

void game_over(void){
    printString("\nGAME OVER\n");
    exit(0);
}

void new_piece(void){
    exit(0);
}

void received(void){
    exit(0);
}

void touchdown(void){
    exit(0);
}

void clear(void){
    exit(0);
}

