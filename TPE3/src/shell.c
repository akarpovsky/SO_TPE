#include "../include/shell.h"

extern size_t my_offset;

int printing_command = FALSE;

typedef void(*commandFnct)(void);

static struct {
	char* name;
	char* description;
	PROCESS task_function;
} commands[NUM_COMMANDS] = { { "help", "Display system commands", help }, {
		"echo", "Prints string", echo },
//		{ "shortcuts","Display keyboard shorcuts", shortcuts },
		{ "top","Display ongoing look at processor activity in real time.", top }, {
		"kill", "Kills the proces with PID = PIDn .", pkill }, { "imprimeUnos",
		"Process that prints 1s forever", imprimeUnos }, { "clearScreen",
		"Erase all the content of the actual TTY", clear_screen }, { "ls",
		"Show files in CWD", ls },
		{ "mkdir", "Creates a new directory", mkdir }, { "touch",
				"Creates a new empty file", touch }, { "cd",
				"Change directory", cd }, { "rm", "Delete fileA", rm }, {
				"lsRemoved", "Show all files (including deleted)", lsRemoved },
		{ "rmHard", "Remove the file the hard way", rmHard }, { "rmRec",
				"Removes files and directories recursively", rmRecursive }, {
				"vh", "Shows the version history of the file", vh },{
						"revert", "Revert to a previous version", revert },
						{"cp", "Copy files", cp },
						{"mv", "Move files", mv },
						{"ln", "Make a soft-link to a file", ln
						}

		};

