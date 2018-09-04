/* Definicije globalnih promenljivih stanja */
#ifndef _STATE
#define _STATE

#include <GL/freeglut.h>
#include <stdbool.h>
#include "timer.h"
#include "track.h"
#include "game.h"

/* Konstante, deklarisane u state.c */
extern GLfloat avatar_h;
extern GLfloat slide_v;
extern GLfloat field_w, field_h;
extern GLfloat coin_radius, coin_height;

/* Globalne promenljive */
typedef struct
{
    GLfloat x, y, z;
    GLfloat vx, vy, vz;
    anim_t anim;
    bool in_drop;
    bool in_jump;
} avatar_t;

extern bool game_running;
extern unsigned long long score;
extern unsigned long long distance_score;
extern int score_timer;
extern int coin_timer;
extern int coin_death_timer;

extern struct tick_key curr_tick;
extern avatar_t avatar;
extern segment_t *curr_seg, *prev_seg, *next_seg;

void init_state();

#endif
