#ifndef _GAME
#define _GAME

#define MOVE_LEFT (-1.0)
#define MOVE_RIGHT 1.0

GLfloat tick_scale(GLfloat);

typedef bool (*anim_t)(void);

anim_t start_side_move(GLfloat);
anim_t start_jump();
anim_t start_drop();
bool collide_track();

#endif
