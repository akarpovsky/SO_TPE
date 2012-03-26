#include <stdio.h>
#include <string.h>


#include "../includes/client.h"
#include "../includes/message.h"
#include "../includes/parseCommand.h"
#include "../includes/fifo_c.h"




int main(void){
	
	char input[COMMAND_MAX_LENGHT];

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
