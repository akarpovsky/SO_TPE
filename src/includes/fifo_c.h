/*
 * fifo_c.h
 *
 */

#ifndef FIFO_C_H_
#define FIFO_C_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "message.h"
#include "marshalling.h"
#include "defines.h"

#define FIFO_OUT "/tmp/fifo"

/**
 * Sends a message Msg_t msg to a server. Returns a Msg_s from the server.
 */

Msg_s communicate(Msg_t msg);

/**
 * This is the low-level function that waits until it recieves a message from
 * the server. Returns a Msg_s.
 */

Msg_s rcvmessage(void);

/**
 * This is the low-level function that sends a Msg_t msg to the server. It
 * returns SUCCESSFULL in case of success.
 */

int sendmessage(Msg_t msg);

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

#endif /* FIFO_C_H_ */
