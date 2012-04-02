/*
* 
* Shared memory client
*
*/

#include "../includes/share_ex.h"
#include "../includes/shmm_c.h"

#include "../../includes/message.h"
#include "../../utils/LinkedList.h"
#include "../../includes/defines.h"
#include "../../includes/transport_s.h"

int fdShMem; // Shared memory handle file descriptor
void * bufferShMem; // Where shared data will be stored!
void * offset; 
Sem memory_lock_sem;
Sem new_msg_sem;

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


Msg_s communicate(Msg_t msg)
{

	if(sendmessage(msg) == SUCCESSFUL){
		return rcvmessage();
	}

	return NULL;
}

Msg_s rcvmessage(void)
{
	
	down(new_msg_sem);

	int rcvFlag = FALSE;

	Msg_s msg = malloc(sizeof(msg_s));

	do{
		int nread, msgSize;
		void * bytestring;
		void * aux;

		down(memory_lock_sem);
			aux = bytestring = malloc(msgSize);
			memcpy(&msgSize, offset, sizeof(int));
			memcpy(aux, offset, msgSize);
			msg = deserialize_s(aux);
		up(memory_lock_sem);
		
		rcvFlag = TRUE;

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
	down(memory_lock_sem);
		memcpy(offset, msgstr, msgSzie+sizepf(int));
	up(memory_lock_sem);

	up(new_msg_sem);

	// free(msgstr);

	return SUCCESSFUL;
}

void connectToServer(void)
{

	server_contact_sem = sem_open(SERVER_MSG_SEM, 0, 0644, 0);
	if(server_contact_sem == SEM_FAILED)
    {
      perror("Unable to open CONTACT semaphore. Maybe the server is down");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

    up(server_contact_sem);

	msg_t com;
	com.type = CONTACT;
	com.data.tempnam = fifoIn;

	Msg_s response = communicate(&com);

	int client_number = response->status;
	if(status == -1){
		perror("No more slots available. Closing connection ...");
		exit(EXIT_FAILURE);
	}

	fdShMem = AllocateSharedMemory(SHMKEY, SERVER_BUF_SIZE);
	bufferShMem = MapSharedMemory(fdShMem);
	ftruncate(fdShMem, SERVER_BUF_SIZE);

	memory_lock_sem = sem_open(SERVER_SEM, 0, 0644, 0); /* Open a preexisting semaphore. */
	if(SERVER_SEM == SEM_FAILED)
    {
      perror("unable to create memory_lock_sem semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

	char sem_name[20];
	sprintf(sem_name, "/client_sem_%d", client_number);

	new_msg_sem = sem_open(sem_name, 0, 0644, 0);
	if(new_msg_sem == SEM_FAILED)
	{
      perror("Unable to create mew_msg_sem semaphore");
      sem_unlink(sem_name);
      exit(EXIT_FAILURE);
    }

    offset = bufferShMem+(client_number*CLIENT_BUF_SIZE);

}