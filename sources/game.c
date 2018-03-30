#include "maraton.h"

unsigned int move_dur = 250;
unsigned int jump_dur = 500;	/* Ekvivalentno vremenu za koje kocka predje 2
				 * polja po z-osi */
GLfloat grav = tick_scale(G_CONST);

/* Parametri animacije */
GLfloat start, end;
float t;			/* Ocekujemo da parameter t ide od 0 do 1 */
GLfloat vy;

bool move_side(void)
{
    GLfloat diff = end - start;

    if(t >= 1.0)
    {
	avatar.x = end;	
	return true;
    }
    
    /* Animiramo pomeranje nalik funkciji sin na [0, 90] tj. [0. pi/2] */
    avatar.x = start + (GLfloat)sin(t * (M_PI / 2)) * diff;

    t += interval_scale(move_dur);
    return false;
}

bool jump(void)
{
    GLfloat y_floor = 0;
    float max_t = (float)jump_dur / SEC_INTERVAL;
    /* Racunamo visinu tako da sinusoidna putanja aproksimira parabolu koja
       odgovara konstanti gravitacije */
    GLfloat height = (G_CONST * max_t * M_PI) / (2 * avatar.vz);

    if(t >= 1.0)
    {
	avatar.y = y_floor;
	avatar.in_jump = false;

	/* Postavlji trenutnu brzinu za slucaj da se pad nastavlja (tj. ako
	 * padne van staze) */
	avatar.vy = height * M_PI / max_t;
	return true;
    }

    /* Animiramo skok kao sinusoidnu funkciju na intervalu [0, pi] */
    avatar.y = (GLfloat)sin(t * M_PI) * height;

    t += interval_scale(jump_dur);
    return false;
}

bool drop(void)
{
    avatar.y = avatar.y - tick_scale(avatar.vy);
    avatar.vy = avatar.vy + grav;
    
    /* Ova animacija ignorise parametar t */
    return false;
}

anim_t start_side_move(GLfloat shift)
{
    t = 0.0;
    start = avatar.x;
    end = start + shift;

    return move_side;
}

anim_t start_jump()
{
    t = 0.0;
    avatar.in_jump = true;
    avatar.in_drop = true;

    return jump;
}

anim_t start_drop()
{
    if(!avatar.in_drop)
	/* Ako je avatar vec u padu ne treba mu menjati brzinu po Y-osi */
    {
	/* FIX: Napravi transfer energije iz vz u vy */
	avatar.vy = avatar.vz * 2; /* Ovako izgleda normalnije */
    }

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
    i = roundf(avatar.z);
    j = 2 + roundf(avatar.x);

    /* Proveri da li je avatar centriran nad postojecim poljem */
    if(collide_field(i, j))
	return true;

    /* Proveri da li avatar stoji na dodatnom kolonom, i odredi je */
    ip = roundf(avatar.z + avatar_dim);
    if(ip != i)
	it = true;
    else
    {
	ip = roundf(avatar.z - avatar_dim);
	if(ip != i)
	    it = true;
    }

    /* Proveri da li avatar stoji nad dodatnim redom, i odredi ga */
    jp = 2 + roundf(avatar.x + avatar_dim);
    if(jp != j)
	jt = true;
    else
    {
	jp = 2 + roundf(avatar.x - avatar_dim);
	if(jp != j)
	    jt = true;
    }

    /* Proveri da li ima susedno polje nad kojim avatar stoji koje moze da ga
     * podrzi */
    if(it && collide_field(ip, j))
	return true;
    if(jt && collide_field(i, jp))
	return true;
    if(it && jt && collide_field(ip, jp))
	return true;

    return false;
}

void avatar_update()
{
    avatar.z += tick_scale(avatar.vz);

    /* Ako je animacija u toku, izvrsi je, i prekini ako je potrebno */
    if(avatar.anim != NULL && avatar.anim())
	avatar.anim = NULL;
    
    /* Obrada kolizije sa stazom */
    if(game_running && !avatar.in_jump)
    {
	if(!collide_track())
	{
	    printf("GAME OVER: [%d, %d]\n",
		   (int)roundf(avatar.x),
		   (int)roundf(avatar.z));
	    avatar.anim = start_drop();
	    game_running = false;
	}
	else
	{
	    /* Avatar je na stazi */
	    avatar.in_drop = false;
	}
    }
}

