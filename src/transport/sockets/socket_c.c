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
#include <signal.h>

#include "../../includes/defines.h"
#include "../../includes/message.h"


#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_un)
#define UNIX_PATH_MAX    108

#define SERVER_PATH "/tmp/socket_server"

int sockfd; // Server socket file descriptor
struct sockaddr_un * client_address;


struct sockaddr_un * fillServerData(){
	
		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * server_address = calloc(1, SOCKET_SIZE);
		sprintf(path1, SERVER_PATH);
		memset(server_address, 0 , sizeof(struct sockaddr_un));
		memcpy(server_address->sun_path, path1, sizeof(server_address->sun_path));
		server_address->sun_family = SAME_MACHINE_CONNECTION;

		return server_address;
}


 struct sockaddr_un * fillClientData(int pid){

		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * address = malloc(SOCKET_SIZE);
		sprintf(path1,"/tmp/socket_client_%d", pid);
		memset(address, 0 , sizeof(struct sockaddr_un));
		address->sun_family = SAME_MACHINE_CONNECTION;
		memcpy(address->sun_path, path1, sizeof(address->sun_path)-1);

		return address;
}

void closeClient(char * client_path){
	// This function removes the created socket file in /tmp/
	unlink(client_path);
	return ;
}

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG socket_s.c> Client have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	printf("Cerrando: %s\n", client_address->sun_path);
	closeClient(client_address->sun_path);
	exit(EXIT_FAILURE);
}

Msg_s rcvmessage(void){

	int rcvFlag = FALSE;
	Msg_s msg = calloc(1, sizeof(msg_s));


	do{
		int nread, msgSize;
		int client_len = SOCKET_SIZE;
		void * bytestring;
		void * aux;
		

		if( (recvfrom(sockfd, &msgSize, sizeof(int), MSG_WAITALL, NULL, NULL)) == -1){
			perror("Error while receiving data");
			return NULL;
		}

		
		if(msgSize > 0){
			printf("<LOG socket_c.c> Client: Recibiendo mensaje de tamaño = %d <end>\n", msgSize);

			

			aux = bytestring = calloc(msgSize, sizeof(char));
			
								

			if( (recvfrom(sockfd, aux, msgSize * sizeof(char), MSG_WAITALL, NULL, NULL)) == -1){
				perror("Reading server message failed");
				return NULL;
			}
			else{
				int status;
				memcpy(&(msg->status), aux, sizeof(int));
				aux += sizeof(int);

				printf("Client - Received message status: %d\n", msg->status);


				List l = malloc(sizeof(llist));
				CreateList(l);

				msg->msgList = l;

				int cantElem;
				memcpy(&(cantElem), aux, sizeof(int));
				aux += sizeof(int);

				int i;
				int strsize;
				char * str;

				for(i = 0; i < cantElem; i++){
					memcpy(&(strsize), aux, sizeof(int));
					aux += sizeof(int);

					str = malloc(strsize);
					memcpy(str, aux, strsize);
					aux += strsize;
					printf("Recibido: %s\n", str);
					AddToList(str, msg->msgList);
				}
				rcvFlag = TRUE;
			}
		}
	}while(!rcvFlag);

	return msg;
}




int main(void){

	signal(SIGINT,sigint);


 	struct sockaddr_un * server_address = fillServerData();
 	client_address = fillClientData(getpid());

	/* Transport endpoint */
	if( (sockfd = socket(SAME_MACHINE_CONNECTION, SOCK_DGRAM, 0)) == -1){
		perror("<LOG socket_c.c> Socket call failed <end>");
		sigint();
		exit(EXIT_FAILURE);
	}



	if( (bind( sockfd, (struct sockaddr *) client_address, SOCKET_SIZE)) == -1 ){
		perror("<LOG socket_s.c> Bind call failed <end>");
		sigint();
		exit(EXIT_FAILURE);
	}



	int pid = getpid();

	if((sendto(sockfd, &pid, sizeof(int), 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("Error while trying to send a msg.\n");
		closeClient(client_address->sun_path);
		// continue ;
	}


	rcvmessage();

	// /* Connect the socket to the server's address and then
	// 	send and receive information from the server */

	
	// if( (connect(sockfd, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
	// 	perror("<LOG socket_c.c> Connect call failed <end>");
	// 	printf("<LOG socket_c.c> Tried to open the server first? <end>\n");
	// 	exit(EXIT_FAILURE);
	// }

	/* Send and receive information with the server */

	// First of all we will send our PID so the server can create an
	// exclusive socket for listening me :)

	// int clientPID = getpid();
	// send(sockfd, &clientPID, sizeof(clientPID), 0);	
	
	// int ping = 0;

	// if(recv(sockfd, &ping, sizeof(int), 0) > 0)
	// 	printf("Recibí: %d\n", ping);
	// else
	// 	printf("Server has died\n");

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
	// sleep(3);
	closeClient(client_address->sun_path);

}