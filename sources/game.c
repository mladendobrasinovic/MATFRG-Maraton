#include "maraton.h"

unsigned int move_dur = 250;
unsigned int jump_dur = 500;	/* Ekvivalentno vremenu za koje kocka predje 2
				 * polja po z-osi. */
GLfloat grav = tick_scale(G_CONST);

/* Parametri animacije */
GLfloat start, end;
float t;			/* Ocekujemo da parameter t ide od 0 do 1. */
GLfloat vy;

bool move_side(void)
{
    GLfloat diff = end - start;

    if(t >= 1.0)
    {
	avatar.x = end;	
	return true;
    }
    
    /* Animiramo pomeranje nalik funkciji sin na [0, 90] tj. [0. pi/2]. */
    avatar.x = start + (GLfloat)sin(t * (M_PI / 2)) * diff;

    t += interval_scale(move_dur);
    return false;
}

bool jump(void)
{
    GLfloat y_floor = 0;
    float max_t = (float)jump_dur / SEC_INTERVAL;
    /* Racunamo visinu tako da sinusoidna putanja aproksimira parabolu koja
       odgovara konstanti gravitacije. */
    GLfloat height = (G_CONST * max_t * M_PI) / (2 * avatar.vz);

    if(t >= 1.0)
    {
	avatar.y = y_floor;
	avatar.in_jump = false;

	/* Postavljamo trenutnu brzinu za slucaj da se pad nastavlja (tj. ako
	 * padne van staze). */
	avatar.vy = height * M_PI / max_t;
	return true;
    }

    /* Animiramo skok kao sinusoidnu funkciju na intervalu [0, pi]. */
    avatar.y = (GLfloat)sin(t * M_PI) * height;

    t += interval_scale(jump_dur);
    return false;
}

bool drop(void)
{
    avatar.y = avatar.y - tick_scale(avatar.vy);
    avatar.vy = avatar.vy + grav;
    
    /* Ova animacija ignorise parametar t. */
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
	/* Ako je avatar vec u padu ne treba mu menjati brzinu po Y-osi. */
	avatar.vy = avatar.vz * 2; /* Ovako izgleda bolje, avatar menja smer
				    * kada pada sa ivice. */

    return drop;
}

bool collide_avatar_sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
    /* Avatarske koordinate blago transformisemo da bi bile uskladjene ostalim
     * objektima na stazi. */
    GLfloat ax = avatar.x + 2.0, ay = avatar.y, az = avatar.z;
    GLfloat dx, dy, dz;
    GLfloat bbox_limit;
    
    /* Bounding-box provera (Kocke su poravnate sa osama). */
    dx = SQUARE((ax - x) * field_w);
    dy = SQUARE((ay - y) * field_w);
    dz = SQUARE((az - z) * field_w);
    
    bbox_limit = SQUARE(avatar_h / 2 + r);
    /* Kvadrat sluzi izjednacavanju znakova. */
    if(dx > bbox_limit || dy > bbox_limit || dz > bbox_limit)
	return false;

    /* Provera kolizije sa sferom. */
    bool nearx, neary, nearz;
    int near_counter = 0;
    GLfloat length = avatar_h / 2;
    nearx = neary = nearz = false;

    /* Sada nam treba absolutna udaljenost sfere i kocke po osama. */
    dx = fabs((ax - x) * field_w);
    dy = fabs((ay - y) * field_w);
    dz = fabs((az - z) * field_w);

    /* Odredjujemo u kojem od odsecaka prostora koje odredjuje kocka se nalazi
     * centar sfere. */
    if(dx < length)
    {
	nearx = true;
	near_counter++;
    }
    if(dy < length)
    {
	neary = true;
	near_counter++;
    }
    if(dz < length)
    {
	nearz = true;
	near_counter++;
    }

    if(near_counter == 3)
	/* Ako je sfera je near_counter == 3, onda je centar unutar kocke. */
	return true;

    if(near_counter == 2)
	/* Sfera je naspram jedne (nepoznate) strane kocke, ali zbog bounding-box provere
	 * zakljucujemo da je sfera dovoljno blizu da je dotakne. */
	return true;

    if(near_counter == 1)
    {
	/* Sfera je naspram ivice kocke, racunamo udaljenost od centra sfere do
	 * kocke. */
	if(nearx)
	    return SQUARE(dz - length) + SQUARE(dy - length) < SQUARE(r);
	if(neary)
	    return SQUARE(dx - length) + SQUARE(dz - length) < SQUARE(r);
	/* nearz: */
	return SQUARE(dx - length) + SQUARE(dz - length) < SQUARE(r);
    }
    
    
    /* near_counter == 0: Ako je naspram temena kocke, racunamo udaljenost
     * centra sfere od temena. */
    return SQUARE(dx - length) + SQUARE(dy - length) + SQUARE(dz - length) < SQUARE(r);
}

