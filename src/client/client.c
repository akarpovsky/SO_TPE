/*

	Client

*/


void * PrintThread(void * threadID){

}



int main(void){
	
	pthread_t printThread;
	int rc;
	
	printf("In main thread creating thread\n");
	rc = pthread_create(&printThread, NULL, PrintThread, (void *) t);
	if(rc){
		perror("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}






	/* Last thing that main() should do */
	pthread_exit(NULL);
}




