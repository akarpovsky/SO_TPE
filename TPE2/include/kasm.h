/*********************************************
kasm.h

************************************************/
#ifndef _KASM_H
#define _KASM_H
#include "defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

 

void		_int_20_hand();      /* Timer tick */

void		_int_21_hand();       /* Teclado */

void		_int_74_hand();		/* Mouse */

void		_int_80_hand();

void		_excp_00_hand();
void		_excp_01_hand();
void		_excp_02_hand();
void		_excp_03_hand();
void		_excp_04_hand();
void		_excp_05_hand();
void		_excp_06_hand();
void		_excp_07_hand();
void		_excp_08_hand();
void		_excp_09_hand();
void		_excp_10_hand();
void		_excp_11_hand();
void		_excp_12_hand();
void		_excp_13_hand();
void		_excp_14_hand();
void		_excp_15_hand();
void		_excp_16_hand();
void		_excp_17_hand();
void		_excp_18_hand();
void		_excp_19_hand();
void		_excp_20_hand();
void		_excp_21_hand();
void		_excp_22_hand();
void		_excp_23_hand();
void		_excp_24_hand();
void		_excp_25_hand();
void		_excp_26_hand();
void		_excp_27_hand();
void		_excp_28_hand();
void		_excp_29_hand();
void		_excp_30_hand();


void		_debug (void);

#endif

