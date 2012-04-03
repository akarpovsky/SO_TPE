
#ifndef STRUCTS_H
#define STRUCTS_H

#include "../utils/LinkedList.h"
#include <pthread.h>

#define MAX_PLAYERS 5

typedef struct trade{
	
	int ID;
	int state;
	char * userFrom;
	char * userTo;
	char * playerFrom;
	char * playerTo;
	}trade_t;

typedef trade_t * Trade;



typedef struct player{

	char * name;
	int points;
	
}player_t;

typedef player_t * Player;



typedef struct user{
		char * user;
		char * pass;
		int leagues;
		List leaguesIDs;
}user_t;

typedef user_t * User;



typedef struct team{
	
	int ID; /* ID del Team */
	char * owner;
	int points;	
	int cantPlayers;
	List players;

}team_t;

typedef team_t * Team;



typedef struct league{
	int ID;
	char * name;
	int status;
	int cantAvailablePlayers;
	List availablePlayers;
	List teams;
	List trades;
}league_t;

typedef league_t * League;



typedef struct game{
	
	List users;
	List leagues;
	List loggedUsers;
	int cantTeams;
	int cantTrades;

	}game_t;

typedef game_t * Game;

typedef struct client_t{
	
	int clientID;
	pthread_t client_thread;
	
	} client_t;

typedef client_t * Client;

#endif