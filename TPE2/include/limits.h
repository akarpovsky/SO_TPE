/*
 * limits.h
 *
 */

#ifndef _LIMITS_H_
#define _LIMITS_H_

#define CHAR_BIT			8
#define CHAR_MIN 	 	 -128
#define CHAR_MAX  	  	  127
#define SCHAR_MIN 	 	 -128
#define SCHAR_MAX	  	  127
#define MB_LEN_MAX			1


#define SHRT_MIN    (-32767-1)
#define SHRT_MAX		32767
#define USHRT_MAX	   0xFFFF

#define INT_MIN	 (-2147483647-1)
#define INT_MAX	   2147483647
#define UINT_MAX   0xFFFFFFFF

#define _NO_LIMIT		  100

#define NGROUPS_MAX			   0
#define ARG_MAX			   16384
#define CHILD_MAX	   _NO_LIMIT
#define OPEN_MAX		  	 127
#define LINK_MAX		  	 127
#define MAX_CANON		  	 255
#define MAX_INPUT		  	 255
#define FILENAME_MAX	  	 512
#define PATH_MAX		 	4096
#define PIPE_BUF		 	8192
#define STREAM_MAX		  	 127
#define TZNAME_MAX			   3
#define SSIZE_MAX		   32767
#define PROCNAME_MAX		 128
#define MAX_PROCESSES 		127//4096
#define MAX_PRIORITIES 		   5

/* Dimensiones de la pantalla */
#define SCREEN_ROWS	80
#define SCREEN_COLS	25
#define SCREEN_POS	0xb8000
#define SCREEN_SIZE	4000
#define SCREEN_LAST_ROW	3840
#define SCREEN_ROW_SIZE	160
#define SCREEN_TAB_SIZE	8

/* Buffer de teclado */

#define K_BUFFER_SIZE	10

/* Buffer de la linea de comando */

#define S_BUFFER_SIZE	1024


#endif /* LIMITS_H_ */
