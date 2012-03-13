#include <stdio.h>
#include <string.h>
#include "../includes/parse.h"

void parseCommand(char * cmd){
	
	char * commandName;
	char * param;
	char * user;
	char * pass;
	int id;
	
	if ((commandName = strtok(cmd,"[")) == NULL){
		printf("Command not found \n");
		return;
	}
	
	if((strcmp(commandName,"login") != 0) && (strcmp(commandName,"register") != 0)){
		if((param = strtok(NULL,"]")) == NULL){
			printf("Command not found \n");
			return;
		}

	}else{

		if((user = strtok(NULL,",")) == NULL){
			printf("Command not found \n");
			return;
		}

		if((pass = strtok(NULL,"]")) == NULL){
			printf("Command not found \n");
			return;
		}
	}
	
	/* Ahora en commandName hay un string con el nombre del comando
	* en param user y pass estan los datos cargados*/
	
	if(strcmp(commandName,"register") == 0){
		// SEND register(user, pass);
	}else if(strcmp(commandName,"login") == 0){
		// SEND login(user,pass);
	}else if(strcmp(commandName,"list") == 0){
		list(param);
	}else{
		if (strcmp(commandName,"leagueShow") == 0){
			// SEND leagueShow(param);
		}else if(strcmp(commandName,"teamShow") == 0){
			// SEND teamShow(param);
		}else if(strcmp(commandName,"tradeShow") == 0){
			// SEND tradeShow(param);
		}else if(strcmp(commandName,"trade") == 0){
			// SEND trade(param);
		}else if(strcmp(commandName,"tradeWithdraw") == 0){
			// SEND tradeWithdraw(param);		
		}else if(strcmp(commandName,"tradeAccept") == 0){
			// SEND tradeAccept(param);
		}else if(strcmp(commandName,"tradeNegotiate") == 0){
			// SEND tradeNegotiate(param);
		}else{
			printf("Command not found \n");
		}
	}

	return;

}

void list(char * param){
	
	if(strcmp(param,"leagues") == 0){
	// SEND	listLeagues();
	}else if(strcmp(param,"teams") == 0){
	//SEND	listTeams();
	}else if(strcmp(param,"trades") == 0){
	//SEND	listTrades();
	}else{
		printf("Command not found \n");
	}
	
}