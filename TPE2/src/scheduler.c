/*
 * scheduler.c
 *
 */
#include "../include/scheduler.h"

extern size_t my_offset;
int kk =0;

Task_t processes[MAX_PROCESSES];
Task_t * foreground_tasks[TTY_NUMBER];
TaskQueue_t ready_tasks[MAX_PRIORITIES];
TaskQueue_t suspended_tasks;
TaskQueue_t delayed_tasks;
TaskQueue_t waiting_tasks;
TaskQueue_t terminated_tasks;
TaskQueue_t empty_tasks;
Task_t * current_task;
Task_t null_process_task;
Task_t ** foreground_tty;

Task_t * tproc1;
Task_t * tproc2;
Task_t * tproc3;

bool printp1 = true;
bool printp2 = true;
bool printp3 = true;

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
	if(i == MAX_PRIORITIES){
		current_task = &null_process_task;
	} else	{
		current_task = pop(&(ready_tasks[i]));
	}

}

void StartNewTask(char * name, PROCESS new_task_function, char * args, bool isBackground){

	atomize();
	Task_t * auxTask = NULL;
	Task_t * c_t = get_current_task();
	Task_t * fg_t = get_foreground_tty();

	int new_task_priority = c_t->priority; // La prioridad del proceso shell serï¿½ = 1


	void * stack_start_address = getFreePage(); // Me devuelve una nueva pï¿½gina vacï¿½a con el "PID de kernel"

	//TODO:
	if(stack_start_address == NULL){
		while(1){
		kprintf("s");
		}
	}

	auxTask = CreateProcess(name, new_task_function, c_t, c_t->tty_number, 1, &args,
				stack_start_address, new_task_priority, !isBackground);



	/* Cambio el PID de la pï¿½gina del stack que me devolviï¿½ getFreePage() ya que previo a la creaciï¿½n del
	 * proceso, éste no tenï¿½a ningï¿½n PID asignado y esa pï¿½gina contenï¿½a un PID invï¿½lido.
	 * Luego de este cambio la pï¿½gina serï¿½ accesible por y solo por el proceso que acabamos de crear.
	 */
//	changePagePID(auxTask->pid, stack_start_address);
	if(!isBackground)
	{
		unatomize();
		suspend_task(auxTask->parent);
		yield();
		atomize();
	}

	unatomize();


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


	if (isFront) {
		int i;
		for (i = 0; i < TTY_NUMBER; i++) {
			if (foreground_tasks[i] == NULL && i + 1 == tty) {
				foreground_tasks[i] = new_proc;
			} else if (foreground_tasks[i]->tty_number == tty) {
				foreground_tasks[i] = new_proc;
			}
		}
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
	int i, j;
	for(i=0; i<MAX_PROCESSES; i++){
		processes[i].priority = MAX_PRIORITIES;
		processes[i].atomic_level = false;
		processes[i].state = TaskEmpty;
		processes[i].pid = i+1;
		add_to_queue(&empty_tasks, &processes[i]);
		for(j=0; j<PAGES_MAX; j++)
		{
			processes[i].pages[j]=-1;
		}
	}

	for(i=0; i<TTY_NUMBER; i++)
	{
		foreground_tasks[i] = NULL;
	}


	null_process_task.state = TaskNULL;
	null_process_task.keyboard = NULL;
	null_process_task.screen = NULL;
	null_process_task.linebuffer = NULL;
	null_process_task.pid = 0;
	strcpy(null_process_task.name, "Null Process");
	void * null_stack_address = getFreePage(); // Ask for a free page to alocate the stack for the NULL task

	//TODO:
	if(null_stack_address == NULL)
	{
		while(1)
		{
		kprintf("n");

		}
	}

	CreateStackFrame(&null_process_task, null_process, null_stack_address);

//	tproc1 = CreateProcess("proc1", proc1, NULL, 1, 0, NULL,(void*) 0x201000, 0, true);
//	tproc1->screen = &screens[0];
//	tproc2 = CreateProcess("proc1", proc2, NULL, 1, 0, NULL,(void*) 0x202000, 0, false);
//	tproc2->screen = &screens[0];
//	tproc3 = CreateProcess("proc1", proc3, NULL, 1, 0, NULL,(void*) 0x203000, 0, false);
//	tproc3->screen = &screens[0];
//	changePagePID(null_process_task.pid, null_stack_address);

	set_foreground_tty(0);

}

Task_t * get_foreground_tty()
{
	return *foreground_tty;
}

void set_foreground_tty(int tty)
{
	foreground_tty = &foreground_tasks[tty];
}

Task_t * get_foreground_task(int tty)
{
	int i;
	for(i=0; i<TTY_NUMBER; i++)
	{
		if(foreground_tasks[i]->tty_number == tty)
		{
			return foreground_tasks[i];
		}
	}

	return NULL;
}

void * getSPPointer(){
	return &(current_task->sp);
}

void * getSSPointer(){
 	return &(current_task->ss);
}

Task_t * get_current_task(){
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

void suspend_task(Task_t * t)
{
	atomize();
	if(t->state != TaskSuspended)
	{
		t->state = TaskSuspended;
		remove_from_q(&ready_tasks[t->priority], t);
		add_to_queue(&suspended_tasks, t);
	}
	unatomize();
}

void unsuspend_task(Task_t *t)
{
	atomize();
	if(t->state == TaskSuspended)
	{
		t->state = TaskReady;
		remove_from_q(&suspended_tasks, t);
		add_to_queue(&ready_tasks[t->priority], t);
	}
	unatomize();
}

int getpid()
{
	return current_task->pid;
}

void cleaner(int argc, char ** argv)
{

	atomize();
	current_task->state = TaskTerminated;
	if(current_task->foreground)
	{
		foreground_tasks[current_task->tty_number-1] = current_task->parent;
		unsuspend_task(current_task->parent);
	}
	unatomize();
	yield();
}

int null_process(int argc, char **argv){
	Task_t * aux;
	_Sti();
	do
	{
//		kprintf("%x", current_task->sp);
//			while(!empty(&terminated_tasks))
//			{
//				aux = pop(&terminated_tasks);
//				aux->state = TaskEmpty;
//				aux->priority = MAX_PRIORITIES;
//				aux->atomic_level = false;
//				free(aux->ss);
//				aux->ss = 0;
//				aux->sp = 0;
//				aux->parent = NULL;
//				aux->foreground = false;
//				aux->ticks = 0;
//				aux->tty_number = 0;
//				aux->screen = NULL;
//				aux->keyboard = NULL;
//				aux->linebuffer = NULL;
//			}
	}while(true);

	return 0;
}

//TODO: test processes

int proc1(int argc, char **argv){
	_Sti();
	int i = 0, j=1;
	while(1){
		if(printp1){
			printf("Corriendo 1\n");
			_sleep();
			if(tproc2->state != TaskSuspended)
			{
				suspend_task(tproc2);
			}
			else
			{
				unsuspend_task(tproc2);

			}
			printp1 = false;
			printp2 = true;
			printp3 = true;
		}
	}
	return 0;
}

int proc2(int argc, char **argv){
	_Sti();

	int i = 0;
		while(1){
			if(printp2){
				printf("\n\n\n\tCorriendo 2\n\n\n\n");
				_sleep();
				printp2 = false;
				printp3 = true;
				printp1 = true;
			}
		}
	return 0;
}

int proc3(int argc, char **argv){
	//asm volatile ("hlt");
	_Sti();
		while(1){
			if(printp3){
				printf("Corriendo 3\n");
				_sleep();
				printp3 = false;
				printp2 = true;
				printp1 = true;
			}		}
	return 0;
}
