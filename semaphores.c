
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semaphores.h"

/**
* Creates a new semaphore set.
*
* @param n Number of semaphores in set.
* @param vals Default values to start off with.
* @return Id of semaphore set.
*/
int CreateSemaphoreSet(int n, short* vals)
{
	union semun arg;
	int id;
	assert(n > 0); /* We need at least one! */
	assert(vals != NULL); /* And they need initialising! */
	id = semget(IPC_PRIVATE, n, SHM_R | SHM_W);
	arg.array = vals;
	semctl(id, 0, SETALL, arg);
	return id;
}

/**
* Frees up the given semaphore set.
*
* @param id Id of the semaphore group.
*/
void DeleteSemaphoreSet(int id)
{
	if (semctl(id, 0, IPC_RMID, NULL) == -1)
	{
	perror("Error releasing semaphore!");
	exit(EXIT_FAILURE);
}
}
/**
* Locks a semaphore within a semaphore set.
*
* @param id Semaphore set it belongs to.
* @param i Actual semaphore to lock.
*
* @note If it’s already locked, you’re put to sleep.
*/
void LockSemaphore(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}

/**
* Unlocks a semaphore within a semaphore set.
*
* @param id Semaphore set it belongs to.
* @param i Actual semaphore to unlock.
*/
void UnlockSemaphore(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}