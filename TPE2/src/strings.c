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

char * strcpy(char* d, const char* s){
	while(*(s++) != '\0')
		*(d++) = *s;
	*(++d) = '\0';
	return d;
}

int streq(char* s, char* t) {

    for(; *s == *t; s++, t++)
        if (*s == 0)
            return TRUE;
            
    return FALSE;

}
