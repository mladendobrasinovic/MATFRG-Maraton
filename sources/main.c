#include "maraton.h"

void keyboard(unsigned char, int, int);
void special(int, int, int);
void reshape(int, int);
void display(void);

/* Obrada dogadjaja ulaza, beleze se sve komande igre za trenutni otkucaj
 * stoperice */
void keyboard(unsigned char key, int x, int y)
{
    UNUSED_2(x, y);

    switch(key)
    {
    case 'Q':
    case 'q':
    case '':
    case '':
	curr_tick.exit = true;
	break;
    case ' ':
	curr_tick.jump = true;
	break;
    default:
	break;
    }
}

void special(int key, int x, int y)
/* Obrada dogadjaja tastature izvan ASCII tabele */
{
    UNUSED_2(x, y);
    
    switch(key)
    {
    case GLUT_KEY_LEFT:
	curr_tick.left = true;
	break;
    case GLUT_KEY_RIGHT:
	curr_tick.right = true;
	break;
    case GLUT_KEY_F11:
	glutFullScreenToggle();
	break;
    default:
	break;
    }
}

void reshape(int width, int height)
/* Funkcija se poziva pri promeni i _stvaranju_ prozora */
{
    GLdouble near = -16, far = 16;
    GLdouble clip = M_PI * 2;
    GLdouble ah, aw;

    /* Azuriraj OpenGL pogled, inace ne bi odgovarao prozoru */
    glViewport(0, 0, width, height);
    
    /* Izracunaj novi aspekat prozora */
    aw = (float)height / width;
    ah = 1.0;

    /* Postavi matricu projekcije, koristimo ortogonalnu (paralelnu)
     * projekciju */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-clip * ah, clip * ah,
	    -clip * aw, clip * aw,
	    near, far);
    
    glutPostRedisplay();
}

void display(void)
{
    /* Inicijalizuj _buffer_ na cistu pozadinu */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    /* Iscrtaj dinamicki deo scene */
    glMatrixMode(GL_MODELVIEW);
    draw_scene();

    display_running();
        
    /* Koristimo dvostruko baferisanje, zameni aktivni bafer i onaj u kome smo
     * renderovali scenu */
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    /* Inicijalizuj podrazumevanu rezoluciju prozora (pre glutInit, tako
     * omogucavamo korisniku da argumentima komandne linije prosledi GLUT-u
     * zeljenu rezoluciju) */
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    glutCreateWindow("Maraton");

    /* Registruj _callback_ funkcije, one ce biti pozvane u slucaju dogadjaja
     * koji nas zanimaju */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    glutReshapeFunc(reshape);

    /* Postavi boju pozadine, promeni podesavanja OpenGL-a */
    glClearColor(0.196, 0.084 , 0.182, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    /* Postavi staticki deo scene */
    set_scene();

    /* Inicijalizuj globalno stanje */
    init_state();

    glutMainLoop();
    return EXIT_FAILURE;
}
