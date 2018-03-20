#include "maraton.h"

struct tick_key curr_tick;
float avatar_shift;

segment_t curr_seg;

void init_state()
{
   /* Inicijalizuj vrednosti promenljivih stanja */
    init_seg(curr_seg);
    curr_tick = null_tick;
    avatar_shift = 1;
}
