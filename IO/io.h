/*
	IO.h
*/

//#include "../includes/estructuras.h"

typedef struct user{

		char * user;
		char * pass;
		int leagues;
		int * leagueIDs;

		/* Alan, leagueIDs[0] = "AMIGOS ITBA" */

		/* LISTA DE LIGAS a las q pertenece league*   */

}user;

typedef user * User;

User loadUser(char * path);

