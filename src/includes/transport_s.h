/*
 * transoprt_s.h (Server)
 * Transport interface
 */

#ifndef TRANSPORT_S_H_
#define TRANSPORT_S_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>

#include "../includes/message.h"
#include "../includes/defines.h"

typedef struct channel_t {
	struct sockaddr_un * client;
	int sockfd;
} channel_t;

typedef channel_t * Channel;

void uplink(void);

Msg_t IPClisten(Channel ch);

int communicate(Channel ch, Msg_s msg);

int establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

void sigint();

#endif