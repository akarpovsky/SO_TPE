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
	
	msg.type = REGISTER;
	msg.data.register_t.user = user;
	msg.data.register_t.pass = pass;

//	response = comunicate(&msg);
	
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void login_c(char * user, char * pass){
	
	msg_t response;
	msg_t msg;
	
	msg.type = LOGIN;

	msg.data.login_t.user = user;
	msg.data.login_t.pass = pass;

//	response = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void list_c(int toList){
	
	msg_t response;
	msg_t msg;
	
	msg.type = toList;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */	
}

void leagueShow_c(char * id){
	msg_t response;
	msg_t msg;
	
	msg.type = LEAGUE_SHOW;
	msg.data.show_t.ID = id;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void teamShow_c(char * id){
	msg_t response;
	msg_t msg;
	
	msg.type = TEAM_SHOW;
	msg.data.show_t.ID = id;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeShow_c(char * id){
	
	msg_t response;
	msg_t msg;
	
	msg.type = TRADE_SHOW;
	msg.data.show_t.ID = atoi(id);
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void trade_c(char * id, char * from, char * to){
	msg_t response;
	msg_t msg;
	
	msg.type = TRADE;
	msg.data.trade_t.teamID = atoi(id);
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeWithdraw_c(char * id){
	msg_t response;
	msg_t msg;
	
	msg.type = TRADE_WITHDRAW;
	msg.data.trade_t.tradeID = atoi(id);
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeAccept_c(char * id){
	msg_t response;
	msg_t msg;
	
	msg.type = TRADE_ACCEPT;
	msg.data.trade_t.tradeID = atoi(id);
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeNegotiate_c(char * id, char * from, char * to){
	msg_t response;
	msg_t msg;
	
	msg.type = TRADE_NEGOTIATE;
	msg.data.trade_t.tradeID = atoi(id);
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	response = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE response */
}
