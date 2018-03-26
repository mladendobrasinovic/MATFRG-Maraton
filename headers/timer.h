#ifndef _TIMER
#define _TIMER

#include <stdbool.h>

#define TIMER_ID 13		/* Identifikuje glavnu stopericu funkciji
				 * timer() */
#define SEC_INTERVAL 1000
#define TICK_RATE 50
/* Vreme u milisekundama izmedju otkucaja (1000/50 = 20) */
#define TIMER_INTERVAL (SEC_INTERVAL / TICK_RATE)


void timer(int);

/* Oznacava stanje svih naredbi koji nas zanimaju, naredbe salje korisnik
 * (pritiskom tastera na tastaturi) */
struct tick_key
{
    /* Nema potrebe da brojimo svaki pritisak, dovoljno je da proverimo da li ga
     * je bilo */
    bool left;
    bool right;
    bool jump;
    bool exit;
};

extern struct tick_key null_tick;

#endif
