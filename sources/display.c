#include "maraton.h"

#define SCORE_FONT GLUT_BITMAP_9_BY_15

void display_running()
{
    char score_buff[CHAR_BUFF_SIZE];
    GLint w_offset, h_offset;

    /* Privremeno onesposobi osvetljenje i proveru nad z-bufferom, zelimo da
     * crtamo samo sa bojama */
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    /* Racunaj poziciju pri gornjem levom cosku prozora */
    w_offset = 5;
    h_offset = glutGet(GLUT_WINDOW_HEIGHT) - 2 - glutBitmapHeight(SCORE_FONT);
    if (game_starting)
	snprintf(score_buff, CHAR_BUFF_SIZE,
		 "Pritisnite bilo koje slovo da biste poceli:\n"
		 "-------------------------------------------\n"
		 "Q ili <ESC> -- izadji iz igre.\n"
		 "P           -- pauziraj igru.\n"
		 "<F11>       -- rezim celog ekrana/rezim prozora.\n"
		 "-------------------------------------------\n"
		 "<razmak>          -- skoci!\n"
		 "<leva-strelica>   -- idi levo!\n"
    		 "<desna-strelica>  -- idi desno!\n"
		 "-------------------------------------------\n"
		 "Skupite sto vise bodova i nemojte da padnete.");
    else if(game_running)
	snprintf(score_buff, CHAR_BUFF_SIZE, "Predjena razdaljina: %lldm\n"
		 "Trenutno bodova: %lld", distance_score, score);
    else if(game_paused)
	snprintf(score_buff, CHAR_BUFF_SIZE, "Predjena razdaljina: %lldm\n"
		 "Trenutno bodova: %lld -- Pauzirano", distance_score, score);
    else
	snprintf(score_buff, CHAR_BUFF_SIZE, "Predjena razdaljina: %lldm\n"
		 "Trenutno bodova: %lld -- Kraj\n"
		 "Pritisnite <R> da biste pokusali ponovo.", distance_score, score);
    
    glColor4f(.5, .9, .15, 0);
    /* Direktno odredjujemo poziciju iscrtavanja */
    glWindowPos2i(w_offset, h_offset);
    glutBitmapString(SCORE_FONT, (const unsigned char *)score_buff);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
