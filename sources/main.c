#include "maraton.h"

void keyboard(unsigned char, int, int);
void special(int, int, int);
void reshape(int, int);
void display(void);

void keyboard(unsigned char key, int x, int y)
/* Obrada dogadjaja ulaza, beleze se sve komande igre za trenutni otkucaj
 * stoperice. */
{
    UNUSED_2(x, y);

    /* U pocetnom stanju: */
    if(game_starting)
    {
	game_starting = false;
	game_running = true;

	/* Dozvoljavamo izlaz za tasterom <ESC>. */
	if(key == '')
	    curr_tick.exit = true;
	return;
    }

    /* U pauziranom stanju: */
    if(game_paused)
    {
	switch(key)
	{
	case 'Q':
	case 'q':
	case '':
	case '':
	    curr_tick.exit = true;
	    break;
	case 'P':
	case 'p':
	case '':
	    game_paused = false;
	    game_running = true;
	    break;
	default:
	    break;
	}
	return;
    }

    /* U stanju igranja: */
    if (game_running)
	switch(key)
	{
	case 'Q':
	case 'q':
	case '':
	case '':
	    curr_tick.exit = true;
	    break;
	case 'P':
	case 'p':
	case '':
	    game_paused = true;
	    game_running = false;
	    break;
	case ' ':
	    curr_tick.jump = true;
	    break;
	default:
	    break;
	}

    /* Konacno, u zavrsnom stanju: */
    if (!game_running && !game_starting && !game_paused)
	switch(key)
	{
	case 'Q':
	case 'q':
	case '':
	case '':
	    curr_tick.exit = true;
	    break;
	case 'r':
	case 'R':
	case '':
	    init_state();
	    break;
	}
}

void special(int key, int x, int y)
/* Obrada dogadjaja tastature izvan ASCII tabele. */
{
    UNUSED_2(x, y);
    
    /* Ova provera se vrsi samo u stanju igre. */
    if (!game_starting && !game_paused)
	switch(key)
	{
	case GLUT_KEY_LEFT:
	    curr_tick.left = true;
	    break;
	case GLUT_KEY_RIGHT:
	    curr_tick.right = true;
	    break;
	default:
	    break;
	}
    
    if (key == GLUT_KEY_F11)
	glutFullScreenToggle();
}

void reshape(int width, int height)
/* Funkcija se poziva pri promeni i _stvaranju_ prozora. */
{
    GLdouble near = -16, far = 16;
    GLdouble clip = M_PI * 2;
    GLdouble ah, aw;

    /* Azuriramo OpenGL pogled, inace ne bi odgovarao prozoru. */
    glViewport(0, 0, width, height);
    
    /* Racunamo novi aspekat pogleda. */
    aw = (float)height / width;
    ah = 1.0;

    /* Postavljamo matricu projekcije, koristimo ortogonalnu (paralelnu)
     * projekciju. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-clip * ah, clip * ah,
	    -clip * aw, clip * aw,
	    near, far);
    
    glutPostRedisplay();
}

void display(void)
{
    /* Inicijalizujemo _buffer_ na cistu pozadinu. */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    /* Iscrtavamo dinamicki deo scene. */
    glMatrixMode(GL_MODELVIEW);
    draw_scene();

    /* Iscrtavamo korisnicki displej. FIX: lose ime funkcije */
    display_running();
        
    /* Posto koristimo dvostruko baferovanje, zahtevamo glut-u da zameni aktivni
     * bafer i onaj u kome smo iscrtali scenu. */
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    /* Inicijalizujemo podrazumevanu rezoluciju prozora (pre glutInit, tako
     * omogucavamo korisniku da argumentima komandne linije prosledi GLUT-u
     * zeljenu rezoluciju). */
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    
    glutSetOption(GLUT_MULTISAMPLE, 16);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    glutCreateWindow("Maraton");

    /* Registrujemo _callback_ funkcije -- one ce biti pozvane u slucaju
     * dogadjaja koji nas zanimaju. */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    glutReshapeFunc(reshape);

    /* Postavljamo boju pozadine i menjamo podesavanja OpenGL-a. */
    glClearColor(0.182, 0.072 , 0.169, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    /* Postavljamo staticki deo scene. */
    set_scene();

    /* Inicijalizujemo globalno stanje igre. */
    init_state();

    glutMainLoop();
}
