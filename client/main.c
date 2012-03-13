#include <stdio.h>
#include "../includes/parse.h"

/* ARREGLAR CONTROL D */

void readPrompt(char * s, int size);

int main(void){
	
	char input[100];

	while(1){
		printf("leagueGame:/$ ");
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
	
	*s = 0;
	
}