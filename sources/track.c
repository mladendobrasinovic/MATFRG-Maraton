#include "maraton.h"

int seg_ind;
segment_t seg_pool[SEG_COUNT];

void set_field(segment_t seg, int i, int j)
{
    /* XOR bitova na polju jedinice (parnosti) predstavlja
     * funkciju boje sahovnice */
    if((j % 2 == 1) ^ (i % 2 == 0))
	seg[i][j] = FLD_W;
    else
	seg[i][j] = FLD_G;
}

void empty_seg(segment_t seg)
/* Postavlja sva polja u seg na vrednost FLD_X */
{
    /* Poznato je da je FLD_X == 0 */
    memset(seg, FLD_X, sizeof(segment_t));
}

void init_seg(segment_t seg)
/* Inicijalizuje segment trake po kome se ce igrac ici */
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
}

void set_segs()
{
    prev_seg = &seg_pool[(seg_ind) % 3];
    curr_seg = &seg_pool[(seg_ind + 1) % 3];
    next_seg = &seg_pool[(seg_ind + 2) % 3];
}

void init_track()
{
    /* Na pocetku igre je iza nas prazan segment */
    empty_seg(seg_pool[0]);
    init_seg(seg_pool[1]);
    init_seg(seg_pool[2]);

    /* Postavi pokazivace od prvog indeksa */
    seg_ind = 0;
    set_segs();
}

void cycle_track()
/* Menja trenutni segment, generise novi */
{
    /* Rotiraj indeks ciklicnog niza */
    seg_ind = (seg_ind + 1) % 3;
    set_segs();

    /* Inicijalizuj sledeci segment, zanemarujuci trenutnu vrednost */
    init_seg(*next_seg);
}
