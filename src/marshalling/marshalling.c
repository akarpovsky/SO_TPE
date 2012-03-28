#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/marshalling.h"
#include "../includes/message.h"

void register_c(char * user, char * pass){

	Msg_s response;
	msg_t msg;
	
	msg.type = REGISTER;

	strcpy((msg.data.register_t.user), user);
	strcpy((msg.data.register_t.pass), pass);

	//TODO: fix this.
/*	msg.data.register_t.user = user;
	msg.data.register_t.pass = pass;*/

	response = communicate(&msg);
	
	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void login_c(char * user, char * pass){
	
	Msg_s response;
	msg_t msg;
	
	msg.type = LOGIN;

	//TODO: reformat to older new message.

	strcpy((msg.data.login_t.user), user);
	strcpy((msg.data.login_t.pass), pass);

	/*msg.data.login_t.user = user;
	msg.data.login_t.pass = pass;*/

	response = communicate(&msg);
	
	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void list_c(int toList){
	
	Msg_s response;
	msg_t msg;
	
	msg.type = toList;
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */	
}

void leagueShow_c(char * id){
	Msg_s response;
	msg_t msg;
	
	msg.type = LEAGUE_SHOW;
	msg.data.show_t.ID = id;
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void teamShow_c(char * id){
	Msg_s response;
	msg_t msg;
	
	msg.type = TEAM_SHOW;
	msg.data.show_t.ID = atoi(id);
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeShow_c(char * id){
	
	Msg_s response;
	msg_t msg;
	
	msg.type = TRADE_SHOW;
	msg.data.show_t.ID = atoi(id);
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void trade_c(char * id, char * from, char * to){
	Msg_s response;
	msg_t msg;
	
	msg.type = TRADE;
	msg.data.trade_t.teamID = atoi(id);
	strcpy((msg.data.trade_t.from), from);
	strcpy((msg.data.trade_t.to ), to);

	//TODO: fix this.
	/*msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;*/
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeWithdraw_c(char * id){
	Msg_s response;
	msg_t msg;
	
	msg.type = TRADE_WITHDRAW;
	msg.data.trade_t.tradeID = atoi(id);
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeAccept_c(char * id){
	Msg_s response;
	msg_t msg;
	
	msg.type = TRADE_ACCEPT;
	msg.data.trade_t.tradeID = atoi(id);
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}

void tradeNegotiate_c(char * id, char * from, char * to){
	Msg_s response;
	msg_t msg;
	
	msg.type = TRADE_NEGOTIATE;
	msg.data.trade_t.tradeID = atoi(id);

	strcpy((msg.data.trade_t.from), from);
	strcpy((msg.data.trade_t.to ), to);

	//TODO: fix this
/*
	msg.data.trade_t.from = from;
	msg.data.trade_t.to = to;
*/
	
	response = communicate(&msg);

	//TODO: remove all this crap!

	printf("%s\n", (char *)response->msgList->pFirst->data);
	/* MANEJO DE ERRORES A PARTIR DE response */
}
