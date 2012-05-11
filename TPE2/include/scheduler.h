/*
 * scheduler.h
 *
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "limits.h"
#include "proc.h"
#include "kernel.h"

void select_next(void);
Task_t * get_current_proces();
int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront);
void CreateStackFrame(Task_t * new_proc, PROCESS p, unsigned int stack_start);
void SetupScheduler(void);
short * getSSPointer();
void unatomize();
void atomize();
//void _change_context();
void yield();

#endif /* SCHEDULER_H_ */
