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
#include <unistd.h>

#include "../../includes/message.h"
#include "../../utils/LinkedList.h"
#include "../../includes/defines.h"
#include "../../includes/socket_s.h"
#include "../../includes/transport_s.h"


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

void uplink(void){

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

	// printf("Empiezo a armar las listas\n");
	FOR_EACH(e, msg->msgList)
	{
		// printf("\tProcesando _%s_\n", (char *) e->data);
		sizes[i] = strlen(e->data)+1;
		// printf("Guardo en sizes[i] = %d\n", sizes[i]);

		strings[i] = e->data;
		msgListSize += sizes[i];
		i+=1;
	}

	// printf("Terminé de armar las listas de mensajes\n");
	// printf("msgListSize = %d\n", msgListSize);
	msgSize = 2*sizeof(int) + (i*sizeof(int)) + msgListSize ;
	msgstr = msgstraux = calloc(msgSize, sizeof(char));

	// printf("Tamaño del mensaje a enviar = %d\n", msgSize);

	memcpy(msgstraux, &(msg->status), sizeof(int));
	msgstraux += sizeof(int);


	memcpy(msgstraux, &(msg->msgList->NumEl), sizeof(int));
	msgstraux += sizeof(int);

	for(i = 0; i < msg->msgList->NumEl; i++)
	{
		memcpy(msgstraux, &(sizes[i]), sizeof(int));
		msgstraux += sizeof(int);

		memcpy(msgstraux, strings[i], sizes[i]);
		msgstraux += sizes[i];


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
		printf("<LOG socket_s.c> Server: Envio la lista de mensajes al cliente <end>\n");
	}

	// free(msgstr);
	return SUCCESSFUL;
}


