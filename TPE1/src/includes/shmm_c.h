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


/**
 * Sends a message Msg_t msg to a server. Returns a Msg_s from the server.
 */

Msg_s communicate(Msg_t msg);

/**
 * This is the low-level function that waits until it recieves a message from
 * the server. Returns a Msg_s.
 */

Msg_s _rcvmessage(void);

/**
 * This is the low-level function that sends a Msg_t msg to the server. It
 * returns SUCCESSFULL in case of success.
 */

int sendmessage(Msg_t msg);

/**
 * This is the non-blocking function used in the case of a draft state.
 */

Msg_s rcvmessage(void);

/**
 * This function contacts to the main server in order to obtain an attention
 * server. This way the it does the operations needed for the subsecuent functions
 * from the Transport Layer to behave as expected.
 */

void connectToServer(void);

/**
 * This function gracefuly closes the connection to the main server. Is supposed
 * to be called once in the client is about to shut down.
 */

void closeConnection(void);

#endif /* SHMM_C_H_ */