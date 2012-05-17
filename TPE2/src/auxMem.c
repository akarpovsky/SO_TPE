#include "../include/auxMem.h"

#define BREAKPOINT \
    asm("0:"                              \
        ".pushsection embed-breakpoints;" \
        ".quad 0b;"                       \
        ".popsection;")

#define MEMORY_START 0x00200000
int * CR3 = (void *) MEMORY_START;
DESCR_PAGE * gdt;

static void flushPageCache();

/*
 * Name: flushPageCache
 * Receives: void
 * Returns: void
 * Description: Flushes the pagination cache of the processor.
 */
static void flushPageCache() {
	dword cr3;
	_scr3(&cr3);
	_lcr3(cr3);
}

void setup_DESCR_PAGE(DESCR_PAGE * item, void * address) {
	item->address = address;
	absent(item);
}

void present(DESCR_PAGE * item) {
	item -> address = (void*) (((int) item -> address) | 0x00000003);
}

void absent(DESCR_PAGE * item) {
	item -> address = (void *) (((int) item -> address) & 0xFFFFFFFC);
}

void myhlt() {
	asm("hlt");
}

void initpages(void * f, void * finMem) {

	// Direccion de la primera pagina para tablas, heap y stack
	int * inicio = CR3;

	// Direccion de la ultima pagina escribible para tablas, heap y stack
	int * fin = (void *) (MAX_PAGE_SIZE * ((int) f / MAX_PAGE_SIZE));

	int * inicioUser;
	int * descriptor;

	int cant_pages_mem, cant_pages, i, cant_dir;

	// Cantidad de paginas disponibles para tablas,heap y stack
	cant_pages_mem = (fin - inicio) / MAX_PAGE_SIZE;

	// Cantidad de paginas para TODA la memoria
	cant_pages = ((int) finMem) / MAX_PAGE_SIZE;

	// Cantidad de DIR ENTRY en PAGE DIRECTORY
	cant_dir = CEILING(cant_pages / 1024);

	// inicializo los descriptores del directorio
	 //y pongo en presente a todos
	for (i = 0; i < cant_dir; i++) {
		// La direccion que va a tener el descriptor es
		// la direccion de la primera tabla de paginas
		// o sea su direccion + la cantidad de descriptores por
		// el tamaño de una pagina
		descriptor = inicio + MAX_PAGE_SIZE + i * MAX_PAGE_SIZE;

		setup_DESCR_PAGE(&gdt[i], descriptor);
		present(&gdt[i]);

	}

	//gdt apunta a la primera tabla
	// asi que ahora bindeo 1 a 1 las direcciones de toda la memoria
	descriptor = 0x00000000;
	gdt = (void *)inicio + MAX_PAGE_SIZE;

	for (i = 0; i < cant_pages; i++) {
		descriptor = descriptor + i * MAX_PAGE_SIZE;
		setup_DESCR_PAGE(&gdt[i], descriptor);

		// Ahora tengo que poner en presente el primer mega
		//	o sea las 256 primeras pags y todo lo demas dejar en ausente
		// el scheduler va a setear el bit de las pags del proceso actual en presente
		if (i < 512 + cant_dir + 1) {
			present(&gdt[i]);
		}
	}

	_lcr3(inicio);
	*inicio = (int) gdt;
	(*inicio) = (*inicio) | 0x00000001;
	_fill_page1();

	/* Activate pagination */
	dword cr0;
	_scr0(&cr0);
	_lcr0(cr0 | 0x80000000);



		// La primera pagina despues de las tablas y los directorios
		inicioUser = CR3 + ((cant_dir + 1) * MAX_PAGE_SIZE);
		createHeadersList(inicioUser,fin);

}

void * getCR3() {

	return CR3;

}

void * getRealPointer(void * pointer) {

	return (void*) (MAX_PAGE_SIZE * CEILING((int)pointer/MAX_PAGE_SIZE));

}

