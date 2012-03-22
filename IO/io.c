#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>



#include "./io.h"



/*
* function loadUser
*
* Alloc memory for a user_t and Loads a specific user
* fiven the path of the saved one.
*
* @path: Path of the saved user file 
*/
User loadUser(char * path){
	
	FILE * file;
	int * id;
	

	User auxUser; // Will store the loaded user
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num, auxNum;
	
	auxUser = calloc(sizeof(user),1);
	
	if(auxUser == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	file = fopen(path,"r");
	
	if(file == NULL){
		printf("<LOG - io.c>\n\tFile not found.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	/* Load username */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->user  = (char *) malloc(dim);
	if(auxUser->user == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->user,aux);
	auxUser->user[dim-1] = '\0';
	
	/* Load password */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->pass  = (char *)  malloc(dim);
	if(auxUser->pass == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->pass,aux);
	auxUser->pass[dim-1] = '\0';
	
	
	/* Load number of Leagues */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxUser->leagues = num;
	
	auxUser->leaguesIDs = malloc(sizeof(llist));
	if(auxUser->leaguesIDs == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	CreateList(auxUser->leaguesIDs);
	
	int i;
	for(i = 0; i < num; i++){
		fgets(aux,BUFFER_SIZE,file);
		auxNum = atoi(aux);
		id = malloc(sizeof(int));
		*id = auxNum;
		AddToList(id,auxUser->leaguesIDs);
	}
	
	
	return auxUser;
}


/*
* function loadTrade
*
* Alloc memory for a trade and Loads a specific trade
* given the path of the saved one.
*
* @path: Path of the saved trade file 
*/

Trade loadTrade(char * path){

	FILE * file;
	
	Trade auxTrade; // Will store the loaded trade
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num;
		
	auxTrade = malloc(sizeof(trade));
	
	if(auxTrade == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	file = fopen(path,"r");
	
	if(file == NULL){
		perror("File not found\n");
		exit(EXIT_FAILURE);
	}
	
	/* Load id */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTrade->ID = num;

	/* Load state */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTrade->state = num;
	
	/* Load userFrom */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->userFrom  = (char *) malloc(dim);
	if(auxTrade->userFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->userFrom,aux);
	auxTrade->userFrom[dim-1] = '\0';
	
	/* Load userTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->userTo  = (char *) malloc(dim);
	if(auxTrade->userTo == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->userTo,aux);
	auxTrade->userTo[dim-1] = '\0';
	
	/* Load playerFrom */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerFrom  = (char *) malloc(dim);
	if(auxTrade->playerFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->playerFrom,aux);
	auxTrade->playerFrom[dim-1] = '\0';
	
	/* Load playerTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerTo  = (char *) malloc(dim);
	if(auxTrade->playerTo == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->playerTo,aux);
	auxTrade->playerTo[dim-1] = '\0';
	
	return auxTrade;
}


/*
* function loadTeam
*
* Alloc memory for a team and Loads a specific team
* given the path of the saved one.
*
* @path: Path of the saved trade file 
*/
Team loadTeam(char * path){
	
	FILE * file;

	Team auxTeam; // Will store the loaded team
	Player auxPlayer;
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num, i;
	
	auxTeam = malloc(sizeof(team));
	
	if(auxTeam == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	file = fopen(path,"r");
	
	if(file == NULL){
		perror("File not found\n");
		exit(EXIT_FAILURE);
	}

	/* Load id */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTeam->ID = num;
	
	/* Load owner */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTeam->owner  = (char *) malloc(dim);
	if(auxTeam->owner == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTeam->owner,aux);
	auxTeam->owner[dim-1] = '\0';
	
	/* Load points */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTeam->points = num;
	
	/* Load players */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	if(num > MAX_PLAYERS){
		printf("<LOG - io.c>\n\tInvalid file, too much players\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	auxTeam->cantPlayers = num;
	

	
	for(i = 0; i < auxTeam->cantPlayers; i++){

		auxPlayer = malloc(sizeof(player));
		if(auxPlayer == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		fgets(aux,BUFFER_SIZE,file);
		dim = strlen(aux);
		auxPlayer->name  = (char *) malloc(dim);
		if(auxPlayer->name == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(auxPlayer->name,aux);
		auxPlayer->name[dim-1] = '\0';

		/* Load points */
		fgets(aux,BUFFER_SIZE,file);
		num = atoi(aux);
		auxPlayer->points = num;	
		
		(auxTeam->players)[i] = auxPlayer;
	}
	return auxTeam;
}


/*
* function dismissWrongFiles
*
* Dismiss "." and ".." files
*
* @entry: Dirent entry 
*/

int dismissWrongFiles(struct dirent * entry){
	if (entry->d_name[0] == '.'){
			return 0;			
		}
	else
		return 1;
}

/*
*
* function fileType
*
* Returns the type (regular file or folder);
*
* @path: Path to the file
* @file: File/folder name
*/

int fileType(char * filePath){
	int status;
	struct stat st_buf;

	// Get the status of the file system object.

	status = stat(filePath, &st_buf);
	if (status != 0) {
		perror("<LOG - io.c>\n\tError loading stat.\n<end>\n");
		return -1;
	}
	
	
	if (S_ISREG (st_buf.st_mode)) {
		return FILE_TYPE;
	}
	
	if (S_ISDIR (st_buf.st_mode)) {
		return DIR_TYPE;
	}
	
	return UNKNOWN_TYPE;
}

char * createFilePath(char * path, char * file){
	char * filePath = (char *) calloc(BUFFER_SIZE, sizeof(char));
	
	strcat(filePath, path);
	strcat(filePath, file);
	
	return filePath;
	
}

List loadUsers(char * path){
	
	List usersList = (List *) malloc(sizeof(llist));
	CreateList(usersList);
//	AddToList((void *) 5, usersList);
//	printf("%d\n", usersList->pFirst);

    DIR * d;
    char * dir_name = path;
    struct dirent * entry;

    /* Open the current directory. */
    d = opendir(dir_name);

    if(!d) {
        printf("Cannot open directory '%s'\n",dir_name);
        exit(EXIT_FAILURE);
    }

    while(1){
        entry = readdir(d);
        
		if (!entry) {
            break;
        }else{
			if(dismissWrongFiles(entry)){
				char * filePath = createFilePath(path, entry->d_name);
				int type = fileType(filePath);
				if( type == FILE_TYPE){
					printf("<LOG - io.c> Cargando usuario: %s <end>\n", entry->d_name);
					
					AddToList((void *) loadUser(filePath), usersList);
										
				}	
				else if(type == DIR_TYPE)
					printf ("Carpeta: %s\n", entry->d_name);
				
			}
        }	
    }
    /* Close the directory. */
    if (closedir(d)) {
        perror("Could not close directory\n");
        exit (EXIT_FAILURE);
    }

	return usersList;
}

int isExt(char * nameFile, char * ext){
	
	int dim = strlen(nameFile);
	int i;
	
	for(i = strlen(ext); i >= 0; i--, dim--){
		if(nameFile[dim] != ext[i]){
			return 0;
		}
	}
	
	return 1;
}

int isTeamsFolder(char * nameFile){

	return strcmp("teams",nameFile) == 0;
}

int isTradesFolder(char * nameFile){

	return strcmp("teams",nameFile) == 0;
}

List loadTeams(char * path){
	
	
		List teamsList = (List *) malloc(sizeof(llist));
		if(teamsList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(teamsList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open teams directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == FILE_TYPE && isExt(entry->d_name,"team")){
						AddToList((void *) loadTeam(filePath), teamsList);
					}	

				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }

		return teamsList;
}

List loadTrades(char * path){
	
	
		List tradesList = (List *) malloc(sizeof(llist));
		if(tradesList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(tradesList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open trades directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == FILE_TYPE && isExt(entry->d_name,"trade")){
						AddToList((void *) loadTrade(filePath), tradesList);
					}	

				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }

		return tradesList;
}

League loadLeague(char * path){


	DIR * d = NULL;
	char * dir_name = path;
	struct dirent * entry;
	int num,i,dim;
	Player auxPlayer = NULL;

	League auxLeague; // Will store the loaded team
	auxLeague = malloc(sizeof(league));
	
	if(auxLeague == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	/* Open the current directory. */
	d = opendir(dir_name);
	
	if(!d){
		printf("Cannot open directory '%s'\n",dir_name);
		exit(EXIT_FAILURE);
	}
	while(1){
		entry = readdir(d);
		
		if(!entry){
			break;
		}else{
			if(dismissWrongFiles(entry)){
				char * filePath = createFilePath(path,entry->d_name);
				int type = fileType(filePath);
				/* si es el archivo league */
				if(type == FILE_TYPE && isExt(entry->d_name,"league")){
					FILE * file;
					char aux[BUFFER_SIZE]; // For reading the file line by line
					file = fopen(filePath,"r");

					if(file == NULL){
						perror("File not found\n");
						exit(EXIT_FAILURE);
					}
					/* Load ID */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->ID = num;

					/* Load name */
					fgets(aux,BUFFER_SIZE,file);
					dim = strlen(aux);
					auxLeague->name  = (char *) malloc(dim);
					if(auxLeague->name == NULL){
						printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
						exit(EXIT_FAILURE);
					}
					strcpy(auxLeague->name,aux);
					auxLeague->name[dim-1] = '\0';

					
					/* Load status */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->status = num;
					
					/* Load available players */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->cantAvailablePlayers = num;

					auxLeague->availablePlayers = (List)malloc(sizeof(llist));
					if(auxLeague == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					CreateList(auxLeague->availablePlayers);
					
					for(i = 0; i < auxLeague->cantAvailablePlayers; i++){
						auxPlayer = malloc(sizeof(player));
						if(auxPlayer == NULL){
							perror("Insufficient memory\n");
							exit(EXIT_FAILURE);
						}
						/* Load Player Name */ 
						fgets(aux,BUFFER_SIZE,file);
						dim = strlen(aux);
						auxPlayer->name  = (char *) malloc(dim);
						if(auxPlayer->name == NULL){
						printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
						exit(EXIT_FAILURE);
						}
						strcpy(auxPlayer->name,aux);
						auxPlayer->name[dim-1] = '\0';

						/* Load Player points */ 
						fgets(aux,BUFFER_SIZE,file);
						num = atoi(aux);
						auxPlayer->points = num;

						AddToList(auxPlayer,auxLeague->availablePlayers);

					} 

					fclose(file);


				}else if(type == DIR_TYPE && isExt(entry->d_name,"teams")){
					filePath = createFilePath(filePath,"/");
					auxLeague->teams = loadTeams(filePath);
					
				}else if(type == DIR_TYPE && isExt(entry->d_name,"trades")){
					filePath = createFilePath(filePath,"/");
					auxLeague->trades = loadTrades(filePath);
					
				}
			}
		}
	}
	
	if (closedir(d)) {
        perror("Could not close directory\n");
        exit (EXIT_FAILURE);
    }
	
	return auxLeague;
}

List loadLeagues(char * path){	
	
		List leaguesList = (List *) malloc(sizeof(llist));
		if(leaguesList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(leaguesList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == DIR_TYPE){
						filePath = createFilePath(filePath,"/");
						printf("path de LEAGUE %s\n", filePath);
						AddToList((void *) loadLeague(filePath), leaguesList);
					}
				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }

		return leaguesList;
}



/*
* function objToFile
*
* Given an object type (i.e. "User"), builds
* the string that will be saved in the hard drive
* for persisting that kind of object.
*
* @path: Path of the saved user file 
*/
char * objToFile(int OBJ_TYPE, void * obj){
	User auxUser;
	switch(OBJ_TYPE){
		case USER_OBJ:
		
			auxUser = (User) obj;
			printf("%s\n\n", auxUser->user);
		
		break;
		
		case LEAGUE_OBJ:
		
		break;
		
		case TRADE_OBJ:
		
		break;
		
		case TEAM_OBJ:
		
		break;
		
		default:
			printf("<LOG - io.c>\n\tUnrecognized data type.\n<end>\n");
		break;
	}
		
}

Game loadGame(void){

	Game game = (Game *)malloc(sizeof(game));
	if(game == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	game->users = loadUsers("../res/users/");
	game->leagues = loadLeagues("../res/legues/");
	game->teams = (List) malloc(sizeof(llist));
	if(game->teams == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	CreateList(game->teams);
	
	Element league_ptr;
	Element team_ptr;
	
	FOR_EACH(league_ptr, game->leagues){
		FOR_EACH(team_ptr, ((League)league_ptr)->teams){
			AddToList((void *) team_ptr, game->teams);
		}
	}

	game->loggedUsers = (List)malloc(sizeof(llist));
		if(game->loggedUsers == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	CreateList(game->loggedUsers);

	
	return game;
}

int main(void){

	League league = loadLeague("../res/leagues/1/");

	printf("id: %d\n",league->ID);
	printf("name: %s\n",league->name);
	printf("status %d\n", league->status);
	printf("cant jugadores disponibles %d\n", league->cantAvailablePlayers);
	
	Element leagueID_ptr;

	FOR_EACH(leagueID_ptr,league->availablePlayers){
		printf("player%s\n points: %d\n",((Player)(leagueID_ptr->data))->name,((Player)(leagueID_ptr->data))->points);
	}
	


	return 0;
	
}
