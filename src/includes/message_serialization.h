/*
 * message_serialization.h
 *
 *  Created on: 30/03/2012
 *      Author: neon
 */

#ifndef MESSAGE_SERIALIZATION_H_
#define MESSAGE_SERIALIZATION_H_

#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "fifo_contact_serialization.h"
#include "message.h"
#include "../utils/LinkedList.h"
#include "structs.h"

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


#endif /* MESSAGE_SERIALIZATION_H_ */
