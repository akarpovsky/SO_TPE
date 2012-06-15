#include "../include/shell.h"

extern size_t my_offset;

int printing_command = FALSE;

typedef void(*commandFnct)(void);

static struct {
	char* name;
	char* description;
	PROCESS task_function;
} commands[NUM_COMMANDS] = { { "help", "Display system commands", help }, {
		"echo", "Prints string", echo }, { "shortcuts",
		"Display keyboard shorcuts", shortcuts }, { "top",
		"Display ongoing look at processor activity in real time.", top }, {
		"kill", "Kills the proces with PID = PIDn .", pkill }, { "imprimeUnos",
		"Process that prints 1s forever", imprimeUnos }, { "clearScreen",
		"Erase all the content of the actual TTY", clear_screen }, { "ls",
		"Show files in CWD", ls },
		{ "mkdir", "Creates a new directory", mkdir }, { "touch",
				"Creates a new empty file", touch }, { "cd",
				"Change directory", cd }, { "rm", "Delete fileA", rm }
//		{"mv", "Move fileA to another place", mv},
		//		{"cp", "Copy fileA to destination", cp},
		};

void shell(void) {

	Task_t * c_t = get_current_task();
	ttyScreen_t * screen = getScreen(c_t);
	shellLine_t * lineBuffer = getLineBuffer(c_t);
	char c;
	command_t * a = (command_t *) malloc(sizeof(command_t));

	printf("BrunOS tty%d:~$ ", c_t->tty_number);
	while ((c = getc()) != '\n') {
		switch (c) {
		case '\b':
			if (lineBuffer->pos > 0) {

				lineBuffer->buffer[--lineBuffer->pos] = 0;
				putc('\b');
			}
			break;
		case '\t':
			parse_command(a);
			auto_complete(a);
			break;
		default:
			if (lineBuffer->pos < lineBuffer->size - 1) {
				lineBuffer->buffer[lineBuffer->pos++] = c;
				lineBuffer->buffer[lineBuffer->pos] = 0;
			}
			putc(c);
			break;
		}
	}

	putc('\n');

	parse_command(a);

	run_command(a);
	lineBuffer->pos = 0;
	erase_buffer();

}

void erase_buffer() {
	shellLine_t * lineBuffer = getLineBuffer(get_current_task());
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++) {
		lineBuffer->buffer[i] = 0;
	}

}

void parse_command(command_t * c) {

	shellLine_t * lineBuffer = getLineBuffer(get_current_task());

	int initpos = 0;

	/* Remover espacios en blanco */
	while ((lineBuffer->buffer[initpos] == ' ')
			&& (++initpos < LINEBUF_LEN - 1))
		;
	sscanf(&lineBuffer->buffer[initpos], "%s %s", c->name, c->args);
}

int run_command(command_t * command) {

	int i;
	if (streq(command->name, ""))
		return 0;

	int len = strlen(command->name);

	bool isBackground = (command->name[len - 1] == '&') ? true : false;
	if (isBackground) {
		command->name[len - 1] = '\0';
	}

	for (i = 0; i < NUM_COMMANDS; i++) {
		if (streq(command->name, commands[i].name)) {
			StartNewTask(commands[i].name, commands[i].task_function,
					command->args, isBackground);
			return 1;
		}
	}
	printfcolor(ERROR_COLOR, "%s: command not found\n", command->name);
	clearCommand(command);
	return 1;
}

void auto_complete(command_t *command) {

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
			clearfromto(screen->wpos - size * 2, screen->wpos);
			lineBuffer->pos = 0;
			lenght = strlen(commName);
			for (j = 0; j < lenght; j++) {
				lineBuffer->buffer[lineBuffer->pos++] = commName[j];
				lineBuffer->buffer[lineBuffer->pos] = 0;
				putc(commName[j]);

			}
		}
		eq = !eq;
	}
	command->name[0] = 0;

}

void clearCommand(command_t * command) {
	int i = 0;
	for (i = 0; i < LINEBUF_LEN; i++) {
		command->name[i] = 0;
		command->args[i] = 0;
	}
}

int divideByZero(int argc, char *argv) {
	erase_buffer();
	int x = 1;
	int y = 1;
	int z = x / --y;
}

