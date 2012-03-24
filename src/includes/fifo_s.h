/*
 * fifo_s.h
 *
 */

#ifndef FIFO_S_H_
#define FIFO_S_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/message.h"
#include "../includes/defines.h"

#define FIFO_IN "/tmp/fifo"

typedef struct channel_t {
	char * fifoIn;
	char * fifoOut;
	int fdIn;
	int fdOut;
} channel_t;

typedef channel_t * Channel;

void uplink(void);

Msg_t listen(void);

int communicate(Channel ch, Msg_s msg);

int establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);


#endif /* FIFO_S_H_ */
