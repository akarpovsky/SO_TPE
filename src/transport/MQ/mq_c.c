#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include "../../includes/message.h"
#include "../../includes/mq_c.h"

int msgqID;
int pid;

typedef struct msg_String{
    long  pid;  
	struct dataString{
		int fromPid;
		char string[MAX_STRING];
		}dataString;
}msg_String;

typedef struct msg_Int{
	long pid;
	struct dataInt{
		int num;
		int fromPid;
	}dataInt;
}msg_Int;

Msg_s communicate(Msg_t msg){
	
	if(sendmessage(msg) == SUCCESSFUL){
		//return rcvmessage();
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
	communicate(&com);
}

int sendmessage(Msg_t msg){
	
	int size;
	msg_Int toSendNum;
	toSendNum.dataInt.fromPid = getpid();
	
	msg_String toSendString;
	toSendString.dataString.fromPid = getpid();
	toSendString.pid = getpid() + CONVENTION;
	
	/* Seteo el destino */
	if(msg->type == CONTACT){
		toSendNum.pid = MAIN_SERVER_PRIORITY;	
	}else{
		toSendNum.pid = getpid() + CONVENTION;
	}
	
	/* Mando el comando */
	toSendNum.dataInt.num = msg->type;
	if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
		perror("msgsnd");
	}

	switch(msg->type){
		case REGISTER:	
						/* Mando size de user */
						size = strlen(msg->data.register_t.user) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando user */
						strcpy(toSendString.dataString.string,msg->data.register_t.user);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						
						/* Mando size de pass */
						size = strlen(msg->data.register_t.pass) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando pass */
						strcpy(toSendString.dataString.string,msg->data.register_t.pass);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						break;
						
		case LOGIN:
						/* Mando size de user */
						size = strlen(msg->data.login_t.user) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando user */
						strcpy(toSendString.dataString.string,msg->data.login_t.user);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						
						/* Mando size de pass */
						size = strlen(msg->data.login_t.pass) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando pass */
						strcpy(toSendString.dataString.string,msg->data.login_t.pass);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						break;
		
		case LEAGUE_SHOW:
		case TEAM_SHOW:
		case TRADE_SHOW:
						/* ENVIO ID */
						toSendNum.dataInt.num = msg->data.show_t.ID;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						break;
						
		case TRADE:
						/* Mando size de from */
						size = strlen(msg->data.trade_t.from) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando from */
						strcpy(toSendString.dataString.string,msg->data.trade_t.from);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						
						/* Mando size de to */
						size = strlen(msg->data.trade_t.to) + 1;
						toSendNum.dataInt.num = size;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						
						/* Mando to */
						strcpy(toSendString.dataString.string,msg->data.trade_t.to);
						if(msgsnd(msgqID,&toSendString,sizeof(int) + size ,0) == -1){
							perror("msgsnd");
						}
						
						/* Mando TeamID */
						toSendNum.dataInt.num = msg->data.trade_t.teamID;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						break;

		case TRADE_WITHDRAW:
		case TRADE_ACCEPT:
		case TRADE_NEGOTIATE:
						
						/* Mando TradeID */
						toSendNum.dataInt.num = msg->data.trade_t.tradeID;
						if(msgsnd(msgqID,&toSendNum,sizeof(msg_Int) - sizeof(long),0) == -1){
							perror("msgsnd");
						}
						break;
						
	}	
	
	return SUCCESSFUL;
	
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

// int main(void){
	
// 	connectToServer();
// 	printf("YA ME CONECTE\n");
	
// 	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
// 	if(msg == NULL){
// 		perror("Insufficient memory\n");
// 		exit(EXIT_FAILURE);		
// 	}
	
// 	msg->type = REGISTER;
// 	msg->data.register_t.user = "vicky";
// 	msg->data.register_t.pass = "PASS123";
// 	communicate(msg);
	
// 	msg->type = LOGIN;
// 	msg->data.login_t.user = "vicky";
// 	msg->data.login_t.pass = "PASS123";
// 	communicate(msg);
		
// 	msg->type = TRADE_SHOW;
// 	msg->data.show_t.ID = atoi("12");
// 	communicate(msg);
	
// 	msg->type = TRADE;
// 	msg->data.trade_t.from = "soyFrom";
// 	msg->data.trade_t.to = "soyFrom";
// 	msg->data.trade_t.teamID = atoi("1234");
// 	communicate(msg);
		
// 	msg->type = TRADE_ACCEPT;
// 	msg->data.trade_t.tradeID = atoi("1234");
// 	communicate(msg);
	
// 	Msg_s resp = rcvmessage();

// 	printf("\n");
// 	printf("Status %d\n",resp->status);
	
// 	Element elem;
	
// 	FOR_EACH(elem, resp->msgList){
// 		printf("cadena: %s\n",(char*)(elem->data));
// 	}
	
// 	return 0;
// }
