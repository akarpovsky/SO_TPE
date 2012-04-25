/***************************************************
  Defs.h
	
****************************************************/

#ifndef _defs_
#define _defs_

#pragma pack(1)

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

/* Dimensiones de la pantalla */
#define SCREEN_ROWS	80
#define SCREEN_COLS	25
#define SCREEN_POS	0xb8000
#define SCREEN_SIZE	4000
#define SCREEN_LAST_ROW	3840
#define SCREEN_ROW_SIZE	160
#define SCREEN_TAB_SIZE	8

/* Atributos de Video */

#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro
#define HEADER_COLOR 0x0A // Atributo de video. Letras verdes, fondo negro
#define COMMAND_COLOR 0x02 // Atributo de video. Letras verdes NO negrita, fondo negro
#define TICKS_COLOR 0x0D 
#define ERROR_COLOR 0x0C // Atributo de video. Letras rojas, fondo negro
#define MARINE_COLOR 0x03 // Atributo de video. Letras color marino, fondo negro

/* Ubicacion de los PICs */

#define PIC1		0x20		// Direccion base de IO para el PIC maestro
#define PIC2		0xA0		// Direccion base de IO para el PIC esclavo
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC1_REMAP 0x20
#define PIC2_REMAP 0x28

/* Numeros de IRQ */

#define IRQ_TIMERTICK 0x20
#define IRQ_TECLADO 0x21

/* Devices */

#define DEVICE_SCREEN 1
#define DEVICE_KEYBOARD 2

/* Layouts del teclado */

#define ENGLISH	1
#define SPANISH	2


#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Scan codes de teclas claves */

#define ALT_PRESSED		0x38
#define ALT_RELEASED		0xb8
#define ARROW_UP		0x48
#define ARROW_LEFT		0x4b
#define ARROW_RIGHT		0x4d
#define ARROW_DOWN		0x50
#define BACKSPACE		0x0e
#define CAPSLOCK		0x3a
#define CONTROL_PRESSED		0x1d
#define CONTROL_RELEASED	0x9d
#define DEAD_KEY		0x1a
#define DELETE			0x53
#define ENTER			0x1c
#define ESCAPE			0x01
#define ESCAPED_KEY		0xe0
#define F1			0x3b
#define F2			0x3c
#define F3			0x3d
#define F4			0x3e
#define LEFT_SHIFT_PRESSED	0x2a
#define LEFT_SHIFT_RELEASED	0xaa
#define NUMLOCK			0x45
#define PAGE_UP			0x49
#define PAGE_DOWN		0x51
#define RIGHT_SHIFT_PRESSED	0x36
#define RIGHT_SHIFT_RELEASED	0xb6
#define SCROLLLOCK		0x46
#define SPACE			0x39
#define TABULAR			0x0f

/* Tipos de teclas */

#define ALPHANUM_KEY	1
#define FUNCTION_KEY	2
#define HIDDEN_KEY	3

/* Buffer de teclado */

#define K_BUFFER_SIZE	10

/* Buffer de la linea de comando */

#define S_BUFFER_SIZE	1024

/* Booleans */

#define TRUE		1
#define FALSE		0

/* TTYs */

#define TTY_NUMBER	4
#define TTY_1		0
#define TTY_2		1
#define TTY_3		2
#define TTY_4		3
#define TTY_SCREEN_HSTART	0
#define TTY_SCREEN_TSTART	SCREEN_ROWS*4*2
#define TTY_SCREEN_TICSTART	TTY_SCREEN_TSTART+36
#define TTY_SCREEN_TICEND	TTY_SCREEN_TSTART+108
#define TTY_SCREEN_SSTART	SCREEN_ROWS*12
#define TTY_SCREEN_RSTART	6

/* I/O */

#define STDIN		0
#define STDOUT		1

/* System Calls */

#define SYSCALL_READ	3
#define SYSCALL_WRITE	4

/* Error */

#define ERROR		-1

/* Ascii */

#define SPACE_A		0x20
#define EMPTY_CHAR	0
#define A_ENIE_MAY	0xa5
#define A_ENIE_MIN	0xa4
#define A_ACC_A		0xa0
#define A_ACC_E		0x82
#define A_ACC_I		0xa1
#define A_ACC_O		0xa2
#define A_ACC_U		0xa3



#define NULL (void *)0

/* Mouse */

typedef struct MOUSE_DATA{
	unsigned char leftClick;
	unsigned char rightClick;
	unsigned char centerClick;
} MOUSE_DATA;

typedef  void (* mouseCallback)(MOUSE_DATA * mData );

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;

/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

     

#endif
