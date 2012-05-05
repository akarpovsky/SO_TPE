/*
 * structs.h
 *
 *  Created on: 23/04/2012
 */
#define MAX_PROCESS_NAME 125

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
	TaskEmpty
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
	char 			name[64];
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
	bool			success;
	bool			in_time_q;
	Task_t *		time_prev;
	Task_t *		time_next;
	Time_t			ticks;
	void *			data;
	Task_t *		from;
	void *			msg;
	unsigned 		size;
	registers_t		registers;
	TaskQueue_t 	send_queue;
};

typedef int (*PROCESS) (int argc, char **argv);

