/*
 * fifo_s.c
 *
 */
#include "../includes/fifo_s.h"

int fdIn;
char * fifoIn;

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

Msg_t listen()
{
	int rcvFlag = FALSE;
	Msg_t msg = malloc(sizeof(msg_t));
	int msgSize;
	do{
		int nread;
		void * stream;
		void * streamAux;
		if((nread = read(fdIn, &(msgSize), sizeof(int))) == -1)
		{
			perror("Reading server message size failed");
			return NULL;
		}
		else if(nread > 0)
		{
			streamAux = stream = malloc(msgSize);
			if((nread = read(fdIn, stream, msgSize)) == -1)
			{
				perror("Reading server message failed");
				return NULL;
			}
			else if(nread > 0)
			{
				int type;
				memcpy(&type, stream, sizeof(int));
				stream += sizeof(int);
				msg->type = type;

				switch(type)
				{
				case CONTACT:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream += sizeof(int);
					msg->data.tempnam = malloc(msgSize);
					memcpy(&(msg->data.tempnam), stream, size);
					break;
				case REGISTER:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.register_t.user = malloc(size);
					memcpy(&(msg->data.register_t.user), stream, size);
					stream+=size;
					memcpy(&size, stream, sizeof(int));
					msg->data.register_t.pass = malloc(size);
					stream+=sizeof(int);
					memcpy(&(msg->data.register_t.pass), stream, size);
					break;
				case LOGIN:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.login_t.user = malloc(size);
					memcpy(&(msg->data.login_t.user), stream, size);
					stream+=size;
					memcpy(&size, stream, sizeof(int));
					msg->data.login_t.pass = malloc(size);
					stream+=sizeof(int);
					memcpy(&(msg->data.login_t.pass), stream, size);
					break;
				case LIST_LEAGUES:
				case LIST_TEAMS:
				case LIST_TRADES:
					break;
				case LEAGUE_SHOW:
				case TEAM_SHOW:
				case TRADE_SHOW:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					memcpy(&(msg->data.show_t.ID), stream, sizeof(int));
					break;
				case TRADE:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.trade_t.from = malloc(size);
					memcpy(&(msg->data.trade_t.from), stream, size);
					stream+=size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.trade_t.to = malloc(size);
					memcpy(&(msg->data.trade_t.to), stream, size);
					stream+=size;
					memcpy(&(msg->data.trade_t.teamID), stream, sizeof(int));
					break;
				case TRADE_WITHDRAW:
				case TRADE_ACCEPT:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream += sizeof(int);
					memcpy(&(msg->data.trade_t.tradeID), stream, sizeof(int));
					break;
				case TRADE_NEGOTIATE:
					int size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.trade_t.from = malloc(size);
					memcpy(&(msg->data.trade_t.from), stream, size);
					stream+=size;
					memcpy(&size, stream, sizeof(int));
					stream+=sizeof(int);
					msg->data.trade_t.to = malloc(size);
					memcpy(&(msg->data.trade_t.to), stream, size);
					stream+=size;
					memcpy(&(msg->data.trade_t.tradeID), stream, sizeof(int));
					break;
				}
				free(streamAux);
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
	Channel ch = malloc(channel_t);

	ch->fifoOut = msg->data.tempnam;

	if((ch->fdOut = open(ch->fifoOut, O_WRONLY)) == -1)
	{
		perror("Output FIFO for file %s couldn't be opened", ch->fifoOut);
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
	response.type = SUCCESSFUL;
	response.msg = ch.fifoIn;

	return communicate(ch, response);
}

int sendmessage(Channel ch, Msg_s msg){

	int msgSize;
	void * msgstr;
	void * msgstraux;
	int sizes[] = malloc(msg->msgList->NumEl);
	char * strings[] = malloc(msg->msgList->NumEl);
	int msgListSize = 0;
	int i;

	Element e;

	FOR_EACH(e, msg->msgList)
	{
		sizes[i] = strlen(e->data)+1;
		strings[i] = e->data;
		e = e->next;

		msgListSize += sizes[i];
	}

	msgSize = 2*sizeof(int) + msgListSize;
	msgstr = msgstraux = malloc(msgSize);


	memcpy(msgstraux,msg->status, sizeof(int));
	msgstraux += sizeof(int);

	memcpy(msgstraux, msg->msgList->NumEl, sizeof(int));
	msgstraux += sizeof(int);

	for(i = 0; i < msg->msgList->NumEl; i++)
	{
		memcpy(msgstraux, &sizes[i], sizeof(int));
		msgstraux += sizeof(int);

		memcpy(msgstraux, strings[i], sizes[i]);
		msgstraux += sizes[i];
	}

	int nwrite;

	if((nwrite = write(ch.fdOut, &msgSize, sizeof(int)) == -1))
	{
		perror("Could not write message size");
		return !SUCCESSFUL;
	}
	if((nwrite = write(ch.fdOut, msgstr, msgSize)) == -1)
	{
		perror("Could not write message");
		return !SUCCESSFUL;
	}


	free(msgstr);
	return SUCCESSFUL;

}

