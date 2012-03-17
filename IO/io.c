#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"


#define BUFFER_SIZE 100

#define USER_OBJ 1
#define LEAGUE_OBJ 2
#define TEAM_OBJ 3
#define TRADE_OBJ 4

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
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	file = fopen(path,"r");
	
	if(file == NULL){
		perror("File not found\n");
		exit(EXIT_FAILURE);
	}
	
	/* Load username */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->user  = (char *) malloc(sizeof(dim));
	if(auxUser->user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->user,aux);
	auxUser->user[dim-1] = '\0';
	
	
	/* Load password */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->pass  = (char *)  malloc(sizeof(dim));
	if(auxUser->pass == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->pass,aux);
	auxUser->pass[dim-1] = '\0';
	
	
	/* Load number of Leagues */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxUser->leagues = num;
	auxUser->leagueIDs = malloc((num-1) * sizeof(int));
	if(auxUser->leagueIDs == NULL){
		perror("Insufficient memory\n");
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
	
	User vicky = loadUser("../res/users/Vicky.user");	
	printf("<LOG - io.c>\n\tCargue al usuario: %s\n\tTene password: %s\n\tCantidad de ligas: %d\n<end>\n", vicky->user, vicky->pass, vicky->leagues);
	objToFile(USER_OBJ, vicky);
//	printf("Prueba obkToFile: %s)
	return 0;
}