#ifndef _GAME
#define _GAME

#define MOVE_LEFT (-1.0)
#define MOVE_RIGHT 1.0

#define tick_scale(dist) ((GLfloat)dist / TICK_RATE)
/* Procena vrednosti jednog otkucaja igre u odnosu na interval msec */
#define interval_scale(msec) (TIMER_INTERVAL / (float)msec)

typedef bool (*anim_t)(void);

void avatar_update();
anim_t start_side_move(GLfloat);
anim_t start_jump();
anim_t start_drop();
bool collide_track();

#endif
