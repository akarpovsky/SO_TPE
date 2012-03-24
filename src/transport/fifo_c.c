/*
 * fifo.c
 *
 */
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/message.h"
#include "../includes/marshalling.h"
#include "../includes/defines.h"

#include "../includes/fifo_c.h"

char * fifoOut;
char * fifoIn;
int fdIn, fdOut;

Msg_s communicate(Msg_t msg)
{

	if(sendmessage(msg) == SUCCESSFUL){
		return rcvmessage();
	}

	return NULL;
}

Msg_s rcvmessage(void)
{
	int rcvFlag = FALSE;
	Msg_s msg = malloc(sizeof(msg_s));
	do{
		int nread, msgSize;
		void * bytestring;
		void * aux;
		if((nread = read(fdIn, &(msgSize), sizeof(int))) == -1)
		{
			perror("Reading server message size failed");
			return NULL;
		}
		else if(nread > 0)
		{
			aux = bytestring = malloc(msgSize);
			if((nread = read(fdIn, aux, msgSize)) == -1)
			{
				perror("Reading server message failed");
				return NULL;
			}
			else if(nread > 0)
			{
				memcpy(&(msg->status), aux, sizeof(int));
				aux += sizeof(int);

				List l = malloc(sizeof(llist));
				CreateList(l);

				msg->msgList = l;

				int cantElem;
				memcpy(&(cantElem), aux, sizeof(int));
				aux += sizeof(int);

				int i;
				int strsize;
				char * str;

				for(i = 0; i < cantElem; i++)
				{
					memcpy(&(strsize), aux, sizeof(int));
					aux += sizeof(int);

					str = malloc(strsize);
					memcpy(str, aux, strsize);
					aux += strsize;

					AddToList(str, msg->msgList);
				}
				rcvFlag = TRUE;
			}
		}

	}while(!rcvFlag);

	return msg;
}

int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;
	void * msgstraux;
	int tempnamSize, passSize, userSize, fromSize, toSize;

	switch(msg->type)
	{
	case CONTACT:
		tempnamSize = strlen(msg->data.tempnam)+1;
		msgSize = 2*sizeof(int) + tempnamSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &tempnamSize, sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.tempnam), tempnamSize);
		break;
	case REGISTER:
		passSize = strlen(msg->data.register_t.pass)+1;
		userSize = strlen(msg->data.register_t.user)+1;
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
		passSize = strlen(msg->data.login_t.pass)+1;
		userSize = strlen(msg->data.login_t.user)+1;
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
		msgSize = 2*sizeof(int);
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.show_t.ID), sizeof(int));
		break;
	case TRADE:
		fromSize = strlen(msg->data.trade_t.from)+1;
		toSize = strlen(msg->data.trade_t.to)+1;
		msgSize = 4*sizeof(int) + fromSize + toSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.from), fromSize);
		msgstraux += fromSize;
		memcpy(msgstraux, &(toSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.to), toSize);
		msgstraux += toSize;
		memcpy(msgstraux, &(msg->data.trade_t.teamID), sizeof(int));
		break;
	case TRADE_WITHDRAW:
	case TRADE_ACCEPT:
		msgSize = 2*sizeof(int);
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));
		break;
	case TRADE_NEGOTIATE:
		fromSize = strlen(msg->data.trade_t.from)+1;
		toSize = strlen(msg->data.trade_t.to)+1;
		msgSize = 4*sizeof(int) + fromSize + toSize;
		msgstraux = msgstr = malloc(msgSize);
		memcpy(msgstraux, &(msg->type), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(fromSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.from), fromSize);
		msgstraux += fromSize;
		memcpy(msgstraux, &(toSize), sizeof(int));
		msgstraux += sizeof(int);
		memcpy(msgstraux, &(msg->data.trade_t.to), toSize);
		msgstraux += toSize;
		memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));
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

	communicate(&com);
}
