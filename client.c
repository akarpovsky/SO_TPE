/**
	Client.c

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getnum.h"
#include "communication_c.h"

#include "defines.h"
#include "structs.h"


//TODO: Sacar
#include "server.h"

int register_f(void){
	int ret = 0;
	char * username;
	char * password;

//TODO: Revisar espacios dentro del username y errores scanf

		printf("\n\t\t*********************************\n");
		printf("\t\t*********** REGISTER ************\n");
		printf("\t\t*********************************\n\n");


	printf("Select a username: \n");
	scanf("%ms", &username);

	printf("Enter your password: \n");
	scanf("%ms", &password);

	printf("User: %s, Pass: %s\n", username, password);	
	ret = register_c(username, password);

	return ret;
}

int register_c(char * username, char * password){

	//Validations
	if( strcmp(username, "") == 0 || strcmp(password, "") == 0 ){
		return BLANK_ERR;	
	}
	
	if( strlen(username) > 100 ){
		return LENG_ERR;
	}

	return SUCCESSFUL;
	//return communicate(PIPES, REGISTER, username, pass); 
}


int login_f(void){
	int ret = 0;
	char * username;
	char * password;

//TODO: Revisar espacios dentro del username y errores scanf


	printf("\n\t\t*********************************\n");
	printf("\t\t************ LOG IN *************\n");
	printf("\t\t*********************************\n\n");

	printf("Enter your username: \n");
	scanf("%ms", &username);

	printf("Enter your password: \n");
	scanf("%ms", &password);

	printf("User: %s, Pass: %s\n", username, password);	
	ret = login_c(username, password);

	return ret;
}




int login_c(char * username, char * password){
	communicate(LOG_IN, username, password); 
	return 1;
}

int show_main_menu(void){

	int option = 0;

	do{

		printf("\n\t\t*********************************\n");
		printf("\t\t*********************************\n");
		printf("\t\t*********** MAIN MENU ***********\n");
		printf("\t\t*********************************\n");
		printf("\t\t*********************************\n\n");
		
		option = getint("Chose an option:\n\t1)Register\n\t2)Log In\n\t3)Exit\n\n");

		if(option < 1 || option > 3){
			printf("Error: Please enter a valid option.\n");
		}

	}while(option < 1 || option > 3);
		
	return option;	

}

int main(void){
	
	
	
	int option = 0;
	int register_ret = 0;
	int login_ret = 0;
	
	do{
		option = show_main_menu();

		switch(option){

			case REGISTER:
				
				register_ret = 0;

				do{

					register_ret = register_f();

				}while(register_ret != SUCCESSFUL);
	
				printf("\nUser succesfully created\n.");

				break;

			case LOG_IN:

				login_ret = 0;

				do{

					login_ret = login_f();

				}while(login_ret != SUCCESSFUL);

				printf("Welcome!!\n.");
	
				//startgame();

				break;
			}

	
	}while(option != EXIT);
	

	return EXIT_SUCCESS;
}




