
typedef struct msg_t{
	
	int type;
	union{
		int ping;
		struct register_t{
			char * user;
			char * pass;
		};
		struct login_t{
			char * user;
			char * pass;
		};
		int listType; 
		struct show_t{
			int showType;
			union{
				char * leagueID;
				int teamID;
				int tradeID;
			};
		};
		struct trade{
			int type;
			int tradeID;
			int *fPlayerID;
			int *tPlayerId;
		};
		
	};
	
	
}