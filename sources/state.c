#include "maraton.h"

struct tick_key curr_tick;
GLfloat avatar_z_shift, avatar_x_shift;

segment_t curr_seg;

void init_state()
{
   /* Inicijalizuj vrednosti promenljivih stanja */
    init_seg(curr_seg);
    curr_tick = null_tick;
    
    avatar_x_shift = 0;
    avatar_z_shift = 1;
}
