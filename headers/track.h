#ifndef _TRACK
#define _TRACK

#define TRK_WIDTH 5
#define SEG_LENGTH 144
#define SEG_COUNT 3

#define COIN_COUNT 256

enum field_type
{
    FLD_X = 0, FLD_W, FLD_G
};

enum coin_type
{
    /* Avanturisticke boje */
    COIN_YEL, COIN_RED, COIN_BLU, COIN_NIL
};

enum bonus_type
{
    BONUS_CU, BONUS_AG, BONUS_AU, BONUS_NIL
};

typedef struct
{
    GLfloat x, z;
    enum coin_type type;
    int death_mod;
    int rot_mod;
    bool dying;
} coin_t;

typedef struct
{
    GLfloat x, z;
    enum bonus_type type;
    int death_mod;
    bool dying;
} bonus_t;

typedef struct
{
    int len_coins;
    bonus_t bonus;
    coin_t coins[COIN_COUNT];
    enum field_type track[SEG_LENGTH][TRK_WIDTH];
} segment_t;

void init_track();
void cycle_track();

#endif
