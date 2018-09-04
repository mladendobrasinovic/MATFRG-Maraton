#include "maraton.h"

int seg_ind;
segment_t seg_pool[SEG_COUNT];
int level_count = 0;

GLfloat randf(GLfloat start, GLfloat end)
/* Generise float iz intervala. */
{
    return start + (GLfloat)((GLdouble)rand() / RAND_MAX) * (end - start);
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

void empty_seg(segment_t *seg)
/* Postavlja sva polja u seg na vrednost FLD_X. */
{
    /* Poznato je da je FLD_X == 0, postavljamo prazno polje. */
    memset(seg->track, FLD_X, sizeof(enum field_type) * SEG_LENGTH * TRK_WIDTH);
    
    seg->len_coins = 0;
}

void init_seg(segment_t *seg)
/* Inicijalizuje segment trake po kojem ce igrac ici. */
{
    int i, j;
    
    empty_seg(seg);

    for(i = 0; i < SEG_LENGTH; i++)
	for(j = 0; j < TRK_WIDTH ; j++)
	{
	    /* TEST:  */
	    if(((i % 13 == 6 || i % 13 == 5) && j < 3) || (i % 13 == 12 && j > 1))
		continue;
	    
	    set_field(seg, i, j);
	}

    
    /* Postavljanje novcica na segmentu */
    int total = 60 + binom(34, 0.7);
    int fail_limit = COIN_COUNT, fail = 0;
    bool failed;
    
    for(i = 0; i < total; i++)
    {
	GLfloat x, z;
	
	x = randf(-.5, 4.5);
	/* Pocetni delovi segmenata su rezervisani.. */
	z = randf(4.0, .5 + (GLfloat)SEG_LENGTH);

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
    }
    seg->len_coins = i;
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
