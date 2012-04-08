/*
 * contact_serialization.c
 *
 */

#include "../../includes/mq_contact_serialization.h"

Msg_t deserialize_contact (Msg_t msg, void * stream){

	printf("Deserializing contact message...\n");
	printf("\nCONTACT message received\n");
	printf("\n<data>\n");
	memcpy(&(msg->data.pidFrom), stream, sizeof(int));
	stream += sizeof(int);
	printf("\tCLIENT_PID = %d \n", msg->data.pidFrom);
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

	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);

	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	
	int client_pid = msg->data.pidFrom;

	memcpy(msgstraux, &client_pid, sizeof(int));
	msgstraux += sizeof(int);

	return msgstr;
}