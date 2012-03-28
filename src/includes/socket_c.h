/*
 * socket_c.h (Client)
 *
 */

#ifndef SOCKET_C_H_
#define SOCKET_C_H_


Msg_s communicate(Msg_t msg);

Msg_s rcvmessage(void);

int sendmessage(Msg_t msg);

void connectToServer(void);


#endif /* SOCKET_C_H_ */
