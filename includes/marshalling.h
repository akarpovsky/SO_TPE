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
