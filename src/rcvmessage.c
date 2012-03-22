/*
 * rcvmessage.c
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MSGSIZ 63

char *fifo = "fifo";

main(int argc, char ** argv)
{
	int fdIn, fdOut;
	void *msgbuf;

	if(mkfifo(fifo, 0666) == -1)
	{
		if(errno != EEXIST)
		{
			perror("receiver: fifoIn");
		}
	}


	if((fdIn = open(fifo, O_RDONLY)) < 0)
	{
		perror("fifo in open failed");

	}

	for(;;)
	{
		int size;
		int count;
		if(read(fdIn, &size, sizeof(int)) == -1)
		{
			perror("failed to read the size");
		}
		else
		{
			msgbuf = malloc(size);
			if((count = read(fdIn, msgbuf, size)) < 0)
			{
				printf("message read failed");
			}
			else if (count > 0)
			{
				//printf("%c\n", msgbuf[0]);
				int opt, msgSize, i;
				float pi;
				char *msg;
				memcpy(&opt, msgbuf, sizeof(int));
				void * aux = msgbuf;
				aux+=sizeof(int);
				printf("Opción: %d\n", opt);
				switch(opt){
				case 0:
					printf("se cierra\n");
					close(fdIn);
					unlink(fifo);
					exit(0);
					break;
				case 1:
					memcpy(&msgSize, aux, sizeof(int));
					msg = malloc(msgSize);
					aux+=sizeof(int);
					memcpy(msg, aux, msgSize);
					printf("Mensaje: %s\n", msg);
					free(msg);
					break;
				case 2:
					memcpy(&i, aux, sizeof(int));
					printf("Mensaje: %d\n", i);
					break;
				case 3:
					memcpy(&pi, aux, sizeof(float));
					printf("Mensaje: %f\n", pi);
					break;
				case 4:
					printf("PING!\n");
					memcpy(&msgSize, aux, sizeof(int));
					msg = malloc(msgSize);
					aux+=sizeof(int);
					memcpy(msg, aux, msgSize);
					printf("PONG en %s\n", msg);
					int nwrite;
					char pong[]="PONG";
					int pongSize = strlen(pong);
					if((fdOut = open(msg, O_WRONLY)) < 0)
					{
						perror("fifo out failed open");
					}
					if((nwrite = write(fdOut, &pongSize, sizeof(int))) == -1)
					{
						perror("pong size write failed");
					}
					if((nwrite = write(fdOut, pong, pongSize)) == -1)
					{
						perror("pong write failed");
					}
					free(msg);
					close(fdOut);
					unlink(msg);
					break;
				}
				free(msgbuf);
			}
		}

		//printf("message received:%s\n", msgbuf);
	}

}
