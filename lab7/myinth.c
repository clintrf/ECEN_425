#include "clib.h"
#include "yakk.h"
#include "lab7defs.h"


extern int KeyBuffer;
//extern YKSEM *NSemPtr; //lab05
//void delay();
//extern void YKTickHandler(void);
//#define DELAY 5000

//extern struct msg MsgArray[MSGARRAYSIZE];
// extern YKQ *MsgQPtr;
// extern int GlobalFlag;

void c_reset_handler(){
	exit(0);
}

void c_tick_handler(){
    // static int next = 0;
    // static int data = 0;
    YKTickHandler(); //lab4c

    /* create a message with tick (sequence #) and pseudo-random data */
    // MsgArray[next].tick = YKTickNum;
    // data = (data + 89) % 100;
    // MsgArray[next].data = data;

    // if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0){
    //     printString("  TickISR: queue overflow! \n");
    // }
    // else if (++next >= MSGARRAYSIZE){
    //     next = 0;
    // }

	// static int tick = 0;
	// printString("\nTICK ");
	// printInt(tick++);
	// printNewLine();

}

void c_key_handler(){
	char c;
    c = KeyBuffer;

    if(c == 'a') YKEventSet(charEvent, EVENT_A_KEY);
    else if(c == 'b') YKEventSet(charEvent, EVENT_B_KEY);
    else if(c == 'c') YKEventSet(charEvent, EVENT_C_KEY);
    else if(c == 'd') YKEventSet(charEvent, EVENT_A_KEY | EVENT_B_KEY | EVENT_C_KEY);
    else if(c == '1') YKEventSet(numEvent, EVENT_1_KEY);
    else if(c == '2') YKEventSet(numEvent, EVENT_2_KEY);
    else if(c == '3') YKEventSet(numEvent, EVENT_3_KEY);
    else {
        print("\nKEYPRESS (", 11);
        printChar(c);
        print(") IGNORED\n", 10);
    }
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
