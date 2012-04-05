/*
	IO.h
*/

#if !defined( IO_H )
#define IO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include "../includes/structs.h"
#include "../utils/LinkedList.h"

#define DIGIT_COUNT(X) ((int)(log10((((X)>0)?(X):-(X)))))
#define BUFFER_SIZE 100

/* Object types for saving */

#define USER_OBJ 1
#define LEAGUE_OBJ 2
#define TEAM_OBJ 3
#define TRADE_OBJ 4
#define MATCHES_DIR "./res/matches"
#define MATCHES_DUMP "./res/matches/dump"
#define MATCHES_NEW "./res/matches/new"
#define TRUE 1
#define FALSE 0

#define DEFINE_VARIABLES
#include "../includes/global.h"

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

char * userToString(User usr);
char * leagueToString(League lg);
char * tradeToString(Trade tr);
char * teamToString(Team t);

void updatePlayers(List l);
List loadMatch(FILE * fp);
void dumpMatch(char * f);
void checkMatches(void);

#endif
