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
	auxString  =  malloc(sizeof(dim));
	if(auxString == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	auxUser -> user = auxString;
	strcpy(auxUser->user,aux);
	auxUser->user[dim-1] = '\0';
//	printf("%s\n",auxUser->user);
	
	
	/* CARGO PASSWORD */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	printf("%d\n",dim);
	auxString  =  malloc(sizeof(dim));
	if(auxString == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	auxUser -> pass = auxString;
	strcpy(auxUser->pass,aux);
	auxUser->pass[dim-1] = '\0';
	
	
	/* CARGO CANT DE LEAGUES */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxUser->leagues = num;
	
	auxInt= malloc(sizeof(num) / sizeof(int));
	if(auxInt == NULL){
		perror("Insufficient memory\n");
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
	printf("%s\n",vicky->user);
	
	printf("<LOG - io.c>\n\tCargue al usuario: %s\n\tTene password: %s\n\tCantidad de ligas: %d\n<end>\n", vicky->user, vicky->pass, vicky->leagues);
	return 0;
}