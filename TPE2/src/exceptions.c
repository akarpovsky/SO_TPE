#include "../include/io.h"
#include "../include/defs.h"
#include "../include/exceptions.h"

unsigned char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void exception_handler(int exception_num)
{
    _Cli();
    if (exception_num < 32)
    {
        printfcolor(ERROR_COLOR,"%s", exception_messages[exception_num]);
        printfcolor(ERROR_COLOR," Exception!\n\tSystem failure, please reboot.\n");
    }

    if (exception_num == 13)
       {
    	int offset = 80*9;
    	int r;
    	char ticks_color = 0x0E;
    	char * video = (char *) 0xb8000;
       }

    _debug();
    _Sti();
}
