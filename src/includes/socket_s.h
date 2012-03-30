/*
 * socket_s.h (Server)
 *
 */

#ifndef SOCKET_S_H_
#define SOCKET_S_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>

#include "../includes/message.h"
#include "../includes/defines.h"

#define SAME_MACHINE_CONNECTION AF_INET
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_in)

#define UNIX_PATH_MAX    108
#define SERVER_PATH "/tmp/socket_server"

typedef struct channel_t {
	struct sockaddr_in * client;
	int port;
} channel_t;

typedef channel_t * Channel;

void closeServer(char * server_path);

struct sockaddr_in * getClientChannel(int pid);

struct sockaddr_in * getServerAddress();

Msg_t IPClisten(Channel ch);

int communicate(Channel ch, Msg_s msg);

Msg_s establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

#endif /* SOCKET_S_H_ */
