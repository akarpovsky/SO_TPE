#include "../include/kernel.h"

DESCR_INT idt[0xFF];			/* IDT */
IDTR idtr;				/* IDTR */

extern Task_t * current_task;

char color_p = WHITE_TXT;

/* IRQ_0 (Ticks)*/
char ticks_tile = '>';
char ticks_color = TICKS_COLOR;
int timer_tick_hz = 10;

int ticks = TTY_SCREEN_TICSTART;
int actualTTY = TTY_1;
int sarasa = 0;
memory_map_t kmmap;
//TODO:

/* Atencion de Interrupcion por Software */
void int_80(int sysCallNumber, void ** args){

	switch(sysCallNumber){

		case SYSCALL_READ:
			sysread((int) args[0], args[1],(int)args[2]);
			break;
		case SYSCALL_WRITE:
			syswrite((int) args[0], args[1], (int)args[2]);
			break;
		case SYSCALL_KERNELWRITE:
			syskernelwrite((int) args[0], args[1], (int)args[2]);
			break;
		default:
			break;

	}
}


/* Atencion de Interrupcion de Timer Tick */

void int_20(){

	char * video = (char *) 0xb8000;
	video[ticks++]=ticks_tile;
	video[ticks++]=ticks_color;

	if(ticks == TTY_SCREEN_TICEND){
		if(ticks_color == TICKS_COLOR){
			ticks_color = 0x0E;
		}
		else{
			ticks_color = TICKS_COLOR;
		}

		ticks = TTY_SCREEN_TICSTART;
	}

	select_next();
}


void printTicks(){

	ttyScreen_t * screen = getScreen(current_task);

	int wpos = screen->wpos;
	screen->wpos=TTY_SCREEN_TSTART;
	printf("           Ticks: ");
	screen->wpos=wpos;

}

void changeTTY(int tty){
    if(tty != actualTTY){
	actualTTY=tty;
	updateLeds();
	v_changeTTY();
    }
}

/* Atencion de Interrupcion de Teclado */
void int_21(unsigned char scancode){
    struct key_t * key = (struct key_t *) parseKey(scancode);
    if(key->keyType == ALPHANUM_KEY){
		insertKey(key->ascii);
     }
     else if(key->keyType == FUNCTION_KEY){

    	 printf("Function key pressed\n");
     }
}

/* Atencion de Interrupcion de Mouse */
void int_74(unsigned char new_byte){

	mouse_routine(new_byte);

}

/* Carga en las primeras 32 posiciones del vector
de atencion de interrupciones, las rutinas de atencion
a las excepciones */

void setup_IDT_exceptions(){

	// Excepcion: Division por cero
 	setup_IDT_entry (&idt[0], 0x08, (dword)&_excp_00_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[1], 0x08, (dword)&_excp_01_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[2], 0x08, (dword)&_excp_02_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[3], 0x08, (dword)&_excp_03_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[4], 0x08, (dword)&_excp_04_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[5], 0x08, (dword)&_excp_05_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[6], 0x08, (dword)&_excp_06_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[7], 0x08, (dword)&_excp_07_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[8], 0x08, (dword)&_excp_08_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[9], 0x08, (dword)&_excp_09_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[10], 0x08, (dword)&_excp_10_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[11], 0x08, (dword)&_excp_11_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[12], 0x08, (dword)&_excp_12_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[13], 0x08, (dword)&_excp_13_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[14], 0x08, (dword)&_excp_14_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[15], 0x08, (dword)&_excp_15_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[16], 0x08, (dword)&_excp_16_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[17], 0x08, (dword)&_excp_17_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[18], 0x08, (dword)&_excp_18_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[19], 0x08, (dword)&_excp_19_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[20], 0x08, (dword)&_excp_20_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[21], 0x08, (dword)&_excp_21_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[22], 0x08, (dword)&_excp_22_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[23], 0x08, (dword)&_excp_23_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[24], 0x08, (dword)&_excp_24_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[25], 0x08, (dword)&_excp_25_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[26], 0x08, (dword)&_excp_26_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[27], 0x08, (dword)&_excp_27_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[28], 0x08, (dword)&_excp_28_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[29], 0x08, (dword)&_excp_29_hand, ACS_INT, 0);
 	setup_IDT_entry (&idt[30], 0x08, (dword)&_excp_30_hand, ACS_INT, 0);


  }

/* Carga en el vector las rutinas de atencion de interrupciones */

void setup_IDT_interrupts(){

	// IRQ20: Timer tick
   	setup_IDT_entry (&idt[32], 0x08, (dword)&_int_20_hand, ACS_INT, 0);

	// IRQ21: Teclado
	setup_IDT_entry (&idt[33], 0x08, (dword)&_int_21_hand, ACS_INT, 0);

	// IRQ74: Mouse
 	setup_IDT_entry (&idt[44], 0x08, (dword)&_int_74_hand, ACS_INT, 0);

	// IRQ80: SysCALL
 	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);

  }


void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    _out(0x43, 0x36);             /* Set our command byte 0x36 */
    _out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    _out(0x40, ((divisor >> 8) & 0xFF));     /* Set high byte of divisor */
}

