/*
 * contact_serialization.c
 *
 */

#include "../../includes/mq_contact_serialization.h"

Msg_t deserialize_contact (Msg_t msg, void * stream){

	printf("Deserializing contact message...\n");
	printf("\nCONTACT message received\n");
	printf("\n<data>\n");
	memcpy(&(msg->pidFrom), stream, sizeof(int));
	stream += sizeof(int);
	printf("\tCLIENT_PID = %d \n", msg->pidFrom);
	printf("</data>\n\n");

	return msg;

}

void * serialize_contact (Msg_t msg){
	printf("Serializing contact message ... \n");
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = 2*sizeof(int);
	if((msgstraux = msgstr = calloc(msgSize+sizeof(int), sizeof(char))) == NULL){
		perror("code_contact: Not enough memory");
		exit(1);
	}

	printf("Serialize size = %d\n", msgSize);

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);

	memcpy(msgstraux, &(msg->type), sizeof(int));
	printf("Serialize type = %d\n", msg->type);
	msgstraux += sizeof(int);
	
	int client_pid = msg->pidFrom;

	memcpy(msgstraux, &client_pid, sizeof(int));
	printf("Serialize PID = %d\n", client_pid);
	msgstraux += sizeof(int);

	return msgstr;
}
