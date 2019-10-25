# 1 "myinth.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "myinth.c"
# 1 "clib.h" 1



void print(char *string, int length);
void printNewLine(void);
void printChar(char c);
void printString(char *string);


void printInt(int val);
void printLong(long val);
void printUInt(unsigned val);
void printULong(unsigned long val);


void printByte(char val);
void printWord(int val);
void printDWord(long val);


void exit(unsigned char code);


void signalEOI(void);
# 2 "myinth.c" 2

void delay();

extern int KeyBuffer;




void c_reset_handler(){
 printString("\nRESET PROGRAM\n");
 exit(0);
}


void c_tick_handler(){
 static unsigned int tick = 1;
 printString("\nTICK ");
 printInt(tick++);
 printNewLine();

 YKTickHandler();
}

void c_key_handler(){
 char c = (char) KeyBuffer;
 if (c == 'd'){
  printString("\r\nDELAY KEY PRESSED\r\n");
  delay();
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
 for(i = 0; i < 5000; i++){}
}

void print_debug(){
 printString("\r\nDEBUGGER\r\n");
}
