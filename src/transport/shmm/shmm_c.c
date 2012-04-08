/*
* 
* Shared memory client
*
*/

#include "../../includes/share_ex.h"
#include "../../includes/shmm_c.h"


int fdShMem; // Shared memory handle file descriptor
void * bufferShMem; // Where shared data will be stored!
void * offset; 
Sem memory_lock_sem; // Locks all the shared memory space
Sem server_contact_sem; // Tells the server we want to connect
Sem response_contact_sem; // Just for waiting the server response

Sem clitoserv_sem; // Assigned once the connection is established tells the client that he has new msgs
Sem servtocli_sem;

char servtocli_sem_name[20]; // Stores the sem name so we can make a clean exit if signaled
char clitoserv_sem_name[20];

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

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG shmm_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	sem_unlink(servtocli_sem_name);
	sem_unlink(clitoserv_sem_name);
	// closeServer(SERVER_PATH);
	exit(EXIT_FAILURE);
}


Msg_s communicate(Msg_t msg)
{
	if(sendmessage(msg) == SUCCESSFUL){
		return _rcvmessage();
	}

	return NULL;
}

Msg_s _rcvmessage(void)
{
	
	Sem new_msg;


	if(servtocli_sem == NULL){
		new_msg = response_contact_sem;
	}else{
		new_msg = servtocli_sem;
	}

	printf("_rcvmessaege: Using blocking shared memory!\n");
	printf("Waiting for new messages ... \n");

	down(new_msg);

	int rcvFlag = FALSE;

	Msg_s msg = malloc(sizeof(msg_s));

	do{
		int msgSize;
		void * bytestring;
		void * aux;

		down(memory_lock_sem);
			memcpy(&msgSize, offset, sizeof(int));
			aux = bytestring = malloc(msgSize);
			memcpy(aux, offset+sizeof(int), msgSize);
			msg = deserialize_s(aux);
		up(memory_lock_sem);

		if(msg->status == -1){ // Server cant handle my connection
			printf("\nERROR: No more slots available in server for new clients. Closing connection.\n");
			exit(EXIT_FAILURE);
		}

		rcvFlag = TRUE;

	}while(rcvFlag != TRUE);

	return msg;
}

Msg_s rcvmessage(void)
{
	
	printf("_rcvmessaege: Using NON blocking shared memory!\n");

	Msg_s msg = malloc(sizeof(msg_s));

	int msgSize;
	void * bytestring;
	void * aux;

	down(memory_lock_sem);
		memcpy(&msgSize, offset, sizeof(int));

		if(msgSize == 0){
			return NULL;
		}

		aux = bytestring = malloc(msgSize);
		memcpy(aux, offset+sizeof(int), msgSize);
		msg = deserialize_s(aux);
	up(memory_lock_sem);

	if(msg->status == -1){ // Server cant handle my connection
		printf("\nERROR: No more slots available in server for new clients. Closing connection.\n");
		exit(EXIT_FAILURE);
	}



	return msg;
}

int sendmessage(Msg_t msg)
{
	printf("Sending message to the server ...\n" );
	int msgSize;
	void * msgstr;

	msgstr = serializeMsg(msg);
	memcpy(&msgSize, msgstr, sizeof(int));

	down(memory_lock_sem);
		memcpy(offset, msgstr, msgSize+sizeof(int));
	up(memory_lock_sem);

	if(clitoserv_sem == NULL){
		up(server_contact_sem);
	}else{
   		up(clitoserv_sem);
	}

	// free(msgstr);

	return SUCCESSFUL;
}

void connectToServer(void)
{

	server_contact_sem = sem_open(SERVER_CONTACT_SEM, 0, 0644, 0);
	if(server_contact_sem == SEM_FAILED)
    {
      perror("Unable to open CONTACT semaphore. Maybe the server is down");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

    memory_lock_sem = sem_open(SERVER_SEM, 0, 0644, 0); /* Open a preexisting semaphore. */
	if(memory_lock_sem == SEM_FAILED)
    {
      perror("unable to open memory_lock_sem semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

	response_contact_sem = sem_open(RESPONSE_CONTACT_SEM, 0, 0644, 0); /* Open a preexisting semaphore. */
	if(response_contact_sem == SEM_FAILED)
    {
      perror("unable to open response_contact_sem semaphore");
      sem_unlink(RESPONSE_CONTACT_SEM);
      exit(EXIT_FAILURE);
    }

	fdShMem = AllocateSharedMemory(SHMKEY, SERVER_BUF_SIZE);
	bufferShMem = MapSharedMemory(fdShMem);
	ftruncate(fdShMem, SERVER_BUF_SIZE);

    offset = bufferShMem;

	msg_t com;
	com.type = CONTACT;

	Msg_s response = communicate(&com);

	int client_number = atoi( (char *) response->msgList->pFirst->data );
	printf("I am the client number \"%d\"\n", client_number);
	
	if(client_number == -1){
		perror("No more slots available. Closing connection ...");
		exit(EXIT_FAILURE);
	}


	sprintf(servtocli_sem_name, "/servtocli_sem_%d", client_number);

	servtocli_sem = sem_open(servtocli_sem_name, 0, 0644, 0);
	if(servtocli_sem == SEM_FAILED)
	{
      perror("Unable to open server to client semaphore");
      sem_unlink(servtocli_sem_name);
      exit(EXIT_FAILURE);
    }

   
	sprintf(clitoserv_sem_name, "/clitoserv_sem_%d", client_number);

    clitoserv_sem = sem_open(clitoserv_sem_name, 0, 0644, 0);
	if(clitoserv_sem == SEM_FAILED)
	{
      perror("Unable to open client to server semaphore");
      sem_unlink(clitoserv_sem_name);
      exit(EXIT_FAILURE);
    }

    offset = bufferShMem+(client_number*CLIENT_BUF_SIZE);

	return ;

}