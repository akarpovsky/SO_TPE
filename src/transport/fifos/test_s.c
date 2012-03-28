/*
 * test_s.c
 *
 *  Created on: 26/03/2012
 *      Author: neon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/fifo_s.h"

int readyFlag = FALSE;

Msg_s doCommand(Channel ch, Msg_t incoming);

int main(void)
{
	uplink();

	Msg_t incoming;
	Msg_s outcoming;
	Channel ch = NULL;

	do{
		if((incoming = listen(ch)) != NULL){
			if(ch == NULL){
				ch = createChannel(incoming);
				establishChannel(ch);
				printf("Ready!\n");
			}
			outcoming = doCommand(ch, incoming);
			communicate(ch, outcoming);

		}
	}while(readyFlag == FALSE);

	return SUCCESSFUL;
}


Msg_s doCommand(Channel ch, Msg_t incoming)
{
	Msg_s outcoming = malloc(sizeof(msg_s));
	outcoming->status = SUCCESSFUL;
	List l = malloc(sizeof(llist));
	CreateList(l);
	char * rStr;
	switch(incoming->type)
	{
	case CONTACT:
		printf("Contacted\n");
		rStr = malloc(strlen(ch->fifoIn));
		strcpy(rStr, ch->fifoIn);
		break;
	case LOGOUT:
		printf("Exiting\n");
		readyFlag = TRUE;
		rStr = malloc(strlen("Exiting"));
		strcpy(rStr, "Exiting");
		break;
	case REGISTER:
		printf("Registration attemp: %s, %s\n", incoming->data.register_t.user, incoming->data.register_t.pass);
		rStr = malloc(strlen("Registering"));
		strcpy(rStr, "Registering");
		break;
	case LOGIN:
		printf("Login attemp: %s, %s\n", incoming->data.login_t.user, incoming->data.login_t.pass);
		rStr = malloc(strlen("Logging"));
		strcpy(rStr, "Logging");
		break;
	case LIST_LEAGUES:
	case LIST_TEAMS:
	case LIST_TRADES:
		printf("List command recieved: %d\n", incoming->type);
		rStr = malloc(strlen("Listing"));
		strcpy(rStr, "Listing");
		break;
	case LEAGUE_SHOW:
	case TEAM_SHOW:
	case TRADE_SHOW:
		printf("showcmd: %d\n", incoming->data.show_t.ID);
		rStr = malloc(strlen("Showing"));
		strcpy(rStr, "Showing");
		break;
	case TRADE:
		printf("Trade made: %s, %s, %d\n", incoming->data.trade_t.from, incoming->data.trade_t.to, incoming->data.trade_t.teamID);
		rStr = malloc(strlen("Trading"));
		strcpy(rStr, "Trading");
		break;
	case TRADE_WITHDRAW:
		printf("Trade withdrawn: %d\n", incoming->data.trade_t.tradeID);
		rStr = malloc(strlen("Withdrawing"));
		strcpy(rStr, "Withdrawing");
		break;
	case TRADE_ACCEPT:
		printf("Accepting trade: %d\n", incoming->data.trade_t.tradeID);
		rStr = malloc(strlen("Accepting"));
		strcpy(rStr, "Accepting");
		break;
	case TRADE_NEGOTIATE:
		printf("Negotiating trade: %d\n", incoming->data.trade_t.tradeID);
		rStr = malloc(strlen("Negotiating"));
		strcpy(rStr, "Negotiating");
		break;

	}


	AddToList(rStr, l);

	return outcoming;
}
