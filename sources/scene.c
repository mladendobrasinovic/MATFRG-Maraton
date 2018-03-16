#include "maraton.h"

void draw_scene()
{
    /* TEST: */
    GLfloat material_diff[] = {1, 1, 1, 1};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diff);
    
    glShadeModel(GL_FLAT);
    glutSolidCube(1.00);
}

void set_scene()
{
    GLfloat light_pos[] = {-1, 2, 2, 1}; /* Pozicija ne predstavlja direkciono
					  * svetlo (w = 1) */
    GLfloat light_diff[] = {0.7, 0.7, 0.8, 1};
    GLfloat light_ambi[] = {0.1, 0.1, 0.1, 1};

    /* Postavi staticko svetlo */
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambi);
    glEnable(GL_LIGHT0);
}
