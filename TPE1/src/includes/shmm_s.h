/*
 * shmm_s.h (Server)
 *
 */

#ifndef SHMM_S_H_
#define SHMM_S_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>

#include "../utils/LinkedList.h"
#include "../includes/transport_s.h"

#include "../includes/message.h"
#include "../includes/defines.h"
#include "../includes/share_ex.h"
#include "../includes/message_serialization.h"

typedef struct channel_t {
	Sem memory_lock_sem; // Locks the shared memory so no data is corrupted
	Sem clitoserv_sem;  // Signals the the server saying: "You have data to read"
	Sem servtocli_sem;  // Signals the the client saying: "You have data to read"
	void * buffer; // Where client will read and write data
} channel_t;

typedef channel_t * Channel;

 #include "../includes/execute.h"

void closeServer(char * server_path);

Msg_t IPClisten(Channel ch);

int communicate(Channel ch, Msg_s msg);

Msg_s establishChannel(Channel ch);

int sendmessage(Channel ch, Msg_s msg);

Channel createChannel(Msg_t msg);

Msg_t rcvmessage(Channel ch);

#endif /* SHMM_S_H_ */
