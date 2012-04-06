/*
* 
* Socket client
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
#include <unistd.h>

#include "../../includes/defines.h"
#include "../../includes/message.h"


#define SAME_MACHINE_CONNECTION AF_INET
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_in)
#define UNIX_PATH_MAX    108

#define SERVER_PATH "/tmp/socket_server"

static int sockfd;
int newsockfd = 0; // Server socket file descriptor
struct sockaddr_in * client_address;
struct sockaddr_in * server_address;
struct sockaddr_in * new_server_address;	

struct sockaddr_in * fillServerData(){

		struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));

		address->sin_family = AF_INET;
		address->sin_port = 7000;
		address->sin_addr.s_addr = htonl(INADDR_ANY);

		return address;
}

 struct sockaddr_in * fillClientData(int pid){

		struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));

		address->sin_family = AF_INET;
		address->sin_port = INADDR_ANY;
		address->sin_addr.s_addr = htonl(INADDR_ANY);

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
	exit(EXIT_FAILURE);
}

void sigpipe(){  
	signal(SIGINT,sigpipe); /* reset signal */
	printf("<LOG socket_s.c> Client have received a SIGPIPE. <end> \n");
	printf("<LOG socket_s.c> Cannot talk to the server. Server may have been initialized wrong or it has crashed. <end> \n");
	exit(EXIT_FAILURE);
}

void bindToAssignedSocket(int port){
	
	printf("Haciendo bind a nueva dirección ... \n");
	new_server_address = malloc(sizeof(struct sockaddr_in));

	new_server_address->sin_family = AF_INET;
	new_server_address->sin_port = port;
	new_server_address->sin_addr.s_addr = htonl(INADDR_ANY);
	
	return ;
}

Msg_s rcvmessage(void){

	
	int rcvFlag = FALSE;
	Msg_s msg;

	do{
		int msgSize;
		int client_len = SOCKET_SIZE;
		void * bytestring;
		void * aux;

		int listenFD;
		if(newsockfd == 0){
			listenFD = sockfd;
		}else{
			listenFD = newsockfd;
		}
		
		if( (recvfrom(listenFD, &msgSize, sizeof(int), 0, (struct sockaddr *) new_server_address, (socklen_t *) &client_len)) == -1){
			perror("Error while receiving data");
			return NULL;
		}
		
		if(msgSize > 0){
			// printf("<LOG socket_c.c> Client - Message header received OK. Full message size = %d <end>\n", msgSize);

			aux = bytestring = calloc(msgSize, sizeof(char));
								

			if( (recv(listenFD, aux, (msgSize * sizeof(char)) + sizeof(int), 0)) == -1){
				perror("Reading server message failed");
				return NULL;
			}else{
				memcpy(&(msg->status), aux, sizeof(int));
				aux += sizeof(int);
				msg = (Msg_s) deserialize_s(aux);

				}
				rcvFlag = TRUE;
			}
	}while(!rcvFlag);

	return msg;
}




int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;
	void * msgstraux;

	msgstr = (Msg_t) serializeMsg(msg);
	memcpy(&msgSize, msgstr, sizeof(int));


	if(new_server_address != NULL){

		printf("Sending data to port #%d - Message size = %d\n", new_server_address->sin_port, msgSize);

		if((sendto(sockfd, &msgSize, sizeof(int), 0, (struct sockaddr *) new_server_address, SOCKET_SIZE)) == -1){
			perror("Error while trying to send a message to server.");
			printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
			exit(EXIT_FAILURE);
		}

		if((sendto(sockfd, msgstr, msgSize+sizeof(int), 0, (struct sockaddr *) new_server_address, SOCKET_SIZE)) == -1){
			perror("Error while trying to send a message to server.\n");
			printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
			exit(EXIT_FAILURE);
		}
	}else{

		printf("Sending data to port #%d - Message size = %d\n", server_address->sin_port, msgSize);
		
		if((sendto(sockfd, &msgSize, sizeof(int), 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
			perror("Error while trying to send a message to server.");
			printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
			exit(EXIT_FAILURE);
		}

		if((sendto(sockfd, msgstr, msgSize+sizeof(int), 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
			perror("Error while trying to send a message to server.\n");
			printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
			exit(EXIT_FAILURE);
		}		
	}

	// free(msgstr);
	return SUCCESSFUL;
}

Msg_s communicate(Msg_t msg)
{

	if(sendmessage(msg) == SUCCESSFUL){
		return rcvmessage();
	}

	return NULL;
}


void connectToServer(void){
	server_address = fillServerData();
	client_address = fillClientData(getpid());

	/* Transport endpoint */
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("<LOG socket_c.c> Socket call failed <end>");
		sigint();
		exit(EXIT_FAILURE);
	}

	int pid = getpid();

	// Create a connection message

	msg_t com;
	com.type = CONTACT;
	com.data.socket_client_t.client_pid = pid;

	Msg_s response;
	response = communicate(&com);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}

	bindToAssignedSocket(response->status);

	return ;

}