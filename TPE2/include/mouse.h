/*
 * mouse.h
 *
 *  Created on: May 31, 2009
 *      Author: dgrandes
 */


#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "structs.h"



/*Función que inicializa el mouse para habilitarlo a que reciba paquetes
 * por el puerto 60 a través de la IRQ12. Recibe como parametro una función
 * callback que este ejecutará cuando se haya completado el paquete de datos
 * del mouse. El prototipo de la función callback se encuentra definido en defs.h
 */
void mouseInitialize( mouseCallback callbckAction );


void mouseButtonAction(MOUSE_DATA * mData);



/*El handler de la interrupción de la IRQ12. La rutina de atención 0x74 de la IDT
 * encargada del mouse.
 */
void mouse_routine(unsigned char new_byte );

/*Función que escribe al mouse, primero indica que se enviara un
 * comando con el código 0xD4 al puerto de estado
 * y luego se envia el comando
 */

static void mouseWrite(unsigned char data);

/*Función que espera que el mouse tenga información nueva antes
 * de leer de el
 */

static unsigned char mouseRead(void);

/* Función que espera con un timeout hasta que el mouse
 * indique con el puerto de estado que esta listo para recibir
 * informaicón
 */

static void waitForMouseToAccept( void );

/*Función que espera que el Mouse tenga nueva información antes
 * de leer cual es esta.
 */

static void waitForMouseToHaveInfo( void );

#endif /* MOUSE_H_ */
