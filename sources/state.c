#include "maraton.h"

GLfloat avatar_h = 1;			   /* Visina polja */
GLfloat slide_v = 4.0;
GLfloat field_w = M_SQRT2, field_h = 0.3; /* Sirina i visina polja */

/* Lebdeci novcici, mali u odnosu na igraca. */
GLfloat coin_radius = 0.36;
GLfloat coin_height = 0.42;

bool game_running;
unsigned long long score;
int score_timer;
int coin_timer;

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

    score = 0;
    score_timer = 0;
}
