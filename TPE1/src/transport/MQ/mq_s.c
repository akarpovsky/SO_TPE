  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>  
#include <errno.h>

#include "../../utils/LinkedList.h"
#include "../../includes/message.h"
#include "../../includes/defines.h"
#include "../../includes/mq_s.h"
#include "../../includes/transport_s.h"

#include "../../includes/message_serialization.h"
#include "../../includes/mq_contact_serialization.h"

int msgqID;

void uplink(void){
	
	key_t key;
	
	key = ftok(FTOK_FILE,FTOK_ID);
	if((msgqID = msgget(key,0666 | IPC_CREAT)) == -1){
		perror("Error in conect to server");
        exit(1);
	}	
}

void sigint(){  
	signal(SIGINT,sigint); /* reset signal */
	printf("<LOG mq_s.c> Server have received a SIGINT. Close connections, free memory, save data and go away! <end> \n");
	msgctl(msgqID, IPC_RMID, NULL);
	exit(EXIT_FAILURE);
}

Msg_t IPClisten(Channel ch){
	
	printf("\nServer listening ...\n\n");

	msg_String string;
	msg_Int num;
	Msg_t msg;
	int msgSize;
	long listenTo;
	if(ch == NULL){
		listenTo = MAIN_SERVER_PRIORITY;
	}else{
		listenTo = ch->pid;
	}
		
	/* Escucho el size del msg */
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), listenTo, 0)) == -1){
		perror("Error in msgrcv 1");
		exit(EXIT_FAILURE);
	}
	printf("Received msg size = %d\n", num.dataInt.num);
	msgSize = num.dataInt.num; /* TAMANIO DE TODO EL STRAM */	

		
	/* Escucho el stream */
	if((msgrcv(msgqID, &string,  msgSize + sizeof(int), listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}


	char * climsg = calloc(msgSize, sizeof(char));
	memcpy(climsg, string.dataString.string, msgSize);
	climsg+=sizeof(int);
		
	msg = deserializeMsg(climsg);
	printf("DESP DEL LISTEN\n" );
	return msg;

}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch,msg);
}

Msg_s establishChannel(Channel ch){
	
	Msg_s serverMsg = (Msg_s) createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
		printf("SALI DEL ESTABLISHCH\n");
	return serverMsg;
}
	
Channel createChannel(Msg_t msg){
	
	Channel ch = (Channel) malloc(sizeof(channel_t));
	if(ch == NULL){
		perror("Insufficient memory to create the channel\n");
	}
	
	ch->pid = (msg->pidFrom) + CONVENTION;
	printf("SALI DEL CREATE\n");
	return ch;
	
}

int sendmessage(Channel ch, Msg_s msg){
	
	int msgSize;
	msg_Int num;
	msg_String string;
	void * msgStr = (void *) serialize_s(msg);
	memcpy(&msgSize, msgStr, sizeof(int));
	
	num.pid = (ch->pid) - CONVENTION;
	num.dataInt.fromPid = ch->pid;
	
	string.pid = (ch->pid) - CONVENTION;
	string.dataString.fromPid = ch->pid;
	
	/* Mando el size del stream */
	num.dataInt.num = msgSize + sizeof(int);
	if(msgsnd(msgqID,&num,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("In server: msgsnd");
	}
	
	memcpy(string.dataString.string, (char *) msgStr, msgSize + sizeof(int));
	
	if(msgsnd(msgqID, &string, sizeof(int) + sizeof(int) + msgSize, 0) == -1){
		perror("In server: msgsnd 2");
	}
	
	return SUCCESSFUL;
	
	
}


Msg_t rcvmessage(Channel ch){
	
	msg_String string;
	msg_Int num;
	Msg_t msg;
	int msgSize;
	long listenTo;
	if(ch == NULL){
		listenTo = MAIN_SERVER_PRIORITY;
	}else{
		listenTo = ch->pid;
	}
		
	/* Escucho el size del msg */
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), listenTo, IPC_NOWAIT)) == -1){
		if(errno == ENOMSG){
			return NULL;
		}else{		
			perror("rcv from server.");
			exit(EXIT_FAILURE);
		}
	}
	printf("Received msg size = %d\n", num.dataInt.num);
	msgSize = num.dataInt.num; /* TAMANIO DE TODO EL STRAM */	

		
	/* Escucho el stream */
	if((msgrcv(msgqID, &string,  msgSize + sizeof(int), listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}


	char * climsg = calloc(msgSize, sizeof(char));
	memcpy(climsg, string.dataString.string, msgSize);
	climsg+=sizeof(int);
		
	msg = deserializeMsg(climsg);
	printf("DESP DEL LISTEN\n" );
	return msg;
}
