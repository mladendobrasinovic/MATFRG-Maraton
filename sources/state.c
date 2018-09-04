#include "maraton.h"

GLfloat avatar_h = 1;			   /* Visina polja */
GLfloat slide_v = 4.0;
GLfloat field_w = M_SQRT2, field_h = 0.3; /* Sirina i visina polja */

/* Lebdeci novcici, mali u odnosu na igraca. */
GLfloat coin_radius = 0.36;
GLfloat coin_height = 0.42;

/* Nagrada za prelazak distance, veca od novcica. */
GLfloat bonus_radius = 0.46;
GLfloat bonus_height = 0.46;    

bool game_running;
bool game_paused;
bool game_starting;
unsigned long long score;
int score_timer;
int coin_timer;
int coin_death_timer;
unsigned long long distance_score;

struct tick_key curr_tick;
segment_t *curr_seg, *prev_seg, *next_seg;
avatar_t avatar;

void init_state()
{
    /* Inicijalizuj vrednosti promenljivih stanja */
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
    game_paused = false;
    game_starting = true;

    score = 0;
    score_timer = 0;
    coin_timer = 0;
    coin_death_timer = 0;
    distance_score = 0;

    init_track();
}
