/*
 * contact_serialization.c
 *
 *  Created on: 29/03/2012
 *      Author: neon
 */

#include "../../includes/fifo_contact_serialization.h"

Msg_t deserialize_contact (Msg_t msg, void * stream){

	msg->data.tempnam = (char*)malloc(strlen((char*)stream));

	strcpy(msg->data.tempnam, (char*)stream);

	return msg;

}

void * serialize_contact (Msg_t msg){
	int tempnamSize;
	int msgSize;
	void * msgstraux;
	void * msgstr;

	tempnamSize = strlen(msg->data.tempnam)+1;
	msgSize = sizeof(int) + tempnamSize;
	if((msgstraux = msgstr = calloc(msgSize+sizeof(int), sizeof(char))) == NULL){
		perror("code_contact: Not enough memory");
		exit(1);
	}
	memcpy(msgstraux, &(msgSize), sizeof(int));
	msgstraux += sizeof(int);
	memcpy(msgstraux, &(msg->type), sizeof(int));
	msgstraux += sizeof(int);
	strcpy((char*)msgstraux, msg->data.tempnam);

	return msgstr;
}
