/**
	Client communication.c

**/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "defines.h"
#include "structs.h"


void * communicate(int ACTION_TYPE, ...){
  
	switch(ACTION_TYPE){
		
		case REGISTER:
		{	
			break;
		}
		
		case LOG_IN:
		{
			// For variable arguments usage
			va_list argumentList;
			va_start( argumentList, ACTION_TYPE );

			char * username = va_arg( argumentList, char * );
			char * password = va_arg( argumentList, char * );

			va_end( argumentList );
			
			User usr = login_s(username, password);
			
			return usr;
		 }   				
		default:
		{
			break;
		}
	}


}
