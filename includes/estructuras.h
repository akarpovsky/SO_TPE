
typedef struct game{
	
	int users; /* usuarios conectados */
	/* Lista de usuarios  user*  */
	
	int trade;
	/* LISTA DE TRADES trade */
	
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
	user * userFrom;
	user * userTo;
	player * playerFrom;
	player * playerTo;
	}trade;
