#include "../include/devices.h"

void initializeTTYScreens() {
	int i, j;
	for (i = 0; i < TTY_NUMBER; i++) {
		for (j = 0; j < SCREEN_SIZE; j++) {
			ttys[i].screen->buffer[j++] = 0;
			ttys[i].screen->buffer[j] = WHITE_TXT;
		}
	}
}

