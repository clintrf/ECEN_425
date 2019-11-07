#include "clib.h"
#include "yakk.h"



extern int KeyBuffer;
extern YKSEM *NSemPtr;
extern void YKSemPost(YKSEM *semaphore);
void delay();
extern void YKTickHandler(void);
#define DELAY 5000

void c_reset_handler(){
	printString("\nRESET PROGRAM\n");
	exit(0);
}

void c_tick_handler(){
	static int tick = 0;
	printString("\nTICK ");
	printInt(tick++);
	printNewLine();

	YKTickHandler(); //lab4c
}

void c_key_handler(){
	char c = (char) KeyBuffer;
	if (c == 'd'){
		printString("\r\nDELAY KEY PRESSED\r\n");
		delay();
		printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
	}
	else if(c == 'p'){
		printString("\r\n P Key Pressed pppppppppppppppppppppppppppp\r\n");
		YKSemPost(NSemPtr);
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
