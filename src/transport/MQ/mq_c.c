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
	toSendNum.dataInt.num = msgSize;
	if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("msgsnd");
	}
	
	memcpy(toSendString.dataString.string, (char *) msgStr, msgSize + sizeof(int));
	
	if(msgsnd(msgqID, &toSendString, sizeof(msg_String) - sizeof(long),0) == -1){
		perror("msgsnd 2");
	}
	
	return SUCCESSFUL;
	
}

Msg_s _rcvmessage(void){
	
	msg_String string;
	msg_Int num;
	int cantStrings, sizeString,i;
	
	/* Pido memoria para el retorno e inicializo todo*/
	Msg_s response = (Msg_s) malloc(sizeof(msg_s));
	if(response == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}
	response->msgList = (List) malloc(sizeof(llist));
	if(response->msgList == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}
	CreateList(response->msgList);
	
	
	/* Recibo el responseType y lo pongo en response */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	response->responseType = num.dataInt.num;
	
	/* Recibo el status y lo pongo en response */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	response->status = num.dataInt.num;
	
	/* Recibo la cantidad de strings que me va a enviar */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	cantStrings = num.dataInt.num;
	
	for(i = 0; i < cantStrings; i++){
		
		/* Recibo la dimension del prox string (incluido el 0) */
		if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
		sizeString = num.dataInt.num;
		
		/* Recibo string y lo pongo en response */
		if(msgrcv(msgqID,&string,sizeString + sizeof(int),pid,0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
		char *s = (char *) malloc(sizeString);
		strcpy(s,string.dataString.string);
		AddToList(s,response->msgList);
	}
	
	return response;
	
}

Msg_s rcvmessage(void){
	
	msg_String string;
	msg_Int num;
	int cantStrings, sizeString,i;
	
	/* Pido memoria para el retorno e inicializo todo*/
	Msg_s response = (Msg_s) malloc(sizeof(msg_s));
	if(response == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}
	response->msgList = (List) malloc(sizeof(llist));
	if(response->msgList == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}
	CreateList(response->msgList);
	
	
	/* Recibo el responseType y lo pongo en response */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,IPC_NOWAIT) == -1){
		if(errno == ENOMSG){
			return NULL;
		}else{		
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
	}
	response->responseType = num.dataInt.num;
	
	/* Recibo el status y lo pongo en response */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	response->status = num.dataInt.num;
	
	/* Recibo la cantidad de strings que me va a enviar */
	if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
		perror("Could not communicate to server. In msgsnd");
		exit(EXIT_FAILURE);
	}
	cantStrings = num.dataInt.num;
	
	for(i = 0; i < cantStrings; i++){
		
		/* Recibo la dimension del prox string (incluido el 0) */
		if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long),pid,0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
		sizeString = num.dataInt.num;
		
		/* Recibo string y lo pongo en response */
		if(msgrcv(msgqID,&string,sizeString + sizeof(int),pid,0) == -1){
			perror("Could not communicate to server. In msgsnd");
			exit(EXIT_FAILURE);
		}
		char *s = (char *) malloc(sizeString);
		strcpy(s,string.dataString.string);
		AddToList(s,response->msgList);
	}
	
	return response;
	
}

void sigint(){
	signal(SIGINT, sigint);
	exit(EXIT_FAILURE);
}