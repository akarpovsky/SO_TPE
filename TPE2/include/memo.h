/*
 * memo.h
 *
 *  Created on: 21/05/2012
 *      Author: neon
 */

#ifndef MEMO_H_
#define MEMO_H_

#include "defs.h"
void * my_malloc(size_t size);
void * getFreePage();
void free(void * p);

#endif /* MEMO_H_ */
