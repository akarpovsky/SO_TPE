/*
 * shmm_c.h (Client)
 *
 */

#ifndef SHMM_C_H_
#define SHMM_C_H_


Msg_s communicate(Msg_t msg);

Msg_s rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServer(void);


#endif /* SHMM_C_H_ */