#define TRUE 1
#define FALSE 0


typedef int boolean;

typedef struct player_t{
	
	int ID;
	char * name;
	int points;
	
	}player_t;
	
typedef  player_t * Player;

typedef struct team_t{
	
	int ID;
	Player * players;
	
	}team_t;
	
typedef team_t * Team;

typedef struct trade_t{
	
	int ID;
	int status;
	Player pFrom;
	Player pTo;
	Team tFrom;
	Team tTo;
	
	}trade_t;

typedef trade_t * Trade;

typedef struct league_t{
	
	int ID;
	char * name;
	Team * teams;
	Player * players;
	Trade * trades;
	
	} league_t;

typedef league_t * League;

typedef struct user_t{
	
	char * username;
	char * password;
	League * leagues;
	
	} user_t;
	
typedef user_t * User;