void print_header(){

	ttyScreen_t * screen = getScreen(current_task);
	keyboard_t * keyboard = getKeyboard(current_task);

	int wpos = screen->wpos;
	char color_aux = color_p;
	screen->wpos=TTY_SCREEN_HSTART;
	color_p = HEADER_COLOR;
	printf("\t\t\t\tTP Sistemas Operativos - 1do Cuatrimestre 2012 \n\t\t\t\tKarpovsky - Mesa Alcorta - Martinez Correa\n\t\t\t\t\t\t\t Keyboard: ");
	printfcolor(ERROR_COLOR,"%s",(keyboard->lang == ENGLISH) ? "EN" : "ES");
	printf(" | TTY: ");
	printfcolor(ERROR_COLOR,"%d",actualTTY+1);
	printf("\n");
	color_p = color_aux;
	screen->wpos=wpos;
}

int shellLoop(int argc, char ** argv){
  	while(1)
        {
  			_Sti();
			shell();
        }
  	return 0;
}

/**********************************************
kmain()
Punto de entrada de cÃ³o C.
*************************************************/

kmain(multiboot_info_t * mbi, unsigned int magic)
{
	_Cli();

	memory_map_t *memmap;
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC){
		return;
	}

	int maxlenght = 0;
	for (memmap = (memory_map_t *) mbi->mmap_addr;
			(unsigned long) memmap < mbi->mmap_addr + mbi->mmap_length;
			memmap = (memory_map_t *) ((unsigned long) memmap
					+ memmap->size + sizeof (memmap->size))){
		if(memmap->type == 1 && memmap->length_low > maxlenght){
			maxlenght = memmap->length_low;
			kmmap = *memmap;
		}
	}


/* Borra la pantalla. */
	k_clear_screen();

/* Ubico los PICs donde sea necesario */

	setup_pics(0x20, 0x28);

	int i;
	int num;

/* CARGA DE IDT CON LAS RUTINA DE ATENCION DE EXCEPCIONES  */

	setup_IDT_exceptions();

/* CARGA DE IDT CON LAS RUTINA DE ATENCION DE INTERRUPCIONES  */

	setup_IDT_interrupts();

/* Carga de IDTR  */

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;

	_lidt (&idtr);

/* Habilito interrupciones necesarias */

	_mascaraPIC1(0xF8);
	_mascaraPIC2(0xEF);

/* Frecuencia inicial del Timer Tick */

	timer_phase(timer_tick_hz);



/* Puntero a funcion a ser llamada por las acciones
	del mouse */

	mouseCallback callbck;
	callbck = &mouseButtonAction;
	mouseInitialize(callbck);

	initpages(kmmap.base_addr_low+kmmap.length_low,kmmap.base_addr_low+kmmap.length_low );

	// _debug(); // ESTA LINEA HACE QUE SE CUELGUE LA EJECUCIÓN !!!!


	//TODO;
	SetupScheduler();

	print_header();

	printTicks();

	/* Funciï¿½n init(): Inicializa el SO para poder empezar a ejecutar con normalidad.
	 * Aloca espacio para los stacks de las shells y crea 4 procesos para las distintas
	 * terminales.
	 */
	init();

	/* Habilitamos interrupciones y el scheduller empieza a jugar!*/
	_Sti();

	_int_20_hand();
}

void init(void){

	int i = 0;
	Task_t * auxShell = NULL;

	int shell_task_priority = 1; // La prioridad del proceso shell serï¿½ = 1

	for (i = 0; i < TTY_NUMBER; i++) {

		void * stack_start_address = getFreePage(); // Me devuelve una nueva pï¿½gina vacï¿½a con el "PID de kernel"

		if(i == 1){
			// If it is the first TTY, isFront = true
			auxShell = CreateProcess("Shell", shellLoop, NULL, i+1, 0, NULL, stack_start_address, shell_task_priority, true);
		}
		else{
			auxShell = CreateProcess("Shell", shellLoop, NULL, i+1, 0, NULL, stack_start_address, shell_task_priority, false);
		}

			/* Cambio el PID de la pï¿½gina del stack que me devolviï¿½ getFreePage() ya que previo a la creaciï¿½n del
			 * proceso, éste no tenï¿½a ningï¿½n PID asignado y esa pï¿½gina contenï¿½a un PID invï¿½lido.
			 * Luego de este cambio la pï¿½gina serï¿½ accesible por y solo por el proceso que acabamos de crear.
			 */

			changePagePID(auxShell->pid, stack_start_address);

			/* Asigno una pantalla, un teclado y un linebuffer exclusivo para cada TTY */
			auxShell->screen = 		&screens[i];
			auxShell->keyboard =	&keyboards[i];
			auxShell->linebuffer =	&shellLine[i];

			/* Dejo el buffer de pantalla de la TTY listo para ser usado (limpio y con formato de
			 * caracter blanco sobre fondo negro */

			int j = 0;
			for (j = 0; j < SCREEN_SIZE; j++) {
				auxShell->screen->buffer[j++] = 0;
				auxShell->screen->buffer[j] = WHITE_TXT;
			}


	}

}

