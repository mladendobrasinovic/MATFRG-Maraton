#include "maraton.h"

/* Inicijalne vrednosti promenljivih stanja */
struct tick_key curr_tick = {false, false, false, false};
float avatar_shift = 2.0;

segment_t curr_seg;

void init_state()
{
    init_seg(curr_seg);
}
