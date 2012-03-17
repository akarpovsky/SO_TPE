/*
 * fifo.c
 *
 *  Created on: 16/03/2012
 *      Author: Facundo
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/defines.h"
#include "../includes/message.h"

#define FIFO_OUT "/tmp/fifo"

char * fifoOut;
char * fifoIn;
int fdIn, fdOut;

msg_s * comunicate(msg_t * msg)
{
	if(sendmessage(msg)){
		return rcvmessage();
	}

	return NULL;
}

int sendmessage(msg_t * msg)
{
	int msgSize;
	void * msgstr;
	void * msgstraux;

	switch(msg->type)
	{
	case REGISTER:
		int passSize = strlen(msg->data.register_t.pass);
		int userSize = strlen(msg->data.register_t.user);
		msgSize = 3*sizeof(int)+passSize+userSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &msgSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(userSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, msg->data.register_t.user, userSize);
		msgstraux += userSize;
		memcpy(msgstraux, &passSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, msg->data.register_t.pass, passSize);
		break;
	case LOG_IN:
		int passSize = strlen(msg->data.login_t.pass);
		int userSize = strlen(msg->data.login_t.user);
		msgSize = 3*sizeof(int)+passSize+userSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(userSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, msg->data.login_t.user, userSize);
		msgstraux += userSize;
		memcpy(msgstraux, &passSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, msg->data.login_t.pass, passSize);
		break;
	case LOG_OUT:
		msgSize = sizeof(int);
		msgstr = malloc(msgSize);
		memcpy(msgstr, &(msg->type), sizeof(int));
		break;
	case LIST:
		break;
	case JOIN:
		break;
	case CREATE:
		break;
	case PICK:
		break;
	case SHOW:
		break;
	case TRADE:
		break;
	}

	int nwrite;
	if((nwrite = write(fdOut, &msgSize, sizeof(int))) == -1)
	{
		perror("Could not write message size");
		return NULL;
	}
	if((nwrite = write(fdOut, msgstr, msgSize)) == -1)
	{
		perror("Could not write message");
		return NULL;
	}

	free(msgstr);
}

void connectToServer(void)
{
	int readyFlag = FALSE;
	fifoIn = tempnam("/tmp/", NULL);
	fifoOut = malloc(strlen(FIFO_OUT));
	*fifoOut = FIFO_OUT;
	int tempnamSize = strlen(fifoIn);


	if(mkfifo(fifoIn, 0666) ==  -1)
	{
		perror("Input FIFO couldn't be created");
		exit(1);
	}

	if((fdOut = open(fifoOut, O_WRONLY)) == -1)
	{
		perror("Output FIFO couldn't be opened");
		unlink(fifoOut);
		exit(1);
	}

	if((fdIn = open(fifoIn, O_RDONLY)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		close(fdOut);
		unlink(fifoOut);
		exit(1);
	}

	int nwrite = 0;

	if((nwrite = write(fdOut, &tempnamSize, sizeof(int))) == 1)
	{
		perror("Couldn't write output FIFO size to server");
		close(fdOut);
		close(fdIn);
		unlink(fifoIn);
		exit(1);
	}

	if((nwrite = write(fdOut, fifoOut, tempnamSize)) == -1)
	{
		perror("Couldn't write output FIFO to server");
		close(fdOut);
		close(fdIn);
		unlink(fifoIn);
		exit(1);
	}

	do{
		int nread = 0;
		if((nread = read(fdIn, &tempnamSize, sizeof(int))) == -1)
		{
			perror("Couldn't read input FIFO size from server");
			close(fdOut);
			close(fdIn);
			unlink(fifoIn);
			exit(1);
		}
		else if(nread > 0)
		{
			char * temp = malloc(tempnamSize);

			if((nread = read(fdIn, temp, tempnamSize)) == -1)
			{
				perror("Couldn't read input FIFO from server");
				close(fdOut);
				close(fdIn);
				unlink(fifoIn);
				exit(1);
			}
			else if(nread > 0)
			{
				free(fifoOut);
				fifoOut = temp;
				readyFlag = TRUE;
			}
		}
	}while(!readyFlag);
}
