#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "maraton.h"

void display(void);
void keyboard(unsigned char, int, int);

void display(void)
{
    /* Inicijalizuj _buffer_ na cistu pozadinu */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    float near = 0.0, far = 1000;
    float clip = 3.14;
    float dist = 3.0;

    /* Postavi matricu projekcije, koristimo ortogonalnu (paralelnu) projekciju */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-clip, clip, -clip, clip, near, far);

    /* Postavi matricu pogleda i modela, koje zajedno transformisu svet u nas
     * koordinatni sistem, u odnosu na kameru */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(dist, dist, dist,
    	      0, 0, 0,
    	      0, 1, 0);

    /* TEST: */
    glutSolidCube(1.00);
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    UNUSED_2(x, y);
    switch(key)
    {
    case '':
	exit(EXIT_SUCCESS);
	/* Doziveli smo da korisnik prekine program! */
	break;
    default:
	break;
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    glutCreateWindow("Maraton");
    glutReshapeWindow(640, 480);

    /*
     * Registruj _callback_ funkcije, one ce biti pozvane u slucaju dogadjaja
     * koji nas zanimaju.
     * */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glClearColor(0.13, 0.13, 0.13, 0);
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    return EXIT_FAILURE;
}
