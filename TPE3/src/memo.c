/*
 * memo.c
 *
 *  Created on: 21/05/2012
 *      Author: neon
 */
#include "../include/memo.h"
char heap_space[1024*1024*16];
size_t my_offset;
// Roughly allocs some memory
void * my_malloc(size_t size)
{
     void* ret = 0;
     ret = heap_space + my_offset;
     my_offset += size;
     return ret;
}

void * getFreePage()
{
	return my_malloc(3*4096) + 2*4096 -1;
}

void free(void * p)
{
	return;
}
