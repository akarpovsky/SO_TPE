/*
* 
* Shared memory server
*
*/


#include "../../includes/share_ex.h"
#include "../../includes/shmm_s.h"



int fdShMem; // Shared memory handle file descriptor
int cantClients = 0; 
void * bufferShMem; // Where shared data will be!
Sem server_lock_sem; // Pointer to server semaphore
Sem server_msg_sem; // Pointer to server semaphore


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
	// close(sockfd);

	return ;
}

void sigint(){  
	// signal(SIGINT,sigint); /* reset signal */
	// printf("<LOG shmm_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	// closeServer(SERVER_PATH);
	// exit(EXIT_FAILURE);
}

void uplink(void){

	sem_unlink(SERVER_SEM); // Just in case server_sem remains open
	sem_unlink(SERVER_MSG_SEM); // Just in case server_msg_sem remains open

	server_lock_sem = sem_open(SERVER_SEM, O_CREAT | O_EXCL, 0644, 0);
	if(server_lock_sem == SEM_FAILED)
    {
      perror("Unable to create semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

    server_msg_sem = sem_open(SERVER_MSG_SEM, O_CREAT | O_EXCL, 0644, 0);
	if(server_msg_sem == SEM_FAILED)
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
			char sem_name[20];
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

		    ch->new_msg_sem = client_sem;
		    ch->memory_lock_sem = server_lock_sem;
			printf("Created \"%s\" and filled sem channel data\n", sem_name);
		    
		    ch->buffer = bufferShMem+((cantClients)*BUFSIZ);

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
	Msg_s serverMsg = (Msg_s) createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	// serverMsg->status = ch->client->sin_port;
	
	return serverMsg;
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch, msg);
}

int sendmessage(Channel ch, Msg_s msg){

	printf("Starting msg send\n");
	int msgSize;
	void * msgstr;
	void * msgstraux;
	int NumEl = msg->msgList->NumEl;
	int * sizes = malloc(NumEl * sizeof(int));
	char ** strings = malloc(NumEl * sizeof(char *));
	int msgListSize = 0;
	int i = 0;


	Element e;

	// printf("Empiezo a armar las listas\n");
	FOR_EACH(e, msg->msgList)
	{
		// printf("\tProcesando _%s_\n", (char *) e->data);
		sizes[i] = strlen(e->data)+1;
		// printf("Guardo en sizes[i] = %d\n", sizes[i]);

		strings[i] = e->data;
		msgListSize += sizes[i];
		i+=1;
	}

	// printf("Terminé de armar las listas de mensajes\n");
	// printf("msgListSize = %d\n", msgListSize);
	msgSize = 2*sizeof(int) + (i*sizeof(int)) + msgListSize ;
	msgstr = msgstraux = calloc(msgSize, sizeof(char));

	printf("Tamaño del mensaje a enviar = %d\n", msgSize);

	memcpy(msgstraux, &(msg->status), sizeof(int));
	msgstraux += sizeof(int);


	memcpy(msgstraux, &(msg->msgList->NumEl), sizeof(int));
	msgstraux += sizeof(int);

	for(i = 0; i < msg->msgList->NumEl; i++)
	{
		memcpy(msgstraux, &(sizes[i]), sizeof(int));
		msgstraux += sizeof(int);

		memcpy(msgstraux, strings[i], sizes[i]);
		msgstraux += sizes[i];

	}

	down(ch->memory_lock_sem);

		printf("<LOG shmm_s.c> Server: Envio el message size = %d <end>\n", msgSize);
		memcpy(ch->buffer, &msgSize, sizeof(int));
		printf("<LOG shmm_s.c> Server: Envio la lista de mensajes al cliente <end>\n");
		memcpy((ch->buffer)+sizeof(int), msgstr, msgSize);

	up(ch->memory_lock_sem);

	up(ch->new_msg_sem); // Tell the client that there is a new msg
	
	free(msgstr);
	return SUCCESSFUL;

}

Msg_t IPClisten(Channel ch){

	Sem msg_signal_sem;
	void * offset;
	if(ch == NULL){
		msg_signal_sem = server_msg_sem;
		offset = bufferShMem;
	}else{
		msg_signal_sem = ch->new_msg_sem;
		offset = ch->buffer;
	}

	int rcvFlag = FALSE;
	Msg_t msg;
	int msgSize;

	do{
		down(msg_signal_sem);
		printf("Me despertaron en IPCListen\n");
		
		void * stream;
		void * streamAux;

		down(server_lock_sem);
		
			memcpy(&msgSize, offset, sizeof(int));

			stream = streamAux = malloc(sizeof(msgSize));
			memcpy(stream, offset+sizeof(int), msgSize);
		
		up(server_lock_sem);
		
		msg = deserializeMsg(stream);
		// free(streamAux);
		rcvFlag = TRUE;

	}while(rcvFlag != TRUE);

	return msg;
}


