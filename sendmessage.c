/*
 * sendmessage.c
 *
 *  Created on: 13/03/2012
 *      Author: neon
 */

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>


typedef struct message {
	int type;
	union data{
		char * type1;
		int type2;
		float type3;
	};
} message;

char *fifo = "fifo";

main (int argc, char **argv)
{
	int fd, j, nwrite;
	char msgbuf[];


	if(argc < 2)
	{
		fprintf(stderr, "Usage: sendmessage msg ... \n");
		exit(1);
	}


	if((fd = open(fifo, O_WRONLY | O_NONBLOCK)) < 0)
	{
		printf("fifo open failed");
	}



	for( j = 1; j < argc; j++)
	{
		int opt = atoi(argv[j]);

		if(opt < 0 || opt > 3)
		{
			printf("Option not available");
			exit(1);
		}

		int size;

		switch(opt)
		{
		case 0:
			size =2*sizeof(char);
			msgbuf = malloc(size);
			msgbuf[0] = "0";
			msgbuf[1] = ";";
			break;
		case 1:
			size=7*sizeof(char);
			msgbuf = malloc(size);
			msgbuf[0] = "1;";
			msgbuf[2] = "hola;";
			break;
		case 2:
			size = 3*sizeof(char) + sizeof(int);
			msgbuf = malloc(size);
			msgbuf[0] = "2;";
			int i = 1024;
			memcpy(((void*)msgbuf+2), &i, sizeof(int));
			msgbuf[2+sizeof(int)] = ";";
			break;
		case 3:
			size =3*sizeof(char) + sizeof(float);
			msgbuf = malloc(size);
			msgbuf[0] = "3;";
			float pi = 3.14;
			memcpy(((void*)msgbuf+2), &pi, sizeof(float));
			msgbuf[2+sizeof(float)] = ";";
			break;
		}

		if((nwrite = write(fd, msgbuf, size)) == -1)
		{
			printf("message write failed");
		}
	}

	exit(0);
}


