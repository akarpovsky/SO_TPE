/*
 * proc.h
 *
 */

#ifndef PROC_H_
#define PROC_H_

#include "limits.h"
#include "defs.h"
#include "structs.h"

typedef unsigned long Time_t;
typedef unsigned Prio_t;

typedef enum
{
	TaskSuspended,
	TaskReady,
	TaskCurrent,
	TaskDelaying,
	TaskWaiting,
	TaskSending,
	TaskReceiving,
	TaskTerminated,
	TaskNULL,
	TaskEmpty,
}
TaskState_t;

typedef enum{
	false,
	true
}bool;

typedef struct task Task_t;
typedef Task_t * Task;

typedef struct{
	char *			name;
	size_t			size;
	Task_t *		head;
	Task_t *		tail;
}TaskQueue_t;

typedef struct registers {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebp;
	unsigned int esp;
	unsigned int esi;
	unsigned int edi;
	unsigned short cs;
	unsigned short ss;
	unsigned short ds;
	unsigned short es;
	unsigned short fs;
	unsigned short gs;
	unsigned int flags;
	unsigned int eip;
}registers_t;

typedef struct stack_frame {
	void * EDI;
	void * ESI;
	void * EBP;
	void * ESP;
	void * EBX;
	void * EDX;
	void * ECX;
	void * EAX;
	void * EIP;
	void * CS;
	void * EFLAGS;
	void * fin_retaddr;
	int argc;
	char* argv;
} STACK_FRAME;

struct task {
	char 			name[PROCNAME_MAX];
	TaskState_t		state;
	Prio_t			priority;
	uint			pid;
	unsigned		atomic_level;
	int				pages[PAGES_MAX];
	char *			stack;
	void *			ss;
	void *			sp;
	Task_t *		parent;
	TaskQueue_t	*	queue;
	Task_t *		prev;
	Task_t *		next;
	bool			foreground;
	bool			success;
	bool			in_time_q;
	Task_t *		time_prev;
	Task_t *		time_next;
	Time_t			ticks;
	int				tty_number;
	ttyScreen_t *	screen;
	keyboard_t *	keyboard;
	shellLine_t	*	linebuffer;
	void *			msg;
	unsigned 		size;
	void *			descriptor_table[STREAM_MAX];
	registers_t		registers;
	TaskQueue_t 	send_queue;
};

typedef int (*PROCESS) (int argc, char *argv);

void add_to_queue(TaskQueue_t * q, Task_t * t);
Task_t *pop(TaskQueue_t * q);
bool empty(TaskQueue_t * q);
Task_t * remove_from_q(TaskQueue_t * q, Task_t * t);

#endif /* PROC_H_ */
