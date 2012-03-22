

#ifndef MARSHALLING_H
#define MARSHALLING_H

void register_c(char * user, char * pass);
void login_c(char * user, char * pass);
void list_c(int toList);
void leagueShow_c(char * id);
void teamShow_c(char * id);
void tradeShow_c(char * id);
void trade_c(char * id, char * from, char * to);
void tradeWithdraw_c(char * id);
void tradeAccept_c(char * id);
void tradeNegotiate_c(char * id, char * from, char * to);

#endif