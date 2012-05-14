#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "scheduler.h"

/********************************** 
*
*  keyboard.h: Funciones para manejo
*         de teclado
*
***********************************/

int isPrintable(unsigned char c);
struct key_t * parseKey(unsigned char c);
void viewBuffer();
unsigned char insertKey(unsigned char c);
unsigned char getKey();
int bufferIsFull();
int isEmptyBuffer();
void changeLang();
int isVowel(char c);
char accVowel(char c);
void updateLeds();

#endif
