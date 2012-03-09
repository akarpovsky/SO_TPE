/**
	Server.c

**/

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int register_s(char * username, char * password){

//TODO: Validar si el user existe, crear XML, etc;

	return 1;

}

User login_s(char * username, char * password){

/*
	//Validate username and password	
	User user = validUser(username, password);


	if(user == null){
		return null;
	}else{
		return user;
	}
	
*/

	User user;
	user = (User) malloc(sizeof(User));
	user->username = username;
	user->password = password;
	return user;
	
	
}


