#include "maraton.h"

void empty_seg(segment_t seg)
/* Postavi sva polja u seg na vrednost FLD_X */
{
    /* ???: Pretpostavljamo da je polje matrice INT */
    memset(seg, FLD_X, sizeof(segment_t));
}


void init_seg(segment_t seg)
{
    int i, j;

    for(i = 0; i < SEG_LENGTH; i++)
    {
	for(j = 0; j < TRK_WIDTH ; j++)
	{
	    /* TEST: */
	    if((i % 13 == 6 && j < 3) ||(i % 13 == 12 && j > 1))
	    {
		seg[i][j] = FLD_X;
		continue;
	    }
	    
	    /* XOR bitova na polju jedinice (parnosti) predstavlja
	     * funkciju boje sahovnice */
	    if((j % 2 == 1) ^ (i % 2 == 0))
		seg[i][j] = FLD_W;
	    else
		seg[i][j] = FLD_G;
	}
    }
}

