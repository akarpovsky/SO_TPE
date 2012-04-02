#ifndef GLOBAL_H_
#define GLOBAL_H_


#ifdef DEFINE_VARIABLES
#define EXTERN /* nothing */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */

#include "./structs.h"

EXTERN pthread_mutex_t game_mutex;

EXTERN Game gameAux; /* Game structure; will store all the game data */

#endif
