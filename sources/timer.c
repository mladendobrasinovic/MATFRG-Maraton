#include "maraton.h"

struct tick_key null_tick = {.left = false, .right = false,
			     .jump = false, .exit = false};
anim_t animate = NULL;

void avatar_update()
{
    avatar_z_shift += tick_scale(5.0);

    /* Ako je animacija u toku, izvrsi je, i prekini ako je potrebno */
    if(animate != NULL && animate())
	animate = NULL;
}


void timer(int timer_val)
{
    UNUSED(timer_val);
    struct tick_key tick;
    
    /* Stanje komandi kopiraj u lokalnu strukturu, nije neophodno da
     * zakljucavamo promenljive */
    tick = curr_tick;
    
    if(tick.exit)		/* Doziveli smo da korisnik prekine program! */
	exit(EXIT_SUCCESS);

    if(game_running && animate == NULL)
    {
	/* Ako animacija nije u toku */

	if(tick.jump)
	    printf("HOP!\n");
	else if(tick.left)
	{
	    animate = start_side_move(MOVE_LEFT);
	    printf("LEVO!\n");
	}
	if(tick.right)
	{
	    animate = start_side_move(MOVE_RIGHT);
	    printf("DESNO!\n");
	}
    }
    
    if(game_running && !collide_track())
    {
	printf("GAME OVER: [%d, %d]\n",
	       (int)roundf(avatar_x_shift),
	       (int)roundf(avatar_z_shift));
	animate = start_drop();
	game_running = false;
    }

    avatar_update();
    
    /* Postavi null_tick za sledeci otkucaja */
    curr_tick = null_tick;

    /* Ponovo postavljamo _callback_ za stopericu */
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    /* GLUT-u se mora reci da je doslo do promene stanja igre */
    glutPostRedisplay();
}
