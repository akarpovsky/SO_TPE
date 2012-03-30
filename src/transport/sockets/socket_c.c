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

#include "../../includes/defines.h"
#include "../../includes/message.h"


#define SAME_MACHINE_CONNECTION AF_INET
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_in)
#define UNIX_PATH_MAX    108

#define SERVER_PATH "/tmp/socket_server"

int sockfd;
int newsockfd = 0; // Server socket file descriptor
struct sockaddr_in * client_address;
struct sockaddr_in * server_address;	

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

void bindToAssignedSocket(void){
	
	struct sockaddr_in * address = malloc(sizeof(struct sockaddr_in));

	address->sin_family = AF_INET;
	address->sin_port = 7000 + getpid();
	address->sin_addr.s_addr = htonl(INADDR_ANY);
	
		/* Transport endpoint */
	if( (newsockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("<LOG socket_c.c> Socket call failed <end>");
		sigint();
		exit(EXIT_FAILURE);
	}		

	printf("Haciendo bind a nueva dirección ... \n");
	if( (bind( newsockfd, (struct sockaddr *) address, sizeof(struct sockaddr_in))) == -1 ){
		perror("<LOG socket_s.c> Bind call failed <end>");
		exit(EXIT_FAILURE);
	}
	printf("Bindeado a PORT = %d\n", address->sin_port);

	return ;
}

Msg_s rcvmessage(void){

	
	int rcvFlag = FALSE;
	Msg_s msg = calloc(1, sizeof(msg_s));

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
		
		printf("Recibiendo mensaje en el FD = %d ...\n", listenFD);	

		if( (recv(listenFD, &msgSize, sizeof(int), 0)) == -1){
			perror("Error while receiving data");
			return NULL;
		}
		
		if(msgSize > 0){
			// printf("<LOG socket_c.c> Client - Message header received OK. Full message size = %d <end>\n", msgSize);

			aux = bytestring = calloc(msgSize, sizeof(char));
								

			if( (recv(listenFD, aux, msgSize * sizeof(char), 0)) == -1){
				perror("Reading server message failed");
				return NULL;
			}
			else{
				int status;
				memcpy(&(msg->status), aux, sizeof(int));
				aux += sizeof(int);

				// printf("<LOG socket_c.c> Client - Received message status: %d <end>\n", msg->status);


				List l = malloc(sizeof(llist));
				CreateList(l);

				msg->msgList = l;

				int cantElem;
				memcpy(&(cantElem), aux, sizeof(int));
				aux += sizeof(int);

				// printf("<LOG socket_c.c> Client - Starting reception of list containing %d messages <end>\n", cantElem);


				int i;
				int strsize;
				// char * str;

				for(i = 0; i < cantElem; i++){
					memcpy(&(strsize), aux, sizeof(int));
					aux += sizeof(int);

					// printf("<LOG socket_c.c> Client - Receiving message %d of %d - Message length = %d  <end>\n", i+1, cantElem, strsize);

					char * str = calloc(strsize, sizeof(char));

					// memset(str, 0 , strsize);
					memcpy(str, aux, strsize);

					// memcpy(str, aux, strsize);
					aux += strsize;
					printf("<LOG socket_c.c> Client - Message %d: \"%s\" <end>\n", i+1, str);

					AddToList(str, msg->msgList);
					free(str);
				}
				rcvFlag = TRUE;
			}
		}
	}while(!rcvFlag);

	if(msg->status == 115){
		bindToAssignedSocket();
	}

	return msg;
}




