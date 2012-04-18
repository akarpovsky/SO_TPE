/* string.c */

#include "../include/strings.h"
#include "../include/defs.h"

int strlen(const char *str){
	int i = 0;

	while(*str){
		str++;
		i++;
	}

	return i;
}

int streq(char* s, char* t) {

    for(; *s == *t; s++, t++)
        if (*s == 0)
            return TRUE;
            
    return FALSE;

}
