/* Definicije globalnih promenljivih stanja */
#ifndef _STATE
#define _STATE

#include "timer.h"
#include "track.h"

extern struct tick_key curr_tick;
extern float avatar_shift;
extern segment_t curr_seg;

void init_state();

#endif
