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
#include <sys/fcntl.h>
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



struct sockaddr_in * getClientChannel(int pid);

struct sockaddr_in * getServerAddress();

/**
 * This function establishes a connection to the main server so in can
 * communicate with incoming clients.
 */

void uplink(void);

/**
 * This function waits in its own Channel ch until it recieves a message, then
 * it passes it back to the calling function.
 *
 * In case of ch being NULL it uses the default Channel stablished for the main server.
 */

Msg_t IPClisten(Channel ch);

/**
 * This function recieves a Msg_s msg to send to the client by a Channel ch. Returns
 * SUCCESFULL in case of success.
 */

int communicate(Channel ch, Msg_s msg);

/**
 * Given an already created Channel ch, it finishes to complete the structure in order
 * to finish the connection between the server and the client.
 */

Msg_s establishChannel(Channel ch);

/**
 * This function sends a Msg_s msg to a Channel ch, and returns SUCCESSFULL in case of
 * success. This function is the low level function wrapped by communicate.
 */

int sendmessage(Channel ch, Msg_s msg);

/**
 * This function creates a Channel for a given Msg_t msg from a Client, provided the
 * message msg is of type CONTACT. Else, the behavior is unexpected.
 * In case of failure returns NULL.
 */

Channel createChannel(Msg_t msg);

/**
 * This function gracefully closes the connection to the main server. Is supposed to
 * be called once the main server exits.
 */

void closeMainServer(void);

/**
 * This function gracefully closes the connections of the service provider assigned
 * to a client. Is supposed to be called at the end of the execution of the such
 * said service provider.
 */

void closeConnection(Channel ch);

/**
 * This function tries to read a message from the Channel ch, if no message has been
 * sent, then returns NULL.
 */
Msg_t rcvmessage(Channel ch);

#endif /* SOCKET_S_H_ */
