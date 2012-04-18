#include "../include/defs.h"
#include "../include/structs.h"

struct screen_t screen = {
    (char *) 0x0B8000,
    SCREEN_ROWS*SCREEN_COLS*2
};

struct ttyScreen_t screens[TTY_NUMBER] = {{
    TTY_SCREEN_SSTART
},{
    TTY_SCREEN_SSTART
},{
    TTY_SCREEN_SSTART
},{
    TTY_SCREEN_SSTART
}} ;

struct keyboard_t keyboards[TTY_NUMBER] = {{
    0,
    0,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    ENGLISH
},{
    0,
    0,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    ENGLISH
},{
    0,
    0,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    ENGLISH
},{
    0,
    0,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    ENGLISH
}} ;

struct shellLine_t shellLine[TTY_NUMBER] = {{
	0,
	S_BUFFER_SIZE
},{
	0,
	S_BUFFER_SIZE
},{
	0,
	S_BUFFER_SIZE
},{
	0,
	S_BUFFER_SIZE
}};


struct tty_t ttys[TTY_NUMBER] = {{
    TTY_1,
    &screens[0],
    &keyboards[0],
    &shellLine[0]
},{
    TTY_2,
    &screens[1],
    &keyboards[1],
    &shellLine[1]
},{
    TTY_3,
    &screens[2],
    &keyboards[2],
    &shellLine[2]
},{
    TTY_4,
    &screens[3],
    &keyboards[3],
    &shellLine[3]
}};



struct system_t system = {
    0
} ;

void initializeTTYScreens(){
    int i, j;
    for( i=0; i<TTY_NUMBER; i++){
	for ( j=0; j<SCREEN_SIZE ; j++){
	    ttys[i].screen->buffer[j++]=0;
	    ttys[i].screen->buffer[j]=WHITE_TXT;
	}
    }
}
 
