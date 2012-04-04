/*
 * fifo_s.h
 *
 */

#ifndef FIFO_S_H_
#define FIFO_S_H_

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "message.h"
#include "message_serialization.h"
#include "defines.h"

#define FIFO_IN "/tmp/fifo"

typedef struct channel_t {
	char * fifoIn;
	char * fifoOut;
	int fdIn;
	int fdOut;
} channel_t;

void sigint();

typedef channel_t * Channel;

/**
 * This function establishes a connection to the main server so in can
 * communicate with incoming clients.
 */

void uplink(void);

/**
 * This function waits in its own Channel ch until it recieves a message, then
 * it passes it back to the calling function.
 *
 * In case of ch being NULL it uses the default Channel stablished for the main server.
 */

Msg_t IPClisten(Channel ch);

/**
 * This function recieves a Msg_s msg to send to the client by a Channel ch. Returns
 * SUCCESFULL in case of success.
 */

int communicate(Channel ch, Msg_s msg);

/**
 * Given an already created Channel ch, it finishes to complete the structure in order
 * to finish the connection between the server and the client.
 */

Msg_s establishChannel(Channel ch);

/**
 * This function sends a Msg_s msg to a Channel ch, and returns SUCCESSFULL in case of
 * success. This function is the low level function wrapped by communicate.
 */

int sendmessage(Channel ch, Msg_s msg);

/**
 * This function creates a Channel for a given Msg_t msg from a Client, provided the
 * message msg is of type CONTACT. Else, the behavior is unexpected.
 * In case of failure returns NULL.
 */

Channel createChannel(Msg_t msg);

/**
 * This function gracefully closes the connection to the main server. Is supposed to
 * be called once the main server exits.
 */

void closeMainServer(void);

/**
 * This function gracefully closes the connections of the service provider assigned
 * to a client. Is supposed to be called at the end of the execution of the such
 * said service provider.
 */

void closeConnection(Channel ch);

/**
 * This function tries to read a message from the Channel ch, if no message has been
 * sent, then returns NULL.
 */
Msg_t rcvmessage(Channel ch);


#endif /* FIFO_S_H_ */
