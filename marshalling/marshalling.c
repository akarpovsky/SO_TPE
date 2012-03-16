#include <stdio.h>
#include <stdlib.h>
#include "../includes/marshalling.h"

#include "../includes/message.h"

void register(char * user, char * pass){

	int status;
	msg_t msg;
	
	msg.type = REGISTER;
	msg.data.register_t.user = user;
	msg.data.register_t.pass = pass;

//	status = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void login(char * user, char * pass){
	
	int status;
	msg_t msg;
	
	msg.type = LOGIN;
	
	msg.data.login_t.user = user;
	msg.data.login_t.pass = pass;

//	status = comunicate(msg);
	
	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void list(int toList){
	
	int status;
	msg_t msg;
	
	msg.type = toList;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */	
}

void leagueShow(char * id){
	int status;
	msg_t msg;
	
	msg.type = LEAGUE_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void teamShow(char * id){
	int status;
	msg_t msg;
	
	msg.type = TEAM_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeShow(char * id){
	
	int status;
	msg_t msg;
	
	msg.type = TRADE_SHOW;
	msg.data.show_t.ID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void trade(char * id, char * from, char * to){
	int status;
	msg_t msg;
	
	msg.type = TRADE;
	msg.data.trade_t.teamID = id;
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeWithdraw(char * id){
	int status;
	msg_t msg;
	
	msg.type = TRADE_WITHDRAW;
	msg.data.trade_t.tradeID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeAccept(char * id){
	int status;
	msg_t msg;
	
	msg.type = TRADE_ACCEPT;
	msg.data.trade_t.tradeID = id;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}

void tradeNegotiate(char * id, char * from, char * to){
	int status;
	msg_t msg;
	
	msg.type = TRADE_NEGOTIATE;
	msg.data.trade_t.tradeID = id;
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
	
//	status = comunicate(msg);

	/* MANEJO DE ERRORES A PARTIR DE STATUS */
}