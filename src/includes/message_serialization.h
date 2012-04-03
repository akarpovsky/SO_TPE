/*
 * message_serialization.h
 *
 */

#ifndef MESSAGE_SERIALIZATION_H_
#define MESSAGE_SERIALIZATION_H_

#include <stdlib.h>
#include <string.h>
#include "defines.h"

#ifdef fifo
	#include "./fifo_contact_serialization.h"
#endif
#ifdef shmm
	#include "./shmm_contact_serialization.h"
#endif


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


Msg_s deserialize_s (void * stream);
void * serialize_s (Msg_s msg);


#endif /* MESSAGE_SERIALIZATION_H_ */