Msg_t IPClisten(Channel ch){

	printf("\nServer listening ...\n\n");
	int rcvFlag = FALSE;
	Msg_t msg = calloc(1, sizeof(msg_t));

	do{
		int msgSize, user_len, pass_len, from_len, to_len, name_len;
		int client_len = SOCKET_SIZE;
		void * bytestring;
		void * aux;
		struct sockaddr_un * client;



		int listenFD;
		if(ch == NULL){
			listenFD = sockfd;
		}
		else{
			listenFD = ch->sockfd;
			client = ch->client;
		}


		int client_size = SOCKET_SIZE;

		if( (recvfrom(listenFD, &msgSize, sizeof(int), MSG_WAITALL, NULL, NULL)) == -1){
			perror("Error while receiving data");
			continue ;
		}

		if(msgSize > 0){
			
			aux = bytestring = malloc(msgSize);
			

			printf("<LOG socket_c.c> Server - Message header received OK. Full message size = %d <end>\n", msgSize);

			if( (recvfrom(listenFD, aux, msgSize * sizeof(char), MSG_WAITALL, NULL, NULL)) == -1){
				perror("Reading client message failed");
				return NULL;
			}
			else{
				
				memcpy(&(msg->type), aux, sizeof(int));
				aux += sizeof(int);

				printf("<LOG socket_c.c> Server - Received message type: %d <end>\n", msg->type);

				int type = msg->type;
				switch(type){
					
					case CONTACT:
						
						printf("\nCONTACT message received\n");
						printf("\n<data>\n");
						memcpy(&(msg->data.socket_client_t.client_pid), aux, sizeof(int));
						aux += sizeof(int);
						printf("\tCLIENT_PID = %d \n", msg->data.socket_client_t.client_pid);

						memcpy(&(msg->data.socket_client_t.socket_family), aux, sizeof(int));
						aux += sizeof(int);
						printf("\tSOCKET_FAMILY = %d \n", msg->data.socket_client_t.socket_family);
					
						int path_len = 0;
						memcpy(&(path_len), aux, sizeof(int));
						aux += sizeof(int);
						printf("\tSOCKET_PATH_LENGHT = %d \n", path_len);

						msg->data.socket_client_t.socket_path = calloc(path_len, sizeof(char));
						memcpy(msg->data.socket_client_t.socket_path, aux, path_len);
						printf("\tSOCKET_PATH = %s \n", msg->data.socket_client_t.socket_path);
						printf("</data>\n\n");
					
						break;

					case REGISTER:

							printf("\nREGISTER message received\n");
							printf("\n<data>\n");
							
							user_len = 0;
							memcpy(&(user_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.register_t.user = calloc(user_len,sizeof(char));
							memcpy(msg->data.register_t.user, aux, user_len);
							printf("\tUsername = %s \n", msg->data.register_t.user);
							aux += user_len;

							pass_len = 0;
							memcpy(&(pass_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.register_t.pass = calloc(pass_len,sizeof(char));
							memcpy(msg->data.register_t.pass, aux, pass_len);
							printf("\tPassword = %s \n", msg->data.register_t.pass);
							aux += pass_len;

							printf("</data>\n\n");

						break;

					case LOGIN:

							printf("\nLOGIN message received\n");
							printf("\n<data>\n");
							
							user_len = 0;
							memcpy(&(user_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.register_t.user = calloc(user_len,sizeof(char));
							memcpy(msg->data.register_t.user, aux, user_len);
							printf("\tUsername = %s \n", msg->data.register_t.user);
							aux += user_len;

							pass_len = 0;
							memcpy(&(pass_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.register_t.pass = calloc(pass_len,sizeof(char));
							memcpy(msg->data.register_t.pass, aux, pass_len);
							printf("\tPassword = %s \n", msg->data.register_t.pass);
							aux += pass_len;

							printf("</data>\n\n");

						break;

				case LIST_LEAGUES:
				case LIST_TEAMS:
				case LIST_TRADES:
						printf("\nLIST_TEAMS / LIST_LEAGUES / LIST_TRADES message received\n");

						break;

				case LEAGUE_SHOW:
				case TEAM_SHOW:
				case TRADE_SHOW:

							printf("\nTEAM_SHOW / TEAM_SHOW / TRADE_SHOW message received\n");
							printf("\n<data>\n");
							
							memcpy(&(msg->data.show_t.ID), aux, sizeof(int));
							printf("\tSHOW_ID = %d \n", msg->data.show_t.ID);
							aux += sizeof(int);

							printf("</data>\n\n");

						break;

				case TRADE:

							printf("\nTRADE message received\n");
							printf("\n<data>\n");
							

							memcpy(&(msg->data.trade_t.tradeID), aux, sizeof(int));
							printf("\tTradeID = %d \n", msg->data.trade_t.tradeID);
							aux += sizeof(int);

							memcpy(&(msg->data.trade_t.teamID), aux, sizeof(int));
							printf("\tTeamID = %d \n", msg->data.trade_t.teamID);
							aux += sizeof(int);

							from_len = 0;
							memcpy(&(from_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.trade_t.from = calloc(from_len, sizeof(char));
							memcpy(msg->data.trade_t.from, aux, from_len);
							printf("\tTRADE FROM = %s \n", msg->data.trade_t.from);
							aux += from_len;

							to_len = 0;
							memcpy(&(to_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.trade_t.to = calloc(to_len, sizeof(char));
							memcpy(msg->data.trade_t.to, aux, to_len);
							printf("\tTRADE TO = %s \n", msg->data.trade_t.to);
							aux += to_len;

							printf("</data>\n\n");

						break;


				case TRADE_WITHDRAW:
				case TRADE_ACCEPT:

							printf("\nTRADE ACCEPT / WITHDRAW message received\n");
							printf("\n<data>\n");
							memcpy(&(msg->data.trade_t.tradeID), aux, sizeof(int));
							printf("\tTRADE_ID = %d \n", msg->data.trade_t.tradeID);
							aux += sizeof(int);
							printf("</data>\n\n");

						break;

				case  TRADE_NEGOTIATE:

							printf("\nTRADE NEGOTIATE message received\n");
							printf("\n<data>\n");
							

							memcpy(&(msg->data.trade_t.tradeID), aux, sizeof(int));
							printf("\tTradeID = %d \n", msg->data.trade_t.tradeID);
							aux += sizeof(int);

							memcpy(&(msg->data.trade_t.teamID), aux, sizeof(int));
							printf("\tTeamID = %d \n", msg->data.trade_t.teamID);
							aux += sizeof(int);

							from_len = 0;
							memcpy(&(from_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.trade_t.from = calloc(from_len, sizeof(char));
							memcpy(msg->data.trade_t.from, aux, from_len);
							printf("\tTRADE FROM = %s \n", msg->data.trade_t.from);
							aux += from_len;

							to_len = 0;
							memcpy(&(to_len), aux, sizeof(int));	
							aux += sizeof(int);

							msg->data.trade_t.to = calloc(to_len, sizeof(char));
							memcpy(msg->data.trade_t.to, aux, to_len);
							printf("\tTRADE TO = %s \n", msg->data.trade_t.to);
							aux += to_len;

							printf("</data>\n\n");

						break;

				case LOGOUT:
							printf("\nLOGOUT message received\n");
						break;
				
				case JOIN_LEAGUE:
				case DRAFT:

						printf("\nJOIN_LEAGUE / DRAFT message received\n");

						printf("\n<data>\n");
						memcpy(&(msg->data.ID), aux, sizeof(int));
						printf("\tLEAGUE_ID = %d \n", msg->data.ID);
						aux += sizeof(int);
						printf("</data>\n\n");
						
						break;
						
				case CREATE_LEAGUE:

						printf("\nCREATE_LEAGUE message received\n");
						
						name_len = 0;
						memcpy(&(name_len), aux, sizeof(int));	
						aux += sizeof(int);

						msg->data.name = calloc(name_len,sizeof(char));
						memcpy(msg->data.name, aux, name_len);
						printf("\tCREATE_LEAGUE_NAME = %s \n", msg->data.name);
						aux += name_len;

				case DRAFT_OUT:

						printf("\nDRAFT_OUT message received\n");

					break;			

				}
				free(bytestring);
				rcvFlag = TRUE;
			}
		}
	}while(!rcvFlag);

	return msg;
}




// int main(void){
	
// 	// First of all be aware of system signals.
// 	// We must exit clean :)

// 	signal(SIGINT,sigint);

// 	upLink();

// 	struct sockaddr_un * client = malloc(sizeof(SOCKET_SIZE));
// 	int client_size = SOCKET_SIZE;

// 	// char client_path[50];
// 	// sprintf(client_path, client->sun_path);
// 	// memcpy(test.data.socket_client_t.socket_path, client_path, sizeof(test.data.socket_client_t.socket_path)-1);
// 	// test.data.socket_client_t.socket_family = client->sun_family;

		
// 	// Channel ch = createChannel(&test);
// 	// establishChannel(ch);
	
// 	int msg = 0;

// 	for ( ; ; ){

// 		msg_t test;

// 		int client_len = strlen(client->sun_path) + 1;
// 		test.data.socket_client_t.socket_path = calloc(client_len, sizeof(char));
// 		test.data.socket_client_t.socket_path = client->sun_path;
// 		test.data.socket_client_t.socket_family = client->sun_family;
		
// 		Channel ch = createChannel(&test);
// 		establishChannel(ch);

// 		IPClisten(ch);


// 		/*
// 			*
// 			* Mensaje de prueba para create channel
// 			*

// 		*/
// 		// msg_t test;




// 		// List l = (List) malloc(sizeof(llist));

// 		// CreateList(l);
// 		// AddToList("Coca-Cola", l);
// 		// AddToList("Naranja", l);
// 		// AddToList("", l);
// 		// AddToList("El anterior era vacio!!!", l);
// 		// Element e;

// 		// msg_s mymen;
// 		// mymen.status = 38;
// 		// mymen.msgList = l;
// 		// printf("Mensaje de prueba creado:\n");
// 		// FOR_EACH(e, mymen.msgList)
// 		// {
// 		// 	printf("\t%s\n", (char *) e->data);
// 		// }



// 		// sendmessage(ch, &mymen);


// 	}

// 	// // -2- Start listening for incoming connections

// 	// // The server will be able to queue up to QUEUE_CONNECTION_SIZE incoming connection requests
// 	// if( listen(sockfd, QUEUE_CONNECTION_SIZE) == -1){
// 	// 	perror("<LOG socket_s.c> Listen call failed <end>");
// 	// 	exit(EXIT_FAILURE);
// 	// }

// 	// int clientPID; // Buffer for storing received data


// 	// // -3- Loop
// 	// for ( ; ; )
// 	// {

// 	// 	/* Accept a connections */
// 	// 	newsockfd = accept(sockfd, NULL, NULL);

// 	// 	if( newsockfd == -1){
// 	// 		perror("<LOG socket_s.c> Accept call failed <end>");
// 	// 		continue;
// 	// 	}else{
// 	// 		printf("<LOG socket_s.c> New connection accepted. Created unique socket file descriptor: newsockfd = %d <end>\n", newsockfd);
// 	// 	}
	
// 	// -4- Spawn a child to deal with the connection
// 	// Here we will have to create a new thread for our child

// 	// if( fork() == 0){
// 	// 	/* Receive data */
// 	// 	while(recv(newsockfd, &clientPID, sizeof(clientPID), 0) > 0){
// 	// 		printf("Server recibe el PID del cliente: %d\n", clientPID);

// 	// 		}
// 	// 	}
// 	// }

// }