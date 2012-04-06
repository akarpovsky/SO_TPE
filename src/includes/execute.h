#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "../utils/LinkedList.h"
#include "structs.h"
#include "message.h"
#include "../IO/io.h"
#include "../utils/printColor.h"

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

#define DEFINE_VARIABLES
#include "../includes/global.h"

#define OK 0
#define ERROR 1


#define INACTIVE 0
#define DRAFTING 2
#define ACTIVE 3

#define TIME_OUT 60

void reverse(char s[]);
void itoa(int n, char s[]);
void * coordinator_thread(void * data);
void * createMsg_s(int responseType);
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
void executeTradeWithdraw(Msg_t msg,Channel ch, User * me);
void executeTradeAccept(Msg_t msg,Channel ch, User * me);
void executeDraft(Msg_t msg,Channel ch, User * me);
void executeLogout(Msg_t msg,Channel ch,User * me);

void * coordinator_thread(void * data);
void changeTurn(League l, int * repeatFlag);
void autoAsign(League l, int * repeatFlag);
void checkStatus(League l);
void makeDraft(League league,Channel ch, User * me);



#endif
