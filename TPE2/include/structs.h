#include "defs.h"

#ifndef _STRUCTS_H
#define _STRUCTS_H


/********************************** 
*
*  Structs.h: Definicion de tipos
*	y de estructuras
*
***********************************/

void inicializeTTYScreens();

typedef int size_t;

typedef struct shellLine_t{
	int pos;
	int size;
	char buffer[S_BUFFER_SIZE];
} shellLine_t;

typedef struct device_t{
    int id;
    char * name;
    char * address;
    int size;
    int wpos;
    int rpos;
 } device_t;
 
 typedef struct screen_t{
	char * address;
	int size;
 } screen_t;
 
 typedef struct ttyScreen_t{
    int wpos;
    char buffer[SCREEN_COLS*SCREEN_ROWS*2];
 } ttyScreen_t;
 
  typedef struct keyboard_t{
    int head;
    int tail;
    int shift_state;
    int alt_state;
    int ctrl_state;
    int caps_state;
    int escaped_key;
    int dead_key;
    int num_state;
    int scroll_state;
    int lang;
    char buffer[K_BUFFER_SIZE];
 } keyboard_t;

 typedef struct system_t{
   int ticks;
 } system_t;
 
 typedef struct key_t{
    int keyType;
    unsigned char ascii;
    char scancode;
 } key_t;
 
 typedef struct tty_t{
	int ttyNumber;
	ttyScreen_t * screen;
	keyboard_t * keyboard;
	shellLine_t * lineBuffer; 
 } tty_t;
 
 #endif
