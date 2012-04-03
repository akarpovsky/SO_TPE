#ifndef EXECUTE_H
#define EXECUTE_H

#ifdef fifo
	#include "fifo_s.h"
#endif
#ifdef sockets
	#include "socket_s.h"
#endif
#ifdef msgqueue
	#include "mq_s.h"
#endif
#ifdef shmm
	#include "shmm_s.h"
#endif

void reverse(char s[]);
void itoa(int n, char s[]);
void * createMsg_s();
void execute(Msg_t msg, Channel ch, User * me);
void executeRegister(Msg_t msg, Channel ch);
void executeLogin(Msg_t msg, Channel ch, User * me);
void executeListLeagues(Msg_t msg, Channel ch);
void executeListTeams(Msg_t msg, Channel ch);
void executeListTrades(Msg_t msg, Channel ch, User * me);
void executeLeagueShow(Msg_t msg, Channel ch);
void executeTeamShow(Msg_t msg, Channel ch);
void executeTradeShow(Msg_t msg, Channel ch);
void executeTrade(Msg_t msg, Channel ch, User * me);

#endif
