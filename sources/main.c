#include "maraton.h"

int height, width;

void display(void);
void keyboard(unsigned char, int, int);
void special(int, int, int);
void reshape(int, int);

void display(void)
{
    float dist = 3.0;

    /* Inicijalizuj _buffer_ na cistu pozadinu */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    /* Postavi matricu pogleda i modela, koje zajedno transformisu svet u nas
     * koordinatni sistem, u odnosu na kameru */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(dist, dist, dist,	/* Lokacija kamere, x == y == z */
    	      0, 0, 0,		/* Tacka ka kojoj je kamera okrenuta */
    	      0, 1, 0);		/* Y osu gledamo kao na uspravnu */

    /* Iscrtaj dinamicki deo scene */
    draw_scene();
    
    glutSwapBuffers();
}

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
/* Obrada dogadjaja tastature izvan ASCII tabele*/
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

void reshape(int new_width, int new_height)
{
    float near = 0.0, far = 1000;
    double clip = M_PI * 2;
    double ah, aw;
    
    /* Funkcija se poziva pri promeni i _stvaranju_ prozora */
    width = new_width;
    height = new_height;

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
     * koji nas zanimaju. */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    glutReshapeFunc(reshape);

    /* glClearColor(1.0, 0.858823529412, 0.34509803922, 0); */
    glClearColor(0.11, 0.11, 0.13, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    /* Postavi staticki deo scene */
    init_state();
    set_scene();

    glutMainLoop();
    return EXIT_FAILURE;
}
