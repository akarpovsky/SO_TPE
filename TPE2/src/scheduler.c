/*
 * scheduler.c
 *
 */
#include "../include/scheduler.h"
#include "../include/io.h"

Task_t processes[MAX_PROCESSES];
TaskQueue_t ready_tasks[MAX_PRIORITIES];
TaskQueue_t suspended_tasks;
TaskQueue_t delayed_tasks;
TaskQueue_t waiting_tasks;
TaskQueue_t terminated_tasks;
TaskQueue_t empty_tasks;
Task_t * current_task;
Task_t null_process_task;

void select_next(){

	if(current_task != NULL){
		if(current_task->atomic_level){
			return;
		}
		switch(current_task->state){
		case TaskReady:
			add_to_queue(&(ready_tasks[current_task->priority]), current_task);
			break;
		case TaskSuspended:
			add_to_queue(&suspended_tasks, current_task);
			break;
		case TaskDelaying:
			add_to_queue(&delayed_tasks, current_task);
			break;
		case TaskWaiting:
			add_to_queue(&waiting_tasks, current_task);
			break;
		case TaskTerminated:
			add_to_queue(&terminated_tasks, current_task);
			break;
		case TaskEmpty:
			add_to_queue(&empty_tasks, current_task);
			break;
		}

		current_task->next = NULL;
	}
	int i;
	for(i = 0; i < MAX_PRIORITIES && empty(&(ready_tasks[i])); i++);
	//putc('1');
	if(i == MAX_PRIORITIES){
		current_task = &null_process_task;
	} else	{
		current_task = pop(&(ready_tasks[i]));
	}

}

int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront){
	Task_t * new_proc = pop(&empty_tasks);

	if(new_proc == NULL){
		return 0;
	}
	int i;


	CreateStackFrame(new_proc, process, stack_start);
	new_proc->background = isFront;
	strcpy(new_proc->name, name);
	new_proc->priority = priority;
	new_proc->atomic_level = false;
	new_proc->state = TaskReady;

	add_to_queue(&ready_tasks[priority], new_proc);

	return 1;
}
void CreateStackFrame( Task_t * new_proc, PROCESS p, unsigned int stack_start){
	new_proc->ss = stack_start;
	new_proc->sp = stack_start - sizeof(STACK_FRAME)-1;
	((STACK_FRAME *) new_proc->sp )->EIP = p;
	((STACK_FRAME *) new_proc->sp )->CS = (void *)0x08;
	((STACK_FRAME *) new_proc->sp )->EBP = 0;
	((STACK_FRAME *) new_proc->sp )->EFLAGS = 0;
	((STACK_FRAME *) new_proc->sp )->fin_retaddr = 0;
	((STACK_FRAME *) new_proc->sp )->argc = 0;
	((STACK_FRAME *) new_proc->sp )->argv = 0;
	((STACK_FRAME *) new_proc->sp )->EAX = 0;
	((STACK_FRAME *) new_proc->sp )->EBX = 0;
	((STACK_FRAME *) new_proc->sp )->ECX = 0;
	((STACK_FRAME *) new_proc->sp )->EDI = 0;
	((STACK_FRAME *) new_proc->sp )->EDX = 0;
	((STACK_FRAME *) new_proc->sp )->ESI = 0;
}

void SetupScheduler(){
	int i;
	for(i=0; i<MAX_PROCESSES; i++){
		processes[i].atomic_level = false;
		processes[i].state = TaskEmpty;
		add_to_queue(&empty_tasks, &processes[i]);
	}

	null_process_task.state = TaskNULL;
	strcpy(null_process_task.name, "Null Process");
	CreateStackFrame(&null_process_task, null_process, 1050000);

	CreateProcess("Shell 1", proc1, 0, 0, NULL, 2000000, 2, true);

	CreateProcess("Shell 2", proc2, 0, 0, NULL, 2500000, 2, true);

	CreateProcess("Shell 3", proc3, 0, 0, NULL, 3000000, 2, true);

//	CreateProcess("Shell 1", shellLoop, 0, 0, NULL, 2000000, 0, true);
//	CreateProcess("Shell 2", shellLoop, 1, 0, NULL, 2500000, 1, true);
//	CreateProcess("Shell 3", shellLoop, 2, 0, NULL, 3000000, 2, true);
//	CreateProcess("Shell 4", shellLoop, 3, 0, NULL, 4500000, 3, true);

	current_task = &null_process_task;
}

int * getSPPointer(){
	return &(current_task->sp);
}

short * getSSPointer(){
 	return &(current_task->ss);
}

Task_t * get_current_proces(){
	return current_task;
}

void yield(void){
	__asm__ __volatile__ ("pushf");
	__asm__ __volatile__ ("push %cs");
	_int_20_hand();
}

void atomize(){
	current_task->atomic_level = true;
}

void unatomize(){
	current_task->atomic_level = false;
}

//void _change_context(){
//	putc('c');
//	uint sp = current_task->sp;
//	putu(sp);
//	__asm__ __volatile__("mov %%esp, %0":"=g" (sp));
//	int_20();
//	sp = current_task->sp;
//	putu(sp);
//	putc('i');
//	__asm__ __volatile__("mov %0, %%esp"::"g" (sp));
//}
