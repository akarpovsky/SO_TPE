#include "../include/io.h"
#include "../include/defs.h"
#include "../include/exceptions.h"

int nestexc = 0;

unsigned char *exception_messages[] = { "Division By Zero", "Debug",
		"Non Maskable Interrupt", "Breakpoint", "Into Detected Overflow",
		"Out of Bounds", "Invalid Opcode", "No Coprocessor",

		"Double Fault", "Coprocessor Segment Overrun", "Bad TSS",
		"Segment Not Present", "Stack Fault", "General Protection Fault",
		"Page Fault", "Unknown Interrupt",

		"Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved",

		"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
		"Reserved", "Reserved" };

void exception_handler(int exception_num) {
	_Cli();
	if (exception_num < 32) {
		printfcolor(ERROR_COLOR, "%s exception!\n",
				exception_messages[exception_num]);
	}

	Task_t * exceptionProducer = (Task_t *) get_current_task();
	char * exceptionProducerName = exceptionProducer->name;
	printf("Exception producer %s\n", exceptionProducer->name);

	if (nestexc > MAX_NESTED_EXCEPTIONS) {
		printfcolor(ERROR_COLOR,
				"PANIC: Exception could not be solved!\n\tSystem failure, please reboot.\n");
		_debug();
	}
	nestexc++;

	if (terminate_task(exceptionProducer->pid) == EXIT_SUCCESS) {
		select_next();
		printfcolor(COMMAND_COLOR, "Failure tolerant module: Process \"%s\" has been killed.\n", exceptionProducerName);
	}else{
		printfcolor(ERROR_COLOR,
				"PANIC 2: Exception could not be solved!\n\tSystem failure, please reboot.\n");
		_debug();
	}

	nestexc--;
	_Sti();
}
