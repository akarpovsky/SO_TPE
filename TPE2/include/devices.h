/*
 * devices.h
 *
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include "defs.h"
#include "structs.h"
#include "limits.h"

struct screen_t main_screen = {
    (char *) 0x0B8000,
    SCREEN_ROWS*SCREEN_COLS*2
};

struct ttyScreen_t screens[TTY_NUMBER] = {{
    TTY_SCREEN_SSTART
},{
    TTY_SCREEN_SSTART
}/*,{
    TTY_SCREEN_SSTART
},{
    TTY_SCREEN_SSTART
}*/} ;

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
}/*,{
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
}*/} ;

struct shellLine_t shellLine[TTY_NUMBER] = {{
	0,
	S_BUFFER_SIZE
},{
	0,
	S_BUFFER_SIZE
}/*,{
	0,
	S_BUFFER_SIZE
},{
	0,
	S_BUFFER_SIZE
}*/};


struct system_t system = {
    0
};


#endif /* DEVICES_H_ */
