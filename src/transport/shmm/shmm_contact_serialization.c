/*
 * contact_serialization.c
 *
 */

#include "../../includes/shmm_contact_serialization.h"

Msg_t deserialize_contact (Msg_t msg, void * stream){


	return msg;

}

void * serialize_contact (Msg_t msg){
	int msgSize;
	void * msgstraux;
	void * msgstr;

	msgSize = sizeof(int);

	if((msgstraux = msgstr = calloc(msgSize+sizeof(int), sizeof(char))) == NULL){
		perror("code_contact: Not enough memory");
		exit(1);
	}
	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);

	return msgstr;
}