void shell(void) {

	Task_t * c_t = get_current_task();
	ttyScreen_t * screen = getScreen(c_t);
	shellLine_t * lineBuffer = getLineBuffer(c_t);
	char c;
	command_t * a = (command_t *) malloc(sizeof(command_t));

	char * current_dir = getcwd();
	printf("BrunOS tty%d:%s~$ ", c_t->tty_number, current_dir);
	free(current_dir);
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

char * getcwd() {
	inode_t * pinode = get_current_task()->cwd->parent;
	fileentry_t * fe;
	int len = 0;
	char * ans;
	if (pinode != NULL) {
		int i = 0;
		while ((fe = fsGetFileentry(pinode, i++)) != NULL) {
			if (fe->inode_number == get_current_task()->cwd->inode_number)
				break;
			free(fe);
		}

		len = strlen(fe->name);
		ans = (char *) malloc(len + 1);
		strcpy(ans, fe->name);
		free(fe);
	} else {
		len = 1;
		ans = (char *) malloc(len + 1);
		strcpy(ans, "/");
	}

	return ans;
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

//int mouse(int argc, char *argv) {
//	printfcolor(
//			MARINE_COLOR,
//			"********************************************************************************\n");
//	printfcolor(COMMAND_COLOR, "Mouse usage: \n\n");
//	printfcolor(ERROR_COLOR, "%s", commands[0].name);
//
//	printfcolor(ERROR_COLOR, "Left Click");
//	printfcolor(MARINE_COLOR, "\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Increments the timer tick frecuency\n");
//
//	printfcolor(ERROR_COLOR, "Right Click");
//	printfcolor(MARINE_COLOR, "\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Decrements the timer tick frecuency\n");
//
//	printfcolor(
//			MARINE_COLOR,
//			"\n********************************************************************************\n");
//	return EXIT_SUCCESS;
//
//}

//int shortcuts(int argc, char *argv) {
//	printfcolor(
//			MARINE_COLOR,
//			"********************************************************************************\n");
//	printfcolor(COMMAND_COLOR, "Keyboard shortcuts: \n\n");
//
//	printfcolor(ERROR_COLOR, "F1");
//	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Goes to TTY #1\n");
//
//	printfcolor(ERROR_COLOR, "F2");
//	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Goes to TTY #2\n");
//
//	printfcolor(ERROR_COLOR, "F3");
//	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Goes to TTY #3\n");
//
//	printfcolor(ERROR_COLOR, "F4");
//	printfcolor(MARINE_COLOR, "\t\t\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Goes to TTY #4\n");
//
//	printfcolor(ERROR_COLOR, "text + TAB");
//	printfcolor(MARINE_COLOR, "\t\t => \t");
//	printfcolor(COMMAND_COLOR, "Autocomplete command\n");
//
//	printfcolor(ERROR_COLOR, "CTRL + SHIFT");
//	printfcolor(MARINE_COLOR, "\t => \t");
//	printfcolor(COMMAND_COLOR, "Change keyboard language (ES | EN)\n");
//
//	printfcolor(
//			MARINE_COLOR,
//			"\n********************************************************************************\n");
//	return EXIT_SUCCESS;
//}

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

//		printf("En LS: -%s-, state: %d, inode: %d, position: %d, type: %d\n", currentFile->name, currentFile->state, currentFile->inode_number, currentFile->position, currentFile->type);
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

int lsRemoved(int argc, char *argv) {
	inode_t * cwd_inode = get_current_task()->cwd;
	fileentry_t * currentFile = NULL;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL) {

//		printf("En LS: -%s-, state: %d, inode: %d\n", currentFile->name, currentFile->state, currentFile->inode_number);
		if (currentFile->state == ABSENT) {
			printfcolor(ERROR_COLOR, "(%s) ", currentFile->name);
		} else if (currentFile->type == DIR_TYPE) {
			printfcolor(DIR_COLOR, "%s ", currentFile->name);
		} else if (currentFile->type == FILE_TYPE) {
			printfcolor(FILE_COLOR, "%s ", currentFile->name);
		} else if (currentFile->type == LINK_TYPE) {
			printfcolor(LINK_COLOR, "%s ", currentFile->name);
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
				== TRUE && (currentFile->state == PRESENT)) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) { // Change the directory of the parent of current_task
		inode_t * aux = (inode_t *) getInodeByNumber(currentFile->inode_number);
		if (aux != NULL){
			(*cwd_inode) = aux;
		} else {
			printfcolor(ERROR_COLOR, "FATAL: Disk corruption.\n");
			return EXIT_FAILURE;
		}

		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
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
				== TRUE && (currentFile->state == PRESENT)) {
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
		return EXIT_FAILURE;
	}
}

int rmHard(int argc, char *argv) {
	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL && !found) {
		if (currentFile->type == FILE_TYPE && streq(argv, currentFile->name)
				== TRUE ) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		if(fsRemoveHard(cwd_inode, currentFile) == EXIT_FAILURE)
		{
			printfcolor(ERROR_COLOR, "ERROR: The file is being linked");
		}

		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}
}

