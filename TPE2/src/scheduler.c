#include "../include/scheduler.h"

Task_t null_process_task;
Task_t active_processes[MAX_PRIORITIES][MAX_PROCESSES];
extern int first = true;
int active_process[2] = {MAX_PRIORITIES-1, MAX_PROCESSES-1};

void select_next(){

	// Si está en estado atómico, conserva el procesador.
	if(CURRENT_PROCESS.atomic_level){
		return;
	}

	//Busco un proceso con mayor prioridad al actual que estΰe esperando el procesador.
	//Si existe se lo cede.
	int i;
	for(i = 0; i<active_process[PRIORITY]; i++){
		if(active_processes[i][0].state != TaskEmpty) {
			active_process[PRIORITY] = i;
			active_process[TASK_NO] = 0;
			return;
		}
	}

	//Busco un proceso de la misma prioridad que esté esperando el procesador.
	//Si existe, se lo cede.
	for(i = active_process[TASK_NO]+1; i != active_process[TASK_NO]; i++)
	{
		i = i % MAX_PROCESSES;
		if(active_processes[active_process[PRIORITY]][i].state != TaskEmpty) {
			active_process[TASK_NO] = i;
			return;
		}
//		printf("%d ", i);
//		_sleep();
	}
	printf("\n");
//	if(active_processes[active_process[PRIORITY]][(active_process[TASK_NO]+1)%MAX_PROCESSES].state != TaskEmpty) {
//		active_process[TASK_NO] = (active_process[TASK_NO] + 1) % MAX_PROCESSES;
//		return;
//	}

	//Verifico que el proceso actual siga estando listo.
	//En caso afirmativo, conserva el procesador.
	if(active_processes[active_process[PRIORITY]][active_process[TASK_NO]].state == TaskReady){
		return;
	}

	//Verifico la existencia de procesos de menor prioridad.
	//Si existen, se les cede el procesador.
	for(i=active_process[PRIORITY]+1; i< MAX_PRIORITIES; i++){
		if(active_processes[i][0].state != TaskEmpty){
			active_process[PRIORITY] = i;
			active_process[TASK_NO] = 0;
			return;
		}
	}

	//En caso de no haber más procesos a quienes cederle el procesador, lo tiene el proces nulo.
	if(active_process[PRIORITY] == 0 || active_processes[active_process[PRIORITY]][active_process[TASK_NO]].state == TaskEmpty){
		active_process[PRIORITY] = MAX_PRIORITIES-1;
		active_process[TASK_NO] = MAX_PROCESSES-1;
	}

}

int CreateProcess(char* name, PROCESS process,int tty, int argc, char** argv, int stack_start, int priority, int isFront){
	int i;
	for(i = 0; i < MAX_PROCESSES; i++){
		if(active_processes[priority][i].state == TaskEmpty)
		{
			break;
		}
	}

	if(i == MAX_PROCESSES)
	{
		return 0;
	}

	CreateStackFrame(priority, i, process, stack_start);
	active_processes[priority][i].background = isFront;
	strcpy(active_processes[priority][i].name, name);
	active_processes[priority][i].priority = priority;
	active_processes[priority][i].atomic_level = 0;
	active_processes[priority][i].state = TaskReady;

	return 1;
}
void CreateStackFrame(int priority, int task_no, PROCESS p, unsigned int stack_start){
	active_processes[priority][task_no].ss = stack_start;
	active_processes[priority][task_no].sp = stack_start - sizeof(STACK_FRAME)-1;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EIP = p;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->CS = (void *)0x08;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EBP = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EFLAGS = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->retaddr = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->argc = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->argv = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EAX = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EBX = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->ECX = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EDI = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->EDX = 0;
	((STACK_FRAME *) active_processes[priority][task_no].sp )->ESI = 0;
}

void SetupScheduler(){
	int i, j;
	for(i=0; i<MAX_PRIORITIES; i++){
		for(j=0; j<MAX_PROCESSES; j++){
			active_processes[i][j].state = TaskEmpty;
		}
	}

	null_process_task.state = TaskReady;
	strcpy(null_process_task.name, "Null Process");
	CreateStackFrame(MAX_PRIORITIES-1, MAX_PROCESSES-1, null_process, 1050000);

	CreateProcess("Shell 1", shellLoop, 0, 0, NULL, 2000000, 2, true);

	CreateProcess("Shell 2", shellLoop, 0, 0, NULL, 2500000, 2, true);

	//CreateProcess("Shell 3", shellLoop, 0, 0, NULL, 3000000, 2, true);

//	CreateProcess("Shell 1", shellLoop, 0, 0, NULL, 2000000, 0, true);
//	CreateProcess("Shell 2", shellLoop, 1, 0, NULL, 2500000, 1, true);
//	CreateProcess("Shell 3", shellLoop, 2, 0, NULL, 3000000, 2, true);
//	CreateProcess("Shell 4", shellLoop, 3, 0, NULL, 4500000, 3, true);
}

int null_process(int argc, char **argv){
	_Sti();
	do
	{
		putc('n');
	}while(true);

	return 0;
}

int * getSPPointer(){
//	if(first){
//		printf("Primer SP %d\n", CURRENT_PROCESS.sp);
//
//		return 0;
//	}
	//printf("SP %d\n", process[active_process%2].sp);

	return &CURRENT_PROCESS.sp;
}

short * getSSPointer(){
//	if(first){
//		printf("Primer SS %d\n", CURRENT_PROCESS.ss);
//
//		return 0;
//	}
	//printf("SS %d\n", process[active_process%2].ss);
	return &CURRENT_PROCESS.ss;
}

int shellLoop(int argc, char ** argv){
  	while(1)
        {
  			_Sti();
			shell();
        }
  	return 0;
}

int proc1(int argc, char **argv){
	_Sti();
	while(1){
		printf("x");
		//video[1800]='a';
		//video[1801]=ticks_color;
	}
	return 0;
}

int proc2(int argc, char **argv){
	//asm volatile ("hlt");
	_Sti();
		while(1){
			printf("y");
			//video[2800]='b';
			//video[2801]=ticks_color;
		}
	return 0;
}

int proc3(int argc, char **argv){
	//asm volatile ("hlt");
	_Sti();
		while(1){
			printf("z");
			//video[2800]='b';
			//video[2801]=ticks_color;
		}
	return 0;
}
