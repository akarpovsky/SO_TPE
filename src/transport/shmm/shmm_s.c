/*
* 
* Shared memory server
*
*/


#include "../../includes/share_ex.h"
#include "../../includes/shmm_s.h"

int fdShMem; // Shared memory handle file descriptor
int cantClients = 0; 
int newconnection = 0;
void * bufferShMem; // Where shared data will be!
Sem server_lock_sem; // Pointer to server semaphore
Sem server_contact_sem; // Pointer to server semaphore
Sem response_contact_sem;

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


void closeMainServer(){
	// This function provides a clean exit for the IPC

	close(fdShMem); // Shared memory handle file descriptor
	sem_unlink(SERVER_SEM); // Pointer to server semaphore
	sem_unlink(SERVER_CONTACT_SEM); // Pointer to server semaphore
	sem_unlink(RESPONSE_CONTACT_SEM); // Pointer to server semaphore
	return ;
}

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG shmm_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	closeMainServer();

	exit(EXIT_FAILURE);
}

void uplink(void){

	sem_unlink(SERVER_SEM); // Just in case server_sem remains open
	sem_unlink(SERVER_CONTACT_SEM); // Just in case server_msg_sem remains open
	sem_unlink(RESPONSE_CONTACT_SEM); // Just in case response_sem remains open


	server_lock_sem = sem_open(SERVER_SEM, O_CREAT | O_EXCL, 0644, 1);
	if(server_lock_sem == SEM_FAILED)
    {
      perror("Unable to create semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

    server_contact_sem = sem_open(SERVER_CONTACT_SEM, O_CREAT | O_EXCL, 0644, 0);
	if(server_contact_sem == SEM_FAILED)
    {
      perror("Unable to create semaphore");
      sem_unlink(SERVER_CONTACT_SEM);
      exit(EXIT_FAILURE);
    }


	response_contact_sem = sem_open(RESPONSE_CONTACT_SEM, O_CREAT | O_EXCL, 0644, 0);
	if(response_contact_sem == SEM_FAILED)
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
		
		Channel ch = malloc(sizeof(channel_t));
		if(++cantClients <= MAX_CLIENTS){

			char sem_name[20];
			sprintf(sem_name, "/clitoserv_sem_%d", cantClients);
			
			char sem_name2[20];
			sprintf(sem_name2, "/servtocli_sem_%d", cantClients);

			Sem client_sem; // Pointer to client semaphore
			sem_unlink(sem_name); // Just in case client_sem remains open

			Sem client_sem2; // Pointer to client semaphore
			sem_unlink(sem_name2); // Just in case client_sem remains open

			client_sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 0);
			if(client_sem == SEM_FAILED)
	    	{
		      perror("Unable to create semaphore");
		      sem_unlink(sem_name);
		      exit(EXIT_FAILURE);
		    }

		    client_sem2 = sem_open(sem_name2, O_CREAT | O_EXCL, 0644, 0);
			if(client_sem2 == SEM_FAILED)
	    	{
		      perror("Unable to create semaphore");
		      sem_unlink(sem_name2);
		      exit(EXIT_FAILURE);
		    }

		    ch->clitoserv_sem = client_sem;
		    ch->servtocli_sem = client_sem2;
		    ch->memory_lock_sem = server_lock_sem;

			printf("Created \"%s\" and filled sem channel data\n", sem_name);
		    
		    ch->buffer = bufferShMem+((cantClients)*BUFSIZ);

		    printf("Now client has a unique offset inside the shared memory space: %d\n", (cantClients-1)*BUFSIZ);

		}
			return ch;

	}

	return NULL;
}

Msg_s establishChannel(Channel ch)
{

	Msg_s serverMsg = (Msg_s) createMsg_s(CONTACT);
	if(cantClients > MAX_CLIENTS){
		serverMsg->status = -1;
	}else{
		char * cliNum = calloc(25, sizeof(char));
		itoa(cantClients, cliNum);
		AddToList(cliNum, serverMsg->msgList);
	}
	newconnection = 1;
	return serverMsg;
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch, msg);
}

