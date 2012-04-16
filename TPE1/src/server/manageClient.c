#include <stdio.h>      
#include <pthread.h> 
#include <stdlib.h>



void manageClient(void * channel){

	int logged = 0;
	User me;

	Channel ch = (Channel) channel;
	int ret;
	Msg_t msg;

	ret = establishChannel(ch);

	if(ret != SUCCESFUL){
		perror("Channel could not be stablished");
		pthread_exit(NULL);
	}

	
	
	msg = listen();

	if(msg->type == EXIT){
		
		//TODO
	}else{

	}

}