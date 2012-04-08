  
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
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}
	msgSize = num.dataInt.num;	
		
	/* Escucho el stream */
	if((msgrcv(msgqID, &string, sizeof(int) + msgSize, listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}
		
	msg = deserializeMsg(string.dataString.string);
	printf("DESP DEL LISTEN\n" );
	return msg;

}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch,msg);
}

Msg_s establishChannel(Channel ch){
	
	Msg_s serverMsg = createMsg_s();
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
	
	msg_Int num;
	msg_String string;
	int cantStrings = msg->msgList->NumEl;
	int i,sizeString;
	
	num.pid = (ch->pid) - CONVENTION;
	num.dataInt.fromPid = ch->pid;
	
	string.pid = (ch->pid) - CONVENTION;
	string.dataString.fromPid = ch->pid;
	
	Element elem;
	
	/* Envio el responseType */
	num.dataInt.num = msg->responseType;
	if(msgsnd(msgqID,&num,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	
	/* Envio el status */
	num.dataInt.num = msg->status;
	if(msgsnd(msgqID,&num,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	
	/* Envio la cantidad de strings que voy a enviar */
	num.dataInt.num = cantStrings;
	if(msgsnd(msgqID,&num,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0, elem = msg->msgList->pFirst; i < cantStrings; i++, elem=elem->next){
		
		/* Envio la dimension del string (incluido 0) q voy a enviar desp */
		sizeString = strlen((char*)elem->data) + 1;
		num.dataInt.num = sizeString;
		if(msgsnd(msgqID,&num,sizeof(msg_Int) - sizeof(long),0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
		
		/* Envio el string */
		strcpy(string.dataString.string,(char*) elem->data);
		if(msgsnd(msgqID,&string,sizeString + sizeof(int),0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
	}
	
	return SUCCESSFUL;
}

Msg_t rcvmessage(Channel ch){
	
	printf("\nServer listening ...\n\n");

	msg_String string;
	msg_Int num;
	Msg_t msg;
	int msgSize;
	int listenTo;
	
	if(ch == NULL){
		listenTo = MAIN_SERVER_PRIORITY;
	}else{
		listenTo = ch->pid;
	}
		
	/* Escucho el size del msg y el pid del nuevo cliente*/
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), listenTo, IPC_NOWAIT)) == -1){
		if(errno == ENOMSG){
			return NULL;
		}else{		
			perror("Could not communicate to server. In msgrcv");
			exit(EXIT_FAILURE);
		}
	}
	msgSize = num.dataInt.num;	
		
	/* Escucho el stream Y EL PID DEL NUEVO CLIENTE */
	if((msgrcv(msgqID, &string, sizeof(int) + msgSize, listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}
		
	msg = deserializeMsg(string.dataString.string);
	return msg;

}
