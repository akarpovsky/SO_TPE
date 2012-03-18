#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>

#include <sys/param.h>


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
	

	User auxUser; // Will store the loaded user
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num, auxNum;
	
	auxUser = malloc(sizeof(user));
	
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
	auxUser->user  = (char *) malloc(sizeof(dim));
	if(auxUser->user == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->user,aux);
	auxUser->user[dim-1] = '\0';
	
	
	/* Load password */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->pass  = (char *)  malloc(sizeof(dim));
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
	auxUser->leagueIDs = malloc((num) * sizeof(int));
	if(auxUser->leagueIDs == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}

	/* Cargo IDs en el vector de IDs anteriormente malloqueado */
	int i;
	for(i = 0; i < num; i++){
		fgets(aux,BUFFER_SIZE,file);
		auxNum = atoi(aux);
		(auxUser->leagueIDs)[i] = auxNum;
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
	auxTrade->userFrom  = (char *) malloc(sizeof(dim));
	if(auxTrade->userFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->userFrom,aux);
	auxTrade->userFrom[dim-1] = '\0';
	
	/* Load userTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->userTo  = (char *) malloc(sizeof(dim));
	if(auxTrade->userTo == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->userTo,aux);
	auxTrade->userTo[dim-1] = '\0';
	
	/* Load playerFrom */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerFrom  = (char *) malloc(sizeof(dim));
	if(auxTrade->playerFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxTrade->playerFrom,aux);
	auxTrade->playerFrom[dim-1] = '\0';
	
	/* Load playerTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerTo  = (char *) malloc(sizeof(dim));
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
		auxPlayer->name  = (char *) malloc(sizeof(dim));
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

int fileType(char * path, char * file){
	int status;
	struct stat st_buf;
	
	char * filePath = (char *) calloc(BUFFER_SIZE, sizeof(char));
	
	strcat(filePath, path);
	strcat(filePath, file);

	// Get the status of the file system object.

	status = stat(filePath, &st_buf);
	if (status != 0) {
		perror("<LOG - io.c>\n\tError loading stat.\n<end>\n");
		return -1;
	}
	
	
	if (S_ISREG (st_buf.st_mode)) {
		free(filePath);
		return FILE_TYPE;
	}
	
	if (S_ISDIR (st_buf.st_mode)) {
		free(filePath);
		return DIR_TYPE;
	}
	
	return UNKNOWN_TYPE;
}

User loadUsers(char * path){

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
				int type = fileType(path, entry->d_name);
				if( type == FILE_TYPE)
					printf ("Archivo: %s\n", entry->d_name);
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

int main(void){
/*	
	int i;
	
	Team vicky = loadTeam("../res/leagues/1/teams/alan.team");	
	printf("<LOG - io.c>\n\t teamID: %d \n\t teamPoints: %d \n\tcantPlayers: %d\n<LOG>",vicky->ID,vicky->points,vicky->cantPlayers);
	
	for(i = 0; i < vicky->cantPlayers; i++){
		
		printf("jugador: %s \n\t puntos: %d \n\t", vicky->players[i]->name,vicky->players[i]->points);
	}
*/	
//	objToFile(USER_OBJ, vicky);
	loadUsers("./");
//	printf("Prueba obkToFile: %s)
	return 0;
}
