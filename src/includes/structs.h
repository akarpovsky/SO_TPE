
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
		List leaguesIDs;
}user;

typedef user * User;



typedef struct team{
	
	int ID; /* ID del Team */
	char * owner;
	int points;	
	int cantPlayers;
	List players;

}team;

typedef team * Team;



typedef struct league{
	int ID;
	char * name;
	int status;
	int cantAvailablePlayers;
	List availablePlayers;
	List teams;
	List trades;
}league;

typedef league * League;



typedef struct game{
	
	List users;
	List leagues;
	List teams;
	List loggedUsers;
	int cantTeams;
	int cantTrades;

	}game;

typedef game * Game;

typedef struct client_t{
	
	int clientID;
	pthread_t client_thread;
	
	} client_t;

typedef client_t * Client;

#endif