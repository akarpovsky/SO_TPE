
 #if !defined( structs_h )
#define structs_h
#endif

#define MAX_PLAYERS 5

typedef struct trade{
	
	int ID;
	int state;
	char * userFrom;
	char * userTo;
	char * playerFrom;
	char * playerTo;
	}trade;

typedef trade * Trade;



typedef struct player{

	char * name;
	int points;
	
}player;

typedef player * Player;



typedef struct user{
		char * user;
		char * pass;
		int leagues;
		int * leagueIDs;
}user;

typedef user * User;



typedef struct team{
	
	int ID; /* ID del Team */
	int points;	
	int cantPlayers;
	Player players[MAX_PLAYERS];

}team;

typedef team * Team;



typedef struct league{
	
	int ID;
	int status;
	int availablePlayers;

	
	/* LISTA DE TRADES  trade * */
	
	
	int teams;
	/* LISTA DE punteros a TEAMS (cada team va a tener de nombre al user al cual pertenece) team* */
	
}league;

typedef league * League;



typedef struct game{
	
	int users; /* usuarios conectados */
	/* Lista de usuarios  user*  */

	int leagues;
	/* LISTA DE LIGAS  league*   */
	
	int teams;
	
	/* lista de Team team*/
	}game;

typedef game * Game;

