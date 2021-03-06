#include "../../include/video.h"

extern struct screen_t main_screen;
extern char color_p ;
char * video = (char *) SCREEN_POS;
extern size_t offset;

void kwriteInVideo(char c){


	int i;
	int colpos;
	switch (c) {
	case '\t':
		for (i = 0; i < 4; i++) {
			main_screen.address[offset += 2] = 0;
		}
		break;
	case '\n':
		colpos = offset % SCREEN_ROW_SIZE;
		for (i = colpos; i < SCREEN_ROW_SIZE; i++) {
			if (i % 2 == 0) {
				main_screen.address[offset += 2] = 0;
			}
		}
		break;
	case '\b':
		if (offset != 0) {
			if (main_screen.address[offset++] == 0) {
				while (main_screen.address[offset -= 2] == 0)
					;
				offset += 2;
			}
			main_screen.address[offset -= 2] = 0;
		}
		break;
	default:
		main_screen.address[offset++] = c;
		main_screen.address[offset++] = color_p;
		break;
	}
}


void writeInVideo(char c){

	atomize();
	ttyScreen_t * screen = (ttyScreen_t *) getScreen(get_current_task());

	int i;
	int colpos;
	c = fixKey(c);
	switch (c) {
	case '\t':
		if (screen->wpos % SCREEN_TAB_SIZE == 0)
			for (i = 0; i < 4; i++) {
				if(get_current_task() == get_foreground_tty())
				{
					main_screen.address[screen->wpos += 2] = 0;
				}
				screen->buffer[screen->wpos] = 0;
			}
		while (screen->wpos % SCREEN_TAB_SIZE > 0) {
			if(get_current_task() == get_foreground_tty())
			{
				main_screen.address[screen->wpos += 2] = 0;
			}
			screen->buffer[screen->wpos] = 0;
		}
		break;
	case '\n':
		if (screen->wpos >= SCREEN_LAST_ROW) {
			scroll();
		} else {
			colpos = screen->wpos % SCREEN_ROW_SIZE;
			for (i = colpos; i < SCREEN_ROW_SIZE; i++) {
				if (i % 2 == 0) {
					if(get_current_task() == get_foreground_tty())
					{
						main_screen.address[screen->wpos += 2] = 0;
					}
					screen->buffer[screen->wpos] = 0;
				}
			}
		}
		break;
	case '\b':
		if (screen->wpos != 0) {
			int auxpos = screen->wpos;
			if (screen->buffer[screen->wpos] == 0) {
				if(get_current_task() == get_foreground_tty())
				{
					while (main_screen.address[auxpos -= 2] == 0);
//					auxpos+=2;
				}
				while (screen->buffer[screen->wpos -= 2] == 0);
//				screen->wpos += 2;
			}
			if(get_current_task() == get_foreground_tty())
			{
//				auxpos+=2;
				main_screen.address[auxpos] = 0;

			}
//			screen->wpos+=2;
			screen->buffer[screen->wpos] = 0;
		}
		break;
	default:
		if(get_current_task() == get_foreground_tty())
		{
			main_screen.address[screen->wpos] = c;
		}
		screen->buffer[screen->wpos++] = c;
		if(get_current_task() == get_foreground_tty())
		{
			main_screen.address[screen->wpos] = color_p;
		}
		screen->buffer[screen->wpos++]
		               = color_p;
		if (screen->wpos >= SCREEN_SIZE) {
			scroll();
		}
		break;
	}
	move_cursor(screen->wpos / 2);
	unatomize();
}


//FIXME: doesn't scroll good
void scroll(){

	ttyScreen_t * screen = (ttyScreen_t *) getScreen(get_current_task());

	int i = 0;
	for (i = TTY_SCREEN_RSTART * SCREEN_ROW_SIZE; i < SCREEN_LAST_ROW; i++) {
		if(get_current_task() == get_foreground_tty()){
			main_screen.address[i] =
					main_screen.address[i + SCREEN_ROW_SIZE];
		}
		screen->buffer[i] =
				screen->buffer[i + SCREEN_ROW_SIZE];

		//		_memcpy(&main_screen.address[i * SCREEN_ROW_SIZE + SCREEN_ROW_SIZE],
		//				&main_screen.address[i * SCREEN_ROW_SIZE],
		//				SCREEN_ROW_SIZE);
		//		_memcpy(&screen->buffer[i * SCREEN_ROW_SIZE + SCREEN_ROW_SIZE],
		//				&screen->buffer[i * SCREEN_ROW_SIZE],
		//				SCREEN_ROW_SIZE);
	}
	screen->wpos = SCREEN_LAST_ROW;
	while (screen->wpos < SCREEN_SIZE) {
		if(get_current_task() == get_foreground_tty())
		{
			main_screen.address[screen->wpos] = 0;

		}
		screen->buffer[screen->wpos++] = 0;
		if(get_current_task() == get_foreground_tty())
		{
			main_screen.address[screen->wpos] = WHITE_TXT;
		}
		screen->buffer[screen->wpos++] = WHITE_TXT;
	}
	screen->wpos = SCREEN_LAST_ROW;
}

void v_changeTTY(){

	Task_t * fg_t = get_foreground_tty();
	ttyScreen_t * screen = (ttyScreen_t *) getScreen(fg_t);

	int i;
	clearScreen();
	offset = 0;
	print_header();
	//	printTicks();
	for (i = TTY_SCREEN_SSTART; i < SCREEN_SIZE; i++) {
		main_screen.address[i] = screen->buffer[i];
	}
	if (screen->wpos == TTY_SCREEN_SSTART) {
		printf("BrunOS tty%d:~$ ", fg_t->tty_number);
	}
	move_cursor(screen->wpos / 2);
}

void clearScreen(){
	int i;
	for (i = 0; i < SCREEN_SIZE; i++) {
		main_screen.address[i++] = 0;
		main_screen.address[i] = WHITE_TXT;
	}
}

void clearScreenBuffer(){
	ttyScreen_t * screen = (ttyScreen_t *) getScreen(get_current_task());
	int i;
	for (i = 0; i < SCREEN_SIZE; i++) {
		screen->buffer[i++] = 0;
		main_screen.address[i] = WHITE_TXT;
	}
}

void clearfromto(int from, int to){

	ttyScreen_t * screen = (ttyScreen_t *) getScreen(get_current_task());

	int i;
	for (i = from; i < to; i++) {
		main_screen.address[i++] = 0;
		main_screen.address[i] = WHITE_TXT;
	}
	screen->wpos = from;
}

char fixKey(char c){
	char ch;
	switch (c) {
	case '�':
		ch = A_ENIE_MIN;
		break;
	case '�':
		ch = A_ENIE_MAY;
		break;
	case '�':
		ch = A_ACC_A;
		break;
	case '�':
		ch = A_ACC_E;
		break;
	case '�':
		ch = A_ACC_I;
		break;
	case '�':
		ch = A_ACC_O;
		break;
	case '�':
		ch = A_ACC_U;
		break;
	default:
		ch = c;
		break;
	}
	return ch;
}

void move_cursor(int position){
	unsigned short p1 = 0x3D4;
	unsigned short p2 = 0x3D5;

	unsigned char low = position & 0xFF;
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
