/*
 * fifo_c.h
 *
 *  Created on: 22/03/2012
 *      Author: neon
 */

#ifndef FIFO_C_H_
#define FIFO_C_H_
#define FIFO_OUT "/tmp/fifo"

msg_s * communicate(msg_t * msg);

msg_s * rcvmessage(void);

int sendmessage(msg_t * msg);

void connectToServe(void);


#endif /* FIFO_C_H_ */
