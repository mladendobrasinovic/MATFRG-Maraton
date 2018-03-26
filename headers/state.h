/* Definicije globalnih promenljivih stanja */
#ifndef _STATE
#define _STATE

#include <GL/freeglut.h>
#include "timer.h"
#include "track.h"

extern struct tick_key curr_tick;
extern GLfloat avatar_x_shift, avatar_z_shift, avatar_y_shift;
extern bool game_running;

extern segment_t curr_seg;

void init_state();

#endif
