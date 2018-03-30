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
extern struct tick_key curr_tick;
extern avatar_t avatar;
extern segment_t curr_seg;

void init_state();

#endif