int clear_screen(int argc, char *argv) {

	ttyScreen_t * screen = getScreen(get_current_task());

	clearScreen();
	clearScreenBuffer();
	print_header();
	printTicks();
	screen->wpos = TTY_SCREEN_SSTART;
	move_cursor(screen->wpos / 2);

}

int invalidOpCode(int argc, char *argv) {
	_invop();
	return EXIT_SUCCESS;

}

int echo(int argc, char *argv) {
	printfcolor(COMMAND_COLOR, "%s\n", argv);
	return EXIT_SUCCESS;

}

int mouse(int argc, char *argv) {
	printfcolor(
			MARINE_COLOR,
			"********************************************************************************\n");
	printfcolor(COMMAND_COLOR, "Mouse usage: \n\n");
	printfcolor(ERROR_COLOR, "%s", commands[0].name);

	printfcolor(ERROR_COLOR, "Left Click");
	printfcolor(MARINE_COLOR, "\t\t => \t");
	printfcolor(COMMAND_COLOR, "Increments the timer tick frecuency\n");

	printfcolor(ERROR_COLOR, "Right Click");
	printfcolor(MARINE_COLOR, "\t\t => \t");
	printfcolor(COMMAND_COLOR, "Decrements the timer tick frecuency\n");

	printfcolor(
			MARINE_COLOR,
			"\n********************************************************************************\n");
	return EXIT_SUCCESS;

}

int shortcuts(int argc, char *argv) {
	printfcolor(
			MARINE_COLOR,
			"********************************************************************************\n");
	printfcolor(COMMAND_COLOR, "Keyboard shortcuts: \n\n");

	printfcolor(ERROR_COLOR, "F1");
	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR, "Goes to TTY #1\n");

	printfcolor(ERROR_COLOR, "F2");
	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR, "Goes to TTY #2\n");

	printfcolor(ERROR_COLOR, "F3");
	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR, "Goes to TTY #3\n");

	printfcolor(ERROR_COLOR, "F4");
	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
	printfcolor(COMMAND_COLOR, "Goes to TTY #4\n");

	printfcolor(ERROR_COLOR, "text + TAB");
	printfcolor(MARINE_COLOR, "\t\t => \t");
	printfcolor(COMMAND_COLOR, "Autocomplete command\n");

	printfcolor(ERROR_COLOR, "CTRL + SHIFT");
	printfcolor(MARINE_COLOR, "\t => \t");
	printfcolor(COMMAND_COLOR, "Change keyboard language (ES | EN)\n");

	printfcolor(
			MARINE_COLOR,
			"\n********************************************************************************\n");
	return EXIT_SUCCESS;
}

int help(int argc, char *argv) {
	int i;
	printfcolor(
			MARINE_COLOR,
			"********************************************************************************\n");
	printfcolor(COMMAND_COLOR, "Available commands: \n\n");
	int len = 0;
	for (i = 0; i < NUM_COMMANDS; i++) {
		len = strlen(commands[i].name) / 4;

		printfcolor(ERROR_COLOR, "%s", commands[i].name);

		if (len < 1)
			printfcolor(MARINE_COLOR, "\t\t\t");
		else if (len < 2)
			printfcolor(MARINE_COLOR, "\t\t");
		else if (len < 3)
			printfcolor(MARINE_COLOR, "\t");

		printfcolor(MARINE_COLOR, " => \t");
		printfcolor(COMMAND_COLOR, "%s\n", commands[i].description);
	}

	printfcolor(
			MARINE_COLOR,
			"\n********************************************************************************\n");
	return EXIT_SUCCESS;

}

int top(int argc, char *argv) {
	Task_t* processes = get_processes();
	int i;
	printfcolor(
			MARINE_COLOR,
			"********************************************************************************\n");
	printfcolor(COMMAND_COLOR, "PID\t\t\tName\t\t\t\tStatus\t\t\tPriority\n\n");
	for (i = 0; i < MAX_PROCESSES; i++) {
		if (processes[i].state != TaskEmpty) {
			printfcolor(
					((processes[i].state == TaskReady) ? COMMAND_COLOR
							: (processes[i].state == TaskTerminated) ? ERROR_COLOR
									: MARINE_COLOR),
					"%d\t\t\t%s\t\t\t\t%s\t\t\t%d\n",
					processes[i].pid,
					processes[i].name,
					((processes[i].state == TaskReady) ? "Ready"
							: (processes[i].state == TaskTerminated) ? "Terminated"
									: "Suspended"), processes[i].priority);
		}
	}
	printfcolor(
			MARINE_COLOR,
			"\n********************************************************************************\n");
	return EXIT_SUCCESS;
}

