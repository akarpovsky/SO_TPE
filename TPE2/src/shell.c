#include "../include/shell.h"


#define NUM_COMMANDS 10
#define LINEBUF_LEN 10

typedef struct command_t {
	char name[LINEBUF_LEN];
	char args[LINEBUF_LEN - 2];

} command_t;

typedef struct hola{
	char name[20];

} hola_t;


int printing_command = FALSE;

typedef void(*commandFnct)(void);

static struct {
    char* name;
    char* description;
    PROCESS task_function;
} commands[NUM_COMMANDS] =  {
		{"help", "Display system commands", help},
		{"echo", "Prints string", echo},
		{"mouse", "Display information about the mouse usage", mouse},
		{"shortcuts", "Display keyboard shorcuts", shortcuts},
		{"top", "Display ongoing look at processor activity in real time.", top},
		{"kill PIDn", "Kills the proces with PID = PIDn .", pkill},
		{"imprimeUnos", "Process that prints 1s forever", imprimeUnos},
		{"invOpcode", "Tries to excecute an invalid Operation Code", invalidOpCode},
		{"divideByZero", "Tries to perform a division by zero", divideByZero},
		{"clearScreen", "Erase all the content of the actual TTY", clear_screen}
	};

void shell(void){


	Task_t * c_t = get_current_task();
	ttyScreen_t * screen = getScreen(c_t);
	shellLine_t * lineBuffer = getLineBuffer(c_t);
	char c;
	command_t  *a = malloc(sizeof(command_t));

	printf("BrunOS tty%d:~$ ", c_t->tty_number);
	while( (c=getc()) != '\n' ){
		switch(c){
		case '\b':
			if( lineBuffer->pos>0){

				lineBuffer->buffer[--lineBuffer->pos]=0;
				putc('\b');
			}
			break;
		case '\t':
			parse_command(a);
			auto_complete(a);
			break;
		default:
			if(lineBuffer->pos < lineBuffer->size-1){
				lineBuffer->buffer[lineBuffer->pos++]=c;
				lineBuffer->buffer[lineBuffer->pos]=0;
			}
			putc(c);
			break;
		}
	}

	putc('\n');

	parse_command(a);

	run_command(a);
	lineBuffer->pos=0;
	clearCommand(a);
	erase_buffer();
	free(a);

}

void erase_buffer(){
	shellLine_t * lineBuffer = getLineBuffer(get_current_task());
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++)
	{
		lineBuffer->buffer[i]=0;
	}

}

void parse_command(command_t * c) {

    shellLine_t * lineBuffer = getLineBuffer(get_current_task());

    int initpos = 0;

    /* Remover espacios en blanco */
    while ( (lineBuffer->buffer[initpos] == ' ') && (++initpos < LINEBUF_LEN - 1) );
    sscanf(&lineBuffer->buffer[initpos], "%s %s", c->name, c->args);
}

void StartNewTask(char * name, PROCESS new_task_function, char * args, bool isBackground){

	atomize();
	Task_t * auxTask = NULL;
	Task_t * c_t = get_current_task();
	Task_t * fg_t = get_foreground_tty();

	int new_task_priority = c_t->priority; // La prioridad del proceso shell serï¿½ = 1


	void * stack_start_address = getFreePage()+MAX_PAGE_SIZE-1; // Me devuelve una nueva pï¿½gina vacï¿½a con el "PID de kernel"

	_debug();
	//TODO:
	if(stack_start_address == NULL){
		while(1){
		kprintf("s");
		}
	}
	auxTask = CreateProcess(name, new_task_function, c_t, c_t->tty_number, 1, &args,
				stack_start_address, new_task_priority, !isBackground);

	/* Cambio el PID de la pï¿½gina del stack que me devolviï¿½ getFreePage() ya que previo a la creaciï¿½n del
	 * proceso, éste no tenï¿½a ningï¿½n PID asignado y esa pï¿½gina contenï¿½a un PID invï¿½lido.
	 * Luego de este cambio la pï¿½gina serï¿½ accesible por y solo por el proceso que acabamos de crear.
	 */
//	changePagePID(auxTask->pid, stack_start_address);

	unatomize();


}

int run_command(command_t * command){

	int i;
	if(streq(command->name, ""))
		return 0;

	int len = strlen(command->name);

	bool isBackground = (command->name[len-1] == '&')? true:false;
	if(isBackground)
	{
		command->name[len-1] = '\0';
	}

	for (i = 0; i < NUM_COMMANDS; i++) {
			if (streq(command->name, commands[i].name))
			{
				StartNewTask(commands[i].name, commands[i].task_function, command->args, isBackground);
			}
	}
	printfcolor(ERROR_COLOR ,"%s: command not found\n", command->name);
	clearCommand(command);
	return 1;
}

void auto_complete(command_t *command){

	Task_t * c_t = get_current_task();
	ttyScreen_t * screen = getScreen(c_t);
	shellLine_t * lineBuffer = getLineBuffer(c_t);

	int i, j, size, lenght, eq = TRUE;
	lenght = strlen(command->name);
	char * commName;
	if (streq(command->name, ""))
		return;

	for (i = 0; i < NUM_COMMANDS; i++) {
		commName = commands[i].name;
		for (j = 0; j < lenght && eq == TRUE; j++) {
			if (command->name[j] != commName[j])
				eq = FALSE;
			if (j == strlen(commName) - 1)
				eq = FALSE;
		}
		if (eq == TRUE) {
			size = strlen(lineBuffer->buffer);
			erase_buffer();
			clearfromto(screen->wpos - size * 2,
					screen->wpos);
			lineBuffer->pos = 0;
			lenght = strlen(commName);
			for (j = 0; j < lenght; j++) {
				lineBuffer->buffer[lineBuffer->pos++]
						= commName[j];
				lineBuffer->buffer[lineBuffer->pos]
						= 0;
				putc(commName[j]);

			}
		}
		eq = !eq;
	}
	command->name[0] = 0;

}

void clearCommand(command_t * command){
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++)
	{
		command->name[i]=0;
		command->args[i]=0;
	}
}

int divideByZero(int argc, char **argv){
//	clearCommand(NULL);
	erase_buffer();
	int x = 1;
	int y = 1;
	int z = x / --y;
}

int clear_screen(int argc, char **argv){

	ttyScreen_t * screen = getScreen(get_current_task());

	clearScreen();
	clearScreenBuffer();
	print_header();
	printTicks();
	screen->wpos=TTY_SCREEN_SSTART;
	move_cursor(screen->wpos/2);

}

int invalidOpCode(int argc, char **argv){
	_invop();
}

int echo(int argc, char **argv){
	//TODO: to do
}

int mouse(int argc, char **argv){
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

int shortcuts(int argc, char **argv){
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



int help(int argc, char **argv){
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

int top(int argc, char **argv){

}

int pkill(int argc, char **argv){

}

int imprimeUnos(int argc, char **argv){

}
