#ifndef _AUXMEM_H_
#define _AUXMEM_H_

#include "memorym.h"

/*GDT */
typedef struct{
	void * address;
} DESCR_PAGE;

void setup_DESCR_PAGE(DESCR_PAGE * item, void * address);
void present(DESCR_PAGE * item);
void absent(DESCR_PAGE * item);
void initpages(void * f, void * finMem);
void * getCR3();

void * getRealPointer(void * pointer);


#endif
