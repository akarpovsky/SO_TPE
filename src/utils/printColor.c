
#include "./printColor.h"

void printYellowColor(Msg_s answer){

	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%d;%dm", 0x1B,1,33);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	

}

void printRedColor(Msg_s answer){

	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%d;%dm", 0x1B,1,31);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	

}

void printCyanColor(Msg_s answer){

	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%d;%dm", 0x1B,1,36);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	

}

void printGreenColor(Msg_s answer){

	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%d;%dm", 0x1B,1,32);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	

}

void printGreenBlink(Msg_s answer){

	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%d;%dm", 0x1B,5,32);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	

}

void releasePrintColor(Msg_s answer){
	
	char * toPrint = NULL;
	asprintf(&toPrint, "%c[%dm", 0x1B, 0);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}		
	AddToList(toPrint,answer->msgList);	
}
