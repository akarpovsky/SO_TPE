
#ifndef MESSAGE_H
#define MESSAGE_H

#include "../utils/LinkedList.h"
#include "structs.h"

#define CONTACT 0
#define REGISTER 1
#define LOGIN 2
#define LIST_LEAGUES 3
#define LIST_TEAMS 4
#define LIST_TRADES 5
#define LEAGUE_SHOW 6
#define TEAM_SHOW 7
#define TRADE_SHOW 8
#define TRADE 9
#define TRADE_WITHDRAW 10
#define TRADE_ACCEPT 11
#define TRADE_NEGOTIATE 12
#define LOGOUT 13
#define JOIN_LEAGUE 14
#define CREATE_LEAGUE 15
#define DRAFT 16
#define DRAFT_OUT 17
#define CHOOSE 18
#define DRAFT_TURN 19
#define DRAFT_START 20
#define DRAFT_END 21

#define MSG_T_MAX_COMMANDS 19
#define DRAFT_STARTED 1
#define DRAFT_ENDED 2

#define CONVENTION 50000
#define MAX_LENGHT 50

typedef struct msg_t{
	
	int type;
	union data{
		char * tempnam;
		int ping;
		int ID;
		char * name;
		struct register_t{
			char * user;
			char * pass;
		}register_t;
		struct login_t{
			char * user;
			char * pass;
		}login_t;
		struct show_t{
			int ID; /* league, team o trade */
		}show_t;
		struct trade_t{
			char * from;
			char * to;
			int teamID;
			int tradeID;
		}trade_t;
		struct socket_client_t{
			int client_pid;
			struct sockaddr_in * client;
		}socket_client_t;
	}data;
	int pidFrom;
} msg_t;

typedef struct msg_s {
	int responseType;
	int status;
	List msgList;

}msg_s;


typedef msg_t * Msg_t;

typedef msg_s * Msg_s;

#endif
