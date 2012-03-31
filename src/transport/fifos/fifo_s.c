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
		if((nread = read(auxfdIn, &(msgSize), sizeof(int)))  > 0)
		{
			streamAux = stream = malloc(msgSize);
			if((nread = read(auxfdIn, stream, msgSize)) > 0)
			{
				msg = deserializeMsg(stream);
				free(streamAux);
				rcvFlag = TRUE;
			}
		}

	}while(rcvFlag != TRUE);

	return msg;
}

int communicate(Channel ch, Msg_s msg)
{
	return sendmessage(ch, msg);
}

Channel createChannel(Msg_t msg)
{
	Channel ch = malloc(sizeof(channel_t));

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

	if((ch->fdIn = open(ch->fifoIn, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		//TODO: gracious exit for this error;
	}

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

	msgstr = serialize_s(msg);
	memcpy(&(msgSize), msgstr, sizeof(int));

	int nwrite;

	if((nwrite = write(ch->fdOut, msgstr, msgSize+sizeof(int))) == -1)
	{
		perror("Could not write message");
		return !SUCCESSFUL;
	}


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

