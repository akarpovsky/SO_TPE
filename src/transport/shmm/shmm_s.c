/*
* 
* Shared memory server
*
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>

#include "../includes/share_ex.h"

#include "../../includes/message.h"
#include "../../utils/LinkedList.h"
#include "../../includes/defines.h"
#include "../../includes/transport_s.h"

int fdShMem; // Shared memory handle file descriptor
int cantClients = 0;
void * bufferShMem; // Where shared data will be!


/**
* Allocates a shared memory segment.
*
* @param n Size (in bytes) of chunk to allocate.
* @return Id of shared memory chunk.
*/
int AllocateSharedMemory(key_t key, int n)
{
	assert(n > 0); 
	int shmid;
	 if ((shmid = shmget(key, n, 0644 | IPC_CREAT )) == -1) {
        perror("shmget");
        exit(1);
    }

    return shmid;
}

/**
* Maps a shared memory segment onto our address space.
*
* @param id Shared memory block to map.
* @return Address of mapped block.
*/

void * MapSharedMemory(int id)
{
	void * addr = shmat(id, (void *)0, 0);
    if (addr == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    return addr;
}

void down(sem_t * sem){
	sem_wait(sem);
	return ;
}

void up(sem_t * sem){
	sem_post(sem);
	return ;
}


void closeServer(char * server_path){
	// This function removes the created socket file in /tmp/
	// unlink(server_path);
	close(sockfd);

	return ;
}

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG socket_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	closeServer(SERVER_PATH);
	exit(EXIT_FAILURE);
}

void uplink(void){

	Sem server_sem; // Pointer to server semaphore
	sem_unlink(SERVER_SEM); // Just in case server_sem remains open

	server_sem = sem_open(SERVER_SEM, O_CREAT | O_EXCL, 0644, 0);
	if(server_sem == SEM_FAILED)
    {
      perror("Unable to create semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

    fdShMem = AllocateSharedMemory(SHMKEY, SERVER_BUF_SIZE);
	bufferShMem = MapSharedMemory(fdShMem);
	ftruncate(fdShMem, SERVER_BUF_SIZE);
	printf("Created and mapped shared memory in FD %d ; SHMM size = %d ; buffer = %d\n", fdShMem, BUFSIZ, (int) bufferShMem);
	printf("Server finished upLink() - Server ready!\n");

}

Channel createChannel(Msg_t msg)
{
	if(msg->type == CONTACT){
		
		if(++cantClients <= MAX_CLIENTS){

			Channel ch = malloc(sizeof(channel_t));
			char[20] sem_name;
			sprintf(sem_name, "/client_sem_%d", cantClients);

			Sem client_sem; // Pointer to client semaphore
			sem_unlink(sem_name); // Just in case client_sem remains open

			client_sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 0);
			if(client_sem == SEM_FAILED)
	    	{
		      perror("Unable to create semaphore");
		      sem_unlink(sem_name);
		      exit(EXIT_FAILURE);
		    }

	    	Sem server_sem;
			server_sem = sem_open(SERVER, 0, 0644, 0); /* Open a preexisting semaphore. */
			if(server_sem == SEM_FAILED)
		    {
		      perror("Unable to create semaphore");
		      sem_unlink(SERVER_SEM);
		      exit(EXIT_FAILURE);
		    }

		    ch->new_msg_sem = client_sem;
		    ch->memory_lock_sem = server_sem;
			printf("Created \"%s\" and filled sem channel data\n", sem_name);
		    
		    ch->buffer = bufferShMem+((cantClients-1)*BUFSIZ);

		    printf("Now client has a unique offset inside the shared memory space: %d\n", (cantClients-1)*BUFSIZ);

			return ch;

		}else{
			return NULL; // No more slots for new clients
		}
	}

	return NULL;
}

Msg_s establishChannel(Channel ch)
{
	Msg_s serverMsg = createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	// serverMsg->status = ch->client->sin_port;
	
	return serverMsg;
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch, msg);
}

int sendmessage(Channel ch, Msg_s msg){

}

Msg_t IPClisten(Channel ch){


}