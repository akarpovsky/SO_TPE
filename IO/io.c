#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"


#define BUFFER_SIZE 100

User loadUser(char * path){
	
	FILE * file;
	char * auxString;
	int * auxInt;
	User auxUser;
	char aux[BUFFER_SIZE];
	int dim, num, auxNum;
	int i;
	
	auxUser = malloc(sizeof(user));
	
	if(auxUser == NULL){
		perror("Insuficient memory\n");
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
	auxString  =  malloc(sizeof(dim + 1) );
	if(auxString == NULL){
		perror("Insuficient memory\n");
		exit(EXIT_FAILURE);
	}
	auxUser -> user = auxString;
	for(i = 0; i < dim ; i++){
		(auxUser->user)[i] = aux[i];
	}
	(auxUser->user)[dim] = 0;
	
	/* CARGO PASSWORD */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxString =  malloc(sizeof(dim + 1) );
	if(auxString == NULL){
		perror("Insuficient memory\n");
		exit(EXIT_FAILURE);
	}
	auxUser -> pass = auxString;
	for(i = 0; i < dim ; i++){
		(auxUser->pass)[i] = aux[i];
	}
	(auxUser->pass)[dim] = 0;
	
	
	/* CARGO CANT DE LEAGUES */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxUser->leagues = num;
	
	auxInt= malloc(sizeof(num) / sizeof(int));
	if(auxInt == NULL){
		perror("Insuficient memory\n");
		exit(EXIT_FAILURE);
	}
	auxUser->leagueIDs = auxInt;

	/* Cargo IDs en el vector de IDs anteriormente malloqueado */
	for(i = 0; i < num; i++){
		fgets(aux,BUFFER_SIZE,file);
		auxNum = atoi(aux);
		(auxUser->leagueIDs)[i] = auxNum;
	}
	
	return auxUser;
}

int main(void){
	
	User vicky = loadUser("../res/users/Vicky.user");
	printf("Cargue al usuario %s que tiene password %s\n", vicky->user, vicky->pass);
	return 0;
}