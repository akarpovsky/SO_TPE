#include <stdio.h>
#include <string.h>

void parseCommand(char * cmd);
void list(char * param);

int main(void){
	char comando[] = "leagueShow[1234]";
	parseCommand(comando);

	return 0;	
}

void parseCommand(char * cmd){
	
	char * commandName;
	char * param;
	char * user;
	char * pass;
	int id;
	
	if ((commandName = strtok(cmd,"[")) == NULL){
		printf("Command {%s} not found \n", cmd);
		return;
	}
	
	if((strcmp(commandName,"login") != 0) && (strcmp(commandName,"register") != 0)){
		if((param = strtok(NULL,"\n")) == NULL){
			printf("Command {%s} not found \n", cmd);
			return;
		}
		if( (param[strlen(param) -1 ]) != ']'){
			printf("Command {%s} not found \n", cmd);
			return;
		}
		
		param[strlen(param)-1] = 0;

	}else{

		if((user = strtok(NULL,",")) == NULL){
			printf("Command {%s} not found \n", cmd);
			return;
		}

		if((pass = strtok(NULL,"\n")) == NULL){
			printf("Command {%s} not found \n", cmd);
			return;
		}

		if( (pass[strlen(pass) -1 ]) != ']'){
			printf("Command {%s} not found \n", cmd);
			return;
		}
		
		pass[strlen(pass)-1] = 0;
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
		id = atoi(param);
		if(id <= 0){
			printf("Incorrect ID \n");
			return;
		}else if (strcmp(commandName,"leagueShow") == 0){
			// SEND leagueShow(id);
		}else if(strcmp(commandName,"teamShow") == 0){
			// SEND teamShow(id);
		}else if(strcmp(commandName,"tradeShow") == 0){
			// SEND tradeShow(id);
		}else if(strcmp(commandName,"trade") == 0){
			// SEND trade(id);
		}else if(strcmp(commandName,"tradeWithdraw") == 0){
			// SEND tradeWithdraw(id);		
		}else if(strcmp(commandName,"tradeAccept") == 0){
			// SEND tradeAccept(id);
		}else if(strcmp(commandName,"tradeNegotiate") == 0){
			// SEND tradeNegotiate(id);
		}else{
			printf("Command {%s} not found \n", cmd);
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
		printf("Command {list[%s]} not found \n",param);
	}
	
}