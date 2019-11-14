/* 
File: lab6defs.h
Revision date: 4 November
Description: Required definitions for EE 425 lab 6 (Message queues)
*/
#ifndef LAB6DEFS
#define LAB6DEFS

#define MSGARRAYSIZE      20

struct msg 
{
    int tick;
    int data;
};
#endif