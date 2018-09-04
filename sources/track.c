#include "maraton.h"

int seg_ind;
segment_t seg_pool[SEG_COUNT];
int level_count = 0;

GLfloat randf(GLfloat start, GLfloat end)
/* Generise float iz intervala. */
{
    return start + (GLfloat)((GLdouble)rand() / RAND_MAX) * (end - start);
}

int randrange(int start, int end)
{
    return (int)floorf(randf(start, end));
}

int binom(int n, float p)
/* Generise broj iz binomne distribucije ~ B[n, p]. */
{
    int i;
    int res = 0;

    for(i = 0; i < n; i++)
	if(randf(0, 1) < p)
	    res++;

    return res;
}

enum coin_type rand_coin()
/* Generise novcic u skladu sa trenutnim nivoom. */
{
    int yel, red, blu;
    float redf, bluf;
    float rand;

    /* Racunamo odnos tako da vrednost novcica napreduje. */
    yel = 9;
    red = 2 + (level_count) * 4;
    blu = (level_count / 2) * 3 +
	(level_count > 2 ? (level_count - 2) * 3 : 0);

    redf = (float)red / (yel + red + blu);
    bluf = redf + (float)blu / (yel + red + blu);

    rand = randf(0, 1);

    if(rand < redf)
	return COIN_RED;
    else if(rand < bluf)
	return COIN_BLU;
    else
	return COIN_YEL;
}

void set_field(segment_t *seg, int i, int j)
{
    /* XOR bitova na polju jedinice (parnosti) predstavlja
     * funkciju boje sahovnice. */
    if((j % 2 == 1) ^ (i % 2 == 0))
	seg->track[i][j] = FLD_W;
    else
	seg->track[i][j] = FLD_G;
}

void empty_seg(segment_t* seg)
/* Postavlja sva polja u seg na vrednost FLD_X. */
{
    /* Poznato je da je FLD_X == 0, postavljamo prazno polje. */
    memset(seg->track, FLD_X, sizeof(enum field_type) * SEG_LENGTH * TRK_WIDTH);
    
    seg->len_coins = 0;
}

enum patch_type
{
    SAFE, EASY, SEMI, HARD
};

int draw_safe(segment_t* seg, int start, int end)
/* Postavlja platformu od 5 do 7 polja, moze biti ogranicena na manje. */
{
    int i, j;
    
    /* Ocekujemo da end - start pripada 5-7.  */
    end = randrange(start + 5, end);

    for(i = start; i < end; i++)
	for(j = 0; j < TRK_WIDTH; j++)
	    set_field(seg, i, j);

    /* Vracamo broj obradjenih polja. */
    return end - start;
}

int draw_semi_safe(segment_t* seg, int start, int end)
/* Postavlja platformu od 2 do 4 polja, moze biti ogranicena na manje. */
{
    int i, j;
    
    end = randrange(start + 2, end);

    for(i = start; i < end; i++)
	for(j = 0; j < TRK_WIDTH; j++)
	    set_field(seg, i, j);

    return end - start;
}


int draw_obstacle(segment_t* seg, int start, int end)
/* Postavlja prepreku do 2 polja. */
{
    int i, j;
    int cutoff = 0;
    int jstart = 0, jend = 0;
    int volume, remove = 0;
    bool flip = false;

    end = randrange(start + 1, end);

    /* Racunamo kakvog ce oblika prepreka biti. */
    if(randf(0, 1) < 0.3)
    {
	/* Zelimo 1 do 4 odsecenih polja. */
	cutoff = 2 + binom(2, 0.5);
	if(randf(0,1) < 0.5)
	{
	    jstart = cutoff;
	    jend = TRK_WIDTH;
	}
	else
	{
	    jend = TRK_WIDTH - cutoff;
	}
    }

    /* Racunamo broj polja koji bi mogli smisleno ipak popuniti. */
    volume = (TRK_WIDTH - cutoff) * (end - start);
    remove = volume / 3;
    
    for(i = start; i < end; i++)
	for(j = 0; j < TRK_WIDTH; j++)
	{
	    /* Omogucavamo da se neka od polja nasumicno popune.*/
	    if(remove >= 0 && randf(0, 1) < 0.66 * ((float)remove / (float)volume))
	    {
		remove--;
		flip = true;
	    }

	    /* Predefinisana sigurna zona. */
	    bool cond = (j >= jstart && j < jend);
	    if((cond && !flip) || (flip && !cond))
		set_field(seg, i, j);
	    
	    flip = false;
	}

    return end - start;
}

