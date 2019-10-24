#include "clib.h"
#include "yakk.h"

void delay();

extern int KeyBuffer;

#define DELAY 5000


void c_reset_handler(){
	printString("\nRESET PROGRAM\n");
	exit(0);
}


void c_tick_handler(){
	static unsigned int tick = 0;
	YKTickHandler();
	printString("\nTICK ");
	printInt(tick++);
	printNewLine();
}

void c_key_handler(){
	char c = (char) KeyBuffer;
	if (c == 'd'){
		printString("\r\nDELAY KEY PRESSED\r\n");
		delay();
		printString("\r\nDELAY COMPLETE\r\n");
	}
	else{
		printString("\r\nKEYPRESS (");
		printChar(c);
		printString(") IGNORED\r\n");
	}
}

void delay(){
	int i=0;
	for(i = 0; i < DELAY; i++){}
}
