#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../includes/marshalling.h"
#include "../includes/message.h"

void register_c(char * user, char * pass){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = REGISTER;
	
	msg->data.register_t.user = (char*) malloc(strlen(user)+1);
	if(msg->data.register_t.user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	msg->data.register_t.pass = (char*) malloc(strlen(pass)+1);
	if(msg->data.register_t.pass == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(msg->data.register_t.user,user);
	strcpy(msg->data.register_t.pass,pass);

	response = communicate(&msg);
	
	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}

}

void login_c(char * user, char * pass){
	
	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = LOGIN;
	
	msg->data.login_t.user = (char*) malloc(strlen(user)+1);
	if(msg->data.login_t.user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	msg->data.login_t.pass = (char*) malloc(strlen(pass)+1);
	if(msg->data.login_t.pass == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(msg->data.login_t.user,user);
	strcpy(msg->data.login_t.pass,pass);

	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}

}

void list_c(int toList){
	
	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = toList;
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void leagueShow_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = LEAGUE_SHOW;
	msg->data.show_t.ID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void teamShow_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}

	msg->type = TEAM_SHOW;
	msg->data.show_t.ID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void tradeShow_c(char * id){
	
	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = TRADE_SHOW;
	msg->data.show_t.ID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void trade_c(char * id, char * from, char * to){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = TRADE;
	msg->data.trade_t.teamID = atoi(id);

	msg->data.trade_t.from = (char*) malloc(strlen(from)+1);
	if(msg->data.trade_t.from == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	msg->data.trade_t.to = (char*) malloc(strlen(to)+1);
	if(msg->data.trade_t.to == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(msg->data.trade_t.from,from);
	strcpy(msg->data.trade_t.to,to);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void tradeWithdraw_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = TRADE_WITHDRAW;
	msg->data.trade_t.tradeID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void tradeAccept_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = TRADE_ACCEPT;
	msg->data.trade_t.tradeID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void tradeNegotiate_c(char * id, char * from, char * to){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = TRADE_NEGOTIATE;
	msg->data.trade_t.tradeID = atoi(id);
	
	msg->data.trade_t.from = (char*) malloc(strlen(from)+1);
	if(msg->data.trade_t.from == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	msg->data.trade_t.to = (char*) malloc(strlen(to)+1);
	if(msg->data.trade_t.to == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(msg->data.trade_t.from,from);
	strcpy(msg->data.trade_t.to,to);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void joinLeague_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = JOIN_LEAGUE;
	msg->data.ID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void createLeague_c(char * name){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = CREATE_LEAGUE;
	
	msg->data.name = (char*) malloc(strlen(name)+1);
	if(msg->data.name == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(msg->data.name,name);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void draft_c(char * id){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = DRAFT;
	msg->data.ID = atoi(id);
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
}

void logout_c(){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = LOGOUT;
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
	
}

void draftout_c(){

	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = DRAFT_OUT;
	
	response = communicate(msg);

	Element elem;
	
	/* Imprimo todos los msjs */
	FOR_EACH(elem, response->msgList){
		printf("%s\n",(char*)(elem->data));
	}
	
}
