/*
 * scheduler.h
 *
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "../include/strings.h"
#include "../include/limits.h"
#include "../include/shell.h"
#include "../include/io.h"
#define MAX_PROCESSES 64
#define MAX_PRIORITIES 5
#define PRIORITY 0
#define TASK_NO 1
#define CURRENT_PROCESS active_processes[active_process[PRIORITY]][active_process[TASK_NO]]

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
	void *			data;
	Task_t *		from;
	void *			msg;
	unsigned 		size;
	registers_t		registers;
	TaskQueue_t 	send_queue;
	char			video[SCREEN_SIZE];
};

typedef int (*PROCESS) (int argc, char **argv);

void select_next(void);
int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront);
void CreateStackFrame(Task_t * new_proc, PROCESS p, unsigned int stack_start);
void SetupScheduler(void);
int null_process(int argc, char **argv);
short * getSSPointer();
int * getSPPointer();
int shellLoop(int argc, char ** argv);
void add_to_queue(TaskQueue_t * q, Task_t * t);
Task_t *pop(TaskQueue_t * q);
bool empty(TaskQueue_t * q);
int proc1(int argc, char **argv);
int proc2(int argc, char **argv);
int proc3(int argc, char **argv);

//XXX: made anew;

#define MAX_PROCESS	SHRT_MAX

#endif /* SCHEDULER_H_ */
