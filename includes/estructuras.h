


typedef struct game{
	
	int users; /* usuarios conectados */
	/* Lista de usuarios  user*  */

	int leagues;
	/* LISTA DE LIGAS  league*   */
	
	int teams;
	
	/* lista de Team team*/
}


typedef struct user{
		
	char * user;
	char * pass;
	int leagues;
	int * leagueIDs;
	
	/* Alan, leagueIDs[0] = "AMIGOS ITBA" */
	
	/* LISTA DE LIGAS a las q pertenece league*   */
	
}user;

typedef struct league{
	
	int ID;
	int status;
	int availablePlayers;
	/* LISTA DE JUGADORES LIBRES player * */
	
	/* LISTA DE TRADES  trade * */
	
	
	int teams;
	/* LISTA DE punteros a TEAMS (cada team va a tener de nombre al user al cual pertenece) team* */
	
}league;

typedef struct team{
	
	int ID; /* Nombre de la liga */
	int points;
	
	int players;
	
	/* LISTA DE punteros a jugadores */

}team;

typedef struct player{

	char * name;
	int points;
	
}player;

typedef struct trade{
	
	int ID;
	league * league;
	int state;
	char * userFrom;
	char * userTo;
	char * playerFrom;
	char * playerTo;
	}trade;


typedef user * User;
typedef game * Game;
typedef league * League;
typedef team * Team;
typedef player * Player;
typedef trade * Trade;