/*
 * proc.h
 *
 */

#ifndef PROC_H_
#define PROC_H_

#include "strings.h"
#include "limits.h"
#include "shell.h"
#include "io.h"

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
	void * retaddr;
	int argc;
	char** argv;
} STACK_FRAME;

struct task {
	char 			name[PROCNAME_MAX];
	TaskState_t		state;
	Prio_t			priority;
	unsigned		atomic_level;
	char *			stack;
	unsigned short	ss;
	unsigned int	sp;
	void *			math_data;
	TaskQueue_t	*	queue;
	Task_t *		prev;
	Task_t *		next;
	bool			background;
	bool			success;
	bool			in_time_q;
	Task_t *		time_prev;
	Task_t *		time_next;
	Time_t			ticks;
	void *			msg;
	unsigned 		size;
	void *			descriptor_table[STREAM_MAX];
	registers_t		registers;
	TaskQueue_t 	send_queue;
	char			video[SCREEN_SIZE];
};

typedef int (*PROCESS) (int argc, char **argv);

int null_process(int argc, char **argv);
int shellLoop(int argc, char ** argv);
void add_to_queue(TaskQueue_t * q, Task_t * t);
Task_t *pop(TaskQueue_t * q);
bool empty(TaskQueue_t * q);
int proc1(int argc, char **argv);
int proc2(int argc, char **argv);
int proc3(int argc, char **argv);

#endif /* PROC_H_ */
