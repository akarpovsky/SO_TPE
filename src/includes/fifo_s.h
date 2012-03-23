/*
 * fifo_s.h
 *
 */

#ifndef FIFO_S_H_
#define FIFO_S_H_
#define FIFO_IN "/tmp/fifo"

typedef struct channel_t {
	char * fifoIn;
	char * fifoOut;
	int fdIn;
	int fdOut;
} channel_t;

typedef channel_t * Channel;

void uplink(void);

msg_t listen(void);

int communicate(Channel ch, msg_s * msg);

int establishChannel(Channel ch);

int sendmessage(Channel ch, msg_s * msg);

Channel createChannel(msg_t * msg);


#endif /* FIFO_S_H_ */
