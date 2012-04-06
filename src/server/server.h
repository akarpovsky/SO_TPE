#ifndef SERVER_H_
#define SERVER_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include "../includes/transport_s.h"

#include "../includes/defines.h"
#include "../includes/structs.h"
#include "../includes/message.h"
#include "../includes/execute.h"
#include "../IO/io.h"
#include "../utils/LinkedList.h"

#ifdef fifo
	#include "../includes/fifo_s.h"
#endif
#ifdef sockets
	#include "../includes/socket_s.h"
#endif
#ifdef msgqueue
	#include "../includes/mq_s.h"
#endif
#ifdef shmm
	#include "../includes/share_ex.h"
#endif

#define DEFINE_VARIABLES
#include "../includes/global.h"

/* Number of threads used to service requests */
#define NUM_HANDLER_THREADS 1

/* Structure of a single request. */
typedef struct request {
    Msg_t msg;		    /* Request number */
    struct request * next;   /* Pointer to next request, NULL if none. */
} request;

typedef request * Request;

void add_request(Msg_t msg, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var);
Request get_request(pthread_mutex_t* p_mutex);
void * client_thread(void * ch);
void handle_request(Request a_request);
void * match_reviewer_main(void * data);

#endif
