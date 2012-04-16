/*
 * sockets_contact_serialization.h
 *
 */

#ifndef SOCKETS_CONTACT_SERIALIZATION_H_
#define SOCKETS_CONTACT_SERIALIZATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "message.h"
#include "../utils/LinkedList.h"

void * serialize_contact (Msg_t msg);
Msg_t deserialize_contact (Msg_t msg, void * stream);

#endif 