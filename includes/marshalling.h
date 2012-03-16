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

void register(char * user, char * pass);
void login(char * user, char * pass);
void list(int toList);
void leagueShow(char * id);
void teamShow(char * id);
void tradeShow(char * id);
void trade(char * id, char * from, char * to);
void tradeWithdraw(char * id);
void tradeAccept(char * id);
void tradeNegotiate(char * id, char * from, char * to);