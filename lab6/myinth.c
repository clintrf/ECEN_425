#include "clib.h"
#include "yakk.h"
#include "lab6defs.h"


//extern int KeyBuffer; //lab05
//extern YKSEM *NSemPtr; //lab05
//void delay();
//extern void YKTickHandler(void);
//#define DELAY 5000

extern struct msg MsgArray[MSGARRAYSIZE];
extern YKQ *MsgQPtr;
extern int GlobalFlag;

void c_reset_handler(){
	exit(0);
}

void c_tick_handler(){
    static int next = 0;
    static int data = 0;

    //YKTickHandler(); //lab4c

    /* create a message with tick (sequence #) and pseudo-random data */
    MsgArray[next].tick = YKTickNum;
    data = (data + 89) % 100;
    MsgArray[next].data = data;

    if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0){
        printString("  TickISR: queue overflow! \n");
    }
    else if (++next >= MSGARRAYSIZE){
        next = 0;
    }

	// static int tick = 0;
	// printString("\nTICK ");
	// printInt(tick++);
	// printNewLine();

}

void c_key_handler(){
	GlobalFlag = 1;
	// char c = (char) KeyBuffer;
	// if (c == 'd'){
	// 	printString("\r\nDELAY KEY PRESSED\r\n");
	// 	delay();
	// 	printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
	// }
	// else if(c == 'p'){
	// 	printString("\r\n P KEY PRESSED\r\n");
	// 	YKSemPost(NSemPtr);
	// }
	// else{
	// 	printString("\r\nKEYPRESS (");
	// 	printChar(c);
	// 	printString(") IGNORED*********************************************\r\n");
	// }
}

/*
void delay(){
	int i=0;
	for(i = 0; i < DELAY; i++){}
}

void print_debug(){
	printString("\r\nDEBUGGER\r\n");
}
*/
