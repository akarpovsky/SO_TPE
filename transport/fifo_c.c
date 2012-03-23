/*
 * fifo.c
 *
 *  Created on: 16/03/2012
 *      Author: Facundo
 */
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/fifo_c.h"
#include "../includes/marshalling.h"
#include "../includes/defines.h"
#include "../includes/message.h"

char * fifoOut;
char * fifoIn;
int fdIn, fdOut;

msg_s * communicate(msg_t * msg)
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
		if((nread = read(fdIn, &(msg->type), sizeof(size_t))) == -1)
		{
			perror("Reading server message size failed");
			return NULL;
		}
		else if(nread > 0)
		{
			charMsg = malloc(msg->type);
			if((nread = read(fdIn, charMsg, msg->type)) == -1)
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
	case CONTACT:
		int tempnamSize = strlen(msg->data.tempnam)+1;
		msgSize = 2*sizeof(int) + tempnamSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &tempnamSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.tempnam, tempnamSize));
		break;
	case REGISTER:
		int passSize = strlen(msg->data.register_t.pass)+1;
		int userSize = strlen(msg->data.register_t.user)+1;
		msgSize = 3*sizeof(int)+passSize+userSize;
		msgstraux = msgstr = malloc(msgSize);
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
		int passSize = strlen(msg->data.login_t.pass)+1;
		int userSize = strlen(msg->data.login_t.user)+1;
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
		int idSize = strlen(msg->data.show_t.ID)+1;
		msgSize = 2*sizeof(int)+idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &idSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.show_t.ID), idSize);
		break;
	case TRADE:
		int fromSize = strlen(msg->data.trade_t.from)+1;
		int toSize = strlen(msg->data.trade_t.to)+1;
		int idSize = strlen(msg->data.trade_t.teamID)+1;
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
		int idSize = strlen(msg->data.trade_t.tradeID)+1;
		msgSize = 2*sizeof(int)+idSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &idSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.tradeID), idSize);
		break;
	case TRADE_NEGOTIATE:
		int fromSize = strlen(msg->data.trade_t.from)+1;
		int toSize = strlen(msg->data.trade_t.to)+1;
		int idSize = strlen(msg->data.trade_t.tradeID)+1;
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

	msg_t com;
	com.type = CONTACT;
	com.data.tempnam = fifoIn;

	communicate(com);
}
