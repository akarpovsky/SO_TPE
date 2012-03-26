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
#include <signal.h>
#include <string.h>

#include "../../includes/message.h"
#include "../../utils/LinkedList.h"
#include "../../includes/defines.h"



#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_un)

#define UNIX_PATH_MAX    108
#define SERVER_PATH "/tmp/socket_server"




struct sockaddr_un {
	sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[UNIX_PATH_MAX];  /* pathname */
 };

 typedef struct channel_t {
	struct sockaddr_un * client;
	int sockfd;
} channel_t;

typedef channel_t * Channel;


int sockfd; // Server socket file descriptor
int newsockfd; // Child socket file descriptor


 struct sockaddr_un * getClientChannel(char * path, int socket_family){

		struct sockaddr_un * address = malloc(SOCKET_SIZE);
		memset(address, 0 , sizeof(struct sockaddr_un));
		address->sun_family = socket_family;
		memcpy(address->sun_path, path, sizeof(address->sun_path)-1);
		return address;
}

 struct sockaddr_un * getServerAddress(){

		char path1[UNIX_PATH_MAX];
		struct sockaddr_un * address = malloc(SOCKET_SIZE);
		sprintf(path1, SERVER_PATH);
		memset(address, 0 , sizeof(struct sockaddr_un));
		address->sun_family = SAME_MACHINE_CONNECTION;
		memcpy(address->sun_path, path1, sizeof(address->sun_path)-1);

		return address;
}

void closeServer(char * server_path){
	// This function removes the created socket file in /tmp/
	unlink(server_path);
	close(sockfd);
	close(newsockfd);

	return ;
}

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG socket_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	closeServer(SERVER_PATH);
	exit(EXIT_FAILURE);
}

void upLink(void){

	/* Transport endpoint */
	if( (sockfd = socket(SAME_MACHINE_CONNECTION, SOCK_DGRAM, 0)) == -1){
		perror("<LOG socket_s.c> Socket call failed <end>");
		exit(EXIT_FAILURE);
	}

	// Set SO_REUSEADDR on a socket to true (1).
	// This will help us if the server unexpectedly crashes and we need to
	// restart it. If we do not use this option, we must remove manually the
	// /tmp/socket_server file. This will prevent bind call failures

	int optval = 1;

	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int)) == -1) {
	    perror("setsockopt");
	    exit(1);
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
}

Channel createChannel(Msg_t msg)
{
	Channel ch = malloc(sizeof(channel_t));
	ch->client = getClientChannel(msg->data.socket_client_t.socket_path, msg->data.socket_client_t.socket_family);
	
	return ch;
}

int establishChannel(Channel ch)
{
	ch->sockfd = sockfd;
	
	return 0;
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch, msg);
}

int sendmessage(Channel ch, Msg_s msg){


	int msgSize;
	void * msgstr;
	void * msgstraux;
	int NumEl = msg->msgList->NumEl;
	int * sizes = malloc(NumEl * sizeof(int));
	char ** strings = malloc(NumEl * sizeof(char *));
	int msgListSize = 0;
	int i = 0;

	Element e;

	printf("Empiezo a armar las listas\n");
	FOR_EACH(e, msg->msgList)
	{
		printf("\tProcesando _%s_\n", (char *) e->data);
		sizes[i] = strlen(e->data)+1;
		printf("Guardo en sizes[i] = %d\n", sizes[i]);

		strings[i] = e->data;
		msgListSize += sizes[i];
		i+=1;
	}

	printf("Terminé de armar las listas\n");
	printf("msgListSize = %d\n", msgListSize);

	msgSize = 2*sizeof(int) + msgListSize*sizeof(int) + msgListSize ;
	msgstr = msgstraux = calloc(msgSize, sizeof(char));

	printf("msgSize = %d\n", msgSize);

	memcpy(msgstraux, &(msg->status), sizeof(int));
	msgstraux += sizeof(int);


	memcpy(msgstraux, &(msg->msgList->NumEl), sizeof(int));
	msgstraux += sizeof(int);

	for(i = 0; i < msg->msgList->NumEl; i++)
	{
		memcpy(msgstraux, &(sizes[i]), sizeof(int));
		msgstraux += sizeof(int);
		printf("Memcpy de mensaje con size[%d] = %d\n", i, sizes[i]);

		memcpy(msgstraux, strings[i], sizes[i]);
		msgstraux += sizes[i];

		printf("Memcpy de mensaje con str[%d] = %s\n", i, strings[i]);

	}


	if((sendto(ch->sockfd, &msgSize, sizeof(int), MSG_WAITALL, (struct sockaddr *) ch->client, SOCKET_SIZE) == -1)){
		perror("<LOG socket_s.c> Server: Could not write message size <end>");
		return !SUCCESSFUL;
	}else{
		printf("<LOG socket_s.c> Server: Envio el message size = %d <end>\n", msgSize);
	}


	if((sendto(ch->sockfd, msgstr, msgSize, MSG_WAITALL, (struct sockaddr *) ch->client, SOCKET_SIZE) == -1)){
		perror("<LOG socket_s.c> Server: Could not write message <end>");
		return !SUCCESSFUL;
	}else{
		printf("<LOG socket_s.c> Server: Envio la lista de mensajes al cliente <end>", msgSize);
	}

	// free(msgstr);
	return SUCCESSFUL;

}



