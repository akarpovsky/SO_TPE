#include "../include/kc.h"
#include "../include/limits.h"

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char * vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (SCREEN_ROWS*SCREEN_COLS*2))
	{
		vidmem[i]='\0';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	}
}

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

void setup_pics(int offset_pic1, int offset_pic2){
	unsigned char mask1 = _in(PIC1_DATA);
	unsigned char mask2 = _in(PIC2_DATA);	

	_out(PIC1_COMMAND, 0x11);
	_out(PIC2_COMMAND, 0x11);

	_out(PIC1_DATA, offset_pic1);
	_out(PIC2_DATA, offset_pic2);
	
	_out(PIC1_DATA, 4);
	_out(PIC2_DATA, 2);

	_out(PIC1_DATA, 0x01);
	_out(PIC2_DATA, 0x01);

	_out(PIC1_DATA, mask1);
	_out(PIC2_DATA, mask2);
}
