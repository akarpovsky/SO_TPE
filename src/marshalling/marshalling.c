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
	strcpy(msg->data.register_t.user,user);
	strcpy(msg->data.register_t.pass,pass);

//	response = comunicate(&msg);
	
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void login_c(char * user, char * pass){
	
	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
		
	msg->type = LOGIN;
	strcpy(msg->data.login_t.user,user);
	strcpy(msg->data.login_t.pass,pass);

//	response = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void list_c(int toList){
	
	Msg_s response;
	Msg_t msg = (Msg_t) malloc(sizeof(msg_t));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);		
	}
	
	msg->type = toList;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */	
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
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	strcpy(msg->data.trade_t.from,from);
	strcpy(msg->data.trade_t.to,to);
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
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
	strcpy(msg->data.trade_t.from,from);
	strcpy(msg->data.trade_t.to,to);
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}
