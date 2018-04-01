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

    if(game_running)
	snprintf(score_buff, CHAR_BUFF_SIZE, "Trenutni bodovi: %lld", score);
    else
	snprintf(score_buff, CHAR_BUFF_SIZE, "Trenutni bodovi: %lld -- Kraj igre", score);
    
    glColor4f(.6, .9, .2, 0);
    /* Direktno odredjujemo poziciju iscrtavanja */
    glWindowPos2i(w_offset, h_offset);
    glutBitmapString(SCORE_FONT, (const unsigned char *)score_buff);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
