/*
* 
* Shared memory server
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>

#include "../../includes/message.h"
#include "../../utils/LinkedList.h"
#include "../../utils/hashmap.h"
#include "../../includes/defines.h"
#include "../../includes/transport_s.h"


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

}

Channel createChannel(Msg_t msg)
{
	Channel ch = malloc(sizeof(channel_t));

	return ch;
}

Msg_s establishChannel(Channel ch)
{
	Msg_s serverMsg = createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	// serverMsg->status = ch->client->sin_port;
	
	return serverMsg;
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch, msg);
}

int sendmessage(Channel ch, Msg_s msg){

}

Msg_t IPClisten(Channel ch){


}