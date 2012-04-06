#include "./server.h"

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

    printf("add_request: New request added in the request queue\n");

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

		Msg_s serverMsg = establishChannel((Channel) ch);

		// Send the CONTACT response created by establishChannel
	
		communicate(ch, serverMsg);

		if(serverMsg->status == -1){ // If server cant handle connection, dismiss client
			return ;
		}

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

void * client_thread(void * ch){
	printf("Inside client_thread\n");
	Channel client_channel = (Channel) ch;
	User me = NULL;

	Msg_t fromClient;

	for(;;){
		fromClient = IPClisten(client_channel);
		execute(fromClient, client_channel, &me);

	}

	// TO DO: Close thread
}

/*
 * function saver_thead_main()
 * 
 * This functions saves all the game data to the hard drive
 * each n secconds (n is a parametter).
 *
 */
void * saver_thread_main(void * data)
{
    
    int rc;	                 
    Request a_request;      /* Stores pointer to a request. */

    /* Lock the mutex, to access the requests list exclusively. */

    while (1) {
    		sleep((int) data);
    		printf("Saving all the data to the harddrive!\n");
			saveGame();

		}
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

    while (1) {
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

			printf("In the request listener waiting for new clients to arrive .... \n");
			rc = pthread_cond_wait(&got_request, &request_mutex);
		}
	}
}

int main(void){
	pthread_t creator_client_thread; // Desencola, crea, etc
	pthread_t match_reviewer_thread;
	pthread_t saver_thread;
 
 	// First of all be aware of system signals.
	// We must exit clean :)

	signal(SIGINT, sigint);

	int rc;

	// First of all take some time to load all the files inside the memory
	gameAux = loadGame();

	// Initialize the threadList
	clientThreadsList = (List) malloc(sizeof(client_t));
	CreateList(clientThreadsList);

	// Create server
	uplink();

	printf("In main creating thread for handling connection requests\n");
	rc = pthread_create(&creator_client_thread, NULL, creator_client_main, NULL);
	if (rc)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(EXIT_FAILURE);
	}	

	printf("In main creating match reviewer thread");
	rc = pthread_create(&match_reviewer_thread, NULL, match_reviewer_main, NULL);
	if(rc)
	{
		printf("ERROR; return code from phtread_create() is %d\n", rc);
		exit(EXIT_FAILURE);
	}

	int save_time = 5;
	printf("In main creating saver thread");
	rc = pthread_create(&saver_thread, NULL, saver_thread_main, save_time);
	if(rc)
	{
		printf("ERROR; return code from phtread_create() is %d\n", rc);
		exit(EXIT_FAILURE);
	}


	Msg_t auxMsg; 

	for ( ; ; ){

		auxMsg = IPClisten(NULL);

		// Encolar msg
		if(auxMsg->type == CONTACT)
			add_request(auxMsg, &request_mutex, &got_request);

	}
	
	printf("Exiting main thread !!.\n");
    
	pthread_exit(NULL);

	return EXIT_SUCCESS;
}

void * match_reviewer_main(void * data)
{

	checkMatchesDir();

	do
	{
		checkMatches();
	}while(1);
}

