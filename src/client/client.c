#include <stdio.h>
#include <string.h>
#include <signal.h>


#include "../includes/client.h"
#include "../includes/marshalling.h"
#include "../includes/message.h"
#include "../includes/parseCommand.h"
#include "../includes/transport_c.h"

int main(void){
	
	char input[COMMAND_MAX_LENGTH];
	signal(SIGINT, sigint);

	connectToServer();

	while(1){
		printf("client:/$ ");
		readPrompt(input,sizeof(input));
		if(input[0] != 0){
			parseCommand(input);
		}
	}
}

void readPrompt(char * s, int size){
	
	int input = 0;

	while(--size && (input = getchar()) != EOF && input != '\n'){
		
		*s = input;
		s++;
	}
	
	if(input == EOF){
		printf("\n");
	}
	*s = 0;
}
