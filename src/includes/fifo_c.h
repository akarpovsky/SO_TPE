/*
 * fifo_c.h
 *
 *  Created on: 22/03/2012
 *      Author: neon
 */

#ifndef FIFO_C_H_
#define FIFO_C_H_

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../includes/message.h"
#include "../includes/marshalling.h"
#include "../includes/defines.h"

#define FIFO_OUT "/tmp/fifo"

Msg_s communicate(Msg_t msg);

Msg_s rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServe(void);


#endif /* FIFO_C_H_ */