int pkill(int argc, char *argv) {
	atomize();
	int pid = atoi(argv);
	if (terminate_task(pid) == EXIT_SUCCESS) {
		printfcolor(COMMAND_COLOR, "PID: %d has been successfully killed.\n",
				pid);
	} else {
		printfcolor(ERROR_COLOR, "PID: %d could not be killed.\n", pid);
	}
	unatomize();
	return EXIT_SUCCESS;
}

int imprimeUnos(int argc, char *argv) {

	_Sti();
	while (1) {
		printf("1");
	}
	return EXIT_SUCCESS;
}

int imprimeDos(int argc, char *argv) {

	_Sti();
	while (1) {
		printf("2");
	}
	return EXIT_SUCCESS;
}

int pagefault(int argc, char * argv) {
	return pagefault(argc, argv);
}

int ls(int argc, char *argv) {
	inode_t * cwd_inode = get_current_task()->cwd;
	fileentry_t * currentFile = NULL;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL) {
		if (currentFile->state != ABSENT) {

			if (currentFile->type == DIR_TYPE) {
				printfcolor(DIR_COLOR, "%s ", currentFile->name);
			} else if (currentFile->type == FILE_TYPE) {
				printfcolor(FILE_COLOR, "%s ", currentFile->name);
			} else if (currentFile->type == LINK_TYPE) {
				printfcolor(LINK_COLOR, "%s ", currentFile->name);
			}
		}

		free(currentFile);
	}

	printf("\n");

}

int mkdir(int argc, char *argv) {
	int ret = fsMkdir(argv, get_current_task()->cwd);

	switch (ret) {
	case NOT_ENOUGH_SPACE_IN_DIR:
		printfcolor(ERROR_COLOR, "ERROR: Directory is full.\n");
		break;

	case NO_SPACE:
		printfcolor(ERROR_COLOR, "ERROR: Disk is full.\n");
		break;

	case NO_FILENAME:
		printfcolor(ERROR_COLOR, "ERROR: Invalid directory name.\n");
		break;

	case FILE_ALREADY_EXISTS:
		printfcolor(ERROR_COLOR, "ERROR: Directory already exists.\n");
		break;
	}
}

int touch(int argc, char *argv) {
	int ret = fsMkfile(argv, get_current_task()->cwd);

	switch (ret) {
	case NOT_ENOUGH_SPACE_IN_DIR:
		printfcolor(ERROR_COLOR, "ERROR: Directory is full.\n");
		break;

	case NO_SPACE:
		printfcolor(ERROR_COLOR, "ERROR: Disk is full.\n");
		break;

	case NO_FILENAME:
		printfcolor(ERROR_COLOR, "ERROR: Invalid filename.\n");
		break;

	case FILE_ALREADY_EXISTS:
		printfcolor(ERROR_COLOR, "ERROR: File already exists.\n");
		break;
	}

}

int cd(int argc, char *argv) {

	inode_t ** cwd_inode = &(get_current_task()->parent->cwd);
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(*cwd_inode, i++))
			!= NULL && !found) {
		if (currentFile->type == DIR_TYPE && streq(argv, currentFile->name)
				== TRUE) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) { // Change the directory of the parent of current_task
		inode_t * aux = (inode_t *) getInodeByNumber(currentFile->inode_number);
		if (aux != NULL)
			(*cwd_inode) = aux;
		else {
			printfcolor(ERROR_COLOR, "FATAL: Disk corruption.\n");
		}

		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
	}
}

int rm(int argc, char *argv) {
	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL && !found) {
		if (currentFile->type == FILE_TYPE && streq(argv, currentFile->name)
				== TRUE) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		fsRemove(cwd_inode, currentFile);

		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
	}
}