int main(void){
	
	// First of all be aware of system signals.
	// We must exit clean :)

	signal(SIGINT,sigint);

	upLink();

	struct sockaddr_un * client = malloc(sizeof(SOCKET_SIZE));
	int client_size = SOCKET_SIZE;
	
	int msg = 0;

	for ( ; ; ){
		
		if( (recvfrom(sockfd, &msg, sizeof(int), MSG_WAITALL, (struct sockaddr *) client, &client_size)) == -1){
			perror("Error while receiving data");
			continue ;
		}


		printf("Recibí PID: %d\n\n", msg);


		/*
			*
			* Mensaje de prueba para create channel
			*

		*/
		msg_t test;

		char client_path[50];
		sprintf(client_path, client->sun_path);
		memcpy(test.data.socket_client_t.socket_path, client_path, sizeof(test.data.socket_client_t.socket_path)-1);
		test.data.socket_client_t.socket_family = client->sun_family;

		
		Channel ch = createChannel(&test);
		establishChannel(ch);


		List l = (List) malloc(sizeof(llist));

		CreateList(l);
		AddToList("Coca-Cola", l);
		AddToList("Prueba", l);
		AddToList("Mensaje laaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaargo", l);
		AddToList("Fin", l);
		Element e;

		msg_s mymen;
		mymen.status = 38;
		mymen.msgList = l;
		printf("Mensaje de prueba creado:\n");
		FOR_EACH(e, mymen.msgList)
		{
			printf("\t%s\n", (char *) e->data);
		}



		sendmessage(ch, &mymen);


	}

	// // -2- Start listening for incoming connections

	// // The server will be able to queue up to QUEUE_CONNECTION_SIZE incoming connection requests
	// if( listen(sockfd, QUEUE_CONNECTION_SIZE) == -1){
	// 	perror("<LOG socket_s.c> Listen call failed <end>");
	// 	exit(EXIT_FAILURE);
	// }

	// int clientPID; // Buffer for storing received data


	// // -3- Loop
	// for ( ; ; )
	// {

	// 	/* Accept a connections */
	// 	newsockfd = accept(sockfd, NULL, NULL);

	// 	if( newsockfd == -1){
	// 		perror("<LOG socket_s.c> Accept call failed <end>");
	// 		continue;
	// 	}else{
	// 		printf("<LOG socket_s.c> New connection accepted. Created unique socket file descriptor: newsockfd = %d <end>\n", newsockfd);
	// 	}
	
	// -4- Spawn a child to deal with the connection
	// Here we will have to create a new thread for our child

	// if( fork() == 0){
	// 	/* Receive data */
	// 	while(recv(newsockfd, &clientPID, sizeof(clientPID), 0) > 0){
	// 		printf("Server recibe el PID del cliente: %d\n", clientPID);

	// 		}
	// 	}
	// }

}