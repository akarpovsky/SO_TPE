#define _GNU_SOURCE
#include <stdio.h>      
#include <pthread.h> 
#include <stdlib.h>
#include "./server.h"

/* Number of threads used to service requests */
#define NUM_HANDLER_THREADS 1


/* List for mantaining clients threads */
List clientThreadsList;

/* Global recursive mutex for our program */
pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/* Global condition variable for our program that
	will be signaled each time a new request arrives
	to the server */
pthread_cond_t  got_request = PTHREAD_COND_INITIALIZER;

/* Pending requests */
int num_requests = 0;

/* Online clients */
int numberOfClients = 0;


Request requests = NULL;     /* Head of linked list of requests. */
Request last_request = NULL; /* Pointer to last request.         */

/*
 * function add_request()
 * 
 *	Add a request to the requests list and
 * 	increases number of pending requests by one.
 * 
 * @request_num:     request number, linked list mutex.
 * @request_num:     request number, linked list mutex.
 * @p_mutex:
 * @p_cond_var: 
 */
void add_request(int request_num, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var){

	int rc;
    Request a_request;      /* 'Pointer to newly added request */

    a_request = (Request)malloc(sizeof(struct request));

	// Check if malloc failed
	if(!a_request){
		printf("<LOG - server.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}

    a_request->number = request_num; // Data that will contain the request
    a_request->next = NULL;

    /* Lock the mutex, to assure exclusive access to the list */
    rc = pthread_mutex_lock(p_mutex);

    /* Add new request to the end of the list */

	if (num_requests == 0) { // List is empty
		requests = a_request;
		last_request = a_request;
	}else{
		last_request->next = a_request;
		last_request = a_request;
 	}

    /* Increase total number of pending requests */
    num_requests++;


    printf("add_request: New added request with id '%d'\n", a_request->number);

    /* Signal the condition variable because there's a new request to handle */
    rc = pthread_cond_signal(p_cond_var);

    /* Unlock mutex */
    rc = pthread_mutex_unlock(p_mutex);
}

/*
 * function get_request
 * 
 * Returns a pointer to the first pending request from the requests list
 * 
 *
 * p_mutex: Just the mutex that is asigned to the request list
 *
 */
Request get_request(pthread_mutex_t* p_mutex){
    int rc;	                   
	Request a_request;

    /* Lock the mutex, to assure exclusive access to the list */
    rc = pthread_mutex_lock(p_mutex);

	if (num_requests > 0) { // Check if there are pending requests
		a_request = requests;
		requests = a_request->next;
		
		if (requests == NULL) { // Last request on the list
			last_request = NULL;
	}
	
		num_requests--; // Decrease the total number of pending requests
	}else{ // No new requests
		a_request = NULL;
	}

    /* Unlock mutex */
    rc = pthread_mutex_unlock(p_mutex);

    /* Return the request to the caller. */
	return a_request;
}

/*
 * function handle_request()
 *
 * Handle a single given request.
 * 
 * @a_request: Pointer to the request that needs to be handled
 *
 */
void handle_request(Request a_request, int thread_id){
    if (a_request) { // Check if a_request is not NULL 

		printf("Handling new client request\n");
		// First validate if the request is a new_client_connection request!
		
		// If a client ask for a connection ...
		pthread_t clientThread;
		
		CreateList(clientThreadsList);	
		AddToList((void *) &clientThread, clientThreadsList);
					
		// I will give a new thread for each client
		
		int iRet;
		numberOfClients++;
		iRet = pthread_create(&clientThread, NULL, client_thread, (void *) numberOfClients);

		printf("Thread '%d' handled request '%d'\n", thread_id, a_request->number);


	}
}

void * client_thread(void * data){
	
	while(1){
		printf("Soy el cliente %d!! \n", (int) data);
		if ((int) data == 1)
		{
			sleep(2);
		}else
			sleep(5);
	}
	// Acá se le debería informar al IPC qué thread lo está atendiendo
	
	
}

/*
 * function request_listener()
 * 
 * Infinite loop of requests handling
 * If there are requests to handle, take the first
 * and handle it. Then wait on the given condition variable,
 * and when it is signaled, re-do the loop.
 *
 */
void * request_listener(void* data)
{
//	printf("Entré al listener\n");
    
    int rc;	                 
    Request a_request;      /* Stores pointer to a request. */
    int thread_id = *((int*)data);  /* Thread identifying number */

    /* Lock the mutex, to access the requests list exclusively. */
    rc = pthread_mutex_lock(&request_mutex);


 //   printf("thread '%d' after pthread_mutex_lock\n", thread_id);

    while (1) {

  //  	printf("thread '%d', num_requests =  %d\n", thread_id, num_requests);
  

		if (num_requests > 0) { /* A request is pending */
			a_request = get_request(&request_mutex);
			if (a_request) { /* Got a request - handle it and free it */
 				rc = pthread_mutex_unlock(&request_mutex);
				handle_request(a_request, thread_id);
				free(a_request);
				rc = pthread_mutex_lock(&request_mutex);
			}
		}else{
	    /* wait for a request to arrive. note the mutex will be */

    	 //   printf("thread '%d' before pthread_cond_wait\n", thread_id);
    	  //  fflush(stdout);
			printf("In the request listener waiting for new clients to arrive .... \n");
			rc = pthread_cond_wait(&got_request, &request_mutex);

    	    //printf("thread '%d' after pthread_cond_wait\n", thread_id);
    	   // fflush(stdout);
		}
	}
}

void generateRequests(void){
	int i;
	/* Genero requests para testing */
	for (i=0; i<3; i++) {
		add_request(i, &request_mutex, &got_request);
	}
}

int main(void){
    int        i;                                /* loop counter          */
    int        thr_id[NUM_HANDLER_THREADS];      /* thread IDs            */
    pthread_t  p_threads[NUM_HANDLER_THREADS];   /* thread's structures   */
	int rc;

	Game game; /* Game structure; will store all the game data */

	// First of all take some time to load all the files inside the memory
	game = loadGame();

	// Initialize the threadList
	clientThreadsList = (List) malloc(sizeof(client_t));
	CreateList(clientThreadsList);


    /* Create the request-handling threads */
	for (i=0; i<NUM_HANDLER_THREADS; i++) {
		thr_id[i] = i;
		printf("In main creating thread %d for handling connection requests\n", i);
		rc = pthread_create(&p_threads[i], NULL, request_listener, &thr_id[i]);
	    
		if (rc){
		         printf("ERROR; return code from pthread_create() is %d\n", rc);
		         exit(-1);
		      }	

    }

	generateRequests();

	
   printf("Termine !!.\n");
    
   pthread_exit(NULL);

return 0;
}