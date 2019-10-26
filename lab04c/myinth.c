#include "clib.h"
#include "yakk.h"

void delay();

extern int KeyBuffer;

#define DELAY 200


void c_reset_handler(){
	printString("\nRESET PROGRAM\n");
	exit(0);
}


void c_tick_handler(){
	static unsigned int tick = 1;
	printString("\nTICK ");
	printInt(tick++);
	printNewLine();
	
	YKTickHandler(); //lab4c
}

void c_key_handler(){
	int i=0;
	char c = (char) KeyBuffer;
	if (c == 'd'){
		printString("\r\nDELAY KEY PRESSED\r\n");
		// delay();
		for(i = 0; i < DELAY; i=i){
			i++;
		}
		printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
	}
	else{
		printString("\r\nKEYPRESS (");
		printChar(c);
		printString(") IGNORED*********************************************\r\n");
	}
}

void delay(){
	int i=0;
	for(i = 0; i < DELAY; i++){}
}

void print_debug(){
	printString("\r\nDEBUGGER\r\n");
}
