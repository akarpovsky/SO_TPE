
/*
* 
* Shared memory server header
*
*/

#include "../../includes/share_ex.h"

#define SERVER_BUF_SIZE (10 * BUFSIZ)
#define CLIENT_BUF_SIZE BUFSIZ;

char SEM_NAME[]= "viik";

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
	// server_sem = (Sem) malloc(sizeof(sem_t));
	
	sem_t * server_sem; 
	// printf("Sem está en %d\n", (int) server_sem);
	sem_unlink(SEM_NAME);
	server_sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 0);
	if(server_sem == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SERVER_SEM);
      exit(EXIT_FAILURE);
    }	

    int value = -1; 
    sem_getvalue(server_sem, &value); 
    printf("The value of the semaphors APENAS lo creo %d\n", value);
    value = -1;



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
	value = -1; 
    sem_getvalue(server_sem, &value); 
    printf("The value of the semaphors ANTES DEL down %d\n", value);
    value = -1;
	
	while(1){
		down(server_sem);
		sem_getvalue(server_sem, &value); 
		printf("Me desbloqueó mi cliente!!!!\n");
    	printf("Valor dsp del desbloqueo %d\n", value);
    	// printf("Hago up %d\n", value);
    	// up(server_sem);
    	// sem_getvalue(server_sem, &value); 
    	// printf("Valor dsp del UP %d\n", value);
    }
	// sem_getvalue(server_sem, &value); 
 //    value = -1;
	// // printf("Luego del down sem está en %d\n", (int) server_sem);
	// printf("Waiting 10 secs to release sem\n");
	// // sleep(5);
	// sleep(5);

	// up(server_sem);
	// sem_getvalue(server_sem, &value); 
 //    printf("The value of the semaphors despues de UP es %d\n", value);
 //    value = -1;
	// printf("Borro y me voy!\n");
	// // }
	// // sleep(15);
	// memset(bufferShMem, 0, strlen(hola)-1);
	// // printf("2) %s\n", (char *) bufferShMem);


}



































