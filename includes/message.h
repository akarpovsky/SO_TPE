
typedef struct msg_t{
	
	int type;
	union data{
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