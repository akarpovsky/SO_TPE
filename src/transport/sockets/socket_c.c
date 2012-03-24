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
#include <sys/un.h>

#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_un)

#define UNIX_PATH_MAX    108


struct sockaddr_un * fillServerData(){
	
		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * server_address = calloc(1, SOCKET_SIZE);
		sprintf(path1,"/tmp/socket_server");
		memset(server_address, 0 , sizeof(struct sockaddr_un));
		memcpy(server_address->sun_path, path1, sizeof(server_address->sun_path));
		server_address->sun_family = SAME_MACHINE_CONNECTION;

		return server_address;
}

int main(void){

	int sockfd; // Server socket file descriptor

 	struct sockaddr_un * server_address = fillServerData();

	/* Transport endpoint */
	if( (sockfd = socket(SAME_MACHINE_CONNECTION, SOCK_STREAM, 0)) == -1){
		perror("<LOG socket_c.c> Socket call failed <end>");
		exit(EXIT_FAILURE);
	}

	/* Connect the socket to the server's address and then
		send and receive information from the server */

	
	if( (connect(sockfd, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("<LOG socket_c.c> Connect call failed <end>");
		printf("<LOG socket_c.c> Tried to open the server first? <end>\n");
		exit(EXIT_FAILURE);
	}

	/* Send and receive information with the server */

	// First of all we will send our PID so the server can create an
	// exclusive socket for listening me :)

	int clientPID = getpid();
	while(1){
		send(sockfd, &clientPID, sizeof(clientPID), 0);	
		sleep(3);
	}
	// char c, rc;
	// for( rc = '\n'; ; ){
	// 	if(rc == '\n')
	// 		printf("Input a lower case character\n");

	// 	c = getchar();

	// 	send(sockfd, &c, 1, 0);
	// 	if(recv(sockfd, &rc, 1, 0) > 0)
	// 		printf("Recibí: %c\n", rc);
	// 	else{
	// 		printf("Server has died\n");
	// 		close(sockfd);
	// 		exit(EXIT_FAILURE);
	// 	}

	// }

}