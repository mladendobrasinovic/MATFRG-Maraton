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

    /* Obrada ulaza */
    if(game_running && avatar.anim == NULL)
    {
	/* Ako animacija nije u toku */
	if(tick.jump)
	{
	    printf("HOP!\n");
	    avatar.anim = start_jump();
	}
	else if(tick.left)
	{
	    avatar.anim = start_side_move(MOVE_LEFT);
	    printf("LEVO!\n");
	}
	if(tick.right)
	{
	    avatar.anim = start_side_move(MOVE_RIGHT);
	    printf("DESNO!\n");
	}
    }

    avatar_update();
    
    /* Postavi null_tick za sledeci otkucaja */
    curr_tick = null_tick;

    /* Ponovo postavljamo _callback_ za stopericu */
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    /* GLUT-u se mora reci da je doslo do promene stanja igre */
    glutPostRedisplay();
}
