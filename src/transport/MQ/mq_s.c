  
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
	int listenTo;
	
	if(ch == NULL){
		listenTo = MAIN_SERVER_PRIORITY;
	}else{
		listenTo = ch->pid;
	}
		
	/* Escucho el size del msg y el pid del nuevo cliente*/
	if((msgrcv(msgqID, &num, sizeof(msg_Int) - sizeof(long), listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}
	msgSize = num.dataInt.num;	
		
	/* Escucho el stream Y EL PID DEL NUEVO CLIENTE */
	if((msgrcv(msgqID, &string, sizeof(int) + msgSize, listenTo, 0)) == -1){
		perror("Error in msgrcv");
		exit(EXIT_FAILURE);
	}
		
	msg = deserializeMsg(string.dataString.string);
	free(string.dataString.string);
	return msg;

}

int communicate(Channel ch, Msg_s msg){
	return sendmessage(ch,msg);
}

Msg_s establishChannel(Channel ch){
	
	Msg_s serverMsg = createMsg_s();
	AddToList("Connection established.", serverMsg->msgList);
	return serverMsg;
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
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	int sizeString;
	
	/* Escucho como server */
	if(ch == NULL){
		/* Escucho el comando CONTACT Y EL PID DEL NUEVO CLIENTE */
		if((msgrcv(msgqID, &num, sizeof(msg_Int)- sizeof(long),MAIN_SERVER_PRIORITY,IPC_NOWAIT)) == -1)
		{
			if(errno == ENOMSG){
				return NULL;
			}else{		
				perror("Error in msgrcv");
				exit(EXIT_FAILURE);
			}
		}
		
		if(num.dataInt.num == CONTACT){
			msg->type = num.dataInt.num;
			msg->pidFrom = num.dataInt.pidFrom;
			printf("\nCONTACT message received\n");
			printf("\n<data>\n");
			printf("\tCLIENT_PID = %d\n", msg->pidFrom);
			printf("</data>\n\n");
			return msg;
		}else{
			return NULL;
		}
	}else{
		/* Escucho como thread */
		
		/* Escucho el comando */
		if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,IPC_NOWAIT) == -1){
			if(errno == ENOMSG){
				return NULL;
			}else{		
				perror("Error in msgrcv");
				exit(EXIT_FAILURE);
			}
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
			
						/* Recibo ID */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						msg->data.trade_t.tradeID = num.dataInt.num;
						
						break;
			
			case TRADE_NEGOTIATE:
				
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
						msg->data.trade_t.tradeID = num.dataInt.num;
						
						break;
						
			case JOIN_LEAGUE:
			case DRAFT:
						/* Recibo ID */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						msg->data.ID = num.dataInt.num;
						
						break;
						
			case CREATE_LEAGUE:
			case CHOOSE:
						
						/* Escucho size de name */
						if(msgrcv(msgqID,&num, sizeof(msg_Int) - sizeof(long) ,ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						sizeString = num.dataInt.num;
						msg->data.name = (char *) malloc(sizeString);
						
						/* Recibo to */
						if(msgrcv(msgqID,&string, sizeof(int) + sizeString, ch->pid ,0) == -1){
								perror("Error in msgrcv");
								exit(EXIT_FAILURE);
						}
						strcpy(msg->data.name,string.dataString.string);
						
							
		}
		
		return msg;
	}
	
	return NULL;
	
}