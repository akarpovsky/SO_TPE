/*
 * socket_s.h (Server)
 *
 */

#ifndef SOCKET_S_H_
#define SOCKET_S_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

 typedef struct channel_t {
	struct sockaddr_in * client;
	int port;
} channel_t;

typedef channel_t * Channel;

#include "../includes/message.h"
#include "../includes/execute.h"
#include "../includes/defines.h"
#include "../includes/transport_s.h"
#include "../utils/LinkedList.h"
#include "../utils/hashmap.h"
#include "../includes/message_serialization.h"
#include "sockets_contact_serialization.h"



#define SAME_MACHINE_CONNECTION AF_INET
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_in)

#define UNIX_PATH_MAX    108
#define SERVER_PATH "/tmp/socket_server"



void closeServer(char * server_path);

struct sockaddr_in * getClientChannel(int pid);

struct sockaddr_in * getServerAddress();

Msg_t IPClisten(Channel ch);

int communicate(Channel ch, Msg_s msg);

Msg_s establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

#endif /* SOCKET_S_H_ */
