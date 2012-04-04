#ifndef MQ_C_H
#define MQ_C_H

#define MAXSIZE	128
#define FTOK_ID 1
#define FTOK_FILE "/tmp/msgq"
#define MAIN_SERVER_PRIORITY 1
#define MAX_STRING 200
#define SUCCESSFUL 1

Msg_s communicate(Msg_t msg);

Msg_s _rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServer(void);


#endif