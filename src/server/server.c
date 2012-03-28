#define _GNU_SOURCE
#include <stdio.h>      
#include <pthread.h> 
#include <stdlib.h>

#include "./server.h"
#include "../includes/transport_s.h"
#include "../includes/mq_s.h"
#include "../includes/defines.h"
#include "../includes/structs.h"
#include "../includes/message.h"



/* Number of threads used to service requests */
#define NUM_HANDLER_THREADS 1


/* List for mantaining clients threads */
List clientThreadsList;

/* Global recursive mutex for our program */
pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/* Global mutex for game */
pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;

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
 * @msg: Message with CONTACT info
 * @request_num:     request number, linked list mutex.
 * @p_mutex:
 * @p_cond_var: 
 */
void add_request(Msg_t msg, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var){

	printf("Agregando request\n");
	int rc;
    Request a_request;      /* 'Pointer to newly added request */

    a_request = (Request)malloc(sizeof(struct request));

	// Check if malloc failed
	if(!a_request){
		printf("<LOG - server.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}

    a_request->msg = msg; // Data that will contain the request
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

    printf("add_request: New added requestd\n");

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
Request get_request(pthread_mutex_t * p_mutex){
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
void handle_request(Request a_request){
   
    if (a_request) { // Check if a_request is not NULL 

		printf("Handling new client request\n");
		
		Msg_t msg = a_request->msg;

		Channel ch = createChannel(msg);

		pthread_t clientThread;
		
		AddToList((void *) &clientThread, clientThreadsList);
					
		// I will give a new thread for each client

		int iRet;
		iRet = pthread_create(&clientThread, NULL, client_thread, (void *) ch);
		if (iRet){
			printf("ERROR; return code from pthread_create() is %d\n", iRet);
			exit(EXIT_FAILURE);
		}	
	}
}

void * createMsg_s(){

	Msg_s msg = (Msg_s) malloc(sizeof(msg_s));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}

	msg->msgList = (List) malloc(sizeof(llist));
		if(msg->msgList == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}

	CreateList(msg->msgList);

	return msg;

}

void * client_thread(void * ch){
	printf("Inside client_thread\n");
	establishChannel((Channel) ch);
	
	// Mando la respuesta de CONTACT
	Msg_s serverMsg = createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	communicate(ch, serverMsg);

	Msg_t fromClient;
	
	for(;;){
		
		fromClient = IPClisten(ch);
		printf("Type que me llego: %d\n", fromClient->type);
		// execute_c()
	}

	// TO DO: Close thread
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
void * creator_client_main(void * data)
{
    
    int rc;	                 
    Request a_request;      /* Stores pointer to a request. */

    /* Lock the mutex, to access the requests list exclusively. */
    rc = pthread_mutex_lock(&request_mutex);


 //   printf("thread '%d' after pthread_mutex_lock\n", thread_id);

    while (1) {

  //  	printf("thread '%d', num_requests =  %d\n", thread_id, num_requests);

		if (num_requests > 0) { /* A request is pending */
			a_request = get_request(&request_mutex);

			if (a_request) { /* Got a request - handle it and free it */
 				rc = pthread_mutex_unlock(&request_mutex);
				handle_request(a_request);
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

int main(void){
	pthread_t creator_client_thread; // Desencola, crea, etc
 
 	// First of all be aware of system signals.
	// We must exit clean :)

	signal(SIGINT, sigint);

	int rc;

	Game game; /* Game structure; will store all the game data */

	// First of all take some time to load all the files inside the memory
	game = loadGame();

	// Initialize the threadList
	clientThreadsList = (List) malloc(sizeof(client_t));
	CreateList(clientThreadsList);

	// Create server
	uplink();

	printf("In main creating thread for handling connection requests\n");
	rc = pthread_create(&creator_client_thread, NULL, creator_client_main, NULL);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(EXIT_FAILURE);
	}	


	Msg_t auxMsg; 

	for ( ; ; ){

		auxMsg = IPClisten(NULL);
		// Encolar msg
		add_request(auxMsg, &request_mutex, &got_request);

	}
	
	printf("Exiting main thread !!.\n");
    
	pthread_exit(NULL);

	return EXIT_SUCCESS;
}