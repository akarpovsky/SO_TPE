
#ifndef MESSAGE_H
#define MESSAGE_H

#include "../utils/LinkedList.h"
#include "./structs.h"

#define MAX_LENGTH 20
#define CONTACT 1012
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
			int socket_family;
			char * socket_path;
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
