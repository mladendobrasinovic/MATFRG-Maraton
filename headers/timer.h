#ifndef _TIMER
#define _TIMER

#include <stdbool.h>

#define TIMER_ID 13		/* Identifikuje glavnu stopericu funkciji
				 * timer() */
#define TIMER_INTERVAL 32	/* Vreme u milisekundama izmedju otkucaja,
				 * ekvivalentno 1000/25 = 31.25 */

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
