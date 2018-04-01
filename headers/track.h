#ifndef _TRACK
#define _TRACK

#define TRK_WIDTH 5
#define SEG_LENGTH 256
#define SEG_COUNT 3

enum field
{
    FLD_X = 0, FLD_W, FLD_G
};

enum
{
    OFFSET_PREV = -1,
    OFFSET_CURR,
    OFFSET_NEXT
};

typedef enum field segment_t[SEG_LENGTH][TRK_WIDTH];

void init_track();
void cycle_track();

#endif
