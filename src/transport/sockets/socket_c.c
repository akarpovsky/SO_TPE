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
struct sockaddr_un * server_address;	

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

void sigpipe(){  
	signal(SIGINT,sigpipe); /* reset signal */
	printf("<LOG socket_s.c> Client have received a SIGPIPE. <end> \n");
	printf("<LOG socket_s.c> Cannot talk to the server. Server may have been initialized wrong or it has crashed. <end> \n");
	exit(EXIT_FAILURE);
}


Msg_s rcvmessage(void){

	int rcvFlag = FALSE;
	Msg_s msg = calloc(1, sizeof(msg_s));

	do{
		int msgSize;
		int client_len = SOCKET_SIZE;
		void * bytestring;
		void * aux;
		

		if( (recvfrom(sockfd, &msgSize, sizeof(int), MSG_WAITALL, NULL, NULL)) == -1){
			perror("Error while receiving data");
			return NULL;
		}

		
		if(msgSize > 0){
			printf("<LOG socket_c.c> Client - Message header received OK. Full message size = %d <end>\n", msgSize);


			aux = bytestring = calloc(msgSize, sizeof(char));
								

			if( (recvfrom(sockfd, aux, msgSize * sizeof(char), MSG_WAITALL, NULL, NULL)) == -1){
				perror("Reading server message failed");
				return NULL;
			}
			else{
				int status;
				memcpy(&(msg->status), aux, sizeof(int));
				aux += sizeof(int);

				printf("<LOG socket_c.c> Client - Received message status: %d <end>\n", msg->status);


				List l = malloc(sizeof(llist));
				CreateList(l);

				msg->msgList = l;

				int cantElem;
				memcpy(&(cantElem), aux, sizeof(int));
				aux += sizeof(int);

				printf("<LOG socket_c.c> Client - Starting reception of list containing %d messages <end>\n", cantElem);


				int i;
				int strsize;
				// char * str;

				for(i = 0; i < cantElem; i++){
					memcpy(&(strsize), aux, sizeof(int));
					aux += sizeof(int);

					printf("<LOG socket_c.c> Client - Receiving message %d of %d - Message length = %d  <end>\n", i+1, cantElem, strsize);

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

	return msg;
}


int sendmessage(Msg_t msg)
{
	int msgSize;
	void * msgstr;
	void * msgstraux;
	int pathSize, from_len, to_len;

	int pass_len, user_len;

	switch(msg->type)	{
		case CONTACT:
			pathSize = strlen(msg->data.socket_client_t.socket_path)+1;
			msgSize = 4*sizeof(int) + pathSize;

			// Contact message: [MSG_TYPE CLIENT_PID SOCKET_FAMILY PATH_LEN PATH]
			// MSG_TYPE (int) --> CLIENT_PID (int) --> 
			// --> SOCKET_FAMILY (int) --> PATH_LEN (int) --> PATH (char[N])

			msgstraux = msgstr = calloc(msgSize, sizeof(char));
			memcpy(msgstraux, &(msg->type), sizeof(int));
			msgstraux += sizeof(int);
			
			int client_pid = msg->data.socket_client_t.client_pid;
			memcpy(msgstraux, &client_pid, sizeof(int));
			msgstraux += sizeof(int);

			int socket_family = msg->data.socket_client_t.socket_family;
			memcpy(msgstraux, &socket_family, sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, &pathSize, sizeof(int));
			msgstraux += sizeof(int);

			memcpy(msgstraux, msg->data.socket_client_t.socket_path, pathSize);
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

	}

	if((sendto(sockfd, &msgSize, sizeof(int), 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("Error while trying to send a message to server.");
		printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
		closeClient(client_address->sun_path);
		exit(EXIT_FAILURE);
	}

	if((sendto(sockfd, msgstr, msgSize, 0, (struct sockaddr *) server_address, SOCKET_SIZE)) == -1){
		perror("Error while trying to send a message to server.\n");
		printf("Server may have been initialized wrong or it has crashed. Start the server first and then restart the client.\n");
		closeClient(client_address->sun_path);
		exit(EXIT_FAILURE);
	}

	free(msgstr);

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

	// Create a connection message

	msg_t com;
	com.type = CONTACT;

	int path_len = strlen(client_address->sun_path)+1;
	com.data.socket_client_t.socket_path = calloc(path_len, sizeof(char));
	memcpy(com.data.socket_client_t.socket_path, client_address->sun_path, path_len-1);
	com.data.socket_client_t.socket_family = client_address->sun_family;
	com.data.socket_client_t.client_pid = pid;

	// msg_t com2;
	// com2.type = REGISTER;
	// com2.data.register_t.user = "A";
	// com2.data.register_t.pass = "B";

	// msg_t com3;
	// com3.type = TEAM_SHOW;
	// com3.data.show_t.ID = 23;

	// msg_t com4;
	// com4.type = TRADE_NEGOTIATE;
	// com4.data.trade_t.from = "Palermo";
	// com4.data.trade_t.to = "Messi";
	// com4.data.trade_t.teamID = 15;
	// com4.data.trade_t.tradeID = 1;

	// msg_t com4;
	// com4.type = TRADE_NEGOTIATE;
	// sprintf(com4.data.trade_t.from, "A");
	// sprintf(com4.data.trade_t.to, "B");
	// com4.data.trade_t.teamID = 15;
	// com4.data.trade_t.tradeID = 1;

	// msg_t com5;
	// com5.type = LOGOUT;

	// communicate(&com);
	// communicate(&com2);
	// communicate(&com3);
	// communicate(&com4);
	communicate(&com);

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