int rmRecursive(int argc, char *argv) {

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL && !found) {
		if (streq(argv, currentFile->name) == TRUE && (currentFile->state == PRESENT)) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

//	printf("CFI:%d--CFP:%d\n", currentFile->inode_number, getInodeByNumber(currentFile->inode_number)->parent->inode_number);
//	printf("CFTy:%d\n", currentFile->type);
//	printf("CWD: %d--CWDP: %d\n", cwd_inode->inode_number, (cwd_inode->parent == NULL) ? -1: cwd_inode->parent->inode_number);
	if (found) {
		if (!(((currentFile->inode_number == cwd_inode->inode_number)
				&& (currentFile->type == DIR_TYPE)) || ((currentFile->inode_number
				== cwd_inode->parent->inode_number) && (currentFile->type
				== DIR_TYPE)))) {
			printf("e!\n");
			fsRecursiveRemoveHardWrapper(cwd_inode, currentFile);
		} else {
			printfcolor(ERROR_COLOR, "ERROR: You can't remove that.\n");
			return EXIT_FAILURE;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}

}

int vh(int argc, char *argv) {

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL && !found) {
		if (streq(argv, currentFile->name) == TRUE && currentFile->type
				== FILE_TYPE) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		if (!((currentFile->inode_number == cwd_inode->inode_number
				&& currentFile->type == DIR_TYPE) || (currentFile->inode_number
				== cwd_inode->parent->inode_number && currentFile->type
				== DIR_TYPE))) {
			inode_t * file = getInodeForEntry(currentFile);
			int current_rev = file->rev_no;
			printfcolor(
					MARINE_COLOR,
					"********************************************************************************\n");
			printfcolor(WHITE_TXT, "Revision\t\t\tName\t\t\tSize\t\t\tStatus\n");
			do{
				printfcolor((file->rev_no == current_rev) ? COMMAND_COLOR : ERROR_COLOR, "%d\t\t\t\t\t", file->rev_no);
				printfcolor((file->rev_no == current_rev) ? COMMAND_COLOR : ERROR_COLOR, "%s\t\t\t\t", file->name);
				printfcolor((file->rev_no == current_rev) ? COMMAND_COLOR : ERROR_COLOR, "%d\t\t\t\t", (file == NULL) ? -1: file->size );
				printfcolor((file->rev_no == current_rev) ? COMMAND_COLOR : ERROR_COLOR, "%s\n", (file->status == ABSENT) ? "Absent": "Present" );
			}while((file = fsGetPrevVersion(file)) != NULL);

			printfcolor(
					MARINE_COLOR,
					"\n********************************************************************************\n");

		} else {
			printfcolor(ERROR_COLOR, "ERROR: That file does not have version history.\n");
			return EXIT_FAILURE;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int revert(int argc, char *argv){

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int i = 0;
	int revision = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, i++))
			!= NULL && !found) {
		if (streq(argv, currentFile->name) == TRUE && currentFile->type
				== FILE_TYPE) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		if (!((currentFile->inode_number == cwd_inode->inode_number
				&& currentFile->type == DIR_TYPE) || (currentFile->inode_number
				== cwd_inode->parent->inode_number && currentFile->type
				== DIR_TYPE))) {
			revision = getInodeForEntry(currentFile)->rev_no-1;

			fsRevert(cwd_inode, currentFile, revision);
		} else {
			printfcolor(ERROR_COLOR, "ERROR: That file does not have version history.\n");
			return EXIT_FAILURE;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}

int cp(int argc, char *argv){
	char c;
	int i = 0;
	char param1[LINEBUF_LEN -2]; // File
	char param2[LINEBUF_LEN -2]; // Dest
	while((c=*argv++) != ' ' && c != '\0'){
		param1[i++] = c;
	}
	i = 0;
	while((c=*argv++) != ' ' && c != '\0'){
		param2[i++] = c;
	}

	if(param1[0] == '\0' || param2[0] == '\0'){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy: param1 = -%s-, param2=-%s-\n", param1, param2);
		return EXIT_FAILURE;
	}

	// Prevent recursive calls
	if(streq(param1, param2)){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy or recursive instruction attempt.\n");
		return EXIT_FAILURE;
	}

	/* First search for the file to copy */

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int j = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
			!= NULL && !found) {
		if (streq(param1, currentFile->name) == TRUE && (currentFile->state == PRESENT) && currentFile->type == FILE_TYPE ) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		/* Now search for the directory to copy the file */

		cwd_inode = get_current_task()->parent->cwd;
		fileentry_t * currentDirFile = NULL;
		found = FALSE;
		j = 0;
		while ((currentDirFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
				!= NULL && !found) {
			if (streq(param2, currentDirFile->name) == TRUE && currentDirFile->type
					== DIR_TYPE) {
				found = TRUE;
				break;
			}
			free(currentDirFile);
		}
		if (found) {
			fsCopy(getInodeForEntry(currentDirFile), currentFile);
			free(currentDirFile);
		} else {
			printfcolor(ERROR_COLOR, "ERROR: No such destination directory.\n");
			return EXIT_FAILURE;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
int mv(int argc, char *argv){
	char c;
	int i = 0;
	char param1[LINEBUF_LEN -2]; // File
	char param2[LINEBUF_LEN -2]; // Dest
	while((c=*argv++) != ' ' && c != '\0'){
		param1[i++] = c;
	}
	i = 0;
	while((c=*argv++) != ' ' && c != '\0'){
		param2[i++] = c;
	}

	if(param1[0] == '\0' || param2[0] == '\0'){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy: param1 = -%s-, param2=-%s-\n", param1, param2);
		return EXIT_FAILURE;
	}

	// Prevent recursive calls
	if(streq(param1, param2)){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy or recursive instruction attempt.\n");
		return EXIT_FAILURE;
	}

	/* First search for the file to copy */

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	bool found = FALSE;
	int j = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
			!= NULL && !found) {
		if (streq(param1, currentFile->name) == TRUE && (currentFile->state == PRESENT) && currentFile->type == FILE_TYPE ) {
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		/* Now search for the directory to copy the file */

		cwd_inode = get_current_task()->parent->cwd;
		fileentry_t * currentDirFile = NULL;
		found = FALSE;
		j = 0;
		while ((currentDirFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
				!= NULL && !found) {
			if (streq(param2, currentDirFile->name) == TRUE) {
				found = TRUE;
				break;
			}
			free(currentDirFile);
		}
		if (found) {
			if(currentDirFile->type == DIR_TYPE){
				fsCopy(getInodeForEntry(currentDirFile), currentFile);
				rmRecursive(1, param1);
				free(currentDirFile);
				return EXIT_SUCCESS;
			}else{
				printfcolor(ERROR_COLOR, "ERROR: Filename already exists.\n");
				free(currentDirFile);
				return EXIT_FAILURE;
			}
		} else {
			// Rename the file in the current directory
			fsRename(cwd_inode, currentFile, param2);
			return EXIT_SUCCESS;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}

int ln(int argc, char *argv){
	char c;
	int i = 0;
	char param1[LINEBUF_LEN -2]; // File
	char param2[LINEBUF_LEN -2]; // Dest
	while((c=*argv++) != ' ' && c != '\0'){
		param1[i++] = c;
	}
	i = 0;
	while((c=*argv++) != ' ' && c != '\0'){
		param2[i++] = c;
	}

	if(param1[0] == '\0' || param2[0] == '\0'){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy: param1 = -%s-, param2=-%s-\n", param1, param2);
		return EXIT_FAILURE;
	}

	// Prevent recursive calls
	if(streq(param1, param2)){
		printfcolor(ERROR_COLOR, "ERROR: Incorrect syntax for copy or recursive instruction attempt.\n");
		return EXIT_FAILURE;
	}

	/* First search for the file to copy */

	inode_t * cwd_inode = get_current_task()->parent->cwd;
	fileentry_t * currentFile = NULL;
	int ino_no = 0;
	bool found = FALSE;
	int j = 0;
	while ((currentFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
			!= NULL && !found) {
		if (streq(param1, currentFile->name) == TRUE && (currentFile->state == PRESENT)) {
			ino_no = currentFile->inode_number;
			found = TRUE;
			break;
		}
		free(currentFile);
	}

	if (found) {
		/* Now search for the directory to copy the file */

		cwd_inode = get_current_task()->parent->cwd;
		fileentry_t * currentDirFile = NULL;
		found = FALSE;
		j = 0;
		while ((currentDirFile = (fileentry_t *) fsGetFileentry(cwd_inode, j++))
				!= NULL && !found) {
			if (streq(param2, currentDirFile->name) == TRUE && currentDirFile->type
					== DIR_TYPE) {
				found = TRUE;
				break;
			}
			free(currentDirFile);
		}
		if (!found) {
			fsAddEntry(cwd_inode, getInodeByNumber(ino_no), param2);
			free(currentDirFile);
		} else {
			printfcolor(ERROR_COLOR, "ERROR: Already exists a file with such name.\n");
			return EXIT_FAILURE;
		}
		free(currentFile);
	} else {
		printfcolor(ERROR_COLOR, "ERROR: No such file or directory.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
