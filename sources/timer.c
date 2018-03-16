#include "maraton.h"

struct tick_key null_tick = {.left = false, .right = false,
			     .jump = false, .exit = false};

void timer(int timer_val)
{
    UNUSED(timer_val);
    struct tick_key tick;
    
    /* Stanje komandi kopiraj u lokalnu strukturu, nije neophodno da
     * zakljucavamo promenljive */
    tick = curr_tick;
    
    if(tick.exit)		/* Doziveli smo da korisnik prekine program! */
	exit(EXIT_SUCCESS);
    
    if(tick.jump) printf("HOP!\n");
    if(tick.left) printf("LEVO!\n");
    if(tick.right) printf("DESNO!\n");

    /* Postavi null_tick za sledeci otkucaja */
    curr_tick = null_tick;

    /* Ponovo postavljamo _callback_ za stopericu */
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
}
