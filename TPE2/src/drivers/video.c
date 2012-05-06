#include "../../include/video.h"

extern struct screen_t screen;
extern struct tty_t ttys[];
extern int actualTTY;
extern char color_p ;
char * video = (char *) SCREEN_POS;

void writeInVideo(char c){
	int i;
	int colpos;
	c=fixKey(c);
	switch(c){
	case '\t':
		if(ttys[actualTTY].screen->wpos % SCREEN_TAB_SIZE == 0)
			for(i=0; i<4; i++){
				screen.address[ttys[actualTTY].screen->wpos+=2] = 0;
				ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos] = 0;
			}
		while(ttys[actualTTY].screen->wpos % SCREEN_TAB_SIZE > 0){
			screen.address[ttys[actualTTY].screen->wpos+=2] = 0;
			ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos] = 0;
		}
		break;
	case '\n':
		if(ttys[actualTTY].screen->wpos >= SCREEN_LAST_ROW){
			scroll();
		}else{
			colpos = ttys[actualTTY].screen->wpos % SCREEN_ROW_SIZE;
			for( i = colpos; i < SCREEN_ROW_SIZE; i++){
				if(i % 2 == 0){
					screen.address[ttys[actualTTY].screen->wpos+=2] = 0;
					ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos] = 0;
				}
			}
		}
		break;
	case '\b':
		if(ttys[actualTTY].screen->wpos != 0){
			if(screen.address[ttys[actualTTY].screen->wpos] == 0){
				while(screen.address[ttys[actualTTY].screen->wpos-=2] == 0);
				ttys[actualTTY].screen->wpos+=2;
			}
			screen.address[ttys[actualTTY].screen->wpos-=2] = 0;
			ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos] = 0;
		}
		break;
	default:
		screen.address[ttys[actualTTY].screen->wpos] = c;
		ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos++] = c;
		screen.address[ttys[actualTTY].screen->wpos] = color_p;
		ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos++] = color_p;
		if(ttys[actualTTY].screen->wpos >= SCREEN_SIZE){
			scroll();
		}
		break;
	}
	move_cursor(ttys[actualTTY].screen->wpos/2);
}

void scroll(){
	int i = 0;
	for( i = TTY_SCREEN_RSTART; i < SCREEN_COLS-1; i++){
		_memcpy(&screen.address[i*SCREEN_ROW_SIZE + SCREEN_ROW_SIZE], &screen.address[i*SCREEN_ROW_SIZE], SCREEN_ROW_SIZE);
		_memcpy(&ttys[actualTTY].screen->buffer[i*SCREEN_ROW_SIZE + SCREEN_ROW_SIZE], &ttys[actualTTY].screen->buffer[i*SCREEN_ROW_SIZE], SCREEN_ROW_SIZE);
	}

	ttys[actualTTY].screen->wpos = SCREEN_LAST_ROW;
	while(ttys[actualTTY].screen->wpos < SCREEN_SIZE){
		screen.address[ttys[actualTTY].screen->wpos] = 0;
		ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos++] = 0;
		screen.address[ttys[actualTTY].screen->wpos] = WHITE_TXT;
		ttys[actualTTY].screen->buffer[ttys[actualTTY].screen->wpos++] = WHITE_TXT;
	}
	ttys[actualTTY].screen->wpos = SCREEN_LAST_ROW;
}

void v_changeTTY(){
	int i;
	clearScreen();
	print_header();
	printTicks();
	for( i = TTY_SCREEN_SSTART; i < SCREEN_SIZE; i++){
		screen.address[i] = ttys[actualTTY].screen->buffer[i];
	}
	if(ttys[actualTTY].screen->wpos==TTY_SCREEN_SSTART){
		printf("CasKarOS tty%d:~$ ",actualTTY+1);
	}
	move_cursor(ttys[actualTTY].screen->wpos/2);
}

void clearScreen(){
	int i;
	for( i= 0; i < SCREEN_SIZE ; i++){
		screen.address[i++] = 0;
		screen.address[i] = WHITE_TXT;
	}
}

void clearScreenBuffer(){
	int i;
	for( i= 0; i < SCREEN_SIZE ; i++){
		ttys[actualTTY].screen->buffer[i++] = 0;
		screen.address[i] = WHITE_TXT;
	}
}

void clearfromto(int from, int to){
	int i;
	for( i= from; i < to ; i++){
		screen.address[i++] = 0;
		screen.address[i] = WHITE_TXT;
	}
	ttys[actualTTY].screen->wpos = from;
}

char fixKey(char c){
	char ch;
	switch(c){
	case 'ñ':
		ch=A_ENIE_MIN;
		break;
	case 'Ñ':
		ch=A_ENIE_MAY;
		break;
	case 'á':
		ch=A_ACC_A;
		break;
	case 'é':
		ch=A_ACC_E;
		break;
	case 'í':
		ch=A_ACC_I;
		break;
	case 'ó':
		ch=A_ACC_O;
		break;
	case 'ú':
		ch=A_ACC_U;
		break;
	default:
		ch=c;
		break;
	}
	return ch;
}

void move_cursor(int position){
	unsigned short p1 = 0x3D4;
	unsigned short p2 = 0x3D5;

	unsigned char low= position & 0xFF ;
	unsigned char high = position >> 8;

	unsigned char exp1 = 0x0E;
	unsigned char exp2 = 0x0F;

	//Out a p1 con 0x0E y pasa la parte alta.	
	__asm__("outb %1,%0":: "dN" (p1), "a"(exp1));
	__asm__("outb %1,%0"::"dN" (p2), "a"(high));
	//Out a p2 con 0x0F y pasa la parte baja.
	__asm__("outb %1,%0"::"dN" (p1), "a"(exp2));
	__asm__("outb %1,%0"::"dN" (p2), "a"(low));
}
