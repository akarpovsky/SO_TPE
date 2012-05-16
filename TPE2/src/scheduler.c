/*
 * scheduler.c
 *
 */
#include "../include/scheduler.h"

Task_t processes[MAX_PROCESSES];
TaskQueue_t ready_tasks[MAX_PRIORITIES];
TaskQueue_t suspended_tasks;
TaskQueue_t delayed_tasks;
TaskQueue_t waiting_tasks;
TaskQueue_t terminated_tasks;
TaskQueue_t empty_tasks;
Task_t * current_task;
Task_t null_process_task;
Task_t * foreground_task;

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

Task_t * CreateProcess(char* name, PROCESS process, Task_t * parent, int tty, int argc, char** argv, void * stack_start, int priority, int isFront){
	Task_t * new_proc = pop(&empty_tasks);

	if(new_proc == NULL){
		return NULL;
	}

	new_proc->tty_number = tty;
	new_proc->parent = parent;

	if(parent != NULL)
	{
		new_proc->screen = parent->screen;
		new_proc->keyboard = parent->keyboard;
		new_proc->linebuffer = parent->linebuffer;
	} // Else, screen, keyboard and linebuffer had already been initialized in the init() kernel call.

	CreateStackFrame(new_proc, process, stack_start);

	new_proc->foreground = isFront;
	strcpy(new_proc->name, name);
	new_proc->priority = priority;
	new_proc->atomic_level = false;
	new_proc->state = TaskReady;

	add_to_queue(&ready_tasks[priority], new_proc);

	if(isFront == true)
	{
		foreground_task = new_proc;
	}

	return new_proc;
}
void CreateStackFrame( Task_t * new_proc, PROCESS p, void * stack_start){
	new_proc->ss = stack_start;
	new_proc->sp = stack_start - sizeof(STACK_FRAME)-1;
	((STACK_FRAME *) new_proc->sp )->EIP = p;
	((STACK_FRAME *) new_proc->sp )->CS = (void *)0x08;
	((STACK_FRAME *) new_proc->sp )->EBP = 0;
	((STACK_FRAME *) new_proc->sp )->EFLAGS = 0;
	((STACK_FRAME *) new_proc->sp )->fin_retaddr = cleaner;
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
		processes[i].priority = MAX_PRIORITIES;
		processes[i].atomic_level = false;
		processes[i].state = TaskEmpty;
		processes[i].pid = i;
		add_to_queue(&empty_tasks, &processes[i]);
	}

	null_process_task.state = TaskNULL;
	null_process_task.keyboard = NULL;
	null_process_task.screen = NULL;
	null_process_task.linebuffer = NULL;
	strcpy(null_process_task.name, "Null Process");

	void * null_stack_address = getFreePage(); // Ask for a free page to alocate the stack for the NULL task
	CreateStackFrame(&null_process_task, null_process, null_stack_address);
	changePagePID(null_process_task.pid, null_stack_address);

	current_task = &null_process_task;
}

void * getSPPointer(){
	return &(current_task->sp);
}

void * getSSPointer(){
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

ttyScreen_t * getScreen(Task_t * task)
{
	return task->screen;
}
keyboard_t  * getKeyboard(Task_t * task)
{
	return task->keyboard;
}
shellLine_t * getLineBuffer(Task_t * task)
{
	return task->linebuffer;
}

//TODO: add to .h

void suspend_task(Task_t * t)
{
	atomize();
	t->state = TaskSuspended;
	remove_from_q(&ready_tasks[t->priority], t);
	add_to_queue(&suspended_tasks, t);
	unatomize();
}

void unsuspend_task(Task_t *t)
{
	atomize();
	t->state = TaskReady;
	remove_from_q(&suspended_tasks, t);
	add_to_queue(&ready_tasks[t->priority], t);
	unatomize();
}

int getpid(Task_t * t)
{
	if(t != NULL)
		return t->pid;
	else
		return 0;
}

void cleaner(int argc, char ** argv)
{
	current_task->state = TaskTerminated;
	yield();
}

int null_process(int argc, char **argv){
	_Sti();
	Task_t * aux;
	do
	{
			while(!empty(&terminated_tasks))
			{
				aux = pop(&terminated_tasks);
				aux->state = TaskEmpty;
				aux->priority = MAX_PRIORITIES;
				aux->atomic_level = false;
				freepages(aux->ss);
				aux->ss = 0;
				aux->sp = 0;
				aux->parent = NULL;
				aux->foreground = false;
				aux->ticks = 0;
				aux->tty_number = 0;
				aux->screen = NULL;
				aux->keyboard = NULL;
				aux->linebuffer = NULL;
			}
	}while(true);

	return 0;
}
//TODO: test processes

int proc1(int argc, char **argv){
	_Sti();
	int i = 0;
	while(1){
		printf("x");
		i++;
		if(!(i%10)){
			yield();
		}
	}
	return 0;
}

int proc2(int argc, char **argv){
	//asm volatile ("hlt");
	_Sti();

	int i = 0;
		while(1){
			printf("y");
			i++;
			if(!(i%10)){
				yield();
			}
		}
	return 0;
}

int proc3(int argc, char **argv){
	//asm volatile ("hlt");
	_Sti();
		while(1){
			printf("z");
		}
	return 0;
}
