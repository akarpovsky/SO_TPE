/*
* 
* Socket server
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_in)


int main(void){
	
	int sockfd; // Server socket file descriptor
	int newsockfd; // Child socket file descriptor

	struct sockaddr_in socket_server = {SAME_MACHINE_CONNECTION, 7000, INADDR_ANY};


	/* Transport endpoint */
	if( (sockfd = socket(SAME_MACHINE_CONNECTION, SOCK_STREAM, 0)) == -1){
		perror("<LOG socket_s.c> Socket call failed <end>");
		exit(EXIT_FAILURE);
	}

	/* FROM HAVILAND (for understanding purposes):

		-1- "bind" the server  address to the end point

		-2- start listening for incoming connections

		-3- loop
				accept a connection
				spawn a chld to deal with the connection (thread!)
				if child
					send an receive information with the client
	*/


    // -1- "Bind" the server  address to the end point
	if( (bind( sockfd, (struct sockaddr *) &(socket_server), SOCKET_SIZE)) == -1 ){
		perror("Bind call failed");
		exit(EXIT_FAILURE);
	}

	// -2- Start listening for incoming connections

	// The server will be able to queue up to QUEUE_CONNECTION_SIZE incoming connection requests
	if( listen(sockfd, QUEUE_CONNECTION_SIZE) == -1){
		perror("Listen call failed");
		exit(EXIT_FAILURE);
	}

	// -3- Loop
	for ( ; ; )
	{
		/* Accept a connection */
		if((newsockfd = accept(sockfd, NULL, NULL)) == -1){
			perror("Accept call failed");
			continue;
		}
	}

	// -4- Spawn a child to deal with the connection

}