void init_seg(segment_t* seg)
/* Inicijalizuje segment trake po kojem ce igrac ici. */
{
    int i, j;
    enum patch_type last_patch = SAFE;
    
    empty_seg(seg);

    for(i = 0; i < SEG_LENGTH;)
    {
	/* Postavljamo sigurnu platformu na pocetku i kraju segmenta, duzu ako je pocetni segment. */
	if(i < (level_count == 0 ? 6 : 3) || i >= SEG_LENGTH-2)
	{
	    for(j = 0; j < TRK_WIDTH ; j++)
		set_field(seg, i, j);

	    i++;
	    continue;
	}
	
	int end, len = SEG_LENGTH - 2 - i;
	switch(last_patch)
	{
	case SEMI:
	case SAFE:
	    /* Postavljamo prepreku. */
	    end = (len >= 3) ? i + 3 : SEG_LENGTH-2;
	    i+= draw_obstacle(seg, i, end);
	    last_patch = EASY;
	    break;
	case EASY:
	    /* Postavljamo neki od sigurnih elemenata. */
	    len = SEG_LENGTH - 2 - i;
	    if(len >= 5 && randf(0, 1) < 0.76)
	    {
		end = (len >= 7) ? i + 7 : SEG_LENGTH-2;
		i+= draw_safe(seg, i, end);
		last_patch = SAFE;
		break;
	    }
	    else if(len >= 2)
	    {
		len = SEG_LENGTH - 2 - i;
		end = (len >= 4) ? i + 4 : SEG_LENGTH-2;
		i+= draw_semi_safe(seg, i, end);
		last_patch = SEMI;
		break;
	    }
	    /* https://developers.redhat.com/blog/2017/03/10/wimplicit-fallthrough-in-gcc-7/ */
	    __attribute__((fallthrough));
	default:
	    /* Ako ne mozemo da nacrtamo zeljeni element crtamo sigurnu platformu. */
	    for(j = 0; j < TRK_WIDTH; j++)
		set_field(seg, i, j);
	    i++;
	    break;
	}
    }
    
    /* Postavljanje novcica na segmentu */
    int total = 42 + binom(21, 0.7);
    int fail_limit = COIN_COUNT, fail = 0;
    bool failed;
    
    for(i = 0; i < total; i++)
    {
	GLfloat x, z;
	
	x = randf(-.5, 4.5);
	/* Pocetni i krajni delovi segmenata su rezervisani za sigurnu platformu za bonus. */
	z = randf(3.5, (GLfloat)SEG_LENGTH - 1.5);

	/* Proveravamo da je novcic na stazi. */
	failed = false;
	if(seg->track[(int)floorf(z + .5)][(int)floorf(x + .5)] == FLD_X)
	    failed = true;

	/* Proveravamo da li se novcic sudara sa nekim od prethodnih. */
	for(j = 0; !failed && j < i; j++)
	{
	    GLfloat dx, dz;
	    /* Ostavljamo dodatni prostor potcenjujuci blizinu novcica. */
	    dx = (x - seg->coins[j].x);
	    dz = (z - seg->coins[j].z);

	    /* FIX: coin_radius je u stvari precnik. */
	    /* Ako su centri ogranicavajucih sfera na manjoj daljini od zbira
	     * precnika onda se seku. */
	    if(SQUARE(dx) + SQUARE(dz) < SQUARE(coin_radius))
		failed = true;
	}

	if(failed)
	{
	    fail++;
	    i--;

	    if(fail == fail_limit)
	    {
		/* Ogranicavamo broj ponavljanja petlje. */
		printf("fail\n");
		break;
	    }
	    else
		continue;
	}
	else
	    fail = 0;
	
	seg->coins[i].x = x;
	seg->coins[i].z = z;
	seg->coins[i].dying = false;
	seg->coins[i].type = rand_coin();
	seg->coins[i].rot_mod = randrange(0, TICK_RATE * COIN_BEAT);
    }
    seg->len_coins = i;

    /* Postavljanje bonus nagrade: */
    if(level_count == 0)
    {
	seg->bonus.type = BONUS_NIL;
	return;
    }
    if(level_count < 3)
    	seg->bonus.type = BONUS_CU;
    else if(level_count < 7)
	seg->bonus.type = BONUS_AG;
    else
	seg->bonus.type = BONUS_AU;

    /* Pocetni deo staze nema drugih novcica i nema rupa, ne radimo provere. */
    seg->bonus.x = randf(-.5, 4.5);
    seg->bonus.z = randf(-.5, 2.5);
    seg->bonus.dying = false;
}

void set_seg_ptrs()
{
    prev_seg = &seg_pool[(seg_ind) % 3];
    curr_seg = &seg_pool[(seg_ind + 1) % 3];
    next_seg = &seg_pool[(seg_ind + 2) % 3];
}

void init_track()
{
    /* Postavljamo seme za generator pseudo-slucajnih brojeva, koji se koristi
     * za generaciju staze igre. */
    srand(time(NULL));
    level_count = 0;

    /* Na pocetku igre je iza avatara prazan segment. */
    empty_seg(&seg_pool[0]);
    init_seg(&seg_pool[1]);

    /* Generisemo nivo na kome je igrac napredovao. */
    level_count++;
    init_seg(&seg_pool[2]);

    /* Postavljamo pokazivace segmenata od prvog indeksa niza. */
    seg_ind = 0;
    set_seg_ptrs();
}

void cycle_track()
/* Menja trenutni segment, generise novi. */
{
    /* Belezimo da je igrac zavrsio jedan segment. */
    level_count++;

    /* Rotiramo indeks ciklicnog niza. */
    seg_ind = (seg_ind + 1) % 3;
    set_seg_ptrs();

    /* Inicijalizujemo sledeci segment, zanemarujuci trenutnu vrednost. */
    init_seg(next_seg);
}
