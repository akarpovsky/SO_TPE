#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"


#define BUFFER_SIZE 100

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
	
	/* CARGO USER */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->user  = (char *) malloc(sizeof(dim));
	if(auxUser->user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->user,aux);
	auxUser->user[dim-1] = '\0';
	
	
	/* CARGO PASSWORD */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->pass  = (char *)  malloc(sizeof(dim));
	if(auxUser->pass == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(auxUser->pass,aux);
	auxUser->pass[dim-1] = '\0';
	
	
	/* CARGO CANT DE LEAGUES */
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

int main(void){
	
	User vicky = loadUser("../res/users/Vicky.user");	
	printf("<LOG - io.c>\n\tCargue al usuario: %s\n\tTene password: %s\n\tCantidad de ligas: %d\n<end>\n", vicky->user, vicky->pass, vicky->leagues);
	return 0;
}