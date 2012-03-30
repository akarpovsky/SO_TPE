/*
 * test_c.c
 *
 *  Created on: 26/03/2012
 *      Author: neon
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../includes/message.h"
#include "../../includes/fifo_c.h"

#define USER "FACUNDO"
#define PASS "123456"
#define SHOW_T_ID 1
#define TRADE_ID 2
#define TEAM_ID 3
#define LEAGUE_ID 4
#define LEAGUE_NAME "AmigosDelITBA"
#define FROM "Palermo"
#define TO "Messi"

int main(void){

	msg_t outcoming;
	Msg_s incoming;
	Element e;

	connectToServer();

	//XXX: Register

	printf("Executing: Register\n");
	outcoming.type = REGISTER;

	outcoming.data.register_t.user = calloc(strlen(USER), sizeof(char));
	strcpy(outcoming.data.register_t.user, USER);
	outcoming.data.register_t.pass = calloc(strlen(PASS), sizeof(char));
	strcpy(outcoming.data.register_t.pass, PASS);

	incoming = communicate(&outcoming);

	printf("Recieved:\n");
	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Login

	outcoming.type = LOGIN;

	outcoming.data.login_t.user = calloc(strlen(USER), sizeof(char));
	strcpy(outcoming.data.login_t.user, USER);
	outcoming.data.login_t.pass = calloc(strlen(PASS), sizeof(char));
	strcpy(outcoming.data.login_t.pass, PASS);

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: List Leagues

	outcoming.type = LIST_LEAGUES;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: List Teams

	outcoming.type = LIST_TEAMS;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}


	//XXX: List Trades

	outcoming.type = LIST_TRADES;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: League Show

	outcoming.type = LEAGUE_SHOW;

	outcoming.data.show_t.ID = SHOW_T_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Teams Show

	outcoming.type = TEAM_SHOW;

	outcoming.data.show_t.ID = SHOW_T_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Trade Show

	outcoming.type = LEAGUE_SHOW;

	outcoming.data.show_t.ID = SHOW_T_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Trade

	outcoming.type = TRADE;

	outcoming.data.trade_t.from = calloc(strlen(FROM), sizeof(char));
	strcpy(outcoming.data.trade_t.from, FROM);
	outcoming.data.trade_t.to = calloc(strlen(TO), sizeof(char));
	strcpy(outcoming.data.trade_t.to, TO);
	outcoming.data.trade_t.teamID = TEAM_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Trade Withdraw

	outcoming.type = TRADE_WITHDRAW;

	outcoming.data.trade_t.tradeID = TRADE_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Trade Accept

	outcoming.type = TRADE_ACCEPT;

	outcoming.data.trade_t.tradeID = TRADE_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Trade Negotiate

	outcoming.type = TRADE_NEGOTIATE;

	outcoming.data.trade_t.from = calloc(strlen(FROM), sizeof(char));
	strcpy(outcoming.data.trade_t.from, FROM);
	outcoming.data.trade_t.to = calloc(strlen(TO), sizeof(char));
	strcpy(outcoming.data.trade_t.to, TO);
	outcoming.data.trade_t.tradeID = TRADE_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Join League

	outcoming.type = JOIN_LEAGUE;

	outcoming.data.ID = LEAGUE_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Create League

	outcoming.type = CREATE_LEAGUE;

	outcoming.data.name= calloc(strlen(LEAGUE_NAME), sizeof(char));
	strcpy(outcoming.data.name, FROM);

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Draft

	outcoming.type = DRAFT;

	outcoming.data.ID = LEAGUE_ID;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Draft out

	outcoming.type = DRAFT_OUT;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}

	//XXX: Logout

	outcoming.type = LOGOUT;

	incoming = communicate(&outcoming);

	FOR_EACH(e, incoming->msgList){
		printf("%s\n", (char*)(e->data));
	}


}

