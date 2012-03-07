#define TRUE 1
#define FALSE 0


typedef boolean int;

typedef struct user_t{
	
	char * username;
	char * password;
	League * leagues;
	
	} user_t;
	
typedef User *user_t;
		
typedef struct league_t{
	
	int ID;
	char * name;
	Team * teams;
	Player * players;
	Trade * trades;
	
	} league_t;

	typedef League *league_t;
	
typedef struct team_t{
	
	int ID;
	User owner;
	Player * players;
	
	}team_t;
	
typedef Team *team_t;

typedef struct player_t{
	
	int ID;
	char * name;
	int points;
	
	}player_t;
	
typedef Player *player_t;

typedef struct trade_t{
	
	int ID;
	int status;
	Player pFrom;
	Player pTo;
	Team tFrom;
	Team tTo;
	
	}trade_t;
	typedef Trade *trade_t;
