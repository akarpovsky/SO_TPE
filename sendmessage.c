/*
 * sendmessage.c
 *
 *  Created on: 13/03/2012
 *      Author: neon
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

typedef struct message {
	int type;
	union data{
		char * type1;
		int type2;
		float type3;
	};
} message;

char *fifoOut = "fifo";

main (int argc, char **argv)
{
	char * fifoIn = tempnam(".", NULL);
	int fdIn, fdOut, j, nwrite;
	void * msgbuf;
	int pingFlag = FALSE;

	if(argc < 2)
	{
		fprintf(stderr, "Usage: sendmessage msg ... \n");
		exit(1);
	}

	if(mkfifo(fifoIn, 0666) == -1)
	{
		perror("fifo in creation failed");
	}
	if((fdOut = open(fifoOut, O_WRONLY )) < 0)
	{
		perror("fifo out open failed");
	}

	if((fdIn = open(fifoIn, O_RDONLY | O_NONBLOCK)) < 0)
	{
		perror("fifo in open failed");
	}

	for( j = 1; j < argc; j++)
	{
		int opt = atoi(argv[j]);

		if(opt < 0 || opt > 4)
		{
			printf("Option not available");
			exit(1);
		}

		int size;
		char msg[] = "hola";
		int msgSize = strlen(msg)+1;

		switch(opt)
		{
		case 0:
			size =sizeof(int);
			msgbuf = malloc(size);
			memcpy(msgbuf, &opt, size);
			break;
		case 1:
			size=2*sizeof(int)+msgSize;
			msgbuf = malloc(size);
			memcpy(msgbuf, &opt, sizeof(int));
			memcpy(msgbuf+sizeof(int), &msgSize, sizeof(int));
			memcpy(msgbuf+2*sizeof(int), msg, msgSize);
			break;
		case 2:
			size = 2*sizeof(int);
			msgbuf = malloc(size);
			memcpy(msgbuf, &opt, sizeof(int));
			int i = 1024;
			memcpy(msgbuf+sizeof(int), &i, sizeof(int));
			break;
		case 3:
			size =sizeof(int) + sizeof(float);
			msgbuf = malloc(size);
			memcpy(msgbuf, &opt, sizeof(int));
			float pi = 3.141592;
			memcpy(msgbuf+sizeof(int), &pi, sizeof(float));
			break;
		case 4:
			size = 2*sizeof(int) + strlen(fifoIn);
			msgbuf = malloc(size);
			memcpy(msgbuf, &opt, sizeof(int));
			int nameSize= strlen(fifoIn);
			memcpy(msgbuf+sizeof(int), &nameSize, sizeof(int));
			memcpy(msgbuf+2*sizeof(int), fifoIn, nameSize);
			pingFlag = TRUE;
			break;
		}

		if((nwrite = write(fdOut, &size, sizeof(int))) == -1)
		{
			perror("size write failed");
		}
		if((nwrite = write(fdOut, msgbuf, size)) == -1)
		{
			perror("message write failed");
		}

		while(pingFlag)
		{
			char * pong;
			int pongSize;
			int nread;
			if((nread = read(fdIn, &pongSize, sizeof(int))) == -1)
			{
				perror("reading pongSize");
			}
			else
			{
				pong = malloc(pongSize);
				if((nread = read(fdIn, pong, pongSize)) < 0)
				{
					perror("reading PONG");
				}
				else if (nread > 0)
				{
					printf("%s\n", pong);
					pingFlag = FALSE;
				}
				free(pong);

			}
		}
	}

	close(fdIn);
	unlink(fifoIn);
	exit(0);
}


