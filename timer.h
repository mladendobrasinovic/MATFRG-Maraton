#ifndef _TIMER
#define TIMER_ID 13		/* Identifikuje glavnu stopericu funkciji timer()  */
#define TIMER_INTERVAL 32	/* Vreme u milisekundama izmedju otkucaja */

void timer(int);

/* Oznacava stanje svih naredbi koji nas zanimaju, naredbe salje korisnik (pritiskom tastera na tastaturi) */
struct tic_key
{
    int fwd;
    int jump;
    int duck;
    int quit;
};
#endif
