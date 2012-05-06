#include "../include/shell.h"
#include "../include/strings.h"
#include "../include/video.h"

extern struct shellLine_t shellLine;
extern tty_t ttys[];

extern int actualTTY;

int printing_command = FALSE;
extern int printing_header;

#define NUM_COMMANDS 7
#define LINEBUF_LEN 100


typedef void(*commandFnct)(void);

static struct {
	char name[LINEBUF_LEN];
	char args[LINEBUF_LEN - 2];
	
} command;

static struct {
    char* name;
    char* description;
    commandFnct exec;
} commands[NUM_COMMANDS] =  {
		{"echo", "Prints string", echo},
		{"help", "Display system commands", help},
		{"mouse", "Display information about the mouse usage", mouse},		
		{"invOpcode", "Tries to excecute an invalid Operation Code", invalidOpcode},		
		{"shortcuts", "Display keyboard shorcuts", shortcuts},
		{"clearScreen", "Erase all the content of the actual TTY", clear_screen},		
		{"divideByZero", "Tries to perform a division by zero", divideByZero}

	};

void shell(){

	char c;
	printf("CasKarOS tty%d:~$ ",actualTTY+1);
	while( (c=getc()) != '\n' ){
		switch(c){
		case '\b':
			if( ttys[actualTTY].lineBuffer->pos>0){

				ttys[actualTTY].lineBuffer->buffer[--ttys[actualTTY].lineBuffer->pos]=0;
				putc('\b');
			}
			break;
		case '\t':
			parse_command();
			auto_complete();
			break;
		default:
			if(ttys[actualTTY].lineBuffer->pos < ttys[actualTTY].lineBuffer->size-1){
				ttys[actualTTY].lineBuffer->buffer[ttys[actualTTY].lineBuffer->pos++]=c;
				ttys[actualTTY].lineBuffer->buffer[ttys[actualTTY].lineBuffer->pos]=0;
			}
			putc(c);
			break;
		}
	}
	
	putc('\n');

	parse_command();


	run_command();
	ttys[actualTTY].lineBuffer->pos=0;
	clearCommand();
	erase_buffer();

}

void erase_buffer(){
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++)
	{
		ttys[actualTTY].lineBuffer->buffer[i]=0;
	}

}

void parse_command() {
    int initpos = 0;

    /* Remover espacios en blanco */
    while ( (ttys[actualTTY].lineBuffer->buffer[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(&ttys[actualTTY].lineBuffer->buffer[initpos], "%s %s", command.name, command.args);
}

int run_command(){

	int i;
	if(streq(command.name, ""))
		return 0;
		
	for (i = 0; i < NUM_COMMANDS; i++) {
	    if (streq(command.name, commands[i].name)) {
	        commands[i].exec();
		return 1;
	    }
	}
	printfcolor(ERROR_COLOR ,"%s: command not found\n", command.name);
	clearCommand();
	return 1;
}

void auto_complete(){
	int i,j,size,lenght, eq=TRUE;
	lenght=strlen(command.name);
	char * commName;
	if(streq(command.name, ""))
		return ;

	for (i = 0; i < NUM_COMMANDS; i++) {
	    commName=commands[i].name;
	    for(j=0; j<lenght && eq==TRUE;j++){
		if(command.name[j]!=commName[j])
		  eq=FALSE;
		if(j==strlen(commName)-1)
		  eq=FALSE;
	    }
	    if(eq==TRUE){
		size=strlen(ttys[actualTTY].lineBuffer->buffer);
		erase_buffer();
		clearfromto(ttys[actualTTY].screen->wpos-size*2,ttys[actualTTY].screen->wpos);
		ttys[actualTTY].lineBuffer->pos = 0;
		lenght=strlen(commName);
		for(j=0; j<lenght ; j++){
			ttys[actualTTY].lineBuffer->buffer[ttys[actualTTY].lineBuffer->pos++]=commName[j];
			ttys[actualTTY].lineBuffer->buffer[ttys[actualTTY].lineBuffer->pos]=0;
			putc(commName[j]);
			
		}
	    }
	    eq=!eq;
	}
	command.name[0]=0;

}

void clearCommand(){
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++)
	{
		command.name[i]=0;
		command.args[i]=0;
	}
}

static void
divideByZero(){
	clearCommand();
	erase_buffer();
	int x = 1;
	int y = 1;
	int z = x / --y;
}

static void
clear_screen(){
	clearScreen();
	clearScreenBuffer();
	print_header();
	printTicks();
	ttys[actualTTY].screen->wpos=TTY_SCREEN_SSTART;
	move_cursor(ttys[actualTTY].screen->wpos/2);
	
}

static void
invalidOpcode(){
	_invop();
}

static
void echo()
{
	char * text = command.args;
	printfcolor(COMMAND_COLOR,text);
	printf("\n");
}

static
void mouse() {
	printfcolor(MARINE_COLOR,"********************************************************************************\n");
	printfcolor(COMMAND_COLOR,"Mouse usage: \n\n");

	printfcolor(ERROR_COLOR,"Left Click");
	printfcolor(MARINE_COLOR,"\t\t => \t");
	printfcolor(COMMAND_COLOR,"Increments the timer tick frecuency\n");

	printfcolor(ERROR_COLOR,"Right Click");
	printfcolor(MARINE_COLOR,"\t\t => \t");
	printfcolor(COMMAND_COLOR,"Decrements the timer tick frecuency\n");

	printfcolor(MARINE_COLOR,"\n********************************************************************************\n");

}

static
void shortcuts() {
	printfcolor(MARINE_COLOR,"********************************************************************************\n");
	printfcolor(COMMAND_COLOR,"Keyboard shortcuts: \n\n");
    
	printfcolor(ERROR_COLOR,"F1");
	printfcolor(MARINE_COLOR,"\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR,"Goes to TTY #1\n");

	printfcolor(ERROR_COLOR,"F2");
	printfcolor(MARINE_COLOR,"\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR,"Goes to TTY #2\n");

	printfcolor(ERROR_COLOR,"F3");
	printfcolor(MARINE_COLOR,"\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR,"Goes to TTY #3\n");

	printfcolor(ERROR_COLOR,"F4");
	printfcolor(MARINE_COLOR,"\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR,"Goes to TTY #4\n");

	printfcolor(ERROR_COLOR,"text + TAB");
	printfcolor(MARINE_COLOR,"\t\t => \t");
	printfcolor(COMMAND_COLOR,"Autocomplete command\n");

	printfcolor(ERROR_COLOR,"CTRL + SHIFT");
	printfcolor(MARINE_COLOR,"\t => \t");
	printfcolor(COMMAND_COLOR,"Change keyboard language (ES | EN)\n");
		
	printfcolor(MARINE_COLOR,"\n********************************************************************************\n");
}



static
void help() {
    int i;
	printfcolor(MARINE_COLOR,"********************************************************************************\n");
	printfcolor(COMMAND_COLOR,"Available commands: \n\n");
    	int len = 0;
	for (i = 0; i < NUM_COMMANDS; i++){
		len = strlen(commands[i].name)/4;

		printfcolor(ERROR_COLOR,"%s", commands[i].name);

		if(len < 1)
			printfcolor(MARINE_COLOR,"\t\t\t");			
		else if(len < 2)
			printfcolor(MARINE_COLOR,"\t\t");			
		else if(len < 3)
			printfcolor(MARINE_COLOR,"\t");			


		printfcolor(MARINE_COLOR," => \t");
		printfcolor(COMMAND_COLOR,"%s\n", commands[i].description);
    }

	printfcolor(MARINE_COLOR,"\n********************************************************************************\n");
}
