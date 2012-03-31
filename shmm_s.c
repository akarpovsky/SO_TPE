
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

#include "share_ex.h"

#define SERVER_BUF_SIZE (10 * BUFSIZ)
#define CLIENT_BUF_SIZE BUFSIZ;
#define SHMM_NAME "/shared_memory"




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

// The various semaphores used in this game
enum
{
	SEM_SERVER, 
	SEM_USER_1,
	SEM_USER_2,
	SEM_USER_3,
	SEM_USER_4,
	SEM_USER_5,
	SEM_USER_6,
	SEM_USER_7,
	SEM_USER_8,
	SEM_USER_9,
	SEM_USER_10
};

int AllocateSharedMem(char * name)
{

	return shm_open(name, O_RDWR|O_CREAT, 0666);

}

void * MapSharedMem(int size, int fdShMem)
{
	return mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fdShMem, 0);
}

int main(void){
	int fdShMem; // Shared memory handle.
	void * bufferShMem;

	// char * dir = "/shared_memory";

	printf("keyy %d\n", SHMKEY);

	fdShMem = AllocateSharedMemory(SHMKEY, SERVER_BUF_SIZE);
	bufferShMem = MapSharedMemory(fdShMem);
	ftruncate(fdShMem, SERVER_BUF_SIZE);
	printf("Created and mapped shared memory in FD %d ; SHMM size = %d ; buffer = %d\n", fdShMem, BUFSIZ, (int) bufferShMem);
	

	char * hola = "Hello world!\0";
	// while(1){
	memcpy(bufferShMem, (void *) hola, strlen(hola)-1);
	printf("1) %s\n", (char *) bufferShMem);

	sleep(10);
	// }
	// sleep(15);
	memset(bufferShMem, 0, strlen(hola)-1);
	// printf("2) %s\n", (char *) bufferShMem);


}



































