
#ifndef _VIDEO_H
#define _VIDEO_H
#include "defs.h"
#include "structs.h"

/********************************** 
*
*  Video.h: Driver de video
*
***********************************/

void writeInVideo(char c);
void scroll();
void v_changeTTY();
void clearScreen();
void clearScreenBuffer();
void clearfromto(int from, int to);
char fixKey(char c);
void move_cursor(int position);
 
 #endif
