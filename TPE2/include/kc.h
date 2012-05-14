/**********************
 kc.h
**********************/

#ifndef _KC_H
#define _KC_H

#include "defs.h"
#include "limits.h"

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

/* Carga en la IDT las rutinas de atencion de excepciones */
void setup_IDT_exceptions();

/* Carga en la IDT las rutinas de atencion de interrupciones */
void setup_IDT_interrupts();

/* Remapea los PICs a otra posicion */

// Esto se hace para lograr dejar en los primeros 32
// lugares del vector, a las excepciones.

void setup_pics(int offset_MasterPIC, int offset_SlavePIC);

#endif
