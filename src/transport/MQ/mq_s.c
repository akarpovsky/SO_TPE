  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>  
#include <errno.h>
#include "../../includes/message.h"
#include "../../includes/defines.h"

#define FTOK_ID 1
#define FTOK_FILE "/tmp/msgq"
#define MAIN_SERVER_PRIORITY 1
#define MAX_STRING 200

int msgqID;

typedef struct msg_String{
    long  pid;  
	struct dataString{
		int pidFrom;
		char string[MAX_STRING];
		}dataString;
}msg_String;

typedef struct msg_Int{
	long pid;
	struct dataInt{
		int num;
		int pidFrom;
	}dataInt;
}msg_Int;


typedef struct channel_t {
	long pid; /* LO QUE EL THREAD ESCUCHA */
} channel_t;

typedef channel_t * Channel;



void uplink(void){
	
	key_t key;
	
	key = ftok(FTOK_FILE,FTOK_ID);
	if((msgqID = msgget(key,0666 | IPC_CREAT)) == -1){
		perror("Error in conect to server");
        exit(1);
	}	
}

Msg_t listen(Channel ch){
	
	msg_String string;
	msg_Int num;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	int sizeString, cantStrings;
	
	/* Escucho como server */
	if(ch == NULL){
		/* Escucho el comando CONTACT Y EL PID DEL NUEVO CLIENTE */
		msgrcv(msgqID,&num, sizeof(msg_Int)- sizeof(long),MAIN_SERVER_PRIORITY,0);
		if(num.dataInt.num == CONTACT){
			msg->type = num.dataInt.num;
			msg->pidFrom = num.dataInt.pidFrom;
			return msg;
		}else
			return NULL;
	}else{
		/* Escucho como thread */
		
		/* Escucho el comando */
		if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
				perror("Error in msgrcv");
				exit(EXIT_FAILURE);
		}
		msg->type = num.dataInt.num;
		msg->pidFrom = num.dataInt.pidFrom;
		
		switch(msg->type){
			
			case REGISTER:
						/* Escucho size de user */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.register_t.user = (char *) malloc(sizeString);
						
						/* Recibo user */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.register_t.user, string.dataString.string);
						
						/* Escucho size de pass */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.register_t.pass = (char *) malloc(sizeString);
						
						/* Recibo pass */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.register_t.pass, string.dataString.string);
						
						break;
			
			case LOGIN:
						/* Escucho size de user */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.login_t.user = (char *) malloc(sizeString);
						
						/* Recibo user */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.login_t.user,string.dataString.string);
						
						/* Escucho size de pass */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.login_t.pass = (char *) malloc(sizeString);
						
						/* Recibo pass */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.login_t.pass, string.dataString.string);
						
						break;
						
			case LEAGUE_SHOW:
			case TEAM_SHOW:
			case TRADE_SHOW:
				
						/* Recibo ID */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						msg->data.show_t.ID = num.dataInt.num;
						
						break;
			
			case TRADE:
						/* Escucho size de from */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.trade_t.from = (char *) malloc(sizeString);
						
						/* Recibo from */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.trade_t.from,string.dataString.string);
						
						/* Escucho size de to */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.trade_t.to = (char *) malloc(sizeString);
						
						/* Recibo to */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.trade_t.to,string.dataString.string);
						
						/* Recibo ID */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						msg->data.trade_t.teamID = num.dataInt.num;
						
						break;
						
			case TRADE_WITHDRAW:
			case TRADE_ACCEPT:
			case TRADE_NEGOTIATE:
			
						/* Recibo ID */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						msg->data.trade_t.tradeID = num.dataInt.num;
						
						break;		
		}
		
		return msg;
	}
	
	return NULL;
	
}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch,msg);
}

int establishChannel(Channel ch){
	
	return SUCCESSFUL;
}

Channel createChannel(Msg_t msg){
	
	Channel ch = (Channel) malloc(sizeof(channel_t));
	if(ch == NULL){
		perror("Insufficient memory to create the channel\n");
	}
	
	ch->pid = (msg->pidFrom) + CONVENTION;
	
	return ch;
	
}

int sendmessage(Channel ch, Msg_s msg){
	
	msg_Int num;
	msg_String string;
	int cantStrings = msg->msgList->NumEl;
	int i,sizeString;
	
	num.pid = (ch->pid) - CONVENTION;
	num.dataInt.pidFrom = ch->pid;
	
	string.pid = (ch->pid) - CONVENTION;
	string.dataString.pidFrom = ch->pid;
	
	Element elem;
	
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

	
int main(void){
	
	uplink();
	
	
	int i;

	Msg_t mess = listen(NULL);
	printf("MENSAJE DE CONTACTO de %d\n", mess->pidFrom);
	
	Channel ch = malloc(sizeof(channel_t));
	ch->pid = mess->pidFrom + CONVENTION;
	
	mess = listen(ch);
	printf("\n");
	printf("MENSAJE REGISTER de user: %s\n", mess->data.register_t.user);
	printf("MENSAJE REGISTER de pass: %s\n", mess->data.register_t.pass);
	
	mess = listen(ch);
	printf("\n");
	printf("MENSAJE LOGGIN de user: %s\n", mess->data.login_t.user);
	printf("MENSAJE LOGGIN de pass: %s\n", mess->data.login_t.pass);
	
	mess = listen(ch);
	printf("\n");
	printf("MENSAJE TRADE_SHOW de id: %d\n", mess->data.show_t.ID);
	
	mess = listen(ch);
	printf("\n");
	printf("MENSAJE TRADE from: %s\n", mess->data.trade_t.from);
	printf("MENSAJE TRADE de to: %s\n", mess->data.trade_t.to);
	printf("MENSAJE TRADE de ID %d\n", mess->data.trade_t.teamID);
	
	mess = listen(ch);
	printf("\n");
	printf("MENSAJE TRADE_ACCEPT de id: %d\n", mess->data.trade_t.tradeID);

	Msg_s resp = malloc(sizeof(msg_t));
	resp->status = 7;
	resp->msgList = (List)malloc(sizeof(llist));
	CreateList(resp->msgList);
	AddToList("UNO",resp->msgList);
	AddToList("DOS",resp->msgList);
	AddToList("ULTIMA FRASE - FIN",resp->msgList);
	
	communicate(ch,resp);
	return 0;
	
	
}	
	
	