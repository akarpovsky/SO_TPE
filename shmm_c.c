/*
* 
* Shared memory client
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

char SEM_NAME[]= "viik";

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

void down(sem_t * sem){
	sem_wait(sem);
	return ;
}

void up(sem_t * sem){
	sem_post(sem);
	return ;
}

int main(void){
	int fdShMem; // Shared memory handle.
	void * bufferShMem;
	sem_t * server_sem;
	// printf("Sem está en %d\n", (int) server_sem);
	server_sem = sem_open(SEM_NAME, 0, 0644, 0); /* Open a preexisting semaphore. */
	if(server_sem == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }

	// printf("Sem está en %d\n", (int) server_sem);

	// char * dir = "/shared_memory";

	printf("keyy %d\n", SHMKEY);

	fdShMem = AllocateSharedMemory(SHMKEY, SERVER_BUF_SIZE);
	bufferShMem = MapSharedMemory(fdShMem);
	ftruncate(fdShMem, SERVER_BUF_SIZE);
	printf("Created and mapped shared memory in FD %d ; SHMM size = %d ; buffer = %d\n", fdShMem, BUFSIZ, (int) bufferShMem);
	

	// printf("Sem está en %d\n", (int) server_sem);
	int value = -1;
	sem_getvalue(server_sem, &value); 
    printf("The value of the semaphors ANTES del UP %d\n", value);
    value = -1;

	up(server_sem);
	// printf("Luego del down %d\n", (int) server_sem);
	printf("1) %s\n", (char *) bufferShMem);
	// up(server_sem);
	// while(1){	
	// }
}




