
#ifndef MESSAGE_H
#define MESSAGE_H

#include "../utils/LinkedList.h"
#include "./structs.h"

#define MAX_LENGTH 20
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

#define MAX_LENGHT 50


typedef struct msg_t{
	
	int type;
	union data{
		char * tempnam;
		int ping;
		struct register_t{
			char user[MAX_LENGTH];
			char pass[MAX_LENGTH];
		}register_t;
		struct login_t{
			char user[MAX_LENGTH];
			char pass[MAX_LENGTH];
		}login_t;
		struct show_t{
			int ID; /* league, team o trade */
		}show_t;
		struct trade_t{
			char from[MAX_LENGTH];
			char to[MAX_LENGTH];
			int teamID;
			int tradeID;
		}trade_t;
		struct socket_client_t{
			int socket_family;
			char socket_path[MAX_LENGHT];
		}socket_client_t;
	}data;
}msg_t;

typedef msg_t * Msg_t;

typedef struct msg_s {
	int status;
	List msgList;

}msg_s;

typedef msg_s * Msg_s;

#endif
