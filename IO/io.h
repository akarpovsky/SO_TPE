/*
	IO.h
*/

#if !defined( io_h )
#define io_h
#endif


#include "../includes/structs.h"

#define BUFFER_SIZE 100

/* Object types for saving */

#define USER_OBJ 1
#define LEAGUE_OBJ 2
#define TEAM_OBJ 3
#define TRADE_OBJ 4

/* Functions for loading */

User loadUser(char * path);
League loadTLeague(char * path);
Trade loadTrade(char * path);
Team loadTeam(char * path);


/* File types */

#define FILE_TYPE 1
#define DIR_TYPE 2
#define UNKNOWN_TYPE -1


