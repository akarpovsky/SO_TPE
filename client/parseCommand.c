#include <stdio.h>
#include <string.h>

#include "../includes/marshalling.h"
#include "../includes/message.h"
#include "../includes/parseCommand.h"


void parseCommand(char * cmd){
	
	char * commandName;
	char * param1;
	char * param2;
	char * param3;
	
	
	/* Cargo commandName */
	if ((commandName = strtok(cmd,"[")) == NULL){
		printf("Command not found \n");
		return;
	}
	
	/* Cargo param1 */
	if((strcmp(commandName,"login") == 0) || (strcmp(commandName,"register") == 0)
		|| (strcmp(commandName,"trade") == 0 ) || (strcmp(commandName,"tradeNegotiate") == 0)){
		if((param1 = strtok(NULL,",")) == NULL){
			printf("Command not found \n");
			return;
		}

	}else{

		if((param1 = strtok(NULL,"]")) == NULL){
			printf("Command not found \n");
			return;
		}
	}
	
	/* Cargo param2 */
	if((strcmp(commandName,"login") == 0) || (strcmp(commandName,"register") == 0)){

		if((param2 = strtok(NULL,"]")) == NULL){
			printf("Command not found \n");
			return;
		}

	}else{
		if((strcmp(commandName,"trade") == 0 ) || (strcmp(commandName,"tradeNegotiate") == 0)){
			if((param2 = strtok(NULL,",")) == NULL){
				printf("Command not found \n");
				return;
			}
			
			/* Cargo param3 */
			if((param3 = strtok(NULL,"]")) == NULL){
				printf("Command not found \n");
				return;
			}
		}
	} 
	
	/* Ahora en commandName hay un string con el nombre del comando
	* y en param 1, 2 y 3 hay user, pass, id, player from, player to, etc...*/
	
	if(strcmp(commandName,"register") == 0){
		register_c(param1,param2);
	}else if(strcmp(commandName,"login") == 0){
		login_c(param1,param2);
	}else if(strcmp(commandName,"list") == 0){
	
		if(strcmp(param1,"leagues") == 0){
			list_c(LIST_LEAGUES);
		}else if(strcmp(param1,"teams") == 0){
			list_c(LIST_TEAMS);
		}else if(strcmp(param1,"trades") == 0){
			list_c(LIST_TRADES);
		}else{
			printf("Command not found \n");
			return;
		}
		
	}else if (strcmp(commandName,"leagueShow") == 0){
		leagueShow_c(param1);
	}else if(strcmp(commandName,"teamShow") == 0){
		teamShow_c(param1);
	}else if(strcmp(commandName,"tradeShow") == 0){
		tradeShow_c(param1);
	}else if(strcmp(commandName,"trade") == 0){
		trade_c(param1,param2,param3);
	}else if(strcmp(commandName,"tradeWithdraw") == 0){
		tradeWithdraw_c(param1);		
	}else if(strcmp(commandName,"tradeAccept") == 0){
		tradeAccept_c(param1);
	}else if(strcmp(commandName,"tradeNegotiate") == 0){
		tradeNegotiate_c(param1,param2,param3);
	}else{
		printf("Command not found \n");
		return;
	}
	

	return;

}
