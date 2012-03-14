/*
 * rcvmessage.c
 *
 *  Created on: 13/03/2012
 *      Author: neon
 */

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

//#define MSGSIZ 63

char *fifo = "fifo";

main(int argc, char ** argv)
{
	int fd;
	char msgbuf[];

	if(mkfifo(fifo, 0666) == -1)
	{
		if(errno != EEXIST)
		{
			printf("receiver: mkfifo");
		}
	}

	if((fd = open(fifo, O_RDWR)) < 0)
	{
		printf("fifo open failed");

	}

	for(;;)
	{
		int count;
		msgbuf = malloc(sizeof(int));

		if((count = read(fd, msgbuf, sizeof(int))) < 0)
		{
			printf("message read failed");
		}
		else if (count > 0)
		{
			int opt = atoi(msgbuf);
			switch(opt){
			case 0:
				exit(0);
				break;
			case 1:
				char msg[5];
				read();
				break;
			}
		}


		printf("message received:%s\n", msgbuf);
	}

}
