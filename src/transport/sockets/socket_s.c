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
#include <arpa/inet.h>
#include <string.h>



#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_un)

#define UNIX_PATH_MAX    108

struct sockaddr_un {
	sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[UNIX_PATH_MAX];  /* pathname */
 };


 struct sockaddr_un * getServerAddress(){

		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * address = malloc(SOCKET_SIZE);
		sprintf(path1,"/tmp/socket_server");
		memset(address, 0 , sizeof(struct sockaddr_un));
		address->sun_family = SAME_MACHINE_CONNECTION;
		memcpy(address->sun_path, path1, sizeof(address->sun_path)-1);

		return address;
}

 struct sockaddr_un * getUniqueAddress(int pid){

		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * address = malloc(SOCKET_SIZE);
		sprintf(path1,"/tmp/socket_%d", pid);
		memset(address, 0 , sizeof(struct sockaddr_un));
		address->sun_family = SAME_MACHINE_CONNECTION;
		memcpy(address->sun_path, path1, sizeof(address->sun_path)-1);

		return address;
}

void closeServer(char * server_path){
	// This function removes the created socket file in /tmp/
	unlink(server_path);

	return ;
}



int main(void){
	
	int sockfd; // Server socket file descriptor
	int newsockfd; // Child socket file descriptor


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

		struct sockaddr_un * addr_server = getServerAddress();


	
    // -1- "Bind" the server  address to the end point
	if( (bind( sockfd, (struct sockaddr *) addr_server, SOCKET_SIZE)) == -1 ){
		perror("<LOG socket_s.c> Bind call failed <end>");
		exit(EXIT_FAILURE);
	}

	// -2- Start listening for incoming connections

	// The server will be able to queue up to QUEUE_CONNECTION_SIZE incoming connection requests
	if( listen(sockfd, QUEUE_CONNECTION_SIZE) == -1){
		perror("<LOG socket_s.c> Listen call failed <end>");
		exit(EXIT_FAILURE);
	}

	int clientPID; // Buffer for storing received data

	// -3- Loop
	for ( ; ; )
	{

		/* Accept a connection */
		if((newsockfd = accept(sockfd, NULL, NULL)) == -1){
			perror("<LOG socket_s.c> Accept call failed <end>");
			continue;
		}

	// -4- Spawn a child to deal with the connection
	// Here we will have to create a new thread for our child

	if( fork() == 0){
		/* Receive data */
		while(recv(newsockfd, &clientPID, sizeof(clientPID), 0) > 0){
			printf("Server recibe el PID del cliente: %d\n", clientPID);
			// c = toupper(c);
			// send(newsockfd, &c, 1, 0);
		}
	}
	}

}