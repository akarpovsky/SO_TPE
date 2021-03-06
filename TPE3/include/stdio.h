
/***************************************************
  Stdio.h
	
****************************************************/

#ifndef _STDIO_H
#define _STDIO_H
#include "../include/structs.h"
#include "../include/defs.h"



int getchar();
int sscanf(char* str, char* fmt, ...);
int isNumeric(char c);

char getc();
void debugger(char c);
void puts(char* str);
void putd(int d);
void putu(unsigned int u);
int printf(char *fmt, ...);


#endif
