
#ifndef _STRUCTS_H
#define _STRUCTS_H
#include "defs.h"


/********************************** 
*
*  Structs.h: Definicion de tipos
*	y de estructuras
*
***********************************/
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

void inicializeTTYScreens();

typedef int size_t;

typedef struct shellLine_t{
	int pos;
	int size;
	char buffer[S_BUFFER_SIZE];
} shellLine_t;

typedef struct device_t{
    int id;
    char * name;
    char * address;
    int size;
    int wpos;
    int rpos;
 } device_t;
 
 typedef struct screen_t{
	char * address;
	int size;
 } screen_t;
 
 typedef struct ttyScreen_t{
    int wpos;
    char buffer[SCREEN_COLS*SCREEN_ROWS*2];
 } ttyScreen_t;
 
  typedef struct keyboard_t{
    int head;
    int tail;
    int shift_state;
    int alt_state;
    int ctrl_state;
    int caps_state;
    int escaped_key;
    int dead_key;
    int num_state;
    int scroll_state;
    int lang;
    char buffer[K_BUFFER_SIZE];
 } keyboard_t;

 typedef struct system_t{
   int ticks;
 } system_t;
 
 typedef struct key_t{
    int keyType;
    unsigned char ascii;
    char scancode;
 } key_t;
 
 typedef struct tty_t{
	int ttyNumber;
	ttyScreen_t * screen;
	keyboard_t * keyboard;
	shellLine_t * lineBuffer; 
 } tty_t;
 
 #endif
