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
#include "../../utils/hashmap.h"
#include "../../includes/defines.h"
#include "../../includes/socket_s.h"
#include "../../includes/transport_s.h"


int sockfd; // Server socket file descriptor
void * sockets_hmap;
int openedSockets = 1;


 struct sockaddr_in * getClientChannel(int pid){

		struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));

		address->sin_family = AF_INET;
		address->sin_port = 7000 + pid;
		address->sin_addr.s_addr = htonl(INADDR_ANY);
		
		return address;
}

 struct sockaddr_in * getServerAddress(){

		struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));

		address->sin_family = AF_INET;
		address->sin_port = 7000;
		address->sin_addr.s_addr = htonl(INADDR_ANY);

		return address;
}

void closeServer(char * server_path){
	// This function removes the created socket file in /tmp/
	// unlink(server_path);
	close(sockfd);

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
	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
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

	struct sockaddr_in * addr_server = getServerAddress();
	
    // -1- "Bind" the server  address to the end point
	if( (bind( sockfd, (struct sockaddr *) addr_server, SOCKET_SIZE)) == -1 ){
		perror("<LOG socket_s.c> Bind call failed in upLink() <end>");
		exit(EXIT_FAILURE);
	}

	sockets_hmap = hashmap_new(10); // Initialize hashmap with space for 10 clients
}

Channel createChannel(Msg_t msg)
{
	int newsockfd;

	if( (newsockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("<LOG socket_s.c> Socket call failed <end>");
		exit(EXIT_FAILURE);
	}

	Channel ch = malloc(sizeof(channel_t));
	ch->client = msg->data.socket_client_t.client;
	ch->port = ch->client->sin_port;
	ch->client->sin_port += openedSockets++;

	if( (bind( newsockfd, (struct sockaddr *) ch->client, SOCKET_SIZE)) == -1 ){
		perror("<LOG socket_s.c> Bind call failed in createChannel() <end>");
		exit(EXIT_FAILURE);
	}
	

	// printf("Creado un channel para el nuevo cliente\n");
	// printf("Guardo el FD %d en el hashmap\n", newsockfd);
	hashmap_insert(sockets_hmap, (void *) newsockfd , ch->port);
	return ch;
}

Msg_s establishChannel(Channel ch)
{
	Msg_s serverMsg = createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	serverMsg->status = ch->client->sin_port;
	
	return serverMsg;
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

	struct sockaddr_in * client = ch->client;
	///////
	client->sin_port = ch->port;
	//
	int cli_size = SOCKET_SIZE;

	int listenFD = 0;

	listenFD = (int) hashmap_get(sockets_hmap , ch->port);
	
	if(listenFD == 0)
		listenFD = sockfd;
		

	// sendto(listenFD, &c, 1,0, msg->data.socket_client_t.client, SOCKET_SIZE);
	if((sendto(listenFD, &msgSize, sizeof(int), 0, (struct sockaddr *) client, cli_size) == -1)){
		perror("<LOG socket_s.c> Server: Could not write message size <end>");
		return !SUCCESSFUL;
	}else{
		printf("<LOG socket_s.c> Server: Envio el message size = %d <end>\n", msgSize);
	}

	if((sendto(listenFD, msgstr, msgSize, MSG_WAITALL, (struct sockaddr *) client, cli_size) == -1)){
		perror("<LOG socket_s.c> Server: Could not write message <end>");
		return !SUCCESSFUL;
	}else{
		printf("<LOG socket_s.c> Server: Envio la lista de mensajes al cliente <end>\n");
	}

	free(msgstr);
	return SUCCESSFUL;
}

Msg_t IPClisten(Channel ch){


	if(ch == NULL){
		printf("\nServer listening on port 7000 ...\n\n");
	}else{
		printf("Client server listening on port %d ... \n", ch->port);
	}

	int rcvFlag = FALSE;
	Msg_t msg = (Msg_t) calloc(1, sizeof(msg_t));

	do{
		int msgSize, user_len, pass_len, from_len, to_len, name_len;
		int client_len = SOCKET_SIZE;
		void * bytestring;
		void * aux;
		struct sockaddr_in * client = calloc(1, SOCKET_SIZE);

		int listenFD;
		if(ch == NULL){
			listenFD = sockfd;
		}else{
			listenFD = (int) hashmap_get( sockets_hmap , ch->port);
		}

		if( (recvfrom(listenFD, &msgSize, sizeof(int), MSG_WAITALL, (struct sockaddr *) client, (socklen_t *) &client_len)) == -1){
			perror("Error while receiving data");
			continue ;
		}

		if(msgSize > 0){
			
			aux = bytestring = malloc(msgSize);

			// printf("<LOG socket_s.c> Server - Message header received OK. Full message size = %d <end>\n", msgSize);

			if( (recvfrom(listenFD, aux, msgSize * sizeof(char), MSG_WAITALL, (struct sockaddr *) client, (socklen_t *) &client_len)) == -1){
				perror("Reading client message failed");
				return NULL;
			}else{
				
				memcpy(&(msg->type), aux, sizeof(int));
				aux += sizeof(int);

				printf("<LOG socket_s.c> Server - Received message type: %d <end>\n", msg->type);

				int type = msg->type;
				switch(type){
					
					case CONTACT:
						
						printf("\nCONTACT message received\n");
						printf("\n<data>\n");
						memcpy(&(msg->data.socket_client_t.client_pid), aux, sizeof(int));
						aux += sizeof(int);
						printf("\tCLIENT_PID = %d \n", msg->data.socket_client_t.client_pid);
						printf("</data>\n\n");
						msg->data.socket_client_t.client = client;
						char c = 'a';
						printf("Tengo22 client: port - %d\n", client->sin_port);

						// sendto(listenFD, &c, 1,0, msg->data.socket_client_t.client, SOCKET_SIZE);
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

						msg->data.login_t.user = calloc(user_len,sizeof(char));
						memcpy(msg->data.login_t.user, aux, user_len);
						printf("\tUsername = %s \n", msg->data.login_t.user);
						aux += user_len;

						pass_len = 0;
						memcpy(&(pass_len), aux, sizeof(int));	
						aux += sizeof(int);

						msg->data.login_t.pass = calloc(pass_len,sizeof(char));
						memcpy(msg->data.login_t.pass, aux, pass_len);
						printf("\tPassword = %s \n", msg->data.login_t.pass);
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

					break;

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