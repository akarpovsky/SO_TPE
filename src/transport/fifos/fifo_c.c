/*
 * fifo.c
 *
 */

#include "../../includes/fifo_c.h"

char * fifoOut;
char * fifoIn;
int fdIn, fdOut;

Msg_s communicate(Msg_t msg)
{

	//TODO:
	printf("Sending message type %d\n", msg->type);
	if(sendmessage(msg) == SUCCESSFUL){
		//todo
		printf("Waiting for message\n");
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
				msg = deserialize_s(aux);
				/*memcpy(&(msg->status), aux, sizeof(int));
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
				rcvFlag = TRUE;*/
			}
		}

	}while(!rcvFlag);

	return msg;
}

int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;
	int tempnamSize, passSize, userSize, fromSize, toSize;

	msgstr = serializeMsg(msg);
	memcpy(&msgSize, msg, sizeof(int));

	int nwrite;
	/*if((nwrite = write(fdOut, &msgSize, sizeof(int))) == -1)
	{
		perror("Could not write message size");
		return !SUCCESSFUL;
	}*/
	if((nwrite = write(fdOut, msgstr, msgSize+sizeof(int))) == -1)
	{
		perror("Could not write message");
		return !SUCCESSFUL;
	}

	free(msgstr);

	return SUCCESSFUL;
}

void connectToServer(void)
{
	//TODO:
	printf("initilizing to server\n");
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
	strcpy(fifoOut, FIFO_OUT);

	if(mkfifo(fifoIn, 0666) ==  -1)
	{
		perror("Input FIFO couldn't be created");
		exit(1);
	}

	if((fdOut = open(fifoOut, O_WRONLY | O_NONBLOCK)) == -1)
	{
		perror("Output FIFO couldn't be opened");
		unlink(fifoOut);
		exit(1);
	}

	//TODO
	printf("opened fifoOut: %s\n", fifoOut);

	if((fdIn = open(fifoIn, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		close(fdOut);
		unlink(fifoOut);
		exit(1);
	}

	printf("openend fifoIn: %s\n", fifoIn);

	msg_t com;
	com.type = CONTACT;
	com.data.tempnam = fifoIn;

	//TODO
	printf("sending CONTACT to server\n");

	Msg_s response = communicate(&com);

	free(fifoOut);

	fifoOut = response->msgList->pFirst->data;

	free(response->msgList->pFirst);
	free(response->msgList);
	free(response);

	close(fdOut);
	unlink(fifoOut);

	if((fdOut = open(fifoOut, O_WRONLY | O_NONBLOCK)) == -1)
	{
		perror("Input FIFO from server couldn't be opened");
		closeConnection();
		exit(1);
	}

}

void closeConnection(void)
{
	if(fdIn != -1){
		close(fdIn);
	}

	if(fdOut != -1){
		close(fdOut);
	}

	unlink(fifoIn);
	unlink(fifoOut);
}

void * serialize_contact (Msg_t msg){
	int tempnamSize;
	int msgSize;
	void * msgstraux;
	void * msgstr;

	tempnamSize = strlen(msg->data.tempnam)+1;
	msgSize = sizeof(int) + tempnamSize;
	if((msgstraux = msgstr = calloc(msgSize+sizeof(int), sizeof(char))) == NULL){
		perror("code_contact: Not enough memory");
		exit(1);
	}
	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy(msgstraux+1, msg->data.tempnam);

	return msgstr;
}
