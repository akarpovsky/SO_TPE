

#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#if !defined(__GNU_LIBRARY__) || defined(_SEM_SEMUN_UNDEFINED)
union semun
{
	int val; // value for SETVAL
	struct semid_ds* buf; // buffer for IPC_STAT, IPC_SET
	unsigned short* array; // array for GETALL, SETALL
	struct seminfo* __buf; // buffer for IPC_INFO
};
#endif

int CreateSemaphoreSet(int n, short* vals);

void DeleteSemaphoreSet(int id);

void LockSemaphore(int id, int i);

void UnlockSemaphore(int id, int i);

#endif