#include "maraton.h"

struct
{
    GLfloat start, end;
    float t;			/* Ocekujemo da parameter animacije t ide od 0
				 * do 1 */
} par;

GLfloat tick_scale(GLfloat dist)
{
    return dist / TICK_RATE;
}

bool move_side(void)
{
    GLfloat diff = par.end - par.start;

    if(par.t >= 1.0)
    {
	avatar_x_shift = par.end;	
	return true;
    }
    
    /* Animiramo pomeranje nalik funkciji sin na [0, 90] tj. [0. pi/2] */
    avatar_x_shift = par.start + (GLfloat)sin(par.t * (M_PI / 2)) * diff;

    par.t += (1.0 / (TICK_RATE)) * 6;
    return false;
}

bool jump(void)
{
    
}

anim_t start_side_move(GLfloat shift)
{
    par.start = avatar_x_shift;
    par.end = par.start + shift;
    par.t = 0.0;

    return move_side;
}

anim_t start_jump()
{
    
}
