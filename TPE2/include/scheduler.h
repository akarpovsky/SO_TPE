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

void select_next(void);
Task_t * get_current_proces();
int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront);
void CreateStackFrame(Task_t * new_proc, PROCESS p, unsigned int stack_start);
void SetupScheduler(void);
short * getSSPointer();
void unatomize();
void atomize();
void yield();

ttyScreen_t * getScreen(Task_t * task);
keyboard_t  * getKeyboard(Task_t * task);
shellLine_t * getLineBuffer(Task_t * task);

//TODO:

int proc1(int argc, char **argv);
int proc2(int argc, char **argv);
int proc3(int argc, char **argv);

#endif /* SCHEDULER_H_ */
