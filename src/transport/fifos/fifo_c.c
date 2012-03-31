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
		if((nread = read(fdIn, &(msgSize), sizeof(int))) > 0)
		{
			aux = bytestring = malloc(msgSize);
			if((nread = read(fdIn, aux, msgSize)) > 0)
			{
				msg = deserialize_s(aux);
				rcvFlag = TRUE;
			}
		}

	}while(rcvFlag != TRUE);

	return msg;
}

int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;

	msgstr = serializeMsg(msg);
	memcpy(&msgSize, msgstr, sizeof(int));

	int nwrite;
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

	if((fdIn = open(fifoIn, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("Input FIFO couldn't be opened");
		close(fdOut);
		unlink(fifoOut);
		exit(1);
	}

	msg_t com;
	com.type = CONTACT;
	com.data.tempnam = fifoIn;

	Msg_s response = communicate(&com);

	close(fdOut);
	unlink(fifoOut);
	free(fifoOut);

	fifoOut = response->msgList->pFirst->data;

	free(response->msgList->pFirst);
	free(response->msgList);
	free(response);

	if((fdOut = open(fifoOut, O_WRONLY /*| O_NONBLOCK**/)) == -1)
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

