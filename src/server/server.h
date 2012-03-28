#ifndef SERVER_H_
#define SERVER_H_

#include <pthread.h>

#include "../includes/structs.h"
#include "../IO/io.h"
#include "../includes/message.h"
#include "../utils/LinkedList.h"

/* Structure of a single request. */
typedef struct request {
    Msg_t msg;		    /* Request number */
    struct request * next;   /* Pointer to next request, NULL if none. */
} request;

typedef request * Request;

void add_request(Msg_t msg, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var);
Request get_request(pthread_mutex_t* p_mutex);
void * client_thread();
void handle_request(Request a_request);

#endif
