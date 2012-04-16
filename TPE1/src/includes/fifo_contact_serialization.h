/*
 * fifo_contact_serialization.h
 *
 */

#ifndef FIFO_CONTACT_SERIALIZATION_H_
#define FIFO_CONTACT_SERIALIZATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "message.h"
#include "../utils/LinkedList.h"

void * serialize_contact (Msg_t msg);
Msg_t deserialize_contact (Msg_t msg, void * stream);

#endif /* FIFO_CONTACT_SERIALIZATION_H_ */
