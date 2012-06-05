
#ifndef _IO_H
#define _IO_H

#include "defs.h"
#include "structs.h"
#include "strings.h"
#include "proc.h"
//#include "video.h"

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
void syskernelwrite(int fd, void * buffer, size_t count);
void kprintfcolor(char color, char *fmt, ...);


int getchar();
int sscanf(char* str, char* fmt, ...);
char isNumeric(char c);

char getc();
void debugger(char c);
void puts(char* str);
void putd(int d);
void putu(unsigned int u);
void kputs(char* str);
void kputd(int d);
void kputu(unsigned int u);
void kputx(int x);
void printf(char *fmt, ...);
void kprintf(char *fmt, ...);
void putx(int x);

#endif
