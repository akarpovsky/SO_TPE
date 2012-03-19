/*
 * fifo_s.c
 *
 *  Created on: 18/03/2012
 *      Author: neon
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/message.h"
#include "../includes/defines.h"

#define FIFO_IN "/tmp/fifo"
int fdOut;
int fdIn;
char * fifoIn;
char * fifoOut;

void uplink(void)
{
	if((fifoIn = malloc(strlen(FIFO_IN))) == NULL)
	{
		 perror("Output FIFO name couldn't be allocated");
		 exit(1);
	}

	*fifoIn = FIFO_IN;

	if((fdIn = mkfifo(fifoIn, 0666)) == -1)
	{

	}

}


