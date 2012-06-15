/* string.c */

#include "../include/strings.h"

int strlen(const char *str){
	int i = 0;

	while(*str){
		str++;
		i++;
	}

	return i;
}

char * strcpy(char* d, const char* s){
	while(*s != '\0')
		*(d++) = *(s++);
	*(++d) = '\0';
	return d;
}

int streq(char* s, char* t) {

    for(; *s == *t; s++, t++){
        if (*s == 0 && *t == 0)
            return TRUE;
        else if( (*s == 0 && *t != 0)  || (*s != 0 && *t == 0))
        	return FALSE;
    }
    return FALSE;

}

int atoi (char * s)
{
	int i;
	i=0;
	while(*s)
	{
		i=(i<<3) + (i<<1) + (*s - '0');
		s++;


	}
	return(i);
}
