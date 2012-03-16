#include <stdio.h>
#include <stdlib.h>

#include "../includes/marshalling.h"
#include "../includes/message.h"

void register_c(char * user, char * pass){

	int status;
	msg_t msg;
	
	msg.type = REGISTER;
	msg.data.register_t.user = user;
	msg.data.register_t.pass = pass;

//	status = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void login_c(char * user, char * pass){
	
	int status;
	msg_t msg;
	
	msg.type = LOGIN;

	msg.data.login_t.user = user;
	msg.data.login_t.pass = pass;

//	status = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void list_c(int toList){
	
	int status;
	msg_t msg;
	
	msg.type = toList;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */	
}

void leagueShow_c(char * id){
	int status;
	msg_t msg;
	
	msg.type = LEAGUE_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void teamShow_c(char * id){
	int status;
	msg_t msg;
	
	msg.type = TEAM_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeShow_c(char * id){
	
	int status;
	msg_t msg;
	
	msg.type = TRADE_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void trade_c(char * id, char * from, char * to){
	int status;
	msg_t msg;
	
	msg.type = TRADE;
	msg.data.trade_t.teamID = id;
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeWithdraw_c(char * id){
	int status;
	msg_t msg;
	
	msg.type = TRADE_WITHDRAW;
	msg.data.trade_t.tradeID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeAccept_c(char * id){
	int status;
	msg_t msg;
	
	msg.type = TRADE_ACCEPT;
	msg.data.trade_t.tradeID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeNegotiate_c(char * id, char * from, char * to){
	int status;
	msg_t msg;
	
	msg.type = TRADE_NEGOTIATE;
	msg.data.trade_t.tradeID = id;
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}