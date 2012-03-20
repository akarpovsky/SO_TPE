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
int * fdOut;
int fdIn;
char * fifoIn;
char * fifoOut;

void uplink(void)
{
	if((fifoIn = malloc(strlen(FIFO_IN))) == NULL)
	{
		 perror("Input FIFO name couldn't be allocated");
		 exit(1);
	}

	*fifoIn = FIFO_IN;

	if(mkfifo(fifoIn, 0666) == -1)
	{
		perror("Input FIFO couldn't be created");
		exit(1);
	}

	if((fdIn = open(fifoIn, O_RDONLY)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		exit(1);
	}

}

void establishChannel(char * newChannel)
{

}

int sendmessage(msg_s * msg){
	
	int msgSize;
	void * msgstr;
	void * msgstraux;
	
	msgSize = strlen(msg->msg);
	msgstr = msgstraux = malloc(msgSize);
	
	memcpy(msgstraux,msg->msg, msgSize);
	
	
	int nwrite;
	
	if((nwrite = write(fdOut, &msgSize, sizeof(int)) == -1))
	{
		perror("Could not write message size");
		return !SUCCESSFUL;
	}
	if((nwrite = write(fdOut, msgstr, msgSize)) == -1)
	{
		perror("Could not write message");
		return !SUCCESSFUL;
	}


	free(msgstr);
	return SUCCESSFUL;
	
}

