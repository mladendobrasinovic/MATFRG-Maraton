#include "maraton.h"

void init_seg(segment_t seg)
{
    int i, j;

    for(i = 0; i < SEG_LENGTH; i++)
    {
	    for(j = 0; j < TRK_WIDTH ; j++)
	    {
		if(i != 0 || j % 2 != 0) 
		    seg[i][j] = FLD_O;
		else
		    seg[i][j] = FLD_X;
	    }
    }
}

void empty_seg(segment_t seg)
/* Postavi sva polja u seg na vrednost FLD_X */
{
    /* ???: Pretpostavljamo da je polje matrice INT */
    memset(seg, FLD_X, sizeof(segment_t));
}
