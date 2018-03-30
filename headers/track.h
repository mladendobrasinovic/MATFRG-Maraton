#ifndef _TRACK
#define _TRACK

#define TRK_WIDTH 5
#define SEG_LENGTH 256

enum field
{
    FLD_X = 0, FLD_W, FLD_G
};

typedef enum field segment_t[SEG_LENGTH][TRK_WIDTH];

void empty_seg(segment_t);
void init_seg(segment_t);

#endif
