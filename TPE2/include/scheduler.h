/*
 * scheduler.h
 *
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "../include/strings.h"
#include "../include/structs.h"
#include "../include/shell.h"
#include "../include/io.h"
#define MAX_PROCESSES 64
#define MAX_PRIORITIES 4
#define PRIORITY 0
#define TASK_NO 1
#define CURRENT_PROCESS active_processes[active_process[PRIORITY]][active_process[TASK_NO]]

void select_next(void);
int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront);
void CreateStackFrame(int priority, int task_no, PROCESS p, unsigned int stack_start);
void SetupScheduler(void);
int null_process(int argc, char **argv);
short * getSSPointer();
int * getSPPointer();
int shellLoop(int argc, char ** argv);
int proc1(int argc, char **argv);
int proc2(int argc, char **argv);
int proc3(int argc, char **argv);


#endif /* SCHEDULER_H_ */
