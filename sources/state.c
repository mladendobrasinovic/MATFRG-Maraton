#include "maraton.h"

GLfloat avatar_h = 1;			   /* Visina polja */
GLfloat slide_v = 4.0;
GLfloat field_w = M_SQRT_2, field_h = 0.3; /* Sirina i visina polja */

bool game_running;

struct tick_key curr_tick;
segment_t *curr_seg, *prev_seg, *next_seg;
avatar_t avatar;

void init_state()
{
    /* Inicijalizuj vrednosti promenljivih stanja */
    init_track();
    
    curr_tick = null_tick;
    
    /* Inicijalizuj poziciju avatara (centralni red, druga kolona, nivo poda) */
    avatar.x = 0;
    avatar.z = 1;
    avatar.y = 0;
    
    /* Inicijalizuj brzinu avatara */
    avatar.vz = 4.0;
    avatar.vx = 0;
    avatar.vy = 0;
    
    avatar.anim = NULL;
    avatar.in_jump = false;
    avatar.in_drop = false;

    /* Jednostavna promenljiva stanja igre */
    game_running = true;
}
