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
#include <../includes/marshalling.h>
#include "../includes/defines.h"
#include "../includes/message.h"

#define FIFO_OUT "/tmp/fifo"

char * fifoOut;
char * fifoIn;
int fdIn, fdOut;

msg_s * comunicate(msg_t * msg)
{
	if(sendmessage(msg) == SUCCESSFUL){
		return rcvmessage();
	}

	return NULL;
}

msg_s * rcvmessage(void)
{
	int rcvFlag = FALSE;
	msg_s * msg = malloc(sizeof(msg_s));
	do{
		int nread;
		char * charMsg;
		if((nread = read(fdIn, &(msg->size), sizeof(size_t))) == -1)
		{
			perror("Reading server message size failed");
			return NULL;
		}
		else if(nread > 0)
		{
			charMsg = malloc(msg->size);
			if((nread = read(fdIn, charMsg, msg->size)) == -1)
			{
				perror("Reading server message failed");
				return NULL;
			}
			else if(nread > 0)
			{
				msg->msg = charMsg;
				rcvFlag = TRUE;
			}
		}

	}while(!rcvFlag);

	return msg;
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
		//memcpy(msgstraux, &msgSize, sizeof(int));
		//msgstraux += sizeof(int);
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
	case LOGIN:
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
	case LIST_LEAGUES:
	case LIST_TEAMS:
	case LIST_TRADES:
	case LOGOUT:
		msgSize = sizeof(int);
		msgstr = malloc(msgSize);
		memcpy(msgstr, &(msg->type), sizeof(int));
		break;
	case LEAGUE_SHOW:
	case TEAM_SHOW:
	case TRADE_SHOW:
		int idSize = strlen(msg->data.show_t.ID);
		msgSize = 2*sizeof(int)+idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &idSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.show_t.ID), idSize);
		break;
	case TRADE:
		int fromSize = strlen(msg->data.trade_t.from);
		int toSize = strlen(msg->data.trade_t.to);
		int idSize = strlen(msg->data.trade_t.teamID);
		msgSize = 4*sizeof(int) + fromSize + toSize + idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.from), fromSize);
		msgstraux += fromSize;
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.to), toSize);
		msgstraux += toSize;
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.teamID), idSize);
		break;
	case TRADE_WITHDRAW:
	case TRADE_ACCEPT:
		int idSize = strlen(msg->data.trade_t.tradeID);
		msgSize = 2*sizeof(int)+idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &idSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.tradeID), idSize);
		break;
	case TRADE_NEGOTIATE:
		int fromSize = strlen(msg->data.trade_t.from);
		int toSize = strlen(msg->data.trade_t.to);
		int idSize = strlen(msg->data.trade_t.tradeID);
		msgSize = 4*sizeof(int) + fromSize + toSize + idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.from), fromSize);
		msgstraux += fromSize;
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.to), toSize);
		msgstraux += toSize;
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.tradeID), idSize);
		break;
	}

	int nwrite;
	if((nwrite = write(fdOut, &msgSize, sizeof(int))) == -1)
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

void connectToServer(void)
{
	int readyFlag = FALSE;
	if((fifoIn = tempnam("/tmp/", "client")) == NULL)
	{
		perror("Input FIFO name couldn't be made");
		exit(1);
	}
	if((fifoOut = malloc(strlen(FIFO_OUT))) == NULL)
	{
		perror("Output FIFO name couldn't be allocated");
		exit(1);
	}
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
