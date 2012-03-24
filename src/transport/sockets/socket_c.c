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
#define SOCKET_SIZE sizeof(struct sockaddr_in)


int main(void){

	int sockfd; // Server socket file descriptor

	struct sockaddr_in server_addr = {SAME_MACHINE_CONNECTION, 7000};

	//server_add.sin_addr.s_addr = inet_addr("")


	/* Transport endpoint */
	if( (sockfd = socket(SAME_MACHINE_CONNECTION, SOCK_STREAM, 0)) == -1){
		perror("<LOG socket_c.c> Socket call failed <end>");
		exit(EXIT_FAILURE);
	}

	/* Connect the socket to the server's address and then
		send and receive information from the server */

	if( (connect(sockfd, (struct sockaddr * ) &server_addr, SOCKET_SIZE)) == -1){
		perror("Connect call failed");
		exit(EXIT_FAILURE);
	}

	/* Send and receive information with the server */

}