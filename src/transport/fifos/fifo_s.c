/*
 * fifo_s.c
 *
 */

#include "../../includes/fifo_s.h"

int fdIn;
char * fifoIn;

void uplink(void)
{
	if((fifoIn = malloc(strlen(FIFO_IN))) == NULL)
	{
		 perror("Input FIFO name couldn't be allocated");
		 exit(1);
	}

	strcpy(fifoIn, FIFO_IN);

	if(mkfifo(fifoIn, 0666) == -1)
	{
		perror("Input FIFO couldn't be created");
		exit(1);
	}

	//TODO
	printf("Uplinked\n");

	if((fdIn = open(fifoIn, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		exit(1);
	}

}

Msg_t listen(Channel ch)
{
	int rcvFlag = FALSE;
	Msg_t msg;
	int msgSize;
	int auxfdIn = ((ch == NULL)? fdIn : ch->fdIn);

	do{
		int nread;
		void * stream;
		void * streamAux;
		if((nread = read(auxfdIn, &(msgSize), sizeof(int))) == -1)
		{
			perror("Reading server message size failed");
			return NULL;
		}
		else if(nread > 0)
		{
			streamAux = stream = malloc(msgSize);
			if((nread = read(auxfdIn, stream, msgSize)) == -1)
			{
				perror("Reading server message failed");
				return NULL;
			}
			else if(nread > 0)
			{
				msg = deserializeMsg(stream);
				free(streamAux);
				rcvFlag = TRUE;
			}
		}

	}while(!rcvFlag);

	return msg;
}

int communicate(Channel ch, Msg_s msg)
{
	return sendmessage(ch, msg);
}

Channel createChannel(Msg_t msg)
{
	Channel ch = malloc(sizeof(channel_t));

	//TODO
	printf("tempnam: %s\n", msg->data.tempnam);
	ch->fifoOut = msg->data.tempnam;

	if((ch->fdOut = open(ch->fifoOut, O_WRONLY | O_NONBLOCK)) == -1)
	{
		perror("Output FIFO couldn't be opened");
		return NULL;
	}

	return ch;

}

int establishChannel(Channel ch)
{
	if((ch->fifoIn = tempnam("/tmp/", "server")) == NULL)
	{
		perror("Input FIFO name couldn't be made");
		//TODO: gracious exit for this error;
	}

	if(mkfifo(ch->fifoIn, 0666) == -1)
	{
		perror("Input FIFO couldn't be created");
		//TODO: gracious exit for this error;
	}

	if((ch->fdIn = open(ch->fifoIn, O_RDONLY)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		//TODO: gracious exit for this error;
	}

	int nwrtie;

	msg_s response;
	response.status = SUCCESSFUL;

	List l = malloc(sizeof(llist));
	CreateList(l);
	AddToList(ch->fifoIn, l);

	response.msgList = l;

	return communicate(ch, &response);

}

int sendmessage(Channel ch, Msg_s msg){

	int msgSize;
	void * msgstr;
	void * msgstraux;
	int NumEl = msg->msgList->NumEl;
	int * sizes = malloc(NumEl * sizeof(int ));
	char ** strings = malloc(NumEl * sizeof(char *));
	int msgListSize = 0;
	int i;

	msgstr = serialize_s(msg);
	memcpy(&(msgSize), msgstr, sizeof(int));

	/*Element e;

	FOR_EACH(e, msg->msgList)
	{
		sizes[i] = strlen(e->data)+1+sizeof(int);
		strings[i] = e->data;

		msgListSize += sizes[i];
		i++;
	}

	msgSize = 2*sizeof(int) + msgListSize;
	msgstr = msgstraux = malloc(msgSize);


	memcpy(msgstraux, &(msg->status), sizeof(int));
	msgstraux += sizeof(int);

	memcpy(msgstraux, &(msg->msgList->NumEl), sizeof(int));
	msgstraux += sizeof(int);

	for(i = 0; i < msg->msgList->NumEl; i++)
	{
		memcpy(msgstraux, &sizes[i], sizeof(int));
		msgstraux += sizeof(int);

		memcpy(msgstraux, strings[i], sizes[i]);
		msgstraux += sizes[i];
	}
*/
	int nwrite;

/*	if((nwrite = write(ch->fdOut, &msgSize, sizeof(int)) == -1))
	{
		perror("Could not write message size");
		return !SUCCESSFUL;
	}*/
	if((nwrite = write(ch->fdOut, msgstr, msgSize+sizeof(int))) == -1)
	{
		perror("Could not write message");
		return !SUCCESSFUL;
	}

	/*FOR_EACH(e, msg->msgList){
		if(msg->msgList->pFirst != e){
			free(e->prev);
		}
		free(e->data);
	}

	free(msg->msgList->pLast);
	free(msg->msgList);
	free(msg);*/

	free(msgstr);
	return SUCCESSFUL;

}

void closeMainServer(void)
{
	close(fdIn);
	unlink(fifoIn);
}


void closeConnection(Channel ch)
{
	close(ch->fdIn);
	close(ch->fdOut);
	unlink(ch->fifoIn);
	unlink(ch->fifoOut);
}
