/*
 * fifo_s.h (Server)
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


#define SAME_MACHINE_CONNECTION AF_UNIX
#define QUEUE_CONNECTION_SIZE 5
#define SOCKET_SIZE sizeof(struct sockaddr_un)

#define UNIX_PATH_MAX    108
#define SERVER_PATH "/tmp/socket_server"




struct sockaddr_un {
	sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[UNIX_PATH_MAX];  /* pathname */
 };

 typedef struct channel_t {
	struct sockaddr_un * client;
	int sockfd;
} channel_t;

typedef channel_t * Channel;


void uplink(void);

Msg_t IPClisten(void);

int communicate(Channel ch, Msg_s msg);

int establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

void closeServer(char * server_path);

struct sockaddr_un * getClientChannel(char * path, int socket_family);

struct sockaddr_un * getServerAddress();

void sigint();


#endif /* SOCKET_S_H_ */
