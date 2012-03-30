
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdlib.h>
#include <string.h>
#include "fifo_contact_serialization.h"
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

#define MSG_T_MAX_COMMANDS 18


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
}msg_t;

typedef msg_t * Msg_t;

typedef struct msg_s {
	int status;
	List msgList;

}msg_s;

typedef msg_s * Msg_s;

Msg_t deserializeMsg(void * stream);
void * serializeMsg(Msg_t msg);

void * serialize_register (Msg_t msg);
void * serialize_login (Msg_t msg);
void * serialize_listLeagues (Msg_t msg);
void * serialize_listTeams (Msg_t msg);
void * serialize_listTrades (Msg_t msg);
void * serialize_leagueShow (Msg_t msg);
void * serialize_teamShow (Msg_t msg);
void * serialize_tradeShow (Msg_t msg);
void * serialize_trade (Msg_t msg);
void * serialize_tradeWithdraw (Msg_t msg);
void * serialize_tradeAccept (Msg_t msg);
void * serialize_tradeNegotiate (Msg_t msg);
void * serialize_joinLeague (Msg_t msg);
void * serialize_createLeague (Msg_t msg);
void * serialize_draft (Msg_t msg);
void * serialize_draftOut (Msg_t msg);
void * serialize_logout (Msg_t msg);

Msg_t deserialize_register (Msg_t msg, void * stream);
Msg_t deserialize_login (Msg_t msg, void * stream);
Msg_t deserialize_listLeagues (Msg_t msg, void * stream);
Msg_t deserialize_listTeams (Msg_t msg, void * stream);
Msg_t deserialize_listTrades (Msg_t msg, void * stream);
Msg_t deserialize_leagueShow (Msg_t msg, void * stream);
Msg_t deserialize_teamShow (Msg_t msg, void * stream);
Msg_t deserialize_tradeShow (Msg_t msg, void * stream);
Msg_t deserialize_trade (Msg_t msg, void * stream);
Msg_t deserialize_tradeWithdraw (Msg_t msg, void * stream);
Msg_t deserialize_tradeAccept (Msg_t msg, void * stream);
Msg_t deserialize_tradeNegotiate (Msg_t msg, void * stream);
Msg_t deserialize_joinLeague (Msg_t msg, void * stream);
Msg_t deserialize_createLeague (Msg_t msg, void * stream);
Msg_t deserialize_draft (Msg_t msg, void * stream);
Msg_t deserialize_draftOut (Msg_t msg, void * stream);
Msg_t deserialize_logout (Msg_t msg, void * stream);


/*Msg_t (*_deserialize_t[MSG_T_MAX_COMMANDS])(Msg_t msg, void * stream) = {
		deserialize_contact,
		deserialize_register,
		deserialize_login,
		deserialize_listLeagues,
		deserialize_listTeams,
		deserialize_listTrades,
		deserialize_leagueShow,
		deserialize_teamShow,
		deserialize_tradeShow,
		deserialize_trade,
		deserialize_tradeWithdraw,
		deserialize_tradeAccept,
		deserialize_tradeNegotiate,
		deserialize_logout,
		deserialize_joinLeague,
		deserialize_createLeague,
		deserialize_draft,
		deserialize_draftOut
};*/
/*void * (*_serialize_t[MSG_T_MAX_COMMANDS]) (Msg_t msg) = {
		serialize_contact,
		serialize_register,
		serialize_login,
		serialize_listLeagues,
		serialize_listTeams,
		serialize_listTrades,
		serialize_leagueShow,
		serialize_teamShow,
		serialize_tradeShow,
		serialize_trade,
		serialize_tradeWithdraw,
		serialize_tradeAccept,
		serialize_tradeNegotiate,
		serialize_logout,
		serialize_joinLeague,
		serialize_createLeague,
		serialize_draft,
		serialize_draftOut
};*/


Msg_s deserialize_s (void * stream);
void * serialize_s (Msg_s msg);

#endif
