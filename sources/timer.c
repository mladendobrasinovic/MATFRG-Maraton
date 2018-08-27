#include "maraton.h"

struct tick_key null_tick = {.left = false, .right = false,
			     .jump = false, .exit = false};

void sync_avatar_track()
{
    GLfloat seg_length = (GLfloat)SEG_LENGTH;
    
    if(avatar.z >= seg_length)
    {
	/* Rotiramo ciklicni niz staze, menjamo koordinate avatara. */
	avatar.z = avatar.z - seg_length;
	cycle_track();
    }
}

void update_time_score()
{
    if(++score_timer == TICK_RATE / 4)
	/* Otkucalo je cetvrt sekunde, manje-vise. */
    {
	score++;
	score_timer = 0;
    }
}

void update_animation_timers()
{
    /* Postavljamo tajmere tako da se posle jednog trajanja vratimo na
     * pocetak. */
    coin_timer = (coin_timer + 1) %
	(int)(TICK_RATE * COIN_BEAT);
}

void timer(int timer_val)
{
    UNUSED(timer_val);
    struct tick_key tick;
    
    /* Stanje komandi kopiraj u lokalnu strukturu, nije neophodno da
     * zakljucavamo promenljive. */
    tick = curr_tick;
    
    if(tick.exit)		/* Doziveli smo da korisnik prekine program! */
	exit(EXIT_SUCCESS);

    /* Obrada ulaza */
    if(game_running && avatar.anim == NULL)
    {
	/* Ako animacija nije u toku. */
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

    /* Napredujemo poziciju avatara kroz stazu. */
    avatar_update();
    sync_avatar_track();

    /* Dodajemo igracu bodove za proteklo vreme. */
    if(game_running)
	update_time_score();
    
    /* Postavi null_tick za sledeci otkucaj. */
    curr_tick = null_tick;

    /* Napredujemo tajmere animacija. */
    update_animation_timers();
    
    /* Ponovo postavljamo _callback_ funkciju za stopericu. */
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    /* GLUT-u se mora reci da je doslo do promene stanja igre. */
    glutPostRedisplay();
}