int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;
	void * msgstraux;
	int pathSize, from_len, to_len, name_len;

	int pass_len, user_len;

	switch(msg->type){
		case CONTACT:
			msgSize = 2*sizeof(int);


			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);
			
			int client_pid = msg->data.socket_client_t.client_pid;
			memcpy(msgstraux, &client_pid, sizeof(int));
			msgstraux += sizeof(int);

			break;


		case REGISTER:
			
			user_len = strlen(msg->data.register_t.user)+1;
			pass_len = strlen(msg->data.register_t.pass)+1;
			
			msgSize = 3*sizeof(int) + user_len + pass_len;

			// REGISTER message: [MSG_TYPE USER_LEN USER PASS_LEN PASS]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);
			
			memcpy(msgstraux, &user_len, sizeof(int));
			msgstraux += sizeof(int);
			
			memcpy(msgstraux, msg->data.register_t.user, user_len);
			msgstraux += user_len;

			memcpy(msgstraux, &pass_len, sizeof(int));
			msgstraux += sizeof(int);
			
			memcpy(msgstraux, msg->data.register_t.pass, pass_len);
			msgstraux += user_len;

			break;

		case LIST_LEAGUES:
		case LIST_TEAMS:
		case LIST_TRADES:

			msgSize = sizeof(int);

			// LIST message: [MSG_TYPE]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			break;

		case LEAGUE_SHOW:
		case TEAM_SHOW:
		case TRADE_SHOW:
			
			msgSize = 2*sizeof(int);

			// SHOW message: [MSG_TYPE SHOW_ID]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);
			
			memcpy(msgstraux, &(msg->data.show_t.ID), sizeof(int));
			msgstraux += sizeof(int);
			
			break;	

		case TRADE:

			from_len = strlen(msg->data.trade_t.from)+1;
			to_len = strlen(msg->data.trade_t.to)+1;
			msgSize = 5*sizeof(int) + from_len + to_len;
			
			// TRADE message: [MSG_TYPE TRADE_ID TEAM_ID FROM_LEN from TO_LEN to]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(msg->data.trade_t.teamID), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(from_len), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, msg->data.trade_t.from, from_len);
			msgstraux += from_len;

			memcpy(msgstraux, &(to_len), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, msg->data.trade_t.to, to_len);
			msgstraux += to_len;

			break;

		case TRADE_WITHDRAW:
		case TRADE_ACCEPT:

			msgSize = 2*sizeof(int);

			// TRADE WITHDRAW / ACCEPT message: [MSG_TYPE TRADE_ID]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);
			
			memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));
			msgstraux += sizeof(int);

			break;	

		case TRADE_NEGOTIATE:

			from_len = strlen(msg->data.trade_t.from)+1;
			to_len = strlen(msg->data.trade_t.to)+1;
			msgSize = 5*sizeof(int) + from_len + to_len;
			
			// TRADE NEGOTIATE message: [MSG_TYPE TRADE_ID TEAM_ID FROM_LEN from TO_LEN to]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(msg->data.trade_t.tradeID), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(msg->data.trade_t.teamID), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(from_len), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, msg->data.trade_t.from, from_len);
			msgstraux += from_len;

			memcpy(msgstraux, &(to_len), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, msg->data.trade_t.to, to_len);
			msgstraux += to_len;

			break;
		
		case LOGOUT:

			msgSize = sizeof(int);

			// LOGOUT message: [MSG_TYPE]

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			break;

		case JOIN_LEAGUE:
		case DRAFT:

			// JOIN_LEAGUE or DRAFT_LEAGUE message: [MSG_TYPE LEAGUE_ID]		
		
			msgSize = 2*sizeof(int);

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
		
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(msg->data.ID), sizeof(int));
			msgstraux += sizeof(int);
				
			break;
					
		case CREATE_LEAGUE:

			// CREATE_LEAGUE message: [MSG_TYPE NAME_LEN NAME]

			name_len = strlen(msg->data.name)+1;
			msgSize = 2*sizeof(int) + name_len;

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
		
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &(name_len), sizeof(int));
			msgstraux += sizeof(int);
			memcpy(msgstraux, msg->data.name, name_len);
			msgstraux +=  name_len;
				
			break;

		case DRAFT_OUT:

			// DRAFT_OUT message: [MSG_TYPE]

			msgSize = sizeof(int);

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
		
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);

			break;


	}

	if((sendto(sockfd, &msgSize, sizeof(int), 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("Error while trying to send a message to server.");
		printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
		exit(EXIT_FAILURE);
	}

	if((sendto(sockfd, msgstr, msgSize, 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("Error while trying to send a message to server.\n");
		printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
		exit(EXIT_FAILURE);
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

	// if( (bind( sockfd, (struct sockaddr *) client_address, SOCKET_SIZE)) == -1 ){
	// 	perror("<LOG socket_s.c> Bind call failed <end>");
	// 	sigint();
	// 	exit(EXIT_FAILURE);
	// }

	int pid = getpid();

	// Create a connection message

	msg_t com;
	com.type = CONTACT;

	// int path_len = strlen(client_address->sun_path)+1;
	// com.data.socket_client_t.socket_path = calloc(path_len, sizeof(char));
	// memcpy(com.data.socket_client_t.socket_path, client_address->sun_path, path_len-1);
	// com.data.socket_client_t.socket_family = client_address->sun_family;
	com.data.socket_client_t.client_pid = pid;

	// msg_t com2;
	// com2.type = LIST_LEAGUES;


	Msg_s response;
	response = communicate(&com);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}

	return ;

}

// int main(void){

// 	signal(SIGINT,sigint);
// 	signal(SIGPIPE,sigpipe);

// 	connectToServer();

// 	rcvmessage();

// 	// /* Connect the socket to the server's address and then
// 	// 	send and receive information from the server */

	
// 	// if( (connect(sockfd, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
// 	// 	perror("<LOG socket_c.c> Connect call failed <end>");
// 	// 	printf("<LOG socket_c.c> Tried to open the server first? <end>\n");
// 	// 	exit(EXIT_FAILURE);
// 	// }

// 	/* Send and receive information with the server */

// 	// First of all we will send our PID so the server can create an
// 	// exclusive socket for listening me :)

// 	// int clientPID = getpid();
// 	// send(sockfd, &clientPID, sizeof(clientPID), 0);	
	
// 	// int ping = 0;

// 	// if(recv(sockfd, &ping, sizeof(int), 0) > 0)
// 	// 	printf("Recibí: %d\n", ping);
// 	// else
// 	// 	printf("Server has died\n");

// 	// char c, rc;
// 	// for( rc = '\n'; ; ){
// 	// 	if(rc == '\n')
// 	// 		printf("Input a lower case character\n");

// 	// 	c = getchar();

// 	// 	send(sockfd, &c, 1, 0);
// 	// 	if(recv(sockfd, &rc, 1, 0) > 0)
// 	// 		printf("Recibí: %c\n", rc);
// 	// 	else{
// 	// 		printf("Server has died\n");
// 	// 		close(sockfd);
// 	// 		exit(EXIT_FAILURE);
// 	// 	}

// 	// }
// 	// sleep(3);
// 	closeClient(client_address->sun_path);

// }