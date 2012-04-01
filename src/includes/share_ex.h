
#ifndef SHARE_EX_H
#define SHARE_EX_H

#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SHMKEY (key_t) 0x10
#define SERVER_SEM "/server"

#define MAX_CLIENTS 10
#define SERVER_BUF_SIZE (MAX_CLIENTS * BUFSIZ)
#define CLIENT_BUF_SIZE BUFSIZ;

typedef sem_t * Sem;

typedef struct channel_t {
	Sem memory_lock_sem; // Locks the shared memory so no data is corrupted
	Sem new_msg_sem; // Signals the client or the server saying: "You have data to read"
	void * buffer; // Where client will read and write data
} channel_t;

typedef channel_t * Channel;

#endif