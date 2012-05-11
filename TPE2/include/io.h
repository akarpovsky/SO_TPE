
#ifndef _IO_H
#define _IO_H
#include "structs.h"

/********************************** 
*
*  io.h: Funciones para manejo
*      de entrada/salida
*
***********************************/

unsigned char _in(unsigned char port);
void _out(unsigned char port, unsigned char c); 
void sysread(int fd, void * buffer, size_t count);
void syswrite(int fd, void * buffer, size_t count);

int getchar();
int sscanf(char* str, char* fmt, ...);
char isNumeric(char c);

char getc();
void debugger(char c);
void puts(char* str);
void putd(int d);
void putu(unsigned int u);
int printf(char *fmt, ...);
void putx(int x);

#endif
