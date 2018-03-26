#include "maraton.h"

/* Parametri animacije */
GLfloat start, end;
float t;			/* Ocekujemo da parameter t ide od 0 do 1 */
GLfloat vy;

GLfloat tick_scale(GLfloat dist)
{
    return dist / TICK_RATE;
}

bool move_side(void)
{
    GLfloat diff = end - start;

    if(t >= 1.0)
    {
	avatar_x_shift = end;	
	return true;
    }
    
    /* Animiramo pomeranje nalik funkciji sin na [0, 90] tj. [0. pi/2] */
    avatar_x_shift = start + (GLfloat)sin(t * (M_PI / 2)) * diff;

    t += (1.0 / (TICK_RATE)) * 6;
    return false;
}

bool jump(void)
{
    return false;
}

bool drop(void)
{
    GLfloat g = tick_scale(9.8);

    avatar_y_shift = avatar_y_shift - vy;
    vy = vy + g;

    return false;
}

anim_t start_side_move(GLfloat shift)
{
    t = 0.0;
    start = avatar_x_shift;
    end = start + shift;

    return move_side;
}

anim_t start_jump()
{
    t = 0.0;

    return jump;
}

anim_t start_drop()
{
    t = 0.0;
    vy = 0.0;

    return drop;
}

bool collide_field(int i, int j)
{
    if(j < 0 || j >= 5)
	/* Polje je van sirine staze */
	return false;
    
    if(i >= 0 && i < SEG_LENGTH)
    {
	if(curr_seg[i][j] != FLD_X)
	    return true;
    }

    return false;
}

bool collide_track()
/* Proverava da li staza podrzava trenutnu poziciju avatara */
{
    bool it = false, jt = false;
    int i, j, ip, jp;
    GLfloat avatar_dim = (avatar_h / field_w) / 2; /* Sirina avatara u odnosu na
						    * meru polja */
    
    /* Nadji polje nad kojim se nalazi teziste */
    i = roundf(avatar_z_shift);
    j = 2 + roundf(avatar_x_shift);

    if(collide_field(i, j))
	return true;


    /* Proveri da li avatar stoji na dodatnom kolonom, i odredi je */
    ip = roundf(avatar_z_shift + avatar_dim);
    if(ip != i)
	it = true;
    else
    {
	ip = roundf(avatar_z_shift - avatar_dim);
	if(ip != i)
	    it = true;
    }

    /* Proveri da li avatar stoji nad dodatnim redom, i odredi ga */
    jp = 2 + roundf(avatar_x_shift + avatar_dim);
    if(jp != j)
	jt = true;
    else
    {
	jp = 2 + roundf(avatar_x_shift - avatar_dim);
	if(jp != j)
	    jt = true;
    }

    if(it && collide_field(ip, j))
	return true;
    if(jt && collide_field(i, jp))
	return true;
    if(it && jt && collide_field(ip, jp))
	return true;

    return false;
}
