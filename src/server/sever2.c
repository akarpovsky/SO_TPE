#define _GNU_SOURCE
#include <stdio.h>      
#include <pthread.h> 
#include <stdlib.h>

#include "./server.h"




/* Global recursive mutex for our program */
pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/* Global condition variable for our program that
	will be signaled each time a new request arrives
	to the server */
pthread_cond_t  got_request   = PTHREAD_COND_INITIALIZER;

/* Pending requests */
int num_requests = 0;

Request requests = NULL;     /* Head of linked list of requests. */
Request last_request = NULL; /* Pointer to last request.      

