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
			char * ID; /* league, team o trade */
		}show_t;
				
		struct trade_t{
			char * from;
			char * to;
			char * teamID;
			char * tradeID;
		}trade_t;
	}data;
}msg_t;

typedef msg_t * Msg_t;

typedef struct msg_s {
	int status;
	List msgList;

}msg_s;

typedef msg_s * Msg_s;
