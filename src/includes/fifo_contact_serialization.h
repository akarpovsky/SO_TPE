/*
 * fifo_contact_serialization.h
 *
 *  Created on: 29/03/2012
 *      Author: neon
 */

#ifndef FIFO_CONTACT_SERIALIZATION_H_
#define FIFO_CONTACT_SERIALIZATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "../utils/LinkedList.h"

void * serialize_contact (Msg_t msg);
Msg_t deserialize_contact (Msg_t msg, void * stream);

#endif /* FIFO_CONTACT_SERIALIZATION_H_ */
