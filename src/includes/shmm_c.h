/*
 * shmm_c.h (Client)
 *
 */

#ifndef SHMM_C_H_
#define SHMM_C_H_

 #include "../includes/message.h"


#include "../includes/message.h"
#include "../utils/LinkedList.h"
#include "../includes/defines.h"
#include "../includes/transport_c.h"
 #include "../includes/message_serialization.h"




Msg_s communicate(Msg_t msg);

Msg_s rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServer(void);


#endif /* SHMM_C_H_ */