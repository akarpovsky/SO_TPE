#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>  
#include <unistd.h>
#include <string.h>  
#include <stdlib.h> 
#include <errno.h>
#include "../../includes/message.h"
#include "../../includes/mq_c.h"
#include "../../includes/transport_c.h"

int msgqID;
int pid;

Msg_s communicate(Msg_t msg){
	
	if(sendmessage(msg) == SUCCESSFUL){
		return _rcvmessage();
	}
	
	return NULL;
}


void connectToServer(void){
		
	key_t key;
	
	key = ftok(FTOK_FILE,FTOK_ID);
	if((msgqID = msgget(key,0666)) == -1){
		perror("Error in conect to server");
        exit(1);
	}
	
	pid = getpid();
	
	msg_t com;
	com.type = CONTACT;	
	com.pidFrom = pid;
	Msg_s response;
	response = communicate(&com);
	printf("RESPONSE!\n");

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

int sendmessage(Msg_t msg){
	
	int msgSize;
	void * msgStr = (void * ) serializeMsg(msg);
	memcpy(&msgSize, msgStr, sizeof(int));
	
	msg_Int toSendNum;
	toSendNum.dataInt.fromPid = getpid();
	
	msg_String toSendString;
	toSendString.dataString.fromPid = getpid();
	
	/* Seteo el destino */
	if(msg->type == CONTACT){
		toSendNum.pid = MAIN_SERVER_PRIORITY;	
		toSendString.pid = MAIN_SERVER_PRIORITY;
	}else{
		toSendNum.pid = getpid() + CONVENTION;
		toSendString.pid = getpid() + CONVENTION;
	}

	/* Mando el size del stream */
	toSendNum.dataInt.num = msgSize + sizeof(int);
	if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("msgsnd");
	}
	
	memcpy(toSendString.dataString.string, (char *) msgStr, msgSize + sizeof(int));
	
	if(msgsnd(msgqID, &toSendString, sizeof(int) + sizeof(int) + msgSize, 0) == -1){
		perror("msgsnd 2");
	}
	
	return SUCCESSFUL;
	
}

Msg_s _rcvmessage(void){
	
	msg_String string;
	msg_Int num;
	Msg_s msg;
	int msgSize;
	
	/* Escucho el size del msg */
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), pid, 0)) == -1){
		perror("Error in msgrcv 1");
		exit(EXIT_FAILURE);
	}
	printf("Received msg size = %d\n", num.dataInt.num);
	msgSize = num.dataInt.num; /* TAMANIO DE TODO EL STRAM */	

		
	/* Escucho el stream */
	if((msgrcv(msgqID, &string,  msgSize + sizeof(int), pid, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}


	char * climsg = calloc(msgSize, sizeof(char));
	memcpy(climsg, string.dataString.string, msgSize);
	climsg+=sizeof(int);
		
	msg = deserialize_s(climsg);
	
	return msg;
	
}

Msg_s rcvmessage(void){
	
	msg_String string;
	msg_Int num;
	Msg_s msg;
	int msgSize;
	
	/* Escucho el size del msg */
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), pid, 0)) == -1){
		perror("Error in msgrcv 1");
		exit(EXIT_FAILURE);
	}
	printf("Received msg size = %d\n", num.dataInt.num);
	msgSize = num.dataInt.num; /* TAMANIO DE TODO EL STRAM */	

		
	/* Escucho el stream */
	if((msgrcv(msgqID, &string,  msgSize + sizeof(int), pid, IPC_NOWAIT)) == -1){
		if(errno == ENOMSG){
			return NULL;
		}else{		
			perror("Client: rcvmsg");
			exit(EXIT_FAILURE);
		}
	}


	char * climsg = calloc(msgSize, sizeof(char));
	memcpy(climsg, string.dataString.string, msgSize);
	climsg+=sizeof(int);
		
	msg = deserialize_s(climsg);
	
	return msg;
	
}

void sigint(){
	signal(SIGINT, sigint);
	exit(EXIT_FAILURE);
}