#ifndef _TRACK
#define _TRACK

#define TRK_WIDTH 5
#define SEG_LENGTH 256

enum field
{
    FLD_O, FLD_X
};

typedef enum field segment_t[SEG_LENGTH][TRK_WIDTH];

void init_seg(segment_t);
void empty_seg(segment_t);

#endif