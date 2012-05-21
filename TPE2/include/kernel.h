
/********************************** 
*
*  Kernel
*
***********************************/

#ifndef _KERNEL_H
#define _KERNEL_H

#include "video.h"
#include "shell.h"
#include "devices.h"
#include "kasm.h"
#include "defs.h"
#include "mouse.h"
#include "structs.h"
#include "scheduler.h"
#include "multiboot.h"
#include "limits.h"
#include "memorym.h"
#include "auxMem.h"
//#include "memo.h"

#define OS_PID	0

int (*player)(void);

void printTicks();
void changeTTY(int);

typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

// Frecuencia a la que interrumpira el PIT en la IRQ_0
void timer_phase( int hz );

void print_header();

/* __write
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
*
**/
size_t __write(int fd, const void* buffer, size_t count);

/* __read
*
* Recibe como parametros:
* - File Descriptor
* - Buffer a donde escribir
* - Cantidad
*
**/
size_t __read(int fd, void* buffer, size_t count);

size_t __kwrite(int fd, const void* buffer, size_t count);

int shellLoop(int argc, char * argv);

void init(void);

#endif