int sendmessage(Channel ch, Msg_s msg){

	printf("Starting msg send ... \n");

	int msgSize;
	void * msgstr;

	msgstr = serialize_s(msg);
	memcpy(&(msgSize), msgstr, sizeof(int));

	Sem auxBlockSem, auxResponseSem;
	void * msg_buffer;

	if(newconnection){
		newconnection = 0;
		auxBlockSem = server_lock_sem;
		auxResponseSem = response_contact_sem;
		msg_buffer = bufferShMem;
	}else{
		auxBlockSem = ch->memory_lock_sem;
		auxResponseSem = ch->servtocli_sem;
		msg_buffer = ch->buffer;
		printf("SERVTOCLI !!! \n");
	}

	down(auxBlockSem); // ---> Start blocking the buffer

		printf("<LOG shmm_s.c> Server: Envio la lista de mensajes al cliente <end>\n");
		printf("<LOG shmm_s.c> Server: Tamaño a enviar = %d en dir %d<end>\n", msgSize+sizeof(int), (int) msg_buffer);
		memcpy(msg_buffer, msgstr, msgSize+sizeof(int));

	up(auxBlockSem); // <--- End blocking the buffer

	up(auxResponseSem); // Tell the client that there is a new msg
	
	printf("Signaling the client... \n");


	// free(msgstr);
	return SUCCESSFUL;

}

Msg_t IPClisten(Channel ch){

	printf("Waiting in IPC listen with ");
	Sem msg_signal_sem;
	void * offset;
	if(ch == NULL){
		printf("null channel ");
		msg_signal_sem = server_contact_sem;
		offset = bufferShMem;
	}else{
		printf("NOT null channel ");
		msg_signal_sem = ch->clitoserv_sem;
		offset = ch->buffer;
	}

	printf("Shared memory: Using blocking shared memory!\n");


	printf("in buffer %d\n", (int) offset);

	int rcvFlag = FALSE;
	Msg_t msg = (Msg_t) calloc(1, sizeof(msg_t));
	int msgSize = 0;
	do{
		down(msg_signal_sem);
		void * stream;	
		void * streamAux;

		down(server_lock_sem);
		
			memcpy(&msgSize, offset, sizeof(int));
			printf("Message size %d\n", msgSize);
			stream = streamAux = calloc(1, msgSize * sizeof(char));
			memcpy(stream, offset+sizeof(int), msgSize);

			/* Reset memory zone so nonblocking shmm work properly */
			int resetZone = 0;
			memcpy(offset, &resetZone, sizeof(int));
		
		up(server_lock_sem);

		int ti = 0;
		memcpy(&ti, stream, sizeof(int));
		printf("Msg type %d\n", ti);
		
		msg = deserializeMsg(stream);
		// free(streamAux);
		rcvFlag = TRUE;

	}while(rcvFlag != TRUE);

	return msg;
}

Msg_t rcvmessage(Channel ch){

	printf("Waiting in IPC listen with ");
	void * offset;
	if(ch == NULL){
		printf("null channel ");
		offset = bufferShMem;
	}else{
		printf("NOT null channel ");
		offset = ch->buffer;
	}

	printf("Shared memory: Using NON blocking shared memory!\n");


	Msg_t msg = (Msg_t) calloc(1, sizeof(msg_t));
	int msgSize = 0;

	void * stream;	
	void * streamAux;

	down(server_lock_sem);
	
		memcpy(&msgSize, offset, sizeof(int));

		if(msgSize == 0){
			return NULL;
		}

		printf("Message size %d\n", msgSize);
		stream = streamAux = calloc(1, msgSize * sizeof(char));
		memcpy(stream, offset+sizeof(int), msgSize);

		/* Reset memory zone so nonblocking shmm work properly */
		int resetZone = 0;
		memcpy(offset, &resetZone, sizeof(int));
	
	up(server_lock_sem);

	int ti = 0;
	memcpy(&ti, stream, sizeof(int));
	printf("Msg type %d\n", ti);
	
	msg = deserializeMsg(stream);
	// free(streamAux);

	return msg;

}


