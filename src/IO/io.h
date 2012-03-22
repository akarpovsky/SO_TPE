/*
	IO.h
*/

#if !defined( IO_H )
#define IO_H

#include "../includes/structs.h"
#include "../utils/LinkedList.h"

#define BUFFER_SIZE 100

/* Object types for saving */

#define USER_OBJ 1
#define LEAGUE_OBJ 2
#define TEAM_OBJ 3
#define TRADE_OBJ 4

/* Functions for loading */

User loadUser(char * path);
Trade loadTrade(char * path);
Team loadTeam(char * path);
League loadLeague(char * path);
List loadUsers(char * path);
List loadTeams(char * path);
List loadTrades(char * path);
List loadLegaues(char * path);
Game loadGame();
/* File types */

#define FILE_TYPE 1
#define DIR_TYPE 2
#define UNKNOWN_TYPE -1

char * createFilePath(char * path, char * file);
int isExt(char * nameFile, char * ext);
int isTeamsFolder(char * nameFile);
int isTradesFolder(char * nameFile);
void printUser(User user);

#endif
