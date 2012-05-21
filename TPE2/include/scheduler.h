/*
 * scheduler.h
 *
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "limits.h"
#include "proc.h"
#include "structs.h"
#include "io.h"
//#include "memorym.h"
#include "memo.h"

void select_next(void);
Task_t * get_current_task();
Task_t * get_foreground_tty();
void set_foreground_tty(int tty);
Task_t * get_foreground_task(int tty);
Task_t * CreateProcess(char* name, PROCESS process, Task_t * parent, int tty, int argc, char** argv, void * stack_start, int priority, int isFront);
void CreateStackFrame(Task_t * new_proc, PROCESS p, void * stack_start);
void SetupScheduler(void);
void * getSSPointer();
void * getSPPointer();
void unatomize();
void atomize();
void yield();
int null_process(int argc, char **argv);
void StartNewTask(char * name, PROCESS new_task_function, char * args, bool isBackground);

void suspend_task(Task_t * t);
void unsuspend_task(Task_t *t);
int getpid();
void cleaner(int argc, char ** argv);

ttyScreen_t * getScreen(Task_t * task);
keyboard_t  * getKeyboard(Task_t * task);
shellLine_t * getLineBuffer(Task_t * task);

//TODO:

int proc1(int argc, char **argv);
int proc2(int argc, char **argv);
int proc3(int argc, char **argv);

#endif /* SCHEDULER_H_ */
