/*
 * fifo_c.h
 *
 */

#ifndef FIFO_C_H_
#define FIFO_C_H_

#define FIFO_OUT "/tmp/fifo"

Msg_s communicate(Msg_t msg);

Msg_s rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServe(void);


#endif /* FIFO_C_H_ */
