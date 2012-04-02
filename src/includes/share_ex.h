
#ifndef SHARE_EX_H
#define SHARE_EX_H

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

#define SHMKEY (key_t) 0x10
#define SERVER_SEM "/server_lock"
#define SERVER_MSG_SEM "/server_msg"

#define MAX_CLIENTS 10
#define SERVER_BUF_SIZE ((MAX_CLIENTS+1) * BUFSIZ)
#define CLIENT_BUF_SIZE BUFSIZ

typedef sem_t * Sem;


#endif