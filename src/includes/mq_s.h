
#ifndef MQ_S_H
#define MQ_S_H

#define FTOK_ID 1
#define FTOK_FILE "/tmp/msgq"
#define MAIN_SERVER_PRIORITY 1
#define MAX_STRING 1024

typedef struct msg_String{
    long  pid;  
	struct dataString{
		int fromPid;
		char string[MAX_STRING];
		}dataString;
}msg_String;

typedef struct msg_Int{
	long pid;
	struct dataInt{
		int num;
		int fromPid;
	}dataInt;
}msg_Int;


typedef struct channel_t {
	long pid; /* LO QUE EL THREAD ESCUCHA */
} channel_t;

typedef channel_t * Channel;

Msg_t IPClisten(Channel ch);

int communicate(Channel ch, Msg_s msg);

Msg_s establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

#endif