bool collide_field(int i, int j)
	{
	    if(j < 0 || j >= 5)
		/* Polje je van sirine staze. */
		return false;

	    /* Proveravamo da li ima polja samo na trenutnom i sledecem, za prosli
	     * smatramo da nema potrebe. */
	    if(i >= 0 && i < SEG_LENGTH)
	    {
		if(curr_seg->track[i][j] != FLD_X)
		    return true;
	    }
	    else if(i >= SEG_LENGTH)
	    {
		if(next_seg->track[i - SEG_LENGTH][j] != FLD_X)
		    return true;
	    }

	    return false;
	}

    bool collide_track()
    /* Proverava da li staza podrzava trenutnu poziciju avatara. */
    {
	bool it = false, jt = false;
	int i, j, ip, jp;
	GLfloat avatar_dim = (avatar_h / field_w) / 2; /* Sirina avatara u odnosu na
						    * meru polja. */
    
    /* Nalazimo polje nad kojim se nalazi teziste. */
    i = roundf(avatar.z);
    j = 2 + roundf(avatar.x);

    /* Proveravamo da li je avatar centriran nad postojecim poljem. */
    if(collide_field(i, j))
	return true;

    /* Proveravamo da li avatar stoji na dodatnom kolonom, odredjujemo je. */
    ip = roundf(avatar.z + avatar_dim);
    if(ip != i)
	it = true;
    else
    {
	ip = roundf(avatar.z - avatar_dim);
	if(ip != i)
	    it = true;
    }

    /* Proveravamo da li avatar stoji nad dodatnim redom, odredjujemo ga. */
    jp = 2 + roundf(avatar.x + avatar_dim);
    if(jp != j)
	jt = true;
    else
    {
	jp = 2 + roundf(avatar.x - avatar_dim);
	if(jp != j)
	    jt = true;
    }

    /* Proveravamo da li ima susedno polje nad kojim avatar stoji koje moze da
     * ga podrzi. */
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

    /* Ako je animacija u toku, izvrsi je, i prekini ako je potrebno. */
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
	    /* Avatar je na stazi. */
	    avatar.in_drop = false;
	}
    }
}

int timer_mod(int point, int dur, int t)
/* Vraca vreme u odnosu na ciklicni tajmer. */
{
    /* point je trenutak cije vreme u animaciji merimo (u jednom punom ciklusu). */
    if(point < t)
	return t - point;
    else
	/* Korekcija za drugi ciklus. */
	return dur - point + t;
}

GLfloat coin_rotation(int rot_mod)
/* Odredjuje rotaciju novcica u skladu sa tajmerom. */
{
    int mod = timer_mod(rot_mod, (int)(COIN_BEAT * TICK_RATE), coin_timer);
    
    /* Izrazavamo razlomak celih brojeva float-om (u stepenima!). */
    return (360 * (GLfloat)(mod)) / (GLfloat)(COIN_BEAT * TICK_RATE);
}

GLfloat coin_scale(int death_mod)
/* Odredjuje faktor skaliranja za animaciju nestajanja objekata. */
{
    int mod = timer_mod(death_mod, (int)(COIN_DEATH_BEAT * TICK_RATE), coin_death_timer);

    return (GLfloat)cosf(((float)mod * (M_PI / 2)) / (COIN_DEATH_BEAT * TICK_RATE));
}

void pickup_coins()
/*  */
{
    int i;
    bonus_t b = curr_seg->bonus;
        
    for(i = 0; i < curr_seg->len_coins; i++)
    {
	coin_t c = curr_seg->coins[i];

	if(c.type != COIN_NIL && !c.dying
	   && collide_avatar_sphere(c.x, coin_height, c.z, coin_radius / 2))
	{
	    switch(c.type)
	    {
	    case COIN_YEL:
		score += 13;
		break;
	    case COIN_RED:
		score += 21;
		break;
	    case COIN_BLU:
		score += 34;
		break;
	    default:
		break;
	    }
	    
	    curr_seg->coins[i].dying = true;
	    curr_seg->coins[i].death_mod = coin_death_timer;
	}
    }

    if(b.type != BONUS_NIL && !b.dying &&
       collide_avatar_sphere(b.x, bonus_height, b.z, bonus_radius))
    {
	switch(b.type)
	{
	case BONUS_CU:
	    score += 160;
	    break;
	case BONUS_AG:
	    score += 270;
	    break;
	case BONUS_AU:
	    score += 360;
	    break;
	default:
	    break;
	}
	curr_seg->bonus.dying = true;
	curr_seg->bonus.death_mod = coin_death_timer;
    }
}

void coin_cleanup(segment_t* seg)
/* Uklanja objekte cija je animacija smrti gotova, poziva se pre kolizije
 * novcica za ovaj otkucaj. */
{
    int i;
    bonus_t b = seg->bonus;
    
    for(i = 0; i < seg->len_coins; i++)
    {
	coin_t c = seg->coins[i];
	
	if(c.dying && c.death_mod == coin_death_timer)
	    seg->coins[i].type = COIN_NIL;
    }

    if(b.dying && b.death_mod == coin_death_timer)
	seg->bonus.type = BONUS_NIL;
}

void object_cleanup()
/* Uklanjanje nepostojecih objekata iz evidencije igre. */
{
    /* Ovo radimo sa prethodni segment u kome moze biti nestajucih novcica i za
     * sadasnji. */
    coin_cleanup(prev_seg);
    coin_cleanup(curr_seg);
}
