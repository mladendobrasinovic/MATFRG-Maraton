#include "maraton.h"

struct tick_key curr_tick;
GLfloat avatar_z_shift, avatar_x_shift, avatar_y_shift;
GLfloat avatar_h = 1;			   /* Visina polja */
GLfloat field_w = M_SQRT_2, field_h = 0.3; /* Sirina i visina polja */
bool game_running;

segment_t curr_seg;

void init_state()
{
   /* Inicijalizuj vrednosti promenljivih stanja */
    init_seg(curr_seg);
    curr_tick = null_tick;
    
    avatar_x_shift = 0;		/* Avatar je na centralnom redu */
    avatar_z_shift = 1;		/* Avatar je na drugoj koloni */
    avatar_y_shift = 0;	        /* Avatar stoji na nivou poda */

    game_running = true;
}
